#include "activityxianmengzhan.hpp"

#include "global/worldstatus/worldstatus.hpp"
#include "config/logicconfigmanager.hpp"
#include "config/activityconfig/xianmengzhanconfig.hpp"

#include "global/rank/rankmanager.hpp"

ActivityXianMengZhan::ActivityXianMengZhan(ActivityManager *activity_manager)
	: Activity(activity_manager, ACTIVITY_TYPE_XIANMENGZHAN)
{

}

ActivityXianMengZhan::~ActivityXianMengZhan()
{

}

void ActivityXianMengZhan::OnActivityStatusChange(int from_status, int to_status)
{
	Activity::OnActivityStatusChange(from_status, to_status);

	if (ACTIVITY_STATUS_CLOSE == to_status)
	{
		RankManager::Instance().Snapshot();
	}
}

int ActivityXianMengZhan::GetRoomMaxUserCount()
{
	return 500;
}

bool ActivityXianMengZhan::IsRecordRoomStatus(int *activity_sceneid)
{
	if (NULL != activity_sceneid) *activity_sceneid = LOGIC_CONFIG->GetXianMengZhanConfig().GetEnterInfo(NULL);
	return true;
}


