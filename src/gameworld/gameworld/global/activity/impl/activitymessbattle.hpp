#ifndef __ACTIVITY_MESS_BATTLE_HPP__
#define __ACTIVITY_MESS_BATTLE_HPP__

#include "global/activity/activity.hpp"

class ActivityMessBattle : public Activity
{
public:
	ActivityMessBattle(ActivityManager *activity_manager);
	virtual ~ActivityMessBattle();

	virtual void Update(unsigned long interval, time_t now_second);
	virtual bool ForceToNextState();
	virtual void OnActivityStatusChange(int from_status, int to_status);
};

#endif // __ACTIVITY_MESS_BATTLE_HPP__
