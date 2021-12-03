#include "specialxianmengzhan.hpp"

#include "scene/worldshadow/worldshadow.hpp"
#include "scene/guildshadow/guildshadow.hpp"
#include "scene/activityshadow/activityshadow.hpp"
#include "scene/scenemanager.h"

#include "obj/character/role.h"
#include "obj/character/monster.h"
#include "obj/otherobj/gatherobj.h"
#include "monster/monsterpool.h"
#include "ai/monsterai/specialai/monsteraixianmengzhanareaboss.hpp"
#include "ai/monsterai/specialai/monsteraixianmengzhanguardboss.hpp"
#include "ai/monsterai/specialai/monsteraixianmengzhandanyaoboss.hpp"

#include "effect/skilleffect/effectonce.hpp"
#include "effect/skilleffect/effectbuff.hpp"

#include "config/logicconfigmanager.hpp"
#include "config/activityconfig/xianmengzhanconfig.hpp"
#include "other/event/eventhandler.hpp"
#include "other/route/mailroute.hpp"

#include "globalconfig/globalconfig.h"
#include "global/rank/rankmanager.hpp"
#include "servercommon/string/gameworldstr.h"
#include "servercommon/userprotocal/msgsystem.h"
#include "servercommon/errornum.h"
#include "servercommon/noticenum.h"

#include "protocal/msgfight.h"
#include "world.h"
#include "gamelog.h"
#include "gamecommon.h"
#include "item/itempool.h"
#include "item/itembase.h"
#include "internalcomm.h"
#include "global/activity/activitymanager.hpp"
#include <algorithm>

#include "global/guild/guild.hpp"
#include "global/guild/guildmanager.hpp"
#include "global/worldstatus/worldstatus.hpp"

#include "effect/sceneeffect/sceneeffect.hpp"
#include "item/knapsack.h"
#include "other/mount/mountmanager.hpp"
#include "other/roleactivity/roleactivity.hpp"

SpecialXianMengZhan::SpecialXianMengZhan(Scene *scene) 
	: SpecialLogic(scene), m_isopen(false), m_center_guild(INVALID_GUILD_ID), m_hold_center_timestamp(0), m_center_gather_obj_id(INVALID_OBJ_ID),
	m_last_center_area_check_time(0), m_last_center_area_reward_time(0), m_last_defend_area_reward_time(0), m_last_update_time(0), 
	m_last_sync_user_info_time(0), m_last_sync_guild_info_time(0), m_next_sync_defend_area_list_time(0), m_next_sync_guild_rank_time(0), m_next_free_reward_time(0)
{
}

SpecialXianMengZhan::~SpecialXianMengZhan()
{
}

void SpecialXianMengZhan::Update(unsigned long interval, time_t now_second)
{
	SpecialLogic::Update(interval, now_second);

	if (!m_isopen) return;

	if (now_second >= m_last_update_time + 1)
	{
		this->CheckGatherPoint(now_second);
		this->CheckCenterArea(now_second);
		this->CheckDefendAreaReward(now_second);

		m_last_update_time = now_second;
	}

	// 定时无条件获得积分
	if (m_next_free_reward_time > 0 && now_second >= m_next_free_reward_time)
	{
		m_next_free_reward_time = now_second + LOGIC_CONFIG->GetXianMengZhanConfig().GetFreeRewardIntervalS();

		int free_reward = LOGIC_CONFIG->GetXianMengZhanConfig().GetFreeReward();

		for (XianMengZhanUserMapIt it = m_user_map.begin(), end = m_user_map.end(); end != it; ++it)
		{
			if (INVALID_OBJ_ID != it->second.obj_id)
			{
				this->UserAddScore(&(it->second), free_reward);
				it->second.free_score += free_reward;
			}
		}
	}

	this->SyncUserInfo(now_second);
	this->SyncGuildInfo(now_second);
	this->SyncDefendAreaList(now_second);
	this->SynGuildRankInfo(now_second);
}

bool SpecialXianMengZhan::SpecialCanGather(Role *role, GatherObj *gather)
{
	if (NULL ==  role || NULL == gather)
	{
		return false;
	}

	int gather_belong_guild_id = INVALID_GUILD_ID;
	{
		if (m_center_gather_obj_id == gather->GetId())
		{
			gather_belong_guild_id = m_center_guild;
		}
		else
		{
			for (int i = 0; i < XIANMENGZHAN_DEFEND_AREA_NUM; ++ i)
			{
				if (gather->GetId() == m_defend_area_list[i].gather_obj_id)
				{
					gather_belong_guild_id =  m_defend_area_list[i].data.guild_id;
					break;
				}
			}
		}
	}

	if (INVALID_GUILD_ID != gather_belong_guild_id && role->GetGuildID() == gather_belong_guild_id)
	{
		role->NoticeNum(errornum::EN_XIANMENGZHAN_GATHER_LIMIT);
		return false;
	}

	return true;
}

void SpecialXianMengZhan::OnGather(Role *role, int gather_id, ObjID obj_id, const Posi &pos)
{
	if (!m_isopen || NULL == role) return;

	Obj *obj = m_scene->GetObj(obj_id);
	if (NULL != obj && Obj::OBJ_TYPE_GATHER == obj->GetObjType()) 
	{
		int belong_defend_area = -1;
		{
			for (int i = 0; i < XIANMENGZHAN_DEFEND_AREA_NUM; ++ i)
			{
				if (obj->GetId() == m_defend_area_list[i].gather_obj_id)
				{
					belong_defend_area = i;
					break;
				}
			}
		}

		if (belong_defend_area >= 0 && belong_defend_area < XIANMENGZHAN_DEFEND_AREA_NUM)
		{
			// 占领了周边据点
			this->OnUserGetDefendArea(role, belong_defend_area);
		}
		else if (m_center_gather_obj_id == obj->GetId())
		{
			// 占领了中心据点
			this->OnGuildGetCenterArea(role->GetGuildID());
		}
	}
}
void SpecialXianMengZhan::OnAttack(Character *cha, ObjID attacker_id, Attribute delta_hp)
{
	if (!m_isopen) return;

	if (Obj::OBJ_TYPE_ROLE == cha->GetObjType())
	{
		Role *attacker_role = m_scene->GetPrincipal(attacker_id);
		if (NULL != attacker_role)
		{
			Role *atk_role = (Role *)cha;
			XianMengZhanUser *user = this->GetXianMengZhanUser(atk_role->GetUserId());
			if (NULL != user)
			{
				user->hurt_map[attacker_role->GetUserId()] = EngineAdapter::Instance().Time();
			}
		}
	}
}
void SpecialXianMengZhan::OnCharacterDie(Character *dead, ObjID killer_id)
{
	if (!m_isopen) return;

	if (Obj::OBJ_TYPE_ROLE == dead->GetObjType())
	{
		Role *killer_role = m_scene->GetPrincipal(killer_id);
		if (NULL != killer_role)
		{
			Role *dead_role = (Role *)dead;
			XianMengZhanUser *dead_user = this->GetXianMengZhanUser(dead_role->GetUserId());
			XianMengZhanUser *killer_user = this->GetXianMengZhanUser(killer_role->GetUserId());
			if (NULL != dead_user && NULL != killer_user)
			{
				int old_lianzhan = killer_user->lianzhan;

				this->OnUserDead(dead_user, killer_user, killer_role, dead->GetPos());
				
				if (old_lianzhan != killer_user->lianzhan)
				{
					this->CheckLianZhanNotice(killer_role, (Role*)dead);
				}

				this->OnResetLianzhan(dead_user);
			}
		}	
	}
}

