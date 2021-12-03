#ifndef __ACTIVITY_TIANJIAN_CAIBAO_HPP__
#define __ACTIVITY_TIANJIAN_CAIBAO_HPP__

#include "global/activity/activity.hpp"

class ActivityTianJianbCaiBao : public Activity
{
public:
	ActivityTianJianbCaiBao(ActivityManager *activity_manager);
	virtual ~ActivityTianJianbCaiBao();

	virtual void OnActivityStatusChange(int from_status, int to_status);

protected:
	virtual bool IsRecordRoomStatus(int *activity_sceneid = NULL);
};

#endif // __ACTIVITY_TIANJIAN_CAIBAO_HPP__
