#include "specialvipfb.hpp"
#include "obj/character/role.h"
#include "engineadapter.h"
#include "config/logicconfigmanager.hpp"
#include "other/fb/vipfbconfig.hpp"
#include "other/event/eventhandler.hpp"
#include "other/fb/patafbconfig.hpp"
#include "protocal/msgfight.h"
#include "protocal/msgfb.h"
#include "npc/transmit/fbmanager.hpp"
#include "task/newtaskmanager.h"
#include "other/fb/rolevipfb.hpp"
#include "global/bossfamily/bossfamilyconfig.hpp"
#include "protocal/msgfight.h"
#include "servercommon/serverconfig/crossconfig.hpp"
#include "scene/scenemanager.h"
#include "world.h"
#include "global/bossfamily/bossfamilymanager.hpp"

SpecialVipFb::SpecialVipFb(Scene *scene)
	: SpecialLogic(scene), m_owner_objid(INVALID_OBJ_ID), m_owner_user_id(INVALID_USER_ID), 
	m_is_finish(false), m_is_pass(false), m_monster_id(0)
{
}

SpecialVipFb::~SpecialVipFb()
{
}

void SpecialVipFb::Update(unsigned long interval, time_t now_second)
{
	SpecialLogic::Update(interval, now_second);
	// 不是在跨服里，但是在跨服的场景id
	if (m_scene != nullptr && LOGIC_CONFIG->GetBossFaimlyConfig().IsCrossVipBossScene(m_scene->GetSceneID()))
	{
		if (!CrossConfig::Instance().IsHiddenServer() && m_scene->RoleNum() > 0)
		{
			this->DelayKickOutAllRole();
		}
	}
}

bool SpecialVipFb::SpecialRelive(Role *role, int realive_type)
{
	if (Protocol::CSRoleReAliveReq::REALIVE_TYPE_HERE_GOLD == realive_type)
	{
		return false;
	}
	const BossFamilyEnterConditionCfg * enter_condition_cfg = LOGIC_CONFIG->GetBossFaimlyConfig().GetEnterConditionInfo(role, m_scene->GetSceneID());
	if (nullptr != enter_condition_cfg)
	{
		role->ReAlive(50, 100, enter_condition_cfg->enter_pos, true);
		return true;
	}
	return false;
}

void SpecialVipFb::OnRoleLeaveScene(Role *role, bool is_logout)
{
	if (CrossConfig::Instance().IsHiddenServer())
	{
		this->AutoPickFallingItem(role);
	}
}

void SpecialVipFb::OnRoleEnterScene(Role *role)
{
	BossFamilyManager::Instance().SendFamilyBossInfo(role, m_scene->GetSceneID());
}

Role * SpecialVipFb::GetOwnerRole()
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

void SpecialVipFb::OnFinish(bool is_pass)
{
	if (!m_is_finish)
	{
		m_is_finish = true;
		m_is_pass = is_pass;

		Role *role = this->GetOwnerRole();
		if (NULL != role)
		{
			this->NotifyBossHPPercent(m_monster_id);

			if (m_is_pass)
			{
				const VipLevelConfig *level_cfg = LOGIC_CONFIG->GetVipFbCfg().GetLevelCfgByMonsterID(m_monster_id);
				if (NULL != level_cfg)
				{
					role->Recover();
					role->GetRoleVipFB()->OnFinish(level_cfg->level);
					role->GetTaskManager()->OnPassFB(FBManager::FBCT_VIPFB);
				}
			}
		}

		this->SyncFBSceneLogicInfo();
	}
}

void SpecialVipFb::SyncFBSceneLogicInfo(bool is_active_leave_fb)
{
	Role *role = this->GetOwnerRole();
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
		cmd.total_allmonster_num = 1;
		cmd.kill_boss_num = m_is_pass ? 1 : 0;
		cmd.kill_allmonster_num = m_is_pass ? 1 : 0;

		cmd.pass_time_s = this->GetPassTime();

		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&cmd, sizeof(cmd));
	}
}