bool SpecialXianMengZhan::SpecialRelive(Role *role, int realive_type)
{
	if (Protocol::CSRoleReAliveReq::REALIVE_TYPE_BACK_HOME != realive_type)
	{
		this->RealiveHereHelper(role, realive_type);
	}
	else
	{
		XianMengZhanGuild *guild = this->GetXianMengZhanGuild(role->GetGuildID());
		int enter_area = -1;

		if (NULL != guild)
		{
			enter_area = guild->enter_area;
		}

		Posi realive_pos(0, 0);
		if (LOGIC_CONFIG->GetXianMengZhanConfig().GetRealivePos(role->GetRoleActivity()->GetXianMengZhanDefendArea(), enter_area, &realive_pos))
		{
			role->ReAlive(10, 10, realive_pos);
		}
	}

	return true;
}

int SpecialXianMengZhan::GetSpecialParam(Role *role)
{
	if (NULL != role)
	{
		XianMengZhanUser *user = this->GetXianMengZhanUser(role->GetUserId());
		if (NULL != user)
		{
			return user->lianzhan;
		}
	}

	return 0;
}

bool SpecialXianMengZhan::SpecialIsEnemy(Role *role, Obj *obj)
{
	bool is_enemy = false;

	switch (obj->GetObjType())	
	{
	case Obj::OBJ_TYPE_ROLE:
		{
			Role *target_role = (Role*)obj;
			is_enemy = target_role->IsAlive() && (target_role->GetGuildID() != role->GetGuildID());
		}
		break;

	case Obj::OBJ_TYPE_MONSTER:
		{
			Monster *monster = (Monster*)obj;
			if (monster->IsAlive())
			{
				is_enemy = true;
			}
		}
		break;
	}

	return is_enemy;
}

bool SpecialXianMengZhan::SpecialIsFriend(Role *role, Obj *obj, bool check_alive)
{
	bool is_friend = false;

	switch (obj->GetObjType())	
	{
	case Obj::OBJ_TYPE_ROLE:
		{
			Role *target_role = (Role*)obj;
			is_friend = (!check_alive || target_role->IsAlive()) && (target_role->GetGuildID() == role->GetGuildID());
		}
		break;
	}

	return is_friend;
}


void SpecialXianMengZhan::OnAddObj(Obj *obj) // (在这里处理角色进入 主要是为了在enterscene之前改变角色坐标)
{
	if (!m_isopen) return;

	if (Obj::OBJ_TYPE_ROLE == obj->GetObjType())
	{
		Role *role = (Role*)obj;

		XianMengZhanUser *user = this->GetXianMengZhanUser(role->GetUserId());
		XianMengZhanGuild *guild = this->GetXianMengZhanGuild(role->GetGuildID());
		int enter_area = -1;

		if (NULL != user && NULL != guild) 
		{
			enter_area = guild->enter_area;

			user->net_id = role->GetNetId();
			user->obj_id = role->GetId();
			user->level = role->GetLevel();
			user->guild_id = role->GetGuildID();
			role->GetName(user->user_name);
			user->call_allow = (GUILD_POST_FU_TUANGZHANG == role->GetGuildPost() || GUILD_POST_TUANGZHANG == role->GetGuildPost()); // TODO: 实时生效

			role->GetRoleActivity()->SetXianMengZhanDefendArea(guild->defend_area);

			this->SendUserInfo(user, Protocol::XIANMENGZHAN_USER_INFO_NOTIFY_REASON_DEFAULT);
			this->SendGuildInfo(guild, user);
			this->SendDefendAreaList(user);

			EventHandler::Instance().OnEnterXianmengzhan(role);

			ROLE_LOG_QUICK6(LOG_TYPE_ACTIVITY_XIANMENGZHAN, role, role->GetLevel(), 0, "Join", NULL);
		}

		Posi enterpos(0, 0);
		if (LOGIC_CONFIG->GetXianMengZhanConfig().GetRealivePos(role->GetRoleActivity()->GetXianMengZhanDefendArea(), enter_area, &enterpos))
		{
			role->SetPos(enterpos);
		}
	}
}

void SpecialXianMengZhan::OnRoleLeaveScene(Role *role, bool is_logout)
{
	if (!m_isopen) return;

	XianMengZhanUser *user = this->GetXianMengZhanUser(role->GetUserId());
	if (NULL != user) 
	{
		role->GetRoleActivity()->SetXianMengZhanDefendArea(-1);

		user->obj_id = INVALID_OBJ_ID;
		user->net_id = INVALID_GSNETID;
	}

	role->GetMountManager()->GoonMount(true, false, true);
}

void SpecialXianMengZhan::OnActivityStatusChange(int status)
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

void SpecialXianMengZhan::OnRoleCallGuildMembers(Role *role, bool use_gold)
{
	if (!m_isopen || NULL == role) return;

	XianMengZhanUserMapIt user_it = m_user_map.find(role->GetUserId());
	if (m_user_map.end() != user_it)
	{
		XianMengZhanUser *user = &(user_it->second);

		XianMengZhanGuild *guild = this->GetXianMengZhanGuild(user->guild_id);
		if (NULL != guild)
		{
			if (!user->call_allow)
			{
				role->NoticeNum(errornum::EN_XIANMENGZHAN_NOT_PRIVILEGE);
				return;
			}

			const int guild_call_cd_time = LOGIC_CONFIG->GetXianMengZhanConfig().GetGuildCallCDTime();

			time_t now_second = EngineAdapter::Instance().Time();
			if (now_second < guild->last_call_time + guild_call_cd_time)
			{
				role->NoticeNum(errornum::EN_XIANMENGZHAN_CALL_CD_LIMIT);
				return;
			}

			const int guild_call_count = LOGIC_CONFIG->GetXianMengZhanConfig().GetGuildCallCount();

			if (user->call_count >= guild_call_count)
			{
				if (!use_gold)
				{
					role->NoticeNum(errornum::EN_XIANMENGZHAN_NOT_PRIVILEGE);
					return;
				}

				const int guild_call_gold_cost = LOGIC_CONFIG->GetXianMengZhanConfig().GetGuildCallGoldCost();

				if (!role->GetKnapsack()->GetMoney()->GoldMoreThan(guild_call_gold_cost))
				{
					role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
					return;
				}

				role->GetKnapsack()->GetMoney()->UseGold(guild_call_gold_cost, "XianMengZhanGuildCall");
			}
			else
			{
				++ user->call_count;
			}

			guild->caller_pos_x = role->GetPos().x;
			guild->caller_pos_y = role->GetPos().y;
			guild->last_call_time = now_second;
			this->SendUserInfo(user, Protocol::XIANMENGZHAN_USER_INFO_NOTIFY_REASON_CALL_SUCCESS);
			this->SendGuildCallNotice(guild, role);
			this->SendGuildInfo(guild, user);

			role->NoticeNum(noticenum::NT_XIANMENGZHAN_CALL_MEMBER_SUCC);
		}
	}
}

