#include "rolepersonbossfb.hpp"
#include "obj/character/role.h"
#include "protocal/msgfb.h"

RolePersonBossFB::RolePersonBossFB()
	: m_role(NULL)
{
}

RolePersonBossFB::~RolePersonBossFB()
{
}

void RolePersonBossFB::Init(Role *role, const PersonBossParam &param)
{
	m_role = role;
	m_param = param;
}

void RolePersonBossFB::GetInitParam(PersonBossParam *param)
{
	*param = m_param;
}

void RolePersonBossFB::OnRoleLogin()
{
	this->SendPersonBossInfo();
}

void RolePersonBossFB::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	memset(m_param.person_boss_today_enter_times, 0, sizeof(m_param.person_boss_today_enter_times));
	this->SendPersonBossInfo();
}

int RolePersonBossFB::GetTodayEnterByLayer(int layer)
{
	if (layer <= 0 || layer > MAX_PERSON_BOSS_LAYER)
	{
		return 0;
	}

	return m_param.person_boss_today_enter_times[layer];
}

void RolePersonBossFB::AddTodayEnterTimesByLayer(int layer)
{
	if (layer <= 0 || layer > MAX_PERSON_BOSS_LAYER)
	{
		return;
	}

	++m_param.person_boss_today_enter_times[layer];
	this->SendPersonBossInfo();
}

void RolePersonBossFB::ReqPersonBossInfo()
{
	this->SendPersonBossInfo();
}

void RolePersonBossFB::SendPersonBossInfo()
{
	Protocol::SCPersonBossInfo pbi;
	for (int i = 0; i < MAX_PERSON_BOSS_LAYER; ++i)
	{
		pbi.info_list[i].layer = i + 1;
		pbi.info_list[i].times = m_param.person_boss_today_enter_times[i + 1];
	}

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&pbi, sizeof(pbi));
}