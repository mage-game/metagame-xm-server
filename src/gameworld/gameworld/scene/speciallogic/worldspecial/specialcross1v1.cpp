#include "specialcross1v1.hpp"
#include "obj/character/role.h"

#include "protocal/msgactivity.hpp"
#include "protocal/msgfight.h"
#include "servercommon/internalprotocal/crossgameprotocal.h"

#include "engineadapter.h"

#include "servercommon/errornum.h"
#include "servercommon/string/gameworldstr.h"
#include "servercommon/serverconfig/cross1v1commonconfig.hpp"

#include "obj/character/roleshadow.hpp"

#include "other/route/mailroute.hpp"
#include "other/cross/rolecross.h"

#include "internalcomm.h"
#include "gamelog.h"
#include "servercommon/userprotocal/msgcrosscommon.h"
#include "scene/activityshadow/activityshadow.hpp"
#include "other/event/eventhandler.hpp"
#include "gameworld/config/logicconfigmanager.hpp"
#include "gameworld/gameworld/ai/roleai/roleaichallengefile.hpp"

SpecialCross1v1::SpecialCross1v1(Scene *scene)
	: SpecialLogic(scene), m_inited(false), m_finish(false), m_fight_start(false), m_fight_start_time(0), m_fight_end_time(0), m_delay_kickout_time(0)
{

}

SpecialCross1v1::~SpecialCross1v1()
{

}

void SpecialCross1v1::Update(unsigned long interval, time_t now_second)
{
	SpecialLogic::Update(interval, now_second);

	const Cross1V1OtherConfig &other_cfg = Cross1V1CommonConfig::Instance().GetOtherCfg();
	if (!m_inited)
	{
		m_inited = true;

		m_fight_end_time = m_scene->GetCreateTime() + other_cfg.fight_wait_s + other_cfg.fight_time_s + 5;
	}

	if (m_delay_kickout_time > 0 && (unsigned int)now_second >= m_delay_kickout_time)
	{
		this->DelayKickOutAllRole();
	}
	
	if (m_finish)
	{
		if (m_delay_kickout_time == 0)
		{
			m_delay_kickout_time = (unsigned int)now_second + 5;
		}
		return;
	}

	// 战斗开始由服务器计时、客户端OnReady请求触发
	unsigned int now = (unsigned int)now_second;
	if (m_fight_start_time > 0 && now >= m_fight_start_time)
	{
		this->OnFightStart();
	}

	if (m_fight_end_time > 0 && now >= m_fight_end_time)
	{
		this->OnFightTimeout();	
		m_delay_kickout_time = now + 5;
	}
}

void SpecialCross1v1::OnAttack(Character *cha, ObjID enemy_id, Attribute delta_hp)
{
	if (!m_fight_start)
	{
		return;
	}

	if (NULL == cha) return;

	if (m_fighter_list[1].is_robot)
	{
		Obj* obj = m_scene->GetObj(enemy_id);
		if (NULL == obj) return;
		if (Obj::OBJ_TYPE_ROLE == cha->GetObjType() && Obj::OBJ_TYPE_ROLE_SHADOW == obj->GetObjType())
		{
			RoleShadow *attack_role = m_scene->GetRoleShadow(enemy_id);
			if (NULL == attack_role) return;
			for (int i = 0; i < FIGHTER_COUNT; ++i)
			{
				if (m_fighter_list[i].role_userid == attack_role->GetUserId())
				{
					m_fighter_list[i].hurt += (-delta_hp);
				}
			}
		}
		else if (Obj::OBJ_TYPE_ROLE_SHADOW == cha->GetObjType() && Obj::OBJ_TYPE_ROLE == obj->GetObjType())
		{
			Role *attack_role = m_scene->GetPrincipal(enemy_id);
			if (NULL == attack_role) return;
			for (int i = 0; i < FIGHTER_COUNT; ++i)
			{
				if (m_fighter_list[i].role_userid == attack_role->GetUserId())
				{
					m_fighter_list[i].hurt += (-delta_hp);
				}
			}
		}
	}
	else
	{
		if (Obj::OBJ_TYPE_ROLE != cha->GetObjType()) return;

		Role *attack_role = m_scene->GetPrincipal(enemy_id);
		if (NULL == attack_role) return;

		for (int i = 0; i < FIGHTER_COUNT; ++i)
		{
			if (m_fighter_list[i].role_userid == attack_role->GetUserId())
			{
				m_fighter_list[i].hurt += (-delta_hp);
			}
		}
	}
}

