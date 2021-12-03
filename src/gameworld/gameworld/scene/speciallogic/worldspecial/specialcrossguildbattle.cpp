#include "specialcrossguildbattle.hpp"
#include <algorithm>
#include "obj/character/role.h"
#include "protocal/msgactivity.hpp"
#include "protocal/msgfight.h"
#include "servercommon/internalprotocal/crossgameprotocal.h"
#include "engineadapter.h"
#include "servercommon/errornum.h"
#include "servercommon/string/gameworldstr.h"
#include "other/route/mailroute.hpp"
#include "internalcomm.h"
#include "gamelog.h"
#include "servercommon/userprotocal/msgcrosscommon.h"
#include "servercommon/userprotocal/msgsystem.h"
#include "obj/character/monster.h"
#include "monster/monsterpool.h"
#include "effect/skilleffect/effectbuff.hpp"
#include "obj/character/attribute.hpp"
#include "effect/effectbase.hpp"
#include <limits>
#include "servercommon/string/crossstr.h"
#include "servercommon/servercommon.h"
#include "world.h"
#include "scene/activityshadow/activityshadow.hpp"
#include "servercommon/serverconfig/crossguildbattleconfig.hpp"
#include "servercommon/serverconfig/crossconfig.hpp"
#include "config/logicconfigmanager.hpp"
#include "globalconfig/globalconfig.h"
#include "gameworld/gameworld/monster/drop/droppool.hpp"
#include "other/event/eventhandler.hpp"
#include "other/cross/rolecross.h"
#include "obj/otherobj/gatherobj.h"
#include "scene/generater/gathergenerater.h"
#include "scene/generater/monstergenerater.h"
#include "global/worldstatus/worldstatus.hpp"
#include "other/shenshou/shenshou.hpp"
#include "gameworld/config/guildconfig.hpp"
#include "other/vip/vip.hpp"
#include "gameworld/config/activitymsgconfig.hpp"
#include "gameworld/global/activity/activitymsgmanager.h"
#include "gameworld/gstr.h"
#include "gameworld/obj/otherobj/fallingitem.h"
#include "gameworld/config/monsterreplaceconfig.hpp"
#include "config/worshipconfig.hpp"


SpecialCrossGuildBattle::SpecialCrossGuildBattle(Scene *scene)
: SpecialLogic(scene), m_first(true), m_activity_open(false), m_had_update(false), m_today_drop_limit_num(0), m_next_sync_scene_info_time(0), 
	m_total_level(0), m_total_num(0),m_boss_hurt_info_time(0), m_worship_start_time(0)
{
	this->SetMonsterGenerater();
}

SpecialCrossGuildBattle::~SpecialCrossGuildBattle()
{

}

void SpecialCrossGuildBattle::Update(unsigned long interval, time_t now_second)
{
	SpecialLogic::Update(interval, now_second);

	this->CheckUpdataBoss(now_second);

	if (!m_had_update)
	{
		m_today_drop_limit_num = CrossGuildBattleConfig::Instance().GetRandDropWingNum();
		m_had_update = true;
	}

	if (!m_activity_open)
	{
		if (now_second >= m_boss_hurt_info_time + 1)
		{
			this->CheckAllBossBound();
		}

		// 活动结束后，才开始计算膜拜
		this->CheckWorshipReward(now_second);

		return;
	}

	if (m_first)
	{
		m_first = false;
		int flag_id_list[CROSS_GUILDBATTLE_MAX_FLAG_IN_SCENE] = {0};
		CrossGuildBattleConfig::Instance().GetFlagIdList(this->GetSceneId(), flag_id_list);
	
		for (int i = 0; i < CROSS_GUILDBATTLE_MAX_FLAG_IN_SCENE; ++ i)
		{
			m_flag_list[i].Reset();
			m_flag_list[i].flag_id = flag_id_list[i];
		}

		this->RefreshCreateFlag();
	}
	
	this->CheckAddGuildScore(static_cast<int>(now_second));
	this->CheckAddRoleScore(static_cast<int>(now_second));
}

void SpecialCrossGuildBattle::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	if (m_total_num > 0)
	{
		WorldStatus::Instance().SetCrossGuildBattleAverageLevel(m_total_level / m_total_num);
		this->SetMonsterGenerater();
	}

	if (!m_activity_open)
	{
		m_total_level = 0;
		m_total_num = 0;
		m_role_map.clear();
	}

	m_today_drop_limit_num = CrossGuildBattleConfig::Instance().GetRandDropWingNum();
}

void SpecialCrossGuildBattle::SetMonsterGenerater()
{
	int cross_guildbattle_average_level = WorldStatus::Instance().GetCrossGuildBattleAverageLevel();

	SceneMonsterLogic::MonsterGeneraterList &monster_list = m_scene->GetMonsterLogic()->GetMonsterGenList();
	for (SceneMonsterLogic::MonsterGeneraterList::iterator monster_it = monster_list.begin(); monster_it != monster_list.end(); monster_it++)
	{
		(*monster_it)->CrossGuildBattleSetAddAttrs(cross_guildbattle_average_level);
	}
}

void SpecialCrossGuildBattle::StopGenerater()
{
	{
		SceneMonsterLogic::GatherGeneraterList &generater_list = m_scene->GetMonsterLogic()->GetGatherGeneraterList();
		for (SceneMonsterLogic::GatherGeneraterList::iterator gather_it = generater_list.begin(); gather_it != generater_list.end(); gather_it++)
		{
			(*gather_it)->SetPause(true);
		}

		int gather_num = (int)m_scene->GatherObjNum();
		for (int i = 0; i < gather_num; i++)
		{
			GatherObj *gather = m_scene->GetGatherObjByIndex(i);
			if (NULL != gather)
			{
				if (NULL != gather->GetGatherGenerater()) gather->GetGatherGenerater()->Gather(NULL);
				m_scene->DeleteObj(gather->GetId());
			}
		}
	}

	{
		SceneMonsterLogic::MonsterGeneraterList &monster_list = m_scene->GetMonsterLogic()->GetMonsterGenList();
		for (SceneMonsterLogic::MonsterGeneraterList::iterator monster_it = monster_list.begin(); monster_it != monster_list.end(); monster_it++)
		{
			(*monster_it)->SetPause(true);
		}
	}
}

void SpecialCrossGuildBattle::StartGenerater()
{
	SceneMonsterLogic::GatherGeneraterList &generater_list = m_scene->GetMonsterLogic()->GetGatherGeneraterList();
	for (SceneMonsterLogic::GatherGeneraterList::iterator gather_it = generater_list.begin(); gather_it != generater_list.end(); gather_it++)
	{
		(*gather_it)->SetPause(false);
	}

	SceneMonsterLogic::MonsterGeneraterList &monster_list = m_scene->GetMonsterLogic()->GetMonsterGenList();
	for (SceneMonsterLogic::MonsterGeneraterList::iterator monster_it = monster_list.begin(); monster_it != monster_list.end(); monster_it++)
	{
		(*monster_it)->SetPause(false);
	}
}

void SpecialCrossGuildBattle::CheckUpdataBoss(time_t now_second)
{
	if (CrossConfig::Instance().IsHiddenServer())
	{
		bool is_sync_boss_info = false;
		unsigned int today_zero_time = static_cast<unsigned int>(GetZeroTime(now_second));
		unsigned int today_sec = static_cast<unsigned int>(now_second) - today_zero_time;
		std::map<int, ShouHuBossInfo>::iterator boss_iter = m_shouhu_boss_map.begin();
		for (; m_shouhu_boss_map.end() != boss_iter; boss_iter++)
		{
			if (static_cast<unsigned int>(now_second) < boss_iter->second.next_refresh_time) continue;

			if (!m_activity_open && SHOUHU_BOSSSTATUS_NONEXIST == boss_iter->second.boss_status)
			{
				this->RefreshShouhuBoss(boss_iter->second);
			}

			is_sync_boss_info = true;

			if (today_sec < static_cast<unsigned int>(boss_iter->second.begin_refresh_time))
			{
				// 第一波的刷新时间戳
				boss_iter->second.next_refresh_time = today_zero_time + boss_iter->second.begin_refresh_time;
			}
			else if (today_sec >= static_cast<unsigned int>(boss_iter->second.begin_refresh_time) && today_sec < static_cast<unsigned int>(boss_iter->second.end_refresh_time))
			{
				if (boss_iter->second.refresh_interval > 0)
				{
					// 第二波的刷新时间戳
					boss_iter->second.next_refresh_time = static_cast<unsigned int>(now_second) + (boss_iter->second.refresh_interval - (today_sec - boss_iter->second.begin_refresh_time) % boss_iter->second.refresh_interval);
				}
			}
			// 3波改为2波
// 			else if (today_sec >= static_cast<unsigned int>(boss_iter->second.end_refresh_time) && today_sec < static_cast<unsigned int>(boss_iter->second.begin_refresh_time1))
// 			{
// 				boss_iter->second.next_refresh_time = today_zero_time + boss_iter->second.begin_refresh_time1;
// 			}
// 			else if (today_sec >= static_cast<unsigned int>(boss_iter->second.begin_refresh_time1) && today_sec < static_cast<unsigned int>(boss_iter->second.end_refresh_time1))
// 			{
// 				if (boss_iter->second.refresh_interval1 > 0)
// 				{
// 					boss_iter->second.next_refresh_time = static_cast<unsigned int>(now_second) + (boss_iter->second.refresh_interval1 - (today_sec - boss_iter->second.begin_refresh_time1) % boss_iter->second.refresh_interval1);
// 				}
// 			}
// 			else if (today_sec >= static_cast<unsigned int>(boss_iter->second.end_refresh_time1))
// 			{
// 				int next_day_refresh_interval = EngineAdapter::Instance().NextDayInterval(0, 0, 0) + boss_iter->second.begin_refresh_time;
// 				boss_iter->second.next_refresh_time = static_cast<unsigned int>(now_second) + next_day_refresh_interval;
// 			}
			else if (today_sec >= static_cast<unsigned int>(boss_iter->second.end_refresh_time))
			{
				// 明天第一波的刷新时间戳
				int next_day_refresh_interval = EngineAdapter::Instance().NextDayInterval(0, 0, 0) + boss_iter->second.begin_refresh_time;
				boss_iter->second.next_refresh_time = static_cast<unsigned int>(now_second) + next_day_refresh_interval;
			}
		}

		if (is_sync_boss_info)
		{
			this->SynBossInfo(NULL, true);
			this->SynBossInfoToOrigin(0, true);
		}
	}
}

void SpecialCrossGuildBattle::RefreshCreateFlag()
{
	for (int i = 0; i < CROSS_GUILDBATTLE_MAX_FLAG_IN_SCENE; ++ i)
	{
		FlagItem &flag_item = m_flag_list[i];
		if (flag_item.flag_id > 0 && INVALID_OBJ_ID == flag_item.obj_id)
		{
			const CrossGuildBattleFlagCfg *flag_cfg = CrossGuildBattleConfig::Instance().GetFlagCfg(this->GetSceneId(), m_flag_list[i].flag_id);
			if (NULL != flag_cfg)
			{
				Monster *monster = MONSTERPOOL->CreateMonster(flag_cfg->flag_id, m_scene, flag_cfg->flush_pos);
				if (NULL != monster)
				{
					flag_item.obj_id = monster->GetId();
					flag_item.has_be_hit_broadcast_flag = 0;
					flag_item.low_hp_broadcast_flag = 0;
				}
			}
		}
	}
}