void SpecialXianMengZhan::OnRoleFollowGuildLeader(Role *role)
{
	if (!m_isopen || NULL == role || !role->IsAlive()) return;

	XianMengZhanUserMapIt user_it = m_user_map.find(role->GetUserId());
	if (m_user_map.end() != user_it)
	{
		XianMengZhanUser *user = &(user_it->second);

		XianMengZhanGuild *guild = this->GetXianMengZhanGuild(user->guild_id);
		if (NULL != guild && 0 != guild->caller_pos_x && 0 != guild->caller_pos_y)
		{
			role->ResetPos(guild->caller_pos_x, guild->caller_pos_y);
		}
	}
}

void SpecialXianMengZhan::OnActivityStandby()
{
	int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_xianmengzhan_standy);
	if (length > 0) World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_ACTIVE_NOTICE);

	this->Clear();
}

void SpecialXianMengZhan::OnActivityStart() 
{
	int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_xianmengzhan_start);
	if (length > 0) World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_ACTIVE_NOTICE);

	m_isopen = true;

	m_next_free_reward_time = EngineAdapter::Instance().Time() + LOGIC_CONFIG->GetXianMengZhanConfig().GetFreeRewardIntervalS();

	this->CheckGatherPoint();							    // 初始化采集点

	{
		// 加血法阵
		int enter_area_count = LOGIC_CONFIG->GetXianMengZhanConfig().GetEnterAreaCount();
		for (int enter_area = 0; enter_area < enter_area_count; ++ enter_area)
		{
			Posi enter_pos(0, 0);
			if (LOGIC_CONFIG->GetXianMengZhanConfig().GetEnterInfo(&enter_pos, enter_area))
			{
				int skill_id = GetSkillIDBySceneTypeAndIndex(m_scene->GetSceneType(), 10);
				SceneEffectSpecialFaZhen *scene_effect = new SceneEffectSpecialFaZhen(m_scene, INVALID_OBJ_ID, skill_id, EffectBase::PRODUCT_METHOD_SCENE_SKILL);
				scene_effect->SetFaZhenInfo(enter_pos, 12, 2000, 900, false, true);

				SkillAttachEffectParam skill_param;
				skill_param.effect_type = SKILL_ATTACH_EFFECT_ADD_HP;
				skill_param.effect_param_list[0] = 10;
				scene_effect->SetEffectParam(SKILL_ATTACH_EFFECT_TARGET_ROLE_PET, 1, &skill_param);

				this->AddSceneEffect(scene_effect);
			}
		}
	}
}

void SpecialXianMengZhan::OnActivityClose() 
{	
	this->Reward();

	this->DelayKickOutAllRole();
	UInt32 monster_num = m_scene->MonsterNum();
	for (int i = 0; i < static_cast<int>(monster_num); ++i)
	{
		Monster *monster = m_scene->GetMonsterByIndex(i);
		if (NULL != monster)
		{
			monster->ForceSetDead();
		}
	}

	for (int i = 0; i < XIANMENGZHAN_DEFEND_AREA_NUM; ++ i)
	{
		if (INVALID_OBJ_ID != m_defend_area_list[i].gather_obj_id)
		{
			Obj *obj = m_scene->GetObj(m_defend_area_list[i].gather_obj_id);
			if (NULL != obj && Obj::OBJ_TYPE_GATHER == obj->GetObjType())
			{
				m_scene->DeleteObj(m_defend_area_list[i].gather_obj_id);
			}
		}
	}

	if (INVALID_OBJ_ID != m_center_gather_obj_id)
	{
		Obj *obj = m_scene->GetObj(m_center_gather_obj_id);
		if (NULL != obj && Obj::OBJ_TYPE_GATHER == obj->GetObjType())
		{
			m_scene->DeleteObj(m_center_gather_obj_id);
		}
	}

	this->Clear();
}

void SpecialXianMengZhan::OnGuildGetDefendArea(XianMengZhanGuild *guild, int defend_area_index)
{
	if (NULL == guild) return;

	if (defend_area_index >= 0 && defend_area_index < XIANMENGZHAN_DEFEND_AREA_NUM)
	{
		XianMengZhanDefendArea &defend_area = m_defend_area_list[defend_area_index];
		if (!defend_area.HasGuild())
		{
			defend_area.data = guild->data;

			this->GuildChangeDefendArea(guild, defend_area_index);
			m_next_sync_defend_area_list_time = EngineAdapter::Instance().Time();
		}
	}
}

void SpecialXianMengZhan::OnGuildDropDefendArea(XianMengZhanGuild *guild, int defend_area_index)
{
	if (NULL == guild) return;

	if (defend_area_index >= 0 && defend_area_index < XIANMENGZHAN_DEFEND_AREA_NUM && defend_area_index == guild->defend_area)
	{
		XianMengZhanDefendArea &defend_area = m_defend_area_list[defend_area_index];

		guild->data = defend_area.data;
		defend_area.data.Clear();

		m_next_sync_defend_area_list_time = EngineAdapter::Instance().Time();
	}
}

void SpecialXianMengZhan::OnGuildLossDefendArea(XianMengZhanGuild *guild, int defend_area_index)
{
	if (NULL == guild) return;

	if (defend_area_index >= 0 && defend_area_index < XIANMENGZHAN_DEFEND_AREA_NUM && defend_area_index == guild->defend_area)
	{
		XianMengZhanDefendArea &defend_area = m_defend_area_list[defend_area_index];

		guild->data = defend_area.data;

		this->GuildChangeDefendArea(guild, -1);
		defend_area.data.Clear();
		m_next_sync_defend_area_list_time = EngineAdapter::Instance().Time();
	}
}

void SpecialXianMengZhan::OnUserGetDefendArea(Role *role, int defend_area)
{
	if (NULL == role || defend_area < 0 || defend_area >= XIANMENGZHAN_DEFEND_AREA_NUM)
	{
		return;
	}

	XianMengZhanUser *user = this->GetXianMengZhanUser(role->GetUserId());
	if (NULL == user)
	{
		m_defend_area_list[defend_area].data.Clear();
		return;
	}

	XianMengZhanGuild *guild = this->GetXianMengZhanGuild(role->GetGuildID());
	if (NULL == guild)
	{
		m_defend_area_list[defend_area].data.Clear();
		return;
	}

	if (guild->defend_area == defend_area)
	{
		return;
	}

	if (m_defend_area_list[defend_area].HasGuild())
	{
		XianMengZhanGuild *old_guild = this->GetXianMengZhanGuild(m_defend_area_list[defend_area].data.guild_id);
		if (NULL != old_guild)
		{
			this->OnGuildLossDefendArea(old_guild, defend_area);

			int length_1 = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_xianmengzhan_occupy_guild_defend_area_notice, guild->data.guild_name, old_guild->data.guild_name, defend_area);
			if (length_1 > 0) this->NotifySystemMsg(gamestring::GAMESTRING_BUF, length_1);

			{
				// 提示仙盟成员失去了据点
				int length_2 = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_xianmengzhan_lost_defend_area, guild->data.guild_name);
				if (length_2 > 0)
				{
					int send_len = 0;
					static Protocol::SCSystemMsg sm;

					if (SceneManager::CreateSystemMsg(&sm, &send_len, gamestring::GAMESTRING_BUF, length_2, SYS_MSG_ACTIVE_NOTICE))
					{
						this->SendToGuild(old_guild->data.guild_id, (const char *)&sm, send_len);
					}
				}
			}
		}
	}
	else
	{
		int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_xianmengzhan_occupy_defend_area_notice, guild->data.guild_name, defend_area);
		if (length > 0) this->NotifySystemMsg(gamestring::GAMESTRING_BUF, length);
	}

	if (guild->HasDefendArea())
	{
		this->OnGuildDropDefendArea(guild, guild->defend_area);
	}

	this->OnGuildGetDefendArea(guild, defend_area);
}

