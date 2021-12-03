#include "activity.hpp"

#include "protocal/msgother.h"
#include "globalconfig/globalconfig.h"
#include "engineadapter.h"
#include "world.h"
#include "config/sharedconfig/sharedconfig.h"
#include "servercommon/internalprotocal/gglobalprotocal.h"
#include "config/logicconfigmanager.hpp"

#include "obj/character/role.h"
#include "scene/scene.h"
#include "servercommon/activitydef.hpp"
#include "global/randactivity/randactivitymanager.hpp"
#include "global/activity/activitymanager.hpp"
#include "scene/activityshadow/activityshadow.hpp"

Activity::Activity(ActivityManager *activity_manager, int activity_type)
	: m_activity_manager(activity_manager), m_activity_type(activity_type), m_activity_status(ACTIVITY_STATUS_CLOSE),
	m_next_status_switch_time(0), m_data(0), m_next_open_wday(0)
{
}

Activity::~Activity()
{

}

void Activity::OnActivityStatusChange(int from_status, int to_status)
{
	{
		memset(m_room_status_list, 0, sizeof(m_room_status_list));
		if (ACTIVITY_STATUS_OPEN == to_status) 
		{
			m_room_status_list[0].Open();
		}
	}

	static ActivityStatus ggas;
	ggas.activity_type = (short)m_activity_type;
	ggas.status = (short)to_status;
	ggas.activity_begin_time = 0;
	ggas.activity_end_time = 0;
	ggas.data = m_data;

	switch (to_status)
	{
	case ACTIVITY_STATUS_STANDY:
		{
			ggas.activity_begin_time = (unsigned int)EngineAdapter::Instance().Time();
		}
		break;

	case  ACTIVITY_STATUS_OPEN:
		{
			if (m_statustime_list_cfg.statustime[ACTIVITY_STATUS_STANDY].Invalid())
			{
				ggas.activity_begin_time = (unsigned int)EngineAdapter::Instance().Time();
			}
			ggas.activity_end_time = m_next_status_switch_time;
		}
		break;

	case ACTIVITY_STATUS_CLOSE:
		{
			
		}
		break;
	}

	World::GetInstWorld()->GetSceneManager()->OnSyncActivityStatus(&ggas);
}

void Activity::Update(unsigned long interval, time_t now_second)
{
	if (0 == m_next_status_switch_time)
	{
		bool is_find = false;
		GLOBALCONFIG->GetDynamicConfig().GetActivityNextCfg(m_activity_type, &m_next_open_wday, &m_statustime_list_cfg, &is_find);
		m_next_status_switch_time = GLOBALCONFIG->GetDynamicConfig().GetActivityNextSwitchTime(m_activity_type, m_activity_status, m_next_open_wday, &m_statustime_list_cfg, is_find);
	}
	else 
	{
		unsigned int now = static_cast<unsigned int>(now_second);

		if (now >= m_next_status_switch_time)
		{
			int old_activity_status = m_activity_status;

			m_activity_status = GLOBALCONFIG->GetDynamicConfig().GetActivtyNextStatus(m_activity_type, m_activity_status, m_next_open_wday, &m_statustime_list_cfg);
			if (ACTIVITY_STATUS_CLOSE == m_activity_status)
			{
				GLOBALCONFIG->GetDynamicConfig().GetActivityNextCfg(m_activity_type, &m_next_open_wday, &m_statustime_list_cfg);
			}
			m_next_status_switch_time = GLOBALCONFIG->GetDynamicConfig().GetActivityNextSwitchTime(m_activity_type, m_activity_status, m_next_open_wday, &m_statustime_list_cfg, true);
			
			// 活动在今日不能开启，则不继续变更活动状态
			int open_game_day = GetDayIndex(SHAREDCONFIG->GetOpenServerConfig().GetServerRealStartTime(), now_second);
			if (GLOBALCONFIG->GetDynamicConfig().IsActivitySpecialCloseDay(m_activity_type, open_game_day))
			{
				return;
			}
			
			this->OnActivityStatusChange(old_activity_status, m_activity_status);

			if (old_activity_status != m_activity_status)
			{
				this->NotfiyActivityStatus(true);
			}
		}
	}
}

