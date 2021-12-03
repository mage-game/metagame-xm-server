#ifndef __ACTIVITY_ZHU_XIE_HPP__
#define __ACTIVITY_ZHU_XIE_HPP__

#include "global/activity/activity.hpp"

class ActivityZhuXie : public Activity
{
public:
	ActivityZhuXie(ActivityManager *activity_manager);
	virtual ~ActivityZhuXie();

	virtual void OnActivityStatusChange(int from_status, int to_status);

protected:
	virtual bool IsRecordRoomStatus(int *activity_sceneid = NULL);
};

#endif // __ACTIVITY_ZHU_XIE_HPP__

