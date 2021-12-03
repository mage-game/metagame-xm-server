#ifndef __ACTIVITY_YIZHANDAODI_HPP__
#define __ACTIVITY_YIZHANDAODI_HPP__

#include "global/activity/activity.hpp"

class ActivityYiZhanDaoDi : public Activity
{
public:
	ActivityYiZhanDaoDi(ActivityManager *activity_manager);
	virtual ~ActivityYiZhanDaoDi();

protected:
	virtual bool IsRecordRoomStatus(int *activity_sceneid = NULL);
};

#endif // __ACTIVITY_YIZHANDAODI_HPP__

