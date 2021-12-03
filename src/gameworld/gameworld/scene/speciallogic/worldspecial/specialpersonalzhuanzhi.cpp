#include "specialpersonalzhuanzhi.hpp"
#include "protocal/msgwushuang.h"
#include "friend/teaminfo.hpp"
#include "engineadapter.h"
#include "obj/character/role.h"
#include "global/team/team.hpp"
#include "servercommon/serverdef.h"

SpecialPersonalZhuanzhi::SpecialPersonalZhuanzhi(Scene *scene) 
	: SpecialLogic(scene), m_is_finish(false), m_is_pass(false), m_kick_out_timestamp(0)
{
	
}

SpecialPersonalZhuanzhi::~SpecialPersonalZhuanzhi()
{

}

void SpecialPersonalZhuanzhi::OnFBFinish(bool is_passed)
{
	if (m_is_finish) return;

	m_is_finish = true;
	m_is_pass = is_passed;

	Role *role = this->GetOwnerRole();
	if (NULL != role)
	{
		int notify_reason = 0;

		if (is_passed)
		{
			m_kick_out_timestamp = EngineAdapter::Instance().Time() + 10;
			notify_reason = Protocol::ZHUANZHI_FB_NOTIFY_REASON_SUCCESS;
		}
		else
		{
			m_kick_out_timestamp = 0;
			notify_reason = Protocol::ZHUANZHI_FB_NOTIFY_REASON_TIME_OUT;
		}

		static Protocol::SCZhuanzhiFBNotify sczzfbn;
		sczzfbn.notify_reason = notify_reason;
		sczzfbn.zhuanzhi_fb_type = 1;
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&sczzfbn, sizeof(sczzfbn));
	}
}

void SpecialPersonalZhuanzhi::Update(unsigned long interval, time_t now_second)
{
	SpecialLogic::Update(interval, now_second);

	if (!m_is_finish)
	{
		this->CheckSceneMonsterStatus(interval, now_second);
	}
	else
	{
		if (0 != m_kick_out_timestamp && now_second >= m_kick_out_timestamp)
		{
			m_kick_out_timestamp = 0;

			Role *role = GetOwnerRole();
			if (NULL != role)
			{
				this->DelayKickOutRole(GetOwnerRole());
			}
		}
	}
}

bool SpecialPersonalZhuanzhi::OnTimeout()
{
	this->OnFBFinish(false);

	return m_is_pass;
}

void SpecialPersonalZhuanzhi::OnRoleEnterScene(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	if (m_is_finish)
	{
		this->DelayKickOutRole(role);
		return;
	}

	m_owner_user_id = role->GetUserId(); 
	m_owner_objid = role->GetId();
}

Role* SpecialPersonalZhuanzhi::GetOwnerRole()
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

void SpecialPersonalZhuanzhi::OnAllMonsterDead()
{
	this->OnFBFinish(true);
}

