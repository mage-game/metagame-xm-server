#include "specialtowerdefendteam.hpp"
#include "engineadapter.h"
#include "gamecommon.h"
#include "servercommon/errornum.h"

#include "obj/character/role.h"
#include "obj/character/monster.h"
#include "monster/monsterpool.h"

#include "scene/scenemanager.h"
#include "world.h"
#include "servercommon/internalprotocal/gglobalprotocal.h"
#include "protocal/msgfight.h"
#include "other/event/eventhandler.hpp"
#include "internalcomm.h"
#include "servercommon/string/gameworldstr.h"

#include "ai/monsterai/specialai/monsteraitowerdefendteam.hpp"

#include "config/logicconfigmanager.hpp"

#include "effect/skilleffect/effectbuff.hpp"
#include "effect/sceneeffect/sceneeffect.hpp"
#include "effect/effectbase.hpp"

#include "other/fb/towerdefendteamskill.hpp"
#include "other/fb/towerdefendteamfunc.hpp"
#include "other/specialappearance/specialappearance.hpp"
#include "other/mount/mountmanager.hpp"
#include "other/fb/rolefbtowerdefend.hpp"
#include "friend/teaminfo.hpp"
#include "global/team/teammanager.hpp"
#include "global/team/team.hpp"
#include "npc/transmit/fbmanager.hpp"
#include "globalconfig/globalconfig.h"
#include "other/daycounter/daycounter.hpp"
#include "gameworld/effect/effectbase.hpp"
#include "gameworld/obj/otherobj/fallingitem.h"
#include "globalconfig/otherconfig.h"
#include "gameworld/item/itempool.h"
#include "other/xiannv/xiannvmanager.hpp"

SpecialTeamTowerDefend::SpecialTeamTowerDefend(Scene *scene)
	: SpecialLogic(scene), m_is_inited(0), m_is_finish(false), m_is_pass(false), m_is_last_wave(false), m_has_role_leave(false), m_team_index(0), m_pass_level(0), m_passtime_s(0),
	m_curr_wave_index(-1), m_next_wave_start_time(0), m_next_refresh_one_time(0), m_clear_wave_count(0),
	m_last_check_monster_status_time(0), m_last_warning_attack_time(0), m_last_sync_to_allclient_time(0),
	m_gongji_uid(0), m_fangyu_uid(0), m_assist_uid(0), m_life_tower_left_hp(0), m_life_tower_left_maxhp(0), 
	m_add_monster_maxhp_per(0), m_add_monster_gongji_per(0), m_add_monster_fangyu_per(0), m_last_sync_role_info_time(0),m_have_role_enter(false), m_pass_kick_out_time(0)
{
	memset(m_refresh_left_count, 0, sizeof(m_refresh_left_count));
	memset(m_kill_count, 0, sizeof(m_kill_count));
}

SpecialTeamTowerDefend::~SpecialTeamTowerDefend()
{

}

void SpecialTeamTowerDefend::Update(unsigned long interval, time_t now_second)
{
	SpecialLogic::Update(interval, now_second);

	if (m_is_finish)
	{
		if (m_pass_kick_out_time > 0 && now_second >= m_pass_kick_out_time)
		{
			this->DelayKickOutAllRole(CHANGE_SCENE_REASON_FB_FINISH);
		}
		return;
	}

	{
		// 刚启动
		if (0 == m_is_inited)
		{
			m_is_inited = static_cast<int>(EngineAdapter::Instance().Time());
			UInt32 monster_num = m_scene->MonsterNum();
			for (int i = 0; i < (int)monster_num; ++i)
			{
				Monster *monster = m_scene->GetMonsterByIndex(i);
				if (NULL != monster && monster->IsAlive() &&
					monster->GetMonsterId() == LOGIC_CONFIG->GetTeamTowerDefendConfig().GetTeamLifeTowerMonsterID())
				{
					m_life_tower_left_maxhp = monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
					m_life_tower_left_hp = m_life_tower_left_maxhp;

					break;
				}
			}

			this->BroadcastToClient(Protocol::SCTeamTowerDefendInfo::NOTIFY_REASON_INIT);
			m_last_sync_to_allclient_time = static_cast<unsigned int>(now_second);
		}
	}

	{
		// 新一波开始
		if (m_next_wave_start_time > 0 && now_second >= m_next_wave_start_time)
		{
			++m_curr_wave_index;

			const TeamTowerDefendWaveCfg * wave_cfg = LOGIC_CONFIG->GetTeamTowerDefendConfig().GetTeamWaveCfg(m_curr_wave_index);
			if (NULL != wave_cfg)
			{
				m_next_wave_start_time = static_cast<unsigned int>(now_second) + wave_cfg->next_wave_s;

				for (int i = 0; i < TEAM_TOWERDEFEND_PATH_MAX; ++i)
				{
					m_refresh_left_count[i] = wave_cfg->monster_count_list[i];
				}

				this->RefreshOne(now_second);
			}

			if (m_curr_wave_index + 1 >= LOGIC_CONFIG->GetTeamTowerDefendConfig().GetTeamDefendWaveCount())	// 最后一波了
			{
				m_is_last_wave = true;
				m_next_wave_start_time = 0;
			}

			this->BroadcastToClient(Protocol::SCTeamTowerDefendInfo::NOTIFY_REASON_NEXT_WAVE); m_last_sync_to_allclient_time = static_cast<unsigned int>(now_second);
		}
	}

	{
		// 按频率在各路线出怪
		if (m_next_refresh_one_time > 0 && now_second >= m_next_refresh_one_time)
		{
			this->RefreshOne(now_second);
		}
	}

	{
		// 检查是否杀光
		this->CheckAttackMonsterStatus(interval, now_second);
	}

	{
		if (m_curr_wave_index >= 0 && !m_is_finish &&
			now_second - m_last_sync_to_allclient_time >= LOGIC_CONFIG->GetTeamTowerDefendConfig().GetSyncInterval())
		{
			this->BroadcastToClient(); m_last_sync_to_allclient_time = static_cast<unsigned int>(now_second);
		}
	}

	if (now_second > m_last_sync_role_info_time + 1)
	{
		m_last_sync_role_info_time = static_cast<int>(now_second);
		this->SyncAllRoleInfo();
	}
}