void SpecialCrossGuildBattle::RefreshShouhuBoss(ShouHuBossInfo &boss_info)
{
	if (boss_info.boss_id < 0 || !CrossConfig::Instance().IsHiddenServer()) return;
	
	boss_info.replace_boss_id = LOGIC_CONFIG->GetReMonsterCfg().GetReMonsterIdByCrossBossLevel(boss_info.boss_id);
	const MonsterInitParam *monster_param = MONSTERPOOL->GetMonsterParam(boss_info.replace_boss_id);
	if (NULL != monster_param)
	{
		//int level = WorldStatus::Instance().GetCrossGuildBattleAverageLevel();
		//int add_maxhp = (int)(monster_param->hp * (0.00009f * level * level - 0.06f * level + 9.975f));
		Monster *new_monster = MONSTERPOOL->CreateMonster(boss_info.replace_boss_id, m_scene, boss_info.born_pos);
		if (NULL != new_monster)
		{
			boss_info.boss_status = SHOUHU_BOSSSTATUS_EXIST;
			boss_info.guild_uuid = INVALID_UNIQUE_USER_ID;
			boss_info.guild_hurt_map.clear();
			//printf("boss:%d,replace:%d\n", boss_info.boss_id, boss_info.replace_boss_id);
		}
	}
}

void SpecialCrossGuildBattle::ChangeAllBossStatus(int status)
{
	std::map<int, ShouHuBossInfo>::iterator iter = m_shouhu_boss_map.begin();
	for (; iter != m_shouhu_boss_map.end(); iter++)
	{
		iter->second.boss_status = status;
	}
}

void SpecialCrossGuildBattle::CheckAddGuildScore(int now_time)
{
	for (int i = 0; i < CROSS_GUILDBATTLE_MAX_FLAG_IN_SCENE; ++ i)
	{
		FlagItem &flag_item = m_flag_list[i];

		if (flag_item.flag_id > 0 && INVALID_UNIQUE_USER_ID != flag_item.guild_id
			&& now_time >= flag_item.next_add_score_time)
		{
			GuildItem *guild_item = this->GetGuildItem(flag_item.guild_id);
			const CrossGuildBattleFlagCfg *flag_cfg = CrossGuildBattleConfig::Instance().GetFlagCfg(this->GetSceneId(), flag_item.flag_id);
			
			if (NULL != guild_item && NULL != flag_cfg)
			{
				flag_item.next_add_score_time = now_time + flag_cfg->add_score_interval;
				guild_item->score += flag_cfg->add_score_once;
			}
		}
	}

	if (now_time >= m_next_sync_scene_info_time)
	{
		m_next_sync_scene_info_time = now_time + 2;
		this->SyncSceneInfo(NULL);
	}
}

void SpecialCrossGuildBattle::CheckAddRoleScore(int now_time)
{
	const CrossGuildBattleOtherCfg &other_cfg = CrossGuildBattleConfig::Instance().GetOtherCfg();
	if (other_cfg.add_score_interval <= 0)
	{
		return;
	}

	const int role_num = (int)m_scene->RoleNum();
	for (int i = 0; i < role_num; ++ i)
	{
		Role *temp_role = m_scene->GetRoleByIndex(i);
		if (NULL == temp_role)
		{
			continue;
		}

		SpecialCrossGuildBattle::UserItem *user_item = this->GetUserItem(temp_role);
		if (NULL == user_item)
		{
			continue;
		}

 		if (0 == user_item->next_add_score_time)
 		{
 			user_item->next_add_score_time = now_time + other_cfg.add_score_interval;
 			continue;
 		}

 		if (now_time >= user_item->next_add_score_time)
 		{
 			user_item->next_add_score_time = now_time + other_cfg.add_score_interval;
 			ActivityShadow::Instance().OnAddGuildBattleRoleScore(temp_role, other_cfg.add_score_once);

			this->SyncChangeNotifyInfo(temp_role, Protocol::SC_CROSS_GUILDBATTLE_NOTIFY_TYPE_SCORE);
 		}
	}
}

void SpecialCrossGuildBattle::CheckTask(Role* role, int task_param)
{
	if (NULL == role) return;

	static CrossGuildBattleGuildItem cross_guildbattle_list[CROSS_GUILDBATTLE_MAX_SCENE_NUM];

	std::map<int, std::map<int, CrossGuildBattleTaskCfg> > &task_map = CrossGuildBattleConfig::Instance().GetGuildBattleTaskMap();
	if (task_map.find(m_scene->GetSceneID()) == task_map.end()) return;

	std::map<int, CrossGuildBattleTaskCfg> &task_cfg_map = task_map[m_scene->GetSceneID()];
	std::map<int, CrossGuildBattleTaskCfg>::iterator iter = task_cfg_map.begin();
	for (; iter != task_cfg_map.end(); iter++)
	{
		CrossGuildBattleTaskCfg &task_cfg = iter->second;
		RoleCrossParam &param = role->GetRoleCross()->GetRoleCrossParam();
		if (task_cfg.task_index < 0 || task_cfg.task_index >= CROSS_GUILDBATTLE_MAX_SCENE_NUM) continue;
		if (task_cfg.task_id < 0 || task_cfg.task_id >= CROSS_GUILDBATTLE_MAX_TASK_NUM) continue;

		int task_reward_flag = param.cross_guildbattle_task_reward_flag[task_cfg.task_index];
		if (0 != (task_reward_flag & (1 << iter->second.task_id))) continue;

		if (task_param == iter->second.task_param)
		{
			++param.cross_guildbattle_task_record[task_cfg.task_index][task_cfg.task_id];
			if (param.cross_guildbattle_task_record[task_cfg.task_index][task_cfg.task_id] >= task_cfg.task_count)
			{
				param.cross_guildbattle_task_reward_flag[task_cfg.task_index] |= (1 << task_cfg.task_id);
				//role->GetShenshou()->AddHunalingScore(task_cfg.reward_credit);
				role->GetKnapsack()->PutListOrMail(task_cfg.reward_item, PUT_REASON_CROSS_GUILDBATTLE_TASK);
				role->AddCrossHonor(task_cfg.cross_honor, "SpecialCrossGuildBattle::CheckTask");

				memset(cross_guildbattle_list, 0, sizeof(cross_guildbattle_list));
				WorldStatus::Instance().GetCrossGuildBattleInfo(cross_guildbattle_list);
				if (cross_guildbattle_list[task_cfg.task_index].guild_id > 0
					&& cross_guildbattle_list[task_cfg.task_index].plat_type == role->GetRoleCross()->GetCrossOriginalUniqueUserID().plat_type
					&& role->GetGuildID() == cross_guildbattle_list[task_cfg.task_index].guild_id)
				{
					//role->GetShenshou()->AddHunalingScore(task_cfg.extra_credit);
					role->GetKnapsack()->PutListOrMail(task_cfg.extra_item, PUT_REASON_CROSS_GUILDBATTLE_TASK);
					role->AddCrossHonor(task_cfg.extra_cross_honor, __FUNCTION__);
				}

				if (ActivityShadow::Instance().CheckCrossGuildBattleSpecialTime())
				{
					//role->GetShenshou()->AddHunalingScore(task_cfg.special_time_extra_credit);
					role->GetKnapsack()->PutListOrMail(task_cfg.special_time_item, PUT_REASON_CROSS_GUILDBATTLE_TASK);
				}
			}
			role->GetRoleCross()->SendGuildBattleTaskInfo();
		}
	}
}

void SpecialCrossGuildBattle::KickAllToOrginalToServer()
{
	for (int i = 0; i < (int)m_scene->RoleNum(); ++i)
	{
		Role *role = m_scene->GetRoleByIndex(i);
		if (NULL != role)
		{
			role->ReAlive(100, 100);
			role->BackToOriginalServer();
		}
	}
}

UniqueUserID SpecialCrossGuildBattle::GetGuildUniqueUserId(Role *role)
{
	if (NULL == role) return INVALID_UNIQUE_USER_ID;

	int plat_type = role->GetRoleCross()->GetCrossOriginalUniqueUserID().plat_type;
	UniqueUserID guild_id(plat_type, IntToUid(role->GetGuildID()));

	return guild_id;
}

void SpecialCrossGuildBattle::OnAddObj(Obj *obj)
{
}

void SpecialCrossGuildBattle::OnAttack(Character *cha, ObjID enemy_id, Attribute delta_hp)
{
	if (!m_activity_open)
	{
		Monster *monster = dynamic_cast <Monster *>(cha);
		if (nullptr != monster)
		{
			ShouHuBossInfo * boss = this->GetShouHuBossInfo(monster->GetMonsterId());
			if (NULL != boss)
			{
				Obj *obj = m_scene->GetObj(enemy_id);
				Role * role = dynamic_cast<Role *> (obj);
				if (nullptr != role)
				{
					long long uuid = UniqueUserIDToLongLong(role->GetRoleCross()->GetCrossOriginalUniqueUserID());
					UniqueUserID guild_id = this->GetGuildUniqueUserId(role);
					auto it = boss->guild_hurt_map.find(guild_id);
					if (boss->guild_hurt_map.end() == it)
					{
						GuildHurt guild_hurt;

						guild_hurt.guild_id = guild_id;
						guild_hurt.hurt -= delta_hp;
						guild_hurt.logout_timestamp = 0;
						guild_hurt.uuid_map.insert(std::make_pair(uuid, delta_hp));

						boss->guild_hurt_map.insert(std::make_pair(guild_id, guild_hurt));
						//m_boss_list[i].guild_hurt_map[guild_id].uuid_map.insert(std::make_pair(uuid, guild_hurt.hurt));
					}
					else
					{
						it->second.hurt -= delta_hp;
						it->second.uuid_map[uuid] -= delta_hp;
						it->second.logout_timestamp = 0;
					}
				}
			}
		}
		return;
	}

	if (Obj::OBJ_TYPE_ROLE == cha->GetObjType())
	{
		Role *role = (Role*)cha;
		Obj *enemy_obj = m_scene->GetObj(enemy_id);
		if (NULL != enemy_obj && Obj::OBJ_TYPE_ROLE == enemy_obj->GetObjType())
		{
			Role *enemy_role = (Role*)enemy_obj;

			UserItem *user_item = this->GetUserItem(role);
			if (NULL != user_item)
			{
				unsigned int now = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				
				UserItem::HurtMap::iterator it = user_item->be_hurt_user_map.find(enemy_role->GetUID());
				if (it != user_item->be_hurt_user_map.end())
				{
					it->second.obj_id = enemy_role->GetId();
					it->second.last_hurt_time = now;
				}
				else
				{
					user_item->be_hurt_user_map[enemy_role->GetUID()] = SpecialCrossGuildBattle::HurtInfo(enemy_role->GetId(), now);
				}
			}

			CrossGuildBattleUserInfo * user_info = ActivityShadow::Instance().GetCrossGuildBattleUser(enemy_role);
			if (NULL != user_info)
			{
				user_info->hurt_roles -= delta_hp;
			}
		}
	}

	else if (Obj::OBJ_TYPE_MONSTER == cha->GetObjType())
	{
		Monster *monster = (Monster *)cha;

		FlagItem *flag_item = this->GetFlagItemById(monster->GetMonsterId());
		if (NULL != flag_item)
		{
			if (INVALID_UNIQUE_USER_ID != flag_item->guild_id && CrossGuildBattleConfig::Instance().IsFlag(monster->GetMonsterId()) && monster->IsAlive())
			{
				unsigned int now_second = static_cast<unsigned int>(EngineAdapter::Instance().Time());

				{
					//if (now_second >= flag_item->last_warning_attack_time + CrossGuildBattleConfig::Instance().GetWarningInterval())
					unsigned int notify_cd = LOGIC_CONFIG->GetActivityMsgConfig().GetMonsterAttackNotifyCD(m_scene->GetSceneType(), monster->GetMonsterId());
					if (notify_cd >0 && now_second >= flag_item->last_warning_attack_time + notify_cd)
					{
						flag_item->last_warning_attack_time = now_second;

						this->NotifyWarning(WARNING_TPYE_ATTACK, flag_item->guild_id, monster->GetMonsterId());
					}
				}
			}
			else
			{
				
				long long now_hp = monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
				long long max_hp = monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
				long long old_hp = now_hp + (-delta_hp);

				int old_percent = static_cast<int>(max_hp > 0 ? old_hp * 100 / max_hp : 0);
				int now_percent = static_cast<int>(max_hp > 0 ? now_hp * 100 / max_hp : 0);

				if (old_percent > now_percent)
				{
					int warning_percent = LOGIC_CONFIG->GetActivityMsgConfig().CheckMonsterHpPercent(m_scene->GetSceneType(), old_percent, now_percent);
					//if (CrossGuildBattleConfig::Instance().CheckWarning(old_percent, now_percent, &warning_percent))
					if (warning_percent >= 0)
					{
						this->NotifyWarning(WARNING_TPYE_HP, flag_item->guild_id, monster->GetMonsterId(), warning_percent);
					}
				}
				
			}

			//this->OnFlagMonsterBeAttacked(flag_item, monster);

			Obj *enemy_obj = m_scene->GetObj(enemy_id);
			if (NULL != enemy_obj && Obj::OBJ_TYPE_ROLE == enemy_obj->GetObjType())
			{
				Role *role = (Role*)enemy_obj;
				CrossGuildBattleUserInfo * user_info = ActivityShadow::Instance().GetCrossGuildBattleUser(role);
				if (NULL != user_info)
				{
					user_info->hurt_flags -= delta_hp;
				}
			}
		}
	}
}

