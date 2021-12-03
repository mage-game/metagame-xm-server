#include "specialguildbattlenew.hpp"

#include <math.h>
#include <string>
#include <algorithm>

#include "gameworld/gstr.h"
#include "other/event/eventhandler.hpp"
#include "obj/character/monster.h"
#include "obj/character/role.h"
#include "scene/scenemanager.h"
#include "scene/activityshadow/activityshadow.hpp"
#include "monster/monsterpool.h"
#include "item/itempool.h"
#include "world.h"
#include "gamelog.h"
#include "effect/skilleffect/effectbuff.hpp"
#include "effect/skilleffect/effecthurtdec.hpp"
#include "engineadapter.h"
#include "internalcomm.h"
#include "servercommon/errornum.h"
#include "servercommon/userprotocal/msgsystem.h"
#include "servercommon/string/gameworldstr.h"
#include "servercommon/servercommon.h"
#include "protocal/msgactivity.hpp"
#include "protocal/msgfight.h"
#include "gameworld/skill/skillpool.hpp"
#include "other/route/mailroute.hpp"
#include "global/rank/rankmanager.hpp"
#include "global/usercache/usercache.hpp"
#include "globalconfig/globalconfig.h"
#include "global/guild/guildmanager.hpp"
#include "global/guild/guild.hpp"
#include "global/worldstatus/worldstatus.hpp"
#include "global/randactivity/randactivitymanager.hpp"
#include "config/activityconfig/guildbattleconfignew.hpp"
#include "item/knapsack.h"
#include "other/shop/roleshop.hpp"
#include "other/specialappearance/specialappearance.hpp"
#include "other/rolegoal/rolegoal.hpp"
#include "config/logicconfigmanager.hpp"
#include "other/vip/vip.hpp"
#include "global/randactivity/impl/randactivityguildbattle.hpp"
#include "config/sharedconfig/sharedconfig.h"
#include "gameworld/global/activity/activitymsgmanager.h"
#include "gameworld/config/activitymsgconfig.hpp"
#include "gameworld/protocal/msgchatmsg.h"
#include "config/worshipconfig.hpp"


SpecialGuildBattleNew::SpecialGuildBattleNew(Scene *scene) :SpecialLogic(scene), m_is_finish(false), m_credit_is_change(false), 
	m_next_calc_time(0), m_last_first_guild_broadcast_time(0), m_last_add_guild_score_time(0), m_last_add_person_score_time(0), m_sync_global_info_time(0), 
	m_last_check_guild_person_rank(0), m_kick_out_all_role_time(0), m_worship_start_time(0)
{
}

SpecialGuildBattleNew::~SpecialGuildBattleNew()
{
	//清理数据
	{
		std::map<int, GuildItem *>::iterator iter;
		for (iter = m_guild_map.begin(); m_guild_map.end() != iter; ++iter)
		{
			delete iter->second;
		}
		m_guild_map.clear();
	}

	{
		std::map<int, JoinerItem *>::iterator iter;
		for (iter = m_joiner_map.begin(); m_joiner_map.end() != iter; ++iter)
		{
			delete iter->second;
		}
		m_joiner_map.clear();
	}

	{
		std::map<int, PointItem *>::iterator iter;
		for (iter = m_point_map.begin(); m_point_map.end() != iter; ++iter)
		{
			delete iter->second;
		}
		m_point_map.clear();
	}
}

struct FitParam 
{
	FitParam() : role_id(0), guild_id(0)
	{}

	int role_id;
	GuildID guild_id;
};

void SpecialGuildBattleNew::OnActivityStatusChange(int status)
{
	switch (status)
	{
	case ACTIVITY_STATUS_STANDY:
	{
		this->OnActivityStandby();
	}
	break;
	case ACTIVITY_STATUS_OPEN:
	{
		this->OnActivityStart();
	}
	break;

	case ACTIVITY_STATUS_CLOSE:
	{
		WorldStatus::Instance().OnActivityFinish(ACTIVITY_FIND_TYPE_GUILDBATTLE);
		this->OnActivityClose();
	}
	break;
	}
}

bool SpecialGuildBattleNew::CanDestroy()
{
	return m_is_finish;
}

void SpecialGuildBattleNew::Update(unsigned long interval, time_t now_second)
{
	SpecialLogic::Update(interval, now_second);

	if (m_is_finish)
	{
		if (m_kick_out_all_role_time > 0 && now_second >= m_kick_out_all_role_time)
		{
			m_kick_out_all_role_time = 0;
			this->DelayKickOutAllRole();
		}

		// 活动结束后，才开始计算膜拜
		this->CheckWorshipReward(now_second);
	}

	if (m_is_finish || !ActivityShadow::Instance().IsActivtyOpen(ACTIVITY_TYPE_GUILDBATTLE)) return;

	this->CheckBornPointBoss(now_second);
	this->AddGuildScore();
	this->AddPersonScore();
	//this->PointBossNotice();

	if (m_credit_is_change && (m_next_calc_time == 0 || static_cast<unsigned int>(now_second) >= m_next_calc_time))
	{
		m_credit_is_change = false;
		this->CalcGuildRank();
		m_next_calc_time = static_cast<unsigned int>(now_second) + LOGIC_CONFIG->GetGuildBattleConfigNew().GuildBattleGetOtherCfg().recalc_time;
	}

	if (m_luck.next_lucky_timestamp > 0 && now_second >= m_luck.next_lucky_timestamp)
	{
		m_luck.next_lucky_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + LOGIC_CONFIG->GetGuildBattleConfigNew().GuildBattleGetOtherCfg().lucky_interval;

		this->GaveLuckyItemToUser();
	}

	{
		// 传闻
		if (static_cast<unsigned int>(EngineAdapter::Instance().Time()) > m_last_first_guild_broadcast_time + FIRST_GUILD_INTERVAL)
		{
			m_last_first_guild_broadcast_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());

			if (m_guild_rank_vec.size() > 0)
			{
				GuildItem *guild_item = m_guild_rank_vec[0];
				if (NULL == guild_item) return;

				Guild *guild = GuildManager::Instance().GetGuild(guild_item->guild_id);
				if (NULL != guild)
				{
					int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_guildbattle_first_guild, guild->GetGuildName());
					if (length > 0)
					{
						this->NotifySystemMsg(gamestring::GAMESTRING_BUF, length, SYS_MSG_ACTIVE_NOTICE);
					}
				}

			}
		}
	}

	if (m_sync_global_info_time != 0 && now_second >= m_sync_global_info_time)
	{
		this->SyncGlobalInfoToAll();
		m_sync_global_info_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + 2;
	}

	if (now_second >= m_last_check_guild_person_rank)
	{
		this->CheckGuildPersonalRank();
	}

}

void SpecialGuildBattleNew::AddGuildScore()
{
	const GuildBattleOtherCfg &other_cfg = LOGIC_CONFIG->GetGuildBattleConfigNew().GuildBattleGetOtherCfg();
	if (static_cast<unsigned int>(EngineAdapter::Instance().Time()) > m_last_add_guild_score_time + other_cfg.get_score_interva)
	{
		for (int i = 0; i < GUILD_BATTLE_NEW_POINT_NUM; i++)
		{
			PointItem * point_item = this->GetPointItemByIndex(i);
			if (NULL != point_item && 0 != point_item->hold_guild_id)
			{
				GuildItem *guild_item = this->GetGuildItem(point_item->hold_guild_id);
				if (NULL == guild_item) continue;
				if (0 == i)
				{
					guild_item->guild_credit += other_cfg.big_point_guild_score;
				}
				else
				{
					guild_item->guild_credit += other_cfg.point_guild_score;
				}
				m_credit_is_change = true;
				guild_item->credit_change_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_last_add_guild_score_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
			}
		}
	}
}

void SpecialGuildBattleNew::AddPersonScore()
{
	const GuildBattleOtherCfg &other_cfg = LOGIC_CONFIG->GetGuildBattleConfigNew().GuildBattleGetOtherCfg();
	if (static_cast<unsigned int>(EngineAdapter::Instance().Time()) <= m_last_add_person_score_time + other_cfg.get_score_interva_person)
	{
		return;
	}
	
	for (int k = 0; k < static_cast<int>(m_scene->RoleNum()); ++k)
	{
		Role *role = m_scene->GetRoleByIndex(k);
		if (nullptr == role) continue;

		for (int i = 0; i < GUILD_BATTLE_NEW_POINT_NUM; i++)
		{
			PointItem * point_item = this->GetPointItemByIndex(i);
			if (nullptr == point_item) continue;

			Posi posi_diffrence = point_item->realive_pos - role->GetPos();	//算出x与x的差,y与y的差
			bool is_out_range = posi_diffrence.IsLongerThan(other_cfg.effective_distance_person);	//判断是否超出了加积分的距离
			if (true == is_out_range || role->GetGuildID() != point_item->hold_guild_id) continue;

			JoinerItem *joiner_item = this->GetJoinerItem(role->GetUID());
			if (NULL == joiner_item) continue;

			if (0 == i)
			{
				joiner_item->history_get_person_credit += other_cfg.big_point_personscore;
			}
			else
			{
				joiner_item->history_get_person_credit += other_cfg.point_person_score;
			}
			m_last_add_person_score_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
		}
	}
	
}

void SpecialGuildBattleNew::CheckBornPointBoss(time_t now_second)
{
	for (int i = 0; i < GUILD_BATTLE_NEW_POINT_NUM; i++)
	{
		PointItem * point_item = this->GetPointItemByIndex(i);
		if (NULL != point_item && !point_item->is_live)
		{
			Monster *monster = MONSTERPOOL->CreateMonster(point_item->boss_id, m_scene, point_item->realive_pos);
			if (NULL != monster)
			{
				point_item->is_live = true;
				point_item->blood = monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
				point_item->max_blood = monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
				point_item->boss_notice_lasttime = 0;
				point_item->has_be_hit_broadcast_flag = 0;
				point_item->low_hp_broadcast_flag = 0;

				this->SyncGlobalInfoToAll();
			}
		}
	}
}

