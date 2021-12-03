#include "specialwanglingexplore.h"

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

#include "config/activityconfig/wanglingexploreconfig.h"
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

SpecialWangLingExplore::SpecialWangLingExplore(Scene *scene)
: SpecialLogic(scene), m_is_finish(true), m_boss_num(0), m_next_reflush_boss_time(0), m_next_reflush_monster_time(0), m_reflush_notice_time(0), m_is_notice(false), 
m_boss_owner(INVALID_USER_ID), m_boss_objid(INVALID_OBJ_ID), m_boos_id(0),m_boss_max_hp(0),m_boss_cur_hp(0), m_send_boss_info_time(0),m_monster_die_num(0), m_next_check_buff_time(0)
{
}

SpecialWangLingExplore::~SpecialWangLingExplore()
{

}

void SpecialWangLingExplore::Update(unsigned long interval, time_t now_second)
{
	SpecialLogic::Update(interval, now_second);

	if (!ActivityShadow::Instance().IsActivtyOpen(ACTIVITY_TYPE_WANGLINGEXPLORE)) return;

	if (m_is_notice && now_second >= m_reflush_notice_time)
	{
		//BOSS刷新提示
		{
			m_boos_id = LOGIC_CONFIG->GetWangLingExploreConfig().GetBoosIdByWorldLevel(WorldShadow::Instance().GetWorldLevel());
			if (-1 != m_boos_id)
			{
				int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_wanglingexplore_reflush_boss_notice, 
					WANGLINGEXPLORE_REFLUSH_NOTICE_TIME, m_boos_id);
				if (length > 0)
				{
					this->NotifySystemMsg(gamestring::GAMESTRING_BUF, length, SYS_MSG_ACTIVE_NOTICE);
				}
			}
		}

		m_is_notice = false;
	}

	//检测是否超出BOSS归属权范围
	Monster *boss_monster = (Monster*)m_scene->GetObj(m_boss_objid);
	Role *boss_owner = m_scene->GetRoleByUID(UidToInt(m_boss_owner));
	if (NULL != boss_owner && NULL != boss_monster && Obj::OBJ_TYPE_MONSTER == boss_monster->GetObjType() && boss_monster->IsAlive())
	{
		Posi role_pos = boss_owner->GetPos();
		Posi boss_pos = boss_monster->GetPos();

		int distance = (boss_pos.y - role_pos.y) * (boss_pos.y - role_pos.y) + (boss_pos.x - role_pos.x) * (boss_pos.x - role_pos.x);
		if (distance > WANGLINGEXPLORE_OWNER_ROLE_AND_BOSS_DISTANCE * WANGLINGEXPLORE_OWNER_ROLE_AND_BOSS_DISTANCE)
		{
			m_boss_owner = INVALID_USER_ID;
			boss_monster->GetMonsterAI()->SetFirstHitUser(INVALID_UNIQUE_USER_ID);
		}
	}


	this->CheckBornBoss(now_second);
	this->CheckBornMonster(now_second);

	if (now_second >= m_send_boss_info_time)
	{
		m_send_boss_info_time = static_cast<unsigned int>(now_second) + 1;
		this->CheckSendBossInfo();
	}

	if (m_next_check_buff_time >0 && now_second >= m_next_check_buff_time)
	{
		m_next_check_buff_time = static_cast<unsigned int>(now_second) + 1;
		this->CheckGatherBuffInfo();
	}
}

void SpecialWangLingExplore::CheckBornBoss(time_t now_second)
{
	if (now_second < m_next_reflush_boss_time)
	{
		return;
	}

	if (m_boss_num != 0)
	{
		return;
	}

	const WangLingExploreBossCfg &boss_cfg = LOGIC_CONFIG->GetWangLingExploreConfig().GetWangLingExploreBossCfg();

	Posi born_pos;
	if (!LOGIC_CONFIG->GetWangLingExploreConfig().GetRandomFlushPos(born_pos, boss_cfg.pos_id_start, boss_cfg.pos_id_len))
	{
		return;
	}
	
	m_boos_id = LOGIC_CONFIG->GetWangLingExploreConfig().GetBoosIdByWorldLevel(WorldShadow::Instance().GetWorldLevel());
	if (-1 == m_boos_id) return;

	Monster *boss_monster = MONSTERPOOL->CreateMonster(static_cast<UInt16>(m_boos_id), m_scene, born_pos, 0, 0, 0, WorldShadow::Instance().GetWorldLevel());
	if (NULL != boss_monster)
	{
		int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_wanglingexplore_wangling_boss_come, m_boos_id );
		if (length > 0)
		{
			this->NotifySystemMsg(gamestring::GAMESTRING_BUF, length, SYS_MSG_ACTIVE_NOTICE);
		}

		++m_boss_num;
		m_boss_objid = boss_monster->GetId();
		m_boss_max_hp = boss_monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
		m_boss_cur_hp = boss_monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
		this->SendBossInfo();
	}

	this->SendWangLingExploreInfoToAllUser();
}