void SpecialTeamTowerDefend::OnRoleEnterScene(Role *role)
{
	if (nullptr == role) return;

	int uid = UidToInt(role->GetUserId());

	bool is_first_enter = false;

	for (int i = 0; i < MAX_TEAM_MEMBER_NUM; ++i)
	{
		if (m_personal_info_list[i].uid == uid) break;

		if (0 == m_personal_info_list[i].uid)
		{
			m_personal_info_list[i].uid = uid;
			m_personal_info_list[i].camp = role->GetCamp();
			role->GetName(m_personal_info_list[i].role_name);

			if (TEAM_TOWERDEFEND_ATTRTYPE_GONGJI == role->GetFBTowerDefend()->GetTeamAttrType())
			{
				m_personal_info_list[i].attr_type = TEAM_TOWERDEFEND_ATTRTYPE_GONGJI;
				m_gongji_uid = uid;

				this->InitSkill(m_personal_info_list[i], TEAM_TOWERDEFEND_ATTRTYPE_GONGJI);
			}
			else if (TEAM_TOWERDEFEND_ATTRTYPE_FANGYU == role->GetFBTowerDefend()->GetTeamAttrType())
			{
				m_personal_info_list[i].attr_type = TEAM_TOWERDEFEND_ATTRTYPE_FANGYU;
				m_fangyu_uid = uid;

				this->InitSkill(m_personal_info_list[i], TEAM_TOWERDEFEND_ATTRTYPE_FANGYU);
			}
			else
			{
				m_personal_info_list[i].attr_type = TEAM_TOWERDEFEND_ATTRTYPE_ASSIST;
				m_assist_uid = uid;

				this->InitSkill(m_personal_info_list[i], TEAM_TOWERDEFEND_ATTRTYPE_ASSIST);
			}

			is_first_enter = true;


			const TeamTowerDefendOtherCfg &other_cfg = LOGIC_CONFIG->GetTeamTowerDefendConfig().GetOtherCfg();
			if (other_cfg.free_join_times >= role->GetDayCounter()->GetDayCount(DayCounter::DAYCOUNT_ID_TEAM_TOWERDEFEND_JOIN_TIMES))
			{
				role->GetFBTowerDefend()->GiveTeamWaveReward(m_personal_info_list[i].reward_list,DROP_ITEM_TYPE_MAX,m_personal_info_list[i].angry_value);
			}
			
			break;
		}
	}

	//if (uid == m_gongji_uid)
	//{
	//	role->GetSpecialAppearance()->SetFbAppearance(SPECIAL_APPEARANCE_TYPE_TOWERDEFENCE_ZHUQUE, 0, 0);
	//}
	//else if (uid == m_fangyu_uid)
	//{
	//	role->GetSpecialAppearance()->SetFbAppearance(SPECIAL_APPEARANCE_TYPE_TOWERDEFENCE_XUANWU, 0, 0);
	//}
	//else
	//{
	//	role->GetSpecialAppearance()->SetFbAppearance(SPECIAL_APPEARANCE_TYPE_TOWERDEFENCE_QINGLONG, 0, 0);
	//}

	this->RecalcAttrAdd(role, is_first_enter);



	// 刚启动
	if (!m_have_role_enter)
	{
		m_have_role_enter = true;

		int min_wave = role->GetFBTowerDefend()->GetTeamMemberMinWave();
		if (min_wave >= 0 && min_wave <= LOGIC_CONFIG->GetTeamTowerDefendConfig().GetTeamDefendWaveCount())
		{
			m_curr_wave_index = min_wave - 1;
			m_clear_wave_count = min_wave;
		}

		m_next_wave_start_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + LOGIC_CONFIG->GetTeamTowerDefendConfig().GetRefreshStartDelay();

		this->BroadcastToClient(Protocol::SCTeamTowerDefendInfo::NOTIFY_REASON_INIT);
		m_last_sync_to_allclient_time = static_cast<unsigned int>(m_is_inited);
	}
	else 
	{
		this->BroadcastToClient(Protocol::SCTeamTowerDefendInfo::NOTIFY_REASON_INIT);
	}

	if (0 == m_team_index)
	{
		m_team_index = role->GetTeamInfo()->GetTeamIndex();

		Team *team = TeamManager::Instance().GetTeam(m_team_index);

		int m_member_num = (nullptr != team) ? team->GetMemberNum() : MAX_TEAM_MEMBER_NUM;
		const OtherTeamfbMonsterAttrConfig* monster_upattr_cfg = LOGIC_CONFIG->GetOtherCfg().GetTeamFbMonsterAttrCfg(m_member_num);
		if (nullptr != monster_upattr_cfg)
		{
			m_add_monster_maxhp_per = monster_upattr_cfg->monster_hp_percent;
			m_add_monster_gongji_per = monster_upattr_cfg->monster_gongji_percent;
			m_add_monster_fangyu_per = monster_upattr_cfg->monster_fangyu_percent;
		}
	}

	if (m_is_finish)
	{
		this->OnFinish(m_is_pass, role);
	}

	this->SyncFBLogicInfo();
	this->SyncAllRoleInfo();
	EventHandler::Instance().OnEnterFBTowerDefendTeam(role,is_first_enter);
}

void SpecialTeamTowerDefend::OnRoleLeaveScene(Role *role, bool is_logout)
{
	m_has_role_leave = true;

	if (NULL != role)
	{
		int uid = UidToInt(role->GetUserId());

		for (int i = 0; i < MAX_TEAM_MEMBER_NUM; ++i)
		{
			if (uid == m_personal_info_list[i].uid)
			{
				m_personal_info_list[i].hp_when_leave = role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
				break;
			}
		}
	}
}

void SpecialTeamTowerDefend::OnCharacterDie(Character *dead, ObjID killer_id)
{
	if (Obj::OBJ_TYPE_MONSTER == dead->GetObjType())
	{
		Monster *monster = (Monster*)dead;

		if (monster->GetMonsterId() == LOGIC_CONFIG->GetTeamTowerDefendConfig().GetTeamLifeTowerMonsterID())
		{
			if (!m_is_finish)
			{
				m_life_tower_left_hp = 0;
				this->BroadcastToClient();

				m_is_finish = true;

				this->FinishBroadcast();
				this->OnFinish(false);
			}
		}
		else
		{
			if (MonsterInitParam::AITYPE_SPECIALAI_TOWERDEFEND_TEAM_ATTACK == monster->GetAIType())
			{
				const TeamTowerDefendWaveCfg *wave_cfg = LOGIC_CONFIG->GetTeamTowerDefendConfig().GetTeamWaveCfgByMonsterID(monster->GetMonsterId());
				if (NULL != wave_cfg)
				{
					int wave = wave_cfg->wave;
					if (wave >= 0 && wave < TEAM_TOWERDEFEND_WAVE_COUNT)
					{
						++m_kill_count[wave];

						const TeamTowerDefendOtherCfg &other_cfg = LOGIC_CONFIG->GetTeamTowerDefendConfig().GetOtherCfg();
						// 杀光一波 更新记录
						if (m_kill_count[wave] >= wave_cfg->total_monster_count)
						{

							if (m_clear_wave_count < LOGIC_CONFIG->GetTeamTowerDefendConfig().GetTeamDefendWaveCount())
							{
								++m_clear_wave_count;
							}

							for (int i = 0; i < (int)m_scene->RoleNum(); ++i)
							{
								Role *temp_role = m_scene->GetRoleByIndex(i);
								if (NULL != temp_role && Obj::OBJ_TYPE_ROLE == temp_role->GetObjType() &&
									other_cfg.free_join_times >= temp_role->GetDayCounter()->GetDayCount(DayCounter::DAYCOUNT_ID_TEAM_TOWERDEFEND_JOIN_TIMES))
								{
									temp_role->GetFBTowerDefend()->SetTeamPassedWave(m_clear_wave_count);
									EventHandler::Instance().OnTowerdefendteamFBPassWave(temp_role, m_clear_wave_count - 1, m_clear_wave_count);
								}
							}
						}

						int angry_value = LOGIC_CONFIG->GetTeamTowerDefendConfig().GetMonsterAngryValue(monster->GetMonsterId());

						if (angry_value > 0)
						{
							for (unsigned int i = 0; i < m_scene->RoleNum(); ++i)
							{
								Role *tmp_role = m_scene->GetRoleByIndex(i);
								if (nullptr == tmp_role)
								{
									continue;
								}
							
								if (other_cfg.free_join_times >= tmp_role->GetDayCounter()->GetDayCount(DayCounter::DAYCOUNT_ID_TEAM_TOWERDEFEND_JOIN_TIMES))
								{
									for (int j = 0; j < MAX_TEAM_MEMBER_NUM; ++j)
									{
										if (m_personal_info_list[j].uid == tmp_role->GetUID())
										{
											m_personal_info_list[j].angry_value += angry_value;
										}
									}
									tmp_role->GetXiannvManager()->AddShengwuEssence(angry_value, "TeamTowerDefend::Add");
								}//if
							}
						}
						this->BroadcastToClient();
					}
				}
			}
		}
	}
}

