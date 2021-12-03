#include "specialcorssbianjingzhidi.h"

#include <algorithm>

#include "obj/character/role.h"
#include "obj/character/monster.h"
#include "obj/otherobj/gatherobj.h"

#include "scene/activityshadow/activityshadow.hpp"

#include "protocal/msgactivity.hpp"
#include "protocal/msgfight.h"

#include "engineadapter.h"
#include "gamelog.h"

#include "servercommon/errornum.h"
#include "servercommon/noticenum.h"
#include "servercommon/string/gameworldstr.h"
#include "servercommon/userprotocal/msgsystem.h"

#include "servercommon/serverconfig/crossbianjingzhidiconfig.h"
#include "config/logicconfigmanager.hpp"
#include "globalconfig/globalconfig.h"

#include "monster/monsterpool.h"
#include "scene/worldshadow/worldshadow.hpp"
#include "scene/scenemanager.h"

#include "world.h"
#include "global/team/team.hpp"
#include "ai/monsterai/monsteraibase.hpp"

#include "global/worldstatus/worldstatus.hpp"
#include "item/knapsack.h"
#include "friend/teaminfo.hpp"
#include "other/event/eventhandler.hpp"
#include "other/magicalprecious/magicalpreciouscfg.hpp"
#include "other/magicalprecious/magicalprecious.hpp"
#include "other/itemlevelreward/itemlevelreward.hpp"
#include "gameworld/gameworld/internalcomm.h"
#include "protocal/msgscguild.hpp"
#include "other/crossgoal/crossgoal.hpp"

SpecialCrossBianJingZhiDi::SpecialCrossBianJingZhiDi(Scene *scene)
: SpecialLogic(scene), m_is_finish(true), m_next_reflush_boss_time(0), m_next_reflush_monster_time(0), m_reflush_notice_time(0), m_is_notice(false),
m_send_boss_info_time(0), m_monster_die_num(0), m_next_check_buff_time(0)
{
}

SpecialCrossBianJingZhiDi::~SpecialCrossBianJingZhiDi()
{

}

void SpecialCrossBianJingZhiDi::Update(unsigned long interval, time_t now_second)
{
	SpecialLogic::Update(interval, now_second);

	if (!CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}

	if (!ActivityShadow::Instance().IsCrossActivityOpen(CROSS_ACTIVITY_TYPE_CROSS_BIANJINGZHIDI))
	{
		return;
	}

	if (m_is_notice && now_second >= m_reflush_notice_time)
	{
		//BOSS刷新提示
		{
			int length = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), "cross_bjzd_reflush_notice", CROSS_BIANJINGZHIDI_REFLUSH_NOTICE_TIME);
			if (length > 0)
			{
				this->NotifySystemMsg(gstr::GSTR_BUF, length, SYS_MSG_ACTIVE_NOTICE);
			}
		}

		m_is_notice = false;
	}

	if (m_next_reflush_boss_time > 0 && now_second >= m_next_reflush_boss_time)
	{
		m_next_reflush_boss_time = now_second + CrossBianJingZhiDiConfig::Instance().GetCrossBianJingZhiDiOtherCfg().reflush_time_s;
		m_reflush_notice_time = m_next_reflush_boss_time - CROSS_BIANJINGZHIDI_REFLUSH_NOTICE_TIME;
		m_is_notice = true;
		this->CheckBornBoss(now_second);
		this->SyncCrossBianJingZhiDiBossInfoToCross();
	}

	this->CheckBornMonster(now_second);

	if (now_second >= m_send_boss_info_time)
	{
		m_send_boss_info_time = static_cast<unsigned int>(now_second) + 2;

		//检测是否超出BOSS归属权范围
		this->CheckAllBossBound();

		this->CheckSendBossInfo();
	}

	if (m_next_check_buff_time > 0 && now_second >= m_next_check_buff_time)
	{
		m_next_check_buff_time = static_cast<unsigned int>(now_second) + 1;
		this->CheckGatherBuffInfo();
	}
}

void SpecialCrossBianJingZhiDi::OnAddObj(Obj *obj)
{
	if (NULL == obj) return;

	if (Obj::OBJ_TYPE_ROLE == obj->GetObjType())
	{
		Role *role = (Role *)obj;

		if (role->GetGuildID() <= 0)  // 没有帮派直接T出
		{
			role->BackToOriginalServer();
			return;
		}

		UniqueUserID guild_id = this->GetGuildUniqueUserId(role);
		if (m_guild_map.end() == m_guild_map.find(guild_id))
		{
			SpecialCrossBianJingZhiDi::GuildItem guild_item;
			guild_item.guild_id = guild_id;
			role->GetGuildName(guild_item.guild_name);

			m_guild_map.insert(std::make_pair(guild_id, guild_item));
		}
	}
}

void SpecialCrossBianJingZhiDi::CheckBornBoss(time_t now_second)
{
	for (int boss_type = 0; boss_type < CROSS_BIANJINGZHIDI_MAX_BOSS_TYPE; ++boss_type)
	{
		BossItem &boss_item = m_boss_list[boss_type];
		if (boss_item.boss_live_flag) continue;  // 存活的BOSS不用刷新

		const CrossBianJingZhiDiBossCfg *boss_cfg = CrossBianJingZhiDiConfig::Instance().GetCrossBianJingZhiDiBossCfg(boss_type);
		if (nullptr == boss_cfg)
		{
			continue;
		}

		Posi born_pos;
		if (!CrossBianJingZhiDiConfig::Instance().GetRandomFlushPos(born_pos, boss_cfg->pos_id_start, boss_cfg->pos_id_len))
		{
			continue;
		}

		int boss_id = CrossBianJingZhiDiConfig::Instance().GetBoosIdByWorldLevel(WorldShadow::Instance().GetWorldLevel(), boss_type);
		if (-1 == boss_id) continue;

		Monster *boss_monster = MONSTERPOOL->CreateMonster(static_cast<UInt16>(boss_id), m_scene, born_pos, 0, 0, 0, WorldShadow::Instance().GetWorldLevel());
		if (NULL != boss_monster)
		{
			// 改为发一次
			//int length = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), "cross_bjzd_boss_come", boss_id);
			//if (length > 0)
			//{
			//	this->NotifySystemMsg(gstr::GSTR_BUF, length, SYS_MSG_ACTIVE_NOTICE);
			//}

			{
				// 清除采集物
				Obj *obj = m_scene->GetObj(boss_item.gather_obj);
				if (NULL != obj && Obj::OBJ_TYPE_GATHER == obj->GetObjType())
				{
					m_scene->DeleteObj(obj->GetId());
				}
			}

			{
				// 设置BOSS初始信息
				boss_item.BossReset();
				boss_item.boss_obj = boss_monster->GetId();
				boss_item.boss_id = boss_id;
				boss_item.born_pos = born_pos;
				boss_item.boss_live_flag = true;
				boss_item.gather_obj = INVALID_OBJ_ID;
			}
		}
	}

	int length = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), "cross_bjzd_boss_come");
	if (length > 0)
	{
		this->NotifySystemMsg(gstr::GSTR_BUF, length, SYS_MSG_ACTIVE_NOTICE);
	}

	this->SendBossInfo();
	this->SendCrossBianJingZhiDiInfoToAllUser();
}

void SpecialCrossBianJingZhiDi::CheckBornMonster(time_t now_second)
{
	return; // 屏蔽小怪刷新
	const CrossBianJingZhiDiMonsterCfg &monster_cfg = CrossBianJingZhiDiConfig::Instance().GetCrossBianJingZhiDiMonsterCfg();
	if (now_second < m_next_reflush_monster_time || m_monster_die_num > monster_cfg.count)
	{
		return;
	}

	Posi born_pos;

	for (int k = 0; k < m_monster_die_num; ++ k)
	{
		if (!CrossBianJingZhiDiConfig::Instance().GetRandomFlushPos(born_pos, monster_cfg.pos_id_start, monster_cfg.pos_id_len))
		{
			continue;
		}

		MONSTERPOOL->CreateMonster(static_cast<UInt16>(monster_cfg.monster_id), m_scene, born_pos, 0, 0, 0, WorldShadow::Instance().GetWorldLevel());
	}

	int length = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), "cross_bjzd_already_reflush_notice", monster_cfg.monster_id);
	if (length > 0)
	{
		this->NotifySystemMsg(gstr::GSTR_BUF, length, SYS_MSG_ACTIVE_NOTICE);
	}

	m_monster_die_num = 0;

	m_next_reflush_monster_time = EngineAdapter::Instance().Time() + monster_cfg.reflush_time_s;

	this->SendCrossBianJingZhiDiInfoToAllUser();
}