void SpecialWangLingExplore::CheckBornMonster(time_t now_second)
{
	const WangLingExploreMonsterCfg &monster_cfg = LOGIC_CONFIG->GetWangLingExploreConfig().GetWangLingExploreMonsterCfg();
	if (now_second < m_next_reflush_monster_time || m_monster_die_num > monster_cfg.count)
	{
		return;
	}

	Posi born_pos;

	for (int k = 0; k < m_monster_die_num; ++ k)
	{
		if (!LOGIC_CONFIG->GetWangLingExploreConfig().GetRandomFlushPos(born_pos, monster_cfg.pos_id_start, monster_cfg.pos_id_len))
		{
			continue;
		}

		MONSTERPOOL->CreateMonster(static_cast<UInt16>(monster_cfg.monster_id), m_scene, born_pos, 0, 0, 0, WorldShadow::Instance().GetWorldLevel());
	}

	int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_wanglingexplore_already_reflush_monster_notice, monster_cfg.monster_id);
	if (length > 0) 
	{
		this->NotifySystemMsg(gamestring::GAMESTRING_BUF, length, SYS_MSG_ACTIVE_NOTICE);
	}

	m_monster_die_num = 0;

	m_next_reflush_monster_time = EngineAdapter::Instance().Time() + monster_cfg.reflush_time_s;

	this->SendWangLingExploreInfoToAllUser();
}

bool SpecialWangLingExplore::SpecialIsEnemy(Role *role, Obj *obj)
{
// 	switch (obj->GetObjType())	
// 	{
// 	case Obj::OBJ_TYPE_MONSTER:
// 		{
// 			Monster *monster = (Monster*)obj;
// 			return monster->IsAlive();
// 		}
// 		break;
// 
// 	case Obj::OBJ_TYPE_ROLE:
// 		{
// 			Role *other_role = (Role*)obj;
// 			if (other_role == role)
// 			{
// 				return false;
// 			}
// 
// 			if (!role->GetTeamInfo()->InTeam() || !other_role->GetTeamInfo()->InTeam() || (role->GetTeamInfo()->GetTeamIndex() != other_role->GetTeamInfo()->GetTeamIndex()))
// 			{
// 				if (other_role->GetUserId() == m_boss_owner)
// 				{
// 					return other_role->IsAlive();
// 				}
// 
// 				GuildID role_guilid = role->GetGuildID();
// 				GuildID other_guild = other_role->GetGuildID();
// 
// 				if (INVALID_GUILD_ID == role_guilid || other_guild != role_guilid)
// 				{
// 					return other_role->IsAlive();
// 				}
// 			}
// 		}
// 		break;
// 
// 	case Obj::OBJ_TYPE_ROLE_SHADOW:
// 		{
// 			RoleShadow *target_role_shadow = (RoleShadow*)obj;
// 			return target_role_shadow->IsAlive() && role->RoleShadowIsEnemy(target_role_shadow);
// 		}
// 		break;
// 	}
// 
// 	return false;

	return SpecialLogic::SpecialIsEnemy(role, obj);
}

bool SpecialWangLingExplore::SpecialIsFriend(Role *role, Obj *obj, bool check_alive /*= true*/)
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
			GuildID role_guilid = role->GetGuildID();
			GuildID other_guild = other_role->GetGuildID();

			if ((role->GetTeamInfo()->InTeam() && other_role->GetTeamInfo()->InTeam() && (role->GetTeamInfo()->GetTeamIndex() == other_role->GetTeamInfo()->GetTeamIndex())) || (INVALID_GUILD_ID != role_guilid && other_guild == role_guilid))
			{
				return true;
			}
		}
		break;
	}

	return false;
}

void SpecialWangLingExplore::OnAttack(Character *cha, ObjID enemy_id, Attribute delta_hp)
{
	if (NULL == cha || INVALID_OBJ_ID == enemy_id) return;

	Obj *attacker = m_scene->GetObj(enemy_id);
	if (NULL == attacker || !attacker->IsCharactor(attacker->GetObjType())) return;

	if (Obj::OBJ_TYPE_ROLE != attacker->GetObjType()) return;

	if (Obj::OBJ_TYPE_MONSTER == cha->GetObjType())
	{
		Monster *monster = (Monster*)cha;
		if (NULL == monster) return;

		if (m_boos_id == monster->GetMonsterId())
		{

			Role *attacker_role = (Role*)attacker;
			if(NULL == attacker_role) return;

			if (m_boss_owner == INVALID_USER_ID)
			{
				m_boss_owner = attacker_role->GetUserId();
				this->SendWangLingExploreInfoToAllUser();
			}
		}
	}
}

