#ifndef __ACTIVITY_WEDDING_HPP__
#define __ACTIVITY_WEDDING_HPP__

#include "global/activity/activity.hpp"

class ActivityWedding : public Activity
{
public:
	ActivityWedding(ActivityManager *activity_manager);
	virtual ~ActivityWedding();

	virtual void Update(unsigned long interval, time_t now_second);

	int GetCurHunyanStatus(unsigned int hunyan_standby_time, int &status);		// 获取婚宴当前状态
	void CheckCreateHunyanFb(unsigned int hunyan_standby_time);					// 检查创建婚宴副本

private:
	int m_old_activity_status;
};

#endif // __ACTIVITY_WEDDING_HPP__