void SpecialXianMengZhan::OnGuildGetCenterArea(GuildID guild_id)
{
	if (INVALID_GUILD_ID == guild_id || guild_id == m_center_guild)
	{
		return;
	}

	// 占领
	m_center_guild = guild_id;
	m_hold_center_timestamp = EngineAdapter::Instance().Time();
	m_last_center_area_reward_time = EngineAdapter::Instance().Time();

	this->SendDefendAreaList(NULL);

	XianMengZhanGuild *guild = this->GetXianMengZhanGuild(m_center_guild);
	if (NULL != guild)
	{
		int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_xianmengzhan_occupy_center_area_notice, guild->data.guild_name);
		if (length > 0) 
		{
			this->NotifySystemMsg(gamestring::GAMESTRING_BUF, length, SYS_MSG_ACTIVE_NOTICE);
		}
	}
}

void SpecialXianMengZhan::OnUserDead(XianMengZhanUser *user, XianMengZhanUser *killer, Role *killer_role, const Posi &dead_pos)
{
	if (NULL == user || NULL == killer || NULL == killer_role) return;

	unsigned int now = (unsigned int)EngineAdapter::Instance().Time();

	this->UserAddKillCount(killer, 1);

	{
		std::map<UserID, time_t>::iterator it = killer->kill_user_map.find(user->user_id);
		if (it != killer->kill_user_map.end())
		{
			time_t old_time = it->second;

			if (now < old_time + XIANMENGZHAN_KILL_VALID_TIME_S)
			{
				killer_role->NoticeNum(errornum::EN_BATTLE_FIELD_KILL_USER_INVALID);
				return;
			}

			it->second = now;
		}
		else
		{
			killer->kill_user_map[user->user_id] = now;
		}

		int reward_user_score = 0, reward_killer_score = 0;
		if (LOGIC_CONFIG->GetXianMengZhanConfig().GetRewardKill(dead_pos, &reward_killer_score, &reward_user_score))
		{
			this->UserAddScore(user, reward_user_score);
			this->UserAddScore(killer, reward_killer_score);
			killer->kill_score += reward_killer_score;
		}
	}

	{
		for (std::map<UserID, time_t>::iterator it = user->hurt_map.begin(), end = user->hurt_map.end(); it != end; ++ it)
		{
			if (it->second + XIANMENGZHAN_ASSIST_VALID_TIME_S >= now && it->first != killer->user_id)
			{
				XianMengZhanUser *assistuser = this->GetXianMengZhanUser(it->first);
				if (NULL != assistuser && assistuser->guild_id == killer->guild_id)
				{
					this->UserAddScore(assistuser, 2);
					assistuser->assist_count += 1;
				}
			}
		}

		user->hurt_map.clear();
	}
}

void SpecialXianMengZhan::OnResetLianzhan(XianMengZhanUser *user)
{
	if (NULL == user)
	{
		return;
	}

	user->lianzhan = 0;

	this->BroadcastLianzhanInfo(user);
}

void SpecialXianMengZhan::GuildAddScore(XianMengZhanGuild *guild, int score)
{
	if (NULL == guild) return;

	guild->data.guild_score += score;
	guild->dirty = true;

	if (guild->defend_area >= 0 && guild->defend_area < XIANMENGZHAN_DEFEND_AREA_NUM)
	{
		m_defend_area_list[guild->defend_area].data.guild_score += score;
	}
}

void SpecialXianMengZhan::GuildChangeDefendArea(XianMengZhanGuild *guild, int defend_area)
{
	if (NULL == guild) return;

	if (defend_area >= -1 && defend_area < XIANMENGZHAN_DEFEND_AREA_NUM)
	{
		if (defend_area != guild->defend_area)
		{
			guild->defend_area = defend_area;
			guild->dirty = true;

			for (XianMengZhanUserMapIt it = m_user_map.begin(), end = m_user_map.end(); end != it; ++it)
			{
				if (guild->data.guild_id == it->second.guild_id)
				{
					Role *role = this->GetRoleHelper(&(it->second));
					if (NULL != role)
					{
						role->GetRoleActivity()->SetXianMengZhanDefendArea(guild->defend_area);
					}
				}
			}
		}
	}
}

void SpecialXianMengZhan::UserAddScore(XianMengZhanUser *user, int score, bool add_to_guild)
{
	if (NULL == user) return;

	if (score > 0)
	{
		user->score += score;
		user->dirty = true;

		if (add_to_guild)
		{
			XianMengZhanGuild *guild = this->GetXianMengZhanGuild(user->guild_id);
			if (NULL != guild) this->GuildAddScore(guild, score);
		}
	}
}

void SpecialXianMengZhan::UserAddKillCount(XianMengZhanUser *user, int kill_count)
{
	if (NULL == user) return;

	user->kill_count += kill_count;
	user->lianzhan += kill_count;
	user->dirty = true;

	this->BroadcastLianzhanInfo(user);
}

void SpecialXianMengZhan::CheckGatherPoint(time_t now_second)
{
	if (!m_isopen) return;

	if (INVALID_OBJ_ID == m_center_gather_obj_id)
	{
		GatherObj *gather_obj = new GatherObj();
		gather_obj->Init(NULL, LOGIC_CONFIG->GetXianMengZhanConfig().GetCenterAreaGatherID(), 10 * 1000, 0, false);
		gather_obj->SetPos(LOGIC_CONFIG->GetXianMengZhanConfig().GetCenterAreaPos());
		m_scene->AddObj(gather_obj);

		m_center_gather_obj_id = gather_obj->GetId();
	}

	/* 干掉外围据点
	for (int i = 0; i < XIANMENGZHAN_DEFEND_AREA_NUM; ++ i)
	{
		XianMengZhanDefendArea &defend_area = m_defend_area_list[i];

		if (INVALID_OBJ_ID == defend_area.gather_obj_id)
		{
			int gather_id; Posi gather_pos;
			if (LOGIC_CONFIG->GetXianMengZhanConfig().GetDefendAreaGatherInfo(i, &gather_id, &gather_pos))
			{
				GatherObj *gather_obj = new GatherObj();
				gather_obj->Init(NULL, gather_id, 10 * 1000, 0, false);
				gather_obj->SetPos(gather_pos);
				m_scene->AddObj(gather_obj);

				defend_area.gather_obj_id = gather_obj->GetId();
			}
		}
	}
	*/
}

void SpecialXianMengZhan::CheckDefendAreaReward(time_t now_second)
{
	const int defend_area_reward_interval = LOGIC_CONFIG->GetXianMengZhanConfig().GetDefendAreaRewardInterval();
	const int reward_has_defend_area = LOGIC_CONFIG->GetXianMengZhanConfig().GetRewardHasDefendArea();

	if (now_second >= m_last_defend_area_reward_time + defend_area_reward_interval)
	{
		for (XianMengZhanUserMapIt it = m_user_map.begin(), end = m_user_map.end(); end != it; ++it)
		{
			if (INVALID_OBJ_ID != it->second.obj_id)
			{
				XianMengZhanGuild *guild = this->GetXianMengZhanGuild(it->second.guild_id);
				if (NULL == guild) continue;
	
				if (guild->HasDefendArea()) 
				{
					this->UserAddScore(&(it->second), reward_has_defend_area);
				}
			}
		}

		m_last_defend_area_reward_time = now_second;
	}
}