void SpecialWangLingExplore::OnCharacterDie(Character *dead_cha, ObjID killer_objid)
{
	if (NULL == dead_cha || INVALID_OBJ_ID == killer_objid) return;

	const WangLingExploreBossCfg &boss_cfg = LOGIC_CONFIG->GetWangLingExploreConfig().GetWangLingExploreBossCfg();
	const WangLingExploreMonsterCfg &monster_cfg = LOGIC_CONFIG->GetWangLingExploreConfig().GetWangLingExploreMonsterCfg();

	Obj *killer_obj = m_scene->GetObj(killer_objid);
	if (NULL == killer_obj)
	{
		return;
	}

	if (Obj::OBJ_TYPE_ROLE != killer_obj->GetObjType())
	{
		// 怪物杀人
		if (Obj::OBJ_TYPE_MONSTER == killer_obj->GetObjType() && Obj::OBJ_TYPE_ROLE == dead_cha->GetObjType())
		{
			Role *dead_role = (Role *)dead_cha;
			WangLingExploreUser *dead_user = this->GetWangLingExploreUser(dead_role->GetUserId());
			if (NULL != dead_user)
			{
				dead_user->lianzhan = 0;
				dead_user->hurt_map.clear();
			}
		}
		return;
	}

	Role *kill_role = (Role*)killer_obj;

	if (Obj::OBJ_TYPE_MONSTER == dead_cha->GetObjType())
	{
		Monster *dead_monster = (Monster*)dead_cha;
		if (dead_monster->GetMonsterId() == m_boos_id)
		{
			--m_boss_num;
			m_boss_owner = INVALID_USER_ID;
			m_boss_objid = INVALID_OBJ_ID;

			m_next_reflush_boss_time = EngineAdapter::Instance().Time() + boss_cfg.reflush_time_s;
			m_reflush_notice_time = m_next_reflush_boss_time - WANGLINGEXPLORE_REFLUSH_NOTICE_TIME;
			m_is_notice = true;	

			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_wanglingexplore_kill_boss, 
				kill_role->GetUID(), kill_role->GetName(), (int)kill_role->GetCamp(), m_boos_id);
			if (length > 0)
			{
				this->NotifySystemMsg(gamestring::GAMESTRING_BUF, length, SYS_MSG_ACTIVE_NOTICE);
			}

			this->OnKillBoss(kill_role);
		}
		else if(dead_monster->GetMonsterId() == monster_cfg.monster_id)
		{
			m_monster_die_num ++;
		}

		this->CheckUserTaskInfo(kill_role, WANGLINGEXPLORE_TYPE_KILL_MONSTER, dead_monster->GetMonsterId());
	}

	if (Obj::OBJ_TYPE_ROLE == dead_cha->GetObjType())
	{
		Role *dead_role = (Role *)dead_cha;

		WangLingExploreUser *killer_user = this->GetWangLingExploreUser(kill_role->GetUserId());
		WangLingExploreUser *dead_user = this->GetWangLingExploreUser(dead_role->GetUserId());
		if (NULL != killer_user && NULL != dead_user)
		{
			for (std::map<UserID, time_t>::iterator it = dead_user->hurt_map.begin(), 
				end = dead_user->hurt_map.end(); it != end; ++ it)
			{
				if (it->second + ASSIST_LIMIT_TIME >= EngineAdapter::Instance().Time() && it->first != kill_role->GetUserId())
				{
					WangLingExploreUser *joiner_assist = this->GetWangLingExploreUser(it->first);
					if (NULL != joiner_assist)
					{
						joiner_assist->assist++;
						joiner_assist->lianzhan++;
						Role *assist_role = m_scene->GetRoleByUID(UidToInt(joiner_assist->user_id));
						if (NULL != assist_role)
						{
							this->CheckLianZhanNotice(assist_role, dead_role, true);
						}
					}
				}
			}

			killer_user->lianzhan++;

			this->CheckLianZhanNotice(kill_role, dead_role);

			dead_user->lianzhan = 0;
			dead_user->hurt_map.clear();
		}

		if (dead_role->GetUserId() == m_boss_owner)
		{
			Obj *boss_obj = m_scene->GetObj(m_boss_objid);
			if (NULL == boss_obj)
			{
				return;
			}

			if (Obj::OBJ_TYPE_MONSTER == boss_obj->GetObjType())
			{
				Monster *boss_monster = (Monster*)boss_obj;

				boss_monster->GetMonsterAI()->SetFirstHitUser(kill_role->GetUniqueUserID());
				m_boss_owner = kill_role->GetUserId();
			}
		}

		this->SendWangLingExploreInfoToAllUser();
	}
}

void SpecialWangLingExplore::OnHurtOther(Character *cha, Character *target)
{
	if (Obj::OBJ_TYPE_ROLE == cha->GetObjType() && Obj::OBJ_TYPE_ROLE == target->GetObjType())
	{
		Role *hurt_role = (Role *)cha;
		Role *target_role = (Role *)target;

		if (NULL == hurt_role || NULL == target_role)
		{
			return;
		}

		WangLingExploreUser *target_joiner = this->GetWangLingExploreUser(target_role->GetUserId());
		if (NULL != target_joiner)
		{
			target_joiner->hurt_map[hurt_role->GetUserId()] = EngineAdapter::Instance().Time();
		}
	}
}