void SpecialTeamTowerDefend::OnAttack(Character *cha, ObjID enemy_id, Attribute delta_hp)
{
	if (Obj::OBJ_TYPE_MONSTER == cha->GetObjType())
	{
		Monster *monster = (Monster*)cha;

		if (monster->GetMonsterId() == LOGIC_CONFIG->GetTeamTowerDefendConfig().GetTeamLifeTowerMonsterID() && monster->IsAlive())
		{
			unsigned int now_second = static_cast<unsigned int>(EngineAdapter::Instance().Time());

			{
				if (now_second >= m_last_warning_attack_time + LOGIC_CONFIG->GetTeamTowerDefendConfig().GetWarningInterval())
				{
					m_last_warning_attack_time = now_second;

					this->NotifyWarning(Protocol::SCTowerDefendWarning::WARNING_TYPE_NORMAL, 0);
				}
			}

			{
				long long now_hp = monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
				long long max_hp = monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
				long long old_hp = now_hp + (-delta_hp);

				int old_percent = static_cast<int>(max_hp > 0 ? old_hp * 100 / max_hp : 0);
				int now_percent = static_cast<int>(max_hp > 0 ? now_hp * 100 / max_hp : 0);

				if (old_percent > now_percent)
				{
					int warning_percent = 0;
					if (LOGIC_CONFIG->GetTeamTowerDefendConfig().CheckWarning(old_percent, now_percent, &warning_percent))
					{
						this->NotifyWarning(Protocol::SCTowerDefendWarning::WARNING_TYPE_LOW_PERCENT, warning_percent);
					}
				}

				m_life_tower_left_hp = static_cast<int>(now_hp);
			}
		}
	}
}

bool SpecialTeamTowerDefend::OnTimeout()
{
	if (!m_is_finish)
	{
		m_is_finish = true;

		this->FinishBroadcast();
		this->OnFinish(false);
		return false;
	}

	return m_is_pass;
}


void SpecialTeamTowerDefend::OnLeaveFB(Role *role)
{
	if (!m_is_finish)
	{
		this->OnFinish(false, role);
	}

	role->GetSpecialAppearance()->SetFbAppearance(SPECIAL_APPEARANCE_TYPE_NORMAL, 0, false);
	role->GetMountManager()->GoonMount(true, false, true);
}

bool SpecialTeamTowerDefend::SpecialRelive(Role *role, int realive_type)
{
	if (Protocol::CSRoleReAliveReq::REALIVE_TYPE_BACK_HOME == realive_type)
	{
		Posi realive_pos;
		int target_scene_id = m_scene->GetTownPoint(role, &realive_pos);
		if (target_scene_id == m_scene->GetSceneID())
		{
			if (role->ReAlive(50, 100, realive_pos)) return true;
		}
	}

	return false;
}

bool SpecialTeamTowerDefend::SpecialIsEnemy(Role *role, Obj *obj)
{
	switch (obj->GetObjType())
	{
	case Obj::OBJ_TYPE_ROLE:
	{
		return false;
	}
	break;

	case Obj::OBJ_TYPE_MONSTER:
	{
		Monster *monster = (Monster*)obj;
		return monster->IsAlive() && MonsterInitParam::AITYPE_SPECIALAI_TOWERDEFEND_TEAM_ATTACK == monster->GetAIType();
	}
	break;

	default:
		break;
	}

	return SpecialLogic::SpecialIsEnemy(role, obj);
}

bool SpecialTeamTowerDefend::SpecialIsFriend(Role *role, Obj *obj, bool check_alive)
{
	switch (obj->GetObjType())
	{
	case Obj::OBJ_TYPE_ROLE:
	{
		Role *friend_role = (Role*)obj;
		return (!check_alive || friend_role->IsAlive());
	}
	break;

	case Obj::OBJ_TYPE_MONSTER:
	{
		Monster *monster = (Monster*)obj;
		return (!check_alive || monster->IsAlive()) && MonsterInitParam::AITYPE_SPECIALAI_TOWERDEFEND_TEAM_ATTACK == monster->GetAIType();
	}
	break;
	}

	return SpecialLogic::SpecialIsFriend(role, obj, check_alive);
}

bool SpecialTeamTowerDefend::SpecialCanReAlive(Role *role, int realive_type)
{
	if (Protocol::CSRoleReAliveReq::REALIVE_TYPE_BACK_HOME == realive_type)
	{
		return role->GetDeadTime() >= TEAM_TOWERDEFEND_HOME_REALIVE_LIMIT_INTERVAL;
	}

	return role->GetDeadTime() >= TEAM_TOWERDEFEND_GOLD_REALIVE_LIMIT_INTERVAL;
}