void SpecialXianMengZhan::CheckCenterArea(time_t now_second)
{
	{
		//奖励积分
		if (INVALID_GUILD_ID != m_center_guild)
		{
			const int center_area_reward_interval = LOGIC_CONFIG->GetXianMengZhanConfig().GetHasCenterRewardInterval();
			const int reward_has_center_area = LOGIC_CONFIG->GetXianMengZhanConfig().GetHasCenterRewardData();

			if (now_second >= m_last_center_area_reward_time + center_area_reward_interval)
			{
				for (XianMengZhanUserMapIt it = m_user_map.begin(), end = m_user_map.end(); end != it; ++it)
				{
					if (INVALID_OBJ_ID != it->second.obj_id && m_center_guild == it->second.guild_id)
					{
						this->UserAddScore(&(it->second), reward_has_center_area);
					}
				}

				m_last_center_area_reward_time = now_second;
			}
		}
	}
}

void SpecialXianMengZhan::CheckLianZhanNotice(Role *kill_role, Role *dead_role)
{
	if (NULL != kill_role && NULL != dead_role)
	{
		XianMengZhanUser *xianmengzhan_killuser = this->GetXianMengZhanUser(kill_role->GetUserId());
		XianMengZhanUser *xianmengzhan_deaduser = this->GetXianMengZhanUser(dead_role->GetUserId());
		if (NULL != xianmengzhan_killuser && NULL != xianmengzhan_deaduser)
		{
			{
				// 击杀额外奖励
				int extra_reward = xianmengzhan_killuser->lianzhan + xianmengzhan_deaduser->lianzhan;
				this->UserAddScore(xianmengzhan_killuser, extra_reward);
			}

			if (xianmengzhan_deaduser->lianzhan >= 3)
			{
				int str_len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_pvpzhanchang_stoplianzhan_notice,
					UidToInt(kill_role->GetUserId()), kill_role->GetName(), (int)kill_role->GetCamp(), 
					UidToInt(dead_role->GetUserId()), dead_role->GetName(), (int)dead_role->GetCamp(), xianmengzhan_deaduser->lianzhan);
				if (str_len > 0) 
				{
					this->NotifySystemMsg(gamestring::GAMESTRING_BUF, str_len);
				}
			}

			if (xianmengzhan_killuser->lianzhan > 0)
			{
				int str_len = 0;
				//int notice_type = SYS_MSG_ACTIVE_NOTICE;
				int notice_type = SYS_MSG_CENTER_NOTICE_3;

				if (3 == xianmengzhan_killuser->lianzhan)
				{
					str_len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_pvpzhanchang_lianzhan_notice1,
						UidToInt(kill_role->GetUserId()), kill_role->GetName(), (int)kill_role->GetCamp(), xianmengzhan_killuser->lianzhan);
				}
				else if (4 == xianmengzhan_killuser->lianzhan)
				{
					str_len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_pvpzhanchang_lianzhan_notice2,
						UidToInt(kill_role->GetUserId()), kill_role->GetName(), (int)kill_role->GetCamp(), xianmengzhan_killuser->lianzhan);
				}
				else if (5 == xianmengzhan_killuser->lianzhan)
				{
					str_len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_pvpzhanchang_lianzhan_notice3,
						UidToInt(kill_role->GetUserId()), kill_role->GetName(), (int)kill_role->GetCamp(), xianmengzhan_killuser->lianzhan);
				}
				else if (6 == xianmengzhan_killuser->lianzhan)
				{
					str_len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_pvpzhanchang_lianzhan_notice4,
						UidToInt(kill_role->GetUserId()), kill_role->GetName(), (int)kill_role->GetCamp(), xianmengzhan_killuser->lianzhan);
				}
				else if (7 == xianmengzhan_killuser->lianzhan)
				{
					str_len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_pvpzhanchang_lianzhan_notice5,
						UidToInt(kill_role->GetUserId()), kill_role->GetName(), (int)kill_role->GetCamp(), xianmengzhan_killuser->lianzhan);
				}
				else if (8 == xianmengzhan_killuser->lianzhan)
				{
					str_len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_pvpzhanchang_lianzhan_notice6,
						UidToInt(kill_role->GetUserId()), kill_role->GetName(), (int)kill_role->GetCamp(), xianmengzhan_killuser->lianzhan);
				}
				else if (9 == xianmengzhan_killuser->lianzhan)
				{
					str_len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_pvpzhanchang_lianzhan_notice7,
						UidToInt(kill_role->GetUserId()), kill_role->GetName(), (int)kill_role->GetCamp(), xianmengzhan_killuser->lianzhan);
				}
				else if (xianmengzhan_killuser->lianzhan >= 10)
				{
					str_len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_pvpzhanchang_lianzhan_notice8,
						UidToInt(kill_role->GetUserId()), kill_role->GetName(), (int)kill_role->GetCamp(), xianmengzhan_killuser->lianzhan);
				}

				if (str_len > 0) this->NotifySystemMsg(gamestring::GAMESTRING_BUF, str_len, notice_type);
			}
		}
	}
}

void SpecialXianMengZhan::SyncUserInfo(time_t now_second)
{
	if (0 == now_second || now_second >= m_last_sync_user_info_time + XIANMENGZHAN_SYNC_USER_INFO_TIME_S)
	{
		for (XianMengZhanUserMapIt it = m_user_map.begin(), end = m_user_map.end(); end != it; ++it)
		{
			if (INVALID_OBJ_ID != it->second.obj_id && it->second.dirty) 
			{
				this->SendUserInfo(&(it->second), Protocol::XIANMENGZHAN_USER_INFO_NOTIFY_REASON_DEFAULT);
			}
		}
		m_last_sync_user_info_time = now_second;
	}
}

void SpecialXianMengZhan::SyncGuildInfo(time_t now_second)
{
	if (0 == now_second || now_second >= m_last_sync_guild_info_time + XIANMENGZHAN_SYNC_GUILD_INFO_TIME_S)
	{
		for (XianMengZhanGuildMapIt it = m_guild_map.begin(), end = m_guild_map.end(); end != it; ++it)
		{
			if (it->second.dirty)
			{
				this->SendGuildInfo(&(it->second));
			}
		}
		m_last_sync_guild_info_time = now_second;
	}
}

void SpecialXianMengZhan::SyncDefendAreaList(time_t now_second)
{
	if (0 == now_second || now_second >= m_next_sync_defend_area_list_time)
	{
		this->SendDefendAreaList();
		m_next_sync_defend_area_list_time = now_second + XIANMENGZHAN_SYNC_DEFEND_AREA_LIST_TIME_S;
	}
}


struct XianMengZhanGuildRankItem
{
	GuildName guild_name;
	GuildID guild_id;
	int score;
	long long rank_score;
	char camp;
};

bool GuildSortFunc(const XianMengZhanGuildRankItem &item1, const XianMengZhanGuildRankItem &item2)
{
	return item1.rank_score > item2.rank_score;
}

struct XianMengZhanUserRankItem
{
	UserID user_id;
	int score;
};
bool UserSortFunc(const XianMengZhanUserRankItem &item1, const XianMengZhanUserRankItem &item2)
{
	return item1.score > item2.score;
}