void SpecialWangLingExplore::OnKillBoss(Role *role)
{
	if (NULL == role) return;

	const WangLingExploreGatherCfg &gather_cfg = LOGIC_CONFIG->GetWangLingExploreConfig().GetWangLingExploreGatherCfg();

	
	for (int i = 0; i < WangLingExploreGatherCfg::MAX_GATHER_CFG_COUNT; ++i)
	{
		WangLingExploreGatherCfg::GatherItem gitem = gather_cfg.gather_list[i];
		std::vector<Posi> born_pos;
		if (!LOGIC_CONFIG->GetWangLingExploreConfig().GetRandomFlushPos(born_pos, gitem.count, gitem.pos_id_start, gitem.pos_id_len))
		{
			return;
		}
		for(int j = 0; j < gather_cfg.gather_list[i].count && gather_cfg.gather_list[i].gather_id > 0; ++j)
		{
			GatherObj *gather_obj = new GatherObj();
			gather_obj->Init(NULL, gather_cfg.gather_list[i].gather_id, gather_cfg.gather_list[i].gather_time * 1000, 0, true,gather_cfg.gather_list[i].disappear_s);
			gather_obj->SetPos(born_pos[j]);
			m_scene->AddObj(gather_obj);
		}
	}

	this->SendWangLingExploreInfoToAllUser();
}

void SpecialWangLingExplore::OnGather(Role *role, int gather_id, ObjID obj_id, const Posi &pos)
{
	if (NULL == role) return;

	WangLingExploreUser *wanglingexplore_user = this->GetWangLingExploreUser(role->GetUserId());
	if (NULL == wanglingexplore_user) return;

	const WangLingExploreGatherCfg &gather_cfg = LOGIC_CONFIG->GetWangLingExploreConfig().GetWangLingExploreGatherCfg();

	bool is_baoxiang = false;
	bool is_other_reward = false;
	for (int i = 0; i < WangLingExploreGatherCfg::MAX_GATHER_CFG_COUNT; ++i)
	{
		if (gather_id == gather_cfg.gather_list[WangLingExploreGatherCfg::MAX_GATHER_CFG_COUNT - 1].gather_id)		// 双倍奖励
		{
			is_other_reward = true;
			is_baoxiang = true;
			break;
		}

		if (gather_id == gather_cfg.gather_list[i].gather_id)
		{
			is_baoxiang = true;
			break;
		}
	}

	if (is_baoxiang)
	{
		if (is_other_reward)
		{
			const WangLingExploreOtherCfg & other_cfg = LOGIC_CONFIG->GetWangLingExploreConfig().GetWangLingExploreOtherCfg();

			role->GetKnapsack()->PutListOrMail(other_cfg.reward_gold_box_item_list, PUT_REASON_WANGLINGEXPLORE_ACTIVITY_REWARD);
			role->GetKnapsack()->GetMoney()->AddGoldBind(other_cfg.reward_gold_box_bind_diamone, "wanglingexplore_reward");
		}
		else
		{
			const RewardPool::RewardItem * item_cfg = LOGIC_CONFIG->GetWangLingExploreConfig().GetRandomRewardItem();
			if (NULL == item_cfg) return;

			role->GetKnapsack()->PutOrMail(item_cfg->item, PUT_REASON_WANGLINGEXPLORE_ACTIVITY_REWARD);
			role->GetKnapsack()->GetMoney()->AddGoldBind(item_cfg->reward_bind_cost, "wanglingexplore_reward");
		}
	}

	// 采集任务
	this->CheckUserTaskInfo(role, WANGLINGEXPLORE_TYPE_GATHER, gather_id);

	this->SendWangLingExploreUserInfo(role);
	role->NoticeNum(noticenum::NT_WANGLINGEXPLORE_GATHER_SUCC);
}

void SpecialWangLingExplore::OnRoleEnterScene(Role *role)
{
	if (m_wanglingexplore_usermap.find(role->GetUserId()) == m_wanglingexplore_usermap.end())
	{
		ROLE_LOG_QUICK6(LOG_TYPE_ACTIVITY_WANGLINGEXPLORE, role, role->GetLevel(), 0, "Join", NULL);
	}

	WangLingExploreUser *wanglingexplore_user = this->GetWangLingExploreUser(role->GetUserId());
	if (NULL == wanglingexplore_user) return;

	role->GetName(wanglingexplore_user->user_name);

	if (ActivityShadow::Instance().IsActivtyOpen(ACTIVITY_TYPE_WANGLINGEXPLORE))
	{
		if (m_role_task_time_map.find(role->GetUID()) == m_role_task_time_map.end())
		{
			m_role_task_time_map[role->GetUID()] = EngineAdapter::Instance().Time() + LOGIC_CONFIG->GetWangLingExploreConfig().GetWangLingExploreOtherCfg().double_reward_limit_time;
		}
	}

	this->SendWangLingExploreUserInfo(role);
	this->SendBossInfo(role);
	EventHandler::Instance().OnEnterWangLingExplore(role);
}