void SpecialTeamTowerDefend::OnNextWaveReq(Role *role)
{
	const TeamTowerDefendWaveCfg * wave_cfg = LOGIC_CONFIG->GetTeamTowerDefendConfig().GetTeamWaveCfg(m_curr_wave_index);
	if (NULL == wave_cfg) return;

	for (int i = 0; i < TEAM_TOWERDEFEND_PATH_MAX; ++i)
	{
		if (m_refresh_left_count[i] > 0)
		{
			role->NoticeNum(errornum::EN_TOWER_DEFEND_CURR_WAVE_NOT_DONE);
			return;
		}
	}

	int monster_num = m_scene->MonsterNum();
	for (int i = 0; i < monster_num; ++i)
	{
		Monster *monster = m_scene->GetMonsterByIndex(i);
		if (monster != NULL && MonsterInitParam::AITYPE_SPECIALAI_TOWERDEFEND_TEAM_ATTACK == monster->GetAIType() && monster->IsAlive())
		{
			role->NoticeNum(errornum::EN_FB_REST_MONSTER_NUM_LIMIT);
			return;
		}
	}

	unsigned int now_second = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	if (m_next_wave_start_time > 0 && now_second < m_next_wave_start_time)
	{
		m_next_wave_start_time = now_second;

		this->BroadcastToClient();
	}
}

void SpecialTeamTowerDefend::RefreshOne(time_t now_second)
{
	const TeamTowerDefendWaveCfg * wave_cfg = LOGIC_CONFIG->GetTeamTowerDefendConfig().GetTeamWaveCfg(m_curr_wave_index);
	if (NULL == wave_cfg) return;

	bool is_left = false;

	for (int path = TOWERDEFEND_PATH_LEFT; path < TEAM_TOWERDEFEND_PATH_MAX; ++path)
	{
		if (m_refresh_left_count[path] > 0)
		{
			Attribute add_maxhp = 0, add_gongji = 0, add_fangyu = 0;

			const MonsterInitParam *monster_param = MONSTERPOOL->GetMonsterParam(wave_cfg->monster_id_list[path]);
			if (nullptr != monster_param)
			{
				if (m_add_monster_maxhp_per > 0)
				{
					add_maxhp = static_cast<Attribute>(m_add_monster_maxhp_per * ROLE_ATTR_PER * monster_param->hp);
				}
				if (m_add_monster_gongji_per > 0)
				{
					add_gongji = static_cast<Attribute>(m_add_monster_gongji_per * ROLE_ATTR_PER * monster_param->gongji);
				}
				if (m_add_monster_fangyu_per > 0)
				{
					add_fangyu = static_cast<Attribute>(m_add_monster_fangyu_per * ROLE_ATTR_PER * monster_param->fangyu);
				}
			}

			Posi birth_pos = LOGIC_CONFIG->GetTowerDefendConfig().GetBirthPos(path);

			Monster *monster = MONSTERPOOL->CreateMonster(wave_cfg->monster_id_list[path], m_scene, gamecommon::GetDisRandPos(birth_pos, 4), add_maxhp, add_gongji, add_fangyu);
			if (NULL != monster)
			{
				MonsterAIBase *aibase = monster->GetMonsterAI();
				if (NULL != aibase && MonsterInitParam::AITYPE_SPECIALAI_TOWERDEFEND_TEAM_ATTACK == aibase->GetAIType())
				{
					MonsterAITowerDefendTeam *aiattack = (MonsterAITowerDefendTeam *)aibase;
					aiattack->SetBirthPath(path);
				}

				--m_refresh_left_count[path];
			}

			if (m_refresh_left_count[path] > 0) is_left = true;
		}
	}

	if (is_left)
	{
		m_next_refresh_one_time = static_cast<unsigned int>(now_second) + LOGIC_CONFIG->GetTeamTowerDefendConfig().GetRefreshInterval();
	}
	else
	{
		m_next_refresh_one_time = 0;
	}
}

void SpecialTeamTowerDefend::CheckAttackMonsterStatus(unsigned long interval, time_t now_second)
{
	if (now_second >= m_last_check_monster_status_time + 1)
	{
		m_last_check_monster_status_time = static_cast<unsigned int>(now_second);

		for (int i = 0; i < TEAM_TOWERDEFEND_PATH_MAX; ++i)
		{
			if (m_refresh_left_count[i] > 0) return; // 怪都没出完呢
		}
		UInt32 monster_num = m_scene->MonsterNum();
		for (int i = 0; i < (int)monster_num; ++i)
		{
			Monster *monster = m_scene->GetMonsterByIndex(i);
			if (NULL != monster && monster->IsAlive() && MonsterInitParam::AITYPE_SPECIALAI_TOWERDEFEND_TEAM_ATTACK == monster->GetAIType())
			{
				return;
			}
		}

		this->OnAllAttackMonsterDead();
	}
}

void SpecialTeamTowerDefend::OnAllAttackMonsterDead()
{
	if (!m_is_finish && m_is_last_wave)
	{
		m_is_finish = true;

		this->FinishBroadcast();
		this->OnFinish(true);
	}
}

