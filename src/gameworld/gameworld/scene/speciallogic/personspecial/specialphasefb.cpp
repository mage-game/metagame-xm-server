#include "specialphasefb.hpp"

#include "config/logicconfigmanager.hpp"
#include "obj/character/role.h"
#include "obj/character/monster.h"
#include "monster/monsterpool.h"
#include "engineadapter.h"
#include "gamecommon.h"
#include "protocal/msgfight.h"
#include "globalconfig/globalconfig.h"
#include "world.h"
#include "gamelog.h"
#include "servercommon/string/gameworldstr.h"
#include "other/event/eventhandler.hpp"
#include "protocal/msgfb.h"
#include "other/fb/phasefbconfig.hpp"
#include "obj/otherobj/fallingitem.h"
#include "gameworld/item/itempool.h"
#include "npc/transmit/fbmanager.hpp"
#include "task/newtaskmanager.h"
#include "other/fb/rolefbphase.hpp"
#include "ai/monsterai/monsteraibase.hpp"

SpecialPhaseFB::SpecialPhaseFB(Scene *scene)
	: SpecialLogic(scene), m_owner_objid(INVALID_OBJ_ID), m_owner_user_id(INVALID_USER_ID),
	m_is_finish(false), m_is_passed(false),
	m_total_boss_num(0), m_total_allmonster_num(0), m_kill_boss_num(0), m_kill_allmonster_num(0), 
	m_is_cancel_monster_static(false), m_pass_kick_out_time(0)
{
}

SpecialPhaseFB::~SpecialPhaseFB()
{	
}

void SpecialPhaseFB::Update(unsigned long interval, time_t now_second)
{
	if (m_is_finish)
	{
		if (m_pass_kick_out_time > 0 && now_second >= m_pass_kick_out_time)
		{
			this->DelayKickOutAllRole(CHANGE_SCENE_REASON_FB_FINISH);
		}
		return;
	}
	SpecialLogic::Update(interval, now_second);
}

void SpecialPhaseFB::OnRoleEnterScene(Role *role)
{
	if (m_is_finish)
	{
		this->DelayKickOutRole(role, CHANGE_SCENE_REASON_FB_FINISH);
	}
	else
	{
		bool is_first_enter = (INVALID_USER_ID == m_owner_user_id);
		m_owner_user_id = role->GetUserId(); 
		m_owner_objid = role->GetId();

		if(is_first_enter)
		{
			int fb_index = role->GetRoleFBPhase()->GetCurrFbIndex();
			int fb_level = role->GetRoleFBPhase()->GetCurrFbLevel();
			const PhaseLevelCfg::ConfigItem *level_cfg = LOGIC_CONFIG->GetPhaseFBConfig().GetLevelCfg(fb_index, fb_level);
			if (NULL != level_cfg)
			{
				m_total_boss_num = 0;
				m_total_allmonster_num = level_cfg->monster_count;
				m_kill_boss_num = 0;
				m_kill_allmonster_num = 0;
				//role->GetRoleFBPhase()->AddFBTodayTimes(level_cfg->fb_index);	// 进入消耗次数
				for (int i = 0; i < level_cfg->monster_count && i < PhaseLevelCfg::MONSTER_COUNT_MAX; ++i)
				{
					int add_hp = 0;
					//int role_level = role->GetLevel();
					//const MonsterInitParam *monster_param = MONSTERPOOL->GetMonsterParam(level_cfg->monster_id_list[i]);
					//if (NULL != monster_param)
					//{
					//	add_hp = static_cast<int>(monster_param->hp * (0.0002f * role_level * role_level - 0.0385f * role_level + 2.275));
					//	if (add_hp < 0)
					//	{
					//		add_hp = 0;
					//	}
					//}

					Monster *monster = MONSTERPOOL->CreateMonster(level_cfg->monster_id_list[i], m_scene, level_cfg->monster_pos_list[i], add_hp);
					if (NULL != monster && monster->IsBossMonster())
					{
						m_total_boss_num++;
					}
				}
			}
		}

		this->SyncFBSceneLogicInfo();

		EventHandler::Instance().OnEnterPhaseFB(role);
	}
}

void SpecialPhaseFB::OnLeaveFB(Role *role)
{
	if (role == NULL) return;

	if (!m_is_finish)
	{
		m_is_finish = true;
		this->KillAllMonster();
		this->SyncFBSceneLogicInfo();
	}
}

bool SpecialPhaseFB::SpecialRelive(Role *role, int realive_type)
{
	if (Protocol::CSRoleReAliveReq::REALIVE_TYPE_BACK_HOME == realive_type)
	{
		Posi realive_pos;
		int target_scene_id = m_scene->GetTownPoint(role, &realive_pos);
		if (target_scene_id == m_scene->GetSceneID())
		{
			if (role->ReAlive(10, 10, realive_pos)) 
			{
				return true;
			}
		}
	}	

	return false;
}