void SpecialGuildBattleNew::OnRoleEnterScene(Role *role)
{
	if (NULL == role) return;

	if (m_is_finish || INVALID_GUILD_ID == role->GetGuildID()) //|| !ActivityShadow::Instance().IsActivtyOpen(ACTIVITY_TYPE_GUILDBATTLE))
	{
		this->DelayKickOutRole(role);
		return;
	}

	GuildItem *guild_item = this->GetGuildItem(role->GetGuildID());
	if (NULL == guild_item)
	{
		guild_item = new GuildItem();
		guild_item->guild_id = role->GetGuildID();
		guild_item->guild_credit = 0;
		guild_item->credit_change_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
		m_guild_map[guild_item->guild_id] = guild_item;
	}

	JoinerItem *joiner_item = this->GetJoinerItem(role->GetUID());
	if (NULL == joiner_item)
	{
		joiner_item = new JoinerItem();
		joiner_item->uid = role->GetUID();
		joiner_item->guild_id = role->GetGuildID();
		role->GetName(joiner_item->user_name);
		joiner_item->guild_post = role->GetGuildPost();

		m_joiner_map[joiner_item->uid] = joiner_item;
		guild_item->member_joiner_vec.push_back(joiner_item);
	}

	this->SyncGlobalInfo(role);
	this->SyncRoleInfo(role);
	this->SendLuckyInfoToUser(role);
	this->SyncGuildMvpInfo(role->GetGuildID(), role);

	EventHandler::Instance().OnEnterGuildBattle(role);
}

void SpecialGuildBattleNew::OnRoleLeaveScene(Role *role, bool is_logout)
{
	if (NULL != role)
	{
		role->GetSpecialAppearance()->SetFbAppearance(0, 0, 0, true);

		JoinerItem *joiner_item = this->GetJoinerItem(role->GetUID());
		if (NULL != joiner_item)
		{
			joiner_item->is_addhudun = false;
		}

		this->ClearSpeedEffect(role);
	}
}

void SpecialGuildBattleNew::OnCharacterDie(Character *dead, ObjID killer_id)
{
	if (NULL == dead || m_is_finish) return;

	const GuildBattleOtherCfg &other_cfg = LOGIC_CONFIG->GetGuildBattleConfigNew().GuildBattleGetOtherCfg();

	Role *killer = m_scene->GetPrincipal(killer_id);
	if (NULL == killer) return;

	// 死亡者为怪物
	if (Obj::OBJ_TYPE_MONSTER == dead->GetObjType() && NULL != killer && Obj::OBJ_TYPE_ROLE == killer->GetObjType())
	{
		Monster *monster = (Monster *)dead;

		GuildItem *guild_item = this->GetGuildItem(killer->GetGuildID());
		JoinerItem *joiner_killer = this->GetJoinerItem(killer->GetUID());
		if (NULL == guild_item || NULL == joiner_killer)
		{
			return;
		}

		++joiner_killer->kill_flags;

		PointItem * point_item = this->GetPointItemByPos(monster->GetPos());
		if (NULL == point_item) return;

		int old_guild_id = point_item->hold_guild_id;

		PointItem * point_item_big = this->GetPointItemByIndex(0);
		if (NULL == point_item_big) return;

		Guild *guild = GuildManager::Instance().GetGuild(guild_item->guild_id);
		if (NULL == guild) return;

		if (point_item->is_live)
		{
// 			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_guildbattle_kill_boss_notice, guild->GetGuildName());
// 			if (length > 0)
// 			{
// 				this->NotifySystemMsg(gamestring::GAMESTRING_BUF, length, SYS_MSG_ACTIVE_NOTICE);
// 			}

			// 不是大据点
			if (monster->GetPos() != point_item_big->realive_pos)
			{
				for (int i = 0; i < GUILD_BATTLE_NEW_POINT_NUM; i++)
				{
					PointItem * point_item = this->GetPointItemByIndex(i);
					if (NULL != point_item)
					{
						if (point_item->hold_guild_id == guild_item->guild_id && point_item->realive_pos != point_item_big->realive_pos)
						{
							point_item->hold_guild_id = 0;
						}
					}
				}
			}

			point_item->is_live = false;
			point_item->hold_guild_id = guild_item->guild_id;

			this->SyncGlobalInfoToAll();
			this->SyncRoleInfo(killer);
		}

		{
			int content_len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_guild_flag_be_occupied_notice,
				monster->GetMonsterId());

			int sendlen = 0;
			Protocol::SCSystemMsg sm;

			if (SceneManager::CreateSystemMsg(&sm, &sendlen, gamestring::GAMESTRING_BUF, content_len, SYS_MSG_ACTIVITY_SPECIAL))
			{
				m_scene->SendToRole((const char*)&sm, sendlen, [old_guild_id](Role *role)->bool {
					if (old_guild_id == role->GetGuildID())
					{
						return true;
					}

					return false;
				});
			}
		}
	}

	// 死亡者为玩家，击杀者击杀数量增加
	else if (Obj::OBJ_TYPE_ROLE == dead->GetObjType() && NULL != killer && Obj::OBJ_TYPE_ROLE == killer->GetObjType() && Obj::OBJ_TYPE_ROLE == killer->GetObjType())
	{
		Role *death = (Role *)dead;

		JoinerItem *joiner_killer = this->GetJoinerItem(killer->GetUID());
		JoinerItem *joiner_dead = this->GetJoinerItem(death->GetUID());

		if (NULL == joiner_killer || NULL == joiner_dead)
		{
			return;
		}

		for (std::map<UserID, time_t>::iterator it = joiner_dead->hurt_map.begin(); it != joiner_dead->hurt_map.end(); ++it)
		{
			if (it->second + ASSIST_LIMIT_TIME >= EngineAdapter::Instance().Time() && it->first != killer->GetUserId())
			{
				JoinerItem *joiner_assist = this->GetJoinerItem(UidToInt(it->first));
				if (NULL != joiner_assist)
				{
					joiner_assist->assist++;
					joiner_assist->history_get_person_credit += other_cfg.assist_score;
					Role * assist_role = m_scene->GetRoleByUID(joiner_assist->uid);
					if (NULL == assist_role) continue;
					this->SyncRoleInfo(assist_role);
					this->CheckPersonalScoreReward(assist_role);
					this->CheckLianZhanNotice(assist_role, death, true);
					
				}
			}
		}

		++joiner_killer->kill_role_num;
		++joiner_killer->lianzhan;
		joiner_killer->history_get_person_credit += other_cfg.kill_score;

		this->SyncRoleInfo(killer);
		this->CheckPersonalScoreReward(killer);
		this->CheckLianZhanNotice(killer, death);
		
		joiner_dead->lianzhan = 0;
		joiner_dead->hurt_map.clear();
		this->ClearSpeedEffect(death);

// 		{
// 			GuildOtherConfig &other_cfg = LOGIC_CONFIG->GetGuildConfig().GetGuildOtherConfig();
// 
// 			if (death->GetGuildPost() > GUILD_POST_CHENG_YUAN || death->GetVip()->GetVipLevel() > other_cfg.chengyuan_event_notice_vip_level)
// 			{
// 				int content_len = 0;
// 
// 				if (RandomNum(10000) < 5000)
// 				{
// 					content_len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_guild_officer_be_killed,
// 						death->GetGuildPost(), death->GetUID(), death->GetName(), death->GetCamp());
// 				}
// 				else
// 				{
// 					content_len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_guild_chengyuan_be_killed,
// 						death->GetGuildPost(),death->GetUID(), death->GetName(), death->GetCamp(), m_scene->GetSceneName(), death->GetPos().x, death->GetPos().y,
// 						m_scene->GetSceneID(), m_scene->GetSceneKey(), 0);
// 				}
// 
// 				if (content_len > 0)
// 				{
// 					int sendlen = 0;
// 					Protocol::SCSystemMsg sm;
// 
// 					if (SceneManager::CreateSystemMsg(&sm, &sendlen, gamestring::GAMESTRING_BUF, content_len, SYS_MSG_ONLY_CHAT_GUILD))
// 					{
// 						m_scene->SendToRole((const char*)&sm, sendlen, [death](Role *role)->bool {
// 							if (death->GetGuildID() == role->GetGuildID())
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

bool SpecialGuildBattleNew::SpecialRelive(Role *role, int realive_type)
{
	if (m_is_finish || NULL == role)
	{
		return false;
	}

	if (Protocol::CSRoleReAliveReq::REALIVE_TYPE_HERE_GOLD == realive_type)
	{
		return false;
	}

	Posi realive_pos = LOGIC_CONFIG->GetGuildBattleConfigNew().GetClosestRealivePos(role->GetPos());
	if (realive_pos.x == 0 || realive_pos.y == 0)
	{
		return true;
	}

	JoinerItem *joiner_item = this->GetJoinerItem(role->GetUID());
	if (NULL == joiner_item) return false;

	GuildItem *guild_item = this->GetGuildItem(role->GetGuildID());
	if (NULL == guild_item) return false;

	//不检查第0个据点，第0个据点不做复活点
	for (int i = 1; i < GUILD_BATTLE_NEW_POINT_NUM; i++)
	{
		PointItem * point_item = this->GetPointItemByIndex(i);
		if (NULL != point_item && point_item->hold_guild_id == guild_item->guild_id)
		{
			// 据点的坐标
			realive_pos = point_item->realive_pos;
			break;
		}
	}

	role->ReAlive(SPECIAL_ACTIVITY_FREE_REALIVE_HP_PER, 100, realive_pos);
	joiner_item->is_addhudun = 0;
	this->OnAddHuDun(role);

	return true;
}

bool SpecialGuildBattleNew::SpecialIsEnemy(Role *role, Obj *obj)
{
	if (m_is_finish)
	{
		return false;
	}

	if (NULL == role || NULL == obj || role == obj) return false;

	if (Obj::OBJ_TYPE_ROLE == obj->GetObjType())
	{
		Role *target = (Role *)obj;
		return role->GetGuildID() != target->GetGuildID();
	}

	if (Obj::OBJ_TYPE_MONSTER == obj->GetObjType())
	{
		Monster *target = (Monster *)obj;
		PointItem * point_item = this->GetPointItemByPos(target->GetPos());
		if(NULL != point_item)
		{ 
			if (point_item->hold_guild_id == role->GetGuildID())
			{
				return false;
			}
		}
	}

	return SpecialLogic::SpecialIsEnemy(role, obj);
}

void SpecialGuildBattleNew::OnAttackCalcHurt(Character *cha, ObjID enemy_id, Attribute &delta_hp)
{
	Obj *enemy_obj = m_scene->GetObj(enemy_id);
	if (NULL == cha || NULL == enemy_obj || Obj::OBJ_TYPE_MONSTER != enemy_obj->GetObjType()) return;

	Role *hurt_role = (Role *)cha;
	Monster *monster = (Monster *)enemy_obj;

	PointItem * point_item = this->GetPointItemByPos(monster->GetPos());
	if (NULL != point_item)
	{
		if (point_item->hold_guild_id == hurt_role->GetGuildID())
		{
			delta_hp = 0;
		}
	}
}

void SpecialGuildBattleNew::OnAttack(Character *cha, ObjID enemy_id, Attribute delta_hp)
{
	if (NULL == cha) return;

	Obj * attacker = m_scene->GetObj(enemy_id);
	Role * role = dynamic_cast<Role*>(attacker);

	if (Obj::OBJ_TYPE_MONSTER == cha->GetObjType())
	{
		Monster *monster = (Monster *)cha;
		PointItem * point_item = this->GetPointItemByPos(monster->GetPos());
		if (NULL != point_item)
		{
			if (0 != point_item->hold_guild_id && monster->IsAlive())
			{
				unsigned int now_second = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				unsigned int notify_cd = LOGIC_CONFIG->GetActivityMsgConfig().GetMonsterAttackNotifyCD(m_scene->GetSceneType(), monster->GetMonsterId());
				if (notify_cd > 0 && now_second >= point_item->last_warning_attack_time + notify_cd)
				{
					point_item->last_warning_attack_time = now_second;

					this->NotifyWarning(WARNING_TPYE_ATTACK, point_item->hold_guild_id, monster->GetMonsterId(), 0);
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
					//if (LOGIC_CONFIG->GetGuildBattleConfigNew().CheckWarning(old_percent, now_percent, &warning_percent))
					if (warning_percent >= 0)
					{
						this->NotifyWarning(WARNING_TPYE_HP, point_item->hold_guild_id, monster->GetMonsterId(), warning_percent);
					}
				}
			}

			//this->OnFlagMonsterBeAttacked(point_item, monster);

			if (NULL != role)
			{
				JoinerItem * joiner = this->GetJoinerItem(role->GetUID());
				if (joiner != NULL)
				{
					joiner->hurt_flags += -delta_hp;
				}
			}
		}
	}

	if (NULL != role && Obj::OBJ_TYPE_ROLE == cha->GetObjType())
	{
		JoinerItem * joiner = this->GetJoinerItem(role->GetUID());
		if (joiner != NULL)
		{
			joiner->hurt_roles += -delta_hp;
		}
	}
}

void SpecialGuildBattleNew::PointBossNotice()
{
	UInt32 monster_num = m_scene->MonsterNum();
	for (int i = 0; i < (int)monster_num; ++i)
	{
		Monster *monster = m_scene->GetMonsterByIndex(i);
		if (NULL == monster) continue;

		for (int i = 0; i < GUILD_BATTLE_NEW_POINT_NUM; i++)
		{
			PointItem * point_item = this->GetPointItemByIndex(i);
			if (NULL == point_item || 0 == point_item->hold_guild_id) continue;

			if (monster->GetMonsterId() != point_item->boss_id || monster->GetPos() != point_item->realive_pos) continue;

			double max_hp = (double)monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
			double cur_hp = (double)monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);

			if (cur_hp >= max_hp || max_hp <= 0 || 0 == cur_hp) continue;
			double cur_hp_pro = cur_hp / max_hp;
			if (cur_hp_pro <= 0.2 && point_item->is_live && (0 == point_item->boss_notice_lasttime || static_cast<int>(EngineAdapter::Instance().Time()) > point_item->boss_notice_lasttime + GUILD_BATTLE_NOTICE_INTERVAL))
			{
				point_item->boss_notice_lasttime = static_cast<int>(EngineAdapter::Instance().Time());

				Guild *guild = GuildManager::Instance().GetGuild(point_item->hold_guild_id);
				if (NULL == guild) return;
				
				int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_guildbattle_point_boss_blood_notice, guild->GetGuildName());
				if (length > 0)
				{
					this->NotifySystemMsg(gamestring::GAMESTRING_BUF, length, SYS_MSG_ACTIVE_NOTICE);
				}
			}
		}
	}
}