void SpecialTeamTowerDefend::OnFinish(bool is_pass, Role *role, short notify_reason)
{
	m_is_pass = is_pass;

	Protocol::SCTeamTowerDefendResult ttdr;
	ttdr.is_passed = m_is_pass ? 1 : 0;
	ttdr.clear_wave_count = m_clear_wave_count;
	ttdr.use_time = static_cast<int>(EngineAdapter::Instance().Time()) - m_is_inited;

	if (NULL != role)
	{
		int role_idx = 0;

		for (int i = 0; i < MAX_TEAM_MEMBER_NUM; ++i)
		{
			if (m_personal_info_list[i].uid == role->GetUID())
			{
				role_idx = i;
			}
		}

		//额外奖励（没次数不给额外奖励，智霖需求2019.4.26）
		const TeamTowerDefendOtherCfg &other_cfg = LOGIC_CONFIG->GetTeamTowerDefendConfig().GetOtherCfg();
		if (other_cfg.free_join_times >= role->GetDayCounter()->GetDayCount(DayCounter::DAYCOUNT_ID_TEAM_TOWERDEFEND_JOIN_TIMES))
		{
			int extra_item_count = 0;
			ItemConfigData extra_item_list[ItemNamespace::MAX_KNAPSACK_GRID_NUM];

			std::vector<UInt16>::const_iterator extra_reward_it = LOGIC_CONFIG->GetTeamTowerDefendConfig().GetOtherCfg().dropid_list.begin();
			for (; extra_reward_it != LOGIC_CONFIG->GetTeamTowerDefendConfig().GetOtherCfg().dropid_list.end(); ++extra_reward_it)
			{
				int extra_reward_count = 0;
				ItemConfigData extra_reward_list[DropConfig::DROP_ITEM_PROB_MAX];
				extra_reward_count = DROPPOOL->GetDropItemList(*extra_reward_it, extra_reward_list);

				for (int i = 0; i < extra_reward_count && i < DropConfig::DROP_ITEM_PROB_MAX; ++i)
				{
					if (!ItemExtern::PutInItemConfigList(ItemNamespace::MAX_KNAPSACK_GRID_NUM, extra_item_list, &extra_item_count,
						extra_reward_list[i].item_id, extra_reward_list[i].num, extra_reward_list[i].is_bind))
					{
						break;
					}
				}
			}

			// 发放奖励
			for (int i = 0; i < extra_item_count && i <= ItemNamespace::MAX_KNAPSACK_GRID_NUM - MAX_ATTACHMENT_ITEM_NUM; i += MAX_ATTACHMENT_ITEM_NUM)
			{
				role->GetKnapsack()->PutListOrMail(extra_item_list + i, PUT_REASON_TOWERDEFENF_TEAM_FB_EXTRA_REWARD);
			}

			for (int j = 0; j < extra_item_count && j < ItemNamespace::MAX_KNAPSACK_GRID_NUM; j++)
			{
				bool is_find = false;
				int i = 0;
				for (; i < DROP_ITEM_TYPE_MAX; i++)
				{
					if (m_personal_info_list[role_idx].reward_list[i].item_id != 0)
					{
						if (m_personal_info_list[role_idx].reward_list[i].item_id == extra_item_list[j].item_id
							&& m_personal_info_list[role_idx].reward_list[i].is_bind == extra_item_list[j].is_bind)
						{
							m_personal_info_list[role_idx].reward_list[i].num += extra_item_list[j].num;
							is_find = true;

							break;
						}
					}
					else
					{
						break;
					}
				}

				if (!is_find && i < DROP_ITEM_TYPE_MAX)
				{
					m_personal_info_list[role_idx].reward_list[i].item_id = extra_item_list[j].item_id;
					m_personal_info_list[role_idx].reward_list[i].num = extra_item_list[j].num;
					m_personal_info_list[role_idx].reward_list[i].is_bind = extra_item_list[j].is_bind;
				}
			}
		}

		this->AutoPickFallingItem(role);
		role->NotifyFBFinish();
		ttdr.item_count = 0;
		ttdr.xiannv_shengwu = 0;
		for (int i = 0; i < MAX_TEAM_MEMBER_NUM; ++i)
		{
			if (m_personal_info_list[i].uid == role->GetUID())
			{
				ItemConfigData * reward_item = m_personal_info_list[i].reward_list;
				for (int j = 0; j < DROP_ITEM_TYPE_MAX && ttdr.item_count < Protocol::SCTeamTowerDefendResult::FB_DROP_ITEM_MAX_NUM; j++)
				{
					if (reward_item[j].item_id == 0) break;
					ttdr.drop_items[ttdr.item_count].item_id = reward_item[j].item_id;
					ttdr.drop_items[ttdr.item_count].num = static_cast<short>(reward_item[j].num);
					ttdr.item_count++;
				}
				ttdr.xiannv_shengwu = m_personal_info_list[i].angry_value;
			}
		}
		int len = sizeof(ttdr) - sizeof(Protocol::SCTeamTowerDefendResult::DropItem)*(Protocol::SCTeamTowerDefendResult::FB_DROP_ITEM_MAX_NUM - ttdr.item_count);
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&ttdr, len);
	}
	else
	{
		for (int i = 0; i < (int)m_scene->RoleNum(); ++i)
		{
			Role *temp_role = m_scene->GetRoleByIndex(i);
			if (NULL != temp_role)
			{
				//额外奖励（没次数不给额外奖励，智霖需求2019.4.26）
				const TeamTowerDefendOtherCfg &other_cfg = LOGIC_CONFIG->GetTeamTowerDefendConfig().GetOtherCfg();
				if (other_cfg.free_join_times >= temp_role->GetDayCounter()->GetDayCount(DayCounter::DAYCOUNT_ID_TEAM_TOWERDEFEND_JOIN_TIMES))
				{
					int role_idx = 0;

					for (int i = 0; i < MAX_TEAM_MEMBER_NUM; ++i)
					{
						if (m_personal_info_list[i].uid == temp_role->GetUID())
						{
							role_idx = i;
						}
					}

					int extra_item_count = 0;
					ItemConfigData extra_item_list[ItemNamespace::MAX_KNAPSACK_GRID_NUM];

					std::vector<UInt16>::const_iterator extra_reward_it = LOGIC_CONFIG->GetTeamTowerDefendConfig().GetOtherCfg().dropid_list.begin();
					for (; extra_reward_it != LOGIC_CONFIG->GetTeamTowerDefendConfig().GetOtherCfg().dropid_list.end(); ++extra_reward_it)
					{
						int extra_reward_count = 0;
						ItemConfigData extra_reward_list[DropConfig::DROP_ITEM_PROB_MAX];
						extra_reward_count = DROPPOOL->GetDropItemList(*extra_reward_it, extra_reward_list);

						for (int i = 0; i < extra_reward_count && i < DropConfig::DROP_ITEM_PROB_MAX; ++i)
						{
							if (!ItemExtern::PutInItemConfigList(ItemNamespace::MAX_KNAPSACK_GRID_NUM, extra_item_list, &extra_item_count,
								extra_reward_list[i].item_id, extra_reward_list[i].num, extra_reward_list[i].is_bind))
							{
								break;
							}
						}
					}

					// 发放额外奖励
					for (int i = 0; i < extra_item_count && i <= ItemNamespace::MAX_KNAPSACK_GRID_NUM - MAX_ATTACHMENT_ITEM_NUM; i += MAX_ATTACHMENT_ITEM_NUM)
					{
						temp_role->GetKnapsack()->PutListOrMail(extra_item_list + i, PUT_REASON_TOWERDEFENF_TEAM_FB_EXTRA_REWARD);
					}

					for (int j = 0; j < extra_item_count && j < ItemNamespace::MAX_KNAPSACK_GRID_NUM; j++)
					{
						bool is_find = false;
						int i = 0;
						for (; i < DROP_ITEM_TYPE_MAX; i++)
						{
							if (m_personal_info_list[role_idx].reward_list[i].item_id != 0)
							{
								if (m_personal_info_list[role_idx].reward_list[i].item_id == extra_item_list[j].item_id
									&& m_personal_info_list[role_idx].reward_list[i].is_bind == extra_item_list[j].is_bind)
								{
									m_personal_info_list[role_idx].reward_list[i].num += extra_item_list[j].num;
									is_find = true;

									break;
								}
							}
							else
							{
								break;
							}
						}

						if (!is_find && i < DROP_ITEM_TYPE_MAX)
						{
							m_personal_info_list[role_idx].reward_list[i].item_id = extra_item_list[j].item_id;
							m_personal_info_list[role_idx].reward_list[i].num = extra_item_list[j].num;
							m_personal_info_list[role_idx].reward_list[i].is_bind = extra_item_list[j].is_bind;
						}
					}
				}

				this->AutoPickFallingItem(temp_role);
				ttdr.item_count = 0;
				for (int j = 0; j < MAX_TEAM_MEMBER_NUM; ++j)
				{
					if (m_personal_info_list[j].uid == temp_role->GetUID())
					{
						ItemConfigData * reward_item = m_personal_info_list[j].reward_list;
						for (int k = 0; k < DROP_ITEM_TYPE_MAX && ttdr.item_count < Protocol::SCTeamTowerDefendResult::FB_DROP_ITEM_MAX_NUM; k++)
						{
							if (reward_item[k].item_id == 0) break;
							ttdr.drop_items[ttdr.item_count].item_id = reward_item[k].item_id;
							ttdr.drop_items[ttdr.item_count].num = static_cast<short>(reward_item[k].num);
							ttdr.item_count++;
						}
						ttdr.xiannv_shengwu = m_personal_info_list[j].angry_value;
					}
				}
				int len = sizeof(ttdr) - sizeof(Protocol::SCTeamTowerDefendResult::DropItem)*(Protocol::SCTeamTowerDefendResult::FB_DROP_ITEM_MAX_NUM - ttdr.item_count);
				EngineAdapter::Instance().NetSend(temp_role->GetNetId(), (const char *)&ttdr, len);

				temp_role->NotifyFBFinish();

				temp_role->GetFBTowerDefend()->SetTeamAttrType(TEAM_TOWERDEFEND_ATTRTYPE_INVALID);
			}
		}
	}

	if (m_is_finish) // 完成了才清理怪物
	{
		if (m_is_pass)
		{
			m_pass_kick_out_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + 9;
		}
		else
		{
			this->DelayKickOutAllRole(CHANGE_SCENE_REASON_FB_FINISH);
		}
		UInt32 monster_num = m_scene->MonsterNum();
		for (int i = 0; i < static_cast<int>(monster_num); i++)
		{
			Monster *monster = m_scene->GetMonsterByIndex(i);
			if (NULL != monster && monster->GetMonsterId() != LOGIC_CONFIG->GetTeamTowerDefendConfig().GetTeamLifeTowerMonsterID())
			{
				monster->ForceSetDead();
			}
		}
	}

	Team *team = TeamManager::Instance().GetTeam(m_team_index);
	if (team != nullptr)
	{
		Role *leader_role = team->GetLeader();
		if (leader_role != nullptr)
		{
			EventHandler::Instance().OnPassTeamFb(leader_role);
		}
	}
}

