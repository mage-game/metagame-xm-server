#include "specialcrossmultiuserchallenge.hpp"

#include "obj/character/role.h"
#include "obj/otherobj/gatherobj.h"
#include "obj/otherobj/effectobj.h"

#include "engineadapter.h"

#include "internalcomm.h"

#include "protocal/msgactivity.hpp"
#include "protocal/msgfight.h"

#include "servercommon/crossdef.hpp"

#include "effect/effectbase.hpp"
#include "effect/skilleffect/effectspecialstate.hpp"
#include "effect/skilleffect/effectonce.hpp"

#include "servercommon/serverconfig/cross3v3config.hpp"
#include "servercommon/userprotocal/msgcrosscommon.h"
#include "servercommon/errornum.h"
#include "other/cross/rolecross.h"
#include "gameworld/item/knapsack.h"
#include "scene/activityshadow/activityshadow.hpp"
#include "other/event/eventhandler.hpp"

SpecialCrossMultiuserChallenge::SpecialCrossMultiuserChallenge(Scene *scene)
	: SpecialLogic(scene), m_state(CROSS_MULTIUSER_CHALLENGE_MATCH_STATE_READY), m_next_state_time(0), m_delay_kick_time(0), m_return_origin_server_time(0), m_win_side(-1), 
	m_stronghold_owner_side(CROSS_MULTIUSER_CHALLENGE_SIDE_MAX),m_stronghold_rate_info(50), m_last_stronghold_add_score_time(0),
	m_first_enter(0), m_wait_ready_timeout(0)
{
	memset(m_side_score_list, 0, sizeof(m_side_score_list));

	//m_next_state_time = EngineAdapter::Instance().Time() + Cross3v3Config::Instance().GetReadyTime();
	m_wait_ready_timeout = EngineAdapter::Instance().Time() + Cross3v3Config::Instance().GetReadyTime() + 15;
}

SpecialCrossMultiuserChallenge::~SpecialCrossMultiuserChallenge()
{

}

bool SpecialCrossMultiuserChallenge::CanDestroy()
{
	return this->IsFinish();
}

bool SpecialCrossMultiuserChallenge::SpecialCanReAlive(Role *role, int realive_type)
{
	if (NULL == role)
	{
		return false;
	}

	return true;
//	int real_inverval_ms = CROSS_MULTIUSER_CHALLENGE_RELIVE_INTERVAL_MS;
//
//	return (Protocol::CSRoleReAliveReq::REALIVE_TYPE_HERE_ALL_GOLD == realive_type ||
//		(Protocol::CSRoleReAliveReq::REALIVE_TYPE_BACK_HOME == realive_type && role->GetDeadTime() >= real_inverval_ms));
}

bool SpecialCrossMultiuserChallenge::SpecialRelive(Role *role, int realive_type)
{
	if (NULL == role)
	{
		return false;
	}

	Posi realive_pos = Cross3v3Config::Instance().GetRelivePos(this->GetRoleSide(role->GetRoleCross()->GetCrossOriginalUniqueUserID()));

	if (Protocol::CSRoleReAliveReq::REALIVE_TYPE_HERE_GOLD == realive_type)
	{
		return false;
// 		int relive_need_gold = 0;  GLOBALCONFIG->GetRealiveCostGold();
// 
// 		if (!role->GetKnapsack()->GetMoney()->AllGoldMoreThan(relive_need_gold))
// 		{
// 			role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
// 			return true;
// 		}
// 
// 		if (!role->GetKnapsack()->GetMoney()->UseAllGold(relive_need_gold, "SpecialCrossMultiuserChallenge::SpecialRelive", true))
// 		{
// 			return true;
// 		}
// 
// 		role->ReAlive(100, 100);
	}
	else // 其他方式复活需要等CD时间
	{
		role->ReAlive(100, 100, realive_pos);
	}

	return true;
}

bool SpecialCrossMultiuserChallenge::SpecialIsEnemy(Role *role, Obj *obj)
{
	if (!this->IsFighting()) return false;

	return this->IsMatchTwoSides(role, obj);
}

bool SpecialCrossMultiuserChallenge::SpecialIsFriend(Role *role, Obj *obj, bool check_alive)
{
	if (!this->IsFighting()) return false;
	if (NULL == role || NULL == obj)
	{
		return false;
	}

	return (!this->IsMatchTwoSides(role, obj) && (!check_alive || role->IsAlive()));
}

int SpecialCrossMultiuserChallenge::GetSpecialParam(Role *role)
{
	if (NULL == role)
	{
		return -1;
	}
	const UniqueUserID & uu_id= role->GetRoleCross()->GetCrossOriginalUniqueUserID();

	return GetRoleSide(uu_id);
}