void SpecialCrossGuildBattle::OnCharacterDie(Character *dead, ObjID killer_id)
{
	Role *killer_role = m_scene->GetPrincipal(killer_id);
	if (NULL == dead || NULL == killer_role)
	{
		return;
	}

	if (!m_activity_open)
	{
		if (Obj::OBJ_TYPE_MONSTER == dead->GetObjType())
		{
			Monster *monster = (Monster *)dead;
			this->CheckTask(killer_role, monster->GetMonsterId());

			ShouHuBossInfo* boss_info = this->GetShouHuBossInfo(monster->GetMonsterId());
			if (MonsterInitParam::MONSTER_TYPE_BOSS == monster->GetMonsterType() && boss_info != NULL)
			{
				const CrossGuildBattleBossCfg *boss_cfg = CrossGuildBattleConfig::Instance().GetGuildBattleBossCfg(boss_info->boss_id);
				if (NULL != boss_cfg)
				{
					killer_role->GetShenshou()->AddHunalingScore(boss_cfg->kill_credit);
				}

				this->CheckSingleBossBound(boss_info->boss_id);

				boss_info->boss_status = SHOUHU_BOSSSTATUS_NONEXIST;
				this->SynBossInfo(NULL, false);
				this->SynBossInfoToOrigin(0, false);	//传0表示广播到原服
				this->OnShouHuBossDie(boss_info);


				const CrossGuildBattleMonsterCredit *credit_cfg = CrossGuildBattleConfig::Instance().GetMonsterConvertCreditCfg(boss_info->boss_id);
				if (nullptr != credit_cfg)
				{
					killer_role->GetShenshou()->AddHunalingScore(credit_cfg->credit);

					int scene_id_list[CROSS_GUILDBATTLE_MAX_SCENE_NUM] = { 0 };
					CrossGuildBattleConfig::Instance().GetGuildBattleSceneIdList(scene_id_list);
					for (int i = 0; i < CROSS_GUILDBATTLE_MAX_SCENE_NUM; ++i)
					{
						if (m_scene->GetSceneID() == scene_id_list[i])
						{
							static CrossGuildBattleGuildItem cross_guildbattle_list[CROSS_GUILDBATTLE_MAX_SCENE_NUM];
							memset(cross_guildbattle_list, 0, sizeof(cross_guildbattle_list));
							WorldStatus::Instance().GetCrossGuildBattleInfo(cross_guildbattle_list);
							if (cross_guildbattle_list[i].guild_id == killer_role->GetGuildID() &&
								cross_guildbattle_list[i].plat_type == killer_role->GetRoleCross()->GetCrossOriginalUniqueUserID().plat_type)
							{
								killer_role->GetShenshou()->AddHunalingScore(credit_cfg->extra_credit);

								// 跨服仙盟击杀榜
								UniqueUserID unique_guild_id(cross_guildbattle_list[i].plat_type, IntToUid(cross_guildbattle_list[i].guild_id));
								WorldStatus::Instance().AddGuildKillBossNum(unique_guild_id, m_scene->GetSceneType());
							}

							break;
						}
					}
				}


				// 羽翼掉落包
				const CrossGuildBattleOtherCfg &other_cfg = CrossGuildBattleConfig::Instance().GetOtherCfg();
				const int drop_wing_total_num = ActivityShadow::Instance().GetCrossGuildBattleDropWingNum();
				const int daily_drop_wing_get_num = killer_role->GetRoleCross()->GetCrossGuildBattleDailyDropWingNum();

				int drop_rate = CrossGuildBattleConfig::Instance().GetSpecialImgDropRate(boss_info->boss_id);

				if (drop_wing_total_num < other_cfg.wing_drop_total_num && daily_drop_wing_get_num < m_today_drop_limit_num)
				{
					if (drop_rate > 0 && RandomNum(10000) < drop_rate)
					{
						ActivityShadow::Instance().IncreaseCrossGuildBattleDropWingNum();
						killer_role->GetRoleCross()->IncreaseCrossGuildBattleDailyDropWingNum();

						DROPPOOL->DropHelper(other_cfg.wing_drop_id, m_scene, dead->GetPos(), killer_role->GetUserId(), killer_role->GetProf(), boss_info->boss_id,
							false, true, nullptr);

						// sync log
	// 					int drop_item_count = 0;
	// 					static ItemConfigData item_list[DropConfig::DROP_ITEM_PROB_MAX];
	//					drop_item_count = DROPPOOL->GetDropItemList(other_cfg.wing_drop_id, item_list);
	// 
	// 					for (int i = 0; i < drop_item_count && i < DropConfig::DROP_ITEM_PROB_MAX; ++i)
	// 					{
	// 						this->OnSyncDropLog(killer_role, 1, item_list[i].item_id, item_list[i].num);
	// 					}
					}
				}
			}
		}
		return;
	}
	
	// flag dead
	if (Obj::OBJ_TYPE_MONSTER == dead->GetObjType())
	{
		Monster *monster = (Monster *)dead;

		FlagItem *flag_item = this->GetFlagItemById(monster->GetMonsterId());
		if (NULL != flag_item)
		{
			int old_guild_id = UidToInt(flag_item->guild_id.user_id);

			flag_item->next_add_score_time = 0;
			flag_item->obj_id = INVALID_OBJ_ID;
			flag_item->guild_id = INVALID_UNIQUE_USER_ID;

			Role *killer_role = m_scene->GetPrincipal(killer_id);
			if (NULL != killer_role)
			{
				flag_item->guild_id = this->GetGuildUniqueUserId(killer_role);

				// 传闻
				{
					GuildItem *guild_item = this->GetGuildItem(this->GetGuildUniqueUserId(killer_role));
					if (NULL != guild_item)
					{
						int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_cross_guildbattle_own_flag_succc, 
							guild_item->guild_id.user_id.db_index, guild_item->guild_name, m_scene->GetSceneID(), flag_item->flag_id); 
						if (length > 0)
						{
							//World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_ACTIVE_NOTICE);
							m_scene->NotifySystemMsg(gamestring::GAMESTRING_BUF, length, SYS_MSG_ACTIVE_NOTICE);
						}
					}
				}
			}

			{
				int city_index = CrossGuildBattleConfig::Instance().GetCityIndex(m_scene->GetSceneID());
				int content_len = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), "cross_guild_battle_occupied",city_index,
					monster->GetMonsterId());

				int sendlen = 0;
				Protocol::SCSystemMsg sm;

				if (SceneManager::CreateSystemMsg(&sm, &sendlen, gstr::GSTR_BUF, content_len, SYS_MSG_ACTIVITY_SPECIAL))
				{
					int scene_id_list[CROSS_GUILDBATTLE_MAX_SCENE_NUM] = { 0 };
					CrossGuildBattleConfig::Instance().GetGuildBattleSceneIdList(scene_id_list);

					for (int j = 0; j < CROSS_GUILDBATTLE_MAX_SCENE_NUM; ++j)
					{
						Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(scene_id_list[j], COMMON_SCENE_KEY);
						if (nullptr != scene)
						{
							scene->SendToRole((const char*)&sm, sendlen, [old_guild_id](Role *role)->bool {
								if (old_guild_id == role->GetGuildID())
								{
									return true;
								}

								return false;
							});
						}
					}
				}
			}
			
			this->RefreshCreateFlag();
			this->SyncSceneInfo(NULL);

			CrossGuildBattleUserInfo * user_info = ActivityShadow::Instance().GetCrossGuildBattleUser(killer_role);
			if (NULL != user_info)
			{
				++ user_info->kill_flags;
			}
		}
	}

	// add score
	if (Obj::OBJ_TYPE_ROLE == dead->GetObjType())
	{
		Role *dead_role = (Role*)dead;

		const CrossGuildBattleOtherCfg &other_cfg = CrossGuildBattleConfig::Instance().GetOtherCfg();

		// 击杀者加分
		{
			ActivityShadow::Instance().OnAddGuildBattleRoleScore(killer_role, other_cfg.kill_add_score);
			this->SyncChangeNotifyInfo(killer_role, Protocol::SC_CROSS_GUILDBATTLE_NOTIFY_TYPE_SCORE);
		}
		
		// 协助者加分
		{
			SpecialCrossGuildBattle::UserItem *user_item = this->GetUserItem(dead_role);
			if (NULL != user_item)
			{
				UserItem::HurtMap::iterator iter = user_item->be_hurt_user_map.begin();

				for (; user_item->be_hurt_user_map.end() != iter; ++ iter)
				{
					if (iter->first != killer_role->GetUID() 
						&& iter->second.obj_id != killer_role->GetId()
						&& iter->second.last_hurt_time + other_cfg.assist_time >= static_cast<unsigned int>(EngineAdapter::Instance().Time()))
					{
						Role *assist_role = m_scene->GetPrincipal(iter->second.obj_id);
						if (NULL != assist_role)
						{
							ActivityShadow::Instance().OnAddGuildBattleRoleScore(assist_role, other_cfg.assist_add_score);
							this->SyncChangeNotifyInfo(assist_role, Protocol::SC_CROSS_GUILDBATTLE_NOTIFY_TYPE_SCORE);
						}
					}
				}
			}
		}

		Role *killer_role = m_scene->GetPrincipal(killer_id);
		if (NULL != killer_role)
		{
			CrossGuildBattleUserInfo * user_info = ActivityShadow::Instance().GetCrossGuildBattleUser(killer_role);
			if (NULL != user_info)
			{
				++user_info->kill_role_num;
			}
		}

// 		{
// 			GuildOtherConfig &other_cfg = LOGIC_CONFIG->GetGuildConfig().GetGuildOtherConfig();
// 
// 			if (dead_role->GetGuildPost() > GUILD_POST_CHENG_YUAN || dead_role->GetVip()->GetVipLevel() > other_cfg.chengyuan_event_notice_vip_level)
// 			{
// 				int content_len = 0;
// 
//  				if (RandomNum(10000) < 5000)
//  				{
//  					content_len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_guild_officer_be_killed,
//  						dead_role->GetGuildPost(), dead_role->GetUID(), dead_role->GetName(), dead_role->GetCamp());
//  				}
//  				else
// 				{
// 					content_len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_guild_chengyuan_be_killed,
// 						dead_role->GetGuildPost(), dead_role->GetUID(), dead_role->GetName(), dead_role->GetCamp(), m_scene->GetSceneName(), dead_role->GetPos().x, 
// 						dead_role->GetPos().y, m_scene->GetSceneID(), m_scene->GetSceneKey(), 1);
// 				}
// 
// 				if (content_len > 0)
// 				{
// 					int sendlen = 0;
// 					Protocol::SCSystemMsg sm;
// 
// 					if (SceneManager::CreateSystemMsg(&sm, &sendlen, gamestring::GAMESTRING_BUF, content_len, SYS_MSG_EVENT_TYPE_SPECIAL_NOTICE))
// 					{
// 						m_scene->SendToRole((const char*)&sm, sendlen, [dead_role](Role *role)->bool {
// 							if (dead_role->GetGuildID() == role->GetGuildID())
// 							{
// 								return true;
// 							}
// 
// 							return false;
// 						});
// 					}
// 				}
// 			}
// 		}
	}
}

