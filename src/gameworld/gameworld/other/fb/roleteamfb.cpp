#include "roleteamfb.hpp"
#include "obj/character/role.h"
#include "protocal/msgfb.h"

RoleTeamFB::RoleTeamFB() : m_role(NULL)
{

}

RoleTeamFB::~RoleTeamFB()
{

}

void RoleTeamFB::Init(Role *role)
{
	m_role = role;
}

void RoleTeamFB::GetInitParam()
{
}

void RoleTeamFB::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{

}

void RoleTeamFB::SendInfo()
{
	/*Protocol::SCTeamFBRoleInfo tfri;
	tfri.open_times = m_open_times;
	tfri.assist_reward_times = m_assist_reward_times;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&tfri, sizeof(tfri));*/
}