bool SpecialCrossMultiuserChallenge::IsMatchTwoSides(Obj *obj1, Obj *obj2, bool is_check_alive)
{
	if (NULL == obj1 || NULL == obj2) return false;
	if (obj1->GetId() == obj2->GetId()) return false;

	if (Obj::IsCharactor(obj1->GetObjType()) && Obj::IsCharactor(obj2->GetObjType()))
	{
		Character *chr1 = (Character *)obj1;
		Character *chr2 = (Character *)obj2;

		if (!is_check_alive || (chr1->IsAlive() && chr2->IsAlive()))
		{
			int side1 = -1;
			if (Obj::OBJ_TYPE_ROLE == chr1->GetObjType())
			{
				side1 = this->GetObjSide(chr1->GetId());
			}
				
			if (side1 >= 0 && side1 < CROSS_MULTIUSER_CHALLENGE_SIDE_MAX)
			{
				int side2 = -1;
				if (Obj::OBJ_TYPE_ROLE == chr2->GetObjType())
				{
					side2 = this->GetObjSide(chr2->GetId());
				}

				if (side2 >= 0 && side2 < CROSS_MULTIUSER_CHALLENGE_SIDE_MAX)
				{
					return (side1 != side2);
				}
			}
		}
	}

	return false;
}

bool SpecialCrossMultiuserChallenge::SpecialCanMountOn(Role *role)
{
	return false;
}

long long SpecialCrossMultiuserChallenge::GetSpecialParam1(Role *role)
{
	if (NULL == role)
	{
		return -1;
	}

	return this->GetRoleSide(role->GetRoleCross()->GetCrossOriginalUniqueUserID());
}

void SpecialCrossMultiuserChallenge::Update(unsigned long interval, time_t now_second)
{
	SpecialLogic::Update(interval, now_second);

	if (this->IsReady())
	{
		if (0 != m_wait_ready_timeout && now_second >= m_wait_ready_timeout && 0 == m_next_state_time)
		{
			m_wait_ready_timeout = 0;
			this->ForceNextState();
		}
	}

	if (!this->IsFinish())
	{
		if (0 != m_next_state_time && now_second >= m_next_state_time)
		{
			++ m_state;
			m_next_state_time = 0;

			this->OnChangeToNextState();
		}
	}
	else
	{
		if (0 != m_return_origin_server_time && now_second >= m_return_origin_server_time)
		{
			m_return_origin_server_time = 0;

			static Protocol::SCReturnOriginalServer ros;
			m_scene->SendToRole((const char*)&ros, sizeof(ros));
		}

		if (0 != m_delay_kick_time && now_second >= m_delay_kick_time)
		{
			m_delay_kick_time = 0;

			this->DelayKickOutAllRole();
		}
	}

	// 检查占据据点加分
	if (this->IsFighting() && (m_last_stronghold_add_score_time + CROSS_MULTIUSER_CHALLENGE_STRONGHOLD_ADD_SCORE_INTERVAL <= now_second))
	{
		this->CheckStrongholdRateInfo();
		m_last_stronghold_add_score_time = now_second;
	}
}

void SpecialCrossMultiuserChallenge::OnRoleEnterScene(Role *role)
{
	if (this->IsFinish() || NULL == role) return;

	CrossMultiUserChallengeUserInfo *user_info = this->GetUserInfo(role);
	if (NULL != user_info)
	{
		user_info->obj_id = role->GetId();

		if (0 == m_first_enter)
		{
			m_next_state_time = EngineAdapter::Instance().Time() + Cross3v3Config::Instance().GetReadyTime();
			m_first_enter = 1;
			m_wait_ready_timeout = 0;
		}

		this->RefreshMultiuserChallengeSelfInfo(role);
		this->RefreshMultiuserChallengeMatchInfo(role);
		this->RefreshMultiuserChallengeMatchState();
	}

	EventHandler::Instance().OnEnterCross3v3(role);
}

void SpecialCrossMultiuserChallenge::OnRoleLeaveScene(Role *role, bool is_logout)
{
	if (NULL == role) return;

	{
		// 把需要同步的个人信息同步回原服
		if (is_logout)
		{
			long long uuid = UniqueUserIDToLongLong(role->GetRoleCross()->GetCrossOriginalUniqueUserID());
			SyncUserInfoOnBack *info = ActivityShadow::Instance().GetCrossUserInfoOnBack(uuid);

			if (NULL != info && info->cost_gold_in_cross > 0)
			{
				int use_gold = info->cost_gold_in_cross;
				info->cost_gold_in_cross = 0;

				// 同步消耗的元宝信息
				static crossgameprotocal::GameCrossCommonInfoSync cis;
				cis.uuid = uuid;
				cis.has_use_gold = use_gold;
				InternalComm::Instance().SendToCross((const char*)&cis, sizeof(cis));
			}

			// 同步活动参与信息
			static crossgameprotocal::GameCrossCommonAttendInfoSync cais;
			cais.uuid = uuid;
			cais.attend_activity_num = CROSS_ACTIVITY_TYPE_CROSS_MULTIUSER_CHALLENGE;
			InternalComm::Instance().SendToCross((const char*)&cais, sizeof(cais));

			ActivityShadow::Instance().DeleteCrossUserInfoOnBack(uuid); // 用完即移除
		}
	}

	{
		// 设置下线标记
		if (is_logout && (this->IsReady() || this->IsFighting()))
		{
			role->GetRoleCross()->SetCross3v3LogoutFlag(1); // 掉线
		}
		else
		{
			role->GetRoleCross()->SetCross3v3LogoutFlag(2); // 主动退出
		}
	}

	if (role->IsAlive())
	{
		role->Recover();
	}
	else
	{
		role->ReAlive(100, 100);
	}

	CrossMultiUserChallengeUserInfo *user_info = this->GetUserInfo(role);
	if (NULL != user_info)
	{
		user_info->obj_id = INVALID_OBJ_ID;
	}

	role->ClearFightBuffEffect(EffectBase::CLEAR_REASON_LEAVE_SCENE);
	this->RefreshMultiuserChallengeMatchState();

	// 通知客户端回原服
	role->BackToOriginalServer();
}