void SpecialCross1v1::OnCharacterDie(Character *dead_cha, ObjID killer_objid)
{
	if (!m_fight_start || m_finish)
	{
		return;
	}

	if (NULL == dead_cha) return;

	//if (Obj::OBJ_TYPE_ROLE != dead_cha->GetObjType()) return;

	Obj *killer_obj = m_scene->GetObj(killer_objid);
	if (NULL == killer_obj) return;

	if (Obj::OBJ_TYPE_ROLE == dead_cha->GetObjType() && Obj::OBJ_TYPE_ROLE_SHADOW == killer_obj->GetObjType())
	{
		Role *dead_role = (Role *)dead_cha;
		this->OnFightResult("die", dead_role, false, NULL);
		this->SyncResultToCross(1);
	}
	else if (Obj::OBJ_TYPE_ROLE_SHADOW == dead_cha->GetObjType() && Obj::OBJ_TYPE_ROLE == killer_obj->GetObjType())
	{
		Role *winner = (Role *)killer_obj;

		this->OnFightResult("die", winner, true, NULL);
		this->SyncResultToCross(0);
	}
	else if (Obj::OBJ_TYPE_ROLE == dead_cha->GetObjType() && Obj::OBJ_TYPE_ROLE == killer_obj->GetObjType())
	{
		Role *dead_role = (Role *)dead_cha;
		Role *winner = NULL;

		for (int i = 0; i < FIGHTER_COUNT; ++i)
		{
			if (INVALID_USER_ID != m_fighter_list[i].role_userid &&
				m_fighter_list[i].role_userid != dead_role->GetUserId())
			{
				// 找到了胜利者
				winner = this->GetRole(i);
				if (NULL != winner)
				{
					this->OnFightResult("die", winner, true, dead_role);
				}

				this->SyncResultToCross(i);

				break;
			}
		}

		this->OnFightResult("die", dead_role, false, NULL);
	}
	
	m_finish = true;

	m_delay_kickout_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + 5;
}

