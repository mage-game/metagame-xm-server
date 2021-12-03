#ifndef __ACTIVITY_XIAN_MENG_ZHAN_HPP__
#define __ACTIVITY_XIAN_MENG_ZHAN_HPP__

#include "global/activity/activity.hpp"

#include <set>
#include <map>

class GlobalUser;

class ActivityXianMengZhan : public Activity
{
public:
	ActivityXianMengZhan(ActivityManager *activity_manager);
	virtual ~ActivityXianMengZhan();

	virtual void OnActivityStatusChange(int from_status, int to_status);
	virtual int GetRoomMaxUserCount();

protected:
	virtual bool IsRecordRoomStatus(int *activity_sceneid = NULL);
};

#endif // __ACTIVITY_XIAN_MENG_ZHAN_HPP__