void SpecialCrossMultiuserChallenge::OnAttack(Character *cha, ObjID enemy_id, Attribute delta_hp)
{
	if (NULL == cha) return;

	if (Obj::OBJ_TYPE_ROLE == cha->GetObjType())
	{
		Role *role = (Role*)cha;
		Obj *enemy_obj = m_scene->GetObj(enemy_id);
		if (NULL != enemy_obj && Obj::OBJ_TYPE_ROLE == enemy_obj->GetObjType())
		{
			Role *enemy_role = (Role*)enemy_obj;

			CrossMultiUserChallengeUserInfo *user_info = this->GetUserInfo(role);
			if (NULL != user_info)
			{
				unsigned int now = static_cast<unsigned int>(EngineAdapter::Instance().Time());

				CrossMultiUserChallengeUserInfo::HurtMapIt it = user_info->be_hurt_user_map.find(enemy_role->GetRoleCross()->GetCrossOriginalUniqueUserID());
				if (it != user_info->be_hurt_user_map.end())
				{
					it->second.obj_id = enemy_role->GetId();
					it->second.last_hurt_time = now;
				}
				else
				{
					user_info->be_hurt_user_map[enemy_role->GetRoleCross()->GetCrossOriginalUniqueUserID()] = ChallengeHurtInfo(enemy_role->GetId(), now);
				}
			}
		}
	}
}

void SpecialCrossMultiuserChallenge::OnCharacterDie(Character *dead, ObjID killer_id)
{
	if (NULL == dead) return;

	UniqueUserID killer_unique_user_id;

	Role *kill_role = m_scene->GetPrincipal(killer_id);
	if (NULL != kill_role && Obj::OBJ_TYPE_ROLE == dead->GetObjType())
	{
		killer_unique_user_id = kill_role->GetRoleCross()->GetCrossOriginalUniqueUserID();

		{
			CrossMultiUserChallengeUserInfo *kill_user_info = this->GetUserInfo(kill_role);
			if (NULL != kill_user_info)
			{
				++ kill_user_info->kills;

				int KILL_ADD_SCORE = Cross3v3Config::Instance().GetKillAddScore();

				int side = this->GetRoleSide(kill_role->GetRoleCross()->GetCrossOriginalUniqueUserID());
				if (0 <= side && side < CROSS_MULTIUSER_CHALLENGE_SIDE_MAX)
				{
					this->AddSideScore(side, KILL_ADD_SCORE);
				}
			}

			this->RefreshMultiuserChallengeSelfInfo(kill_role);
			this->RefreshMultiuserChallengeMatchInfo();
		}
	}

	if (Obj::OBJ_TYPE_ROLE == dead->GetObjType())
	{
		Role *dead_role = (Role*)dead;

		CrossMultiUserChallengeUserInfo *dead_user_info = this->GetUserInfo(dead_role);
		if (NULL != dead_user_info)
		{
			unsigned int now = static_cast<unsigned int>(EngineAdapter::Instance().Time());

			++ dead_user_info->dead;

			for (CrossMultiUserChallengeUserInfo::HurtMapIt it = dead_user_info->be_hurt_user_map.begin(), end = dead_user_info->be_hurt_user_map.end(); it != end; ++ it)
			{
				if (it->first != killer_unique_user_id && it->second.last_hurt_time + CROSS_MULTIUSER_CHALLENGE_MATCH_ASSIST_TIME_LIMIT >= now)
				{
					CrossMultiUserChallengeUserInfo *assist_user_info = this->GetUserInfo(it->first);
					if (NULL != assist_user_info)
					{
						++ assist_user_info->assist;

						Obj *assist_obj = m_scene->GetObj(it->second.obj_id);
						if (NULL != assist_obj && Obj::OBJ_TYPE_ROLE == assist_obj->GetObjType())
						{
							Role *assist_role = (Role*)assist_obj;
							this->RefreshMultiuserChallengeSelfInfo(assist_role);
						}
					}
				}
			}

			dead_user_info->be_hurt_user_map.clear();
		}

		this->RefreshMultiuserChallengeSelfInfo(dead_role);
	}
}

