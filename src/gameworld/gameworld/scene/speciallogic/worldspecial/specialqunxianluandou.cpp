#include "specialqunxianluandou.hpp"
#include "protocal/msgactivity.hpp"
#include "global/rank/rankmanager.hpp"

#include "scene/activityshadow/activityshadow.hpp"
#include "scene/scenemanager.h"

#include "servercommon/string/gameworldstr.h"
#include "servercommon/userprotocal/msgsystem.h"
#include "servercommon/string/gameworldstr.h"

#include "obj/character/battlefieldshenshi.hpp"
#include "obj/character/role.h"
#include "obj/character/monster.h"

#include "monster/monsterpool.h"
#include "ai/monsterai/monsteraibase.hpp"
#include "ai/monsterai/specialai/monsteraibattlefield.hpp"

#include "other/route/mailroute.hpp"
#include "other/event/eventhandler.hpp"
#include "config/logicconfigmanager.hpp"
#include "config/activityconfig/multibattlefieldskillconfig.hpp"

#include "world.h"
#include "gamelog.h"

#include "effect/skilleffect/effectonce.hpp"
#include "effect/skilleffect/effectwudiprotect.hpp"
#include "effect/skilleffect/effecthpstore.hpp"
#include "effect/skilleffect/effectbuff.hpp"
#include "effect/skilleffect/effectspecialstate.hpp"
#include "effect/skilleffect/effectmovespeed.hpp"
#include "effect/sceneeffect/sceneeffect.hpp"

#include "item/itempool.h"

#include "servercommon/errornum.h"

#include "internalcomm.h"
#include "servercommon/challengefielddef.hpp"
#include "global/challengefield/challengefield.hpp"
#include "global/challengefield/challengeuser.hpp"
#include "obj/character/roleshadow.hpp"
#include "globalconfig/globalconfig.h"
#include "config/activityconfig/qunxianluandouconfig.hpp"
#include "ai/roleai/roleaielementfield.hpp"

#include "scene/worldshadow/worldshadow.hpp"
#include "protocal/msgfight.h"
#include <algorithm>

#include "global/worldstatus/worldstatus.hpp"
#include "global/usercache/usercache.hpp"
#include "item/knapsack.h"
#include "other/vip/vip.hpp"
#include "other/mount/mountmanager.hpp"
#include "other/roleactivity/roleactivity.hpp"
#include "other/rolegoal/rolegoal.hpp"
#include "other/shop/roleshop.hpp"
#include "global/activity/activitymanager.hpp"
#include "global/activity/impl/activityqunxianluandou.hpp"
#include "gameworld/globalconfig/otherconfig.h"
#include "gameworld/global/activity/activitymsgmanager.h"
#include "config/worshipconfig.hpp"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void QunXianLuanDouUser::OnEnterActivity(Role *role)
{
	if (NULL == role) return;

	role->GetName(user_name);
	vip = (char)(role->GetVip()->GetVipLevel());
	sex = role->GetSex();
	prof = role->GetProf();
	camp = role->GetCamp();
	level = role->GetLevel();

	gsnetid = role->GetNetId(); 
	obj_id = role->GetId();

	last_leavetime = 0; 
	dead_time = 0;

	ROLE_LOG_QUICK6(LOG_TYPE_ACTIVITY_QUNXIANLUANDOU, role, role->GetLevel(), 0, "Join", NULL);
}

void QunXianLuanDouUser::OnHurt(const UserID &attack_userid, time_t now_second)
{
	hurt_map[attack_userid] = now_second;
}

void QunXianLuanDouUser::OnKill(SpecialQunXianLuanDou *special_logic, bool is_reward)
{
	++kills; 
	++lianzhan;

	Role *role = this->GetRole(special_logic->m_scene);
	if (NULL == role) return;

	if (lianzhan > lianzhan_max)
	{
		lianzhan_max = lianzhan;
	}

	int reward_times = special_logic->IsRewardDouble(side) ? 2 : 1;

	if (is_reward)
	{
		const int KILL_SCORE = LOGIC_CONFIG->GetQunXianLuanDouConfig().GetKillHonor() * reward_times;
		kill_honor += KILL_SCORE;

		special_logic->CheckPersonalScoreReward(role);
		special_logic->AddSideScore(side, KILL_SCORE);
	}

	special_logic->SendQunXianLuanDouUserInfo(this);
	special_logic->BroadcastLianzhanInfo(this);

	EventHandler::Instance().OnQunXianLuanDouKill(role);
}

void QunXianLuanDouUser::OnDead(SpecialQunXianLuanDou *special_logic, time_t _dead_time, bool reset_lianzhan) 
{ 
	int old_lianzhan = lianzhan;

	if (reset_lianzhan)
	{
		lianzhan = 0; 
	}

	dead_time = _dead_time; 
	hurt_map.clear();

	if (old_lianzhan != lianzhan)
	{
		special_logic->BroadcastLianzhanInfo(this);
	}
}

void QunXianLuanDouUser::OnAssist(SpecialQunXianLuanDou *special_logic)
{ 
	++ assists;
	++ lianzhan;

	int reward_times = special_logic->IsRewardDouble(side) ? 2 : 1;
	const int ASSIST_SCORE = LOGIC_CONFIG->GetQunXianLuanDouConfig().GetAssistHonor() * reward_times;

	assist_honor += ASSIST_SCORE;

	special_logic->SendQunXianLuanDouUserInfo(this);

	Role *role = this->GetRole(special_logic->m_scene);
	if (NULL != role && role->GetId() == obj_id)
	{
		EventHandler::Instance().OnQunXianLuanDouAssist(role);
	}

	special_logic->CheckPersonalScoreReward(role);
	special_logic->AddSideScore(side, ASSIST_SCORE);
}

void QunXianLuanDouUser::OnRobShenShi(SpecialQunXianLuanDou *special_logic) 
{ 
	++ rob_shenshi;

	const int ROB_SHENSHI_SCORE = LOGIC_CONFIG->GetQunXianLuanDouConfig().GetRobShenShiHonor();

	rob_shenshi_honor += ROB_SHENSHI_SCORE;
	special_logic->SendQunXianLuanDouUserInfo(this);

	Role *role = this->GetRole(special_logic->m_scene);
	if (NULL != role)
	{
		special_logic->CheckShenShiOwonerStatus(role, false);
	}
}

void QunXianLuanDouUser::OnSideMemberRobShenShi(SpecialQunXianLuanDou *special_logic)
{
	if (this->TotalHonor() >= 1) 
	{
		const int ROB_SHENSHI_SIDEHONOR = LOGIC_CONFIG->GetQunXianLuanDouConfig().GetRobShenShiSideHonor();

		rob_shenshi_honor += ROB_SHENSHI_SIDEHONOR;
		special_logic->SendQunXianLuanDouUserInfo(this);
	}
}

void QunXianLuanDouUser::OnFreeReward(SpecialQunXianLuanDou *special_logic, int reward_honor)
{
	Role *role = this->GetRole(special_logic->m_scene);
	if (NULL == role) return;

	free_reward_honor += reward_honor;

	special_logic->SendQunXianLuanDouUserInfo(this);

	special_logic->CheckPersonalScoreReward(role);
	special_logic->AddSideScore(side, reward_honor);
}

void QunXianLuanDouUser::OnExtraHonor(SpecialQunXianLuanDou *special_logic, int reward_honor)
{
	Role *role = this->GetRole(special_logic->m_scene);
	if (NULL == role) return;

	extra_honor += reward_honor;

	special_logic->SendQunXianLuanDouUserInfo(this);
	special_logic->CheckPersonalScoreReward(role);
	special_logic->AddSideScore(side, reward_honor);
}

