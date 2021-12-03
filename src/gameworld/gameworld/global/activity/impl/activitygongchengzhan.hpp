#ifndef __ACTIVITY_GONGCHANGZHAN_HPP__
#define __ACTIVITY_GONGCHANGZHAN_HPP__

#include "global/activity/activity.hpp"

class ActivityGongChengZhan : public Activity
{
public:
	ActivityGongChengZhan(ActivityManager *activity_manager);
	virtual ~ActivityGongChengZhan();

	virtual void OnActivityStatusChange(int from_status, int to_status);

protected:
};

#endif // __ACTIVITY_GONGCHANGZHAN_HPP__

