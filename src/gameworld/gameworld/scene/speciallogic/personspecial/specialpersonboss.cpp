#include "specialpersonboss.hpp"
#include "servercommon/servercommon.h"
#include "obj/character/role.h"
#include "config/logicconfigmanager.hpp"
#include "obj/character/monster.h"
#include "obj/obj.h"
#include "protocal/msgfb.h"
#include "monster/monsterpool.h"
#include "other/event/eventhandler.hpp"
#include "npc/transmit/fbmanager.hpp"
#include "config/personbossconfig.hpp"
#include "obj/otherobj/fallingitem.h"
#include "other/daycounter/daycounter.hpp"
#include "other/fb/rolepersonbossfb.hpp"

SpecialPersonBoss::SpecialPersonBoss(Scene *scene)
: SpecialLogic(scene), m_owner_objid(INVALID_OBJ_ID), m_owner_user_id(INVALID_USER_ID), m_is_finish(false), m_is_pass(false), m_is_flushboss(false), m_flush_boss_timestamp(0), m_kick_out_timestamp(0)
{

}

SpecialPersonBoss::~SpecialPersonBoss()
{

}

void SpecialPersonBoss::Update(unsigned long interval, time_t now_second)
{
	SpecialLogic::Update(interval, now_second);

	if (!m_is_flushboss && now_second > m_flush_boss_timestamp)
	{
		if (this->FlushMonster())
		{
			m_is_flushboss = true;
		}
	}

	if (m_kick_out_timestamp > 0 && now_second > m_kick_out_timestamp)
	{
		this->DelayKickOutAllRole(CHANGE_SCENE_REASON_FB_FINISH);
	}
}

void SpecialPersonBoss::OnRoleEnterScene(Role *role)
{
	m_owner_user_id = role->GetUserId();
	m_owner_objid = role->GetId();

	if (!m_is_flushboss)
	{
		m_flush_boss_timestamp = EngineAdapter::Instance().Time() + 5;
	}

	// 进入自动满血
	role->Recover(true, true, true);

	this->SyncFBSceneLogicInfo();
}

bool SpecialPersonBoss::FlushMonster()
{
	Role *role = this->GetOwnerRole(m_owner_objid);
	if (NULL == role)
	{
		return false;
	}

	int prof = role->GetBaseProf();
	if (prof < 0 || prof >= PROF_TYPE_PROF_NO_LIMIT)
	{
		return false;
	}

	const PersonBossCfgItem *cfg = LOGIC_CONFIG->GetPersonBossConfig().GetBossSceneCfgBySceneId(m_scene->GetSceneID());
	if (NULL == cfg || cfg->boss_id[prof] <= 0)
	{
		return false;
	}

	Monster *monster = MONSTERPOOL->CreateMonster(cfg->boss_id[prof], m_scene, cfg->flush_pos);
	if (NULL == monster)
	{
		return false;
	}

	return true;
}

void SpecialPersonBoss::OnLeaveFB(Role *role)
{
	if (!m_is_finish)
	{
		if (role->GetUserId() == m_owner_user_id)
		{
			this->OnFBFinish(false);
			this->SyncFBSceneLogicInfo(true);
		}
	}
	else if (m_is_finish && m_is_pass)
	{
		this->AutoPickFallingItem(role);
	}
}

void SpecialPersonBoss::OnCharacterDie(Character *dead, ObjID killer_id)
{
	if (NULL == dead) return;
	
	if (Obj::OBJ_TYPE_MONSTER == dead->GetObjType())
	{
		Monster *boss = (Monster *)dead;

		Role *role = this->GetOwnerRole(m_owner_objid);
		if (NULL != role)
		{
			int prof = role->GetBaseProf();
			const PersonBossCfgItem *cfg = LOGIC_CONFIG->GetPersonBossConfig().GetBossSceneCfgBySceneId(m_scene->GetSceneID());
			if (NULL != cfg && prof >= 0 && prof < PROF_TYPE_PROF_NO_LIMIT)
			{
				if (boss->GetMonsterId() == cfg->boss_id[prof])
				{
					this->OnFBFinish(true);
					this->SyncFBSceneLogicInfo(false);
					role->GetRolePersonBossFb()->AddTodayEnterTimesByLayer(cfg->layer);
				}
			}
		}
	}
}

bool SpecialPersonBoss::OnTimeout()
{
	this->DelayKickOutAllRole(CHANGE_SCENE_REASON_FB_FINISH);

	return true;
}

void SpecialPersonBoss::OnFBFinish(bool is_passed)
{
	if (m_is_finish) return;

	m_is_finish = true;
	m_is_pass = is_passed;

	m_kick_out_timestamp = EngineAdapter::Instance().Time() + 30;

	this->KillAllMonster();

	this->SyncFBSceneLogicInfo();
}

Role * SpecialPersonBoss::GetOwnerRole(ObjID obj_id)
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

void SpecialPersonBoss::SyncFBSceneLogicInfo(bool is_active_leave_fb)
{
	Role *role = this->GetOwnerRole(m_owner_objid);
	if (NULL != role)
	{
		static Protocol::SCFBSceneLogicInfo cmd;
		cmd.reset();

		cmd.time_out_stamp = m_scene->GetSceneTimeOutStamp();
		cmd.flush_timestamp = static_cast<unsigned int>(m_flush_boss_timestamp);
		cmd.kick_timestamp = (m_kick_out_timestamp > 0 && m_kick_out_timestamp < m_scene->GetSceneTimeOutStamp()) ? static_cast<unsigned int>(m_kick_out_timestamp) : m_scene->GetSceneTimeOutStamp();
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
