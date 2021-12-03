#ifndef __ACTIVITY_WORSHIP_HPP__
#define __ACTIVITY_WORSHIP_HPP__

#include "global/activity/activity.hpp"

class ActivityWorship : public Activity
{
public:
	struct WorshipItem
	{
		WorshipItem() :manual_worship_time(0), next_can_manual_worship_timestamp(0), manual_worship_add_exp_timestamp(0) {}

		Int32 manual_worship_time;
		UInt32 next_can_manual_worship_timestamp;
		UInt32 manual_worship_add_exp_timestamp;
	};

	ActivityWorship(ActivityManager *activity_manager);
	virtual ~ActivityWorship();

	virtual void OnActivityStatusChange(int from_status, int to_status);
	virtual void Update(unsigned long interval, time_t now_second);

	void OnWorship(Role* role);
	void SendWorshipInfo(Role *role);

protected:
	void CheckWorshipAddexp(time_t now_second);

private:

	UInt32 m_auto_worship_next_addexp_timestamp;		// 自动膜拜活动下次增加经验时间戳
	std::map<int, WorshipItem> m_worship_user_map;
};

#endif // __ACTIVITY_WORSHIP_HPP__