bool SpecialCrossBianJingZhiDi::SpecialIsEnemy(Role *role, Obj *obj)
{
	switch (obj->GetObjType())
	{
	case Obj::OBJ_TYPE_ROLE:
	{
		Role *other_role = (Role*)obj;
		if (other_role == role)
		{
			return false;
		}

		// 不能攻击队友
		if (role->GetTeamInfo()->InTeam() && other_role->GetTeamInfo()->InTeam() && role->GetTeamInfo()->GetTeamIndex() == other_role->GetTeamInfo()->GetTeamIndex())
		{
			return false;
		}

		// 不能攻击仙盟盟友
		UniqueUserID role_guilid = this->GetGuildUniqueUserId(role);
		UniqueUserID other_guild = this->GetGuildUniqueUserId(other_role);
		if (INVALID_UNIQUE_USER_ID != role_guilid && other_guild == role_guilid)
		{
			return false;
		}

		// 其他全是敌人
		return true;
	}
	break;
	}

	return SpecialLogic::SpecialIsEnemy(role, obj);
}

bool SpecialCrossBianJingZhiDi::SpecialIsFriend(Role *role, Obj *obj, bool check_alive /*= true*/)
{
	switch (obj->GetObjType())
	{
	case Obj::OBJ_TYPE_MONSTER:
		{
			return false;
		}
		break;

	case Obj::OBJ_TYPE_ROLE:
		{
			Role *other_role = (Role*)obj;
			UniqueUserID role_guilid = this->GetGuildUniqueUserId(role);
			UniqueUserID other_guild = this->GetGuildUniqueUserId(other_role);

			// 同一个队伍或者同一个公会
			if ((role->GetTeamInfo()->InTeam() && other_role->GetTeamInfo()->InTeam() && (role->GetTeamInfo()->GetTeamIndex() == other_role->GetTeamInfo()->GetTeamIndex())) || 
				(INVALID_UNIQUE_USER_ID != role_guilid && other_guild == role_guilid))
			{
				return true;
			}
		}
		break;
	}

	return false;
}

void SpecialCrossBianJingZhiDi::OnAttack(Character *cha, ObjID enemy_id, Attribute delta_hp)
{
	if (NULL == cha || INVALID_OBJ_ID == enemy_id) return;
	if (delta_hp >= 0) return;

	Obj *attacker = m_scene->GetObj(enemy_id);
	if (NULL == attacker || !attacker->IsCharactor(attacker->GetObjType())) return;

	if (Obj::OBJ_TYPE_ROLE != attacker->GetObjType()) return;

	if (Obj::OBJ_TYPE_MONSTER == cha->GetObjType())
	{
		Monster *monster = (Monster*)cha;
		if (NULL == monster) return;

		if (monster->IsBossMonster())
		{
			Role *role = (Role*)attacker;
			long long uuid = UniqueUserIDToLongLong(role->GetUniqueUserID());

			for (int boss_type = 0; boss_type < CROSS_BIANJINGZHIDI_MAX_BOSS_TYPE; ++boss_type)
			{
				BossItem &boss_item = m_boss_list[boss_type];
				if (monster->GetMonsterId() == boss_item.boss_id && monster->GetId() == boss_item.boss_obj)
				{
					UniqueUserID guild_id = this->GetGuildUniqueUserId(role);
					std::map<UniqueUserID, BossItem::GuildHurt>::iterator it = boss_item.guild_hurt_map.find(guild_id);
					if (boss_item.guild_hurt_map.end() == it)
					{
						BossItem::GuildHurt guild_hurt;

						guild_hurt.guild_id = guild_id;
						guild_hurt.hurt -= delta_hp;
						guild_hurt.logout_timestamp = 0;
						guild_hurt.uuid_map.insert(std::make_pair(uuid, delta_hp));

						boss_item.guild_hurt_map.insert(std::make_pair(guild_id, guild_hurt));
					}
					else
					{
						it->second.hurt -= delta_hp;
						it->second.uuid_map[uuid] -= delta_hp;
						it->second.logout_timestamp = 0;
					}

					break;
				}
			}
		}
	}
}

void SpecialCrossBianJingZhiDi::OnCharacterDie(Character *dead_cha, ObjID killer_objid)
{
	if (NULL == dead_cha || INVALID_OBJ_ID == killer_objid) return;

	Obj *killer_obj = m_scene->GetObj(killer_objid);
	if (!killer_obj)
	{
		return;
	}
	if (Obj::OBJ_TYPE_ROLE != killer_obj->GetObjType())
	{
		// 怪物杀人
		if (Obj::OBJ_TYPE_MONSTER == killer_obj->GetObjType() && Obj::OBJ_TYPE_ROLE == dead_cha->GetObjType())
		{
			Role *dead_role = (Role *)dead_cha;
			CrossBianJingZhiDiUser *dead_user = this->GetCrossBianJingZhiDiUser(dead_role->GetUniqueUserID());
			if (NULL != dead_user)
			{
				dead_user->lianzhan = 0;
				dead_user->hurt_map.clear();
			}
		}
		return;
	}

	Role *kill_role = (Role*)killer_obj;
	const CrossBianJingZhiDiMonsterCfg &monster_cfg = CrossBianJingZhiDiConfig::Instance().GetCrossBianJingZhiDiMonsterCfg();

	if (Obj::OBJ_TYPE_MONSTER == dead_cha->GetObjType())
	{
		Monster *dead_monster = (Monster*)dead_cha;

		int boss_type = this->GetBossTypeByBossObjId(dead_monster->GetId(), dead_monster->GetMonsterId());
		const CrossBianJingZhiDiBossCfg *boss_cfg = CrossBianJingZhiDiConfig::Instance().GetCrossBianJingZhiDiBossCfg(boss_type);

		if (boss_cfg) // 是边境BOSS
		{
			// 触发BOSS死亡
			this->OnKillBoss(kill_role, dead_monster, boss_type);
		}
		else if(dead_monster->GetMonsterId() == monster_cfg.monster_id)
		{
			m_monster_die_num ++;

			std::set<UniqueUserID> fliter;
			this->CheckUserTaskInfo(kill_role, CROSS_BIANJINGZHIDI_TYPE_KILL_MONSTER, dead_monster->GetMonsterId(), fliter);
		}
		else
		{
			std::set<UniqueUserID> fliter;
			this->CheckUserTaskInfo(kill_role, CROSS_BIANJINGZHIDI_TYPE_KILL_MONSTER, dead_monster->GetMonsterId(), fliter);
		}
	}

	if (Obj::OBJ_TYPE_ROLE == dead_cha->GetObjType())
	{
		Role *dead_role = (Role *)dead_cha;
		std::set<UniqueUserID> hurt_dead_uuid;

		CrossBianJingZhiDiUser *killer_user = this->GetCrossBianJingZhiDiUser(kill_role->GetUniqueUserID());
		CrossBianJingZhiDiUser *dead_user = this->GetCrossBianJingZhiDiUser(dead_role->GetUniqueUserID());
		if (NULL != killer_user && NULL != dead_user)
		{
			for (std::map<UniqueUserID, time_t>::iterator it = dead_user->hurt_map.begin(), 
				end = dead_user->hurt_map.end(); it != end; ++ it)
			{
				// 助攻连斩暂时屏蔽，只算伤害列表
				//if (it->second + ASSIST_LIMIT_TIME >= EngineAdapter::Instance().Time() && it->first != kill_role->GetUniqueUserID())
				//{
				//	CrossBianJingZhiDiUser *joiner_assist = this->GetCrossBianJingZhiDiUser(it->first);
				//	if (NULL != joiner_assist)
				//	{
				//		joiner_assist->assist++;
				//		joiner_assist->lianzhan++;
				//		Role *assist_role = m_scene->GetRoleByUUID(joiner_assist->user_id);
				//		if (NULL != assist_role)
				//		{
				//			this->CheckLianZhanNotice(assist_role, dead_role, true);
				//		}
				//	}
				//}
				hurt_dead_uuid.insert(it->first);
			}

			killer_user->lianzhan++;

			this->CheckLianZhanNotice(kill_role, dead_role);

			dead_user->lianzhan = 0;
			dead_user->hurt_map.clear();  // 死亡后清除伤害我的玩家列表
		}

		this->CheckUserTaskInfo(kill_role, CROSS_BIANJINGZHIDI_TYPE_KILL_ROLE, 1, hurt_dead_uuid);

		this->SendCrossBianJingZhiDiInfoToAllUser();
	}
}