void SpecialPhaseFB::OnCharacterDie(Character *dead, ObjID killer_id)
{
	if (NULL == dead)
	{
		return;
	}

	Role *role = GetOwnerRole();
	if (NULL != role)
	{
		if(!role->IsAlive())
		{
			this->SyncFBSceneLogicInfo();
			return;
		}
	}

	if (Obj::OBJ_TYPE_MONSTER == dead->GetObjType() && killer_id == m_owner_objid)
	{
		Monster *monster = (Monster *)dead;
		if (monster->IsBossMonster())
		{
			m_kill_boss_num++;
		}
		m_kill_allmonster_num++;

		if (m_kill_allmonster_num >= m_total_allmonster_num)
		{
			m_is_passed = true;
			m_is_finish = true;

			this->OnFinish();
		}

		this->SyncFBSceneLogicInfo();
	}
}

bool SpecialPhaseFB::OnTimeout()
{
	if (!m_is_finish)
	{
		m_is_finish = true;
		this->KillAllMonster();
		this->SyncFBSceneLogicInfo();
	}

	return m_is_passed;
}

void SpecialPhaseFB::OnAddObj(Obj *obj)
{
	if (NULL == obj) return;

	if (Obj::OBJ_TYPE_MONSTER == obj->GetObjType())
	{
		Monster * monster = (Monster*)obj;

		if (!m_is_cancel_monster_static)
		{
			MonsterAIBase *monster_ai = monster->GetMonsterAI();
			if (NULL != monster_ai)
			{
				unsigned now_sec = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				monster_ai->SetIsStatic(true);
				monster_ai->SetStaticEndTime(now_sec + 10);
			}
		}
	}
}

void SpecialPhaseFB::SpecialCancelMonsterStatic()
{
	if (m_is_cancel_monster_static) return;

	m_is_cancel_monster_static = true;
	UInt32 monster_num = m_scene->MonsterNum();
	for (unsigned int i = 0; i < monster_num; i++)
	{
		Monster *monster = m_scene->GetMonsterByIndex(i);
		if (NULL != monster)
		{
			MonsterAIBase *monster_ai = monster->GetMonsterAI();
			if (NULL != monster_ai)
			{
				monster_ai->SetIsStatic(false);
				monster_ai->SetStaticEndTime(0);
			}
		}
	}
}

Role * SpecialPhaseFB::GetOwnerRole()
{
	Obj *obj = m_scene->GetObj(m_owner_objid);
	if (NULL != obj && Obj::OBJ_TYPE_ROLE == obj->GetObjType())
	{
		Role *role = (Role*)obj;
		if (m_owner_user_id == role->GetUserId()) 
		{
			return role;
		}
	}

	return NULL;
}

void SpecialPhaseFB::OnFinish()
{
	Role *role = this->GetOwnerRole();
	if (NULL != role)
	{
		role->Recover(); 

		int fb_index = role->GetRoleFBPhase()->GetCurrFbIndex();
		int fb_level = role->GetRoleFBPhase()->GetCurrFbLevel();
		const PhaseLevelCfg::ConfigItem *level_cfg = LOGIC_CONFIG->GetPhaseFBConfig().GetLevelCfg(fb_index, fb_level);
		if (NULL != level_cfg)
		{
			role->GetRoleFBPhase()->OnPassLevel(level_cfg->fb_index, level_cfg->fb_level);
			EventHandler::Instance().OnPassPhaseFB(role, level_cfg->fb_type);
			//role->GetRoleFBPhase()->DecFBTodayTimes(level_cfg->fb_index); // 首通返还次数
			role->GetTaskManager()->OnPassFB(FBManager::FBCT_PHASE);

			ROLE_LOG_QUICK6(LOG_TYPE_PHASE_FB, role, role->GetLevel(), level_cfg->fb_index, NULL, NULL);
		}

		{
			// 日志
			int use_time = static_cast<int>(EngineAdapter::Instance().Time() - m_scene->GetCreateTime());

			gamelog::g_log_fb.printf(LL_INFO, "PhaseFB::Onfinish user[%d, %s], level[%d], index[%d], use_time[%d]", 
				role->GetUID(), role->GetName(), role->GetLevel(), role->GetRoleFBPhase()->GetCurrFbIndex(), use_time);
		}
		if (m_is_passed)
		{
			m_pass_kick_out_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + 9;
		}
		else
		{
			this->DelayKickOutRole(role, CHANGE_SCENE_REASON_FB_FINISH);
		}
	}
}

void SpecialPhaseFB::SyncFBSceneLogicInfo(bool is_active_leave_fb)
{
	Role *role = this->GetOwnerRole();
	if (NULL != role)
	{
		static Protocol::SCFBSceneLogicInfo cmd;
		cmd.reset();

		cmd.time_out_stamp = m_scene->GetSceneTimeOutStamp();
		cmd.scene_type = this->GetSceneType();

		cmd.is_finish = m_is_finish ? 1 : 0;
		cmd.is_pass = m_is_passed ? 1 : 0;
		cmd.is_active_leave_fb = is_active_leave_fb ? 1 : 0;

		cmd.total_boss_num = m_total_boss_num;
		cmd.total_allmonster_num = m_total_allmonster_num;
		cmd.kill_boss_num = m_kill_boss_num;
		cmd.kill_allmonster_num = m_kill_allmonster_num;

		cmd.pass_time_s = this->GetPassTime();

		cmd.param1 = role->GetRoleFBPhase()->GetCurrFbIndex();
		cmd.param2 = role->GetRoleFBPhase()->GetCurrFbLevel();

		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&cmd, sizeof(cmd));
	}
}