void SpecialCross1v1::OnRoleEnterScene(Role *role)
{
	if (m_finish)	// 开始了不让进
	{
		return;
	}

	if (NULL == role) return;

	CrossActivity1v1FightUnit *match_unit = ActivityShadow::Instance().GetCross1v1FightMatchUnit(UniqueUserIDToLongLong(role->GetRoleCross()->GetCrossOriginalUniqueUserID()));
	if (NULL == match_unit)
	{
		role->NoticeNum(errornum::EN_1V1_NOT_MATCH);

		role->BackToOriginalServer();
		return;
	}
	
	if (1 == match_unit->is_with_robot1)
	{
		const Cross1V1OtherConfig &other_cfg = Cross1V1CommonConfig::Instance().GetOtherCfg();
	 	double role_shodow_add_per = other_cfg.role_shadow_add_attr_per / 100.0 + 1.0;
		RoleShadow *role_shadow = new RoleShadow(ROLE_SHADOW_TYPE_CHALLENGE_FIELD);

		ChallengeParam roleshadow_param;
		RoleAppearance appearance;

		roleshadow_param.plat_type = 0;
		roleshadow_param.role_id = 10010;
		SNPRINTF(roleshadow_param.role_name, sizeof(roleshadow_param.role_name), "%s", match_unit->user_name2);
		roleshadow_param.avatar = 0;
		roleshadow_param.sex = match_unit->sex2;
		roleshadow_param.prof = match_unit->prof2;
		roleshadow_param.camp = match_unit->prof2;
		roleshadow_param.move_speed = 1000;
		roleshadow_param.vip_level = 0;

		roleshadow_param.capability = static_cast<int>(match_unit->capability1);

		roleshadow_param.level = match_unit->level2;
	
		roleshadow_param.gongji = static_cast<Attribute>(role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI) * (match_unit->rand_per_attr / 100.0) * role_shodow_add_per);
		roleshadow_param.max_hp = static_cast<Attribute>(role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP) * (match_unit->rand_per_attr / 100.0) * role_shodow_add_per);
		roleshadow_param.fangyu = static_cast<Attribute>(role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_FANGYU) * (match_unit->rand_per_attr / 100.0) * role_shodow_add_per);
		roleshadow_param.mingzhong = static_cast<Attribute>(role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MINGZHONG) * (match_unit->rand_per_attr / 100.0) * role_shodow_add_per);
		roleshadow_param.shanbi = static_cast<Attribute>(role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_SHANBI) * (match_unit->rand_per_attr / 100.0) * role_shodow_add_per);
		roleshadow_param.baoji = static_cast<Attribute>(role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_BAOJI) * (match_unit->rand_per_attr / 100.0)* role_shodow_add_per);
		roleshadow_param.jianren = static_cast<Attribute>(role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_JIANREN) * (match_unit->rand_per_attr / 100.0) * role_shodow_add_per);

		roleshadow_param.fujiashanghai = static_cast<Attribute>(role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_FUJIA_SHANGHAI) * (match_unit->rand_per_attr / 100.0) * role_shodow_add_per);
		roleshadow_param.dikangshanghai = static_cast<Attribute>(role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_DIKANG_SHANGHAI) * (match_unit->rand_per_attr / 100.0) * role_shodow_add_per);


		roleshadow_param.baoji_per = static_cast<Attribute>(role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_PER_BAOJI_HURT) * (match_unit->rand_per_attr / 100.0) * role_shodow_add_per);
		roleshadow_param.kangbao_per = static_cast<Attribute>(role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_PER_KANGBAO) * (match_unit->rand_per_attr / 100.0) * role_shodow_add_per);
		roleshadow_param.pofang_per = static_cast<Attribute>(role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_PER_POFANG) * (match_unit->rand_per_attr / 100.0) * role_shodow_add_per);
		roleshadow_param.mianshang_per = static_cast<Attribute>(role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_PER_MIANSHANG) * (match_unit->rand_per_attr / 100.0) * role_shodow_add_per);

		roleshadow_param.guild_post = 0;
		roleshadow_param.guild_id = INVALID_GUILD_ID;
		memset(roleshadow_param.guild_name, 0, sizeof(roleshadow_param.guild_name));
		roleshadow_param.avatar_timestamp = 0;

		static ItemID item_id_list[] = { 0, 5110, 5210, 5310, 5410 };

		appearance.wuqi_id = 0;
		if (roleshadow_param.prof >= 0 && roleshadow_param.prof < static_cast<char>(sizeof(item_id_list) / sizeof(item_id_list[0])))
		{
			appearance.wuqi_id = item_id_list[(int)roleshadow_param.prof];
		}

		{
			// 个人技能信息
			roleshadow_param.skill_list[0].skill_id = roleshadow_param.prof * 10 + 101;
			roleshadow_param.skill_list[0].level = 1;

			roleshadow_param.skill_list[1].skill_id = roleshadow_param.prof * 10 + 102;
			roleshadow_param.skill_list[1].level = 1;

			roleshadow_param.skill_list[2].skill_id = roleshadow_param.prof * 10 + 103;
			roleshadow_param.skill_list[2].level = 1;

			roleshadow_param.skill_list[3].skill_id = roleshadow_param.prof * 10 + 104;
			roleshadow_param.skill_list[3].level = 1;
		}

		role_shadow->Init(roleshadow_param, appearance, 0, 0);
		role_shadow->SetPos(Cross1V1CommonConfig::Instance().GetPos2());
		m_scene->AddObj(role_shadow);

		m_fighter_list[0].role_obj_id = role->GetId();
		m_fighter_list[0].role_userid = role->GetUserId();
		m_fighter_list[0].start_hp = role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
		role->ResetPos(Cross1V1CommonConfig::Instance().GetPos1().x, Cross1V1CommonConfig::Instance().GetPos1().y);
		m_fighter_list[1].is_robot = true;
		m_fighter_list[1].role_obj_id = role_shadow->GetId();
		m_fighter_list[1].role_userid = role_shadow->GetUserId();
		m_fighter_list[1].start_hp = role_shadow->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
		m_fighter_list[1].ready = true;

		gamelog::g_log_debug.printf(LL_DEBUG, "role[%d,%s] enter 1v1[%d]",
			UidToInt(role->GetUserId()), role->GetName(), m_scene->GetSceneKey());
	}
	else
	{
		for (int i = 0; i < FIGHTER_COUNT; ++i)
		{
			if (m_fighter_list[i].role_userid == role->GetUserId())
			{
				m_fighter_list[i].role_obj_id = role->GetId();
				if (i == 0)
				{
					role->ResetPos(Cross1V1CommonConfig::Instance().GetPos1().x, Cross1V1CommonConfig::Instance().GetPos1().y);
				}
				else
				{
					role->ResetPos(Cross1V1CommonConfig::Instance().GetPos2().x, Cross1V1CommonConfig::Instance().GetPos2().y);
				}
				break;
			}

			if (INVALID_USER_ID == m_fighter_list[i].role_userid &&
				role->GetRoleCross()->GetCrossOriginalUniqueUserID() == LongLongToUniqueUserID(m_fighter_list[i].uuid))
			{
				m_fighter_list[i].role_obj_id = role->GetId();
				m_fighter_list[i].role_userid = role->GetUserId();
				m_fighter_list[i].start_hp = role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
				if (i == 0)
				{
					role->ResetPos(Cross1V1CommonConfig::Instance().GetPos1().x, Cross1V1CommonConfig::Instance().GetPos1().y);
				}
				else
				{
					role->ResetPos(Cross1V1CommonConfig::Instance().GetPos2().x, Cross1V1CommonConfig::Instance().GetPos2().y);
				}

				gamelog::g_log_debug.printf(LL_DEBUG, "role[%d,%s] enter 1v1[%d]",
					UidToInt(role->GetUserId()), role->GetName(), m_scene->GetSceneKey());
				break;
			}
		}
	}

	EventHandler::Instance().OnEnterCross1v1(role);
}