void SpecialCrossBianJingZhiDi::OnHurtOther(Character *cha, Character *target)
{
	if (Obj::OBJ_TYPE_ROLE == cha->GetObjType() && Obj::OBJ_TYPE_ROLE == target->GetObjType())
	{
		Role *hurt_role = (Role *)cha;
		Role *target_role = (Role *)target;

		if (NULL == hurt_role || NULL == target_role)
		{
			return;
		}

		CrossBianJingZhiDiUser *target_joiner = this->GetCrossBianJingZhiDiUser(target_role->GetUniqueUserID());
		if (NULL != target_joiner)
		{
			target_joiner->hurt_map[hurt_role->GetUniqueUserID()] = EngineAdapter::Instance().Time();
		}
	}
}

void SpecialCrossBianJingZhiDi::OnKillBoss(Role *role, Monster* boss, int boss_type)
{
	if (NULL == role || NULL == boss) return;
	if (boss_type < 0 || boss_type >= CROSS_BIANJINGZHIDI_MAX_BOSS_TYPE) return;

	// 设置BOSS死亡信息
	{
		// 检查归属，检查后立即将boss存活标记重置，下面代码有用到这个标记
		this->CheckSingleBossBound(boss_type);
		m_boss_list[boss_type].boss_live_flag = false;
		m_boss_list[boss_type].boss_obj = INVALID_OBJ_ID;
	}
	
	{
		std::set<UniqueUserID> hurt_dead_uuid;

		// 取出归属公会的角色伤害列表
		auto guild_it = m_boss_list[boss_type].guild_hurt_map.find(m_boss_list[boss_type].guild_uuid);
		if (m_boss_list[boss_type].guild_hurt_map.end() != guild_it)
		{
			for (auto uuid_it = guild_it->second.uuid_map.begin(); guild_it->second.uuid_map.end() != uuid_it; ++ uuid_it)
			{
				if (uuid_it->second > 0)
				{
					hurt_dead_uuid.insert(LongLongToUniqueUserID(uuid_it->first));
				}
			}

			// 跨服仙盟击杀榜
			WorldStatus::Instance().AddGuildKillBossNum(m_boss_list[boss_type].guild_uuid, m_scene->GetSceneType());
		}

		this->CheckUserTaskInfo(role, CROSS_BIANJINGZHIDI_TYPE_KILL_BOSS, 1, hurt_dead_uuid);
	}

	{
		// 重置采集次数，以便于采集本次刷新的采集物
		for (CrossBianJingZhiDiUserMapIt it = m_bianjingzhidi_usermap.begin(); it != m_bianjingzhidi_usermap.end(); ++it)
		{
			CrossBianJingZhiDiUser *bianjingzhidi_user = &(it->second);
			if (nullptr != bianjingzhidi_user)
			{
				bianjingzhidi_user->gather_box_count[boss_type] = 0;
			}
		}
	}

	const CrossBianJingZhiDiGatherCfg::GatherItem *gather_cfg = CrossBianJingZhiDiConfig::Instance().GetCrossBianJingZhiDiGatherCfg(boss_type);
	if (gather_cfg)
	{
		Posi born_pos;
		if (!CrossBianJingZhiDiConfig::Instance().GetRandomFlushPos(born_pos, gather_cfg->pos_id_start, gather_cfg->pos_id_len))
		{
			return;
		}

		// 刷新采集物，采集后不消失，BOSS刷新时直接由代码DeleteObj
		GatherObj *gather_obj = new GatherObj();
		gather_obj->Init(NULL, gather_cfg->gather_id, gather_cfg->gather_time * 1000, 0, false, gather_cfg->disappear_s);

		{
			GuildMap::iterator itr = m_guild_map.find(m_boss_list[boss_type].guild_uuid);
			if (itr != m_guild_map.end())
			{
				gather_obj->SetSpecialGatherType(SPEICAL_GATHER_TYPE_CROSS_GUILD_BATTLE_BOSS);
				gather_obj->InitCrossGuildBattleBoss(itr->second.guild_name);

				int length = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), "cross_bjzd_kill_boss",
					itr->second.guild_name, boss->GetMonsterId(), gather_cfg->gather_id);
				if (length > 0)
				{
					this->NotifySystemMsg(gstr::GSTR_BUF, length, SYS_MSG_ACTIVE_NOTICE);
				}
			}
		}

		gather_obj->SetPos(born_pos);
		m_scene->AddObj(gather_obj);

		m_boss_list[boss_type].gather_obj = gather_obj->GetId();
	}

	this->SendCrossBianJingZhiDiInfoToAllUser();
	this->SyncCrossBianJingZhiDiBossInfoToCross();
	
	//role->GetCrossGoal()->ChangeCrossGoalParam(CROSS_GOAL_COND_KILL_BAIZHAN_BOSS, 1);
	UniqueUserID guild_id = m_boss_list[boss_type].guild_uuid;
	if (guild_id != INVALID_UNIQUE_USER_ID)
	{
		crossgameprotocal::GameGameKillCrossBoss klboss;
		klboss.guild_id = UidToInt(guild_id.user_id);
		klboss.uid = INVALID_USER_ID;
		klboss.scene_type = Scene::SCENE_TYPE_CROSS_BIANJINGZHIDI;
		klboss.reserve_sh = 0;

		InternalComm::Instance().SendToCrossTransferToGame(crossgameprotocal::CROSS_TRANSFER_TYPE_BY_UUID,UniqueUserIDToLongLong(guild_id) , (const char *)&klboss, sizeof(klboss));
	}
}

void SpecialCrossBianJingZhiDi::OnStartGather(Role *role, int gather_id, ObjID obj_id, const Posi &pos)
{
	if (NULL == role) return;
	CrossBianJingZhiDiUser *bianjingzhidi_user = this->GetCrossBianJingZhiDiUser(role->GetUniqueUserID());
	if (NULL == bianjingzhidi_user) return;

	const CrossBianJingZhiDiGatherCfg::GatherItem *gather_cfg = CrossBianJingZhiDiConfig::Instance().GetCrossBianJingZhiDiGatherCfgByID(gather_id);
	if (gather_cfg)
	{
		bianjingzhidi_user->gathering_gather_id = gather_id;  // 宝箱采集
	}
	else
	{
		bianjingzhidi_user->gathering_gather_id = 0;
	}
}

bool SpecialCrossBianJingZhiDi::SpecialCanGather(Role *role, GatherObj *gather)
{
	if (NULL == role || NULL == gather)
	{
		return false;
	}

	CrossBianJingZhiDiUser *bianjingzhidi_user = this->GetCrossBianJingZhiDiUser(role->GetUniqueUserID());
	if (NULL == bianjingzhidi_user) return false;

	const CrossBianJingZhiDiGatherCfg::GatherItem *gather_cfg = CrossBianJingZhiDiConfig::Instance().GetCrossBianJingZhiDiGatherCfgByID(gather->GatherId());
		
	if (gather_cfg)
	{
		// 采集宝箱

		// 进入条件限制
		BossItem *boss_item = this->GetBossItemByGatherObjId(gather->GetId());
		if (NULL == boss_item)
		{
			gstr::SendError(role, "cross_bjzd_cannot_find_gather");
			return false;
		}

		// 不是归属帮派不可采集
		UniqueUserID guid_uuid = this->GetGuildUniqueUserId(role);
		if (boss_item->guild_uuid != guid_uuid)
		{
			gstr::SendError(role, "cross_bjzd_cannot_gather");
			return false;
		}

		// 超过采集次数不能采集
		int boss_type = gather_cfg->boss_type;
		if (boss_type >= 0 && boss_type < CROSS_BIANJINGZHIDI_MAX_BOSS_TYPE)
		{
			if (bianjingzhidi_user->gather_box_count[boss_type] > 0)
			{
				gstr::SendError(role, "cross_bjzd_already_gather");
				return false;
			}
		}
	}

	return true;
}