bool SpecialGuildBattleNew::OnSosCost(Protocol::CSSendGuildSosReq *req, Role *role)
{
	if (nullptr == role || nullptr == req)
	{
		return false;
	}

	JoinerItem *joiner = this->GetJoinerItem(role->GetUID());
	if (nullptr == joiner)
	{
		return false;
	}

	GuildID sos_guild_id = role->GetGuildID();

	int cost = LOGIC_CONFIG->GetGuildBattleConfigNew().GetSosCost(joiner->sos_times);
	if (cost > 0)
	{
		if (!role->GetKnapsack()->GetMoney()->UseAllGold(cost, "SpecialGuildBattleNew::OnSosCost"))
		{
			role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
			return false;
		}
	}
	
	++joiner->sos_times;

	static Protocol::SCGuildMemberSos gms;
	gms.sos_type = req->sos_type;
	gms.member_uid = UidToInt(role->GetUserId());
	role->GetName(gms.member_name);
	gms.member_scene_id = role->GetScene()->GetSceneID();
	gms.member_pos_x = role->GetPos().x;
	gms.member_pos_y = role->GetPos().y;
	gms.enemy_uid = 0;
	memset(gms.enemy_name, 0, sizeof(gms.enemy_name));
	gms.enemy_camp = 0;
	gms.enemy_guild_id = INVALID_GUILD_ID;
	memset(gms.enemy_guild_name, 0, sizeof(gms.enemy_guild_name));

// 	FitParam param;
// 	param.guild_id = sos_guild_id;
// 	param.role_id = role->GetUID();
// 
// 	Scene::RoleFilterFunc filter_func = [](Role *role, void *param)->bool
// 	{
// 		if (nullptr != param)
// 		{
// 			FitParam *fit_param = (FitParam *)param;
// 			return role->GetGuildID() == fit_param->guild_id && role->GetUID() != fit_param->role_id;
// 		}
// 
// 		return false;
// 	};
// 
// 	m_scene->SendToRole((const char *)&gms, sizeof(gms), filter_func, (void *)&param);

	Guild * guild = GuildManager::Instance().GetGuild(sos_guild_id);
	if (nullptr != guild)
	{
		guild->SendMessageToGuildAll(Protocol::GUILD_NOTIFY_TYPE_MEMBER_SOS, (const char*)&gms, sizeof(gms));
	}

	this->SyncRoleInfo(role);

	return true;
}

void SpecialGuildBattleNew::OnHurtOther(Character *cha, Character *target)
{
	if (NULL == cha || NULL == target)
	{
		return;
	}

	if (Obj::OBJ_TYPE_ROLE == cha->GetObjType() && Obj::OBJ_TYPE_ROLE == target->GetObjType())
	{
		Role *hurt_role = (Role *)cha;
		Role *target_role = (Role *)target;

		if (NULL == hurt_role || NULL == target_role)
		{
			return;
		}

		JoinerItem *target_joiner = this->GetJoinerItem(target_role->GetUID());
		if (NULL != target_joiner)
		{
			target_joiner->hurt_map[hurt_role->GetUserId()] = EngineAdapter::Instance().Time();
		}
	}
	else if (Obj::OBJ_TYPE_ROLE == cha->GetObjType() && Obj::OBJ_TYPE_MONSTER == target->GetObjType())
	{
		Role *hurt_role = (Role *)cha;
		Monster *monster = (Monster *)target;

		const GuildBattleOtherCfg &other_cfg = LOGIC_CONFIG->GetGuildBattleConfigNew().GuildBattleGetOtherCfg();

		JoinerItem *joiner_item = this->GetJoinerItem(hurt_role->GetUID());
		if (NULL == joiner_item) return;

		for (int i = 0; i < GUILD_BATTLE_NEW_POINT_NUM; i++)
		{
			PointItem * point_item = this->GetPointItemByIndex(i);
			if (NULL != point_item)
			{
				if (monster->GetMonsterId() == point_item->boss_id)
				{
					joiner_item->history_get_person_credit += other_cfg.hurt_point_score;
					this->SyncRoleInfo(hurt_role);
					this->CheckPersonalScoreReward(hurt_role);
					break;
				}
			}
		}

		PointItem * point_item = this->GetPointItemByPos(monster->GetPos());
		if (NULL != point_item)
		{
			point_item->blood = monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
		}
		
		this->SyncGlobalInfoToAll();
	}
}

SpecialGuildBattleNew::GuildItem * SpecialGuildBattleNew::GetGuildItem(GuildID guild_id)
{
	std::map<int, GuildItem *>::iterator iter = m_guild_map.find(guild_id);
	if (m_guild_map.end() == iter)
	{
		return NULL;
	}

	return iter->second;
}

SpecialGuildBattleNew::JoinerItem * SpecialGuildBattleNew::GetJoinerItem(int uid)
{
	std::map<int, JoinerItem *>::iterator iter = m_joiner_map.find(uid);
	if (m_joiner_map.end() == iter)
	{
		return NULL;
	}

	return iter->second;
}

SpecialGuildBattleNew::PointItem * SpecialGuildBattleNew::GetPointItemByPos(Posi pos)
{
	for (std::map<int, PointItem *>::iterator it = m_point_map.begin(); it != m_point_map.end(); it++)
	{
		PointItem * point_item = it->second;
		if (NULL != point_item)
		{
			if (point_item->realive_pos == pos)
			{
				return it->second;
			}
		}
	}
	return NULL;
}

SpecialGuildBattleNew::PointItem * SpecialGuildBattleNew::GetPointItemByIndex(int index)
{
	std::map<int, PointItem *>::iterator iter = m_point_map.find(index);
	if (m_point_map.end() == iter)
	{
		return NULL;
	}

	return iter->second;
}