bool SpecialCross1v1::SpecialCanMountOn(Role *role)
{
	return false;
}

void SpecialCross1v1::OnRoleLeaveScene(Role *role, bool is_logout)
{
	if (NULL == role) return;

	if (role->IsAlive())
	{
		role->Recover();
	}
	else
	{
		role->ReAlive(100, 100);
	}

	// 把需要同步的个人信息同步回原服
	{
		if (is_logout)
		{
			long long uuid = UniqueUserIDToLongLong(role->GetRoleCross()->GetCrossOriginalUniqueUserID());
			SyncUserInfoOnBack *info = ActivityShadow::Instance().GetCrossUserInfoOnBack(uuid);

			if (NULL != info && info->cost_gold_in_cross > 0)
			{
				int use_gold = info->cost_gold_in_cross;
				info->cost_gold_in_cross = 0;

				static crossgameprotocal::GameCrossCommonInfoSync cis;
				cis.uuid = uuid;
				cis.has_use_gold = use_gold;
				InternalComm::Instance().SendToCross((const char*)&cis, sizeof(cis));
			}

			ActivityShadow::Instance().DeleteCrossUserInfoOnBack(uuid); // 用完即移除
		}

		long long uuid = UniqueUserIDToLongLong(role->GetRoleCross()->GetCrossOriginalUniqueUserID());
		static crossgameprotocal::GameCrossCommonAttendInfoSync cais;
		cais.uuid = uuid;
		cais.attend_activity_num = CROSS_ACTIVITY_TYPE_CROSS_1V1;
		InternalComm::Instance().SendToCross((const char*)&cais, sizeof(cais));
	}

	{
		Protocol::SCReturnOriginalServer ros;
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&ros, sizeof(ros));
	}

	if (!m_fight_start || m_finish)
	{
		return;
	}

	Role *winner = NULL;
	bool is_syc_result = false;
	for (int i = 0; i < FIGHTER_COUNT; ++i)
	{
		if (INVALID_USER_ID != m_fighter_list[i].role_userid && 
			m_fighter_list[i].role_userid != role->GetUserId())
		{
			// 找到了胜利者
			winner = this->GetRole(i);
			if (NULL != winner)
			{
				this->OnFightResult("leave", winner, true, role);
			}

			this->SyncResultToCross(i);
			is_syc_result = true;

			break;
		}
	}

	this->OnFightResult("leave", role, false, winner);
	if (!is_syc_result)
	{
		this->SyncResultToCross(-1);
	}

	m_finish = true;
}

bool SpecialCross1v1::SpecialCanPerformSkill(Role *role)
{
	//开始战斗,且战斗结束前才可以释放技能
	if (m_fight_start && !m_finish)
	{
		return true;
	}

	return false;
}

Role * SpecialCross1v1::GetRole(int index)
{
	if (index < 0 || index >= FIGHTER_COUNT)
	{
		return NULL;
	}

	Obj *obj = m_scene->GetObj(m_fighter_list[index].role_obj_id);
	if (NULL != obj && Obj::OBJ_TYPE_ROLE == obj->GetObjType())
	{
		Role *role = (Role*)obj;
		if (m_fighter_list[index].role_userid == role->GetUserId())
		{
			return role;
		}
	}

	return NULL;
}