void SpecialCrossMultiuserChallenge::OnMatchStart(crossgameprotocal::CrossGameCrossMultiuserChallengeStartMatch *cgcmcsm)
{
	for (int i = 0; i < CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT * CROSS_MULTIUSER_CHALLENGE_SIDE_MAX; i++)
	{
		m_side_user_list[i].obj_id = INVALID_OBJ_ID;
		m_side_user_list[i].unique_user_id = LongLongToUniqueUserID(cgcmcsm->user_info_list[i].uuid);
		F_STRNCPY(m_side_user_list[i].user_name, cgcmcsm->user_info_list[i].user_name, sizeof(m_side_user_list[i].user_name));
		m_side_user_list[i].level = cgcmcsm->user_info_list[i].level;
		m_side_user_list[i].prof = cgcmcsm->user_info_list[i].prof;
		m_side_user_list[i].sex = cgcmcsm->user_info_list[i].sex;
		m_side_user_list[i].challenge_score = cgcmcsm->user_info_list[i].challenge_score;
		m_side_user_list[i].challenge_exp = cgcmcsm->user_info_list[i].challenge_exp;
		m_side_user_list[i].challenge_level = cgcmcsm->user_info_list[i].challenge_level;
		m_side_user_list[i].match_total_count = cgcmcsm->user_info_list[i].match_total_count;
		m_side_user_list[i].match_win_count = cgcmcsm->user_info_list[i].match_win_count;
		m_side_user_list[i].mvp_count = cgcmcsm->user_info_list[i].mvp_count;
	}

	m_stronghold_owner_side = CROSS_MULTIUSER_CHALLENGE_SIDE_MAX;
	m_stronghold_rate_info = 50;
}

void SpecialCrossMultiuserChallenge::ForceNextState()
{
	if (!this->IsFinish())
	{
		m_next_state_time = EngineAdapter::Instance().Time();
	}
}

void SpecialCrossMultiuserChallenge::SendSideMemberPosToRole(Role * role)
{
	if (NULL == role) return;

	int side = this->GetRoleSide(role->GetRoleCross()->GetCrossOriginalUniqueUserID());
	if (side < 0 || side >= CROSS_MULTIUSER_CHALLENGE_SIDE_MAX) return;

	Protocol::SCMultiuserChallengeTeamMemberPosList mctmpl;
	mctmpl.member_count = 0;

	for (int i = 0; i < CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT && mctmpl.member_count < CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT; ++ i)
	{
		int locate_idx = side * CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT + i;
		ObjID obj_id = m_side_user_list[locate_idx].obj_id;

		Obj *obj = m_scene->GetObj(obj_id);
		if (NULL != obj && Obj::OBJ_TYPE_ROLE == obj->GetObjType())
		{
			Role *role = (Role*)obj;
			if (NULL != role) 
			{
				Posi pos = role->GetPos();
				mctmpl.member_list[mctmpl.member_count].dir = role->GetDir();
				mctmpl.member_list[mctmpl.member_count].role_id = UidToInt(role->GetUserId());
				mctmpl.member_list[mctmpl.member_count].obj_id = role->GetId();
				mctmpl.member_list[mctmpl.member_count].reserved = 0;
				mctmpl.member_list[mctmpl.member_count].is_leave_scene = 0;
				mctmpl.member_list[mctmpl.member_count].pos_x = pos.x;
				mctmpl.member_list[mctmpl.member_count].pos_y = pos.y;
				mctmpl.member_list[mctmpl.member_count].dir = role->GetDir();
				mctmpl.member_list[mctmpl.member_count].distance = role->GetDirDistance();
				mctmpl.member_list[mctmpl.member_count].move_speed = (int)role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MOVE_SPEED);
				++ mctmpl.member_count;
			}
		}
	}

	int len = sizeof(mctmpl) - sizeof(mctmpl.member_list[0]) * (CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT - mctmpl.member_count);
	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&mctmpl, len);
}

void SpecialCrossMultiuserChallenge::OnChangeToNextState()
{
	switch (m_state)
	{
	case CROSS_MULTIUSER_CHALLENGE_MATCH_STATE_FIGHTING:
		{
			m_next_state_time = EngineAdapter::Instance().Time() + Cross3v3Config::Instance().GetFightingTime();
			this->RefreshMultiuserChallengeMatchState();

		}
		break;

	case CROSS_MULTIUSER_CHALLENGE_MATCH_STATE_FINISH:
		{
			int win_side = -1;
			if (m_side_score_list[CROSS_MULTIUSER_CHALLENGE_SIDE_0] != m_side_score_list[CROSS_MULTIUSER_CHALLENGE_SIDE_1])
			{
				win_side = (m_side_score_list[CROSS_MULTIUSER_CHALLENGE_SIDE_0] > m_side_score_list[CROSS_MULTIUSER_CHALLENGE_SIDE_1] ? CROSS_MULTIUSER_CHALLENGE_SIDE_0 : CROSS_MULTIUSER_CHALLENGE_SIDE_1);
			}

			this->OnFinish(win_side);
			this->RefreshMultiuserChallengeMatchState();
		}
		break;
	}
}

