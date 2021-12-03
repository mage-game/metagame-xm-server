#include "hangup.hpp"

#include "obj/character/role.h"
#include "engineadapter.h"
#include "protocal/msgroleother.h"

Hangup::Hangup()
	:m_role(NULL)
{
	memset(m_hangup_set, 0, sizeof(m_hangup_set));
	memset(m_custom_set, 0, sizeof(m_custom_set));
}

Hangup::~Hangup()
{

}

void Hangup::Init(Role *role, const RoleHangupParam &p)
{
	m_role = role;

	memcpy(m_hangup_set, p.hangup_set, sizeof(m_hangup_set));
	memcpy(m_custom_set, p.custom_set, sizeof(m_custom_set));
}

void Hangup::GetInitParam(RoleHangupParam *p)
{
	memcpy(p->hangup_set, m_hangup_set, sizeof(p->hangup_set));
	memcpy(p->custom_set, m_custom_set, sizeof(p->custom_set));
}

void Hangup::SaveHangupSetData(HangupSetData setdata)
{
	memcpy(m_hangup_set, setdata, sizeof(m_hangup_set));
}

void Hangup::SaveCustomSetData(CustomSetData customsetdata)
{
	memcpy(m_custom_set, customsetdata, sizeof(m_custom_set));
}

void Hangup::SendHangupInfo()
{
	static Protocol::SCHangupSetInfo hsi;
	memcpy(hsi.hangup_set, m_hangup_set, sizeof(hsi.hangup_set));
	memcpy(hsi.custom_set, m_custom_set, sizeof(hsi.custom_set));
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&hsi, sizeof(hsi));
}