void SpecialWangLingExplore::OnRoleLeaveScene(Role *role, bool is_logout)
{
	if (NULL == role) return;

	if (role->GetUserId() == m_boss_owner)
	{
		m_boss_owner = INVALID_USER_ID;
	}
}

int SpecialWangLingExplore::GetSpecialParam(Role *role)
{
	if (NULL == role) return 0;

	WangLingExploreUser * user = this->GetWangLingExploreUser(role->GetUserId());

	if (NULL == user) return 0;

	unsigned int now_sec = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	
	return user->wangling_buff_timestamp > now_sec;
}

bool SpecialWangLingExplore::SpecialStopGahterOnAttack(Role *role)
{
	if (role == NULL) return true;

	WangLingExploreUser * user = this->GetWangLingExploreUser(role->GetUserId());
	if (user == NULL) return true;

	if (user->wangling_buff_timestamp >= (unsigned int)(EngineAdapter::Instance().Time()))
	{
		return false;
	}

	return true;
}

void SpecialWangLingExplore::OnActivityStatusChange(int status)
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
			WorldStatus::Instance().OnActivityFinish(ACTIVITY_FIND_TYPE_WANGLINGEXPLORE);
			this->OnActivityClose();
		}
		break;
	}
}

// 完成任务自动给奖励(奖励至少有四种物品)
void SpecialWangLingExplore::OnFetchTaskExtraReward(Role *role)
{
	if (NULL == role) return;

	WangLingExploreUser *wanglingexplore_user = this->GetWangLingExploreUser(role->GetUserId());
	if (NULL == wanglingexplore_user) return;

	const WangLingExploreExtraRewardCfg  &reward_cfg = LOGIC_CONFIG->GetWangLingExploreConfig().GetTaskExtraReward();

	if (wanglingexplore_user->task_finish_num == WANGLINGEXPLORE_TASK_MAX && 0 == wanglingexplore_user->is_fetch_reward)
	{
		wanglingexplore_user->is_fetch_reward = 1;

		if (role->GetKnapsack()->PutOrMail(reward_cfg.item1, PUT_REASON_WANGLINGEXPLORE_ACTIVITY_REWARD))
		{
			wanglingexplore_user->role_get_item_map[reward_cfg.item1.item_id] += reward_cfg.item1.num;
		}

		if (role->GetKnapsack()->PutOrMail(reward_cfg.item2, PUT_REASON_WANGLINGEXPLORE_ACTIVITY_REWARD))
		{
			wanglingexplore_user->role_get_item_map[reward_cfg.item2.item_id] += reward_cfg.item2.num;
		}

		if (role->GetKnapsack()->PutOrMail(reward_cfg.item3, PUT_REASON_WANGLINGEXPLORE_ACTIVITY_REWARD))
		{	
			wanglingexplore_user->role_get_item_map[reward_cfg.item3.item_id] += reward_cfg.item3.num;
		}

		if (role->GetKnapsack()->PutOrMail(reward_cfg.item4, PUT_REASON_WANGLINGEXPLORE_ACTIVITY_REWARD))
		{	
			wanglingexplore_user->role_get_item_map[reward_cfg.item4.item_id] +=  reward_cfg.item4.num;
		}

// 		int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_wanglingexplore_finish_task_getgift, role->GetUID(), role->GetName(), role->GetCamp());
// 		if (length > 0)
// 		{
// 			this->NotifySystemMsg(gamestring::GAMESTRING_BUF, length, SYS_MSG_ACTIVE_NOTICE);
// 		}

		this->SendWangLingExploreUserInfo(role);
		role->NoticeNum(noticenum::NT_WANGLINGEXPLORE_REWARD_ITEM);
		EventHandler::Instance().OnWanglingFinishTask(role);

		return;
	}
}