Role * QunXianLuanDouUser::GetRole(Scene *scene)
{
	if (NULL != scene)
	{
		Obj *obj = scene->GetObj(obj_id);
		if (NULL != obj && Obj::OBJ_TYPE_ROLE == obj->GetObjType())
		{
			Role *role = (Role*)obj;
			if (role->GetUserId() == user_id)
			{
				return role;
			}
		}
	}

	return NULL;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SpecialQunXianLuanDou::SpecialQunXianLuanDou(Scene *scene) 
	: SpecialLogic(scene), m_isopen(false), m_has_calc_weak_side_protect(false), m_need_notify_sideinfo(false), m_rankinfo_count(0), m_shenshi_next_flush_timestamp(0),
	m_last_check_leaveuser_timestamp(0), m_last_check_rank_timestamp(0), m_next_free_reward_time(0), m_last_first_guild_broadcast_time(0), m_last_best_killer_broadcast_time(0),
	m_robot_max_count(0), m_robot_create_next_interval(0), m_robot_create_interval(0), m_robot_birth_pos(0, 0),m_kick_out_all_user_time(0), m_worship_start_time(0)
{
	this->Clear();
}

SpecialQunXianLuanDou::~SpecialQunXianLuanDou()
{

}

void SpecialQunXianLuanDou::OnRoleEnterScene(Role *role)
{
	if (NULL == role) return;
	this->SendLuckyInfoToUser(role);
}

void SpecialQunXianLuanDou::Update(unsigned long interval, time_t now_second)
  {
	SpecialLogic::Update(interval, now_second);

	this->CheckLeaveQunXianLuanDouUer(now_second);

	if (!m_isopen)
	{
		if (m_kick_out_all_user_time > 0 && now_second >= m_kick_out_all_user_time)
		{
			m_kick_out_all_user_time = 0;
			this->DelayKickOutAllRole();
		}

		// 活动结束后，才开始计算膜拜
		this->CheckWorshipReward(now_second);

		return;
	}


	if (now_second >= m_last_check_rank_timestamp + 1)
	{
		m_last_check_rank_timestamp = now_second;

		this->CheckUserRank();
	}

	if (0 != m_shenshi_next_flush_timestamp && now_second >= m_shenshi_next_flush_timestamp)
	{
		int shenshi_hp = 0; Posi shenshi_birthpos(0, 0);
		if (LOGIC_CONFIG->GetQunXianLuanDouConfig().GetShenShiBirthInfo(&shenshi_hp, &shenshi_birthpos))
		{
			BattleFieldShenShi *battlefield_shenshi = new BattleFieldShenShi();

			battlefield_shenshi->SetPos(shenshi_birthpos); 
			battlefield_shenshi->Init(shenshi_hp);
			m_scene->AddObj(battlefield_shenshi);
			m_shenshi_next_flush_timestamp = 0;

			m_shenshi_objidlist.insert(battlefield_shenshi->GetId());
			m_need_notify_sideinfo = true;
		}
	}

	if (m_next_free_reward_time > 0 && now_second >= m_next_free_reward_time)
	{
		m_next_free_reward_time = now_second + LOGIC_CONFIG->GetQunXianLuanDouConfig().GetFreeRewardIntervalS();
		int reward_honor = LOGIC_CONFIG->GetQunXianLuanDouConfig().GetFreeRewardHonor();

		for (QunXianLuanDouUserMapIt it = m_qunxianluandou_usermap.begin(), end = m_qunxianluandou_usermap.end(); it != end; ++ it)
		{
			QunXianLuanDouUser &user = it->second;

			user.OnFreeReward(this, reward_honor);
		}
	}
	if (m_need_notify_sideinfo) { this->SendSideInfoToUser(); }

	if (m_robot_create_next_interval > 0 && static_cast<unsigned int>(now_second) >= m_robot_create_next_interval)
	{
		m_robot_create_next_interval = static_cast<unsigned int>(now_second) + m_robot_create_interval;
		this->CreateRobot();
	}

	if (m_luck.next_lucky_timestamp > 0 && now_second >= m_luck.next_lucky_timestamp)
	{
		m_luck.next_lucky_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + LOGIC_CONFIG->GetQunXianLuanDouConfig().GetLuckInterval();

		this->GaveLuckyItemToUser();
	}

	{
		// 传闻
		if (static_cast<unsigned int>(EngineAdapter::Instance().Time()) > m_last_first_guild_broadcast_time + FIRST_GUILD_INTERVAL)
		{
			m_last_first_guild_broadcast_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());

			int side_max_score = m_sideinfo_list[QUNXIANLUANDOU_SIDE_1].score;
			int first_side = QUNXIANLUANDOU_SIDE_1;

			for (int i = QUNXIANLUANDOU_SIDE_1; i < QUNXIANLUANDOU_SIDE_MAX; ++ i)
			{
				if (m_sideinfo_list[i].score > side_max_score)
				{
					side_max_score = m_sideinfo_list[i].score;
					first_side = i;
				}
			}

			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_qunxianluandou_first_side, first_side);
			if (length > 0) 
			{
				this->NotifySystemMsg(gamestring::GAMESTRING_BUF, length, SYS_MSG_ACTIVITY_SPECIAL);
			}
		}

// 		if (static_cast<unsigned int>(EngineAdapter::Instance().Time()) > m_last_best_killer_broadcast_time + BEST_KILLER_INTERVAL)
// 		{
// 			m_last_best_killer_broadcast_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
// 			if (m_rankinfo_count > 0)
// 			{
// 				QunXianLuanDouUser *qunxianluandou_killuser = this->GetQunXianLuanDouUser(m_rankinfo_list[0].user_id);
// 				if (NULL != qunxianluandou_killuser && qunxianluandou_killuser->kills > 1)
// 				{
// 					const UserCacheNode *cache_user = UserCacheManager::Instance().GetUserNode(m_rankinfo_list[0].user_id);
// 					if (NULL != cache_user)
// 					{
// 						int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_guildbattle_best_killer,
// 							cache_user->uid, cache_user->user_name, (int)cache_user->camp, qunxianluandou_killuser->kills);
// 						if (length > 0)
// 						{
// 							this->NotifySystemMsg(gamestring::GAMESTRING_BUF, length, SYS_MSG_ACTIVE_NOTICE);
// 						}
// 					}
// 				}
// 			}//m_rankinfo_count > 0
// 		}
	}


// 	{
// 		// 计算弱阵营保护
// 		if (!m_has_calc_weak_side_protect)
// 		{
// 			const static unsigned int WEAK_SIDE_PROTECT_START_NEED_ACTIVITY_PASS_TIME_S = 25 * 60;
// 			unsigned int activity_begin_time = ActivityShadow::Instance().GetActivityBeginTime(ACTIVITY_TYPE_QUNXIANLUANDOU);
// 
// 			if (ActivityShadow::Instance().IsActivtyOpen(ACTIVITY_TYPE_QUNXIANLUANDOU) &&
// 				now_second >= activity_begin_time + WEAK_SIDE_PROTECT_START_NEED_ACTIVITY_PASS_TIME_S)
// 			{
// 				m_has_calc_weak_side_protect = true;
// 
// 				int total_score = 0, max_score = 0, max_score_side = -1;
// 				for (int i = 0; i < QUNXIANLUANDOU_SIDE_MAX; ++ i)
// 				{
// 					total_score += m_sideinfo_list[i].score;
// 
// 					if (m_sideinfo_list[i].score > max_score)
// 					{
// 						max_score = m_sideinfo_list[i].score;
// 						max_score_side = i;
// 					}
// 				}
// 
// 				bool is_weak_side_protect_effect = false;
// 
// 				for (int side = 0; side < QUNXIANLUANDOU_SIDE_MAX; ++ side)
// 				{
// 					if (total_score < max_score * 2 && side != max_score_side)
// 					{
// 						m_sideinfo_list[side].is_weak_side_protect = true;
// 
// 						is_weak_side_protect_effect = true;
// 					}
// 					else
// 					{
// 						m_sideinfo_list[side].is_weak_side_protect = false;
// 					}
// 				}
// 
// 				if (is_weak_side_protect_effect && max_score_side >= 0 && max_score_side < QUNXIANLUANDOU_SIDE_MAX)
// 				{
// 					int week_sides_index = 0;
// 					int week_sides[QUNXIANLUANDOU_SIDE_MAX - 1]; memset(week_sides, 0, sizeof(week_sides));
// 					{
// 						for (int i = 0; i < QUNXIANLUANDOU_SIDE_MAX && week_sides_index < QUNXIANLUANDOU_SIDE_MAX - 1; ++ i)
// 						{
// 							if (i != max_score_side)
// 							{
// 								week_sides[week_sides_index] = i;
// 								++ week_sides_index;
// 							}
// 						}
// 					}
// 
// 					if (QUNXIANLUANDOU_SIDE_MAX - 1 == week_sides_index)
// 					{
// 						UNSTD_STATIC_CHECK(QUNXIANLUANDOU_SIDE_MAX - 1 >= 2);
// 
// 						int len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_qunxian_luandou_weak_side_protect_notice, 
// 							week_sides[0], week_sides[1], max_score_side);
// 
// 						if (len > 0) 
// 						{
// 							this->NotifySystemMsg(gamestring::GAMESTRING_BUF, len, SYS_MSG_SPECIAL_SCENE_CENTER_AND_ROLL);
// 						}
// 					}
// 				}
// 			}
// 		}
// 	}
}

void SpecialQunXianLuanDou::OnAddObj(Obj *obj) // (在这里处理角色进入 主要是为了在enterscene之前改变角色坐标)
{
	//if (!m_isopen) return;

	if (Obj::OBJ_TYPE_ROLE == obj->GetObjType())
	{
		Role *role = (Role*)obj;

		QunXianLuanDouUser *qunxianluandou_user = this->GetQunXianLuanDouUser(role->GetUserId());
		if (NULL != qunxianluandou_user) 
		{
			if (qunxianluandou_user->InvalidSide())
			{
				int least_side = this->GetRoleNumLeastSide();
				if (QUNXIANLUANDOU_SIDE_1 <= least_side && least_side < QUNXIANLUANDOU_SIDE_MAX) // 2：该边人数+1
				{
					++ m_sideinfo_list[least_side].role_num;
				}

				qunxianluandou_user->SetSide(least_side);
			}

			Posi enterpos(0, 0);
			if (LOGIC_CONFIG->GetQunXianLuanDouConfig().GetEnterInfo(qunxianluandou_user->side, &enterpos))
			{
				role->SetPos(enterpos);
			}

			role->GetRoleActivity()->SetTempZhanchangSide(qunxianluandou_user->side);
			qunxianluandou_user->OnEnterActivity(role);
			EventHandler::Instance().OnEnterQuanXianLuanDou(role);
			
			{
				this->SendRankToUser(role);
				this->SendSideInfoToUser(role);
				this->SendQunXianLuanDouUserInfo(qunxianluandou_user);
			}
		}
	}
}

void SpecialQunXianLuanDou::OnRemoveObj(Obj *obj)
{
	if (!m_isopen) return;

	if (Obj::OBJ_TYPE_BATTLEFIELD_SHENSHI == obj->GetObjType())
	{
		BattleFieldShenShi *shenshi = (BattleFieldShenShi*)obj;
		this->OnBattleFiledShenShiDisappear(shenshi);
	}
	else if (Obj::OBJ_TYPE_ROLE_SHADOW == obj->GetObjType())
	{
		RoleAIBase *ai = ((RoleShadow*)obj)->GetRoleAI();
		if (NULL != ai && ROLE_SHADOW_TYPE_ELEMENT_FILED == ai->GetAIType())
		{
			RoleAIElementField *field_ai = (RoleAIElementField *)ai;
			int field_side = field_ai->GetSide();
			if (QUNXIANLUANDOU_SIDE_1 <= field_side && field_side < QUNXIANLUANDOU_SIDE_MAX)
			{
				--m_sideinfo_list[field_side].role_num;
			}
		}
	}
}

void SpecialQunXianLuanDou::OnRoleLeaveScene(Role *role, bool is_logout)
{
	//if (!m_isopen) return;

	unsigned int now_second = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	role->GetRoleActivity()->SetZhanchangRoomInfo(now_second, m_scene->GetSceneKey() - 1);

	this->CheckShenShiOwonerStatus(role, false);

	QunXianLuanDouUser *qunxianluandou_user = this->GetQunXianLuanDouUser(role->GetUserId());
	if (NULL != qunxianluandou_user)
	{
		qunxianluandou_user->OnLeaveActivity(now_second);
	}
}

void SpecialQunXianLuanDou::OnCharacterDie(Character *dead, ObjID killer_objid)
{
	if (!m_isopen) return;

	if (Obj::OBJ_TYPE_ROLE == dead->GetObjType())
	{
		this->OnRoleDead((Role*)dead, killer_objid);
	}
	else if (Obj::OBJ_TYPE_BATTLEFIELD_SHENSHI == dead->GetObjType())
	{
		this->OnBattleFiledShenShiDead((BattleFieldShenShi*)dead, killer_objid);
	}
	else if(Obj::OBJ_TYPE_ROLE_SHADOW == dead->GetObjType())
	{
	}
}