void SpecialCrossGuildBattle::OnGather(Role *role, int gather_id, ObjID obj_id, const Posi &pos)
{
	if (!m_activity_open)
	{
		this->CheckTask(role, gather_id);

		const CrossGuildBattleGather * gather_cfg= CrossGuildBattleConfig::Instance().GetGatherCfgById(gather_id);
		if (nullptr != gather_cfg && nullptr != role)
		{
			std::map<int, ShouHuBossInfo>::iterator itr = m_shouhu_boss_map.find(gather_cfg->boss_id);
			if(itr != m_shouhu_boss_map.end())
			{
				itr->second.gather_list.insert(role->GetRoleCross()->GetCrossOriginalUniqueUserID());
			}

			ItemConfigData item = gather_cfg->guild_reward_item;
			Knapsack * knapsack = role->GetKnapsack();
			knapsack->PutOrMail(item, PUT_REASON_CROSS_GUILDBATTLE_BOSS);
			CrossGuildBattleGuildItem cross_guildbattle_list[CROSS_GUILDBATTLE_MAX_SCENE_NUM];

			WorldStatus::Instance().GetCrossGuildBattleInfo(cross_guildbattle_list);
			int plat_type = role->GetRoleCross()->GetCrossOriginalUniqueUserID().plat_type;
			int scene_index = CrossGuildBattleConfig::Instance().GetCityIndex(m_scene->GetSceneID());
			if (scene_index >= 0 && scene_index < CROSS_GUILDBATTLE_MAX_SCENE_NUM && cross_guildbattle_list[scene_index].plat_type == plat_type 
				&& cross_guildbattle_list[scene_index].guild_id == role->GetGuildID())
			{
				knapsack->PutOrMail(item, PUT_REASON_CROSS_GUILDBATTLE_BOSS);//占领奖励双倍
			}
		
		}
	}
}

bool SpecialCrossGuildBattle::SpecialRelive(Role *role, int realive_type)
{
	if (Protocol::CSRoleReAliveReq::REALIVE_TYPE_BACK_HOME == realive_type)		
	{
		const CrossGuildBattleOtherCfg &other_cfg = CrossGuildBattleConfig::Instance().GetOtherCfg();
		if (role->GetDeadTime() < (other_cfg.relive_time - 5) * 1000)  // 允许有误差
		{
			return true;
		}

		Posi realive_pos = CrossGuildBattleConfig::Instance().GetRandEnterPosBySceneID(this->GetSceneId());
		if (realive_pos != Posi(0, 0))
		{
			role->ReAlive(100, 100, realive_pos);
		}

		return true;
	}
	
	// 其他项目注意。元宝需要回到原服扣。此项目不用

	return false;
}

void SpecialCrossGuildBattle::OnRoleEnterScene(Role *role)
{
	if (nullptr == role) return;

	if (role->GetGuildID() <= 0)  // 没有帮派直接T出
	{
		role->BackToOriginalServer();
		return;
	}

	if (NULL == this->GetUserItem(role))
	{
		SpecialCrossGuildBattle::UserItem user_item;
		user_item.role_id = role->GetUID();
		m_role_map.insert(std::make_pair(role->GetUID(), user_item));
		m_total_level += role->GetLevel();
		++m_total_num;
	}

	UniqueUserID guild_id = this->GetGuildUniqueUserId(role);
	GuildMap::iterator g_itr = m_guild_map.find(guild_id);
	if (m_guild_map.end() == g_itr)
	{
		SpecialCrossGuildBattle::GuildItem guild_item;
		guild_item.guild_id = guild_id;
		guild_item.score = 0;
		guild_item.own_num = 0;
		guild_item.role_num = 1;
		role->GetRoleCross()->GetCrossGuildBattleTuanzhangName(guild_item.guild_tuanzhang_name);
		guild_item.guild_tuanzhang_prof = role->GetRoleCross()->GetCrossGuildBattleTuanzhangProf();
		guild_item.guild_tuanzhang_sex = role->GetRoleCross()->GetCrossGuildBattleTuanzhangSex();
		guild_item.guild_tuangzhang_uuid = role->GetRoleCross()->GetCrossGuildBattleTuanzhangUuid();

		role->GetGuildName(guild_item.guild_name);

		const RoleAppearance  &appearance = role->GetAppearance();
		guild_item.wuqi_id = appearance.wuqi_id;
		guild_item.shizhuang_wuqi = appearance.shizhuang_wuqi;
		guild_item.shizhuang_wuqi_is_special = appearance.shizhuang_wuqi_is_special;
		guild_item.shizhuang_body = appearance.shizhuang_body;
		guild_item.shizhuang_body_is_special = appearance.shizhuang_body_is_special;
		guild_item.halo_used_imageid = appearance.halo_used_imageid;
		guild_item.toushi_used_imageid = appearance.toushi_used_imageid;
		guild_item.qilinbi_used_imageid = appearance.qilinbi_used_imageid;
		guild_item.mask_used_imageid = appearance.mask_used_imageid;

		m_guild_map.insert(std::make_pair(guild_id, guild_item));
	}
	else
	{
		g_itr->second.role_num++;
		if (g_itr->second.guild_tuangzhang_uuid == UniqueUserIDToLongLong(role->GetUniqueUserID()))
		{
			const RoleAppearance  &appearance = role->GetAppearance();
			g_itr->second.wuqi_id = appearance.wuqi_id;
			g_itr->second.shizhuang_wuqi = appearance.shizhuang_wuqi;
			g_itr->second.shizhuang_wuqi_is_special = appearance.shizhuang_wuqi_is_special;
			g_itr->second.shizhuang_body = appearance.shizhuang_body;
			g_itr->second.shizhuang_body_is_special = appearance.shizhuang_body_is_special;
			g_itr->second.halo_used_imageid = appearance.halo_used_imageid;
			g_itr->second.toushi_used_imageid = appearance.toushi_used_imageid;
			g_itr->second.qilinbi_used_imageid = appearance.qilinbi_used_imageid;
			g_itr->second.mask_used_imageid = appearance.mask_used_imageid;
		}
	}

	this->SyncSceneInfo(role);
	this->SyncChangeNotifyInfo(role, Protocol::SC_CROSS_GUILDBATTLE_NOTIFY_TYPE_ENTER);
	role->GetRoleCross()->SendGuildBattleTaskInfo();
	this->SynBossInfo(role);
	this->SyncFlagInfo(role);
	ActivityShadow::Instance().SyncCrossGuildBattleSceneRoleNum(guild_id);
	ActivityShadow::Instance().GetCrossGuildBattleUser(role);//确保进场景表有玩家
	ActivityShadow::Instance().SyncCrossGuildBattleMvpInfo(guild_id, role);
	if (m_activity_open)
	{
		EventHandler::Instance().OnEnterCrossGuildBattle(role);
	}
}

void SpecialCrossGuildBattle::OnRoleLeaveScene(Role *role, bool is_logout)
{
	if (m_role_map.end() != m_role_map.find(role->GetUID()))
	{
		m_role_map.erase(role->GetUID());
	}
	if (is_logout)
	{
		role->BackToOriginalServer();
	}

	UniqueUserID guild_id = this->GetGuildUniqueUserId(role);
	GuildMap::iterator g_itr = m_guild_map.find(guild_id);
	if (g_itr != m_guild_map.end() && g_itr->second.role_num >0)
	{
		g_itr->second.role_num--;
	}

	long long uuid = UniqueUserIDToLongLong(role->GetRoleCross()->GetCrossOriginalUniqueUserID());
	for (auto mitr = m_shouhu_boss_map.begin(); mitr != m_shouhu_boss_map.end(); ++mitr)
	{
		if (SHOUHU_BOSSSTATUS_NONEXIST == mitr->second.boss_status)
		{
			continue;
		}

		UniqueUserID guild_id = this->GetGuildUniqueUserId(role);
		auto guild_hurt_it = mitr->second.guild_hurt_map.find(guild_id);
		if (guild_hurt_it != mitr->second.guild_hurt_map.end())
		{
			GuildHurt &guild_hurt = guild_hurt_it->second;

			guild_hurt.uuid_map.erase(uuid);

			if (!is_logout)  // 主动退出就清除归属
			{
				if (guild_hurt.uuid_map.size() <= 0)
				{
					if (mitr->second.guild_uuid == guild_hurt.guild_id)
					{
						mitr->second.guild_uuid = INVALID_UNIQUE_USER_ID;
						//printf("\n Clear owner leave ... \n");
					}
					mitr->second.guild_hurt_map.erase(guild_hurt_it);
				}
			}
			else
			{
				// 最后一名玩家掉线设置离线标记
				if (guild_hurt.uuid_map.size() <= 0)
				{
					guild_hurt.logout_timestamp = EngineAdapter::Instance().Time();
				}
			}
		}
	}

	ActivityShadow::Instance().SyncCrossGuildBattleSceneRoleNum(guild_id);
}