void SpecialGuildBattleNew::OnAddHuDun(Role *role)
{
	if (NULL == role) return;

	JoinerItem *joiner_item = this->GetJoinerItem(role->GetUID());
	if (NULL == joiner_item) return;

	const GuildBattleOtherCfg &other_cfg = LOGIC_CONFIG->GetGuildBattleConfigNew().GuildBattleGetOtherCfg();

	if (joiner_item->is_addhudun)
	{
		role->NoticeNum(errornum::EN_GUILD_BATTLE_HAS_USE_SHILD);
		return;
	}

	EffectDecHurt *effect_inc_hurt = new EffectDecHurt(role->GetId(), 0, other_cfg.shield_times, 1, EffectBase::PRODUCT_METHOD_SCENE_SKILL);
	effect_inc_hurt->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_1);
	effect_inc_hurt->SetDecPercent(10000);
	role->AddEffect(effect_inc_hurt);

	joiner_item->is_addhudun = true;

	this->SyncRoleInfo(role);
}

void SpecialGuildBattleNew::OnActivityStandby()
{
	//清理数据
	{
		std::map<int, GuildItem *>::iterator iter;
		for (iter = m_guild_map.begin(); m_guild_map.end() != iter; ++iter)
		{
			delete iter->second;
		}
		m_guild_map.clear();
	}

	{
		std::map<int, JoinerItem *>::iterator iter;
		for (iter = m_joiner_map.begin(); m_joiner_map.end() != iter; ++iter)
		{
			delete iter->second;
		}
		m_joiner_map.clear();
	}

	{
		std::map<int, PointItem *>::iterator iter;
		for (iter = m_point_map.begin(); m_point_map.end() != iter; ++iter)
		{
			delete iter->second;
		}
		m_point_map.clear();
	}
	
	m_worship_start_time = 0;
	m_kick_out_all_role_time = 0;
	m_guild_rank_vec.clear();
	m_person_rank_vec.clear();
	m_worship_user_map.clear();
}

void SpecialGuildBattleNew::OnActivityStart()
{
	m_is_finish = false;
	m_credit_is_change = false;
	m_next_calc_time = 0;
	m_last_first_guild_broadcast_time = 0;
	m_worship_start_time = 0;
	m_worship_user_map.clear();

	m_luck.next_lucky_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + LOGIC_CONFIG->GetGuildBattleConfigNew().GuildBattleGetOtherCfg().lucky_interval;
	m_last_add_guild_score_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	m_last_add_person_score_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	m_sync_global_info_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	m_luck.luck_user_count = 0;

	//理论上开服活动不会重复开启，但还是清一下数据吧
	//WorldStatus::Instance().SetOpenServerGuildBattelID(0);
	//WorldStatus::Instance().OnSyncGBMenzhuInfo(0);

	for (int i = 0; i < GUILD_BATTLE_NEW_POINT_NUM; i++)
	{
		const GuildBattlePointCfg * point_cfg = LOGIC_CONFIG->GetGuildBattleConfigNew().GetGuildBattlePointCfg(i);
		if (NULL != point_cfg)
		{
			PointItem * point_item = new PointItem();

			point_item->hold_guild_id = 0;
			point_item->is_live = false;
			point_item->boss_id = point_cfg->boss_id;
			point_item->realive_pos = point_cfg->realive_pos;

			m_point_map[i] = point_item;
		}
	}
	this->SyncGlobalInfoToAll();
	this->SendLuckyInfoToUser();
}
void SpecialGuildBattleNew::OnActivityClose()
{
	if (m_is_finish) return;

	m_is_finish = true;

	int kick_out_time = 120; // 服务端多延迟两分钟
	const WorshipOtherConfig *other_cfg = LOGIC_CONFIG->GetWorshipConfig().GetOtherConfig(ACTIVITY_TYPE_GUILDBATTLE);
	if (other_cfg)
	{
		kick_out_time += other_cfg->worship_time;
	}
	m_kick_out_all_role_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + kick_out_time;

	m_worship_start_time = EngineAdapter::Instance().Time();

	this->CalcRewardOnActClose();
	this->SendGuildInfoStatisticAll();
	this->SendResultInGuildChat();

	//复活所有玩家，防止活动结束时玩家不能复活的问题，这里全补满血
	this->AllRoleReLive();
	this->KillAllMonster();
	//this->DelayKickOutAllRole();

	// 传闻，广播胜利的公会
	{
		if (!m_guild_rank_vec.empty())
		{
			GuildItem *guild_item = m_guild_rank_vec[0];
			if (NULL == guild_item) return;

			WorldStatus::Instance().OnSyncGBMenzhuInfo(guild_item->guild_id);

			Guild *guild = GuildManager::Instance().GetGuild(guild_item->guild_id);
			if (NULL != guild)
			{
				int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_guildbattle_close, guild->GetGuildName());
				if (length > 0) World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_ACTIVE_NOTICE);
			}
		}
		else
		{
			WorldStatus::Instance().OnSyncGBMenzhuInfo(INVALID_GUILD_ID);
		}
	}

	//江湖传闻
	int rank_num = 0;
	std::set<GuildID> guild_notice_set;
	guild_notice_set.clear();

	for (auto &iter : m_guild_rank_vec)
	{
		rank_num++;

		int temp_max_score = 0;
		Role *role = NULL;

		std::map<int, JoinerItem *>::iterator it;
		for (it = m_joiner_map.begin(); m_joiner_map.end() != it; ++it)
		{
			role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(it->second->uid));
			if (role == NULL) continue;

			if (it->second->guild_id == iter->guild_id)
			{
				if (it->second->history_get_person_credit > temp_max_score)
				{
					temp_max_score = it->second->history_get_person_credit;
				}
			}
		}

		if (role != NULL)
		{
			EventHandler::Instance().OnAddGuildRumorEvent(iter->guild_id, role->GetName(), NULL, GUILD_EVENT_TYPE_RUMOR_GUILD_QIANGQI_RESULT, rank_num);
			guild_notice_set.insert(iter->guild_id);
		}
	}

	m_guild_rank_vec.clear();
	m_person_rank_vec.clear();
	m_worship_user_map.clear();

	GuildRank *guild_rank = RankManager::Instance().GetGuildRank();
	if (NULL != guild_rank)
	{
		guild_rank->ClearRank(GUILD_RANK_TYPE_TERRITORYWAR);
	}
}

int SpecialGuildBattleNew::FindGuildRank(GuildID guild_id)
{
	if (0 >= guild_id)
	{
		return -1;
	}

	for (unsigned int i = 0; i < m_guild_rank_vec.size(); i++)
	{
		GuildItem * rank_item = m_guild_rank_vec[i];
		if (NULL == rank_item) return -1;

		if (rank_item->guild_id == guild_id)
		{
			return i + 1;
		}
	}

	return -1;
}