void SpecialQunXianLuanDou::OnAttack(Character *cha, ObjID attacker_id, Attribute delta_hp)
{
	if (!m_isopen) return;

	if (Obj::OBJ_TYPE_ROLE == cha->GetObjType())
	{
		Role *beattack_role = (Role*)cha;

		Role *attack_role = m_scene->GetPrincipal(attacker_id);
		if (NULL != attack_role)
		{
			QunXianLuanDouUser *qunxianluandou_beattackuser = this->GetQunXianLuanDouUser(beattack_role->GetUserId());
			if (NULL != qunxianluandou_beattackuser)
			{
				qunxianluandou_beattackuser->OnHurt(attack_role->GetUserId(), EngineAdapter::Instance().Time());
			}
		}
	}
}

bool SpecialQunXianLuanDou::SpecialRelive(Role *role, int realive_type)
{
	this->CheckShenShiOwonerStatus(role, false);

	if (Protocol::CSRoleReAliveReq::REALIVE_TYPE_BACK_HOME == realive_type)
	{
		QunXianLuanDouUser *qunxianluandou_user = this->GetQunXianLuanDouUser(role->GetUserId());
		if (NULL != qunxianluandou_user)
		{
			Posi realive_pos(0, 0);
			if (LOGIC_CONFIG->GetQunXianLuanDouConfig().GetRealivePos(qunxianluandou_user->side, &realive_pos))
			{
				if (!role->ReAlive(SPECIAL_ACTIVITY_FREE_REALIVE_HP_PER, 10, realive_pos))
				{
					return false;
				}	
				return true;
			}
		}
	}
	else
	{
// 		int relive_need_gold = LOGIC_CONFIG->GetOtherCfg().GetOtherCfg().cross_relive_gold;
// 		if (!role->GetKnapsack()->GetMoney()->UseAllGold(relive_need_gold, "SpecialCrossXiuLuoTa::SpecialRelive"))
// 		{
// 			role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
// 			return true;
// 		}
// 		role->ReAlive(100, 100);
// 
// 		return true;
	}
	
	return false;
}

bool SpecialQunXianLuanDou::SpecialIsEnemy(Role *role, Obj *obj)
{
	if (!m_isopen)
	{
		return false;
	}

	switch (obj->GetObjType())	
	{
	case  Obj::OBJ_TYPE_BATTLEFIELD_SHENSHI:
		{
			BattleFieldShenShi *battlefield_shenshi = (BattleFieldShenShi*)obj;
			return battlefield_shenshi->IsAlive();
		}
		break;

	case Obj::OBJ_TYPE_ROLE:
		{
			Role *target_role = (Role*)obj;

			bool isfriend = (role->GetRoleActivity()->GetTempZhanchangSide() == target_role->GetRoleActivity()->GetTempZhanchangSide());

			return target_role->IsAlive() && !isfriend;
		}
		break;

	case Obj::OBJ_TYPE_ROLE_SHADOW:
		{
			RoleShadow *target_role_shadow = (RoleShadow*)obj;
			RoleAIBase *target_shadow_ai = target_role_shadow->GetRoleAI();
			if (NULL != target_shadow_ai && ROLE_SHADOW_TYPE_ELEMENT_FILED == target_shadow_ai->GetAIType())
			{
				RoleAIElementField *filed_ai = (RoleAIElementField*)target_shadow_ai;

				return target_role_shadow->IsAlive() && filed_ai->GetSide() != role->GetRoleActivity()->GetTempZhanchangSide();
			}
		}
		break;

	}

	return false;
}

bool SpecialQunXianLuanDou::SpecialIsFriend(Role *role, Obj *obj, bool check_alive)
{
	switch (obj->GetObjType())
	{
	case Obj::OBJ_TYPE_ROLE:
		{
			Role *friend_role = (Role*)obj;

			bool isfriend = (role->GetRoleActivity()->GetTempZhanchangSide() == friend_role->GetRoleActivity()->GetTempZhanchangSide());

			return (!check_alive || friend_role->IsAlive()) && isfriend;
		}
		break;

	case Obj::OBJ_TYPE_MONSTER:
		{
			Monster *monster = (Monster*)obj;

			bool is_friend = false;
			{
				MonsterAIBase *monsterai_ibase = monster->GetMonsterAI();
				if (NULL != monsterai_ibase && MonsterInitParam::AITYPE_SPECIALAI_BATTLEFIELD == monsterai_ibase->GetAIType())
				{
					MonsterAIBattleField *battlefield_ai = (MonsterAIBattleField*)monsterai_ibase;

					if (role->GetRoleActivity()->GetTempZhanchangSide() == battlefield_ai->GetBattleFieldSide())
					{	
						is_friend = true;
					}
				}
			}

			return (!check_alive || monster->IsAlive()) && is_friend;
		}
		break;

	case Obj::OBJ_TYPE_ROLE_SHADOW:
		{
			RoleShadow *target_role_shadow = (RoleShadow*)obj;
			RoleAIBase *target_shadow_ai = target_role_shadow->GetRoleAI();
			if (NULL != target_shadow_ai && ROLE_SHADOW_TYPE_ELEMENT_FILED == target_shadow_ai->GetAIType())
			{
				RoleAIElementField *filed_ai = (RoleAIElementField*)target_shadow_ai;

				return (!check_alive || target_role_shadow->IsAlive()) && filed_ai->GetSide() == role->GetRoleActivity()->GetTempZhanchangSide();
			}
		}
		break;
	}

	return false;
}

bool SpecialQunXianLuanDou::SpecialCanPerformSkill(Role *role)
{
	return !this->IsRoleHasShenshi(role);
}

int SpecialQunXianLuanDou::GetSpecialParam(Role *role) 
{
	int param_val = role->GetRoleActivity()->GetTempZhanchangSide() * 1000000;
	
	QunXianLuanDouUser *qunxianluandou_user = this->GetQunXianLuanDouUser(role->GetUserId());
	if (NULL != qunxianluandou_user)
	{
		param_val += qunxianluandou_user->lianzhan;
	}

	return param_val; 
}

int SpecialQunXianLuanDou::GetRoleShadowSpecialParam(RoleShadow *roleshadow)
{
	if (NULL == roleshadow) return -1;

	if (ROLE_SHADOW_TYPE_ELEMENT_FILED == roleshadow->GetRoleShadowType())
	{
		
		return roleshadow->GetSpecialParam();
	}

	return -1;
}

int SpecialQunXianLuanDou::GetObjSide(ObjID id)
{
	Role* role = dynamic_cast<Role*>(m_scene->GetObj(id));
	if (!role)
	{
		return -1;
	}
	UserID uid = role->GetUserId();
	QunXianLuanDouUser* qu = GetQunXianLuanDouUser(uid);
	if (!qu)
	{
		return -1;
	}
	
	return 1 << qu->GetSide();
}

void SpecialQunXianLuanDou::OnActivityStatusChange(int status)
{
	switch (status)
	{
	case ACTIVITY_STATUS_OPEN:
		{
			this->OnActivityStart();
		}
		break;

	case ACTIVITY_STATUS_CLOSE:
		{
			for (unsigned int i = 0; i < m_scene->RoleNum(); i ++)
			{
				Role *tmp_role = m_scene->GetRoleByIndex(i);
				if (nullptr != tmp_role && !tmp_role->IsAlive())
				{
					tmp_role->ReAlive(100, 100, tmp_role->GetPos());
				}
			}

			WorldStatus::Instance().OnActivityFinish(ACTIVITY_FIND_TYPE_QUNXIANLUANDOU);
			this->OnActivityClose();
		}
		break;
	}
}

void SpecialQunXianLuanDou::OnPickUpBattleFiledShenShi(Role *role, ObjID shenshi_objid)
{
	if (m_isopen && role->IsAlive())
	{
		Obj *shenshi_obj = m_scene->GetObj(shenshi_objid);
		if (NULL != shenshi_obj)
		{
			if (!(role->GetPos() - shenshi_obj->GetPos()).IsLongerThan(BATTLEFIELD_SHENSHI_MAX_PICK_UP_DISTANCE))
			{
				this->CheckShenShiOwonerStatus(role, true, shenshi_objid);
			}
			else
			{
				role->NoticeNum(errornum::EN_TOO_FAR_FROM_SHENSHI);
			}
		}
	}
}

QunXianLuanDouUser * SpecialQunXianLuanDou::GetQunXianLuanDouUser(const UserID &user_id)
{
	QunXianLuanDouUser &user = m_qunxianluandou_usermap[user_id];
	user.user_id = user_id;
	return &user;
	//QunXianLuanDouUserMapIt it = m_qunxianluandou_usermap.find(user_id);
	//if (it == m_qunxianluandou_usermap.end()) // 不存在 插入新的用户
	//{
	//	QunXianLuanDouUser qunxianluandou_user;

	//	qunxianluandou_user.user_id = user_id;
	//	m_qunxianluandou_usermap[user_id] = qunxianluandou_user;
	//}

	//if (it == m_qunxianluandou_usermap.end()) it = m_qunxianluandou_usermap.find(user_id);

	//if (it != m_qunxianluandou_usermap.end())
	//{
	//	return &it->second;
	//}

	//return NULL;
}

void SpecialQunXianLuanDou::CheckLeaveQunXianLuanDouUer(time_t now_second)
{
	if (now_second >= m_last_check_leaveuser_timestamp + 5)
	{
		m_last_check_leaveuser_timestamp = now_second;

		for (QunXianLuanDouUserMapIt it = m_qunxianluandou_usermap.begin(); it != m_qunxianluandou_usermap.end();)
		{
			if (0 != it->second.last_leavetime && now_second >= it->second.last_leavetime + ZHANCHANG_RESERVE_INFO_TIME_S)
			{
				if (QUNXIANLUANDOU_SIDE_1 <= it->second.side && it->second.side < QUNXIANLUANDOU_SIDE_MAX)
				{
					-- m_sideinfo_list[it->second.side].role_num;
				}

				m_qunxianluandou_usermap.erase(it ++);
			}
			else
			{
				++ it;
			}
		}
	}
}

void SpecialQunXianLuanDou::AddSideScore(int side, int score)
{
	if (QUNXIANLUANDOU_SIDE_1 <= side && side < QUNXIANLUANDOU_SIDE_MAX)
	{
		m_sideinfo_list[side].score += score;
		m_need_notify_sideinfo = true;
	}
}

int SpecialQunXianLuanDou::GetRoleNumLeastSide()
{
	int min_side = QUNXIANLUANDOU_SIDE_1;

	for (int i = QUNXIANLUANDOU_SIDE_1 + 1; i < QUNXIANLUANDOU_SIDE_MAX; i ++)
	{
		if (m_sideinfo_list[i].role_num < m_sideinfo_list[min_side].role_num)
		{
			min_side = i;
		}
	}

	return min_side;
}