void SpecialTeamTowerDefend::BroadcastToClient(int reason)
{
	static Protocol::SCTeamTowerDefendInfo ttdi;
	ttdi.reason = reason;
	ttdi.life_tower_left_hp = m_life_tower_left_hp;
	ttdi.life_tower_left_maxhp = m_life_tower_left_maxhp;
	ttdi.gongji_uid = m_gongji_uid;
	ttdi.fangyu_uid = m_fangyu_uid;
	ttdi.assist_uid = m_assist_uid;
	ttdi.curr_wave = m_curr_wave_index;
	ttdi.next_wave_refresh_time = m_next_wave_start_time;
	ttdi.clear_wave = m_clear_wave_count;
	ttdi.exp = 0;
	ttdi.score = 0;

	for (int i = 0; i < (int)m_scene->RoleNum(); ++i)
	{
		ttdi.exp = 0;
		ttdi.score = 0;

		Role *temp_role = m_scene->GetRoleByIndex(i);
		if (NULL != temp_role && Obj::OBJ_TYPE_ROLE == temp_role->GetObjType())
		{
			int uid = UidToInt(temp_role->GetUserId());

			for (int j = 0; j < MAX_TEAM_MEMBER_NUM; ++j)
			{
				if (uid == m_personal_info_list[j].uid)
				{
					ttdi.exp = 0;
					ttdi.score = 0;

					UNSTD_STATIC_CHECK(sizeof(m_personal_info_list[j].skill_list) / sizeof(m_personal_info_list[j].skill_list[0]) == sizeof(ttdi.skill_list) / sizeof(ttdi.skill_list[0]));

					for (int k = 0; k < TEAM_TOWERDEFEND_NORMAL_SKILL_COUNT + TEAM_TOWERDEFEND_EXTRA_SKILL_COUNT; ++k)
					{
						ttdi.skill_list[k].skill_id = m_personal_info_list[j].skill_list[k].skill_id;
						ttdi.skill_list[k].skill_level = 1;
						ttdi.skill_list[k].next_can_perform_time = m_personal_info_list[j].skill_list[k].next_can_perform_timestamp;
					}

					break;
				}
			}

			EngineAdapter::Instance().NetSend(temp_role->GetNetId(), (const char *)&ttdi, sizeof(ttdi));
		}
	}
}

void SpecialTeamTowerDefend::NotifyWarning(short warning_type, short percent)
{
	Protocol::SCTowerDefendWarning ttdw;
	ttdw.warning_type = warning_type;
	ttdw.percent = percent;
	m_scene->SendToRole((const char *)&ttdw, sizeof(ttdw));
}

///// 技能相关 //////////////////////////////////////////////////////////////////////////////
void SpecialTeamTowerDefend::InitSkill(FBPersonalInfo &person_info, int attr_type)
{
	int skill_id_list[TEAM_TOWERDEFEND_NORMAL_SKILL_COUNT] = { 0 };
	LOGIC_CONFIG->GetTeamTowerDefendConfig().GetTeamSkillListByAttr(attr_type, skill_id_list);

	UNSTD_STATIC_CHECK(static_cast<int>(sizeof(person_info.skill_list) / sizeof(person_info.skill_list[0])) >= TEAM_TOWERDEFEND_NORMAL_SKILL_COUNT);

	for (int i = 0; i < TEAM_TOWERDEFEND_NORMAL_SKILL_COUNT; ++i)
	{
		person_info.skill_list[i].skill_id = skill_id_list[i];
		person_info.skill_list[i].next_can_perform_timestamp = 0;
	}
}

void SpecialTeamTowerDefend::RecalcAttrAdd(Role *role, bool first_enter)
{
	if (NULL != role)
	{
		int uid = UidToInt(role->GetUserId());

		if (uid == m_gongji_uid)
		{
			for (int i = 0; i < MAX_TEAM_MEMBER_NUM; ++i)
			{
				if (uid == m_personal_info_list[i].uid)
				{
					this->AddGongjiBySkill(role, m_personal_info_list[i]);
					break;
				}
			}
		}
		else if (uid == m_fangyu_uid)
		{
			for (int i = 0; i < MAX_TEAM_MEMBER_NUM; ++i)
			{
				if (uid == m_personal_info_list[i].uid)
				{
					bool recover_hp = (first_enter ? true : false);
					this->AddMaxhpBySkill(role, m_personal_info_list[i], recover_hp, false);
					break;
				}
			}
		}
	}
}

