#ifndef __ACTIVITY_NIGHT_FIGHT_HPP__
#define __ACTIVITY_NIGHT_FIGHT_HPP__

#include "global/activity/activity.hpp"

class ActivityNightFight : public Activity
{
public:
	ActivityNightFight(ActivityManager *activity_manager);
	virtual ~ActivityNightFight();

	virtual void Update(unsigned long interval, time_t now_second);
	virtual bool ForceToNextState();
	virtual void OnActivityStatusChange(int from_status, int to_status);
};

#endif // __ACTIVITY_NIGHT_FIGHT_HPP__