void SpecialCrossBianJingZhiDi::OnGather(Role *role, int gather_id, ObjID obj_id, const Posi &pos)
{
	if (NULL == role) return;

	CrossBianJingZhiDiUser *bianjingzhidi_user = this->GetCrossBianJingZhiDiUser(role->GetUniqueUserID());
	if (NULL == bianjingzhidi_user) return;

	const CrossBianJingZhiDiGatherCfg::GatherItem *gather_cfg = CrossBianJingZhiDiConfig::Instance().GetCrossBianJingZhiDiGatherCfgByID(gather_id);
	if (gather_cfg && gather_cfg->item_count > 0 && gather_cfg->item_count <= MAX_ATTACHMENT_ITEM_NUM)
	{
		role->GetKnapsack()->PutListOrMail(gather_cfg->reward_item, PUT_REASON_CROSS_BIANJINGZHIDI_ACTIVITY_REWARD);
	
		int boss_type = gather_cfg->boss_type;
		if (boss_type >= 0 && boss_type < CROSS_BIANJINGZHIDI_MAX_BOSS_TYPE)
		{
			bianjingzhidi_user->gather_box_count[boss_type] = 1;
		}
	}

	bianjingzhidi_user->gathering_gather_id = 0;

	// 采集任务
	std::set<UniqueUserID> fliter;
	this->CheckUserTaskInfo(role, CROSS_BIANJINGZHIDI_TYPE_GATHER, gather_id, fliter);

	this->SendCrossBianJingZhiDiUserInfo(role);
	gstr::SendNotice(role, "cross_bjzd_gather_succ");
}

void SpecialCrossBianJingZhiDi::OnRoleEnterScene(Role *role)
{
	if (m_bianjingzhidi_usermap.find(role->GetUniqueUserID()) == m_bianjingzhidi_usermap.end())
	{
		gamelog::g_log_roleactivity.printf(LL_INFO, "SpecialCrossBianJingZhiDi::OnRoleEnterScene Join. user[%d, %s], level[%d]",
			UidToInt(role->GetUniqueUserID().user_id), role->GetName(), role->GetLevel());
	}

	CrossBianJingZhiDiUser *bianjingzhidi_user = this->GetCrossBianJingZhiDiUser(role->GetUniqueUserID());
	if (NULL == bianjingzhidi_user) return;

	role->GetName(bianjingzhidi_user->user_name);

	long long uuid = UniqueUserIDToLongLong(role->GetUniqueUserID());

	if (ActivityShadow::Instance().IsCrossActivityOpen(CROSS_ACTIVITY_TYPE_CROSS_BIANJINGZHIDI))
	{
		if (m_role_task_time_map.find(uuid) == m_role_task_time_map.end())
		{
			m_role_task_time_map[uuid] = EngineAdapter::Instance().Time() + CrossBianJingZhiDiConfig::Instance().GetCrossBianJingZhiDiOtherCfg().double_reward_limit_time;
		}
	}


	this->SendCrossBianJingZhiDiUserInfo(role);
	this->SendBossInfo(role);
}