void SpecialCross1v1::OnFightResult(const char *reason, Role *role, bool is_win, Role *op_role)
{
	if (NULL == role) return;

	// 清除匹配信息
	{
		ActivityShadow::Instance().ClearRoleMatchInfo(UniqueUserIDToLongLong(role->GetRoleCross()->GetCrossOriginalUniqueUserID()));
		if (nullptr != op_role)
		{
			ActivityShadow::Instance().ClearRoleMatchInfo(UniqueUserIDToLongLong(op_role->GetRoleCross()->GetCrossOriginalUniqueUserID()));
		}
	}
}

void SpecialCross1v1::SyncResultToCross(int winner_index)
{
	crossgameprotocal::GameCrossCross1V1FightResult gccfr;
	gccfr.uuid1 = m_fighter_list[0].uuid;
	gccfr.uuid2 = m_fighter_list[1].uuid;

	if (0 == winner_index)
	{
		gccfr.user1_is_win = 1; gccfr.user2_is_win = 0;
	}
	else if (1 == winner_index)
	{
		gccfr.user1_is_win = 0; gccfr.user2_is_win = 1;
	}
	else
	{
		gccfr.user1_is_win = 0; gccfr.user2_is_win = 0;
	}

	InternalComm::Instance().SendToCross((const char*)&gccfr, sizeof(gccfr));
}

void SpecialCross1v1::OnFightTimeout()
{
	int winner_index = -1;
	int loser_index1 = -1, loser_index2 = -1;

	if (INVALID_USER_ID != m_fighter_list[0].role_userid && INVALID_USER_ID != m_fighter_list[1].role_userid)
	{
		if (m_fighter_list[0].hurt > m_fighter_list[1].hurt)
		{
			winner_index = 0;
			loser_index1 = 1;
		}
		else if (m_fighter_list[1].hurt > m_fighter_list[0].hurt)
		{
			winner_index = 1;
			loser_index1 = 0;
		}
		else
		{
			loser_index1 = 0;
			loser_index2 = 1;
		}
	}
	else if (INVALID_USER_ID != m_fighter_list[0].role_userid && INVALID_USER_ID == m_fighter_list[1].role_userid)
	{
		winner_index = 0;
	}
	else if (INVALID_USER_ID == m_fighter_list[0].role_userid && INVALID_USER_ID != m_fighter_list[1].role_userid)
	{
		winner_index = 1;
	}
	else
	{
		loser_index1 = 0;
		loser_index2 = 1;
	}

	Role *winner = this->GetRole(winner_index);
	Role *loser1 = this->GetRole(loser_index1);
	Role *loser2 = this->GetRole(loser_index2);

	if (NULL != winner)
	{
		this->OnFightResult("fighttimeout", winner, true, (NULL != loser1 ? loser1 : loser2));
	}

	if (NULL != loser1)
	{
		this->OnFightResult("fighttimeout", loser1, false, (NULL != winner ? winner : loser2));
	}

	if (NULL != loser2)
	{
		this->OnFightResult("fighttimeout", loser2, false, (NULL != winner ? winner : loser1));
	}

	this->SyncResultToCross(winner_index);

	m_finish = true;
}