WangLingExploreUser * SpecialWangLingExplore::GetWangLingExploreUser(const UserID &user_id)
{
	WangLingExploreUserMapIt it = m_wanglingexplore_usermap.find(user_id);
	if (it == m_wanglingexplore_usermap.end())								// 不存在 插入新的用户
	{
		WangLingExploreUser wanglingexplore_user;
		wanglingexplore_user.user_id = user_id;

		WangLingExploreTaskInfo wanglingexplore_task_list[WANGLINGEXPLORE_TASK_MAX];
		LOGIC_CONFIG->GetWangLingExploreConfig().GetWangLingExploreTaskList(wanglingexplore_task_list);

		int index_one = 0;				//五个任务随机选两个做为限时任务
		int index_two = 0;
		while(index_one == index_two)
		{
			index_one =	1 + RandomNum(WANGLINGEXPLORE_TASK_MAX);
			index_two = 1 + RandomNum(WANGLINGEXPLORE_TASK_MAX);
		}

		for (int i = 0; i < WANGLINGEXPLORE_TASK_MAX; i++)
		{
			if (wanglingexplore_task_list[i].task_id >= 0)	
			{
				if (index_one == wanglingexplore_task_list[i].task_id)
				{
					wanglingexplore_task_list[i].is_double_reward = true;
				}

				if (index_two == wanglingexplore_task_list[i].task_id)
				{
					wanglingexplore_task_list[i].is_double_reward = true;
				}

				wanglingexplore_user.wanglingexplore_task_list.push_back(wanglingexplore_task_list[i]);
			}
		}

		m_wanglingexplore_usermap[user_id] = wanglingexplore_user;
	}

	if (it == m_wanglingexplore_usermap.end()) it = m_wanglingexplore_usermap.find(user_id); 

	if (it != m_wanglingexplore_usermap.end())
	{
		return &it->second;
	}

	return NULL;
}
void SpecialWangLingExplore::OnBuyGatherBuff(Role * role)
{
	if (NULL == role) return;

	WangLingExploreUser * user = this->GetWangLingExploreUser(role->GetUserId());
	if (NULL == user) return;

	const WangLingExploreOtherCfg & other_cfg = LOGIC_CONFIG->GetWangLingExploreConfig().GetWangLingExploreOtherCfg();
	if (!role->GetKnapsack()->GetMoney()->UseAllGold(other_cfg.gather_buff_gold,"WangLingExplore::BuyBuff"))
	{
		role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	user->is_brocast_gather_buff = false;
	user->wangling_buff_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + other_cfg.gather_buff_time;

	if (m_next_check_buff_time <= 0)
	{
		m_next_check_buff_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) +1;
	}
	
	this->SendWangLingExploreUserInfo(role);
	this->NotifyGatherBuffInfo(role, 1);
}

void SpecialWangLingExplore::OnActivityStandby()
{
	m_wanglingexplore_usermap.clear();
	m_role_task_time_map.clear();
	m_is_finish = false;
}

void SpecialWangLingExplore::OnActivityStart()
{
	const WangLingExploreMonsterCfg  &monster_cfg = LOGIC_CONFIG->GetWangLingExploreConfig().GetWangLingExploreMonsterCfg();

	m_next_reflush_monster_time = EngineAdapter::Instance().Time() + monster_cfg.start_reflush_time_s;

	const WangLingExploreBossCfg  &boss_cfg = LOGIC_CONFIG->GetWangLingExploreConfig().GetWangLingExploreBossCfg();

	m_next_reflush_boss_time = EngineAdapter::Instance().Time() + boss_cfg.start_reflush_time_s;
	m_reflush_notice_time = m_next_reflush_boss_time - WANGLINGEXPLORE_REFLUSH_NOTICE_TIME;
	m_is_notice = true;	

	m_monster_die_num = monster_cfg.count;

	int role_num = m_scene->RoleNum();
	for (int i = 0; i < role_num; ++i)
	{
		Role * role = m_scene->GetRoleByIndex(i);
		if (NULL != role)
		{
			if (m_role_task_time_map.find(role->GetUID()) == m_role_task_time_map.end())
			{
				m_role_task_time_map[role->GetUID()] = EngineAdapter::Instance().Time() + LOGIC_CONFIG->GetWangLingExploreConfig().GetWangLingExploreOtherCfg().double_reward_limit_time;
			}
			this->SendWangLingExploreUserInfo(role);
		}
	}
	
}

void SpecialWangLingExplore::OnActivityClose()
{
	this->OnWangLingExploreClose();

	m_is_finish = true;
	m_boss_num = 0;
	m_boss_owner = INVALID_USER_ID;
	m_boss_objid = INVALID_OBJ_ID;

	// 移除怪物
	this->KillAllMonster();

	// 自动传出
	this->DelayKickOutAllRole();

}

void SpecialWangLingExplore::OnWangLingExploreClose()
{
	for (WangLingExploreUserMapIt it = m_wanglingexplore_usermap.begin(); it != m_wanglingexplore_usermap.end(); ++it)
	{
		WangLingExploreUser *wanglingexplore_user = &(it->second);

		for (std::vector< WangLingExploreTaskInfo>::iterator task_it = wanglingexplore_user-> wanglingexplore_task_list.begin(); task_it != wanglingexplore_user-> wanglingexplore_task_list.end(); ++task_it)
		{
			WangLingExploreTaskInfo &taskinfo = *task_it;

			{
				// 王陵探险任务日志
				if (taskinfo.IsFinish())
				{
					gamelog::g_log_roleactivity.printf(LL_INFO, "Activitywanglingexplore::JoinRecord user[%d, %s], complete_task_id[%d]",
						UidToInt(wanglingexplore_user->user_id), wanglingexplore_user->user_name, task_it->task_id);

					LOG_QUICK8(gamelog::g_log_quick, LOG_TYPE_ACTIVITY_WANGLINGEXPLORE, UidToInt(wanglingexplore_user->user_id), wanglingexplore_user->user_name, NULL, 0, task_it->task_id, "Close", NULL);
				}
			}
		}
	}
}