void SpecialGuildBattleNew::CalcRewardOnActClose()
{
	if (!m_is_finish)
	{
		return;
	}

	// 计算最后排名,根据记录的公会积分排一次，确保完全按照公会积分排名次
	this->CalcGuildRank();

	{
		//保存公会争霸排行前十的公会
		gglobalprotocal::SyncGuildRankInfo guild_rank_info;
		guild_rank_info.guild_rank_type = GUILD_RANK_TYPE_GUILDBATTLE;
		guild_rank_info.guild_rank_count = 0;

		const GuildBattleOtherCfg & other_cfg = LOGIC_CONFIG->GetGuildBattleConfigNew().GuildBattleGetOtherCfg();

		for (int i = 0; i < 10 && i < static_cast<int>(m_guild_rank_vec.size()) && guild_rank_info.guild_rank_count < gglobalprotocal::SyncGuildRankInfo::MAX_GUILD_RANK_NUM; i++)
		{
			GuildItem * rank_item = m_guild_rank_vec[i];
			if (NULL == rank_item) return;

			guild_rank_info.guild_rank_list[guild_rank_info.guild_rank_count].guild_id = rank_item->guild_id;
			guild_rank_info.guild_rank_list[guild_rank_info.guild_rank_count].rank_value = i + 1;
			guild_rank_info.guild_rank_count++;

			// 开服活动期间，第一次仙盟争霸开启时，取得第一仙盟的帮主获得称号(额外奖励)
			int open_day = GetDayIndex(SHAREDCONFIG->GetOpenServerConfig().GetServerRealStartTime(), EngineAdapter::Instance().Time()) + 1;
			//if (0 == WorldStatus::Instance().GetOpenServerGuildBattelID() && RandActivityManager::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_OPEN_SERVER_BATTLE) && 0 == i)
			if (0 == WorldStatus::Instance().GetOpenServerGuildBattelID() && 0 == i && open_day <= other_cfg.opengame_day)
			{
				Guild *guild = GuildManager::Instance().GetGuild(rank_item->guild_id);
				if (NULL != guild)
				{
					int tuanzhang_uid = guild->GetGuildMemberManager().GetTuanZhangUid();
					UserCacheNode *user_node = UserCacheManager::Instance().GetUserNode(tuanzhang_uid);
					if (NULL != user_node)
					{
						WorldStatus::Instance().SetOpenServerGuildBattelID(tuanzhang_uid);
						MailContentParam contentparam; contentparam.Reset();

						
						for (int item_count = 0; item_count < other_cfg.huizhang_extra_reward_item_count && item_count < MAX_ATTACHMENT_ITEM_NUM; item_count++)
						{
							if (other_cfg.huizhang_extra_reward[item_count].num > 0)
							{
								contentparam.AddItem(other_cfg.huizhang_extra_reward[item_count].item_id, other_cfg.huizhang_extra_reward[item_count].num, other_cfg.huizhang_extra_reward[item_count].is_bind);
								LOG_QUICK12(gamelog::g_log_quick, LOG_TYPE_ACTIVITY_GUILDBATTLE, user_node->uid, user_node->user_name, NULL, user_node->level, other_cfg.huizhang_extra_reward[item_count].item_id, NULL, NULL, other_cfg.huizhang_extra_reward[item_count].num, other_cfg.huizhang_extra_reward[item_count].is_bind, 0, 0);
							}
						}

						int length_1 = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_guildbattle_rankreward_open_server);
						if (length_1 > 0 && contentparam.item_count > 0)
						{
							MailRoute::MailToUser(IntToUid(tuanzhang_uid), SYSTEM_MAIL_REASON_INVALID, contentparam);
						}
					}
				}
			}
		}

		if (m_guild_rank_vec.size() > 0)
		{
			RankManager::Instance().SyncGuildRankInfo(&guild_rank_info);
		}

		GuildManager::Instance().ClearTerritoryWarRank();
		RankManager::Instance().Snapshot();

		if (!m_guild_rank_vec.empty())
		{
			GuildItem * rank_item = m_guild_rank_vec[0];
			if (NULL == rank_item) return;

			WorldStatus::Instance().OnSyncGBMenzhuInfo(rank_item->guild_id);
		}
		else
		{
			WorldStatus::Instance().OnSyncGBMenzhuInfo(INVALID_GUILD_ID);
		}

		for (int i = 0; i < 10 && i < static_cast<int>(m_guild_rank_vec.size()); ++i)
		{
			GuildItem * rank_item = m_guild_rank_vec[i];
			if (nullptr != rank_item)
			{
				Guild *guild = GuildManager::Instance().GetGuild(rank_item->guild_id);
				if (nullptr != guild)
				{
					guild->GetGuildMemberManager().SendGuildBattleDailyRewardInfo(i + 1);

					// 工资
					const GuildBattleRankRewardCfg *rank_reward_cfg = LOGIC_CONFIG->GetGuildBattleConfigNew().GetRankRewardCfg(i + 1);
					if (NULL != rank_reward_cfg)
					{
						guild->AddGuildGongzi(rank_reward_cfg->gongzi);

						int length = gstr::SysMsgContent(gamestring::GAMESTRING_BUF + sizeof(Protocol::CSChannelChat), sizeof(gamestring::GAMESTRING_BUF) - sizeof(Protocol::CSChannelChat), "guild_gongzi_guildbattle_new_chat",
							i + 1, rank_reward_cfg->gongzi);
						if (length > 0)
						{
							Protocol::CSChannelChat *cc = (Protocol::CSChannelChat *)gamestring::GAMESTRING_BUF;
							cc->channel_type = chatdef::CHANNEL_TYPE_GUILD_SYSTEM;
							cc->msg_length = length;

							guild->GetGuildMemberManager().GuildSystemChat(cc, cc->msg_length + sizeof(Protocol::CSChannelChat), chatdef::ORIGIN_TYPE_GUILD_SYSTEM_MSG);
						}
					}
				}
			}
		}
	}

	this->SyncGlobalInfoToAll();

	// 给不同排名发奖励,活动参与者才有奖励
	{
		std::map<int, JoinerItem *>::iterator iter;

		std::map<int, int> guild_id_map;		//用于发放集体奖励
		guild_id_map.clear();

		for (iter = m_joiner_map.begin(); m_joiner_map.end() != iter; ++iter)
		{
			Role *joimer_role = m_scene->GetRoleByUID(iter->second->uid);
			if (NULL != joimer_role)
			{
				this->SyncRoleInfo(joimer_role);
			}

			GuildItem *guild_item = this->GetGuildItem(iter->second->guild_id);
			//玩家个人积分少于1不能获得奖励
			if (1 > iter->second->history_get_person_credit || NULL == guild_item || guild_item->Invalid())
			{
				continue;
			}

			int guild_rank = this->FindGuildRank(iter->second->guild_id);
			const GuildBattleRankRewardCfg *rank_reward_cfg = LOGIC_CONFIG->GetGuildBattleConfigNew().GetRankRewardCfg(guild_rank);
			if (NULL != rank_reward_cfg)
			{
				//发送公会排名邮件奖励

				UserCacheNode *user_node = UserCacheManager::Instance().GetUserNode(iter->second->uid);
				if (NULL == user_node)
				{
					continue;
				}

				static MailContentParam contentparam; contentparam.Reset();

				contentparam.AddVirtualItem(0, MAIL_VIRTUAL_ITEM_GUILDGONGXIAN, rank_reward_cfg->banggong);
				contentparam.AddVirtualItem(1, MAIL_VIRTUAL_ITEM_CONVERTSHOP_SHENGWANG, rank_reward_cfg->shengwang);
				contentparam.AddVirtualItem(2, MAIL_VIRTUAL_ITEM_EXP, rank_reward_cfg->base_role_exp);

				for (int item_count = 0; item_count < rank_reward_cfg->reward_item_count && item_count < MAX_ATTACHMENT_ITEM_NUM; item_count++)
				{
					contentparam.AddItem(rank_reward_cfg->reward_item[item_count].item_id, rank_reward_cfg->reward_item[item_count].num, rank_reward_cfg->reward_item[item_count].is_bind);
					LOG_QUICK12(gamelog::g_log_quick, LOG_TYPE_ACTIVITY_GUILDBATTLE, user_node->uid, user_node->user_name, NULL, user_node->level, rank_reward_cfg->reward_item[item_count].item_id, NULL, NULL, rank_reward_cfg->reward_item[item_count].num, rank_reward_cfg->reward_item[item_count].is_bind, 0, 0);
				}

				int length_1 = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_guildbattle_rankreward_guild, guild_rank);

				if (length_1 > 0)
				{
					MailRoute::MailToUser(IntToUid(iter->second->uid), SYSTEM_MAIL_REASON_INVALID, contentparam);
				}

				LOG_QUICK12(gamelog::g_log_quick, LOG_TYPE_ACTIVITY_GUILDBATTLE, iter->second->uid, user_node->user_name, NULL, user_node->level, rank_reward_cfg->banggong, NULL, NULL, rank_reward_cfg->base_role_exp, 0, 0, 0);

				{//记录要给集体奖励的仙盟
					if (guild_id_map.find(iter->second->guild_id) == guild_id_map.end())
					{
						guild_id_map[iter->second->guild_id] = guild_rank;
					}
				}
			}
		}//for m_joiner_map

		for (auto iter : guild_id_map)
		{
			Guild *guild = GuildManager::Instance().GetGuild(iter.first);
			if (NULL != guild)
			{
				UserCacheNode *tuanzhang_user_node = UserCacheManager::Instance().GetUserNode(guild->GetTuanzhuangRoleId());
				if (NULL == tuanzhang_user_node)
				{
					continue;
				}

				//集体奖励（只发帮主）
				{
					static MailContentParam contentparam_collective_reward; contentparam_collective_reward.Reset();

					const GuildBattleRankRewardCfg *rank_reward_cfg = LOGIC_CONFIG->GetGuildBattleConfigNew().GetRankRewardCfg(iter.second);
					if (NULL != rank_reward_cfg)
					{
						for (int item_count = 0; item_count < rank_reward_cfg->collective_reward_item_count && item_count < MAX_ATTACHMENT_ITEM_NUM; item_count++)
						{
							if (rank_reward_cfg->collective_reward_item[item_count].item_id <= 0 || rank_reward_cfg->collective_reward_item[item_count].num <= 0)
							{
								break;
							}

							contentparam_collective_reward.AddItem(rank_reward_cfg->collective_reward_item[item_count].item_id, rank_reward_cfg->collective_reward_item[item_count].num, rank_reward_cfg->collective_reward_item[item_count].is_bind);
							LOG_QUICK12(gamelog::g_log_quick, LOG_TYPE_ACTIVITY_GUILDBATTLE, tuanzhang_user_node->uid, tuanzhang_user_node->user_name, NULL, tuanzhang_user_node->level, rank_reward_cfg->collective_reward_item[item_count].item_id, NULL, NULL, rank_reward_cfg->collective_reward_item[item_count].num, rank_reward_cfg->collective_reward_item[item_count].is_bind, 0, 0);

							if (item_count == 0)
							{
								int length = SNPRINTF(gamestring::GAMESTRING_BUF + sizeof(Protocol::CSChannelChat), sizeof(gamestring::GAMESTRING_BUF) - sizeof(Protocol::CSChannelChat),
									gamestring::g_guild_activity_team_reward_notice, ACTIVITY_TYPE_GUILDBATTLE, rank_reward_cfg->collective_reward_item[item_count].item_id,
									rank_reward_cfg->collective_reward_item[item_count].num);

								if (length > 0)
								{
									Protocol::CSChannelChat *cc = (Protocol::CSChannelChat *)(gamestring::GAMESTRING_BUF);
									cc->channel_type = chatdef::CHANNEL_TYPE_GUILD;
									cc->msg_length = length;

									guild->GetGuildMemberManager().GuildSystemChat(cc, cc->msg_length + sizeof(Protocol::CSChannelChat));
								}
							}
						}
					}

					int length_2 = SNPRINTF(contentparam_collective_reward.contenttxt, sizeof(contentparam_collective_reward.contenttxt), gamestring::g_guildbattle_collective_reward, iter.second);

					if (length_2 > 0)
					{
						MailRoute::MailToUser(IntToUid(tuanzhang_user_node->uid), SYSTEM_MAIL_REASON_INVALID, contentparam_collective_reward);
					}
				}
			}
		}
	}
	
}

bool SortFuncByScore(const SpecialGuildBattleNew::GuildItem *rankitem_1, const SpecialGuildBattleNew::GuildItem *rankitem_2)
{
	if (NULL == rankitem_1 || NULL == rankitem_2) return true;

	if (rankitem_1->is_hold_big_point != rankitem_2->is_hold_big_point)
	{
		return rankitem_1->is_hold_big_point > rankitem_2->is_hold_big_point;
	}

	if (rankitem_1->guild_credit != rankitem_2->guild_credit)
	{
		return rankitem_1->guild_credit > rankitem_2->guild_credit;
	}

	return rankitem_1->credit_change_time < rankitem_2->credit_change_time;
}