void SpecialXianMengZhan::SynGuildRankInfo(time_t now_second)
{
	if (0 == now_second || now_second >= m_next_sync_guild_rank_time)
	{
		m_next_sync_guild_rank_time = EngineAdapter::Instance().Time() + XIANMENGZHAN_SYNC_GUILD_RANK_TIME_S;

		std::vector<XianMengZhanGuildRankItem> guild_rank;
		for (XianMengZhanGuildMapIt it = m_guild_map.begin(), end = m_guild_map.end(); end != it; ++it)
		{
			if (it->second.data.guild_score > 0)
			{
				XianMengZhanGuildRankItem rank_item;
				F_STRNCPY(rank_item.guild_name, it->second.data.guild_name, sizeof(rank_item.guild_name));
				rank_item.guild_id = it->second.data.guild_id;
				rank_item.score = it->second.data.guild_score;
				rank_item.camp = it->second.data.camp;
				rank_item.rank_score = it->second.data.guild_score;

				guild_rank.push_back(rank_item);
			}
		}
		std::sort(guild_rank.begin(), guild_rank.end(), GuildSortFunc);

		static Protocol::SCXianMengZhanGuildRankInfo xmzgri;
		xmzgri.rank_count = 0;

		for (std::vector<XianMengZhanGuildRankItem>::iterator it = guild_rank.begin(); it != guild_rank.end() && 
			xmzgri.rank_count < Protocol::SCXianMengZhanGuildRankInfo::GUILD_RANK_ITEM_MAX_COUNT; ++ it)
		{
			xmzgri.rank_item_list[xmzgri.rank_count].guild_id = it->guild_id;
			F_STRNCPY(xmzgri.rank_item_list[xmzgri.rank_count].guild_name, it->guild_name, sizeof(xmzgri.rank_item_list[xmzgri.rank_count].guild_name));
			xmzgri.rank_item_list[xmzgri.rank_count].guild_score = it->score;
			xmzgri.rank_item_list[xmzgri.rank_count].camp = it->camp;

			++ xmzgri.rank_count;
		}

		int send_len = sizeof(xmzgri) - sizeof(xmzgri.rank_item_list[0]) * (Protocol::SCXianMengZhanGuildRankInfo::GUILD_RANK_ITEM_MAX_COUNT - xmzgri.rank_count);
		m_scene->SendToRole((const char *)&xmzgri, send_len);
	}
}

void SpecialXianMengZhan::SendToGuild(GuildID guild_id, const char *data, unsigned int length, UserID exclude_user)
{
	if (NULL == data) return;

	int netid_size = 0;
	static GSNetID netid_list[MAX_MEMBER_COUNT];

	for (XianMengZhanUserMapIt it = m_user_map.begin(), end = m_user_map.end(); end != it && netid_size < MAX_MEMBER_COUNT; ++it)
	{
		if (guild_id == it->second.guild_id && INVALID_GSNETID != it->second.net_id && exclude_user != it->second.user_id)
		{
			netid_list[netid_size++] = it->second.net_id;
		}
	}

	EngineAdapter::Instance().NetMulSend(netid_list, netid_size, data, length);
}

void SpecialXianMengZhan::SendUserInfo(XianMengZhanUser *user, int notify_reason)
{
	if (NULL == user || INVALID_GSNETID == user->net_id) return;

	Protocol::SCXianMengZhanUserInfo xmzui;
	xmzui.notify_reason = notify_reason;
	xmzui.call_count = user->call_count;
	xmzui.call_allow  = user->call_allow ? 1 : 0;
	xmzui.reserve_ch  = 0;
	xmzui.kill_count  = user->kill_count;
	xmzui.score       = user->score;
	xmzui.last_realive_here_timestamp = user->last_realive_here_timestamp;
	xmzui.lianzhan	  = user->lianzhan;
	xmzui.reserve_sh  = 0;
	xmzui.assist_count = user->assist_count;

	EngineAdapter::Instance().NetSend(user->net_id, (const char*)&xmzui, sizeof(xmzui));

	user->dirty = false;
}

void SpecialXianMengZhan::SendGuildInfo(XianMengZhanGuild *guild, XianMengZhanUser *user)
{
	if (NULL == guild) return;

	Protocol::SCXianMengZhanGuildInfo xmzgi;
	xmzgi.area_index = guild->defend_area;
	xmzgi.score  = guild->data.guild_score;
	xmzgi.last_call_time = static_cast<unsigned int>(guild->last_call_time);

	if (NULL == user)
	{
		this->SendToGuild(guild->data.guild_id, (const char*)&xmzgi, sizeof(xmzgi));
		guild->dirty = false;
	}
	else if (INVALID_GSNETID != user->net_id)
	{
		EngineAdapter::Instance().NetSend(user->net_id, (const char*)&xmzgi, sizeof(xmzgi));
	}
}

void SpecialXianMengZhan::SendDefendAreaList(XianMengZhanUser *user)
{
	Protocol::SCXianMengZhanDefendAreaInfo xmzdai;

	memset(xmzdai.center_area_guild_name, 0, sizeof(xmzdai.center_area_guild_name));
	xmzdai.center_area_guild_id = INVALID_GUILD_ID;

	XianMengZhanGuild *guild = this->GetXianMengZhanGuild(m_center_guild);
	if (NULL != guild)
	{
		xmzdai.center_area_guild_id = m_center_guild;
		F_STRNCPY(xmzdai.center_area_guild_name, guild->data.guild_name, sizeof(xmzdai.center_area_guild_name));
		xmzdai.center_area_guild_score = guild->data.guild_score;
		xmzdai.center_area_guild_camp = static_cast<char>(guild->data.camp);
		xmzdai.reserve_ch = 0;
		xmzdai.reserve_sh = 0;
	}

	xmzdai.area_count = 0;

	for (int i = 0; i < XIANMENGZHAN_DEFEND_AREA_NUM; ++i)
	{
		xmzdai.defend_area_list[i].area_index = i;
		F_STRNCPY(xmzdai.defend_area_list[i].guild_name, m_defend_area_list[i].data.guild_name, sizeof(xmzdai.defend_area_list[0].guild_name));
		xmzdai.defend_area_list[i].guild_score = m_defend_area_list[i].data.guild_score;
		xmzdai.defend_area_list[i].has_center_area = (INVALID_GUILD_ID != m_center_guild && m_defend_area_list[i].data.guild_id == m_center_guild) ? 1 : 0;
		xmzdai.defend_area_list[i].guild_id = m_defend_area_list[i].data.guild_id;
		xmzdai.defend_area_list[i].camp= static_cast<char>(m_defend_area_list[i].data.camp);
		xmzdai.defend_area_list[i].reserve_ch = 0;
		xmzdai.defend_area_list[i].reserve_sh = 0;
	}

	xmzdai.area_count = XIANMENGZHAN_DEFEND_AREA_NUM;

	int send_len = sizeof(xmzdai) - (sizeof(xmzdai.defend_area_list) - sizeof(xmzdai.defend_area_list[0]) * xmzdai.area_count);

	if (NULL == user)
	{
		m_scene->SendToRole((const char*)&xmzdai, send_len);
	}
	else if (INVALID_GSNETID != user->net_id)
	{
		EngineAdapter::Instance().NetSend(user->net_id, (const char*)&xmzdai, send_len);
	}
}

void SpecialXianMengZhan::SendGuildCallNotice(XianMengZhanGuild *guild, Role *caller)
{
	if (NULL == guild) return;

	Protocol::SCXianMengZhanGuildCallNotice xmzgcn;
	caller->GetName(xmzgcn.caller_name);

	this->SendToGuild(guild->data.guild_id, (const char*)&xmzgcn, sizeof(xmzgcn), caller->GetUserId());
}

