#include "specialxinmofb.hpp"
#include "servercommon/servercommon.h"
#include "obj/character/role.h"
#include "config/logicconfigmanager.hpp"
#include "obj/character/monster.h"
#include "obj/obj.h"
#include "obj/otherobj/fallingitem.h"
#include "protocal/msgfb.h"
#include "monster/monsterpool.h"
#include "other/event/eventhandler.hpp"
#include "other/fb/xinmofbconfig.hpp"
#include "npc/transmit/fbmanager.hpp"
#include "other/rolezhuanzhi/rolezhuanzhi.hpp"

SpecialXinMoFb::SpecialXinMoFb(Scene *scene)
	: SpecialLogic(scene), m_owner_objid(INVALID_OBJ_ID), m_owner_user_id(INVALID_USER_ID), m_is_finish(false), m_is_pass(false), 
	m_is_flushboss(false), m_kick_out_timestamp(0)
{

}

SpecialXinMoFb::~SpecialXinMoFb()
{

}

void SpecialXinMoFb::Update(unsigned long interval, time_t now_second)
{
	SpecialLogic::Update(interval, now_second);

	if (m_kick_out_timestamp > 0 && now_second > m_kick_out_timestamp)
	{
		Role *role = this->GetOwnerRole(m_owner_objid);
		if (role != nullptr)
		{
			this->DelayKickOutRole(role, CHANGE_SCENE_REASON_FB_FINISH);
		}
	}
}

void SpecialXinMoFb::OnRoleEnterScene(Role *role)
{
	m_owner_user_id = role->GetUserId();
	m_owner_objid = role->GetId();

	if (!m_is_flushboss)
	{
		this->FlushMonster();

		m_is_flushboss = true;
	}

	// 进入自动满血
	role->Recover(true, true, true);

	this->SyncFBSceneLogicInfo();
}

bool SpecialXinMoFb::FlushMonster()
{
	Role *role = this->GetOwnerRole(m_owner_objid);
	if (NULL == role)
	{
		return false;
	}

	int index = 0;
	//if (role->GetProfLevel() == CANGLONG_LIGHT_UP_PROF_LEVEL_LIMIT)
	//{
	//	int canglong_lv = role->GetRoleZhuanZhi()->GetCangLongLevel();
	//	if (canglong_lv > 0 && MAX_CANGLONG_LEVEL_PER_GRADE > 0)
	//	{
	//		index = static_cast<int>((canglong_lv - 1) / MAX_CANGLONG_LEVEL_PER_GRADE);
	//	}
	//}

	const ZhuanzhiFbCfg *cfg = LOGIC_CONFIG->GetZhuanzhiCfg().GetZhuanzhiFbCfg(role->GetProfLevel() + 1, index);
	if (NULL == cfg || cfg->boss_id <= 0)
	{
		return false;
	}

	Monster *monster = MONSTERPOOL->CreateMonster(cfg->boss_id, m_scene, cfg->boss_pos);
	if (NULL == monster)
	{
		return false;
	}

	return true;
}

void SpecialXinMoFb::OnLeaveFB(Role *role)
{
	if (!m_is_finish)
	{
		if (role->GetUserId() == m_owner_user_id)
		{
			this->OnFBFinish(false);
			this->SyncFBSceneLogicInfo(true);
		}
	}
}

void SpecialXinMoFb::OnCharacterDie(Character *dead, ObjID killer_id)
{
	if (NULL == dead) return;

	if (Obj::OBJ_TYPE_ROLE == dead->GetObjType())
	{
		Role *role = (Role *)dead;
		if (role->GetId() == m_owner_objid)
		{
			this->OnFBFinish(false);
			this->SyncFBSceneLogicInfo(false);
		}
	}
	else if (Obj::OBJ_TYPE_MONSTER == dead->GetObjType())
	{
		Monster *boss = (Monster *)dead;

		Role *role = this->GetOwnerRole(m_owner_objid);
		if (NULL != role)
		{
			int index = 0;
			/*if (role->GetProfLevel() == CANGLONG_LIGHT_UP_PROF_LEVEL_LIMIT)
			{
				int canglong_lv = role->GetRoleZhuanZhi()->GetCangLongLevel();
				if (canglong_lv > 0 && MAX_CANGLONG_LEVEL_PER_GRADE > 0)
				{
					index = static_cast<int>((canglong_lv - 1) / MAX_CANGLONG_LEVEL_PER_GRADE);
				}
			}*/

			const ZhuanzhiFbCfg *cfg = LOGIC_CONFIG->GetZhuanzhiCfg().GetZhuanzhiFbCfg(role->GetProfLevel() + 1, index);
			if (NULL != cfg)
			{
				if (boss->GetMonsterId() == cfg->boss_id)
				{
					this->OnFBFinish(true);
					this->SyncFBSceneLogicInfo(false);
				}
			}
		}
	}
}

void SpecialXinMoFb::OnFBFinish(bool is_passed)
{
	if (m_is_finish) return;

	m_is_finish = true;
	m_is_pass = is_passed;

	this->KillAllMonster();

	this->SyncFBSceneLogicInfo();

	Role *role = this->GetOwnerRole(m_owner_objid);
	if (NULL != role && is_passed)
	{
		EventHandler::Instance().OnPassXinMoFb(role, role->GetProfLevel() + 1);
	}
	
	if (!is_passed)
	{
		m_kick_out_timestamp = EngineAdapter::Instance().Time();
	}
	else
	{
		const RoleZhuanZhiOtherCfg &other_cfg = LOGIC_CONFIG->GetZhuanzhiCfg().GetRoleZhuanZhiOtherCfg();
		m_kick_out_timestamp = EngineAdapter::Instance().Time() + (time_t)other_cfg.fb_kick_time_s;
	}
}

Role * SpecialXinMoFb::GetOwnerRole(ObjID obj_id)
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

void SpecialXinMoFb::SyncFBSceneLogicInfo(bool is_active_leave_fb)
{
	Role *role = this->GetOwnerRole(m_owner_objid);
	if (NULL != role)
	{
		static Protocol::SCFBSceneLogicInfo cmd;
		cmd.reset();

		cmd.time_out_stamp = m_scene->GetSceneTimeOutStamp();
		cmd.scene_type = this->GetSceneType();

		cmd.is_finish = m_is_finish ? 1 : 0;
		cmd.is_pass = m_is_pass ? 1 : 0;
		cmd.is_active_leave_fb = is_active_leave_fb ? 1 : 0;

		cmd.total_boss_num = 1;
		cmd.total_allmonster_num = 0;
		cmd.kill_boss_num = this->GetKillBoss();
		cmd.kill_allmonster_num = this->GetKillAllMonster();

		cmd.pass_time_s = this->GetPassTime();
		cmd.coin = 0;
		cmd.exp = 0;

		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&cmd, sizeof(cmd));
	}
}