void SpecialGuildBattleNew::CalcGuildRank()
{
	if (m_is_finish || !ActivityShadow::Instance().IsActivtyOpen(ACTIVITY_TYPE_GUILDBATTLE)) return;

	GuildID old_guild = 0;
	if (m_guild_rank_vec.size()>0)
	{
		old_guild = m_guild_rank_vec[0]->guild_id;
	}

	m_guild_rank_vec.clear();
	

	PointItem * big_point_item = this->GetPointItemByIndex(0);
	if (NULL == big_point_item) return;

	for (std::map<int, GuildItem *>::iterator it = m_guild_map.begin(); it != m_guild_map.end(); it++)
	{
		GuildItem *info = it->second;
		if (info->guild_credit > 0)
		{
			info->is_hold_big_point = 0;
			if (info->guild_id == big_point_item->hold_guild_id && 0 != big_point_item->hold_guild_id)
			{
				info->is_hold_big_point = 1;
			}

			if (NULL != it->second)
			{
				m_guild_rank_vec.push_back(it->second);
			}
		}
	}

	if (!m_guild_rank_vec.empty())
	{
		std::sort(m_guild_rank_vec.begin(), m_guild_rank_vec.end(), &SortFuncByScore);
		this->SyncGlobalInfoToAll();

		//战场播报
		if (sizeof(GameName) == sizeof(GuildName)) 
		{
			if (old_guild != m_guild_rank_vec[0]->guild_id && old_guild != 0)
			{
				ActivityMsgManager::RankTopItem first;
				first.uid = m_guild_rank_vec[0]->guild_id;
				Guild *guild = GuildManager::Instance().GetGuild(m_guild_rank_vec[0]->guild_id);
				if (nullptr != guild)
				{
					F_STRNCPY(first.user_name, guild->GetGuildName(), sizeof(GameName));
					ActivityMsgManager::Instance().RankFirstMsg(m_scene, first);
				}
			}
		}

		if (ActivityMsgManager::Instance().IsNeedRankTopMsg(m_scene))
		{
			ActivityMsgManager::RankTopItem top[ACTIVITY_RANK_TOP_MSG_MAX];
			int count = 0;
			for (int i = 0; i< (int)m_guild_rank_vec.size() && count < ACTIVITY_RANK_TOP_MSG_MAX; ++i)
			{
				GuildID guild_id = m_guild_rank_vec[i]->guild_id;
				Guild *guild = GuildManager::Instance().GetGuild(guild_id);
				if (guild != nullptr)
				{
					top[count].uid = guild_id;
					F_STRNCPY(top[count].user_name, guild->GetGuildName(), sizeof(GameName));
					count++;
				}
			}
			ActivityMsgManager::Instance().RankTopMsg(m_scene, top, count);
		}
	}
}

void SpecialGuildBattleNew::SyncGlobalInfoToAll()
{
	for (int i = 0; i < (int)m_scene->RoleNum(); ++i)
	{
		Role *temp_role = m_scene->GetRoleByIndex(i);
		this->SyncGlobalInfo(temp_role);
		this->SyncRoleInfo(temp_role);
	}
}

void SpecialGuildBattleNew::SyncGlobalInfo(Role *role)
{
	if (NULL == role || INVALID_GUILD_ID == role->GetGuildID()) return;

	GuildItem *guild_item = this->GetGuildItem(role->GetGuildID());
	if (NULL == guild_item)
	{
		return;
	}
	
	static Protocol::SCGBGlobalInfoNew cmd;
	cmd.self_guild_score = guild_item->guild_credit;
	cmd.self_guild_rank = 0;
	cmd.is_finish = m_is_finish ? 1 : 0;
	cmd.reserve1 = 0;
	cmd.reserve2 = 0;
	cmd.rank_count = 0;

	for (int i = 0; i < GUILD_BATTLE_NEW_POINT_NUM; i++)
	{
		cmd.hold_point_guild_list[i].Reset();
	}

	for (int i = 0; i < Protocol::SCGBGlobalInfoNew::RANK_NUM; ++i)
	{
		cmd.rank_list[i].Reset();
	}

	int gild_size = static_cast<int>(m_guild_rank_vec.size());
	for (int i = 0; i < Protocol::SCGBGlobalInfoNew::RANK_NUM && i < gild_size; ++i)
	{
		Protocol::SCGBGlobalInfoNew::RankItem &rank_item = cmd.rank_list[i];

		GuildItem *guild_item = m_guild_rank_vec[i];
		if (NULL == guild_item) return;

		Guild *guild = GuildManager::Instance().GetGuild(guild_item->guild_id);
		if (NULL == guild) continue;

		rank_item.guild_id = guild_item->guild_id;
		rank_item.score = guild_item->guild_credit;
		guild->GetGuildName(rank_item.guild_name);
		rank_item.tuangzhang_uid = guild->GetTuanzhuangRoleId();

		++cmd.rank_count;

		if (guild_item->guild_id == role->GetGuildID())
		{
			cmd.self_guild_rank = cmd.rank_count;
		}
	}

	for (int i = 0; i < GUILD_BATTLE_NEW_POINT_NUM; i++)
	{
		PointItem * point_item = this->GetPointItemByIndex(i);
		if (NULL != point_item)
		{
			cmd.hold_point_guild_list[i].guild_id = point_item->hold_guild_id;
			Guild *guild = GuildManager::Instance().GetGuild(point_item->hold_guild_id);
			if (NULL != guild)
			{
				guild->GetGuildName(cmd.hold_point_guild_list[i].guild_name);
			}
			cmd.hold_point_guild_list[i].blood = point_item->blood;
			cmd.hold_point_guild_list[i].max_blood = point_item->max_blood;
		}
	}

	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&cmd, sizeof(cmd));
}


void SpecialGuildBattleNew::SyncRoleInfo(Role *role)
{
	if (NULL == role) return;

	GuildItem *guild_item = this->GetGuildItem(role->GetGuildID());
	if (NULL == guild_item)
	{
		return;
	}

	JoinerItem *joiner_item = this->GetJoinerItem(role->GetUID());
	if (NULL == joiner_item)
	{
		return;
	}

	static Protocol::SCGBRoleInfoNew cmd;

	cmd.kill_role_num = joiner_item->kill_role_num;
	cmd.history_get_person_credit = joiner_item->history_get_person_credit;
	cmd.is_add_hudun = joiner_item->is_addhudun ? 1 : 0;
	cmd.sos_times = joiner_item->sos_times;
	cmd.reserve2 = 0;

	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&cmd, sizeof(cmd));
}

void SpecialGuildBattleNew::NotifySystemMsg(char *str_buff, int str_len, int notice_type)
{
	if (NULL != str_buff && str_len > 0)
	{
		int sendlen = 0;
		static Protocol::SCSystemMsg sm;

		if (SceneManager::CreateSystemMsg(&sm, &sendlen, str_buff, str_len, notice_type))
		{
			m_scene->SendToRole((const char*)&sm, sendlen);
		}
	}
}

void SpecialGuildBattleNew::ClearSpeedEffect(Role *role)
{
	if (NULL == role)
	{
		return;
	}
	role->GetEffectList()->FindAndRemoveEffect(EffectBase::EFFECT_TYPE_MOVESPEED, EBT_CHIHUAN, role->GetId(), EffectBase::PRODUCT_METHOD_SCENE_SKILL);
}

void SpecialGuildBattleNew::CheckPersonalScoreReward(Role *role)
{
	if (NULL == role) return;

	JoinerItem * joiner = this->GetJoinerItem(role->GetUID());
	if (NULL == joiner) return;

	role->GetRoleGoal()->CheckBattleFieldCond(ACTIVITY_TYPE_GUILDBATTLE, joiner->history_get_person_credit);

	const GuildBattlePersonalRewardCfg * person_reward_cfg = LOGIC_CONFIG->GetGuildBattleConfigNew().GetPersonalRewardCfg(joiner->history_get_person_credit);
	if (NULL != person_reward_cfg)
	{
		for (int i = person_reward_cfg->reward_index; i > joiner->fetch_last_personal_reward_index && i > 0; i--)
		{
			if (joiner->IsFetchPersonalCreditReward(i))
			{
				continue;
			}

			const GuildBattlePersonalRewardCfg * reward = LOGIC_CONFIG->GetGuildBattleConfigNew().GetPersonalRewardCfgByIndex(i);
			if (NULL == reward)
			{
				continue;
			}

			role->GetKnapsack()->PutListOrMail(reward->reward_item, PUT_REASON_GUILD_BATTEL);
			role->AddGuildGongxian(reward->banggong, "GuildBattle");
			role->GetRoleShop()->AddChestShopShengwang(reward->shengwang, "GuildBattle");
			joiner->SetFetchPersonalRewardFlag(i);

			UNSTD_STATIC_CHECK(3 == MAX_ATTACHMENT_ITEM_NUM);

			LOG_QUICK12(gamelog::g_log_quick, LOG_TYPE_ACTIVITY_GUILDBATTLE, role->GetUID(), role->GetName(), NULL, role->GetLevel(), reward->reward_item[0].item_id, NULL, NULL, reward->reward_item[0].num, reward->reward_item[0].is_bind, 0, 0);
			LOG_QUICK12(gamelog::g_log_quick, LOG_TYPE_ACTIVITY_GUILDBATTLE, role->GetUID(), role->GetName(), NULL, role->GetLevel(), reward->reward_item[1].item_id, NULL, NULL, reward->reward_item[1].num, reward->reward_item[1].is_bind, 0, 0);
			LOG_QUICK12(gamelog::g_log_quick, LOG_TYPE_ACTIVITY_GUILDBATTLE, role->GetUID(), role->GetName(), NULL, role->GetLevel(), reward->reward_item[2].item_id, NULL, NULL, reward->reward_item[2].num, reward->reward_item[2].is_bind, 0, 0);
			LOG_QUICK12(gamelog::g_log_quick, LOG_TYPE_ACTIVITY_GUILDBATTLE, role->GetUID(), role->GetName(), NULL, role->GetLevel(), reward->banggong, NULL, NULL, 0, 0, 0, 0);
		}

		// 传闻
		if (joiner->fetch_last_personal_reward_index != person_reward_cfg->reward_index)
		{
			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_guildbattle_fetch_target_reward,
				role->GetUID(), role->GetName(), (int)role->GetCamp(), person_reward_cfg->require_credit_min);
			if (length > 0)
			{
				this->NotifySystemMsg(gamestring::GAMESTRING_BUF, length, SYS_MSG_ACTIVE_NOTICE);
			}
		}

		joiner->fetch_last_personal_reward_index = person_reward_cfg->reward_index;
	}
}

void SpecialGuildBattleNew::SendLuckyInfoToUser(Role *role)
{
	if (NULL == role)
	{
		int sendlen = sizeof(m_luck) - (ZHANCHANG_LUACY_ROLE_MAX - m_luck.luck_user_count) * sizeof(m_luck.luck_user_namelist[0]);
		m_scene->SendToRole((const char*)&m_luck, sendlen);
	}
	else
	{
		int sendlen = sizeof(m_luck) - (ZHANCHANG_LUACY_ROLE_MAX - m_luck.luck_user_count) * sizeof(m_luck.luck_user_namelist[0]);
		if (sendlen > 0 && sendlen <= static_cast<int>(sizeof(m_luck)))
		{
			EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&m_luck, sendlen);
		}
	}
}

