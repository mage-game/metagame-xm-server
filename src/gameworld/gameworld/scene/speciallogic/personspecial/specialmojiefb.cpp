#include "specialmojiefb.hpp"
#include "config/logicconfigmanager.hpp"
#include "obj/character/role.h"
#include "obj/character/monster.h"
#include "monster/monsterpool.h"
#include "engineadapter.h"
#include "gamecommon.h"
#include "protocal/msgfight.h"
#include "obj/otherobj/bigcoinobj.hpp"
#include "effect/skilleffect/effectbuff.hpp"
#include "effect/skilleffect/effectonce.hpp"
#include "other/event/eventhandler.hpp"
#include "globalconfig/globalconfig.h"
#include "protocal/msgfb.h"
#include "obj/otherobj/fallingitem.h"
#include "gameworld/item/itempool.h"
#include "other/fb/mojiefbconfig.hpp"
#include "other/fb/rolemojiefb.hpp"

SpecialMoJieFB::SpecialMoJieFB(Scene *scene)
: SpecialLogic(scene), m_owner_objid(INVALID_OBJ_ID), m_owner_user_id(INVALID_USER_ID),
m_is_finish(false), m_is_passed(false), m_fb_id(0)
{

}

SpecialMoJieFB::~SpecialMoJieFB()
{

}

void SpecialMoJieFB::OnRoleEnterScene(Role *role)
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
		m_fb_id = role->GetFBMojie()->GetMojieFbParam()->mojie_human_pian_task_mark + 1;

		if(is_first_enter)
		{
			const HumanPianCfg *cfg = LOGIC_CONFIG->GetMoJieFBConfig().GetHumanPianCfg(m_fb_id);
			if (NULL != cfg)
			{
				MONSTERPOOL->CreateMonster(cfg->boss_id, m_scene, cfg->born_pos);
			}
		}

		this->SyncFBSceneLogicInfo();

	}
}

void SpecialMoJieFB::OnCharacterDie(Character *dead, ObjID killer_id)
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
	
	const HumanPianCfg *cfg = LOGIC_CONFIG->GetMoJieFBConfig().GetHumanPianCfg(m_fb_id);
	
	if(NULL != cfg)
	{
		if (Obj::OBJ_TYPE_MONSTER == dead->GetObjType() && killer_id == m_owner_objid)
		{
			Monster *monster = (Monster *)dead;

			if (monster->GetId() == cfg->boss_id)
			{
				m_is_passed = true;
				m_is_finish = true;

				this->OnFinish();
			}

			this->SyncFBSceneLogicInfo();
		}
	}
}

Role * SpecialMoJieFB::GetOwnerRole()
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

void SpecialMoJieFB::OnFinish()
{
	Role *role = this->GetOwnerRole();
	if (NULL != role)
	{
		role->Recover();
		role->GetFBMojie()->OnPassRenLeiFb(m_fb_id);
		
		{
			// »’÷æ
			int use_time = static_cast<int>(EngineAdapter::Instance().Time() - m_scene->GetCreateTime());

			gamelog::g_log_fb.printf(LL_INFO, "MoJieFB::Onfinish user[%d, %s], level[%d], index[%d], use_time[%d]", 
				role->GetUID(), role->GetName(), role->GetLevel(), m_fb_id, use_time);
		}
	}
}

void SpecialMoJieFB::SyncFBSceneLogicInfo(bool is_active_leave_fb)
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

		cmd.total_boss_num = 0;
		cmd.total_allmonster_num = 0;
		cmd.kill_boss_num = 0;
		cmd.kill_allmonster_num = 0;

		cmd.pass_time_s = this->GetPassTime();

		cmd.param1 = m_fb_id;

		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&cmd, sizeof(cmd));
	}
}