void SpecialTeamTowerDefend::AddGongjiBySkill(Role *role, FBPersonalInfo &person_info)
{
	if (NULL != role)
	{
		SkillInfo *skill_info = person_info.GetNormalSkillInfo(TeamTowerDefendSkill::ADD_GONGJI_SKILL_ID);
		if (NULL != skill_info)
		{
			const TeamTowerDefendSkillCfg::ConfigItem *skill_cfg = LOGIC_CONFIG->GetTeamTowerDefendConfig().GetTeamSkillCfg(TeamTowerDefendSkill::ADD_GONGJI_SKILL_ID);
			if (NULL != skill_cfg)
			{
				int scene_skill_id = GetSkillIDBySceneTypeAndIndex(m_scene->GetSceneType(), TeamTowerDefendSkill::ADD_GONGJI_SKILL_ID);

				int gongji_percent = skill_cfg->param_a;
				Attribute add_gongji = static_cast<Attribute>(role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI) * (gongji_percent / 100.0));

				EffectBuff *effect_gongji = new EffectBuff(INVALID_OBJ_ID, scene_skill_id, 24 * 3600 * 1000, static_cast<char>(100), EffectBase::PRODUCT_METHOD_SCENE_SKILL, EBT_ADD_GONGJI);
				effect_gongji->AddGongJiEffect(add_gongji, gongji_percent);
				effect_gongji->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);
				effect_gongji->SetSave(false);
				effect_gongji->SetClearOnDie(false);
				effect_gongji->SetClientEffectType(EffectBase::ECT_TDT_ADD_GONGJI);

				role->AddEffect(effect_gongji);

				//printf("------ %d,%s Add gongji %d", role->GetUID(), role->GetName(), add_gongji);
			}
		}
	}
}

void SpecialTeamTowerDefend::AddMaxhpBySkill(Role *role, FBPersonalInfo &person_info, bool recover_hp, bool add_hp)
{
	if (NULL == role)
	{
		return;
	}

	SkillInfo *skill_info = person_info.GetNormalSkillInfo(TeamTowerDefendSkill::ADD_MAXHP_FANGYU_SKILL_ID);
	if (NULL == skill_info)
	{
		return;
	}

	const TeamTowerDefendSkillCfg::ConfigItem *skill_cfg = LOGIC_CONFIG->GetTeamTowerDefendConfig().GetTeamSkillCfg(TeamTowerDefendSkill::ADD_MAXHP_FANGYU_SKILL_ID);
	if (NULL == skill_cfg)
	{
		return;
	}

	int scene_skill_id = GetSkillIDBySceneTypeAndIndex(m_scene->GetSceneType(), TeamTowerDefendSkill::ADD_MAXHP_FANGYU_SKILL_ID);

	int hp_percent = skill_cfg->param_a;
	Attribute add_maxhp = static_cast<Attribute>(role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP) * (hp_percent / 100.0f));

	Attribute old_maxhp = role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
	Attribute old_hp = role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);

	// 气血
	EffectBuff *effect_maxhp_fangyu = new EffectBuff(INVALID_OBJ_ID, scene_skill_id, 24 * 3600 * 1000, static_cast<char>(100), EffectBase::PRODUCT_METHOD_SCENE_SKILL);
	effect_maxhp_fangyu->AddAttrEffect(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP, add_maxhp, hp_percent);

	// 防御
	int fangyu_percent = skill_cfg->param_b;
	Attribute add_fangyu = static_cast<Attribute>(role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU) * (fangyu_percent / 100.0f));

	effect_maxhp_fangyu->AddFangYuEffect(add_fangyu, fangyu_percent);

	effect_maxhp_fangyu->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);
	effect_maxhp_fangyu->SetClearOnDie(false);
	effect_maxhp_fangyu->SetSave(false);
	effect_maxhp_fangyu->SetClientEffectType(EffectBase::ECT_TDT_ADD_FANGYU);

	role->AddEffect(effect_maxhp_fangyu);

	Attribute new_maxhp = role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
	Attribute new_hp = role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);

	Attribute hp_add_value = 0;
	if (recover_hp)					// 满血
	{
		hp_add_value = role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
	}
	else if (add_hp)				// hp和maxhp增加相同 --- buff替换的过程中，hp会变化，要找回这部分变化
	{
		hp_add_value = (old_hp + (new_maxhp - old_maxhp)) - new_hp;
	}
	else							// 恢复到下线前hp --- 原理同上
	{
		if (person_info.hp_when_leave > 0)
		{
			hp_add_value = person_info.hp_when_leave - new_hp;
		}
	}

	if (hp_add_value > 0)
	{
		role->ChangeHp(&hp_add_value, true);
	}
}

bool SpecialTeamTowerDefend::PerformSkill(Role *role, int skill_index, const Posi &pos, ObjID target_id)
{
	if (NULL == role) return false;

	int uid = UidToInt(role->GetUserId());

	FBPersonalInfo *fb_info = NULL;

	for (int i = 0; i < MAX_TEAM_MEMBER_NUM; ++i)
	{
		if (uid == m_personal_info_list[i].uid)
		{
			fb_info = &m_personal_info_list[i];
			break;
		}
	}

	if (NULL == fb_info) return false;

	if (skill_index < 0 || skill_index >= TEAM_TOWERDEFEND_NORMAL_SKILL_COUNT + TEAM_TOWERDEFEND_EXTRA_SKILL_COUNT)
	{
		return false;
	}

	SkillInfo *skill_info = &(fb_info->skill_list[skill_index]);

	if (NULL == skill_info || skill_info->Invalid())
	{
		role->NoticeNum(errornum::EN_SKILL_NOT_LEARNT);
		return false;
	}

	const TeamTowerDefendSkillCfg::ConfigItem * skill_cfg = LOGIC_CONFIG->GetTeamTowerDefendConfig().GetTeamSkillCfg(skill_info->skill_id);
	if (NULL == skill_cfg) return false;

	const TeamTowerDefendSkill *skill_base = LOGIC_CONFIG->GetTeamTowerDefendConfig().GetTeamSkillBase(skill_info->skill_id);
	if (NULL == skill_base) return false;

	unsigned int now_second = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	if (now_second < skill_info->next_can_perform_timestamp)
	{
		role->NoticeNum(errornum::EN_ROLE_FIGHT_CD_NOT_ENOUGH);
		return false;
	}

	Character *target_cha = NULL;
	Obj *target_obj = m_scene->GetObj(target_id);
	if (NULL != target_obj && Obj::IsCharactor(target_obj->GetObjType()))
	{
		target_cha = (Character *)target_obj;
	}
	else
	{
		target_cha = role;
	}

	skill_base->Perform(role, 1, target_cha, pos);

	skill_info->next_can_perform_timestamp = static_cast<unsigned int>(now_second + skill_base->GetCoolDown(1));;

	this->SyncSkillInfo(role, skill_info);

	return true;
}

