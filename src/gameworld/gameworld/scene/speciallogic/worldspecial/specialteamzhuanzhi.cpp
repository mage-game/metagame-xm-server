#include "specialteamzhuanzhi.hpp"
#include "protocal/msgwushuang.h"
#include "friend/teaminfo.hpp"
#include "engineadapter.h"
#include "obj/character/role.h"
#include "global/team/team.hpp"
#include "servercommon/serverdef.h"

SpecialTeamZhuanzhi::SpecialTeamZhuanzhi(Scene *scene) 
: SpecialLogic(scene), team(NULL), m_is_finish(false), m_is_pass(false), m_kick_out_timestamp(0)
{
	
}

SpecialTeamZhuanzhi::~SpecialTeamZhuanzhi()
{

}

void SpecialTeamZhuanzhi::OnFBFinish(bool is_passed)
{
	if (m_is_finish) return;

	m_is_finish = true;
	m_is_pass = is_passed;

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

	for (std::set<ObjID>::const_iterator it = m_member_objid_setlist.begin(); it != m_member_objid_setlist.end(); ++ it)
	{
		Role *role = GetOwnerRole(*it);
		if (NULL != role)
		{
			EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&sczzfbn, sizeof(sczzfbn));
		}
	}
}

void SpecialTeamZhuanzhi::Update(unsigned long interval, time_t now_second)
{
	SpecialLogic::Update(interval, now_second);

	if (!m_is_finish)
	{
		this->CheckSceneMonsterStatus(interval, now_second);
	}
	else
	{
		if (m_kick_out_timestamp > 0 && now_second >= m_kick_out_timestamp)
		{
			m_kick_out_timestamp = 0;

			this->DelayKickOutAllRole(CHANGE_SCENE_REASON_FB_FINISH);
		}
	}
}

bool SpecialTeamZhuanzhi::OnTimeout()
{
	this->OnFBFinish(false);

	return m_is_pass;
}

void SpecialTeamZhuanzhi::OnRoleEnterScene(Role *role)
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

	{
		TeamInfo *team_info = role->GetTeamInfo();
		if (NULL != team_info && team_info->InTeam())
		{
			team = team_info->GetMyTeam();
		}
	}

	m_member_user_id_setlist.insert(role->GetUserId()); 
	m_member_objid_setlist.insert(role->GetId());
}

void SpecialTeamZhuanzhi::OnRoleLeaveScene(Role *role, bool is_logout)
{
	if (NULL == role)
	{
		return;
	}

	if (m_is_finish)
	{
		return;
	}

	m_member_user_id_setlist.erase(role->GetUserId());
	m_member_objid_setlist.erase(role->GetId());
}

Role * SpecialTeamZhuanzhi::GetOwnerRole(ObjID obj_id)
{
	Obj *obj = m_scene->GetObj(obj_id);
	if (NULL != obj && Obj::OBJ_TYPE_ROLE == obj->GetObjType())
	{
		Role *role = (Role*)obj;
		if (NULL != role) 
		{
			return role;
		}
	}

	return NULL;
}

void SpecialTeamZhuanzhi::OnAllMonsterDead()
{
	this->OnFBFinish(true);
}

