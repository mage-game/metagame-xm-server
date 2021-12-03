#include "activitygongchengzhan.hpp"
#include "global/rank/rankmanager.hpp"
#include "global/worldstatus/worldstatus.hpp"

ActivityGongChengZhan::ActivityGongChengZhan(ActivityManager *activity_manager)
	: Activity(activity_manager, ACTIVITY_TYPE_GONGCHENGZHAN)
{
	
}

ActivityGongChengZhan::~ActivityGongChengZhan()
{

}

void ActivityGongChengZhan::OnActivityStatusChange(int from_status, int to_status)
{
	switch (to_status)
	{
	case ACTIVITY_STATUS_OPEN:

		break;

	case ACTIVITY_STATUS_CLOSE:
		{	
			WorldStatus::Instance().OnActivityFinish(ACTIVITY_FIND_TYPE_GONGCHENGZHAN);
		}
		break;
	}

	Activity::OnActivityStatusChange(from_status, to_status); // 保证上面的数据已经同步之后 再同步活动状态
}
