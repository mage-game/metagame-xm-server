#include "activitywanglingexplore.h"
#include "engineadapter.h"
#include "protocal/msgother.h"
#include "global/worldstatus/worldstatus.hpp"
#include "config/logicconfigmanager.hpp"

ActivityWangLingExplore::ActivityWangLingExplore(ActivityManager *activity_manager)
: Activity(activity_manager, ACTIVITY_TYPE_WANGLINGEXPLORE)
{

}

ActivityWangLingExplore::~ActivityWangLingExplore()
{

}

void ActivityWangLingExplore::OnActivityStatusChange(int from_status, int to_status)
{
	Activity::OnActivityStatusChange(from_status, to_status);
}