void SpecialCrossBianJingZhiDi::OnRoleLeaveScene(Role *role, bool is_logout)
{
	if (NULL == role) return;

	long long uuid = UniqueUserIDToLongLong(role->GetUniqueUserID());
	for (int boss_type = 0; boss_type < CROSS_BIANJINGZHIDI_MAX_BOSS_TYPE; ++boss_type)
	{
		if (!m_boss_list[boss_type].boss_live_flag)
		{
			continue;
		}

		Monster *monster = m_scene->GetMonster(m_boss_list[boss_type].boss_obj);
		if (NULL == monster)
		{
			continue;
		}

		UniqueUserID guild_id = this->GetGuildUniqueUserId(role);
		std::map<UniqueUserID, BossItem::GuildHurt>::iterator guild_hurt_it = m_boss_list[boss_type].guild_hurt_map.find(guild_id);
		if (guild_hurt_it != m_boss_list[boss_type].guild_hurt_map.end())
		{
			BossItem::GuildHurt &guild_hurt = guild_hurt_it->second;

			guild_hurt.uuid_map.erase(uuid);

			if (!is_logout)  // 主动退出就清除归属
			{
				if (guild_hurt.uuid_map.size() <= 0)
				{
					if (m_boss_list[boss_type].guild_uuid == guild_hurt.guild_id)
					{
						m_boss_list[boss_type].guild_id = 0;
						m_boss_list[boss_type].guild_uuid = INVALID_UNIQUE_USER_ID;
					}
					m_boss_list[boss_type].guild_hurt_map.erase(guild_hurt_it);
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

	if (!role->IsAlive())
	{
		role->ReAlive(100, 100, role->GetPos(), true);
	}
	else
	{
		role->Recover();
	}
}

int SpecialCrossBianJingZhiDi::GetSpecialParam(Role *role)
{
	if (NULL == role) return 0;

	CrossBianJingZhiDiUser * user = this->GetCrossBianJingZhiDiUser(role->GetUniqueUserID());

	if (NULL == user) return 0;

	unsigned int now_sec = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	
	return user->bianjingzhidi_buff_timestamp > now_sec;
}

bool SpecialCrossBianJingZhiDi::SpecialStopGahterOnAttack(Role *role)
{
	if (role == NULL) return true;

	CrossBianJingZhiDiUser * user = this->GetCrossBianJingZhiDiUser(role->GetUniqueUserID());
	if (user == NULL) return true;

	const CrossBianJingZhiDiGatherCfg::GatherItem *gather_cfg = CrossBianJingZhiDiConfig::Instance().GetCrossBianJingZhiDiGatherCfgByID(user->gathering_gather_id);
	if (gather_cfg)
	{
		return false;
	}

	if (user->bianjingzhidi_buff_timestamp >= (unsigned int)(EngineAdapter::Instance().Time()))
	{
		return false;
	}

	return true;
}

void SpecialCrossBianJingZhiDi::OnActivityStatusChange(int status)
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
			this->OnActivityClose();
		}
		break;
	}
}

CrossBianJingZhiDiUser * SpecialCrossBianJingZhiDi::GetCrossBianJingZhiDiUser(const UniqueUserID &user_id)
{
	CrossBianJingZhiDiUserMapIt it = m_bianjingzhidi_usermap.find(user_id);
	if (it == m_bianjingzhidi_usermap.end())								// 不存在 插入新的用户
	{
		CrossBianJingZhiDiUser bianjingzhidi_user;
		bianjingzhidi_user.user_id = user_id;

		CrossBianJingZhiDiTaskInfo bianjingzhidi_task_list[CROSS_BIANJINGZHIDI_TASK_MAX];
		CrossBianJingZhiDiConfig::Instance().GetCrossBianJingZhiDiTaskList(bianjingzhidi_task_list);

		// 随机选两个能够双倍的任务做为限时任务
		int index_oneId = 0; int index_twoId = 0;
		{
			std::vector<int> index_vec; // 筛选能够双倍的任务
			for (int i = 0; i < CROSS_BIANJINGZHIDI_TASK_MAX; i++)
			{
				if (bianjingzhidi_task_list[i].task_id >= 0 && bianjingzhidi_task_list[i].can_limit_flag)
				{
					index_vec.push_back(bianjingzhidi_task_list[i].task_id);
				}
			}

			if (index_vec.size() > 2)
			{
				std::random_shuffle(index_vec.begin(), index_vec.end());
				index_oneId = index_vec[0];
				index_twoId = index_vec[1];
			}
			else if (2 == index_vec.size())
			{
				index_oneId = index_vec[0];
				index_twoId = index_vec[1];
			}
			else if (1 == index_vec.size())
			{
				index_oneId = index_vec[0];
				index_twoId = index_vec[0];
			}
		}

		for (int i = 0; i < CROSS_BIANJINGZHIDI_TASK_MAX; i++)
		{
			if (bianjingzhidi_task_list[i].task_id >= 0)	
			{
				if (index_oneId == bianjingzhidi_task_list[i].task_id)
				{
					bianjingzhidi_task_list[i].is_double_reward = true;
				}

				if (index_twoId == bianjingzhidi_task_list[i].task_id)
				{
					bianjingzhidi_task_list[i].is_double_reward = true;
				}

				bianjingzhidi_user.bianjingzhidi_task_list.push_back(bianjingzhidi_task_list[i]);
			}
		}

		m_bianjingzhidi_usermap[user_id] = bianjingzhidi_user;
	}

	if (it == m_bianjingzhidi_usermap.end()) it = m_bianjingzhidi_usermap.find(user_id); 

	if (it != m_bianjingzhidi_usermap.end())
	{
		return &it->second;
	}

	return NULL;
}

void SpecialCrossBianJingZhiDi::OnBuyGatherBuff(Role * role)
{
	if (NULL == role) return;

	CrossBianJingZhiDiUser * user = this->GetCrossBianJingZhiDiUser(role->GetUniqueUserID());
	if (NULL == user) return;

	const CrossBianJingZhiDiOtherCfg & other_cfg = CrossBianJingZhiDiConfig::Instance().GetCrossBianJingZhiDiOtherCfg();
	if (other_cfg.gather_buff_gold > 0 && !role->GetKnapsack()->GetMoney()->UseAllGold(other_cfg.gather_buff_gold,"CrossBianJingZhiDi::BuyBuff"))
	{
		role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	user->is_brocast_gather_buff = false;
	user->bianjingzhidi_buff_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + other_cfg.gather_buff_time;

	if (m_next_check_buff_time <= 0)
	{
		m_next_check_buff_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) +1;
	}
	
	this->SendCrossBianJingZhiDiUserInfo(role);
	this->NotifyGatherBuffInfo(role, 1);
}

SpecialCrossBianJingZhiDi::BossItem *SpecialCrossBianJingZhiDi::GetBossItemByGatherObjId(ObjID gather_obj)
{
	for (int boss_type = 0; boss_type < CROSS_BIANJINGZHIDI_MAX_BOSS_TYPE; ++boss_type)
	{
		if (m_boss_list[boss_type].gather_obj == gather_obj)
		{
			return &m_boss_list[boss_type];
		}
	}

	return nullptr;
}

int SpecialCrossBianJingZhiDi::GetBossTypeByBossObjId(ObjID boss_obj, int boss_id)
{
	for (int boss_type = 0; boss_type < CROSS_BIANJINGZHIDI_MAX_BOSS_TYPE; ++boss_type)
	{
		if (m_boss_list[boss_type].boss_obj == boss_obj && m_boss_list[boss_type].boss_id == boss_id)
		{
			return boss_type;
		}
	}

	return -1;
}

void SpecialCrossBianJingZhiDi::OnActivityStandby()
{
	m_is_finish = false;

	m_bianjingzhidi_usermap.clear();
	m_role_task_time_map.clear();

	m_next_reflush_monster_time = 0;
	m_next_reflush_boss_time = 0;
	m_monster_die_num = 0;
	
	for (int boss_type = 0; boss_type < CROSS_BIANJINGZHIDI_MAX_BOSS_TYPE; ++boss_type)
	{
		m_boss_list[boss_type].Reset();
	}
	 
	m_guild_map.clear();
}

void SpecialCrossBianJingZhiDi::OnActivityStart()
{
	const CrossBianJingZhiDiMonsterCfg  &monster_cfg = CrossBianJingZhiDiConfig::Instance().GetCrossBianJingZhiDiMonsterCfg();
	time_t now_second = EngineAdapter::Instance().Time();

	m_next_reflush_monster_time = now_second + monster_cfg.start_reflush_time_s;
	m_next_reflush_boss_time = now_second + CrossBianJingZhiDiConfig::Instance().GetCrossBianJingZhiDiOtherCfg().start_reflush_time_s;
	m_reflush_notice_time = m_next_reflush_boss_time - CROSS_BIANJINGZHIDI_REFLUSH_NOTICE_TIME;
	m_is_notice = true;

	m_monster_die_num = monster_cfg.count;

	this->SyncCrossBianJingZhiDiBossInfoToCross();

	for (auto itr = m_bianjingzhidi_usermap.begin(); itr != m_bianjingzhidi_usermap.end(); ++itr)
	{
		if (m_role_task_time_map.find(UniqueUserIDToLongLong(itr->first)) == m_role_task_time_map.end())
		{
			m_role_task_time_map[UniqueUserIDToLongLong(itr->first)] = EngineAdapter::Instance().Time() + CrossBianJingZhiDiConfig::Instance().GetCrossBianJingZhiDiOtherCfg().double_reward_limit_time;
		}
	}

	this->SendCrossBianJingZhiDiInfoToAllUser();
}

void SpecialCrossBianJingZhiDi::OnActivityClose()
{
	this->OnCrossBianJingZhiDiClose();

	m_is_finish = true;
	for (int boss_type = 0; boss_type < CROSS_BIANJINGZHIDI_MAX_BOSS_TYPE; ++boss_type)
	{
		m_boss_list[boss_type].Reset();
	}

	m_next_reflush_monster_time = 0;
	m_next_reflush_boss_time = 0;
	m_is_notice = false;
	m_monster_die_num = 0;

	// 移除怪物
	this->KillAllMonster();

	// 自动传出
	this->KickOutAllRole();

	this->SyncCrossBianJingZhiDiBossInfoToCross();
}

void SpecialCrossBianJingZhiDi::OnCrossBianJingZhiDiClose()
{
	for (CrossBianJingZhiDiUserMapIt it = m_bianjingzhidi_usermap.begin(); it != m_bianjingzhidi_usermap.end(); ++it)
	{
		CrossBianJingZhiDiUser *bianjingzhidi_user = &(it->second);

		for (std::vector< CrossBianJingZhiDiTaskInfo>::iterator task_it = bianjingzhidi_user->bianjingzhidi_task_list.begin(); task_it != bianjingzhidi_user->bianjingzhidi_task_list.end(); ++task_it)
		{
			CrossBianJingZhiDiTaskInfo &taskinfo = *task_it;

			{
				// 跨服边境之地任务日志
				if (taskinfo.IsFinish())
				{
					gamelog::g_log_roleactivity.printf(LL_INFO, "SpecialCrossBianJingZhiDi task record. user[%d, %s], complete_task_id[%d]",
						UidToInt(bianjingzhidi_user->user_id.user_id), bianjingzhidi_user->user_name, task_it->task_id);
				}
			}
		}
	}
}

void SpecialCrossBianJingZhiDi::CheckUserTaskInfo(Role *role, short task_type, int param_id, const std::set<UniqueUserID> &uuid_set)
{
	if (NULL == role) return;

	Role *member_list[16] = {NULL};
	int team_rolenum = role->GetTeamInfo()->GetOtherMemberInTheSameScene(member_list, sizeof(member_list) / sizeof(member_list[0]));

	if (team_rolenum < static_cast<int>(sizeof(member_list) / sizeof(member_list[0])))
	{
		member_list[team_rolenum++] = role; // 把自己加进列表
	}

	// 处理队友
	for (int r = 0; r < team_rolenum; r++)
	{
		if (NULL == member_list[r]) continue;

		// 助攻列表的玩家不处理
		if (uuid_set.end() != uuid_set.find(member_list[r]->GetUniqueUserID())) continue;

		CrossBianJingZhiDiUser *bianjingzhidi_user = this->GetCrossBianJingZhiDiUser(member_list[r]->GetUniqueUserID());
		if (NULL != bianjingzhidi_user)
		{
			if (task_type == CROSS_BIANJINGZHIDI_TYPE_KILL_BOSS)
			{
				member_list[r]->GetCrossGoal()->ChangeCrossGoalParam(CROSS_GOAL_COND_KILL_BAIZHAN_BOSS, 1);
			}

			for (std::vector<CrossBianJingZhiDiTaskInfo>::iterator it = bianjingzhidi_user->bianjingzhidi_task_list.begin(), end = bianjingzhidi_user->bianjingzhidi_task_list.end(); it != end; ++ it)
			{
				CrossBianJingZhiDiTaskInfo &taskinfo = *it;
				if (!taskinfo.IsFinish() && taskinfo.IsMatch(task_type, param_id))
				{
					taskinfo.AddParamVal();

					if (taskinfo.IsFinish() && !taskinfo.IsFetchReward())
					{
						for(int i = 0; i < taskinfo.item_count && i < MAX_ATTACHMENT_ITEM_NUM; i++)
						{
							bianjingzhidi_user->role_get_item_map[taskinfo.reward_item[i].item_id] += taskinfo.reward_item[i].num;
						}

						++ bianjingzhidi_user->task_finish_num;
						taskinfo.is_fetch_reward = true;
						member_list[r]->GetKnapsack()->PutListOrMail(taskinfo.reward_item, PUT_REASON_CROSS_BIANJINGZHIDI_ACTIVITY_REWARD);
						member_list[r]->GetCrossGoal()->ChangeCrossGoalParam(CROSS_GOAL_COND_FINISH_BAIZHAN_TASK, 1);

						std::map<long long, time_t>::iterator it = m_role_task_time_map.find(UniqueUserIDToLongLong(bianjingzhidi_user->user_id));
						if (it != m_role_task_time_map.end())
						{
							if (taskinfo.IsDoubleReward() && EngineAdapter::Instance().Time() <= it->second)
							{
								for(int i = 0; i < taskinfo.item_count && i < MAX_ATTACHMENT_ITEM_NUM; i++)
								{
									bianjingzhidi_user->role_get_item_map[taskinfo.reward_item[i].item_id] += taskinfo.reward_item[i].num;
								}

								member_list[r]->GetKnapsack()->PutListOrMail(taskinfo.reward_item, PUT_REASON_CROSS_BIANJINGZHIDI_ACTIVITY_REWARD);
							}
						}

						gstr::SendNotice(member_list[r], "cross_bjzd_get_task_reward");
					}
				}
			}
		}

		this->SendCrossBianJingZhiDiUserInfo(member_list[r]);
	}

	// 处理助攻人员
	for (auto uuid_it = uuid_set.begin(); uuid_it != uuid_set.end(); ++ uuid_it)
	{
		UniqueUserID uuid = *uuid_it;
		Role *tmp_role = m_scene->GetRoleByUUID(uuid);
		if (nullptr == tmp_role) return;

		CrossBianJingZhiDiUser *bianjingzhidi_user = this->GetCrossBianJingZhiDiUser(uuid);
		if (NULL != bianjingzhidi_user)
		{
			if (task_type == CROSS_BIANJINGZHIDI_TYPE_KILL_BOSS)
			{
				tmp_role->GetCrossGoal()->ChangeCrossGoalParam(CROSS_GOAL_COND_KILL_BAIZHAN_BOSS, 1);
			}

			for (std::vector<CrossBianJingZhiDiTaskInfo>::iterator it = bianjingzhidi_user->bianjingzhidi_task_list.begin(), end = bianjingzhidi_user->bianjingzhidi_task_list.end(); it != end; ++it)
			{
				CrossBianJingZhiDiTaskInfo &taskinfo = *it;
				if (!taskinfo.IsFinish() && taskinfo.IsMatch(task_type, param_id))
				{
					taskinfo.AddParamVal();

					if (taskinfo.IsFinish() && !taskinfo.IsFetchReward())
					{
						for (int i = 0; i < taskinfo.item_count && i < MAX_ATTACHMENT_ITEM_NUM; i++)
						{
							bianjingzhidi_user->role_get_item_map[taskinfo.reward_item[i].item_id] += taskinfo.reward_item[i].num;
						}

						++bianjingzhidi_user->task_finish_num;
						taskinfo.is_fetch_reward = true;
						tmp_role->GetKnapsack()->PutListOrMail(taskinfo.reward_item, PUT_REASON_CROSS_BIANJINGZHIDI_ACTIVITY_REWARD);
						tmp_role->GetCrossGoal()->ChangeCrossGoalParam(CROSS_GOAL_COND_FINISH_BAIZHAN_TASK, 1);

						std::map<long long, time_t>::iterator time_map_it = m_role_task_time_map.find(UniqueUserIDToLongLong(bianjingzhidi_user->user_id));
						if (time_map_it != m_role_task_time_map.end())
						{
							if (taskinfo.IsDoubleReward() && EngineAdapter::Instance().Time() <= time_map_it->second)
							{
								for (int i = 0; i < taskinfo.item_count && i < MAX_ATTACHMENT_ITEM_NUM; i++)
								{
									bianjingzhidi_user->role_get_item_map[taskinfo.reward_item[i].item_id] += taskinfo.reward_item[i].num;
								}

								tmp_role->GetKnapsack()->PutListOrMail(taskinfo.reward_item, PUT_REASON_CROSS_BIANJINGZHIDI_ACTIVITY_REWARD);
							}
						}

						gstr::SendNotice(tmp_role, "cross_bjzd_get_task_reward");
					}
				}
			}
		}

		this->SendCrossBianJingZhiDiUserInfo(tmp_role);
	}
}

void SpecialCrossBianJingZhiDi::NotifySystemMsg(char *str_buff, int str_len, int notice_type)
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

void SpecialCrossBianJingZhiDi::SendCrossBianJingZhiDiInfoToAllUser()
{
	for (int i = 0; i < (int)m_scene->RoleNum(); ++ i)
	{
		Role *role = m_scene->GetRoleByIndex(i);
		if (NULL != role)
		{
			this->SendCrossBianJingZhiDiUserInfo(role);
		}
	}
}

void SpecialCrossBianJingZhiDi::SendCrossBianJingZhiDiUserInfo(Role *role)
{
	if (NULL == role) return;

	CrossBianJingZhiDiUser *bianjingzhidi_user = this->GetCrossBianJingZhiDiUser(role->GetUniqueUserID());
	if (NULL == bianjingzhidi_user) return;

	static Protocol::SCCrossBianJingZhiDiUserInfo cmd;

	{
		std::map<long long, time_t>::iterator it = m_role_task_time_map.find(UniqueUserIDToLongLong(role->GetUniqueUserID()));
		if (it != m_role_task_time_map.end())
		{
			cmd.limit_task_time = (int)it->second;
		}
		else
		{
			cmd.limit_task_time = 0;
		}
	}

	cmd.boss_reflush_time = (unsigned int)m_next_reflush_boss_time;
	cmd.gather_buff_time = bianjingzhidi_user->bianjingzhidi_buff_timestamp;
	cmd.sos_times = bianjingzhidi_user->sos_times;

	memset(cmd.reward_list, 0, sizeof(cmd.reward_list));

	if (bianjingzhidi_user->is_fetch_reward && !bianjingzhidi_user->is_send_item_info)
	{
		int i = 0;
		for(std::map<int, int>::iterator it = bianjingzhidi_user->role_get_item_map.begin(), end = bianjingzhidi_user->role_get_item_map.end(); it != end && i < CROSS_BIANJINGZHIDI_MAX_REWARD_ITEM_COUNT; ++it, ++i)
		{
			cmd.reward_list[i].item_id = it->first;
			cmd.reward_list[i].num = it->second;
		}

		bianjingzhidi_user->is_send_item_info = true;
	}

	int task_count = 0;
	for (std::vector<CrossBianJingZhiDiTaskInfo>::iterator it = bianjingzhidi_user->bianjingzhidi_task_list.begin(), end = bianjingzhidi_user->bianjingzhidi_task_list.end();
		it != end; ++ it)
	{
		if (task_count >= 0 && task_count < CROSS_BIANJINGZHIDI_TASK_MAX)
		{
			const CrossBianJingZhiDiTaskInfo *task_cfg = CrossBianJingZhiDiConfig::Instance().GetCrossBianJingZhiDiTaskCfg(it->task_id);
			if (NULL == task_cfg)
			{
				continue;
			}

			cmd.taskinfo_list[task_count].task_id = it->task_id;
			cmd.taskinfo_list[task_count].is_finish = it->is_finish;
			cmd.taskinfo_list[task_count].cur_param_value = it->cur_param_value;
			cmd.taskinfo_list[task_count].param_count = it->param_count;
			cmd.taskinfo_list[task_count].is_double_reward = it->is_double_reward;
			++task_count;
		}
		else
		{
			break;
		}
	}

	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&cmd, sizeof(cmd));
}