long long SpecialTeamTowerDefend::SpecialRecalcHate(Role *role, int skill_id, long long hate)
{
	UInt16 team_tower_skill_8 = static_cast<UInt16>(GetSkillIDBySceneTypeAndIndex(Scene::SCENE_TYPE_TEAM_TOWERDEFEND, 8));

	if (NULL != role && skill_id == team_tower_skill_8)
	{
		hate = hate * 100;
	}

	return hate;
}

void SpecialTeamTowerDefend::OnPickFallingItem(Role *role, FallingItem *falling_item, ItemDataWrapper *item_wrapper)
{
	if (nullptr == role || nullptr == falling_item)
	{
		return;
	}

	for (int i = 0; i < MAX_TEAM_MEMBER_NUM; i++)
	{
		if (m_personal_info_list[i].uid == role->GetUID())
		{
			ItemConfigData * reward_list = m_personal_info_list[i].reward_list;
			for (int i = 0; i < DROP_ITEM_TYPE_MAX; i++)
			{
				if (reward_list[i].item_id == falling_item->GetItemId() || reward_list[i].item_id == 0)
				{
					reward_list[i].item_id = falling_item->GetItemId();
					reward_list[i].num += falling_item->GetItemNum();
					break;
				}
			}

			//用于客户端掉落统计
			{
				Protocol::SCFBDropInfo fbdi;
				fbdi.get_coin = 0;	//暂时没用的
				fbdi.get_item_count = 0;

				memset(fbdi.drop_items, 0, sizeof(Protocol::SCFBDropInfo::DropItem) * Protocol::SCFBDropInfo::FB_DROP_ITEM_MAX_NUM);
				for (int i = 0; i < DROP_ITEM_TYPE_MAX && i < Protocol::SCFBDropInfo::FB_DROP_ITEM_MAX_NUM; ++i)
				{
					if (reward_list[i].item_id == 0)
					{
						break;
					}

					fbdi.drop_items[i].item_id = reward_list[i].item_id;
					fbdi.drop_items[i].num = static_cast<short>(reward_list[i].num);

					++fbdi.get_item_count;
				}

				int len = sizeof(fbdi) - sizeof(fbdi.drop_items[0]) * (Protocol::SCFBDropInfo::FB_DROP_ITEM_MAX_NUM - fbdi.get_item_count);
				EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&fbdi, len);
			}
		}
	}
}

void SpecialTeamTowerDefend::SyncFBLogicInfo(Role *role)
{
	Protocol::SCFBSceneLogicInfo sli;

	sli.time_out_stamp = m_scene->GetSceneTimeOutStamp();
	sli.scene_type = m_scene->GetSceneType();
	sli.pass_time_s = this->GetPassTime();

	if (nullptr != role)
	{
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&sli, sizeof(sli));
	}
	else
	{
		m_scene->SendToRole((const char *)&sli, sizeof(sli));
	}
}

void SpecialTeamTowerDefend::GmTowave(int wave)
{
	if (wave <= m_curr_wave_index || wave > LOGIC_CONFIG->GetTeamTowerDefendConfig().GetTeamDefendWaveCount() - 3)
	{
		return;
	}
	m_clear_wave_count += (wave - m_curr_wave_index);
	m_curr_wave_index = wave;
	this->BroadcastToClient(Protocol::SCTeamTowerDefendInfo::NOTIFY_REASON_NEXT_WAVE);
	m_last_sync_to_allclient_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	return;
}

void SpecialTeamTowerDefend::FinishBroadcast()
{
	if (m_clear_wave_count < 10)
	{
		return;
	}

	if (m_is_pass)
	{
		m_clear_wave_count = LOGIC_CONFIG->GetTeamTowerDefendConfig().GetTeamDefendWaveCount();
	}

	char temp_role_list[256] = { 0 };
	int total_len = 0;

	for (int i = 0; i < MAX_TEAM_MEMBER_NUM; ++i)
	{
		if (m_personal_info_list[i].uid <= 0)
		{
			continue;
		}

		int tmp_len = SNPRINTF(temp_role_list + total_len, sizeof(temp_role_list) - total_len, "{r;%d;%s;%d} ",
			m_personal_info_list[i].uid, m_personal_info_list[i].role_name, m_personal_info_list[i].camp);
		total_len += tmp_len;
	}

	int str_len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_team_towerdefend_pass_wave,
		temp_role_list, m_clear_wave_count);
	if (str_len > 0)
	{
		World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, str_len, SYS_MSG_CENTER_NOTICE_3);
	}
}

void SpecialTeamTowerDefend::SyncSkillInfo(Role *role, SkillInfo *skill_info)
{
	if (NULL == role) return;

	static Protocol::SCTeamTowerDefendSkill ttds;
	ttds.skill_index = skill_info->skill_id;
	ttds.next_can_perform_time = skill_info->next_can_perform_timestamp;
	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&ttds, sizeof(ttds));
}

void SpecialTeamTowerDefend::SyncAllRoleInfo(Role * role)
{
	if (m_scene->RoleNum() <= 1)
	{
		return;
	}

	Protocol::SCTeamTowerDefendAllRole ttar;
	ttar.role_count = 0;
	memset(ttar.role_list, 0, sizeof(ttar.role_list));

	static int effect[4] = { EffectBase::EFFECT_TYPE_ATTRBUFF,EffectBase::EFFECT_TYPE_ATTRBUFF,EffectBase::EFFECT_TYPE_WUDI_PROTECT, EffectBase::EFFECT_TYPE_ATTRBUFF };
	static int buff[4] = { EBT_ADD_FANGYU, EBT_ADD_GONGJI,EBT_WUDI_PROTECT,EBT_ADD_MULTI_ATTR };

	for (int i = 0; i < MAX_TEAM_MEMBER_NUM; ++i)
	{
		if (0 == m_personal_info_list[i].uid) continue;

		Role *temp = m_scene->GetRoleByUID(m_personal_info_list[i].uid);
		if (nullptr != temp)
		{
			ttar.role_list[ttar.role_count].uid = m_personal_info_list[i].uid;
			ttar.role_list[ttar.role_count].attr_type = m_personal_info_list[i].attr_type;
			F_STRNCPY(ttar.role_list[ttar.role_count].user_name, m_personal_info_list[i].role_name, sizeof(GameName));
			ttar.role_list[ttar.role_count].hp = temp->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
			ttar.role_list[ttar.role_count].max_hp = temp->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
			for (int i = 0; i < 4; ++i)
			{
				if (NULL != temp->GetEffectList()->FindEffectByType(effect[i], buff[i]))
				{
					int count = ttar.role_list[ttar.role_count].buff_count;
					ttar.role_list[ttar.role_count].buff[count] = buff[i];
					ttar.role_list[ttar.role_count].buff_count++;
				}
			}
			ttar.role_count++;
		}
	}
	if (nullptr != role)
	{
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&ttar, sizeof(ttar));
	}
	else
	{
		m_scene->SendToRole((const char*)&ttar, sizeof(ttar));
	}
}