bool SpecialCrossGuildBattle::SpecialIsEnemy(Role *role, Obj *obj)
{
	if (NULL == role || NULL == obj)
	{
		return false;
	}

	if (role->GetId() == obj->GetId())
	{
		return false;
	}

	if (Obj::OBJ_TYPE_ROLE == obj->GetObjType())
	{
		if (IsWorshipDuring())
		{
			return false;
		}

		Role *target = (Role *)obj;
		UniqueUserID role_guild_id = this->GetGuildUniqueUserId(role);
		UniqueUserID target_guild_id = this->GetGuildUniqueUserId(target);

		if (role_guild_id == target_guild_id)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	
	if (Obj::OBJ_TYPE_MONSTER == obj->GetObjType())
	{
		UniqueUserID role_guild_id = this->GetGuildUniqueUserId(role);

		Monster *target = (Monster *)obj;
		SpecialCrossGuildBattle::FlagItem *flag_item = this->GetFlagItemById(target->GetMonsterId());
		if (NULL != flag_item && (!m_activity_open || flag_item->guild_id == role_guild_id))
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	return false;
}

bool SpecialCrossGuildBattle::SpecialCanGather(Role *role, GatherObj *gather)
{
	if (m_activity_open) return false;
	if (nullptr == gather|| nullptr == role) return false;
	const CrossGuildBattleGather * gathercfg = CrossGuildBattleConfig::Instance().GetGatherCfgById(gather->GatherId());
	if (nullptr != gathercfg)
	{
		auto it = m_shouhu_boss_map.find(gathercfg->boss_id);
		if (m_shouhu_boss_map.end() != it && it->second.boss_status == SHOUHU_BOSSSTATUS_NONEXIST &&it->second.gather_guild_uuid == GetGuildUniqueUserId(role))
		{	
			if (it->second.gather_list.end() != it->second.gather_list.find(role->GetRoleCross()->GetCrossOriginalUniqueUserID()))
			{
				role->NoticeNum(errornum::EN_CROSS_GUILDBATTLE_GATHER_LIMIT);
				return false;
			}
			return true;
		}
		return false;
	}
	return true;
}

void SpecialCrossGuildBattle::OnPickFallingItem(Role *role, FallingItem *falling_item, ItemDataWrapper *item_wrapper)
{
	if (NULL == role || NULL == falling_item)return;

	if (m_activity_open) return;

	if (falling_item->isBroadcast())
	{
		this->OnSyncDropLog(role, 1, falling_item->GetItemId(), falling_item->GetItemNum());
	}
}

void SpecialCrossGuildBattle::OnActivityStatusChange(int status)
{
	if (ACTIVITY_STATUS_STANDY == status)
	{
		this->OnActivitystandy();
	}
	if (ACTIVITY_STATUS_OPEN == status)
	{
		this->OnActivityOpen();
	}
	else if (ACTIVITY_STATUS_CLOSE == status)
	{
		this->OnActivityClose();
	}
}

void SpecialCrossGuildBattle::OnActivitystandy()
{
	this->KickAllToOrginalToServer();

	m_worship_user_map.clear();
	m_worship_start_time = 0;
}

void SpecialCrossGuildBattle::OnActivityOpen()
{
	if (m_activity_open)
	{
		return;
	}

	m_activity_open = true;
	m_first = true;

	m_worship_user_map.clear();
	m_worship_start_time = 0;

	this->KillAllMonster();
	this->ChangeAllBossStatus(SHOUHU_BOSSSTATUS_NONEXIST);
	this->StopGenerater();
}

void SpecialCrossGuildBattle::OnActivityClose()
{
	if (!m_activity_open)
	{
		return;
	}

	m_activity_open = false;

	{
		m_guild_map.clear();
		m_role_map.clear();
		m_score_rank_vec.clear();
		m_worship_user_map.clear();
		m_worship_start_time = EngineAdapter::Instance().Time();
		for (int i = 0; i < CROSS_GUILDBATTLE_MAX_FLAG_IN_SCENE; ++ i)
		{
			m_flag_list[i].Reset();
		}
	}
	this->KillAllMonster();
	this->StartGenerater();
}

void SpecialCrossGuildBattle::SendRankLogToRole(const char *log_msg, const int msg_length)
{
	// 下发最新的排行榜信息到各个参与的玩家
	if (NULL != log_msg)
	{
		m_scene->SendToRole(log_msg, msg_length);
	}
}

SpecialCrossGuildBattle::FlagItem *SpecialCrossGuildBattle::GetFlagItemById(int flag_id)
{
	for (int i = 0; i < CROSS_GUILDBATTLE_MAX_FLAG_IN_SCENE; ++ i)
	{
		if (m_flag_list[i].flag_id == flag_id)
		{
			return &m_flag_list[i];
		}
	}

	return NULL;
}

int SpecialCrossGuildBattle::GetOwnNum(UniqueUserID guild_id)
{
	if (INVALID_UNIQUE_USER_ID == guild_id)
	{
		return 0;
	}

	int count = 0;
	for (int i = 0; i < CROSS_GUILDBATTLE_MAX_FLAG_IN_SCENE; ++ i)
	{
		if (m_flag_list[i].guild_id == guild_id)
		{
			++ count;
		}
	}

	return count;
}

void SpecialCrossGuildBattle::OnFlagMonsterBeAttacked(FlagItem *flag_item, Monster *monster)
{
	if (nullptr == flag_item || nullptr == monster)
	{
		return;
	}

	if (0 == flag_item->has_be_hit_broadcast_flag)
	{
		flag_item->has_be_hit_broadcast_flag = 1;

		int content_len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_guild_flag_first_be_hit_notice,
			monster->GetMonsterId());

		int sendlen = 0;
		Protocol::SCSystemMsg sm;

		int guild_id = UidToInt(flag_item->guild_id.user_id);

		if (SceneManager::CreateSystemMsg(&sm, &sendlen, gamestring::GAMESTRING_BUF, content_len, SYS_MSG_EVENT_TYPE_SPECIAL_NOTICE))
		{
			int scene_id_list[CROSS_GUILDBATTLE_MAX_SCENE_NUM] = { 0 };
			CrossGuildBattleConfig::Instance().GetGuildBattleSceneIdList(scene_id_list);

			for (int j = 0; j < CROSS_GUILDBATTLE_MAX_SCENE_NUM; ++j)
			{
				Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(scene_id_list[j], COMMON_SCENE_KEY);
				if (nullptr != scene)
				{
					scene->SendToRole((const char*)&sm, sendlen, [guild_id](Role *role)->bool {
						if (guild_id == role->GetGuildID())
						{
							return true;
						}

						return false;
					});
				}
			}
		}
	}
	else
	{
		Attribute cur_hp = monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
		Attribute max_hp = monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);

		// 血量广播百分比
		static int low_hp_per_list[] = { 5000, 4000, 3000, 2000, 1000, 500, 100 };

		int hp_per = static_cast<int>(cur_hp * 1.0 / max_hp * 10000);

		for (UInt32 i = 0; i < (sizeof(low_hp_per_list) / sizeof(low_hp_per_list[0])); ++i)
		{
			if (hp_per < low_hp_per_list[i] && 0 == (flag_item->low_hp_broadcast_flag & (1 << i)))
			{
				flag_item->low_hp_broadcast_flag |= 1 << i;

				int content_len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_guild_flag_be_attacked_notice,
					monster->GetMonsterId(), low_hp_per_list[i] / 100);

				int sendlen = 0;
				Protocol::SCSystemMsg sm;

				const CrossGuildBattleOtherCfg &other_cfg = CrossGuildBattleConfig::Instance().GetOtherCfg();
				int guild_id = UidToInt(flag_item->guild_id.user_id);

				if (SceneManager::CreateSystemMsg(&sm, &sendlen, gamestring::GAMESTRING_BUF, content_len, SYS_MSG_EVENT_TYPE_SPECIAL_NOTICE))
				{
					if (m_scene->GetSceneID() == other_cfg.main_scene_id)
					{
						int scene_id_list[CROSS_GUILDBATTLE_MAX_SCENE_NUM] = { 0 };
						CrossGuildBattleConfig::Instance().GetGuildBattleSceneIdList(scene_id_list);

						for (int j = 0; j < CROSS_GUILDBATTLE_MAX_SCENE_NUM; ++j)
						{
							Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(scene_id_list[j], COMMON_SCENE_KEY);
							if (nullptr != scene)
							{
								scene->SendToRole((const char *)&sm, sendlen);
							}
						}
					}
					else
					{
						int scene_id_list[CROSS_GUILDBATTLE_MAX_SCENE_NUM] = { 0 };
						CrossGuildBattleConfig::Instance().GetGuildBattleSceneIdList(scene_id_list);

						for (int j = 0; j < CROSS_GUILDBATTLE_MAX_SCENE_NUM; ++j)
						{
							Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(scene_id_list[j], COMMON_SCENE_KEY);
							if (nullptr != scene)
							{
								scene->SendToRole((const char*)&sm, sendlen, [guild_id](Role *role)->bool {
									if (guild_id == role->GetGuildID())
									{
										return true;
									}

									return false;
								});
							}
						}
					}
				}
				break;
			}
		}
	}
}

SpecialCrossGuildBattle::UserItem *SpecialCrossGuildBattle::GetUserItem(Role *role)
{
	if (NULL == role) return NULL;

	UserMap::iterator iter = m_role_map.find(role->GetUID());
	if (m_role_map.end() == iter)
	{
		return NULL;
	}

	return &(iter->second);
}

SpecialCrossGuildBattle::GuildItem *SpecialCrossGuildBattle::GetGuildItem(UniqueUserID guild_id)
{
	GuildMap::iterator iter = m_guild_map.find(guild_id);
	if (m_guild_map.end() == iter)
	{
		return NULL;
	}

	return &(iter->second);
}

bool SortFunc(const SpecialCrossGuildBattle::GuildItem &item1, const SpecialCrossGuildBattle::GuildItem &item2)
{
	return item1.score > item2.score;
}

