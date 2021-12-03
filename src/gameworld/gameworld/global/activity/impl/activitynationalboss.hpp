#ifndef __ACTIVITY_NATIONALBOSS_HPP__
#define __ACTIVITY_NATIONALBOSS_HPP__

#include "global/activity/activity.hpp"

class ActivityNationalBoss : public Activity
{
public:
	ActivityNationalBoss(ActivityManager *activity_manager);
	virtual ~ActivityNationalBoss();

	virtual void OnActivityStatusChange(int from_status, int to_status);

private:
	bool m_is_camp_finish[CAMP_TYPE_MAX];
};

#endif // __ACTIVITY_ZHU_XIE_HPP__

