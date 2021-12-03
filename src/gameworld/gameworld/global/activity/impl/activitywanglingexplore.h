#ifndef __WANGLINGEXPLORE_CAIBAO_HPP__
#define __WANGLINGEXPLORE_CAIBAO_HPP__

#include "global/activity/activity.hpp"

class ActivityWangLingExplore : public Activity
{
public:
	ActivityWangLingExplore(ActivityManager *activity_manager);
	virtual ~ActivityWangLingExplore();

	virtual void OnActivityStatusChange(int from_status, int to_status);
};

#endif // __WANGLINGEXPLORE_CAIBAO_HPP__