void SpecialCrossMultiuserChallenge::OnFinish(int win_side)
{
	if (-1 != win_side && (win_side < 0 || win_side >= CROSS_MULTIUSER_CHALLENGE_SIDE_MAX)) return;		// -1表示平局

	m_state = CROSS_MULTIUSER_CHALLENGE_MATCH_STATE_FINISH;
	m_next_state_time = 0;
	m_return_origin_server_time = EngineAdapter::Instance().Time() + CROSS_MULTIUSER_CHALLENGE_MATCH_DELAY_KICK_TIME_S;
	m_delay_kick_time = m_return_origin_server_time + 5;
	m_win_side = win_side;

	static crossgameprotocal::GameCrossCrossMultiuserChallengeMatchResult gccmcmr;
	gccmcmr.match_key = m_scene->GetSceneKey();
	gccmcmr.win_side = win_side;

	// 按双方的平均积分值获取段位
	/*int side_average_division[CROSS_MULTIUSER_CHALLENGE_SIDE_MAX] = {0};
	for (int i = 0; i < CROSS_MULTIUSER_CHALLENGE_SIDE_MAX; ++ i)
	{
		int average_score = this->CalSideAverageScore(i);
		const Cross3V3GradeScoreConfig *grade_cfg = Cross3v3Config::Instance().GetDivisionConfigWithScore(average_score);
		if (NULL != grade_cfg)
		{
			side_average_division[i] = grade_cfg->grade;
		}
	}*/

	float max_mvp_value = -1000000.0f;
	for (int i = 0; i < CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT * CROSS_MULTIUSER_CHALLENGE_SIDE_MAX; i++)
	{
		CrossMultiUserChallengeUserInfo *user_info = &m_side_user_list[i];
		{
			int side = i / CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT;
			if (side < 0 || side >= CROSS_MULTIUSER_CHALLENGE_SIDE_MAX) continue;

			int opponent_side = this->GetOpponentSide(side);
			if (opponent_side < 0 || opponent_side >= CROSS_MULTIUSER_CHALLENGE_SIDE_MAX) continue;

			if (side == win_side)
			{
				// 3v3 MVP计算公式
				float mvp_value = user_info->kills * 0.5f + user_info->assist * 0.2f - user_info->dead * 0.3f + user_info->occupy;
				
				CrossMultiUserChallengeUserInfo *mvp_user_info = this->GetUserInfo(m_mvp_uuid);
				if (NULL == mvp_user_info || mvp_value > max_mvp_value || (mvp_value == max_mvp_value && user_info->kills > mvp_user_info->kills))
				{
					m_mvp_uuid = user_info->unique_user_id;
					max_mvp_value = mvp_value;
				}

				++user_info->match_win_count;
				++user_info->dur_win_count;
			}
			else
			{
				user_info->dur_win_count = 0; // 失败一次就把连胜清理了
			}

			++ user_info->match_total_count;

			const Cross3v3OtherConfig &other_cfg = Cross3v3Config::Instance().GetOtherCfg();

			// 自己队伍跟对方队伍的段位差，大于0则表示自己额外扣分，反之加分
			//int delta_division = side_average_division[side] - side_average_division[opponent_side];
			//int extra_score = Cross3v3Config::Instance().GetDivisionDiffExtraScore(delta_division);
			//int add_score = extra_score; // 段位差积分
			//add_score += Cross3v3Config::Instance().GetDurWinExtraScore(user_info->dur_win_count); // 连胜积分
			// 修改积分计算公式
			int add_score = 0;

			int add_shengwang;
			int add_gongxun;
			//double reward_exp_factor;

			const Cross3V3GradeScoreConfig *division_cfg = Cross3v3Config::Instance().GetDivisionConfigWithScore(user_info->challenge_score);
			if (NULL == division_cfg) continue;

			if (INVALID_OBJ_ID == user_info->obj_id) // 结算时不在线，当作中途退出处理，无论在哪一方，都按失败处理
			{
				add_score += division_cfg->fail_score;
				add_shengwang = other_cfg.fail_shengwang;
				//reward_exp_factor = other_cfg.fail_exp_per / 100.0f;
				add_gongxun = other_cfg.fail_gongxun;
			}
			else
			{
				if (-1 == win_side) // 平局
				{
					add_score += division_cfg->equal_score;
					add_shengwang = other_cfg.equal_shengwang;
					add_gongxun = other_cfg.equal_gongxun;
					//reward_exp_factor = other_cfg.equal_exp_per / 100.0f;
				}
				else if (side == win_side) // 胜利
				{
					//add_score += division_cfg->win_score;
					// 修改积分公式 单场获得积分 = 本段位胜利积分 * （MAX（连胜场次 - 2，0） * 0.5 + 1）
					add_score += (int)(division_cfg->win_score * (((std::max)(user_info->dur_win_count - 2, 0)) * 0.5f + 1));

					if (add_score < 0) add_score = 0; // 胜利不允许减分
					add_shengwang = other_cfg.win_shengwang;
					add_gongxun = other_cfg.win_gongxun;
					//reward_exp_factor = other_cfg.win_exp_per / 100.0f;
				}
				else // 失败
				{
					add_score -= division_cfg->fail_score;
					if (add_score > 0) add_score = 0; // 失败不允许加分
					add_shengwang = other_cfg.fail_shengwang;
					add_gongxun = other_cfg.fail_gongxun;
					//reward_exp_factor = other_cfg.fail_exp_per / 100.0f;
				}
			}
			
			if (user_info->challenge_score + add_score < 0) add_score = -user_info->challenge_score; // 最终积分不要小于0
			user_info->challenge_score += add_score; user_info->add_challenge_score = add_score;

			user_info->add_honor = add_shengwang;
			user_info->add_gongxun = add_gongxun;

			Role* role = m_scene->GetRoleByUUID(user_info->unique_user_id);
			if (NULL != role) // 尽量都传回原服再给角色，防止结算是角色不在线，给不了奖励
			{
				//const long long exp_dan_exp = LOGIC_CONFIG->GetItemLevelRewardConfig()->GetExpDanReward( role->GetLevel(), other_cfg.exp_dan_type);
				//const int exp_dan_count = other_cfg.exp_dan_count;
				//role->AddExp(static_cast<long long>(reward_exp_factor * exp_dan_exp * exp_dan_count), "CrossMultiuserChallengeFightReward");

//				role->AddShengwang(add_shengwang, "CrossMultiuserChallengeFightReward");
//				role->GetRoleCross()->Add3v3Gongxun(add_gongxun, "CrossMultiuserChallengeFightReward");
				role->GetRoleCross()->SetCrossMultiuserChallengeDurWinCount(user_info->dur_win_count);
//				role->GetRoleCross()->SetCrossMultiuserChallengeScore(user_info->challenge_score);
			}

			gccmcmr.sync_user_info_list[i].uuid = UniqueUserIDToLongLong(user_info->unique_user_id);
			F_STRNCPY(gccmcmr.sync_user_info_list[i].user_name, user_info->user_name, sizeof(gccmcmr.sync_user_info_list[i].user_name));
			gccmcmr.sync_user_info_list[i].level = user_info->level;
			gccmcmr.sync_user_info_list[i].prof = user_info->prof;
			gccmcmr.sync_user_info_list[i].sex = user_info->sex;
			gccmcmr.sync_user_info_list[i].challenge_score = user_info->challenge_score;
			gccmcmr.sync_user_info_list[i].challenge_exp = user_info->challenge_exp;
			gccmcmr.sync_user_info_list[i].challenge_level = user_info->challenge_level;
			gccmcmr.sync_user_info_list[i].match_total_count = user_info->match_total_count;
			gccmcmr.sync_user_info_list[i].match_win_count = user_info->match_win_count;
			gccmcmr.sync_user_info_list[i].dur_win_count = user_info->dur_win_count;
			gccmcmr.sync_user_info_list[i].mvp_count = user_info->mvp_count;
			gccmcmr.sync_user_info_list[i].add_challenge_score = user_info->add_challenge_score;
			gccmcmr.sync_user_info_list[i].add_challenge_exp = user_info->add_challenge_exp;
			gccmcmr.sync_user_info_list[i].is_mvp = 0;
			gccmcmr.sync_user_info_list[i].add_honor = user_info->add_honor;
			gccmcmr.sync_user_info_list[i].add_gongxun = user_info->add_gongxun;
		}
	}
	
	for (int i = 0; i < CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT * CROSS_MULTIUSER_CHALLENGE_SIDE_MAX; i++)
	{
		if (m_mvp_uuid == m_side_user_list[i].unique_user_id)
		{
			++ gccmcmr.sync_user_info_list[i].mvp_count;
			gccmcmr.sync_user_info_list[i].is_mvp = 1;
			break;
		}
	}

	InternalComm::Instance().SendToCross((const char*)&gccmcmr, sizeof(gccmcmr));
}

