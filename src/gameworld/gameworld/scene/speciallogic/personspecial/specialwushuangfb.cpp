#include "specialwushuangfb.hpp"
#include "servercommon/qingyuandef.hpp"
#include "other/qingyuan/qingyuanconfig.hpp"
#include "engineadapter.h"
#include "obj/character/role.h"
#include "config/logicconfigmanager.hpp"
#include "monster/monsterpool.h"
#include "world.h"
#include "protocal/msgwushuang.h"
#include "servercommon/errornum.h"

void SpecialWushuangFB::OnReqNextLevel()
{
// 	if (!m_is_finish)
// 	{
// 		return;
// 	}
// 
// 	if (!this->IsAllMonsterDead())
// 	{
// 		return;
// 	}
// 
// 	Role *role = GetOwnerRole();
// 	if (NULL == role)
// 	{
// 		return;
// 	}
// 
// 	int pass_level = role->GetWushuang()->GetFBPassLevel();
// 	const WushuangFBLevelCfg *wushuang_cfg = LOGIC_CONFIG->GetWushuangCfg().GetFBLevelCfg(pass_level + 1);
// 	if (NULL == wushuang_cfg)
// 	{
// 		return;
// 	}
// 
// 	if (role->GetLevel() < wushuang_cfg->need_role_level)
// 	{
// 		role->NoticeNum(errornum::EN_WUSHUANG_FB_NOT_ENOUGH_LEVEL);
// 		return;
// 	}
// 
// 	if (!m_scene->ResetScene(role))
// 	{
// 		return;
// 	}
// 
// 	if (NULL != MONSTERPOOL->CreateMonster(wushuang_cfg->monster_id, this->GetScene(), wushuang_cfg->monster_posistion))
// 	{
// 		m_is_finish = false;
// 		m_is_pass = false;
// 		m_kick_out_timestamp = 0;
// 	}
}

void SpecialWushuangFB::OnFBFinish(bool is_passed)
{
// 	if (m_is_finish) return;
// 
// 	m_is_finish = true;
// 	m_is_pass = is_passed;
// 
// 	Role *role = this->GetOwnerRole();
// 	if (NULL != role)
// 	{
// 		int notify_reason = 0;
// 
// 		if (is_passed)
// 		{
// 			m_kick_out_timestamp = EngineAdapter::Instance().Time() + 10;
// 			notify_reason = Protocol::WUSHUANG_FB_KICKOUT_REASON_PASSED;
// 			role->GetWushuang()->OnPassFB();
// 		}
// 		else
// 		{
// 			m_kick_out_timestamp = 0;
// 			notify_reason = Protocol::WUSHUANG_FB_KICKOUT_REASON_TIME_UP;
// 		}
// 
// 		static Protocol::SCWushuangFBKickOutNotify sc_wsfbkn;
// 		sc_wsfbkn.notify_reason = notify_reason;
// 		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&sc_wsfbkn, sizeof(sc_wsfbkn));
// 	}
}

void SpecialWushuangFB::Update(unsigned long interval, time_t now_second)
{
//	SpecialLogic::Update(interval, now_second);

// 	if (!m_is_finish)
// 	{
// 		this->CheckSceneMonsterStatus(interval, now_second);
// 	}
// 	else
// 	{
// 		if (0 != m_kick_out_timestamp && now_second >= m_kick_out_timestamp)
// 		{
// 			m_kick_out_timestamp = 0;
// 
// 			Role *role = GetOwnerRole();
// 			if (NULL != role)
// 			{
// 				this->DelayKickOutRole(GetOwnerRole());
// 			}
// 		}
// 	}
}

bool SpecialWushuangFB::OnTimeout()
{
	this->OnFBFinish(false);

	return m_is_pass;
}

void SpecialWushuangFB::OnRoleEnterScene(Role *role)
{
// 	if (NULL == role)
// 	{
// 		return;
// 	}
// 
// 	if (m_is_finish)
// 	{
// 		this->DelayKickOutRole(role);
// 		return;
// 	}
// 
// 	bool is_first_enter = (INVALID_USER_ID == m_owner_user_id);
// 	if (is_first_enter)
// 	{
// 		int pass_level = role->GetWushuang()->GetFBPassLevel();
// 		const WushuangFBLevelCfg *wushuang_fb_cfg = LOGIC_CONFIG->GetWushuangCfg().GetFBLevelCfg(pass_level + 1);
// 		if (NULL != wushuang_fb_cfg)
// 		{
// 			MONSTERPOOL->CreateMonster(wushuang_fb_cfg->monster_id, this->GetScene(), wushuang_fb_cfg->monster_posistion);
// 		}
// 	}
// 
// 	m_owner_user_id = role->GetUserId(); 
// 	m_owner_objid = role->GetId();
}

Role* SpecialWushuangFB::GetOwnerRole()
{
// 	Obj *obj = m_scene->GetObj(m_owner_objid);
// 	if (NULL != obj && Obj::OBJ_TYPE_ROLE == obj->GetObjType())
// 	{
// 		Role *role = (Role*)obj;
// 		if (m_owner_user_id == role->GetUserId()) 
// 		{
// 			return role;
// 		}
// 	}

	return NULL;
}

void SpecialWushuangFB::OnAllMonsterDead()
{
//	this->OnFBFinish(true);
}