void SpecialGuildBattleNew::GaveLuckyItemToUser()
{
	int luck_people_num = LOGIC_CONFIG->GetGuildBattleConfigNew().GuildBattleGetOtherCfg().luck_people_num;

	int lucky_role_count = 0;
	int lucky_role_idx_list[ZHANCHANG_LUACY_ROLE_MAX] = { 0 };

	{
		std::vector<int> idx_vec;

		int max_rolenum = m_scene->RoleNum();
		if (max_rolenum > 0)
		{
			idx_vec.reserve(max_rolenum);

			for (int i = 0; i < max_rolenum; i++)
			{
				idx_vec.push_back(i);
			}

			for (int i = 0; i < max_rolenum; i++)
			{
				int rand_num = RandomNum(max_rolenum);
				int temp = idx_vec[i];
				idx_vec[i] = idx_vec[rand_num];
				idx_vec[rand_num] = temp;
			}

			for (int i = 0; i < max_rolenum && i < luck_people_num && i < ZHANCHANG_LUACY_ROLE_MAX; i++)
			{
				lucky_role_idx_list[lucky_role_count++] = idx_vec[i];
			}
		}
	}

	m_luck.luck_user_count = 0;

	char str_format[256] = { 0 };
	int max_length = sizeof(str_format) / sizeof(str_format[0]);
	int str_length = 0;

	const ItemConfigData &lucky_item = LOGIC_CONFIG->GetGuildBattleConfigNew().GuildBattleGetOtherCfg().lucky_item;

	for (int i = 0; i < lucky_role_count && i < ZHANCHANG_LUACY_ROLE_MAX; i++)
	{
		int role_idx = lucky_role_idx_list[i];
		if (role_idx < (int)m_scene->RoleNum())
		{
			Role *lucky_role = m_scene->GetRoleByIndex(role_idx);
			if (NULL != lucky_role)
			{
				const ItemBase *itembase = ITEMPOOL->GetItem(lucky_item.item_id);
				if (NULL != itembase)
				{
					lucky_role->GetKnapsack()->PutOrMail(lucky_item, PUT_REASON_GHZB_LUCK_REWARD);
				}

				if (m_luck.luck_user_count < ZHANCHANG_LUACY_ROLE_MAX)
				{
					lucky_role->GetName(m_luck.luck_user_namelist[m_luck.luck_user_count]);
					++m_luck.luck_user_count;
				}

				if (i < 5)	// 最多打包5个名字 不用ZHANCHANG_LUACY_ROLE_MAX 防止改动会导致字符串不够
				{
					int temp_length = SNPRINTF(str_format + str_length, max_length - str_length, "{r;%d;%s;%d} ",
						UidToInt(lucky_role->GetUserId()), lucky_role->GetName(), lucky_role->GetCamp());
					str_length += temp_length;
				}
			}
		}
	}

	this->SendLuckyInfoToUser();

	{
		int str_len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_wuxingguaji_lucky_notice,
			str_format);
		if (str_len > 0)
		{
			this->NotifySystemMsg(gamestring::GAMESTRING_BUF, str_len, SYS_MSG_ACTIVE_NOTICE);
		}
	}
}

int SpecialGuildBattleNew::GetBestKiller()
{
	int best_killer_id = -1;
	int best_killer_num = -1;
	for (std::map<int, JoinerItem *>::iterator it = m_joiner_map.begin(); it != m_joiner_map.end(); it++)
	{
		if (NULL != it->second)
		{
			if (it->second->kill_role_num > best_killer_num)
			{
				best_killer_id = it->second->uid;
				best_killer_num = it->second->kill_role_num;
			}
		}
	}

	return best_killer_id;
}

void SpecialGuildBattleNew::CheckLianZhanNotice(Role *kill_role, Role *dead_role, bool is_assist)
{
	return ;
	if (NULL != kill_role && NULL != dead_role)
	{
		JoinerItem *killer = this->GetJoinerItem(kill_role->GetUID());
		JoinerItem *dead = this->GetJoinerItem(dead_role->GetUID());
		if (NULL == killer || NULL == dead)
		{
			return;
		}

		if (!is_assist && dead->lianzhan >= 3)
		{
			int str_len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_pvpzhanchang_stoplianzhan_notice,
				UidToInt(kill_role->GetUserId()), kill_role->GetName(), (int)kill_role->GetCamp(),
				UidToInt(dead_role->GetUserId()), dead_role->GetName(), (int)dead_role->GetCamp(), dead->lianzhan);
			if (str_len > 0)
			{
				this->NotifySystemMsg(gamestring::GAMESTRING_BUF, str_len, SYS_MSG_ACTIVE_NOTICE);
			}
		}

		if (killer->lianzhan > 0)
		{
			int str_len = 0;
			int notice_type = SYS_MSG_ACTIVE_NOTICE;

			if (0 == killer->lianzhan % 3)
			{
				str_len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_pvpzhanchang_lianzhan_notice,
					UidToInt(kill_role->GetUserId()), kill_role->GetName(), (int)kill_role->GetCamp(),
					killer->lianzhan, killer->lianzhan);
			}

			if (str_len > 0) this->NotifySystemMsg(gamestring::GAMESTRING_BUF, str_len, notice_type);
		}
	}
}

void SpecialGuildBattleNew::NotifyWarning(int warning_type, int guild_id, int monster_id, int warning_percent)
{
// 	if (0 == guild_id)
// 	{
// 		return;
// 	}

	int length = 0;
	if (warning_type == WARNING_TPYE_ATTACK)
	{
		//length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_guildbattle_on_attack_warning);
		length = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), "on_attack_warning_guild", monster_id);
	}
	else if(warning_type == WARNING_TPYE_HP)
	{
		//length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_guildbattle_on_flag_hp_warning, warning_percent);
		length = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), "monster_hp_warning", monster_id, warning_percent);
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
						GuildID *guild = (GuildID *)param;
						return role->GetGuildID() == *guild;
					}

					return false;
				};
				m_scene->SendToRole((const char *)&sm, sendlen, filter_func, (void *)&guild_id);
			}
			else if (warning_type == WARNING_TPYE_HP)
			{
				m_scene->SendToRole((const char *)&sm, sendlen);
			}
		}
	}
}

void SpecialGuildBattleNew::OnFlagMonsterBeAttacked(PointItem *point_item, Monster *monster)
{
	if (nullptr == point_item || nullptr == monster)
	{
		return;
	}

	if (0 == point_item->has_be_hit_broadcast_flag)
	{
		point_item->has_be_hit_broadcast_flag = 1;

		int content_len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_guild_flag_first_be_hit_notice,
			monster->GetMonsterId());

		int sendlen = 0;
		Protocol::SCSystemMsg sm;

		int guild_id = point_item->hold_guild_id;

		if (SceneManager::CreateSystemMsg(&sm, &sendlen, gamestring::GAMESTRING_BUF, content_len, SYS_MSG_ONLY_CHAT_GUILD))
		{
			m_scene->SendToRole((const char*)&sm, sendlen, [guild_id](Role *role)->bool {
				if (guild_id == role->GetGuildID())
				{
					return true;
				}

				return false;
			});
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
			if (hp_per < low_hp_per_list[i] && 0 == (point_item->low_hp_broadcast_flag & (1 << i)))
			{
				point_item->low_hp_broadcast_flag |= 1 << i;

				int content_len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_guild_flag_be_attacked_notice,
					monster->GetMonsterId(), low_hp_per_list[i] / 100);

				int sendlen = 0;
				Protocol::SCSystemMsg sm;

				int guild_id = point_item->hold_guild_id;

				if (SceneManager::CreateSystemMsg(&sm, &sendlen, gamestring::GAMESTRING_BUF, content_len, SYS_MSG_ONLY_CHAT_GUILD))
				{
					m_scene->SendToRole((const char*)&sm, sendlen, [guild_id](Role *role)->bool {
						if (guild_id == role->GetGuildID())
						{
							return true;
						}

						return false;
					});
				}

				break;
			}
		}
	}
}

bool SortFuncByCredit(const SpecialGuildBattleNew::JoinerItem * item1, const SpecialGuildBattleNew::JoinerItem * item2)
{
	if (NULL == item1 || NULL == item2) return true;

	return item1->history_get_person_credit > item2->history_get_person_credit;
}

void SpecialGuildBattleNew::CheckGuildPersonalRank()
{
	m_last_check_guild_person_rank = static_cast<unsigned int>(EngineAdapter::Instance().Time())+1 ;

	for (std::map<int, GuildItem*>::iterator gd_it = m_guild_map.begin(); gd_it != m_guild_map.end(); ++gd_it)
	{
		if (gd_it->second->member_joiner_vec.size() > 0)
		{
			int old_mvp = gd_it->second->member_joiner_vec[0]->uid;
			std::sort(gd_it->second->member_joiner_vec.begin(), gd_it->second->member_joiner_vec.end(), &SortFuncByCredit);
			if (old_mvp != gd_it->second->member_joiner_vec[0]->uid)
			{
				this->SyncGuildMvpInfo(gd_it->first);
				
			}
		}
	}
}

void SpecialGuildBattleNew::SendGuildInfoStatisticAll()
{
	this->CheckGuildPersonalRank();

	for (std::map<int, GuildItem*>::iterator gd_it = m_guild_map.begin(); gd_it != m_guild_map.end(); ++gd_it)
	{
		this->SendGuildInfoStatistic(gd_it->first, Protocol::SCGuildInfoStatistic::NOTIFY_ACTIVITY_CLOSE);
	}
}

