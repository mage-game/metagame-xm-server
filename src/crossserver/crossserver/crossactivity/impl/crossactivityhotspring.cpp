#include "crossactivityhotspring.hpp"

#include "engineadapter.h"
#include "internalcomm.h"

#include "servercommon/serverconfig/crosscommonconfig.hpp"
#include "servercommon/serverconfig/crosshotspringconfig.hpp"

#include "crossserver.h"
#include "crossserver/crossuser/crossuser.h"
#include "crossserver/crosslog.hpp"

#include "servercommon/internalprotocal/crossgameprotocal.h"
#include "crossactivity/crossactivitymanager.hpp"


CrossActivityHS::CrossActivityHS(CrossActivityManager *cross_activity_manager)
	: CrossActivity(cross_activity_manager, CROSS_ACTIVITY_TYPE_HOT_SPRING)
{

}

CrossActivityHS::~CrossActivityHS()
{

}

void CrossActivityHS::Init(const CrossActivityData &data)
{
	m_status_end_time = this->CalcActivityNextOpenTime();
	m_is_init_finish = true;
}

void CrossActivityHS::Update(unsigned long interval, time_t now_second)
{
	CrossActivity::Update(interval, now_second);

	if (ACTIVITY_STATUS_OPEN != m_activity_status)
	{
		return;
	}
}

void CrossActivityHS::OnDayChange()
{

}

void CrossActivityHS::OnWeekChange()
{

}

bool CrossActivityHS::CheckCanStartCross(const UniqueUserID &unique_user_id, int merge_server_id)
{
	if (ACTIVITY_STATUS_OPEN != m_activity_status && ACTIVITY_STATUS_STANDY != m_activity_status)
	{
		return false;
	}
	return true;
}



void CrossActivityHS::OnCrossUserLogin(CrossUser *cross_user)
{
	if (NULL == cross_user) return;

	if (ACTIVITY_STATUS_CLOSE == m_activity_status)
	{
		return;
	}

	UserID user_id = IntToUid(cross_user->GetCrossUid());
	if (CROSS_ACTIVITY_TYPE_HOT_SPRING != GetCrossUserCrossActivityType(user_id))
	{
		return;
	}

	crossgameprotocal::CrossGameCrossActivityGotoScene cgtzgs;
	cgtzgs.cross_uid = cross_user->GetCrossUid();
	cgtzgs.cross_activity_type = CROSS_ACTIVITY_TYPE_HOT_SPRING;
	CrossServer::GetInstCrossServer()->SendToHiddenGameWorld((const char*)&cgtzgs, sizeof(cgtzgs));
}

time_t CrossActivityHS::CalcActivityNextOpenTime()
{
	const tm * now_tm = EngineAdapter::Instance().LocalTime();
	int now_day_second = now_tm->tm_hour * 3600 + now_tm->tm_min * 60 + now_tm->tm_sec;

	CrossHotSpringActivityTimeCfg *time_cfg = CrossHotSpringConfig::Instance().GetActivityTimeCfg();
	if (NULL == time_cfg || !time_cfg->is_open) return 0;

	int min_interval = 0;
	for (int i = 0; i < 7; i++)
	{
		int interval = 0;
		int week_day = (now_tm->tm_wday + i) % 7;
		if (0 <= week_day && week_day < 7)
		{
			if (time_cfg->open_day_list[week_day])
			{
				if (now_tm->tm_wday == week_day)
				{
					for(int open_day_time = 0; open_day_time < time_cfg->open_day_secon_count && open_day_time < MAX_ACTIVIYT_TIME_CFG_NUM; open_day_time++)
					{
						if (now_day_second >= time_cfg->open_day_second[open_day_time])
						{
							interval = EngineAdapter::Instance().NextWeekInterval(week_day, 0, 0, 0) + time_cfg->open_day_second[0];
						}
						else
						{
							interval = time_cfg->open_day_second[open_day_time] - now_day_second;
							break;
						}
					}
				}
				else
				{
					interval = EngineAdapter::Instance().NextWeekInterval(week_day, 0, 0, 0) + time_cfg->open_day_second[0];
				}

				if (min_interval <= 0 || interval < min_interval)
				{
					min_interval = interval;
				}
			}
		}
	}

	return EngineAdapter::Instance().Time() + min_interval;
}

void CrossActivityHS::OnChangeToNextStatus()
{
	if (ACTIVITY_STATUS_CLOSE == m_activity_status)
	{
		int standby_time = CrossHotSpringConfig::Instance().GetActivityStandbyTime();
		if (standby_time > 0)
		{
			m_activity_status = ACTIVITY_STATUS_STANDY;
			m_status_end_time = EngineAdapter::Instance().Time() + standby_time;
		}
		else
		{
			m_activity_status = ACTIVITY_STATUS_OPEN;
			m_status_end_time = EngineAdapter::Instance().Time() + CrossHotSpringConfig::Instance().GetActivityOpenTime();
		}


	}
	else if (ACTIVITY_STATUS_STANDY == m_activity_status)
	{
		m_activity_status = ACTIVITY_STATUS_OPEN;
		m_status_end_time = EngineAdapter::Instance().Time() + CrossHotSpringConfig::Instance().GetActivityOpenTime();
	}
	else
	{
		m_activity_status = ACTIVITY_STATUS_CLOSE;

		m_status_end_time = this->CalcActivityNextOpenTime();
	}

	if (ACTIVITY_STATUS_STANDY == m_activity_status)
	{

	}
	else if (ACTIVITY_STATUS_OPEN == m_activity_status)
	{

	}
	else if (ACTIVITY_STATUS_CLOSE == m_activity_status)
	{

	}
}