int SpecialCrossMultiuserChallenge::GetObjSide(ObjID obj_id)
{
	if (INVALID_OBJ_ID == obj_id) return -1;

	for (int i = 0; i < CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT * CROSS_MULTIUSER_CHALLENGE_SIDE_MAX; i++)
	{
		if (m_side_user_list[i].obj_id == obj_id)
		{
			UNSTD_STATIC_CHECK(CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT > 0);
			return i / CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT;
		}
	}

	return -1;
}

int SpecialCrossMultiuserChallenge::GetRoleSide(const UniqueUserID &uuid)
{
	for (int i = 0; i < CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT * CROSS_MULTIUSER_CHALLENGE_SIDE_MAX; i++)
	{
		if (m_side_user_list[i].unique_user_id == uuid)
		{
			UNSTD_STATIC_CHECK(CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT > 0);
			return i / CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT;
		}
	}

	return -1;
}

void SpecialCrossMultiuserChallenge::RefreshMultiuserChallengeSelfInfo(Role *role, CrossMultiUserChallengeUserInfo *user_info)
{
	if (NULL == role) return;

	if (NULL == user_info)
	{
		user_info = this->GetUserInfo(role);
	}

	if (NULL == user_info) return;

	static Protocol::SCCrossMultiuserChallengeSelfInfoRefresh cmcsir;
	cmcsir.self_side = this->GetRoleSide(role->GetRoleCross()->GetCrossOriginalUniqueUserID());
	cmcsir.kills = user_info->kills;
	cmcsir.assist = user_info->assist; 
	cmcsir.dead = user_info->dead;
	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&cmcsir, sizeof(cmcsir));
}