void SpecialGuildBattleNew::SendGuildInfoStatistic(long long guild_id, int notify, Role *role)
{
	Protocol::SCGuildInfoStatistic sta;
	sta.notify_type = notify;
	sta.activity_type = ACTIVITY_TYPE_GUILDBATTLE;
	sta.guild_id =static_cast<int>(guild_id);
	sta.count = 0;
	std::map<int, GuildItem *>::iterator gd_it = m_guild_map.find(static_cast<int>(guild_id));
	if (gd_it != m_guild_map.end())
	{
		for (unsigned int i = 0; i < gd_it->second->member_joiner_vec.size() && sta.count < MAX_MEMBER_COUNT; ++i)
		{
			JoinerItem * joiner_item = gd_it->second->member_joiner_vec[i];
			if (NULL == joiner_item) continue;
			Protocol::SCGuildInfoStatistic::InfoItem & info_item = sta.info_list[sta.count];
			info_item.uid = joiner_item->uid;
			F_STRNCPY(info_item.user_name, joiner_item->user_name, sizeof(GameName));
			info_item.guild_post = joiner_item->guild_post;
			info_item.kill_role = joiner_item->kill_role_num;
			info_item.kill_target = joiner_item->kill_flags;
			info_item.hurt_roles = joiner_item->hurt_roles;
			info_item.hurt_targets = joiner_item->hurt_flags;
			sta.count++;
		}
	}

	int lenth = sizeof(sta) - sizeof(Protocol::SCGuildInfoStatistic::InfoItem)*(MAX_MEMBER_COUNT - sta.count);
	if (role != NULL)
	{
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&sta, lenth);
	}
	else
	{
		m_scene->SendToRole((const char *)&sta, lenth, [guild_id](Role *role)->bool {
			if (guild_id == role->GetGuildID())
				return true;
			return false;
		});
	}
}

void SpecialGuildBattleNew::SyncGuildMvpInfo(GuildID guild_id, Role *role)
{
	std::map<int, GuildItem*>::iterator gd_it = m_guild_map.find(guild_id);
	if(gd_it != m_guild_map.end())
	{
		GuildID guild_id = gd_it->first;
		Protocol::SCGuildMvpInfo mvp;
		mvp.activity_type = ACTIVITY_TYPE_GUILDBATTLE;
		mvp.uid = 0;
		memset(mvp.user_name, 0, sizeof(mvp.user_name));
		if (gd_it->second->member_joiner_vec.size() > 0)
		{
			mvp.uid = gd_it->second->member_joiner_vec[0]->uid;
			F_STRNCPY(mvp.user_name, gd_it->second->member_joiner_vec[0]->user_name, sizeof(GameName));
		}
		if (NULL != role)
		{
			EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&mvp, sizeof(mvp));
		}
		else
		{
			m_scene->SendToRole((const char *)&mvp, sizeof(mvp), [guild_id](Role * role)->bool {
				if (guild_id == role->GetGuildID())
					return true;
				return false;
			});
		}
	}
}

void SpecialGuildBattleNew::SendResultInGuildChat()
{
	// 内部不再排序，外部调用前保证已经是在排序后
	for (std::map<int, GuildItem*>::iterator gd_it = m_guild_map.begin(); gd_it != m_guild_map.end(); ++gd_it)
	{
		if (gd_it->second->member_joiner_vec.size() > 0)
		{
			int mvp_uid = gd_it->second->member_joiner_vec[0]->uid; // 排序后的mvp
			int kill_role_uid = 0;  // 最高击杀玩家者
			int kill_flag_uid = 0;  // 最高击杀旗帜者
			int hurt_role_uid = 0;  // 最高伤害玩家者
			int hurt_flag_uid = 0;  // 最高伤害旗帜者
			static GameName tmp_role_name[5]; memset(tmp_role_name, 0, sizeof(tmp_role_name));

			int hight_kill_role = 0;
			int hight_kill_flag = 0;
			long long hight_hurt_role = 0;
			long long hight_hurt_flag = 0;

			for (auto vec_it = gd_it->second->member_joiner_vec.begin(); vec_it != gd_it->second->member_joiner_vec.end(); ++vec_it)
			{
				const JoinerItem * item = *vec_it;
				if (NULL == item) continue;
				if (item->uid == 0) continue;

				if (item->kill_role_num > hight_kill_role) { kill_role_uid = item->uid;  hight_kill_role = item->kill_role_num; }
				if (item->kill_flags > hight_kill_flag) { kill_flag_uid = item->uid;  hight_kill_flag = item->kill_flags; }
				if (item->hurt_roles > hight_hurt_role) { hurt_role_uid = item->uid;  hight_hurt_role = item->hurt_roles; }
				if (item->hurt_flags > hight_hurt_flag) { hurt_flag_uid = item->uid;  hight_hurt_flag = item->hurt_flags; }
			}

			for (auto vec_it = gd_it->second->member_joiner_vec.begin(); vec_it != gd_it->second->member_joiner_vec.end(); ++vec_it)
			{
				const JoinerItem * item = *vec_it;
				if (NULL == item) continue;
				if (item->uid == 0) continue;

				if (item->uid == mvp_uid) { F_STRNCPY(tmp_role_name[0], item->user_name, sizeof(GameName)); }
				if (item->uid == kill_role_uid) { F_STRNCPY(tmp_role_name[1], item->user_name, sizeof(GameName)); }
				if (item->uid == kill_flag_uid) { F_STRNCPY(tmp_role_name[2], item->user_name, sizeof(GameName)); }
				if (item->uid == hurt_role_uid) { F_STRNCPY(tmp_role_name[3], item->user_name, sizeof(GameName)); }
				if (item->uid == hurt_flag_uid) { F_STRNCPY(tmp_role_name[4], item->user_name, sizeof(GameName)); }
			}

			// 发送聊天
			Guild *guild = GuildManager::Instance().GetGuild(gd_it->first);
			if (NULL != guild)
			{
				static char buff[1024]; memset(buff, 0, sizeof(buff));
				std::string sysmsg_buff;
				int msg_len = 0;

				if (mvp_uid > 0)
				{
					msg_len = gstr::SysMsgContent(buff, 1024, "guild_guildbattlenew_sysmsg_title", tmp_role_name[0]);
					if (msg_len > 0)
					{
						sysmsg_buff.append(buff);
					}
				}

				if (kill_flag_uid > 0)
				{
					msg_len = gstr::SysMsgContent(buff, 1024, "guild_guildbattlenew_sysmsg_killflag", tmp_role_name[2], hight_kill_flag);
					if (msg_len > 0)
					{
						sysmsg_buff.append(buff);
					}
				}

				if (kill_role_uid > 0)
				{
					msg_len = gstr::SysMsgContent(buff, 1024, "guild_guildbattlenew_sysmsg_killrole", tmp_role_name[1], hight_kill_role);
					if (msg_len > 0)
					{
						sysmsg_buff.append(buff);
					}
				}

				if (hurt_role_uid > 0)
				{
					msg_len = gstr::SysMsgContent(buff, 1024, "guild_guildbattlenew_sysmsg_hurtrole", tmp_role_name[3], hight_hurt_role);
					if (msg_len > 0)
					{
						sysmsg_buff.append(buff);
					}
				}

				//if (hurt_flag_uid > 0)
				//{
				//	msg_len = gstr::SysMsgContent(buff, 1024, "guild_guildbattlenew_sysmsg_hurtflag", tmp_role_name[4], hight_hurt_flag);
				//	if (msg_len > 0)
				//	{
				//		sysmsg_buff.append(buff);
				//	}
				//}

				//if ((int)sysmsg_buff.size() > 10 && sysmsg_buff.size() < sizeof(buff))
				//{
				//	F_STRNCPY(buff, sysmsg_buff.c_str(), (int)sysmsg_buff.size());
				//	buff[(int)sysmsg_buff.size() - 10] = '\0';  // 去掉最后一个换行
				//	guild->GuildSystemMsg((int)sysmsg_buff.size() - 10, buff);
				//}
				int sysmsg_len = sysmsg_buff.size() - 10;  // 去掉最后一个换行，换行字符串占用10个字节
				if (sysmsg_len > 0 && sysmsg_len < (int)sizeof(buff) - (int)sizeof(Protocol::CSChannelChat))
				{
					Protocol::CSChannelChat *cc = (Protocol::CSChannelChat *)buff;
					cc->channel_type = chatdef::CHANNEL_TYPE_GUILD;
					cc->msg_length = sysmsg_len;
					F_STRNCPY(buff + sizeof(Protocol::CSChannelChat), sysmsg_buff.c_str(), sysmsg_len + 1);

					guild->GetGuildMemberManager().GuildSystemChat(cc, cc->msg_length + sizeof(Protocol::CSChannelChat));
				}
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
bool SpecialGuildBattleNew::IsWorshipDuring()
{
	const WorshipOtherConfig *other_cfg = LOGIC_CONFIG->GetWorshipConfig().GetOtherConfig(ACTIVITY_TYPE_GUILDBATTLE);
	if (nullptr == other_cfg)
	{
		return false;
	}

	time_t now = EngineAdapter::Instance().Time();

	return m_worship_start_time > 0 && now > m_worship_start_time && now < m_worship_start_time + other_cfg->worship_time;
}

void SpecialGuildBattleNew::OnWorship(Role* role)
{
	if (nullptr == role)
	{
		return;
	}

	if (!this->IsWorshipDuring())
	{
		return;
	}

	const WorshipOtherConfig *other_cfg = LOGIC_CONFIG->GetWorshipConfig().GetOtherConfig(ACTIVITY_TYPE_GUILDBATTLE);
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

void SpecialGuildBattleNew::SendWorshipInfo(Role *role)
{
	if (nullptr == role) return;

	Protocol::SCRoleWorshipInfo rwi;

	WorshipItem &item = m_worship_user_map[role->GetUniqueUserID()];

	rwi.activity_type = ACTIVITY_TYPE_GUILDBATTLE;
	rwi.worship_times = item.manual_worship_times;
	rwi.next_worship_timestamp = item.next_can_manual_worship_timestamp;

	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&rwi, sizeof(rwi));
}

void SpecialGuildBattleNew::SyncWorshipStart(Role *role)
{
	if (nullptr == role) return;

	Protocol::SCRoleWorshipStart rws;
	rws.role_obj = role->GetId();
	rws.resever_sh = 0;

	m_scene->SendToRole((const char*)&rws, sizeof(rws));
}

void SpecialGuildBattleNew::CheckWorshipReward(time_t now_second)
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
		temp_role->GetKnapsack()->GetMoney()->AddGoldBind(reward_gold_bind, "SpecialGuildBattleNew::CheckWorshipReward");

	}
}
