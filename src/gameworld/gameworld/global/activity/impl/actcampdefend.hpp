#ifndef __ACTIVITY_CAMPDEFEND_HPP__
#define __ACTIVITY_CAMPDEFEND_HPP__

#include "global/activity/activity.hpp"

class ActivityCampDefend : public Activity
{
public:
	ActivityCampDefend(ActivityManager *activity_manager, int activitytype);
	virtual ~ActivityCampDefend();

	virtual void Update(unsigned long interval, time_t now_second);
	virtual bool ForceToNextState();

protected:
};

#endif 