void SpecialCrossMultiuserChallenge::RefreshMultiuserChallengeMatchInfo(Role *role)
{
	static Protocol::SCCrossMultiuserChallengeMatchInfoRefresh cmcmir;

	for (int i = 0; i < CROSS_MULTIUSER_CHALLENGE_SIDE_MAX; i++)
	{
		cmcmir.side_score_list[i] = m_side_score_list[i];
	}

	cmcmir.strong_hold_rate_info = m_stronghold_rate_info;

	if (NULL != role)
	{
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&cmcmir, sizeof(cmcmir));
	}
	else
	{
		m_scene->SendToRole((const char*)&cmcmir, sizeof(cmcmir));
	}
}

void SpecialCrossMultiuserChallenge::RefreshMultiuserChallengeMatchState(Role *role)
{
	static Protocol::SCCrossMultiuserChallengeMatchState cmcms;
	cmcms.match_state = m_state;
	cmcms.win_side = m_win_side;
	cmcms.next_state_time = static_cast<unsigned int>(m_next_state_time);
	
	for (int i = 0; i < CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT * CROSS_MULTIUSER_CHALLENGE_SIDE_MAX; i++)
	{
		cmcms.user_info_list[i].plat_type = static_cast<short>(m_side_user_list[i].unique_user_id.plat_type);
		cmcms.user_info_list[i].obj_id = m_side_user_list[i].obj_id;
		cmcms.user_info_list[i].role_id = UidToInt(m_side_user_list[i].unique_user_id.user_id);
		cmcms.user_info_list[i].prof = m_side_user_list[i].prof;
		cmcms.user_info_list[i].sex	= m_side_user_list[i].sex;
		cmcms.user_info_list[i].kills = m_side_user_list[i].kills;
		cmcms.user_info_list[i].assist = m_side_user_list[i].assist;
		cmcms.user_info_list[i].dead = m_side_user_list[i].dead;
		cmcms.user_info_list[i].occupy = m_side_user_list[i].occupy;
		cmcms.user_info_list[i].origin_score = m_side_user_list[i].challenge_score - m_side_user_list[i].add_challenge_score;
		cmcms.user_info_list[i].add_score = m_side_user_list[i].add_challenge_score;
		cmcms.user_info_list[i].add_honor = m_side_user_list[i].add_honor;
		cmcms.user_info_list[i].add_gongxun = m_side_user_list[i].add_gongxun;
		F_STRNCPY(cmcms.user_info_list[i].name, m_side_user_list[i].user_name, sizeof(cmcms.user_info_list[i].name));

		if (m_side_user_list[i].unique_user_id == m_mvp_uuid) cmcms.user_info_list[i].is_mvp = 1;
		else cmcms.user_info_list[i].is_mvp = 0;
	}

	if (NULL != role)
	{
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&cmcms, sizeof(cmcms));
	}
	else
	{
		m_scene->SendToRole((const char*)&cmcms, sizeof(cmcms));
	}
}