void SpecialCross1v1::OnFightStart()
{
	if (m_fighter_list[1].is_robot)
	{
		if (INVALID_USER_ID != m_fighter_list[0].role_userid && NULL != this->GetRole(0) && INVALID_USER_ID != m_fighter_list[1].role_userid)
		{
			Role *role = this->GetRole(0);

			Protocol::SCCross1v1FightStart fs;
			fs.timestamp_type = Protocol::CROSS1V1_FIGHT_TIME_TYPE_START;
			const Cross1V1OtherConfig &other_cfg = Cross1V1CommonConfig::Instance().GetOtherCfg();
			m_fight_end_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + other_cfg.fight_time_s;

			fs.fight_start_timestmap = static_cast<unsigned int>(EngineAdapter::Instance().Time());
			fs.fight_end_timestmap = m_fight_end_time;			EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&fs, sizeof(fs));

			Obj *obj = m_scene->GetObj(m_fighter_list[1].role_obj_id);
			if (nullptr != obj && Obj::OBJ_TYPE_ROLE_SHADOW == obj->GetObjType())
			{
				RoleShadow *rs = (RoleShadow*)obj;
				rs->ForceSetEnemy(m_fighter_list[0].role_obj_id);

				if (ROLE_SHADOW_TYPE_CHALLENGE_FIELD == rs->GetRoleAI()->GetAIType())
				{
					RoleAIChallengefile *ai = (RoleAIChallengefile *)rs->GetRoleAI();
					ai->SetWaitMs(other_cfg.role_shadow_alay_ms);
				}
			}

			m_fight_start = true;
			m_fight_start_time = 0;
		}
	}
	else
	{
		if (INVALID_USER_ID != m_fighter_list[0].role_userid && INVALID_USER_ID != m_fighter_list[1].role_userid &&
			NULL != this->GetRole(0) && NULL != this->GetRole(1))
		{
			// 两个人都到位了，战斗开始
			Protocol::SCCross1v1FightStart fs;
			fs.timestamp_type = Protocol::CROSS1V1_FIGHT_TIME_TYPE_START;
			const Cross1V1OtherConfig &other_cfg = Cross1V1CommonConfig::Instance().GetOtherCfg();
			m_fight_end_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + other_cfg.fight_time_s;

			fs.fight_start_timestmap = static_cast<unsigned int>(EngineAdapter::Instance().Time());
			fs.fight_end_timestmap = m_fight_end_time;
			m_scene->SendToRole((const char *)&fs, sizeof(fs));

			m_fight_start = true;
			m_fight_start_time = 0;
		}
		else
		{
			// 开战时间到，只有一个人，这个人直接胜利
			int winner_index = -1;

			if ((INVALID_USER_ID != m_fighter_list[0].role_userid && INVALID_USER_ID == m_fighter_list[1].role_userid) ||
				(NULL != this->GetRole(0) && NULL == this->GetRole(1)))
			{
				winner_index = 0;
			}
			else if ((INVALID_USER_ID == m_fighter_list[0].role_userid && INVALID_USER_ID != m_fighter_list[1].role_userid) ||
				(NULL == this->GetRole(0) && NULL != this->GetRole(1)))
			{
				winner_index = 1;
			}

			Role *winner = this->GetRole(winner_index);
			if (NULL != winner)
			{
				this->OnFightResult("startnobody", winner, true, NULL);
			}

			this->SyncResultToCross(winner_index);

			m_finish = true;
		}
	}
}

bool SpecialCross1v1::SpecialIsEnemy(Role *role, Obj *obj)
{
	if (!m_fight_start)
	{
		return false;
	}

	if (NULL == role || NULL == obj)
	{
		return false;
	}

	if (role->GetId() == obj->GetId())
	{
		return false;
	}

	if (Obj::IsCharactor(obj->GetObjType()))
	{
		Character *chara = (Character*)obj;
		return chara->IsAlive();
	}

	return false;
}

void SpecialCross1v1::OnReady(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	if (m_fight_start || m_finish)
	{
		return;
	}

	for (int i = 0; i < FIGHTER_COUNT; ++i)
	{
		if (m_fighter_list[i].role_userid == role->GetUserId() || m_fighter_list[i].is_robot)
		{
			m_fighter_list[i].ready = true;

			break;
		}
	}

	bool all_ready = true;
	for (int i = 0; i < FIGHTER_COUNT; ++i)
	{
		if (!m_fighter_list[i].ready)
		{
			all_ready = false;

			// 第一个人进入场景时，设置战斗等待时间
			const Cross1V1OtherConfig &other_cfg = Cross1V1CommonConfig::Instance().GetOtherCfg();
			m_fight_start_time = (unsigned int)(EngineAdapter::Instance().Time()) + other_cfg.fight_wait_s;

			break;
		}
	}

	if (all_ready)
	{
		m_fight_start_time = (unsigned int)(EngineAdapter::Instance().Time());
	}
}

void SpecialCross1v1::OnSetFighterInfo(long long uuid1, long long uuid2)
{
	UNSTD_STATIC_CHECK(FIGHTER_COUNT >= 2);
	
	m_fighter_list[0].uuid = uuid1;
	m_fighter_list[1].uuid = uuid2;
}

bool SpecialCross1v1::SpecialCanSupplyEffect(Role *role, int supply_type)
{
	if (SUPPLY_TYPE_ROLE_HP == supply_type)
	{
		return false;
	}

	return true;
}

bool SpecialCross1v1::CanDestroy()
{
	if (m_fight_start || m_finish)
	{
		return true;
	}

	return false;
}

long long SpecialCross1v1::GetSpecialParam1(Role *role)
{
	if (NULL != role)
	{
		return role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MP);
	}

	return 0;
}


