#include "specialactivity.hpp"

#include "protocal/msgother.h"

#include "engineadapter.h"
#include "internalcomm.h"

#include "servercommon/serverconfig/configstruct.h"
#include "servercommon/serverconfig/localconfig.hpp"

#include "obj/character/role.h"
#include "world.h"
#include "config/sharedconfig/sharedconfig.h"

#include "globalconfig/globalconfig.h"
#include "protocal/msgactivity.hpp"

SpecialActivity::SpecialActivity(SpecialActivityManager *special_activity_manager, int activity_type)
	: m_special_activity_manager(special_activity_manager), m_special_activity_type(activity_type), m_special_activity_status(ACTIVITY_STATUS_CLOSE), 
	m_begin_time(0), m_end_time(0) 
{

}

SpecialActivity::~SpecialActivity()
{

}

void SpecialActivity::InitActivityTime()
{
	if (SPECIAL_ACTIVITY_TYPE_OPENGAME == m_special_activity_type)
	{
		SHAREDCONFIG->GetOpenServerConfig().GetOpenGameActivityTime(&m_begin_time, &m_end_time);
	}
	else if (SPECIAL_ACTIVITY_TYPE_COMBINE_SERVER == m_special_activity_type)
	{
		SHAREDCONFIG->GetOpenServerConfig().GetCombineServerActivityTime(&m_begin_time, &m_end_time);
	}
	else
	{
		GLOBALCONFIG->GetDynamicConfig().GetSpecialActivityTime(m_special_activity_type, &m_begin_time, &m_end_time);
	}
}

void SpecialActivity::OnSpecialActivityStatusChange(int from_status, int to_status)
{
	this->SyncActivityStatusToGameworld();

	this->NotfiySpecialActivityStatus(true);
}

void SpecialActivity::Update(unsigned long interval, time_t now_second)
{

	if (0 == m_begin_time || 0 == m_end_time)
	{
		this->InitActivityTime();
	}
	else
	{
		unsigned int now = static_cast<unsigned int>(now_second);

		switch (m_special_activity_status)
		{
		case ACTIVITY_STATUS_CLOSE:
			{
				if (m_begin_time <= now && now < m_end_time)
				{
					m_special_activity_status = ACTIVITY_STATUS_OPEN;

					this->OnSpecialActivityStatusChange(ACTIVITY_STATUS_CLOSE, ACTIVITY_STATUS_OPEN);
				}
			}
			break;

		case ACTIVITY_STATUS_OPEN:
			{
				if (now < m_begin_time || m_end_time <= now)
				{
					m_special_activity_status = ACTIVITY_STATUS_CLOSE;

					this->OnSpecialActivityStatusChange(ACTIVITY_STATUS_OPEN, ACTIVITY_STATUS_CLOSE);
				}
			}
			break;
		}
	}
}

void SpecialActivity::OnUserLogin(Role *role)
{
	if (ACTIVITY_STATUS_CLOSE != m_special_activity_status)
	{
		this->NotfiySpecialActivityStatus(false, role);
	}
}

void SpecialActivity::SyncActivityStatusToGameworld()
{
	static ActivityStatus ggas;
	ggas.activity_type = (short)m_special_activity_type;
	ggas.status = m_special_activity_status;
	ggas.activity_begin_time = m_begin_time;
	ggas.activity_end_time = m_end_time;
	World::GetInstWorld()->GetSceneManager()->OnSyncActivityStatus(&ggas);
}

bool SpecialActivity::ForceSetSpecialActivityTime(unsigned int begin_time, unsigned int end_time)
{
	if (begin_time <= 0 || end_time <= 0 || end_time <= begin_time) return false;

	m_begin_time = begin_time; m_end_time = end_time;

	return true;
}

void SpecialActivity::NotfiySpecialActivityStatus(bool is_broadcast, Role *role)
{
	static Protocol::SCActivityStatus as;
	as.activity_type = (short)m_special_activity_type;
	as.status = static_cast<char>(m_special_activity_status);
	as.is_broadcast = is_broadcast ? 1 : 0;
	as.next_status_switch_time = (ACTIVITY_STATUS_OPEN == m_special_activity_status ? m_end_time : m_begin_time);

	if (NULL != role)
	{
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&as, sizeof(as));
	}
	else 
	{
		World::GetInstWorld()->SendToAllGateway((const char*)&as, sizeof(as));
	}
}