void SpecialCrossGuildBattle::GetGuildRankList(
	CrossGuildBattleRankItem rank_list[CROSS_GUILDBATTLE_MAX_GUILD_RANK_NUM], 
	CrossGuildBattleRankLogItem log_rank_list[CROSS_GUILDBATTLE_MAX_LOG_RANK_NUM])
{
	typedef std::vector<SpecialCrossGuildBattle::GuildItem> RankVec;
	RankVec vec;
	for (GuildMap::iterator iter = m_guild_map.begin(); m_guild_map.end() != iter; ++ iter)
	{
		vec.push_back(iter->second);
	}
	std::sort(vec.begin(), vec.end(), SortFunc);
	
	const int max_loop_cnt = (CROSS_GUILDBATTLE_MAX_GUILD_RANK_NUM > CROSS_GUILDBATTLE_MAX_LOG_RANK_NUM ? CROSS_GUILDBATTLE_MAX_GUILD_RANK_NUM : CROSS_GUILDBATTLE_MAX_LOG_RANK_NUM);
	int num = 0;
	for (RankVec::iterator iter = vec.begin(); vec.end() != iter; ++ iter)
	{
		if (num < CROSS_GUILDBATTLE_MAX_GUILD_RANK_NUM)
		{
			CrossGuildBattleRankItem rank_item;
			rank_item.guild_id = iter->guild_id;
			rank_item.score = iter->score;
			F_STRNCPY(rank_item.guild_name, iter->guild_name, sizeof(rank_item.guild_name));
			F_STRNCPY(rank_item.guild_tuanzhang_name, iter->guild_tuanzhang_name, sizeof(rank_item.guild_tuanzhang_name));
			rank_item.prof = iter->guild_tuanzhang_prof;
			rank_item.sex = iter->guild_tuanzhang_sex;
			rank_item.wuqi_id = iter->wuqi_id;
			rank_item.shizhuang_wuqi = iter->shizhuang_wuqi;
			rank_item.shizhuang_wuqi_is_special = iter->shizhuang_wuqi_is_special;
			rank_item.shizhuang_body = iter->shizhuang_body;
			rank_item.shizhuang_body_is_special = iter->shizhuang_body_is_special;
			rank_item.halo_used_imageid = iter->halo_used_imageid;
			rank_item.qilinbi_used_imageid = iter->qilinbi_used_imageid;
			rank_item.toushi_used_imageid = iter->toushi_used_imageid;
			rank_item.mask_used_imageid = iter->mask_used_imageid;

			rank_list[num] = rank_item;
		}

		if (num < CROSS_GUILDBATTLE_MAX_LOG_RANK_NUM)
		{
			CrossGuildBattleRankLogItem log_rank_item;
			F_STRNCPY(log_rank_item.guild_name, iter->guild_name, sizeof(log_rank_item.guild_name));
			log_rank_item.server_id = iter->guild_id.user_id.db_index;
			log_rank_item.score = iter->score;
			log_rank_item.tuanzhang_uuid = iter->guild_tuangzhang_uuid;

			log_rank_list[num] = log_rank_item;
		}

		if (num >= max_loop_cnt)
		{
			break;
		}
		else
		{
			++num;
		}
	}
}

int SpecialCrossGuildBattle::GetGuildJoinNum()
{
	int num = 0;
	for (GuildMap::iterator iter = m_guild_map.begin(); m_guild_map.end() != iter; ++ iter)
	{
		if (iter->second.score > 0)
		{
			++ num;
		}
	}

	return num;
}

void SpecialCrossGuildBattle::OnServerStart()
{
	if (!CrossConfig::Instance().IsHiddenServer()) return;
	
	unsigned int now_sec = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	unsigned int today_zero_time = static_cast<unsigned int>(GetZeroTime(static_cast<time_t>(now_sec)));
	unsigned int today_sec = now_sec - today_zero_time;

	const std::map<int, CrossGuildBattleBossCfg> &boss_map = CrossGuildBattleConfig::Instance().GetGuildBattleBossMap();
	std::map<int, CrossGuildBattleBossCfg>::const_iterator iter = boss_map.begin();
	for (; iter != boss_map.end(); iter++)
	{
		if (iter->second.scene_id != m_scene->GetSceneID()) continue;

		const CrossGuildBattleBossCfg &boss_cfg = iter->second;

		ShouHuBossInfo &boss_info = m_shouhu_boss_map[boss_cfg.boss_id];
		boss_info.boss_id = boss_cfg.boss_id;
		boss_info.born_pos = boss_cfg.born_pos;
		boss_info.refresh_interval = boss_cfg.refresh_interval;
		boss_info.begin_refresh_time = boss_cfg.start_refresh_time;
		boss_info.end_refresh_time = boss_cfg.end_refresh_time;
		boss_info.refresh_interval1 = boss_cfg.refresh_interval1;
		boss_info.begin_refresh_time1 = boss_cfg.start_refresh_time1;
		boss_info.end_refresh_time1 = boss_cfg.end_refresh_time1;

		if (today_sec < static_cast<unsigned int>(boss_cfg.start_refresh_time))
		{
			// 第一波的时间戳
			boss_info.next_refresh_time = today_zero_time + boss_cfg.start_refresh_time;
		}
		else if (today_sec >= static_cast<unsigned int>(boss_cfg.start_refresh_time) && today_sec < static_cast<unsigned int>(boss_cfg.end_refresh_time))
		{
			if (boss_cfg.refresh_interval > 0)
			{
				// 第二波的时间戳
				boss_info.next_refresh_time = now_sec + (boss_cfg.refresh_interval - (today_sec - boss_cfg.start_refresh_time) % boss_cfg.refresh_interval);
			}
		}
// 3波改为2波
// 		else if(today_sec >= static_cast<unsigned int>(boss_cfg.end_refresh_time) && today_sec < static_cast<unsigned int>(boss_cfg.start_refresh_time1))
// 		{
// 			boss_info.next_refresh_time = today_zero_time + boss_cfg.start_refresh_time1;
// 		}
// 		else if(today_sec >= static_cast<unsigned int>(boss_cfg.start_refresh_time1) && today_sec < static_cast<unsigned int>(boss_cfg.end_refresh_time1))
// 		{
// 			if (boss_cfg.refresh_interval1 > 0)
// 			{
// 				boss_info.next_refresh_time = now_sec + (boss_cfg.refresh_interval1 - (today_sec - boss_cfg.start_refresh_time1) % boss_cfg.refresh_interval1);
// 			}
// 		}
// 		else if (today_sec >= static_cast<unsigned int>(boss_cfg.end_refresh_time1))
// 		{
// 			int next_day_refresh_interval = EngineAdapter::Instance().NextDayInterval(0, 0, 0) + boss_cfg.start_refresh_time;
// 			boss_info.next_refresh_time = now_sec + next_day_refresh_interval;
// 		}
		else if (today_sec >= static_cast<unsigned int>(boss_cfg.end_refresh_time))
		{
			// 明天第一波的时间戳
			int next_day_refresh_interval = EngineAdapter::Instance().NextDayInterval(0, 0, 0) + boss_cfg.start_refresh_time;
			boss_info.next_refresh_time = now_sec + next_day_refresh_interval;
		}

		boss_info.boss_status = SHOUHU_BOSSSTATUS_NONEXIST;
	}
}

void SpecialCrossGuildBattle::SyncSceneInfo(Role *role)
{
	static Protocol::SCCrossGuildBattleSceneInfo cmd;
	
	cmd.scene_id = this->GetSceneId();
	// 旗子信息
	{
		for (int i = 0; i < CROSS_GUILDBATTLE_MAX_FLAG_IN_SCENE; ++ i)
		{
			Protocol::SCCrossGuildBattleSceneInfo::FlagItem &flag_item = cmd.flag_list[i];

			flag_item.monster_id = m_flag_list[i].flag_id;
			flag_item.plat_type = m_flag_list[i].guild_id.plat_type;
			flag_item.server_id = m_flag_list[i].guild_id.user_id.db_index;

			Monster *monster = m_scene->GetMonster(m_flag_list[i].obj_id);
			if (NULL != monster)
			{
				flag_item.cur_hp = monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
				flag_item.max_hp = monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
			}

			memset(flag_item.guild_name, 0, sizeof(flag_item.guild_name));
			GuildMap::iterator iter = m_guild_map.find(m_flag_list[i].guild_id);
			if (m_guild_map.end() != iter)
			{
				F_STRNCPY(flag_item.guild_name, iter->second.guild_name, sizeof(flag_item.guild_name));
			}
		}
	}

	// 所有城仙盟参与数信息
	{
		int scene_id_list[CROSS_GUILDBATTLE_MAX_SCENE_NUM] = {0};
		CrossGuildBattleConfig::Instance().GetGuildBattleSceneIdList(scene_id_list);
		
		for (int i = 0; i < CROSS_GUILDBATTLE_MAX_SCENE_NUM; ++ i)
		{
			Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(scene_id_list[i], CROSS_PUBLIC_SCENE_KEY);
			if (NULL != scene && Scene::SCENE_TYPE_CROSS_GUILDBATTLE == scene->GetSceneType())
			{
				SpecialCrossGuildBattle *scene_logic = dynamic_cast<SpecialCrossGuildBattle*>(scene->GetSpecialLogic());
				cmd.guild_join_num_list[i] = nullptr != scene_logic ? scene_logic->GetGuildJoinNum() : 0;
			}	
		}
	}
	
	// 排行榜信息
	{
		cmd.rank_list_count = 0;

// 		typedef std::vector<SpecialCrossGuildBattle::GuildItem> RankVec;
// 		RankVec vec;
		UniqueUserID old_first(INVALID_UNIQUE_USER_ID);
		if (m_score_rank_vec.size() > 0)
		{
			old_first = m_score_rank_vec[0]->guild_id;
		}
		m_score_rank_vec.clear();
		for (GuildMap::iterator iter = m_guild_map.begin(); m_guild_map.end() != iter; ++ iter)
		{
			iter->second.own_num = this->GetOwnNum(iter->second.guild_id);
			if(iter->second.score <= 0) continue;
			m_score_rank_vec.push_back(&iter->second);
		}
		std::sort(m_score_rank_vec.begin(), m_score_rank_vec.end(), [](const GuildItem *item1, const GuildItem *item2)
		{return item1->score > item2->score; });

		for (RankVec::iterator iter = m_score_rank_vec.begin(); m_score_rank_vec.end() != iter; ++ iter)
		{
			if (cmd.rank_list_count >= CROSS_GUILDBATTLE_MAX_GUILD_RANK_NUM)
			{
				break;
			}

			Protocol::SCCrossGuildBattleSceneInfo::RankItem &rank_item = cmd.rank_list[cmd.rank_list_count ++];
			rank_item.plat_type = (*iter)->guild_id.plat_type;
			rank_item.server_id = (*iter)->guild_id.user_id.db_index;
			memcpy(rank_item.guild_name, (*iter)->guild_name, sizeof(rank_item.guild_name));
			rank_item.score = (*iter)->score;
			rank_item.own_num = (*iter)->own_num;
		}

		if (m_score_rank_vec.size() > 0)
		{
			if (m_score_rank_vec[0]->guild_id != old_first && old_first != INVALID_UNIQUE_USER_ID)
			{
				UNSTD_STATIC_CHECK(sizeof(GameName)==sizeof(GuildName));
				ActivityMsgManager::RankTopItem first;
				first.uid = UniqueUserIDToLongLong(m_score_rank_vec[0]->guild_id);
				F_STRNCPY(first.user_name, m_score_rank_vec[0]->guild_name, sizeof(GameName));
				ActivityMsgManager::Instance().RankFirstMsg(m_scene, first);
			}

			if (ActivityMsgManager::Instance().IsNeedRankTopMsg(m_scene))
			{
				ActivityMsgManager::RankTopItem top[ACTIVITY_RANK_TOP_MSG_MAX];
				int i = 0;
				for (; i < (int)m_score_rank_vec.size() && i < ACTIVITY_RANK_TOP_MSG_MAX; ++i)
				{
					if(m_score_rank_vec[i]->score <= 0) break;
					top[i].uid = UniqueUserIDToLongLong(m_score_rank_vec[i]->guild_id);
					F_STRNCPY(top[i].user_name, m_score_rank_vec[i]->guild_name, sizeof(GameName));
				}
				ActivityMsgManager::Instance().RankTopMsg(m_scene, top, i);
			}
		}
	}

	if (NULL == role)
	{
		int scene_id_list[CROSS_GUILDBATTLE_MAX_SCENE_NUM] = { 0 };
		CrossGuildBattleConfig::Instance().GetGuildBattleSceneIdList(scene_id_list);

		for (int i = 0; i < CROSS_GUILDBATTLE_MAX_SCENE_NUM; ++i)
		{
			Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(scene_id_list[i], CROSS_PUBLIC_SCENE_KEY);
			if (nullptr != scene)
			{
				int len = sizeof(cmd) - sizeof(cmd.rank_list[0]) * (CROSS_GUILDBATTLE_MAX_GUILD_RANK_NUM - cmd.rank_list_count);
				scene->SendToRole((char*)&cmd, len);
			}
		}
	}
	else
	{
		int len = sizeof(cmd) - sizeof(cmd.rank_list[0]) * (CROSS_GUILDBATTLE_MAX_GUILD_RANK_NUM - cmd.rank_list_count);
		EngineAdapter::Instance().NetSend(role->GetNetId(), (char*)&cmd, len);
	}
}