void SpecialWangLingExplore::CheckUserTaskInfo(Role *role, short task_type, int param_id)
{
	if (NULL == role) return;

	Role *member_list[16] = {NULL};
	int team_rolenum = role->GetTeamInfo()->GetOtherMemberInTheSameScene(member_list, sizeof(member_list) / sizeof(member_list[0]));

	if (team_rolenum < static_cast<int>(sizeof(member_list) / sizeof(member_list[0]))) member_list[team_rolenum ++] = role;

	for (int r = 0; r < team_rolenum; r++)
	{
		if (NULL == member_list[r]) continue;

		WangLingExploreUser *wanglingexplore_user = this->GetWangLingExploreUser(member_list[r]->GetUserId());
		if (NULL != wanglingexplore_user)
		{
			for (std::vector<WangLingExploreTaskInfo>::iterator it = wanglingexplore_user->wanglingexplore_task_list.begin(), end = wanglingexplore_user->wanglingexplore_task_list.end(); it != end; ++ it)
			{
				WangLingExploreTaskInfo &taskinfo = *it;
				if (!taskinfo.IsFinish() && taskinfo.IsMatch(task_type, param_id))
				{
					taskinfo.AddParamVal();

					if (taskinfo.IsFinish() && !taskinfo.IsFetchReward())
					{
						for(int i = 0; i < taskinfo.item_count && i < MAX_ATTACHMENT_ITEM_NUM; i++)
						{
							wanglingexplore_user->role_get_item_map[taskinfo.reward_item[i].item_id] += taskinfo.reward_item[i].num;
						}

						++ wanglingexplore_user->task_finish_num;
						taskinfo.is_fetch_reward = true;
						member_list[r]->GetKnapsack()->PutListOrMail(taskinfo.reward_item, PUT_REASON_WANGLINGEXPLORE_ACTIVITY_REWARD);

						//const int task_exp_reward = LOGIC_CONFIG->GetWangLingExploreConfig().GetTaskRewardExp(member_list[r]->GetLevel());

						long long task_exp_reward = 0;

						const WangLingExploreTaskInfo *task_cfg = LOGIC_CONFIG->GetWangLingExploreConfig().GetWangLingExploreTaskCfg(taskinfo.task_id);
						if (NULL != task_cfg)
						{
							//策划需求，修改增加经验规则（2018-1-3）
							task_exp_reward = static_cast<long long>(task_cfg->exp_factor_task * (LOGIC_CONFIG->GetItemLevelRewardConfig().GetExpDanReward(role->GetLevel(), 0)));
						}

						if (task_exp_reward > 0)
						{
							member_list[r]->AddExp(task_exp_reward, "wanglingexplore_reward", Role::EXP_ADD_REASON_DEFAULT);
						}

						std::map<int, time_t>::iterator it = m_role_task_time_map.find(UidToInt(wanglingexplore_user->user_id));
						if (it != m_role_task_time_map.end())
						{
							if (taskinfo.IsDoubleReward() && EngineAdapter::Instance().Time() <= it->second)
							{
								for(int i = 0; i < taskinfo.item_count && i < MAX_ATTACHMENT_ITEM_NUM; i++)
								{
									wanglingexplore_user->role_get_item_map[taskinfo.reward_item[i].item_id] += taskinfo.reward_item[i].num;
								}

								member_list[r]->GetKnapsack()->PutListOrMail(taskinfo.reward_item, PUT_REASON_WANGLINGEXPLORE_ACTIVITY_REWARD);

								if (task_exp_reward > 0)
								{
									member_list[r]->AddExp(task_exp_reward, "wanglingexplore_reward", Role::EXP_ADD_REASON_DEFAULT);
								}
							}
						}

						member_list[r]->NoticeNum(noticenum::NT_WANGLINGEXPLORE_REWARD_ITEM);
// 						if(wanglingexplore_user->task_finish_num < WANGLINGEXPLORE_TASK_MAX)
// 						{
// 							int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_wanglingexplore_finish_task,
// 								member_list[r]->GetUID(), member_list[r]->GetName(), member_list[r]->GetCamp(), taskinfo.task_id);
// 							if (length > 0)
// 							{
// 								this->NotifySystemMsg(gamestring::GAMESTRING_BUF, length, SYS_MSG_ACTIVE_NOTICE);
// 							}
// 						}

						role->GetMagicalPreciousManager()->AddConditionTimes(CONDITION_TYPE_WANGLING_TASK, 1);
					}
				}
			}
		}

		this->OnFetchTaskExtraReward(member_list[r]);

		this->SendWangLingExploreUserInfo(member_list[r]);
	}
}

void SpecialWangLingExplore::NotifySystemMsg(char *str_buff, int str_len, int notice_type)
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