void SpecialXianMengZhan::NotifySystemMsg(char *str_buff, int str_len, int msg_type)
{
	if (NULL != str_buff && str_len > 0) 
	{
		int sendlen = 0;
		static Protocol::SCSystemMsg sm;

		if (SceneManager::CreateSystemMsg(&sm, &sendlen, str_buff, str_len, msg_type))
		{
			m_scene->SendToRole((const char*)&sm, sendlen);
		}
	}
}

Role * SpecialXianMengZhan::GetRoleHelper(XianMengZhanUser *user)
{
	if (NULL != user && INVALID_OBJ_ID != user->obj_id)
	{
		Obj *obj = m_scene->GetObj(user->obj_id);
		if (NULL != obj && Obj::OBJ_TYPE_ROLE == obj->GetObjType())
		{
			Role *role = (Role *)obj;
			if (role->GetUserId() == user->user_id)
			{
				return role;
			}
		}
	}

	return NULL;
}

XianMengZhanUser * SpecialXianMengZhan::GetXianMengZhanUser(UserID user_id)
{
	if (INVALID_USER_ID == user_id) return NULL;

	XianMengZhanUserMapIt it = m_user_map.find(user_id);

	if (m_user_map.end() == it) 
	{
		XianMengZhanUser user;
		user.user_id = user_id;

		m_user_map[user_id] = user;
		it = m_user_map.find(user_id);
	}

	if (m_user_map.end() != it) return &(it->second);

	return NULL;
}

XianMengZhanGuild * SpecialXianMengZhan::GetXianMengZhanGuild(GuildID guild_id)
{
	if (INVALID_GUILD_ID == guild_id) return NULL;

	XianMengZhanGuildMapIt it = m_guild_map.find(guild_id);

	if (m_guild_map.end() == it) 
	{
		XianMengZhanGuild guild; 
		guild.data.guild_id = guild_id;
		guild.data.camp = GuildShadow::Instance().GetGuildCamp(guild_id);
		GuildShadow::Instance().GetGuildName(guild_id, guild.data.guild_name);
		guild.enter_area = 0;
		
		int enter_area_count = LOGIC_CONFIG->GetXianMengZhanConfig().GetEnterAreaCount();
		if (enter_area_count > 0)
		{
			guild.enter_area = RandomNum(enter_area_count);
		}

		m_guild_map[guild_id] = guild;
		it = m_guild_map.find(guild_id);
	} 

	return (m_guild_map.end() != it) ? &(it->second) : NULL;
}

void SpecialXianMengZhan::CalcMonsterAddAttrs(int monster_id, int *add_maxhp, int *add_gongji)
{
	if (monster_id <= 0 || NULL == add_maxhp || NULL == add_gongji) return;

	*add_maxhp = 0; *add_gongji = 0;
}

void SpecialXianMengZhan::BroadcastLianzhanInfo(XianMengZhanUser *user)
{
	if (NULL != user)
	{
		Role * role = this->GetRoleHelper(user);
		if (NULL != role)
		{
			Protocol::SCXianmengzhanLianzhanChange xmzlzc;
			xmzlzc.obj_id = role->GetId();
			xmzlzc.lianzhan = user->lianzhan;
			m_scene->GetZoneMatrix()->NotifyAreaMsg(role->GetObserHandle(),(void*)&xmzlzc, sizeof(xmzlzc));
		}
	}
}