void SpecialCrossGuildBattle::SyncSceneMonsterCountInfoToOrigin(Role *role)
{
	if (nullptr != role)
	{
	}
}

void SpecialCrossGuildBattle::OnSyncDropLog(Role *role, int log_type, unsigned short item_id, short num)
{
	static crossgameprotocal::GameCrossSyncGuildBattleDropInfo pro;

	pro.uuid = UniqueUserIDToLongLong(role->GetUniqueUserID());
	role->GetName(pro.name);
	pro.log_type = log_type;
	pro.timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	pro.item_id = item_id;
	pro.item_num = num;

	InternalComm::Instance().SendToCross((const char *)&pro, sizeof(pro));
}

int SpecialCrossGuildBattle::GetSpecialMonsterCount()
{
	int monster_count = 0;
	for (auto it = m_shouhu_boss_map.begin(); it != m_shouhu_boss_map.end(); ++it)
	{
		if (nullptr != CrossGuildBattleConfig::Instance().GetMonsterConvertCreditCfg(it->first))
		{
			if (it->second.boss_status == SHOUHU_BOSSSTATUS_EXIST)
			{
				++monster_count;
			}
		}
	}

	return monster_count;
}

void SpecialCrossGuildBattle::GetFlagInfo(Protocol::SCCrossGuildBattleFlagInfo::FlagItem flag_list[CROSS_GUILDBATTLE_MAX_FLAG_IN_SCENE])
{
	for (int i = 0; i < CROSS_GUILDBATTLE_MAX_FLAG_IN_SCENE; ++i)
	{
		Protocol::SCCrossGuildBattleFlagInfo::FlagItem &flag_item = flag_list[i];

		flag_item.monster_id = m_flag_list[i].flag_id;
		flag_item.plat_type = m_flag_list[i].guild_id.plat_type;
		flag_item.server_id = m_flag_list[i].guild_id.user_id.db_index;

		Monster *monster = m_scene->GetMonster(m_flag_list[i].obj_id);
		if (NULL != monster)
		{
			flag_item.cur_hp = monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
			flag_item.max_hp = monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
		}

		memset(flag_item.guild_name, 0, sizeof(flag_item.guild_name));
		GuildMap::iterator iter = m_guild_map.find(m_flag_list[i].guild_id);
		if (m_guild_map.end() != iter)
		{
			F_STRNCPY(flag_item.guild_name, iter->second.guild_name, sizeof(flag_item.guild_name));
		}
	}
}

void SpecialCrossGuildBattle::SyncChangeNotifyInfo(Role *role, int notify_type)
{
	static Protocol::SCCrossGuildBattleNotifyInfo cmd;
	cmd.notify_type = notify_type;
	cmd.param_1 = 0;
	cmd.param_2 = 0;
	
	if (NULL != role)
	{
		cmd.param_1 = ActivityShadow::Instance().GetGuildBattleRoleScore(role);
		cmd.param_2 = ActivityShadow::Instance().GetGuildBattleRoleSosTimes(role);
	}


	if (NULL == role)
	{
		const int role_num = (int)m_scene->RoleNum();
		for (int i = 0; i < role_num; ++ i)
		{
			Role *temp_role = m_scene->GetRoleByIndex(i);
			if (NULL != temp_role)
			{	
				EngineAdapter::Instance().NetSend(temp_role->GetNetId(), (char*)&cmd, sizeof(cmd));
			}
		}
	}
	else
	{
		EngineAdapter::Instance().NetSend(role->GetNetId(), (char*)&cmd, sizeof(cmd));
	}
}

void SpecialCrossGuildBattle::SynBossInfo(Role *role, bool is_refresh)
{
	static Protocol::SCCrossGuildBattleBossInfo cmd;
	cmd.scene_id = m_scene->GetSceneID();
	cmd.count = 0;
	cmd.is_refresh = is_refresh ? 1 : 0;

	std::map<int, ShouHuBossInfo>::iterator iter = m_shouhu_boss_map.begin();
	for (; iter != m_shouhu_boss_map.end() && cmd.count < Protocol::SCCrossGuildBattleBossInfo::MAX_BOSS_COUNT; iter++)
	{
		Protocol::SCCrossGuildBattleBossInfo::BossInfo &boss_info = cmd.boss_list[cmd.count++];
		boss_info.boss_id = iter->second.boss_id;
		boss_info.replace_boss_id = iter->second.replace_boss_id;
		boss_info.status = iter->second.boss_status;
		boss_info.next_refresh_time = iter->second.next_refresh_time;
	}

	int len = sizeof(Protocol::SCCrossGuildBattleBossInfo) - sizeof(Protocol::SCCrossGuildBattleBossInfo::BossInfo) * (Protocol::SCCrossGuildBattleBossInfo::MAX_BOSS_COUNT - cmd.count);
	if (len < 0) return;

	if (NULL != role)
	{
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&cmd, len);
	}
	else
	{
		m_scene->SendToRole((const char*)&cmd, len);
	}
}

void SpecialCrossGuildBattle::SyncFlagInfo(Role * role)
{
	if (nullptr == role || !m_activity_open ) return;

	static Protocol::SCCrossGuildBattleFlagInfo scfi;
	memset(scfi.flag_info, 0, sizeof(scfi.flag_info));

	int scene_id_list[CROSS_GUILDBATTLE_MAX_SCENE_NUM] = { 0 };
	CrossGuildBattleConfig::Instance().GetGuildBattleSceneIdList(scene_id_list);

	for (int i = 0; i < CROSS_GUILDBATTLE_MAX_SCENE_NUM; ++i)
	{
		Scene* scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(scene_id_list[i], CROSS_PUBLIC_SCENE_KEY);
		if (nullptr != scene)
		{
			SpecialCrossGuildBattle * special = dynamic_cast<SpecialCrossGuildBattle*>(scene->GetSpecialLogic());
			if (nullptr != special)
			{
				scfi.flag_info[i].scene_id = scene_id_list[i];
				special->GetFlagInfo(scfi.flag_info[i].flag_list);
			}
		}
	}

	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&scfi, sizeof(scfi));
}

void SpecialCrossGuildBattle::SynBossInfoToOrigin(long long uuid, bool is_refresh)
{
	static Protocol::SCCrossGuildBattleBossInfo cmd;
	cmd.scene_id = m_scene->GetSceneID();
	cmd.count = 0;
	cmd.is_refresh = is_refresh ? 1 : 0;

	std::map<int, ShouHuBossInfo>::iterator iter = m_shouhu_boss_map.begin();
	for (; iter != m_shouhu_boss_map.end() && cmd.count < Protocol::SCCrossGuildBattleBossInfo::MAX_BOSS_COUNT; iter++)
	{
		Protocol::SCCrossGuildBattleBossInfo::BossInfo &boss_info = cmd.boss_list[cmd.count++];
		boss_info.boss_id = iter->second.boss_id;
		boss_info.replace_boss_id =	iter->second.replace_boss_id;
		boss_info.status = iter->second.boss_status;
		boss_info.next_refresh_time = iter->second.next_refresh_time;
	}

	int len = sizeof(Protocol::SCCrossGuildBattleBossInfo) - sizeof(Protocol::SCCrossGuildBattleBossInfo::BossInfo) * (Protocol::SCCrossGuildBattleBossInfo::MAX_BOSS_COUNT - cmd.count);
	if (len < 0) return;

	
	InternalComm::Instance().UserProtocalTransferToGame(uuid, (const char *)&cmd, len);
}

void SpecialCrossGuildBattle::NotifyWarning(int warning_type, UniqueUserID guild_id, int monster_id, int percent)
{
// 	if (INVALID_UNIQUE_USER_ID == guild_id)
// 	{
// 		return;
// 	}

	int length = 0;
	if (warning_type == WARNING_TPYE_ATTACK)
	{
		//length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_cross_guildbattle_on_attack_warning, param_0);
		length = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), "on_attack_warning_guild", monster_id);
	}
	else if (warning_type == WARNING_TPYE_HP)
	{
		//length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_cross_guildbattle_on_flag_hp_warning, param_0);
		length = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), "monster_hp_warning",monster_id, percent);
	}

	if (length > 0)
	{
		int sendlen = 0;
		static Protocol::SCSystemMsg sm;

		if (SceneManager::CreateSystemMsg(&sm, &sendlen, gstr::GSTR_BUF, length, SYS_MSG_ACTIVITY_SPECIAL))
		{
			if (warning_type == WARNING_TPYE_ATTACK)
			{
				Scene::RoleFilterFunc filter_func = [](Role *role, void *param)->bool
				{
					if (nullptr != param)
					{
						UniqueUserID *unique_guild_id = (UniqueUserID *)param;

						int plat_type = role->GetRoleCross()->GetCrossOriginalUniqueUserID().plat_type;
						return UniqueUserID(plat_type, IntToUid(role->GetGuildID())) == *unique_guild_id;
					}

					return false;
				};

				int scene_id_list[CROSS_GUILDBATTLE_MAX_SCENE_NUM] = { 0 };
				CrossGuildBattleConfig::Instance().GetGuildBattleSceneIdList(scene_id_list);
				for (int i = 0; i < CROSS_GUILDBATTLE_MAX_SCENE_NUM; ++i)
				{
					Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(scene_id_list[i], CROSS_PUBLIC_SCENE_KEY);
					if (nullptr != scene)
					{
						scene->SendToRole((const char *)&sm, sendlen, filter_func, (void *)&guild_id);
					}
				}
			}
			else if (warning_type == WARNING_TPYE_HP)
			{
				m_scene->SendToRole((const char *)&sm, sendlen);
			}
		}
	}
}

void SpecialCrossGuildBattle::OnShouHuBossDie(ShouHuBossInfo *boss_info)
{
	if (NULL == boss_info) return;

	if ( SHOUHU_BOSSSTATUS_NONEXIST == boss_info->boss_status)
	{
		const CrossGuildBattleGather * gather_cfg = CrossGuildBattleConfig::Instance().GetGatherCfgByBossId(boss_info->boss_id);
		if (nullptr != gather_cfg)
		{
			GatherObj * gather_obj = m_scene->GetGatherObj(boss_info->gather_obj);
			if (NULL != gather_obj && gather_obj->GatherId() == gather_cfg->gather_id)
			{
				m_scene->DeleteObj(boss_info->gather_obj);
			}

			boss_info->gather_obj = INVALID_OBJ_ID;
			boss_info->gather_list.clear();
			boss_info->gather_guild_uuid = boss_info->guild_uuid;

			gather_obj = new GatherObj();
			gather_obj->Init(NULL, gather_cfg->gather_id, gather_cfg->gather_time * 1000, 0, true,gather_cfg->disappear_s, gather_cfg->disappear_gather_max);
			gather_obj->SetPos(boss_info->born_pos);
			GuildMap::iterator it = m_guild_map.find(boss_info->gather_guild_uuid);
			if (it != m_guild_map.end())
			{
				gather_obj->SetSpecialGatherType(SPEICAL_GATHER_TYPE_CROSS_GUILD_BATTLE_BOSS);
				gather_obj->InitCrossGuildBattleBoss(it->second.guild_name);
			}
			m_scene->AddObj(gather_obj);
			boss_info->gather_obj = gather_obj->GetId();
		}
	}
}

