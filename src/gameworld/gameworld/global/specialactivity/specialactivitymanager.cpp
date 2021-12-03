#include <memory>

#include "specialactivitymanager.hpp"
#include "specialactivity.hpp"

#include "obj/character/role.h"
#include "global/worldstatus/worldstatus.hpp"
#include "global/specialactivity/impl/specialactivityopengame.hpp"
#include "global/specialactivity/impl/specialactivityclosedbeta.hpp"
#include "global/specialactivity/impl/specialactivitycommonactivity.hpp"
#include "global/specialactivity/impl/specialactivitycombineserver.hpp"
#include "config/activityconfig/activityswitchconfig.hpp"
#include "config/logicconfigmanager.hpp"

SpecialActivityManager::SpecialActivityManager()
{
	UNSTD_STATIC_CHECK(5 == SPECIAL_ACTIVITY_TYPE_RESVERED - SPECIAL_ACTIVITY_TYPE_INVALID);

	memset(m_special_activity_list, 0, sizeof(m_special_activity_list));

	m_special_activity_list[SPECIAL_ACTIVITY_TYPE_OPENGAME - SPECIAL_ACTIVITY_TYPE_INVALID] = new SpecialActivityOpenGame(this);
	m_special_activity_list[SPECIAL_ACTIVITY_TYPE_CLOSEDBETA - SPECIAL_ACTIVITY_TYPE_INVALID] = new SpecialActivityClosedBeta(this);
	m_special_activity_list[SPECIAL_ACTIVITY_TYPE_COMMON_ACTIVITY - SPECIAL_ACTIVITY_TYPE_INVALID] = new SpecialActivityCommonActivity(this);
	m_special_activity_list[SPECIAL_ACTIVITY_TYPE_COMBINE_SERVER - SPECIAL_ACTIVITY_TYPE_INVALID] = new SpecialActivityCombineServer(this);
}

SpecialActivityManager::~SpecialActivityManager()
{
	for (int i = 0; i < SPECIAL_ACTIVITY_TYPE_MAX; i++)
	{
		if (NULL != m_special_activity_list[i])
		{
			delete m_special_activity_list[i];
		}
	}

	memset(m_special_activity_list, 0, sizeof(m_special_activity_list));
}

SpecialActivityManager & SpecialActivityManager::Instance()
{
	static SpecialActivityManager sam;
	return sam;
}

void SpecialActivityManager::ReInitActivityTime()
{
	for (int i = 0; i < SPECIAL_ACTIVITY_TYPE_MAX; i++)
	{
		if (NULL != m_special_activity_list[i])
		{
			m_special_activity_list[i]->InitActivityTime();
		}
	}
}

void SpecialActivityManager::Update(unsigned long interval, time_t now_second)
{
	if (!WorldStatus::Instance().IsLoadFinish()) return;

	for (int i = 0; i < SPECIAL_ACTIVITY_TYPE_MAX; i++)
	{
		if (NULL != m_special_activity_list[i])
		{
			m_special_activity_list[i]->Update(interval, now_second);
		}
	}
}

void SpecialActivityManager::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	for (int i = 0; i < SPECIAL_ACTIVITY_TYPE_MAX; i++)
	{
		if (NULL != m_special_activity_list[i])
		{
			m_special_activity_list[i]->OnDayChange(old_dayid, now_dayid);
		}
	}
}

void SpecialActivityManager::OnUserLogin(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	for (int i = 0; i < SPECIAL_ACTIVITY_TYPE_MAX; i++)
	{
		if (NULL != m_special_activity_list[i])
		{
			m_special_activity_list[i]->OnUserLogin(role);
		}
	}
}

void SpecialActivityManager::OnUserLogout(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	for (int i = 0; i < SPECIAL_ACTIVITY_TYPE_MAX; i++)
	{
		if (NULL != m_special_activity_list[i])
		{
			m_special_activity_list[i]->OnUserLogout(role);
		}
	}
}

bool SpecialActivityManager::OnForceSetSpecialActivityTime(int special_activity_type, unsigned int begin_time, unsigned int end_time)
{
	if (LOGIC_CONFIG->GetActivitySwitchConfig().IsBanActivity(special_activity_type)) return false;

	special_activity_type -= SPECIAL_ACTIVITY_TYPE_INVALID;
	if (special_activity_type <= 0 || special_activity_type >= SPECIAL_ACTIVITY_TYPE_MAX) return false;

	if (NULL != m_special_activity_list[special_activity_type])
	{
		return m_special_activity_list[special_activity_type]->ForceSetSpecialActivityTime(begin_time, end_time);
	}

	return false;
}

bool SpecialActivityManager::IsSpecialActivityOpening(int special_activity_type)
{
	special_activity_type -= SPECIAL_ACTIVITY_TYPE_INVALID;
	if (special_activity_type <= 0 || special_activity_type >= SPECIAL_ACTIVITY_TYPE_MAX) return false;

	if (NULL != m_special_activity_list[special_activity_type])
	{
		return ACTIVITY_STATUS_OPEN == m_special_activity_list[special_activity_type]->GetStatus();
	}

	return false;
}

SpecialActivityCombineServer * SpecialActivityManager::GetSpecialActivityCombineServer()
{
	return (SpecialActivityCombineServer *)m_special_activity_list[SPECIAL_ACTIVITY_TYPE_COMBINE_SERVER - SPECIAL_ACTIVITY_TYPE_INVALID];
}