void SpecialWangLingExplore::SendWangLingExploreInfoToAllUser()
{
	for (int i = 0; i < (int)m_scene->RoleNum(); ++ i)
	{
		Role *role = m_scene->GetRoleByIndex(i);
		if (NULL != role)
		{
			this->SendWangLingExploreUserInfo(role);
		}
	}
}

void SpecialWangLingExplore::SendWangLingExploreUserInfo(Role *role)
{
	if (NULL == role) return;

	WangLingExploreUser *wanglingexplore_user = this->GetWangLingExploreUser(role->GetUserId());
	if (NULL == wanglingexplore_user) return;

	static Protocol::SCWangLingExploreUserInfo cmd;

	{
		std::map<int, time_t>::iterator it = m_role_task_time_map.find(role->GetUID());
		if (it != m_role_task_time_map.end())
		{
			cmd.limit_task_time = (int)it->second;
		}
		else
		{
			cmd.limit_task_time = 0;
		}
	}

	cmd.boss_reflush_time = (int)m_next_reflush_boss_time;
	cmd.boss_num = m_boss_num;
	cmd.boss_owner_uid = UidToInt(m_boss_owner);
	cmd.gather_buff_time = wanglingexplore_user->wangling_buff_timestamp;

	memset(cmd.reward_list, 0, sizeof(cmd.reward_list));

	if (wanglingexplore_user->is_fetch_reward && !wanglingexplore_user->is_send_item_info)
	{
		int i = 0;
		for(std::map<int, int>::iterator it = wanglingexplore_user->role_get_item_map.begin(), end = wanglingexplore_user->role_get_item_map.end(); it != end && i < WANGLINGEXPLORE_MAX_REWARD_ITEM_COUNT; ++it, ++i)
		{
			cmd.reward_list[i].item_id = it->first;
			cmd.reward_list[i].num = it->second;
		}

		wanglingexplore_user->is_send_item_info = true;
	}

	int task_count = 0;
	for (std::vector<WangLingExploreTaskInfo>::iterator it = wanglingexplore_user->wanglingexplore_task_list.begin(), end = wanglingexplore_user->wanglingexplore_task_list.end();
		it != end; ++ it)
	{
		if (task_count >= 0 && task_count < WANGLINGEXPLORE_TASK_MAX)
		{
			const WangLingExploreTaskInfo *task_cfg = LOGIC_CONFIG->GetWangLingExploreConfig().GetWangLingExploreTaskCfg(it->task_id);
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

void SpecialWangLingExplore::CheckLianZhanNotice(Role *kill_role, Role *dead_role, bool is_assist)
{
	return;
	if (NULL != kill_role && NULL != dead_role)
	{
		WangLingExploreUser *killer = this->GetWangLingExploreUser(kill_role->GetUserId());
		WangLingExploreUser *dead = this->GetWangLingExploreUser(dead_role->GetUserId());
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

void SpecialWangLingExplore::SendBossInfo(Role *role)
{
	Protocol::WangLingExploreBossInfo boss_info;
	
	boss_info.monster_id = m_boos_id;
	boss_info.max_hp = m_boss_max_hp;
	boss_info.cur_hp = m_boss_cur_hp;

	if (role != nullptr)
	{
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&boss_info, sizeof(boss_info));
	}
	else
	{
		m_scene->SendToRole((const char*)&boss_info, sizeof(boss_info));
	}
}

void SpecialWangLingExplore::CheckSendBossInfo()
{
	if (INVALID_OBJ_ID == m_boos_id) return;
	Monster *monster = m_scene->GetMonster(m_boss_objid);
	if (NULL != monster)
	{
		long long cur_hp = monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
		if (m_boss_cur_hp != cur_hp)
		{
			m_boss_cur_hp = cur_hp;
			this->SendBossInfo();
		}
	}
}

void SpecialWangLingExplore::CheckGatherBuffInfo()
{
	int role_num = m_scene->RoleNum();
	for (int i = 0; i < role_num; ++i)
	{
		Role * temp_role = m_scene->GetRoleByIndex(i);
		if (NULL != temp_role)
		{
			WangLingExploreUser * user = this->GetWangLingExploreUser(temp_role->GetUserId());
			if (NULL != user)
			{
				if (!user->is_brocast_gather_buff && user->wangling_buff_timestamp <= static_cast<unsigned int>(EngineAdapter::Instance().Time()))
				{
					user->is_brocast_gather_buff = true;
					this->NotifyGatherBuffInfo(temp_role, 0);
				}
			}
		}
	}
}

void SpecialWangLingExplore::NotifyGatherBuffInfo(Role *role, short is_has_buff)
{
	if (NULL == role) return;

	Protocol::SCNoticeGatherBuffInfo cmd;
	cmd.obj_id = role->GetId();
	cmd.is_gather_wudi = is_has_buff;

	role->GetScene()->GetZoneMatrix()->NotifyAreaMsg(role->GetObserHandle(), (void*)&cmd, sizeof(cmd));
}