bool static SortFunc(const QunXianLuanDouUser &rank_item_1, const QunXianLuanDouUser &rank_item_2)
{
	return rank_item_1.TotalHonor() > rank_item_2.TotalHonor();
}

void SpecialQunXianLuanDou::CheckUserRank()
{
	std::vector<QunXianLuanDouUser> m_honor_rank_vec;
	m_honor_rank_vec.reserve(m_qunxianluandou_usermap.size());

	for (QunXianLuanDouUserMap::iterator it = m_qunxianluandou_usermap.begin(); it != m_qunxianluandou_usermap.end(); ++it)
	{
		m_honor_rank_vec.push_back(it->second);
	}

	std::sort(m_honor_rank_vec.begin(), m_honor_rank_vec.end(), &SortFunc);

	UserID old_first = INVALID_USER_ID;
	if (m_rankinfo_count > 0)
	{
		old_first = m_rankinfo_list[0].user_id;
	}

	m_rankinfo_count = 0;
	
	for (std::vector<QunXianLuanDouUser>::iterator it = m_honor_rank_vec.begin(); it != m_honor_rank_vec.end() && m_rankinfo_count < QUNXIANLUANDOU_RANK_NUM; ++ it)
	{
		m_rankinfo_list[m_rankinfo_count].user_id = it->user_id;
		m_rankinfo_list[m_rankinfo_count].score = it->TotalHonor();
		m_rankinfo_list[m_rankinfo_count].side = it->GetSide();
		F_STRNCPY(m_rankinfo_list[m_rankinfo_count].user_name, it->user_name, sizeof(GameName));

		++ m_rankinfo_count;
	}

	this->SendRankToUser();

	if (m_rankinfo_count > 0)
	{
		if (m_rankinfo_list[0].user_id != old_first && old_first != INVALID_USER_ID)
		{
			ActivityMsgManager::RankTopItem first;
			first.uid = UidToInt(m_rankinfo_list[0].user_id);
			F_STRNCPY(first.user_name, m_rankinfo_list[0].user_name, sizeof(GameName));
			ActivityMsgManager::Instance().RankFirstMsg(m_scene, first);
		}

		if (ActivityMsgManager::Instance().IsNeedRankTopMsg(m_scene))
		{
			ActivityMsgManager::RankTopItem top[ACTIVITY_RANK_TOP_MSG_MAX];
			int i = 0;
			for (; i < ACTIVITY_RANK_TOP_MSG_MAX && i < m_rankinfo_count; ++i)
			{
				top[i].uid = UidToInt(m_rankinfo_list[i].user_id);
				F_STRNCPY(top[i].user_name, m_rankinfo_list[i].user_name, sizeof(GameName));
			}
			ActivityMsgManager::Instance().RankTopMsg(m_scene, top, i);
		}
	}
}

void SpecialQunXianLuanDou::CheckFirstRank(QunXianLuanDouUser *qxlduser)
{
	int side = qxlduser->GetSide();
	if (side < 0 || side >= QUNXIANLUANDOU_SIDE_MAX)
	{
		return;
	}

	if (m_rank_first_one_side_list[side].score < qxlduser->TotalHonor())
	{
		m_rank_first_one_side_list[side].user_id = qxlduser->user_id;
		m_rank_first_one_side_list[side].side = side;
		F_STRNCPY(m_rank_first_one_side_list[side].user_name, qxlduser->user_name, sizeof(m_rank_first_one_side_list[side].user_name));
		m_rank_first_one_side_list[side].score = qxlduser->TotalHonor();
	}
}

bool SpecialQunXianLuanDou::IsRoleHasShenshi(Role *role, ObjID *out_shenshi_objid)
{
	if (NULL == role)
	{
		return false;
	}

	for (std::set<ObjID>::iterator it = m_shenshi_objidlist.begin(), end = m_shenshi_objidlist.end(); it != end; ++ it)
	{
		Obj *shenshi_obj = m_scene->GetObj(*it);
		if (NULL != shenshi_obj && Obj::OBJ_TYPE_BATTLEFIELD_SHENSHI == shenshi_obj->GetObjType())
		{
			BattleFieldShenShi *battlefield_shenshi = (BattleFieldShenShi*)shenshi_obj;
			if (battlefield_shenshi->GetOwnerUserId() == role->GetUserId())
			{
				if (NULL != out_shenshi_objid) *out_shenshi_objid = *it;
				return true;
			}
		}
	}

	return false;
}

bool SpecialQunXianLuanDou::CheckShenShiOwonerStatus(Role *role, bool is_set, ObjID shenshi_objid)
{
	static const UInt16 QUNXIANLUANDOU_BUFF_SKILL_INDEX = 1;
	if (role == nullptr) return false;

	if (is_set)
	{
		Obj *shenshi_obj = m_scene->GetObj(shenshi_objid);
		if (NULL != shenshi_obj && Obj::OBJ_TYPE_BATTLEFIELD_SHENSHI == shenshi_obj->GetObjType())
		{
			BattleFieldShenShi *battlefield_shenshi = (BattleFieldShenShi*)shenshi_obj;

			if (this->IsRoleHasShenshi(role))
			{
				role->NoticeNum(errornum::EN_QUNXIANLUANDOU_CONTROLLING_SHENSHI);
			}
			else if (!battlefield_shenshi->IsAlive() && !battlefield_shenshi->HaveOwner())
			{
				battlefield_shenshi->SetOwner(role->GetId(), role->GetUserId());

				if (role->GetMountManager()->IsOnMount())
				{
					role->GetMountManager()->GoonMount(false);
				}

				{
					UInt16 skill_id = GetSkillIDBySceneTypeAndIndex(m_scene->GetSceneType(), QUNXIANLUANDOU_BUFF_SKILL_INDEX);
					int dec_speed_percent = 50;
					Attribute movespeed_dec_value = role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_MOVE_SPEED) * dec_speed_percent / 100;
					static const int LAST_TIME_MS = 3600 * 1000;

					EffectBuff *effect_buff = new EffectBuff(role->GetId(), skill_id, LAST_TIME_MS, 1, EffectBase::PRODUCT_METHOD_SHENSHI);
					effect_buff->AddMoveSpeedEffect(-movespeed_dec_value, -dec_speed_percent);
					role->AddEffect(effect_buff);
				}

				return true;
			}
		}
	}
	else
	{
		UInt16 skill_id = GetSkillIDBySceneTypeAndIndex(m_scene->GetSceneType(), QUNXIANLUANDOU_BUFF_SKILL_INDEX);
		role->GetEffectList()->FindAndRemoveEffect(EffectBase::EFFECT_TYPE_ATTRBUFF, EBT_INVALID, role->GetId(), EffectBase::PRODUCT_METHOD_SHENSHI, skill_id);

		ObjID owner_shenshi_objid = INVALID_OBJ_ID;
		if (this->IsRoleHasShenshi(role, &owner_shenshi_objid))
		{
			Obj *shenshi_obj = m_scene->GetObj(owner_shenshi_objid);
			if (NULL != shenshi_obj && Obj::OBJ_TYPE_BATTLEFIELD_SHENSHI == shenshi_obj->GetObjType())
			{
				BattleFieldShenShi *battlefield_shenshi = (BattleFieldShenShi*)shenshi_obj;

				if (battlefield_shenshi->GetOwnerUserId() == role->GetUserId())
				{
					battlefield_shenshi->SetOwner(INVALID_OBJ_ID, INVALID_USER_ID);
					return true;
				}
			}
		}
	}

	return false;
}

void SpecialQunXianLuanDou::OnBattleFiledShenShiDead(BattleFieldShenShi *dead_shenshi, ObjID killer_objid)
{
	Role *killer_role = m_scene->GetPrincipal(killer_objid);
	if (NULL != killer_role)
	{
		this->CheckShenShiOwonerStatus(killer_role, true, dead_shenshi->GetId());
	}

	m_shenshi_next_flush_timestamp = EngineAdapter::Instance().Time() + QUNXIANLUANDOU_SHENSHI_REFRESH_TIME_S;

	this->SendSideInfoToUser();
}

void SpecialQunXianLuanDou::OnBattleFiledShenShiDisappear(BattleFieldShenShi *shenshi)
{
	QunXianLuanDouUser *qunxianluandou_owner_user = this->GetQunXianLuanDouUser(shenshi->GetOwnerUserId());	
	if (NULL != qunxianluandou_owner_user)
	{
		qunxianluandou_owner_user->OnRobShenShi(this);

		for (QunXianLuanDouUserMapIt it = m_qunxianluandou_usermap.begin(), end = m_qunxianluandou_usermap.end(); it != end; ++ it)
		{
			if (it->second.user_id != qunxianluandou_owner_user->user_id && it->second.side == qunxianluandou_owner_user->side &&
				!it->second.IsLeaveBattleField())
			{
				it->second.OnSideMemberRobShenShi(this); 
			}
		}

		int lenth = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_qunxian_luandou_shenshi_disappear,
			UidToInt(qunxianluandou_owner_user->user_id), qunxianluandou_owner_user->user_name, (int)qunxianluandou_owner_user->camp);
		if (lenth > 0)
		{
			this->NotifySystemMsg(gamestring::GAMESTRING_BUF, lenth, SYS_MSG_ACTIVE_NOTICE);
		}

	}

	m_shenshi_objidlist.erase(shenshi->GetId());
}