void SpecialCrossGuildBattle::CheckAllBossBound()
{
	for (auto it = m_shouhu_boss_map.begin(); it != m_shouhu_boss_map.end();++it)
	{
		CheckSingleBossBound(it->first);
	}
}

void SpecialCrossGuildBattle::CheckSingleBossBound(int monster_id)
{
	auto mitr = m_shouhu_boss_map.find(monster_id);
	if (m_shouhu_boss_map.end() == mitr || mitr->second.boss_status == SHOUHU_BOSSSTATUS_NONEXIST)
	{
		return;
	}

	const CrossGuildBattleOtherCfg &other_cfg = CrossGuildBattleConfig::Instance().GetOtherCfg();

	auto guild_hurt_it = mitr->second.guild_hurt_map.begin();
	for (; guild_hurt_it != mitr->second.guild_hurt_map.end();)
	{
		GuildHurt &guild_hurt = guild_hurt_it->second;
		std::map<long long, long long>::iterator uuid_it = guild_hurt.uuid_map.begin();
		for (; uuid_it != guild_hurt.uuid_map.end();)
		{
			UniqueUserID uuid = LongLongToUniqueUserID(uuid_it->first);
			Role *temp_role = m_scene->GetRoleByUUID(uuid);
			if (NULL == temp_role)
			{
				uuid_it++;
				continue;
			}

			if ((temp_role->GetPos() - mitr->second.born_pos).IsLongerThan(other_cfg.effective_range)) // 不在boss范围内
			{
				guild_hurt.uuid_map.erase(uuid_it++);
			}
			else
			{
				uuid_it++;
			}
		}

		if (guild_hurt.logout_timestamp > 0 && (guild_hurt.logout_timestamp + CORSS_GUILD_BATTLE_OWNER_TIME_S) <= EngineAdapter::Instance().Time())
		{
			// 掉线时间已经超时，就清掉
			guild_hurt.uuid_map.clear();
			//printf("\n Clear guild_hurt timeout... \n");
		}
		else if (guild_hurt.logout_timestamp > 0)
		{
			// 掉线没有超时， 保留
			guild_hurt_it++;
			continue;
		}

		if (guild_hurt.uuid_map.size() <= 0)
		{
			// 清掉该归属
			if (mitr->second.guild_uuid == guild_hurt.guild_id)
			{
				mitr->second.guild_uuid = INVALID_UNIQUE_USER_ID;
				//printf("\n Clear owner  ... \n");
			}
			mitr->second.guild_hurt_map.erase(guild_hurt_it++);
		}
		else
		{
			guild_hurt_it++;
		}
	}

	//伤害排行 
	std::vector<GuildHurt*> hurt_vec;
	guild_hurt_it = mitr->second.guild_hurt_map.begin();
	for (; guild_hurt_it != mitr->second.guild_hurt_map.end(); ++guild_hurt_it)
	{
		hurt_vec.push_back(&guild_hurt_it->second);
	}

	std::sort(hurt_vec.begin(), hurt_vec.end(), [](const GuildHurt* hurt1, const GuildHurt* hurt2) {return hurt1->hurt > hurt2->hurt; });
	if (hurt_vec.size() > 0)
	{
		// 设置新归属或失去归属
		mitr->second.guild_uuid = hurt_vec[0]->guild_id;
		//发送伤害列表
		Protocol::SCCrossGuildBattleBossHurtInfo scbinfo;
		scbinfo.count = 0;
		scbinfo.boss_id = mitr->second.boss_id;
		for (unsigned int i = 0; i < hurt_vec.size() ; i++)
		{
			hurt_vec[i]->rank = i;
			if (scbinfo.count < Protocol::SCCrossGuildBattleBossHurtInfo::BOSS_HURT_INFO_RANK_MAX)
			{
				UniqueUserID guid = hurt_vec[i]->guild_id;
				auto guild_item = m_guild_map.find(guid);
				if (guild_item != m_guild_map.end())
				{
					scbinfo.info_list[scbinfo.count].hurt = hurt_vec[i]->hurt;
					scbinfo.info_list[scbinfo.count].guild_id = UidToInt(guid.user_id);
					F_STRNCPY(scbinfo.info_list[scbinfo.count].guild_name, guild_item->second.guild_name, sizeof(GuildName));
					scbinfo.count++;
				}
			}
		}


		const int MAX_GET_ROLE = 256;
		ObjID obj_list[MAX_GET_ROLE] = { 0 };
		int count = m_scene->GetZoneMatrix()->GetObjByCircleArea(mitr->second.born_pos,other_cfg.effective_range+2, obj_list, sizeof(obj_list) / sizeof(obj_list[0]));
		for (int i = 0; i < count && i < MAX_GET_ROLE; ++i)
		{
			Role * role = m_scene->GetPrincipal(obj_list[i]);
			if (nullptr != role)
			{
				scbinfo.own_guild_rank = hurt_vec.size();
				scbinfo.own_guild_hurt = 0;
				std::map<UniqueUserID, GuildHurt>::iterator hurt_it = mitr->second.guild_hurt_map.find(this->GetGuildUniqueUserId(role));
				if (hurt_it != mitr->second.guild_hurt_map.end())
				{
					scbinfo.own_guild_hurt = hurt_it->second.hurt;
					scbinfo.own_guild_rank = hurt_it->second.rank;
				}
				EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&scbinfo, sizeof(scbinfo));
			}
		}
	}
}

SpecialCrossGuildBattle::ShouHuBossInfo* SpecialCrossGuildBattle::GetShouHuBossInfo(int boss_id)
{
	for (auto itr = m_shouhu_boss_map.begin(); itr != m_shouhu_boss_map.end(); ++itr)
	{
		if (itr->second.replace_boss_id == boss_id)
		{
			return &itr->second;
		}
	}

	return NULL;
}

int SpecialCrossGuildBattle::GetSceneGuildRoleNum(UniqueUserID guild_id)
{
	GuildMap::iterator g_itr = m_guild_map.find(guild_id);

	if (g_itr != m_guild_map.end() && g_itr->second.role_num >= 0)
	{
		return g_itr->second.role_num;
	}

	return 0;
}

void SpecialCrossGuildBattle::SendMsgToSceneGuild(const char * msg, int len, UniqueUserID guild_id)
{
	Scene::RoleFilterFunc filter_func = [](Role *role, void *param)->bool
	{
		if (nullptr != param)
		{
			UniqueUserID *unique_guild_id = (UniqueUserID *)param;

			int plat_type = role->GetRoleCross()->GetCrossOriginalUniqueUserID().plat_type;
			return UniqueUserID(plat_type, IntToUid(role->GetGuildID())) == *unique_guild_id;
		}

		return false;
	};

	m_scene->SendToRole(msg, len, filter_func, (void*)&guild_id);
}

//////////////////////////////////////////////////////////////////////////
bool SpecialCrossGuildBattle::IsWorshipDuring()
{
	const WorshipOtherConfig *other_cfg = LOGIC_CONFIG->GetWorshipConfig().GetOtherConfig(CROSS_ACTIVITY_TYPE_CROSS_GUILDBATTLE);
	if (nullptr == other_cfg)
	{
		return false;
	}

	time_t now = EngineAdapter::Instance().Time();

	return m_worship_start_time > 0 && now > m_worship_start_time && now < m_worship_start_time + other_cfg->worship_time;
}

void SpecialCrossGuildBattle::OnWorship(Role* role)
{
	if (nullptr == role)
	{
		return;
	}

	if (!this->IsWorshipDuring())
	{
		return;
	}

	const WorshipOtherConfig *other_cfg = LOGIC_CONFIG->GetWorshipConfig().GetOtherConfig(CROSS_ACTIVITY_TYPE_CROSS_GUILDBATTLE);
	if (nullptr == other_cfg)
	{
		return;
	}

	if (nullptr == role->GetScene() || role->GetScene()->GetSceneID() != other_cfg->worship_scene_id)
	{
		return;
	}

	//if (std::abs(role->GetPos().x - other_cfg->worship_pos.x) > other_cfg->worship_range || std::abs(role->GetPos().y - other_cfg->worship_pos.y) > other_cfg->worship_range)
	//{
	//	return;
	//}

	WorshipItem &item = m_worship_user_map[role->GetUniqueUserID()];
	if (item.manual_worship_times >= other_cfg->worship_click_times)
	{
		gstr::SendError(role, "worship_baiye_count_not_enouth");
		return;
	}

	unsigned int now_second = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	if (item.next_can_manual_worship_timestamp > now_second)
	{
		return;
	}

	++item.manual_worship_times;
	item.next_can_manual_worship_timestamp = now_second + other_cfg->worship_click_cd;
	item.manual_worship_reward_timestamp = now_second + other_cfg->worship_gather_time;

	this->SendWorshipInfo(role);
	this->SyncWorshipStart(role);
}

void SpecialCrossGuildBattle::SendWorshipInfo(Role *role)
{
	if (nullptr == role) return;

	Protocol::SCRoleWorshipInfo rwi;

	WorshipItem &item = m_worship_user_map[role->GetUniqueUserID()];

	rwi.activity_type = CROSS_ACTIVITY_TYPE_CROSS_GUILDBATTLE;
	rwi.worship_times = item.manual_worship_times;
	rwi.next_worship_timestamp = item.next_can_manual_worship_timestamp;

	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&rwi, sizeof(rwi));
}

void SpecialCrossGuildBattle::SyncWorshipStart(Role *role)
{
	if (nullptr == role) return;

	Protocol::SCRoleWorshipStart rws;
	rws.role_obj = role->GetId();
	rws.resever_sh = 0;

	m_scene->SendToRole((const char*)&rws, sizeof(rws));
}

void SpecialCrossGuildBattle::CheckWorshipReward(time_t now_second)
{
	UInt32 now_sec = static_cast<UInt32>(now_second);

	// 膜拜奖励
	for (auto &it : m_worship_user_map)
	{
		if (0 == it.second.manual_worship_reward_timestamp || now_sec < it.second.manual_worship_reward_timestamp)
		{
			continue;
		}

		it.second.manual_worship_reward_timestamp = 0;

		Role *temp_role = m_scene->GetRoleByUUID(it.first);
		if (nullptr == temp_role)
		{
			continue;
		}

		// 给予奖励
		int reward_gold_bind = LOGIC_CONFIG->GetWorshipConfig().GetReardGoldBind();
		temp_role->GetKnapsack()->GetMoney()->AddGoldBind(reward_gold_bind, "SpecialCrossGuildBattle::CheckWorshipReward");

	}
}
