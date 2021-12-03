#include "activitynationalboss.hpp"
#include "engineadapter.h"
#include "protocal/msgother.h"
#include "global/worldstatus/worldstatus.hpp"
#include "global/activity/activitymanager.hpp"

ActivityNationalBoss::ActivityNationalBoss(ActivityManager *activity_manager)
	: Activity(activity_manager, ACTIVITY_TYPE_NATIONAL_BOSS)
{
	memset(m_is_camp_finish, 0, sizeof(m_is_camp_finish));
}

ActivityNationalBoss::~ActivityNationalBoss()
{

}

void ActivityNationalBoss::OnActivityStatusChange(int from_status, int to_status)
{
	switch (to_status)
	{
	case ACTIVITY_STATUS_CLOSE:
		{
			for (int i = CAMP_TYPE_FEIXING; i < CAMP_TYPE_MAX; ++ i)
			{
				m_is_camp_finish[i] = true;

				//WorldStatus::Instance().OnActivityFinish(ACTIVITY_FIND_TYPE_NATIONAL_BOSS);
			}
		}
		break;

	case ACTIVITY_STATUS_OPEN:
		{
			for (int i = CAMP_TYPE_FEIXING; i < CAMP_TYPE_MAX; ++ i)
			{
				m_is_camp_finish[i] = false;
			}
		}
		break;
	}

	Activity::OnActivityStatusChange(from_status, to_status);
}