void SpecialQunXianLuanDou::OnRoleDead(Role *dead_role, ObjID killer_objid)
{
	time_t now_second = EngineAdapter::Instance().Time();

	Role *kill_role = m_scene->GetPrincipal(killer_objid);
	if (NULL != kill_role)
	{
		QunXianLuanDouUser *qunxianluandou_deaduser = this->GetQunXianLuanDouUser(dead_role->GetUserId());
		QunXianLuanDouUser *qunxianluandou_killeruser = this->GetQunXianLuanDouUser(kill_role->GetUserId());
		if (NULL != qunxianluandou_deaduser && NULL != qunxianluandou_killeruser)
		{
			bool is_reward = true;
			{
				std::map<UserID, time_t>::iterator it = qunxianluandou_killeruser->kill_user_map.find(dead_role->GetUserId());
				if (it != qunxianluandou_killeruser->kill_user_map.end())
				{
					if (now_second < it->second + QUNXIANLUANDOU_KILL_VALID_TIME_S) is_reward = false;
					if (is_reward) it->second = now_second;
				}
				else
				{
					qunxianluandou_killeruser->kill_user_map[dead_role->GetUserId()] = now_second;
				}

				if (is_reward)
				{
					qunxianluandou_killeruser->OnKill(this, is_reward);
					this->CheckLianZhanNotice(kill_role, dead_role);
				}
				else
				{
					kill_role->NoticeNum(errornum::EN_BATTLE_FIELD_KILL_USER_INVALID);
				}
			}

			if (is_reward)
			{
				for (std::map<UserID, time_t>::iterator it = qunxianluandou_deaduser->hurt_map.begin(), 
					end = qunxianluandou_deaduser->hurt_map.end(); it != end; ++ it)
				{
					if (it->second + QUNXIANLUANDOU_ASSIST_VALID_TIME_S >= now_second && it->first != qunxianluandou_killeruser->user_id)
					{
						QunXianLuanDouUser *qunxianluandou_assistuser = this->GetQunXianLuanDouUser(it->first);
						if (NULL != qunxianluandou_assistuser && qunxianluandou_assistuser->GetSide() == qunxianluandou_killeruser->GetSide())
						{
							qunxianluandou_assistuser->OnAssist(this);
						}
					}
				}
			}
		}
	}

	QunXianLuanDouUser *qunxianluandou_deaduser = this->GetQunXianLuanDouUser(dead_role->GetUserId());
	if (NULL != qunxianluandou_deaduser) qunxianluandou_deaduser->OnDead(this, now_second, NULL != kill_role);

	this->CheckShenShiOwonerStatus(dead_role, false);
}

void SpecialQunXianLuanDou::OnActivityStandy()
{
	this->Clear();
	m_luck.next_lucky_timestamp = 0;
	m_luck.luck_user_count = 0;
	m_worship_start_time = 0;
	m_kick_out_all_user_time = 0;
}

void SpecialQunXianLuanDou::OnActivityStart() 
{
	m_isopen = true;
	m_has_calc_weak_side_protect = false;

	unsigned int now_sec = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	m_shenshi_next_flush_timestamp = 0;
	m_next_free_reward_time = now_sec + LOGIC_CONFIG->GetQunXianLuanDouConfig().GetFreeRewardIntervalS();
	m_luck.next_lucky_timestamp = now_sec + LOGIC_CONFIG->GetQunXianLuanDouConfig().GetLuckInterval();
	m_luck.luck_user_count = 0;
	m_robot_max_count = LOGIC_CONFIG->GetQunXianLuanDouConfig().GetRobotMaxNum();
	m_robot_create_next_interval = now_sec + LOGIC_CONFIG->GetQunXianLuanDouConfig().GetCreateRobotStartInterval();
	m_robot_create_interval = LOGIC_CONFIG->GetQunXianLuanDouConfig().GetCreateRobotInterval();
	m_robot_birth_pos = LOGIC_CONFIG->GetQunXianLuanDouConfig().GetCreateRobotBirthPos();
	m_worship_start_time = 0;
	m_worship_user_map.clear();

	for (int side = QUNXIANLUANDOU_SIDE_1; side < QUNXIANLUANDOU_SIDE_MAX; ++ side)
	{
		Posi defender_pos(0, 0);
		if (LOGIC_CONFIG->GetQunXianLuanDouConfig().GetDefenderPos(side, &defender_pos))
		{
			{
				int monster_id = LOGIC_CONFIG->GetQunXianLuanDouConfig().GetDefenderMonsterId(side);
				Monster *realive_area_guard_monster = MONSTERPOOL->CreateMonster(monster_id, m_scene, defender_pos);
				if (NULL != realive_area_guard_monster)
				{
					MonsterAIBase *monsterai_base = realive_area_guard_monster->GetMonsterAI();
					if (NULL != monsterai_base && MonsterInitParam::AITYPE_SPECIALAI_BATTLEFIELD == monsterai_base->GetAIType())
					{
						MonsterAIBattleField *monsterai_battlefield = (MonsterAIBattleField*)monsterai_base;

						monsterai_battlefield->SetParams(side, 0);
					}
					else
					{
						m_scene->DeleteObj(realive_area_guard_monster->GetId());
					}
				}
			}

			{
				int skill_id = GetSkillIDBySceneTypeAndIndex(m_scene->GetSceneType(), 10);
				SceneEffectSpecialFaZhen *scene_effect = new SceneEffectSpecialFaZhen(m_scene, INVALID_OBJ_ID, skill_id, EffectBase::PRODUCT_METHOD_SCENE_SKILL);
				scene_effect->SetFaZhenInfo(defender_pos, 17, 2000, 900, false, true, 1 << side);

				SkillAttachEffectParam skill_param;
				skill_param.effect_type = SKILL_ATTACH_EFFECT_ADD_HP;
				skill_param.effect_param_list[0] = 10;
				skill_param.effect_param_list[1] = side;
				scene_effect->SetEffectParam(SKILL_ATTACH_EFFECT_TARGET_ROLE_PET, 1, &skill_param);

				this->AddSceneEffect(scene_effect);
			}
		}
	}

	this->SendLuckyInfoToUser();
}

static int ConvertParamInt(short param1, short param2)
{
	union ParamToInt
	{
		struct
		{
			short m_param1;
			short m_param2;
		};

		int val;
	};

	ParamToInt param;

	param.m_param1 = param1;
	param.m_param2 = param2;

	return param.val;
}

