#include "specialstory.hpp"

#include "config/logicconfigmanager.hpp"
#include "obj/character/role.h"
#include "obj/character/monster.h"
#include "monster/monsterpool.h"
#include "engineadapter.h"
#include "gamecommon.h"
#include "gamelog.h"
#include "protocal/msgfight.h"
#include "obj/otherobj/bigcoinobj.hpp"
#include "effect/skilleffect/effectbuff.hpp"
#include "effect/skilleffect/effectonce.hpp"
#include "other/event/eventhandler.hpp"
#include "globalconfig/globalconfig.h"
#include "protocal/msgfb.h"
#include "obj/otherobj/fallingitem.h"
#include "gameworld/item/itempool.h"
#include "other/fb/storyfbconfig.hpp"
#include "other/fb/rolestoryfb.hpp"

SpecialStoryFB::SpecialStoryFB(Scene *scene)
	: SpecialLogic(scene), m_owner_objid(INVALID_OBJ_ID), m_owner_user_id(INVALID_USER_ID),
	m_is_finish(false), m_is_passed(false),
	m_total_boss_num(0), m_total_allmonster_num(0), m_kill_boss_num(0), m_kill_allmonster_num(0)
{

}

SpecialStoryFB::~SpecialStoryFB()
{

}

void SpecialStoryFB::OnRoleEnterScene(Role *role)
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
			const StoryFBLevelCfg::ConfigItem *level_cfg = LOGIC_CONFIG->GetStoryFBConfig().GetLevelCfg(role->GetFBStory()->GetCurrFbIndex());
			if (NULL != level_cfg)
			{
				m_total_boss_num = 0;
				m_total_allmonster_num = level_cfg->monster_count;
				m_kill_boss_num = 0;
				m_kill_allmonster_num = 0;

				for (int i = 0; i < level_cfg->monster_count && i < StoryFBLevelCfg::MONSTER_COUNT_MAX; ++i)
				{
					Monster *monster = MONSTERPOOL->CreateMonster(level_cfg->monster_id_list[i], m_scene, level_cfg->monster_pos_list[i]);
					if (NULL != monster && monster->IsBossMonster())
					{
						m_total_boss_num++;
					}
				}
			}
		}

		this->SyncFBSceneLogicInfo();

		EventHandler::Instance().OnEnterFBStory(role);
	}
}

bool SpecialStoryFB::SpecialRelive(Role *role, int realive_type)
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

void SpecialStoryFB::OnCharacterDie(Character *dead, ObjID killer_id)
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

bool SpecialStoryFB::OnTimeout()
{
	if (!m_is_finish)
	{
		m_is_finish = true;
		this->KillAllMonster();
		this->SyncFBSceneLogicInfo();
	}

	return m_is_passed;
}

Role * SpecialStoryFB::GetOwnerRole()
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

void SpecialStoryFB::OnFinish()
{
	Role *role = this->GetOwnerRole();
	if (NULL != role)
	{
		role->Recover();

		const StoryFBLevelCfg::ConfigItem *level_cfg = LOGIC_CONFIG->GetStoryFBConfig().GetLevelCfg(role->GetFBStory()->GetCurrFbIndex());
		if (NULL != level_cfg)
		{
			role->GetFBStory()->OnPassLevel(level_cfg->fb_index);
			EventHandler::Instance().OnPassStoryFB(role, level_cfg->fb_index);
		}

		{
			// »’÷æ
			int use_time = static_cast<int>(EngineAdapter::Instance().Time() - m_scene->GetCreateTime());

			gamelog::g_log_fb.printf(LL_INFO, "StoryFBFB::Onfinish user[%d, %s], level[%d], index[%d], use_time[%d]", 
				role->GetUID(), role->GetName(), role->GetLevel(), role->GetFBStory()->GetCurrFbIndex(), use_time);
		}
	}
}

void SpecialStoryFB::SyncFBSceneLogicInfo(bool is_active_leave_fb)
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

		cmd.param1 = role->GetFBStory()->GetCurrFbIndex();

		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&cmd, sizeof(cmd));
	}
}