void SpecialXianMengZhan::Reward()
{
	std::vector<XianMengZhanGuildRankItem> guild_rank;
	for (XianMengZhanGuildMapIt it = m_guild_map.begin(), end = m_guild_map.end(); end != it; ++it)
	{
		XianMengZhanGuildRankItem rank_item;
		F_STRNCPY(rank_item.guild_name, it->second.data.guild_name, sizeof(rank_item.guild_name));
		rank_item.guild_id = it->second.data.guild_id;
		rank_item.score = it->second.data.guild_score;
		rank_item.camp = it->second.data.camp;
		rank_item.rank_score = it->second.data.guild_score;

		guild_rank.push_back(rank_item);
	}
	std::sort(guild_rank.begin(), guild_rank.end(), GuildSortFunc);

	if (!guild_rank.empty())
	{
		if (guild_rank[0].score > 0)
		{
			Guild *guild = GuildManager::Instance().GetGuild(guild_rank[0].guild_id);
			if (NULL != guild)
			{
				GameName mengzhu_name;
				UserID mengzhu_userid = guild->GetGuildRegister().GetGuildTuanZhangInfo(mengzhu_name); 

				WorldStatus::Instance().SetXianmengzhanMengzhu(UidToInt(mengzhu_userid));
			}
		}
	}

	{
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

		// 排名奖励
		for (int rank_index = 0; rank_index < XIANMENGZHAN_REWARD_GUILD_RANK_MAX && rank_index < static_cast<int>(guild_rank.size()); ++ rank_index)
		{
			GuildID guild_id = guild_rank[rank_index].guild_id;

			for (XianMengZhanUserMapIt it = m_user_map.begin(), end = m_user_map.end(); end != it; ++it)
			{
				if (guild_id == it->second.guild_id)
				{
					// 发邮件
					const XianMengZhanConfig::RankRewardCfg::RewardDetail *reward_detail = LOGIC_CONFIG->GetXianMengZhanConfig().GetRankReward(rank_index + 1, it->second.level);
					if (NULL != reward_detail)
					{
						static MailContentParam contentparam; contentparam.Reset();
						contentparam.AddVirtualItem(0, MAIL_VIRTUAL_ITEM_GUILDGONGXIAN, reward_detail->gongxian * reward_factor);
						contentparam.AddVirtualItem(1, MAIL_VIRTUAL_ITEM_BATTLEFIELDHONOR, reward_detail->honor * reward_factor);

						long long reward_exp = reward_detail->exp;
						reward_exp += WorldShadow::Instance().GetWorLeveldExtraExp(it->second.level, reward_exp);

						contentparam.AddVirtualItem(2, MAIL_VIRTUAL_ITEM_EXP, reward_exp);

						int l1 = SNPRINTF(contentparam.subject, sizeof(contentparam.subject), gamestring::g_xianmengzhan_reward_subject);
						int l2 = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_xianmengzhan_reward_content, rank_index + 1);

						if (l1 > 0 && l2 > 0)
						{
							MailRoute::MailToUser(it->second.user_id, SYSTEM_MAIL_REASON_INVALID, contentparam);
						}
					}
				}

				if (0 == rank_index)
				{
					XianMengZhanGuild *guild = this->GetXianMengZhanGuild(guild_id);
					if (NULL != guild)
					{
						int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_xianmengzhan_close, guild->data.guild_name);
						if (length > 0) World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_ACTIVE_NOTICE);
					}
				}
			}
		}
	}

	{
		// 所有仙盟排名奖励
		for (int rank_index = 0; rank_index < static_cast<int>(guild_rank.size()); ++ rank_index)
		{
			GuildID guild_id = guild_rank[rank_index].guild_id;

			const int MAX_GUILD_MEMBER_COUNT = 256; 

			int user_count = 0;
			UserID user_id_list[MAX_GUILD_MEMBER_COUNT]; 

			bool is_in_rank = false;
			const ItemConfigData *reward_item = LOGIC_CONFIG->GetXianMengZhanConfig().GetRewardGuildRankGift(rank_index + 1, &is_in_rank);
			if (NULL == reward_item) continue;

			static MailContentParam contentparam; contentparam.Reset();
			contentparam.item_list[0].item_id = reward_item->item_id;
			contentparam.item_list[0].num = reward_item->num;
			contentparam.item_list[0].is_bind = reward_item->is_bind;

			int l1 = 0;
			if (is_in_rank)
			{
				l1 = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_xianmengzhan_guild_in_rank_context, rank_index + 1);
			}
			else
			{
				l1 = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_xianmengzhan_guild_not_in_rank_context);
			}


			for (XianMengZhanUserMapIt it = m_user_map.begin(), end = m_user_map.end(); end != it && user_count < MAX_GUILD_MEMBER_COUNT; ++it)
			{
				if (guild_id == it->second.guild_id)
				{
					user_id_list[user_count] = it->second.user_id;
					++ user_count;

					{
						// 仙盟战参与日志
						gamelog::g_log_roleactivity.printf(LL_INFO, "Xianmengzhan::JoinRecord user[%d, %s], guild_id[%d], level[%d], kill[%d], score[%d]",
							UidToInt(it->second.user_id), it->second.user_name, it->second.guild_id, it->second.level, it->second.kill_count, it->second.score);

						LOG_QUICK8(gamelog::g_log_quick, LOG_TYPE_ACTIVITY_XIANMENGZHAN, UidToInt(it->second.user_id), it->second.user_name, NULL, it->second.level, it->second.free_score, "Reward", NULL);
					}
				}
			}

			gamelog::g_log_roleactivity.printf(LL_INFO, "Xianmengzhan:GuildRank guild_id[%d], rank[%d], score[%d], rank_score[%lld]", 
				guild_id, rank_index, guild_rank[rank_index].score, guild_rank[rank_index].rank_score);

			if (l1 > 0 && user_count > 0)
			{
				MailRoute::MailToMultUser(user_id_list, user_count, SYSTEM_MAIL_REASON_INVALID, contentparam);
			}
		}
	}

	{
		// 仙盟战结束同步排行榜
		gglobalprotocal::SyncGuildRankInfo guild_rank_info;
		guild_rank_info.guild_rank_type = GUILD_RANK_TYPE_XIANMENGZHAN;
		int count = 0;
		{
			for (int i = 0; i < (int)guild_rank.size() && count < gglobalprotocal::SyncGuildRankInfo::MAX_GUILD_RANK_NUM; ++ i)
			{
				if (guild_rank[i].rank_score <= 0)
				{
					continue;
				}

				guild_rank_info.guild_rank_list[count].guild_id = guild_rank[i].guild_id;
				guild_rank_info.guild_rank_list[count].rank_value = guild_rank[i].rank_score;

				++ count;
			}

			guild_rank_info.guild_rank_count = count;
		}

		RankManager::Instance().SyncGuildRankInfo(&guild_rank_info);
	}

	{
		for (XianMengZhanUserMapIt it = m_user_map.begin(), end = m_user_map.end(); end != it; ++it)
		{
			for (int rank_index = 0; rank_index < static_cast<int>(guild_rank.size()); ++ rank_index)
			{
				if (guild_rank[rank_index].guild_id == it->second.guild_id)
				{
					// 结果协议
					if (INVALID_GSNETID != it->second.net_id)
					{
						static Protocol::SCXianMengZhanResult xmzr;
						xmzr.guild_rank = rank_index + 1;
						xmzr.guild_score = guild_rank[rank_index].score;
						xmzr.kill_score = it->second.kill_score;
						xmzr.total_score = it->second.score;
						xmzr.assist_count = it->second.assist_count;
						EngineAdapter::Instance().NetSend(it->second.net_id, (const char*)&xmzr, sizeof(xmzr));
					}
				}	
			}

			{
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

				// 积分奖励
				long long reward_exp = (long long)it->second.score * 10;
				int reward_coin = it->second.score * 100 * reward_factor;

				Role *role = World::GetInstWorld()->GetSceneManager()->GetRole(it->second.user_id);
				if (NULL != role)
				{
					role->GetKnapsack()->GetMoney()->AddCoinBind(reward_coin, "XianmengzhanScoreReward");
					role->AddExp(reward_exp, "XianmengzhanScoreReward", Role::EXP_ADD_REASON_DEFAULT);
				}
				else
				{	
					// 发邮件
					static MailContentParam contentparam; contentparam.Reset();

					contentparam.AddVirtualItem(0, MAIL_VIRTUAL_ITEM_BIND_COIN, reward_coin);

					reward_exp += WorldShadow::Instance().GetWorLeveldExtraExp(it->second.level, reward_exp);

					contentparam.AddVirtualItem(1, MAIL_VIRTUAL_ITEM_EXP, reward_exp);

					int l1 = SNPRINTF(contentparam.subject, sizeof(contentparam.subject), gamestring::g_xianmengzhan_join_reward_subject);
					int l2 = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_xianmengzhan_join_reward_content);

					if (l1 > 0 && l2 > 0)
					{
						MailRoute::MailToUser(it->second.user_id, SYSTEM_MAIL_REASON_INVALID, contentparam);
					}				
				}
			}
		}
	}
}

void SpecialXianMengZhan::Clear()
{
	m_next_sync_defend_area_list_time = 0;
	m_next_sync_guild_rank_time = 0;
	m_last_sync_guild_info_time = 0;
	m_last_sync_user_info_time = 0;
	m_last_center_area_reward_time = 0;
	m_last_center_area_check_time = 0;
	m_last_defend_area_reward_time = 0;
	m_next_free_reward_time = 0;
	m_center_gather_obj_id = INVALID_OBJ_ID;
	
	for (int i = 0; i < XIANMENGZHAN_DEFEND_AREA_NUM; ++i)
	{
		m_defend_area_list[i].Clear();
	}

	m_guild_map.clear();
	m_user_map.clear();

	m_center_guild = INVALID_GUILD_ID;
	m_isopen = false;
}


void SpecialXianMengZhan::RealiveHereHelper(Role *role, int realive_type)
{
	if (NULL == role)
	{
		return;
	}

	XianMengZhanUser *xianmengzhan_user = this->GetXianMengZhanUser(role->GetUserId());
	if (NULL != xianmengzhan_user)
	{
		unsigned int now = static_cast<unsigned int>(EngineAdapter::Instance().Time());
		if (now >= xianmengzhan_user->last_realive_here_timestamp + XIANMENGZHAN_ROLE_REALIVE__HERE_TIME_S - 1)
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
				if (!ItemExtern::ConsumeItem(role, consume_list, "SpecialXianMengZhan::RealiveHereHelper"))
				{
					return;
				}
			}

			role->ReAlive(100, 100, role->GetPos(), true);
			xianmengzhan_user->last_realive_here_timestamp = now;
			this->SendUserInfo(xianmengzhan_user, Protocol::XIANMENGZHAN_USER_INFO_NOTIFY_REASON_DEFAULT);
		}
		else
		{
			int relive_cd_left = XIANMENGZHAN_ROLE_REALIVE__HERE_TIME_S - (now - xianmengzhan_user->last_realive_here_timestamp);
			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_xianmengzhan_relive_cd_left, relive_cd_left);

			if (length > 0) 
			{
				role->SendPersonalMsg(length, gamestring::GAMESTRING_BUF);
			}
		}
	}
}