void SpecialQunXianLuanDou::OnActivityClose() 
{
	m_isopen = false;
	m_has_calc_weak_side_protect = false;

	m_worship_start_time = EngineAdapter::Instance().Time();

	{
		for (SceneEffectVectorIt it = m_scene_effect_vec.begin(), end = m_scene_effect_vec.end(); it != end; ++ it)
		{
			delete *it;
		}

		m_scene_effect_vec.clear();
	}

// 	{
// 		ActivityQunXianLuanDou *activity = static_cast<ActivityQunXianLuanDou *>(ActivityManager::Instance().ForceGetActivity(ACTIVITY_TYPE_QUNXIANLUANDOU));
// 		if (nullptr != activity)
// 		{
// 			activity->CheckFirstThreeRankUser(m_rank_first_one_side_list, QUNXIANLUANDOU_SIDE_MAX);
// 		}
// 
// 		for (int i = 0; i < QUNXIANLUANDOU_SIDE_MAX; ++i)
// 		{
// 			m_rank_first_one_side_list[i].score = 0;
// 			m_rank_first_one_side_list[i].user_id = UserID();
// 			memset(m_rank_first_one_side_list[i].user_name, 0, sizeof(m_rank_first_one_side_list[i].user_name));
// 			m_rank_first_one_side_list[i].side = 0;
// 		}
// 	}

	{
		int side_max_score = m_sideinfo_list[QUNXIANLUANDOU_SIDE_1].score;
		int win_side = QUNXIANLUANDOU_SIDE_1;

		for (int i = QUNXIANLUANDOU_SIDE_1; i < QUNXIANLUANDOU_SIDE_MAX; ++ i)
		{
			if (m_sideinfo_list[i].score > side_max_score)
			{
				side_max_score = m_sideinfo_list[i].score;
				win_side = i;
			}
		}
		
		for (QunXianLuanDouUserMapIt it = m_qunxianluandou_usermap.begin(), end = m_qunxianluandou_usermap.end(); it != end; ++ it)
		{
			{
				int notify_reason = this->GetResultNotifyReason(it->second.side);
				this->SendQunXianLuanDouUserInfo(&it->second, notify_reason);
			}

			{
				// 积分奖励
				// 策划要求改成积分奖励即发放
// 				const QunXianLuanDouConfig::ScoreRewardItem * reward_cfg = LOGIC_CONFIG->GetQunXianLuanDouConfig().GetScoreRewardHonor(it->second.TotalHonor());
// 				if (NULL != reward_cfg)
// 				{
// 					int reward_honor = reward_cfg->reward_honor;
// 					int reward_factor = 1;
// 
// 					if (ActivityShadow::Instance().IsSpecialActivityOpen(SPECIAL_ACTIVITY_TYPE_CLOSEDBETA)) // 封测期间奖励翻倍
// 					{
// 						reward_factor *= ZHANCHANG_FANGBEI_MAX_REWARD_TIMES;
// 					}
// 
// 					if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_ZHANCHANG_FANBEI))
// 					{
// 						reward_factor *= ZHANCHANG_FANGBEI_MAX_REWARD_TIMES;
// 					}
// 
// 					if (ActivityShadow::Instance().IsSpecialActivityOpen(SPECIAL_ACTIVITY_TYPE_COMBINE_SERVER))
// 					{
// 						if (ActivityShadow::Instance().IsCSASubActivityOpen(CSA_SUB_TYPE_ZHANCHANG_FANBEI))
// 						{
// 							reward_factor *= ZHANCHANG_FANGBEI_MAX_REWARD_TIMES;
// 						}
// 					}
// 
// 					if (reward_factor > ZHANCHANG_FANGBEI_MAX_REWARD_TIMES) 
// 					{
// 						reward_factor = ZHANCHANG_FANGBEI_MAX_REWARD_TIMES;
// 					}
// 
// 					reward_honor *= reward_factor;
// 
// 					static MailContentParam contentparam; contentparam.Reset();
// 					contentparam.virtual_item_list.virtual_item[MAIL_VIRTUAL_ITEM_CONVERTSHOP_SHENGWANG] = reward_honor;
// 
// 					for(int item_count = 0; item_count < reward_cfg->reward_item_count && item_count < MAX_ATTACHMENT_ITEM_NUM; item_count++)
// 					{
// 						contentparam.AddItem(reward_cfg->reward_item[item_count].item_id, reward_cfg->reward_item[item_count].num, reward_cfg->reward_item[item_count].is_bind);
// 					}
// 
// 					if (reward_honor > 0 || reward_cfg->reward_item_count > 0)
// 					{
// 						int length1 = SNPRINTF(contentparam.subject, sizeof(contentparam.subject), gamestring::g_qunxianluandou_score_reward_subject);
// 						int length2 = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_qunxianluandou_score_reward_content, 
// 							it->second.TotalHonor(), reward_honor);
// 
// 						if (length1 > 0 && length2 > 0)
// 						{
// 							MailRoute::MailToUser(it->second.user_id, SYSTEM_MAIL_REASON_INVALID, contentparam);
// 						}
// 					}
// 				}
			}

			{
				// 阵营胜败奖励
				bool is_win_side = it->second.side == win_side;

				const QXLDSideRewardConfig::ConfigItem *reward_cfg = LOGIC_CONFIG->GetQunXianLuanDouConfig().GetSideRewardCfg(it->second.level);
				if (NULL == reward_cfg)
				{
					continue;
				}

				int reward_honor = reward_cfg->lose_honor;
				long long reward_exp = reward_cfg->lose_exp;
				ItemConfigData reward_item = reward_cfg->lose_item;

				if (is_win_side)
				{
					reward_honor = reward_cfg->win_honor;
					reward_exp = reward_cfg->win_exp;
					reward_item = reward_cfg->win_item;
				}

				int reward_factor = 1;

				if (ActivityShadow::Instance().IsSpecialActivityOpen(SPECIAL_ACTIVITY_TYPE_CLOSEDBETA)) // 封测期间奖励翻倍
				{
					reward_factor *= ZHANCHANG_FANGBEI_MAX_REWARD_TIMES;
				}

				if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_ZHANCHANG_FANBEI))
				{
					reward_factor *= ZHANCHANG_FANGBEI_MAX_REWARD_TIMES;
				}

				if (ActivityShadow::Instance().IsSpecialActivityOpen(SPECIAL_ACTIVITY_TYPE_COMBINE_SERVER))
				{
					if (ActivityShadow::Instance().IsCSASubActivityOpen(CSA_SUB_TYPE_ZHANCHANG_FANBEI))
					{
						reward_factor *= ZHANCHANG_FANGBEI_MAX_REWARD_TIMES;
					}
				}

				if (reward_factor > ZHANCHANG_FANGBEI_MAX_REWARD_TIMES) 
				{
					reward_factor = ZHANCHANG_FANGBEI_MAX_REWARD_TIMES;
				}

				reward_honor *= reward_factor;

				static MailContentParam contentparam; contentparam.Reset();
				contentparam.AddVirtualItem(0, MAIL_VIRTUAL_ITEM_CONVERTSHOP_SHENGWANG, reward_honor);

				reward_exp += WorldShadow::Instance().GetWorLeveldExtraExp(it->second.level, reward_exp);

				contentparam.AddVirtualItem(1, MAIL_VIRTUAL_ITEM_EXP, reward_exp);

				contentparam.item_count = 1;
				contentparam.item_list[0].item_id = reward_item.item_id;
				contentparam.item_list[0].num = reward_item.num;
				contentparam.item_list[0].is_bind = reward_item.is_bind;

				int length1 = 0; int length2 = 0;
				if (is_win_side)
				{
					length1 = SNPRINTF(contentparam.subject, sizeof(contentparam.subject), gamestring::g_qunxianluandou_side_win_subject);
					length2 = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_qunxianluandou_side_win_content);
				}
				else
				{
					length1 = SNPRINTF(contentparam.subject, sizeof(contentparam.subject), gamestring::g_qunxianluandou_side_lose_subject);
					length2 = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_qunxianluandou_side_lose_content);
				}

				if (length1 > 0 && length2 > 0)
				{
					MailRoute::MailToUser(it->second.user_id, SYSTEM_MAIL_REASON_INVALID, contentparam);
				}
			}

			{
				// 记录日志
				gamelog::g_log_roleactivity.printf(LL_INFO, "Qunxianluandou::JoinRecord user[%d, %s], level[%d], kill[%d], assist[%d], jifen[%d]",
					UidToInt(it->second.user_id), it->second.user_name, it->second.level, it->second.kills, it->second.assists, it->second.TotalHonor());

				LOG_QUICK8(gamelog::g_log_quick, LOG_TYPE_ACTIVITY_QUNXIANLUANDOU, UidToInt(it->second.user_id), it->second.user_name, NULL, it->second.level, it->second.free_reward_honor, "Close", NULL);
			}
		}
	}

	{
		// 三界战场结束时同步排行榜
		std::vector<QunXianLuanDouUser> temp_vec;
		temp_vec.reserve(m_qunxianluandou_usermap.size());

		for (QunXianLuanDouUserMap::iterator it = m_qunxianluandou_usermap.begin(); it != m_qunxianluandou_usermap.end(); ++ it)
		{
			temp_vec.push_back(it->second);
		}

		std::sort(temp_vec.begin(), temp_vec.end(), &SortFunc);
		{
			gglobalprotocal::SyncLastBattleFieldRankInfo rank_info;
			rank_info.rank_type = PERSON_RANK_TYPE_QUNXIANLUANDOU;
			int count = 0;

			for (int i = 0; i < (int)temp_vec.size() && count < PERSON_RANK_MAX_SIZE; ++ i)
			{
				QunXianLuanDouUser &temp_user = temp_vec[i];

				if (temp_user.TotalHonor() <= 0)
				{
					continue;
				}

				rank_info.rank_list[count].user_id = temp_user.user_id;
				rank_info.rank_list[count].level = temp_user.level;
				rank_info.rank_list[count].prof = temp_user.prof;
				rank_info.rank_list[count].sex = temp_user.sex;
				rank_info.rank_list[count].vip = temp_user.vip;
				rank_info.rank_list[count].camp = temp_user.camp;
				rank_info.rank_list[count].rank_value = temp_user.TotalHonor();
				rank_info.rank_list[count].flexible_int = ConvertParamInt(temp_user.kills, temp_user.assists);
				rank_info.rank_list[count].flexible_ll = temp_user.lianzhan_max;
				F_STRNCPY(rank_info.rank_list[count].user_name, temp_user.user_name, sizeof(rank_info.rank_list[count].user_name));

				++ count;
			}

			rank_info.rank_count = count;
			RankManager::Instance().SyncPersonRankBattleFieldRankInfo(&rank_info);
		}
	}

// 	{
// 		ActivityQunXianLuanDou *activity = static_cast<ActivityQunXianLuanDou *>(ActivityManager::Instance().ForceGetActivity(ACTIVITY_TYPE_QUNXIANLUANDOU));
// 		if (nullptr != activity)
// 		{
// 			activity->CheckFirstThreeRankUser(m_rank_first_one_side_list, QUNXIANLUANDOU_SIDE_MAX);
// 		}
// 
// 		for (int i = 0; i < QUNXIANLUANDOU_SIDE_MAX; ++i)
// 		{
// 			m_rank_first_one_side_list[i].score = 0;
// 			m_rank_first_one_side_list[i].user_id = UserID();
// 			memset(m_rank_first_one_side_list[i].user_name, 0, sizeof(m_rank_first_one_side_list[i].user_name));
// 			m_rank_first_one_side_list[i].side = 0;
// 		}
// 	}

	{
		//this->DelayKickOutAllRole();
		int kick_out_time = 120; // 服务端多延迟两分钟
		const WorshipOtherConfig *other_cfg = LOGIC_CONFIG->GetWorshipConfig().GetOtherConfig(ACTIVITY_TYPE_QUNXIANLUANDOU);
		if (other_cfg)
		{
			kick_out_time += other_cfg->worship_time;
		}
		m_kick_out_all_user_time = EngineAdapter::Instance().Time() + kick_out_time;

		for (std::set<ObjID>::iterator it = m_shenshi_objidlist.begin(), end = m_shenshi_objidlist.end(); it != end; ++ it)
		{
			Obj *shenshi_obj = m_scene->GetObj(*it);
			if (NULL != shenshi_obj)
			{
				m_scene->DeleteObj(shenshi_obj->GetId());
			}
		}
		UInt32 monster_num = m_scene->MonsterNum();
		for (int i = 0; i < static_cast<int>(monster_num); i++)
		{
			Monster *monster = m_scene->GetMonsterByIndex(i);
			if (NULL != monster)
			{
				monster->ForceSetDead();
			}
		}

		this->Clear();
	}
}

void SpecialQunXianLuanDou::CheckSendFirstRank()
{
	ActivityQunXianLuanDou *activity = static_cast<ActivityQunXianLuanDou *>(ActivityManager::Instance().ForceGetActivity(ACTIVITY_TYPE_QUNXIANLUANDOU));
	if (nullptr != activity)
	{
		activity->CheckFirstThreeRankUser(m_rank_first_one_side_list, QUNXIANLUANDOU_SIDE_MAX);
	}

	for (int i = 0; i < QUNXIANLUANDOU_SIDE_MAX; ++i)
	{
		m_rank_first_one_side_list[i].score = 0;
		m_rank_first_one_side_list[i].user_id = UserID();
		memset(m_rank_first_one_side_list[i].user_name, 0, sizeof(m_rank_first_one_side_list[i].user_name));
		m_rank_first_one_side_list[i].side = 0;
	}
}

void SpecialQunXianLuanDou::SendRankToUser(Role *role)
{
	if (NULL == role)
	{
		m_qxldri.count = 0;

		for (int i = 0; i < m_rankinfo_count && i < QUNXIANLUANDOU_RANK_NUM; i ++)
		{
			m_qxldri.rank_list[i].uid = UidToInt(m_rankinfo_list[i].user_id);
			m_qxldri.rank_list[i].score = m_rankinfo_list[i].score;
			m_qxldri.rank_list[i].side = m_rankinfo_list[i].side;
			F_STRNCPY(m_qxldri.rank_list[i].name, m_rankinfo_list[i].user_name, sizeof(GameName));

			++ m_qxldri.count;
		}

		int sendlen = sizeof(m_qxldri) - (QUNXIANLUANDOU_RANK_NUM - m_qxldri.count) * sizeof(m_qxldri.rank_list[0]);
		for (int i = 0; i < (int)m_scene->RoleNum(); ++i)
		{
			m_qxldri.self_score = 0; m_qxldri.self_rank_pos = 0;
			Role *t_role = m_scene->GetRoleByIndex(i);
			if (t_role != nullptr)
			{
				this->GetSelfRankInfo(t_role, m_qxldri.self_score, m_qxldri.self_rank_pos);
				EngineAdapter::Instance().NetSend(t_role->GetNetId(), (const char*)&m_qxldri, sendlen);
			}
		}
		//m_scene->SendToRole((const char*)&m_qxldri, sendlen);
	}
	else
	{
		int sendlen = sizeof(m_qxldri) - (QUNXIANLUANDOU_RANK_NUM - m_qxldri.count) * sizeof(m_qxldri.rank_list[0]);
		if (sendlen > 0 && sendlen <= static_cast<int>(sizeof(m_qxldri)))
		{
			m_qxldri.self_score = 0; m_qxldri.self_rank_pos = 0;
			this->GetSelfRankInfo(role, m_qxldri.self_score, m_qxldri.self_rank_pos);
			EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&m_qxldri, sendlen);
		}
	}
}