void SpecialCrossBianJingZhiDi::SyncCrossBianJingZhiDiBossInfoToCross()
{
	crossgameprotocal::GameCrossSyncCrossBianJingZhiBossInfo gcsbi;
	gcsbi.next_boss_refresh_time = (unsigned int)m_next_reflush_boss_time;
	
	for (int boss_type = 0; boss_type < CROSS_BIANJINGZHIDI_MAX_BOSS_TYPE; ++boss_type)
	{
		gcsbi.boss_list[boss_type].boss_id = m_boss_list[boss_type].boss_id;
		gcsbi.boss_list[boss_type].live_flag = m_boss_list[boss_type].boss_live_flag ? (int)1 : (int)0;
	}

	InternalComm::Instance().SendToCross((const char *)&gcsbi, sizeof(gcsbi));
}

void SpecialCrossBianJingZhiDi::KickOutAllRole()
{
	for (int i = 0; i < (int)m_scene->RoleNum(); ++i)
	{
		Role *role = m_scene->GetRoleByIndex(i);
		if (NULL != role)
		{
			if (Role::ROLE_STATUS_GATHER == role->GetRoleStatus())
			{
				role->ResetRoleStatus();  // 活动结束打断采集
			}

			role->BackToOriginalServer();
		}
	}
}