void Activity::OnUserLogin(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	if (ACTIVITY_STATUS_CLOSE != m_activity_status)
	{
		this->NotfiyActivityStatus(false, role);
	}

	if (ACTIVITY_TYPE_TERRITORY_WAR == m_activity_type)
	{
		this->NotfiyActivityStatus(false, role);
	}

	if (RandActivityManager::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_OPEN_SERVER_BATTLE))
	{
		if (ACTIVITY_TYPE_QUNXIANLUANDOU == m_activity_type || ACTIVITY_TYPE_GONGCHENGZHAN == m_activity_type || ACTIVITY_TYPE_GUILDBATTLE == m_activity_type)
		{
			this->NotfiyActivityStatus(false, role);
		}
	}

	if (ActivityShadow::Instance().IsSpecialActivityOpen(SPECIAL_ACTIVITY_TYPE_COMBINE_SERVER))
	{
		if (ACTIVITY_TYPE_GONGCHENGZHAN == m_activity_type || ACTIVITY_TYPE_GUILDBATTLE == m_activity_type)
		{
			this->NotfiyActivityStatus(false, role);
		}
	}
}

void Activity::OnUserLogout(Role *role)
{
	if (NULL == role)
	{
		return;
	}
}

void Activity::OnUserEnterScene(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	if (ACTIVITY_STATUS_OPEN != m_activity_status && ACTIVITY_STATUS_STANDY != m_activity_status) 
	{
		return;
	}

	int activity_sceneid = 0;
	if (!this->IsRecordRoomStatus(&activity_sceneid)) 
	{
		return;
	}

	if (NULL == role->GetScene())
	{
		return;
	}

	if (role->GetScene()->GetSceneID() == activity_sceneid) // 进入活动场景
	{
		int to_room_index = role->GetScene()->GetSceneKey() - 1;
		if (to_room_index >= 0 && to_room_index < ACTIVITY_ROOM_MAX)
		{
			++ m_room_status_list[to_room_index].role_num;
			if (m_room_status_list[to_room_index].role_num >= this->GetRoomMaxUserCount() * 4 / 5)
			{
				int next_room_index = to_room_index + 1;
				if (next_room_index < ACTIVITY_ROOM_MAX && !m_room_status_list[next_room_index].IsOpen())
				{
					m_room_status_list[next_room_index].Open();
				}
			}
		}
	}
}

void Activity::OnUserLeaveScene(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	if (ACTIVITY_STATUS_OPEN != m_activity_status && ACTIVITY_STATUS_STANDY != m_activity_status) 
	{
		return;
	}

	int activity_sceneid = 0;
	if (!this->IsRecordRoomStatus(&activity_sceneid)) 
	{
		return;
	}

	if (NULL == role->GetScene())
	{
		return;
	}

	if (role->GetScene()->GetSceneID() == activity_sceneid) // 离开活动场景
	{
		int from_room_index = role->GetScene()->GetSceneKey() - 1;
		if (from_room_index >= 0 && from_room_index < ACTIVITY_ROOM_MAX)
		{
			-- m_room_status_list[from_room_index].role_num;
			if (m_room_status_list[from_room_index].role_num < 0) m_room_status_list[from_room_index].role_num = 0;
		}
	}
}

void Activity::NotfiyActivityStatus(bool is_broadcast, Role *role)
{
	static Protocol::SCActivityStatus as;
	as.activity_type = (short)m_activity_type;
	as.status = static_cast<char>(m_activity_status);
	as.is_broadcast = is_broadcast ? 1 : 0;
	as.next_status_switch_time = m_next_status_switch_time;

	if (NULL != role)
	{
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&as, sizeof(as));
	}
	else 
	{
		World::GetInstWorld()->SendToAllGateway((const char*)&as, sizeof(as));
	}
}

bool Activity::ForceToNextState()
{
	m_next_status_switch_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	return true;
}

void Activity::OnQueryRoomStatusReq(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	static Protocol::SCActivityRoomStatusAck rsa;
	rsa.activity_type = m_activity_type;
	rsa.room_user_max = this->GetRoomMaxUserCount();
	UNSTD_STATIC_CHECK(sizeof(rsa.room_status_list) == sizeof(m_room_status_list));
	memcpy(rsa.room_status_list, m_room_status_list, sizeof(rsa.room_status_list));
	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&rsa, sizeof(rsa));
}