void SpecialQunXianLuanDou::GetSelfRankInfo(Role *role, int &self_score, int &self_rank_pos)
{
	if (role == nullptr) return;

	for (int i = 0; i < m_rankinfo_count && i < QUNXIANLUANDOU_RANK_NUM; i++)
	{
		if (m_rankinfo_list[i].user_id == role->GetUserId())
		{
			self_score = m_rankinfo_list[i].score;
			self_rank_pos = i + 1;
			return;
		}
	}
}

void SpecialQunXianLuanDou::SendSideInfoToUser(Role *role)
{
	Protocol::SCQunXianLuanDouSideInfo qxldci;

	for (int i = QUNXIANLUANDOU_SIDE_1; i < QUNXIANLUANDOU_SIDE_MAX; i ++)
	{
		qxldci.score[i] = m_sideinfo_list[i].score;
	}

	qxldci.shenshi_next_refresh_time = static_cast<unsigned int>(m_shenshi_next_flush_timestamp);

	if (NULL == role)
	{
		m_need_notify_sideinfo = false;
		m_scene->SendToRole((const char*)&qxldci, sizeof(qxldci));
	}
	else
	{
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&qxldci, sizeof(qxldci));
	}
}

void SpecialQunXianLuanDou::SendQunXianLuanDouUserInfo(QunXianLuanDouUser *qunxianluandou_user, int notify_reason)
{
	if (NULL != qunxianluandou_user && qunxianluandou_user->GSNetIDValid())
	{
		Protocol::SCQunXianLuanDouUserInfo qxldui;
		qxldui.notify_reason = notify_reason;
		qxldui.side = qunxianluandou_user->side;
		qxldui.kills = qunxianluandou_user->kills;
		qxldui.lianzhan = qunxianluandou_user->lianzhan;
		qxldui.assists = qunxianluandou_user->assists;
		qxldui.rob_shenshi = qunxianluandou_user->rob_shenshi;
		qxldui.realive_guard_monsterid = LOGIC_CONFIG->GetQunXianLuanDouConfig().GetDefenderMonsterId(qunxianluandou_user->side);
		qxldui.kill_honor = qunxianluandou_user->kill_honor;
		qxldui.assist_honor = qunxianluandou_user->assist_honor;
		qxldui.rob_shenshi_honor = qunxianluandou_user->rob_shenshi_honor;
		qxldui.free_reward_honor = qunxianluandou_user->free_reward_honor;
		qxldui.last_realive_here_timestamp = qunxianluandou_user->last_realive_here_timestamp;
		qxldui.extra_honor = qunxianluandou_user->extra_honor;

		memset(qxldui.first_rank_num, 0, sizeof(qxldui.first_rank_num));
		memset(qxldui.first_rank_uid, 0, sizeof(qxldui.first_rank_uid));

		for (int i = 0; i < QUNXIANLUANDOU_SIDE_MAX; ++i)
		{
			int user_id = WorldStatus::Instance().GetCommonData().qunxian_luandou_user_id_list[i];
			UserCacheNode *user = UserCacheManager::Instance().GetUserNode(user_id);
			if (nullptr != user)
			{
				user->GetName(qxldui.first_rank_num[i]);
				qxldui.first_rank_uid[i] = user_id;
			}
		}

		EngineAdapter::Instance().NetSend(qunxianluandou_user->gsnetid, (const char*)&qxldui, sizeof(qxldui));
	}
}

void SpecialQunXianLuanDou::BroadcastLianzhanInfo(QunXianLuanDouUser *qunxianluandou_user)
{
	if (NULL != qunxianluandou_user && NULL != m_scene)
	{
		Role *role = qunxianluandou_user->GetRole(m_scene);
		if (NULL != role)
		{
			Protocol::SCQunxianluandouLianzhanChange qxldlzc;
			qxldlzc.obj_id = role->GetId();
			qxldlzc.lianzhan = qunxianluandou_user->lianzhan;
			m_scene->GetZoneMatrix()->NotifyAreaMsg(role->GetObserHandle(),(void*)&qxldlzc, sizeof(qxldlzc));
		}
	}
}

void SpecialQunXianLuanDou::CheckLianZhanNotice(Role *kill_role, Role *dead_role)
{
	return;
	if (NULL != kill_role && NULL != dead_role)
	{
		QunXianLuanDouUser *qunxianluandou_killuser = this->GetQunXianLuanDouUser(kill_role->GetUserId());
		QunXianLuanDouUser *qunxianluandou_deaduser = this->GetQunXianLuanDouUser(dead_role->GetUserId());
		if (NULL != qunxianluandou_killuser && NULL != qunxianluandou_deaduser)
		{
			{
				// 击杀额外奖励
				int extra_reward = qunxianluandou_killuser->lianzhan + qunxianluandou_deaduser->lianzhan;
				qunxianluandou_killuser->OnExtraHonor(this, extra_reward);
			}

			if (qunxianluandou_deaduser->lianzhan >= 3)
			{
				int str_len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_pvpzhanchang_stoplianzhan_notice,
					UidToInt(kill_role->GetUserId()), kill_role->GetName(), (int)kill_role->GetCamp(), 
					UidToInt(dead_role->GetUserId()), dead_role->GetName(), (int)dead_role->GetCamp(), qunxianluandou_deaduser->lianzhan);
				if (str_len > 0) 
				{
					this->NotifySystemMsg(gamestring::GAMESTRING_BUF, str_len, SYS_MSG_ACTIVE_NOTICE);
				}
			}

			if (qunxianluandou_killuser->lianzhan > 0)
			{
				int str_len = 0;
				int notice_type = SYS_MSG_ACTIVE_NOTICE;

				if (0 == qunxianluandou_killuser->lianzhan % 3)
				{
					str_len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_pvpzhanchang_lianzhan_notice,
						UidToInt(kill_role->GetUserId()), kill_role->GetName(), (int)kill_role->GetCamp(), 
						qunxianluandou_killuser->lianzhan, qunxianluandou_killuser->lianzhan);
				}

				//if (5 == qunxianluandou_killuser->lianzhan)
				//{
				//	notice_type = SYS_MSG_ACTIVE_NOTICE;
				//	str_len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_pvpzhanchang_lianzhan_notice1,
				//		UidToInt(kill_role->GetUserId()), kill_role->GetName(), (int)kill_role->GetCamp(), qunxianluandou_killuser->lianzhan);
				//}
				//else if (7 == qunxianluandou_killuser->lianzhan)
				//{
				//	notice_type = SYS_MSG_ACTIVE_NOTICE;
				//	str_len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_pvpzhanchang_lianzhan_notice2,
				//		UidToInt(kill_role->GetUserId()), kill_role->GetName(), (int)kill_role->GetCamp(), qunxianluandou_killuser->lianzhan);
				//}
				//else if (9 == qunxianluandou_killuser->lianzhan)
				//{
				//	notice_type = SYS_MSG_ACTIVE_NOTICE;
				//	str_len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_pvpzhanchang_lianzhan_notice3,
				//		UidToInt(kill_role->GetUserId()), kill_role->GetName(), (int)kill_role->GetCamp(), qunxianluandou_killuser->lianzhan);
				//}
				//else if (11 == qunxianluandou_killuser->lianzhan)
				//{
				//	notice_type = SYS_MSG_ACTIVE_NOTICE;
				//	str_len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_pvpzhanchang_lianzhan_notice4,
				//		UidToInt(kill_role->GetUserId()), kill_role->GetName(), (int)kill_role->GetCamp(), qunxianluandou_killuser->lianzhan);
				//}
				//else if (13 == qunxianluandou_killuser->lianzhan)
				//{
				//	notice_type = SYS_MSG_ACTIVE_NOTICE;
				//	str_len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_pvpzhanchang_lianzhan_notice5,
				//		UidToInt(kill_role->GetUserId()), kill_role->GetName(), (int)kill_role->GetCamp(), qunxianluandou_killuser->lianzhan);
				//}
				//else if (15 == qunxianluandou_killuser->lianzhan)
				//{
				//	notice_type = SYS_MSG_ACTIVE_NOTICE;
				//	str_len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_pvpzhanchang_lianzhan_notice6,
				//		UidToInt(kill_role->GetUserId()), kill_role->GetName(), (int)kill_role->GetCamp(), qunxianluandou_killuser->lianzhan);
				//}
				//else if (17 == qunxianluandou_killuser->lianzhan)
				//{
				//	notice_type = SYS_MSG_ACTIVE_NOTICE;
				//	str_len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_pvpzhanchang_lianzhan_notice7,
				//		UidToInt(kill_role->GetUserId()), kill_role->GetName(), (int)kill_role->GetCamp(), qunxianluandou_killuser->lianzhan);
				//}
				//else if (qunxianluandou_killuser->lianzhan >= 18)
				//{
				//	notice_type = SYS_MSG_ACTIVE_NOTICE;
				//	str_len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_pvpzhanchang_lianzhan_notice8,
				//		UidToInt(kill_role->GetUserId()), kill_role->GetName(), (int)kill_role->GetCamp(), qunxianluandou_killuser->lianzhan);
				//}
				
				if (str_len > 0) this->NotifySystemMsg(gamestring::GAMESTRING_BUF, str_len, notice_type);
			}
		}
	}
}

void SpecialQunXianLuanDou::NotifySystemMsg(char *str_buff, int str_len, int notice_type)
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

int SpecialQunXianLuanDou::GetResultNotifyReason(int side)
{
	if (side < QUNXIANLUANDOU_SIDE_1 || side >= QUNXIANLUANDOU_SIDE_MAX) return Protocol::QUNXIANLUANDOU_NOTIFY_REASON_LOSE;

	bool is_lowest = true, is_highest = true;
	for (int i = QUNXIANLUANDOU_SIDE_1; i < QUNXIANLUANDOU_SIDE_MAX; i++)
	{
		if (i != side)
		{
			if (m_sideinfo_list[i].score < m_sideinfo_list[side].score) is_lowest = false;
			else if (m_sideinfo_list[i].score > m_sideinfo_list[side].score) is_highest = false;
		}
	}

	if (is_highest && is_lowest) return Protocol::QUNXIANLUANDOU_NOTIFY_REASON_DRAW;
	else if (is_highest) return Protocol::QUNXIANLUANDOU_NOTIFY_REASON_WIN;

	return Protocol::QUNXIANLUANDOU_NOTIFY_REASON_LOSE;
}