void SpecialCrossBianJingZhiDi::CheckLianZhanNotice(Role *kill_role, Role *dead_role, bool is_assist)
{
	return; // 屏蔽
	if (NULL != kill_role && NULL != dead_role)
	{
		CrossBianJingZhiDiUser *killer = this->GetCrossBianJingZhiDiUser(kill_role->GetUniqueUserID());
		CrossBianJingZhiDiUser *dead = this->GetCrossBianJingZhiDiUser(dead_role->GetUniqueUserID());
		if (NULL != killer && NULL != dead)
		{
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
}

void SpecialCrossBianJingZhiDi::SendBossInfo(Role *role)
{
	Protocol::SCCrossBianJingZhiDiBossInfo boss_info;
	boss_info.count = 0;
	
	for (int boss_type = 0; boss_type < CROSS_BIANJINGZHIDI_MAX_BOSS_TYPE && boss_info.count < CROSS_BIANJINGZHIDI_MAX_BOSS_TYPE; ++boss_type)
	{
		const BossItem &boss_item = m_boss_list[boss_type];
		if (boss_item.boss_id > 0)
		{
			Protocol::SCCrossBianJingZhiDiBossInfo::BossInfoList &info = boss_info.boss_list[boss_info.count];
			info.boss_obj = boss_item.boss_obj;
			info.boss_live_flag = boss_item.boss_live_flag ? (short)1 : (short)0;
			info.boss_id = boss_item.boss_id;
			info.born_pos_x = (short)boss_item.born_pos.x;
			info.born_pos_y = (short)boss_item.born_pos.y;
			if (INVALID_UNIQUE_USER_ID == boss_item.guild_uuid)
			{
				info.guild_uuid = 0;
				memset(info.guild_name, 0, sizeof(GuildName));
			}
			else
			{
				info.guild_uuid = UniqueUserIDToLongLong(boss_item.guild_uuid);
				F_STRNCPY(info.guild_name, boss_item.guild_name, sizeof(GuildName));
			}

			++boss_info.count;
		}
	}


	int send_len = sizeof(boss_info) - sizeof(boss_info.boss_list[0]) * (CROSS_BIANJINGZHIDI_MAX_BOSS_TYPE - boss_info.count);
	if (role != nullptr)
	{
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&boss_info, send_len);
	}
	else
	{
		m_scene->SendToRole((const char*)&boss_info, send_len);
	}
}

void SpecialCrossBianJingZhiDi::CheckSendBossInfo()
{
	this->SendBossInfo();
}

void SpecialCrossBianJingZhiDi::CheckGatherBuffInfo()
{
	int role_num = m_scene->RoleNum();
	for (int i = 0; i < role_num; ++i)
	{
		Role * temp_role = m_scene->GetRoleByIndex(i);
		if (NULL != temp_role)
		{
			CrossBianJingZhiDiUser * user = this->GetCrossBianJingZhiDiUser(temp_role->GetUniqueUserID());
			if (NULL != user)
			{
				if (!user->is_brocast_gather_buff && user->bianjingzhidi_buff_timestamp <= static_cast<unsigned int>(EngineAdapter::Instance().Time()))
				{
					user->is_brocast_gather_buff = true;
					this->NotifyGatherBuffInfo(temp_role, 0);
				}
			}
		}
	}
}

void SpecialCrossBianJingZhiDi::NotifyGatherBuffInfo(Role *role, short is_has_buff)
{
	if (NULL == role) return;

	Protocol::SCNoticeGatherBuffInfo cmd;
	cmd.obj_id = role->GetId();
	cmd.is_gather_wudi = is_has_buff;

	role->GetScene()->GetZoneMatrix()->NotifyAreaMsg(role->GetObserHandle(), (void*)&cmd, sizeof(cmd));
}

UniqueUserID SpecialCrossBianJingZhiDi::GetGuildUniqueUserId(Role *role)
{
	if (NULL == role) return INVALID_UNIQUE_USER_ID;

	int plat_type = role->GetUniqueUserID().plat_type;
	UniqueUserID guild_id(plat_type, IntToUid(role->GetGuildID()));

	return guild_id;
}

void SpecialCrossBianJingZhiDi::CheckAllBossBound()
{
	for (int boss_type = 0; boss_type < CROSS_BIANJINGZHIDI_MAX_BOSS_TYPE; ++boss_type)
	{
		CheckSingleBossBound(boss_type);
	}
}

void SpecialCrossBianJingZhiDi::CheckSingleBossBound(int boss_type/*, long long *high_hurt_role_uuid = NULL */)
{
	if (boss_type < 0 || boss_type >= CROSS_BIANJINGZHIDI_MAX_BOSS_TYPE)
	{
		return;
	}

	if (!m_boss_list[boss_type].boss_live_flag)  // BOSS已被标记死亡则不计算归属
	{
		return;
	}

	//const auto &other_cfg = CrossBianJingZhiDiConfig::Instance().GetCrossBianJingZhiDiOtherCfg();

	Monster *monster = m_scene->GetMonster(m_boss_list[boss_type].boss_obj);
	if (NULL == monster)
	{
		return;
	}

	std::map<UniqueUserID, BossItem::GuildHurt>::iterator guild_hurt_it = m_boss_list[boss_type].guild_hurt_map.begin();
	for (; guild_hurt_it != m_boss_list[boss_type].guild_hurt_map.end();)
	{
		BossItem::GuildHurt &guild_hurt = guild_hurt_it->second;
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

			if ((temp_role->GetPos() - monster->GetPos()).IsLongerThan(CROSS_BIANJINGZHIDI_OWNER_ROLE_AND_BOSS_DISTANCE)) // 不在boss范围内
			{
				guild_hurt.uuid_map.erase(uuid_it++);
			}
			else
			{
				uuid_it++;
			}
		}

		if (guild_hurt.logout_timestamp > 0 && (guild_hurt.logout_timestamp + 60) <= EngineAdapter::Instance().Time())
		{
			// 掉线时间已经超时，就清掉
			guild_hurt.uuid_map.clear();
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
			if (m_boss_list[boss_type].guild_uuid == guild_hurt.guild_id)
			{
				m_boss_list[boss_type].guild_id = 0;
				m_boss_list[boss_type].guild_uuid = INVALID_UNIQUE_USER_ID;
			}
			m_boss_list[boss_type].guild_hurt_map.erase(guild_hurt_it++);
		}
		else
		{
			guild_hurt_it++;
		}
	}

	std::vector<BossItem::GuildHurt*> hurt_vec;
	for (guild_hurt_it = m_boss_list[boss_type].guild_hurt_map.begin(); guild_hurt_it != m_boss_list[boss_type].guild_hurt_map.end(); ++guild_hurt_it)
	{
		hurt_vec.push_back(&guild_hurt_it->second);
	}

	std::sort(hurt_vec.begin(), hurt_vec.end(), [](const BossItem::GuildHurt * hurt1, const BossItem::GuildHurt * hurt2) {return hurt1->hurt > hurt2->hurt; });

	{
		// 设置新归属或失去归属
		if (hurt_vec.size() <= 0)
		{
			m_boss_list[boss_type].guild_id = 0;
			m_boss_list[boss_type].guild_uuid = INVALID_UNIQUE_USER_ID;
		}
		else
		{
			m_boss_list[boss_type].guild_id = UidToInt(hurt_vec[0]->guild_id.user_id);
			m_boss_list[boss_type].guild_uuid = hurt_vec[0]->guild_id;
		}
	}

	if (hurt_vec.size() > 0)
	{
		Protocol::SCCrossBianJingZhiDiBossHurtInfo hurt_info;
		hurt_info.boss_id = m_boss_list[boss_type].boss_id;
		hurt_info.count = 0;
		for (unsigned int i = 0; i < hurt_vec.size(); ++i)
		{
			hurt_vec[i]->rank = i;
			if (hurt_info.count < Protocol::SCCrossBianJingZhiDiBossHurtInfo::BOSS_HURT_INFO_RANK_MAX)
			{
				auto itr = m_guild_map.find(hurt_vec[i]->guild_id);
				if (itr != m_guild_map.end())
				{
					hurt_info.info_list[hurt_info.count].guild_id = UidToInt(hurt_vec[i]->guild_id.user_id);
					hurt_info.info_list[hurt_info.count].hurt = hurt_vec[i]->hurt;
					F_STRNCPY(hurt_info.info_list[hurt_info.count].guild_name, itr->second.guild_name, sizeof(GuildName));
					hurt_info.count++;
				}
			}
		}

		const int MAX_GET_ROLE = 256;
		ObjID obj_list[MAX_GET_ROLE] = { 0 };
		int count = m_scene->GetZoneMatrix()->GetObjByCircleArea(m_boss_list[boss_type].born_pos, CROSS_BIANJINGZHIDI_OWNER_ROLE_AND_BOSS_DISTANCE + 2, obj_list, sizeof(obj_list) / sizeof(obj_list[0]));
		for (int i = 0; i < count && i < MAX_GET_ROLE; ++i)
		{
			Role * role = m_scene->GetPrincipal(obj_list[i]);
			if (nullptr != role)
			{
				hurt_info.own_guild_rank = hurt_vec.size();
				hurt_info.own_guild_hurt = 0;
				std::map<UniqueUserID, BossItem::GuildHurt>::iterator hurt_it = m_boss_list[boss_type].guild_hurt_map.find(this->GetGuildUniqueUserId(role));
				if (hurt_it != m_boss_list[boss_type].guild_hurt_map.end())
				{
					hurt_info.own_guild_hurt = hurt_it->second.hurt;
					hurt_info.own_guild_rank = hurt_it->second.rank;
				}
				
				EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&hurt_info, sizeof(hurt_info));
			}
		}
	}
}