void SpecialCrossMultiuserChallenge::CheckStrongholdRateInfo()
{
	// 获取据点范围内的双方人数
	int distance = 0;
	Posi stronghold_pos = Cross3v3Config::Instance().GetStrongholdPos(distance);
	int side_role_num[CROSS_MULTIUSER_CHALLENGE_SIDE_MAX] = { 0 };
	for (int i = 0; i < CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT * CROSS_MULTIUSER_CHALLENGE_SIDE_MAX; i++)
	{
		ObjID obj_id = m_side_user_list[i].obj_id;

		Obj *obj = m_scene->GetObj(obj_id);
		if (NULL == obj || Obj::OBJ_TYPE_ROLE != obj->GetObjType()) continue;

		Role *role = (Role*)obj;
		if (NULL != role)
		{
			Posi pos = role->GetPos();
			if (!(pos - stronghold_pos).IsLongerThan(distance) && role->IsAlive())
			{
				side_role_num[i / CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT]++;
			}
		}
	}
	
	// 只有单方在据点内，才可以增加占领进度
	if (side_role_num[CROSS_MULTIUSER_CHALLENGE_SIDE_0] > 0 && side_role_num[CROSS_MULTIUSER_CHALLENGE_SIDE_1] == 0)
	{
		int rate = Cross3v3Config::Instance().GetStrongholdRate(side_role_num[CROSS_MULTIUSER_CHALLENGE_SIDE_0]);
		
		m_stronghold_rate_info += rate;
		if (m_stronghold_rate_info >= 100)
		{
			m_stronghold_rate_info = 100;
			m_stronghold_owner_side = CROSS_MULTIUSER_CHALLENGE_SIDE_0;
		}
	}
	else if(side_role_num[CROSS_MULTIUSER_CHALLENGE_SIDE_1] > 0 && side_role_num[CROSS_MULTIUSER_CHALLENGE_SIDE_0] == 0)
	{
		int rate = Cross3v3Config::Instance().GetStrongholdRate(side_role_num[CROSS_MULTIUSER_CHALLENGE_SIDE_1]);
		
		m_stronghold_rate_info -= rate;
		if (m_stronghold_rate_info <= 0)
		{
			m_stronghold_rate_info = 0;
			m_stronghold_owner_side = CROSS_MULTIUSER_CHALLENGE_SIDE_1;
		}
	}

	// 给占领者增加积分。若占领者当前无人在据点范围内，则取最低加分速率
	if (CROSS_MULTIUSER_CHALLENGE_SIDE_0 == m_stronghold_owner_side)
	{
		int role_num = side_role_num[CROSS_MULTIUSER_CHALLENGE_SIDE_0] > 0 ? side_role_num[CROSS_MULTIUSER_CHALLENGE_SIDE_0] : 1;
		int add_score = Cross3v3Config::Instance().GetStrongholdRoleRate(role_num);
		this->AddSideScore(CROSS_MULTIUSER_CHALLENGE_SIDE_0, add_score);
	}
	else if (CROSS_MULTIUSER_CHALLENGE_SIDE_1 == m_stronghold_owner_side)
	{
		int role_num = side_role_num[CROSS_MULTIUSER_CHALLENGE_SIDE_1] > 0 ? side_role_num[CROSS_MULTIUSER_CHALLENGE_SIDE_1] : 1;
		int add_score = Cross3v3Config::Instance().GetStrongholdRoleRate(role_num);
		this->AddSideScore(CROSS_MULTIUSER_CHALLENGE_SIDE_1, add_score);
	}

	this->RefreshMultiuserChallengeMatchInfo();
}

CrossMultiUserChallengeUserInfo *SpecialCrossMultiuserChallenge::GetUserInfo(UniqueUserID original_unique_user_id)
{
	for (int i = 0; i < CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT * CROSS_MULTIUSER_CHALLENGE_SIDE_MAX; i++)
	{
		if (m_side_user_list[i].unique_user_id == original_unique_user_id)
		{
			return &m_side_user_list[i];
		}
	}

	return NULL;
}

CrossMultiUserChallengeUserInfo * SpecialCrossMultiuserChallenge::GetUserInfo(Role *role)
{
	if (NULL == role) return NULL;

	return this->GetUserInfo(role->GetRoleCross()->GetCrossOriginalUniqueUserID());
}

void SpecialCrossMultiuserChallenge::AddSideScore(int side, int add_score)
{
	if (side < 0 || side >= CROSS_MULTIUSER_CHALLENGE_SIDE_MAX) return;

	m_side_score_list[side] += add_score;
	
	if (this->IsFighting())
	{
		// 一方首先达到完成积分方，比赛结束
		int FINISH_MATCH_SCORE = Cross3v3Config::Instance().GetFinishMatchScore();
		if (m_side_score_list[side] >= FINISH_MATCH_SCORE)
		{
			m_next_state_time = EngineAdapter::Instance().Time();
		}
	}
}

int SpecialCrossMultiuserChallenge::CalSideAverageScore(int side)
{
	if (side < 0 || side >= CROSS_MULTIUSER_CHALLENGE_SIDE_MAX) return 0;

	int average_score = 0;
	for (int i = 0; i < CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT; i++)
	{
		int index = side * CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT + i;

		if (index < 0 || index >= CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT * CROSS_MULTIUSER_CHALLENGE_SIDE_MAX)
		{
			continue;
		}

		average_score += m_side_user_list[index].challenge_score;
	}

	return average_score / CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT;
}

int SpecialCrossMultiuserChallenge::GetOpponentSide(int side)
{
	if (CROSS_MULTIUSER_CHALLENGE_SIDE_0 == side) return CROSS_MULTIUSER_CHALLENGE_SIDE_1;
	else return CROSS_MULTIUSER_CHALLENGE_SIDE_0;
}