void SpecialQunXianLuanDou::Clear()
{
	m_qunxianluandou_usermap.clear();

	memset(m_sideinfo_list, 0, sizeof(m_sideinfo_list));

	m_rankinfo_count = 0;
	memset(m_rankinfo_list, 0, sizeof(m_rankinfo_list));

	m_shenshi_next_flush_timestamp = 0;
	m_last_check_leaveuser_timestamp = 0;
	m_next_free_reward_time = 0;

	m_qxldri.count = 0;

	m_shenshi_objidlist.clear();
	m_worship_user_map.clear();
}

bool SpecialQunXianLuanDou::IsRewardDouble(int side)
{
	if (!ActivityShadow::Instance().IsActivtyOpen(ACTIVITY_TYPE_QUNXIANLUANDOU))
	{
		return false;
	}

	return m_sideinfo_list[side].is_weak_side_protect;
}

void SpecialQunXianLuanDou::RealiveHereHelper(Role *role, int realive_type)
{
	if (NULL == role)
	{
		return;
	}

// 	if (Protocol::CSRoleReAliveReq::REALIVE_TYPE_HERE_GOLD != realive_type && Protocol::CSRoleReAliveReq::REALIVE_TYPE_HERE_ICON != realive_type)
// 	{
// 		return;
// 	}

	QunXianLuanDouUser *qunxianluandou_user = this->GetQunXianLuanDouUser(role->GetUserId());
	if (NULL != qunxianluandou_user)
	{
		// 计算消耗
		static ItemExtern::ItemConsumeList consume_list; consume_list.Reset();
		{
			const ItemID REALIVE_ITEM_ID = GLOBALCONFIG->GetRealiveItemId();

			ItemExtern::ItemConsumeConfig stuff;
			stuff.item_id = REALIVE_ITEM_ID;
			stuff.num = 1;
			stuff.buyable = false;

			if (!ItemExtern::GetConsumeListByOrder(role, false, 1, &stuff, &consume_list, true))
			{
				return;
			}

			// 消耗
			if (!ItemExtern::ConsumeItem(role, consume_list, "SpecialQunXianLuanDou::RealiveHereHelper"))
			{
				return;
			}
		}

		role->ReAlive(100, 100, role->GetPos(), true);
		qunxianluandou_user->last_realive_here_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
		this->SendQunXianLuanDouUserInfo(qunxianluandou_user);
	}
}

void SpecialQunXianLuanDou::CheckPersonalScoreReward(Role *role)
{
	if (NULL == role) return;

	QunXianLuanDouUser * qxlduser = this->GetQunXianLuanDouUser(role->GetUserId());
	if (NULL == qxlduser) return;

	int total_honor = qxlduser->TotalHonor();
	this->CheckFirstRank(qxlduser);

	role->GetRoleGoal()->CheckBattleFieldCond(ACTIVITY_TYPE_QUNXIANLUANDOU, total_honor);

	const QunXianLuanDouConfig::ScoreRewardItem * reward_cfg = LOGIC_CONFIG->GetQunXianLuanDouConfig().GetScoreRewardHonor(total_honor);
	if (NULL != reward_cfg)
	{
		if (qxlduser->IsFetchScoreRewardFlag(reward_cfg->reward_index))
		{
			return;
		}

		int reward_honor = reward_cfg->reward_honor;
		int reward_factor = 1;

		if (ActivityShadow::Instance().IsSpecialActivityOpen(SPECIAL_ACTIVITY_TYPE_CLOSEDBETA)) // 封测期间奖励翻倍
		{
			reward_factor *= ZHANCHANG_FANGBEI_MAX_REWARD_TIMES;
		}

		if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_ZHANCHANG_FANBEI))
		{
			reward_factor *= ZHANCHANG_FANGBEI_MAX_REWARD_TIMES;
		}

		if (ActivityShadow::Instance().IsSpecialActivityOpen(SPECIAL_ACTIVITY_TYPE_COMBINE_SERVER))
		{
			if (ActivityShadow::Instance().IsCSASubActivityOpen(CSA_SUB_TYPE_ZHANCHANG_FANBEI))
			{
				reward_factor *= ZHANCHANG_FANGBEI_MAX_REWARD_TIMES;
			}
		}

		if (reward_factor > ZHANCHANG_FANGBEI_MAX_REWARD_TIMES) 
		{
			reward_factor = ZHANCHANG_FANGBEI_MAX_REWARD_TIMES;
		}

		reward_honor *= reward_factor;

		if (reward_honor > 0 || reward_cfg->reward_item_count > 0)
		{
			role->GetKnapsack()->PutListOrMail(reward_cfg->reward_item, PUT_REASON_QUNXIANLUANDOU);
			role->GetRoleShop()->AddChestShopShengwang(reward_honor, "Qunxianluandou");
			qxlduser->SetFetchScoreRewardFlag(reward_cfg->reward_index);

			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_guildbattle_fetch_target_reward,
				role->GetUID(), role->GetName(), (int)role->GetCamp(), reward_cfg->need_score);
			if (length > 0) 
			{
				this->NotifySystemMsg(gamestring::GAMESTRING_BUF, length, SYS_MSG_ACTIVE_NOTICE);
			}
		}
	}
}

void SpecialQunXianLuanDou::SendLuckyInfoToUser(Role *role)
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

void SpecialQunXianLuanDou::GaveLuckyItemToUser()
{
	int luck_people_num = LOGIC_CONFIG->GetQunXianLuanDouConfig().GetLuckPeopleNum();

	int lucky_role_count = 0;
	int lucky_role_idx_list[ZHANCHANG_LUACY_ROLE_MAX] = {0};

	{
		std::map<int, int> idx_map;

		int max_rolenum = m_scene->RoleNum();
		for (int i = 0; i < max_rolenum; i++)  // 通过map来打乱idx，用于随机
		{
			idx_map.insert(std::make_pair(RandomNum(1024), i));
		}

		int loop_count = 0;
		while (!idx_map.empty() && lucky_role_count < luck_people_num && ++ loop_count < 128)
		{
			int rand_index = RandomNum(static_cast<int>(idx_map.size()));
			std::map<int, int>::iterator it = idx_map.begin();
			std::advance(it, rand_index);

			lucky_role_idx_list[lucky_role_count ++] = it->second;
			idx_map.erase(it);
		}
	}

	m_luck.luck_user_count = 0;

	char str_format[256] = {0};
	int max_length = sizeof(str_format) / sizeof(str_format[0]);
	int str_length = 0;

	const ItemConfigData &lucky_item = LOGIC_CONFIG->GetQunXianLuanDouConfig().GetLuckItem();

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
					lucky_role->GetKnapsack()->PutOrMail(lucky_item, PUT_REASON_YSZC_LUCK_REWARD);
				}

				if (m_luck.luck_user_count < ZHANCHANG_LUACY_ROLE_MAX)
				{
					lucky_role->GetName(m_luck.luck_user_namelist[m_luck.luck_user_count]);
					++ m_luck.luck_user_count;
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

void SpecialQunXianLuanDou::CreateRobot()
{
	if (m_robot_max_count <= 0) return;

	for (int i = 0; i < QUNXIANLUANDOU_SIDE_MAX; i++)
	{
		QunXianLuanDouSideInfo &side_info = m_sideinfo_list[i];

		if (side_info.robot_num + side_info.role_num >= m_robot_max_count) continue;

		int rand_challenge_pos = RandomNum(100);
		ChallengeUser *rand_challenge_user = ChallengeField::Instance().GetChallengeUserByPos(rand_challenge_pos);
		if (NULL != rand_challenge_user)
		{
			static Posi enter_posi = Posi(0, 0);
			if (LOGIC_CONFIG->GetQunXianLuanDouConfig().GetRealivePos(i, &enter_posi))
			{
				RoleShadow *role_shadow = new RoleShadow(ROLE_SHADOW_TYPE_ELEMENT_FILED);
				ChallengeParam param = rand_challenge_user->GetChallengeParam();
				RoleAppearance appearance = rand_challenge_user->GetChallengeAppearance();
				role_shadow->Init(param, appearance, 0, 0);
				role_shadow->SetPos(enter_posi);
				if (NULL != role_shadow->GetRoleAI())
				{
					RoleAIElementField *ai = (RoleAIElementField*)role_shadow->GetRoleAI();
					ai->SetBirthPos(m_robot_birth_pos);
					ai->SetSide(i);
					role_shadow->SetSpecialParam(i);
				}

				m_scene->AddObj(role_shadow);
				++side_info.robot_num;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
bool SpecialQunXianLuanDou::IsWorshipDuring()
{
	const WorshipOtherConfig *other_cfg = LOGIC_CONFIG->GetWorshipConfig().GetOtherConfig(ACTIVITY_TYPE_QUNXIANLUANDOU);
	if (nullptr == other_cfg)
	{
		return false;
	}

	time_t now = EngineAdapter::Instance().Time();

	return m_worship_start_time > 0 && now > m_worship_start_time && now < m_worship_start_time + other_cfg->worship_time;
}

void SpecialQunXianLuanDou::OnWorship(Role* role)
{
	if (nullptr == role)
	{
		return;
	}

	if (!this->IsWorshipDuring())
	{
		return;
	}

	const WorshipOtherConfig *other_cfg = LOGIC_CONFIG->GetWorshipConfig().GetOtherConfig(ACTIVITY_TYPE_QUNXIANLUANDOU);
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

void SpecialQunXianLuanDou::SendWorshipInfo(Role *role)
{
	if (nullptr == role) return;

	Protocol::SCRoleWorshipInfo rwi;

	WorshipItem &item = m_worship_user_map[role->GetUniqueUserID()];

	rwi.activity_type = ACTIVITY_TYPE_QUNXIANLUANDOU;
	rwi.worship_times = item.manual_worship_times;
	rwi.next_worship_timestamp = item.next_can_manual_worship_timestamp;

	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&rwi, sizeof(rwi));
}

void SpecialQunXianLuanDou::SyncWorshipStart(Role *role)
{
	if (nullptr == role) return;

	Protocol::SCRoleWorshipStart rws;
	rws.role_obj = role->GetId();
	rws.resever_sh = 0;

	m_scene->SendToRole((const char*)&rws, sizeof(rws));
}

void SpecialQunXianLuanDou::CheckWorshipReward(time_t now_second)
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
		temp_role->GetKnapsack()->GetMoney()->AddGoldBind(reward_gold_bind, "SpecialQunXianLuanDou::CheckWorshipReward");

	}
}