void SpecialCrossBianJingZhiDi::SendGuildSos(Protocol::CSSendGuildSosReq *req, Role *role)
{
	if (!ActivityShadow::Instance().IsCrossActivityOpen(CROSS_ACTIVITY_TYPE_CROSS_BIANJINGZHIDI))
	{
		return;
	}

	if (nullptr == role) return;
	if (INVALID_UNIQUE_USER_ID == this->GetGuildUniqueUserId(role)) return;
	CrossBianJingZhiDiUser *bianjingzhidi_user = this->GetCrossBianJingZhiDiUser(role->GetUniqueUserID());
	if (nullptr == bianjingzhidi_user) return;

	//UniqueUserID sos_guild_id = this->GetGuildUniqueUserId(role);

	int cost = CrossBianJingZhiDiConfig::Instance().GetSosCost(bianjingzhidi_user->sos_times);
	if (cost > 0)
	{
		if (!role->GetKnapsack()->GetMoney()->UseAllGold(cost, "SpecialCrossBianJingZhiDi::SendGuildSos"))
		{
			role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
			return;
		}
	}
	else if (cost < 0)  // 次数不对，就返回
	{
		return;
	}

	++bianjingzhidi_user->sos_times;

	this->SendCrossBianJingZhiDiUserInfo(role);

	crossgameprotocal::GameGameCrossGuildBattleSos pro;
	pro.guild_id = role->GetGuildID();
	pro.member_uid = UidToInt(bianjingzhidi_user->user_id.user_id);
	role->GetName(pro.member_name);
	pro.member_camp = role->GetCamp();
	pro.member_scene_id = role->GetScene()->GetSceneID();
	pro.member_scene_key = role->GetScene()->GetSceneKey();
	pro.member_pos_x = role->GetPos().x;
	pro.member_pos_y = role->GetPos().y;
	UNSTD_STATIC_CHECK(MAX_SCENE_NAME_LENGTH == sizeof(pro.member_scene_name))
	F_STRNCPY(pro.member_scene_name, role->GetScene()->GetSceneName(), sizeof(pro.member_scene_name));
	pro.sos_type = GUILD_SOS_TYPE_CROSS_BIANJINGZHIDI;
	pro.sos_sysmsg = 0;

	InternalComm::Instance().SendToCrossTransferToGame(crossgameprotocal::CROSS_TRANSFER_TYPE_BY_UUID, 
		UniqueUserIDToLongLong(bianjingzhidi_user->user_id),
		(const char *)&pro, sizeof(pro));

// 	static Protocol::SCGuildMemberSos gms;
// 	gms.sos_type = req->sos_type;
// 	gms.member_uid = UidToInt(role->GetUserId());
// 	role->GetName(gms.member_name);
// 	gms.member_scene_id = role->GetScene()->GetSceneID();
// 	gms.member_pos_x = role->GetPos().x;
// 	gms.member_pos_y = role->GetPos().y;
// 	gms.enemy_uid = 0;
// 	memset(gms.enemy_name, 0, sizeof(gms.enemy_name));
// 	gms.enemy_camp = 0;
// 	gms.enemy_guild_id = INVALID_GUILD_ID;
// 	memset(gms.enemy_guild_name, 0, sizeof(gms.enemy_guild_name));
// 
// 	const UniqueUserID guild_id = this->GetGuildUniqueUserId(role);
// 	
// 	for (int i = 0; i < (int)m_scene->RoleNum(); ++i)
// 	{
// 		Role *send_role = m_scene->GetRoleByIndex(i);
// 		if (NULL != send_role && guild_id == this->GetGuildUniqueUserId(send_role))
// 		{
// 			EngineAdapter::Instance().NetSend(send_role->GetNetId(), (const char*)&gms, sizeof(gms));
// 		}
// 	}

	return;
}

void SpecialCrossBianJingZhiDi::ReplyGuildSos(Protocol::CSReplyGuildSosReq *sos_reply, Role *role)
{
	if (!ActivityShadow::Instance().IsCrossActivityOpen(CROSS_ACTIVITY_TYPE_CROSS_BIANJINGZHIDI))
	{
		return;
	}

	if (nullptr == role) return;
	if (INVALID_UNIQUE_USER_ID == this->GetGuildUniqueUserId(role)) return;
	CrossBianJingZhiDiUser *bianjingzhidi_user = this->GetCrossBianJingZhiDiUser(role->GetUniqueUserID());
	if (nullptr == bianjingzhidi_user) return;

	role->ResetPos(sos_reply->pos_x, sos_reply->pos_y);
}
