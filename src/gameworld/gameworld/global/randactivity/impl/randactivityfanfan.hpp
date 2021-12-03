#ifndef __RAND_ACTIVITY_FANFAN_HPP__
#define __RAND_ACTIVITY_FANFAN_HPP__

#include "global/randactivity/randactivity.hpp"

class RandActivityFanFan : public RandActivity
{
	struct ActivityData
	{
		ActivityData() : server_chou_times(0) {}

		int server_chou_times;
	};

public:
	RandActivityFanFan(RandActivityManager *rand_activity_manager);
	virtual ~RandActivityFanFan();

	virtual void OnSpecialActivityStatusChange(int from_status, int to_status);

	virtual void Init(const RandActivityData &data);
	virtual void GetInitParam(RandActivityData *data);

	void IncreaseServerChouTimes() { ++m_activity_data.server_chou_times; }
	const int GetServerChouTimes() { return m_activity_data.server_chou_times; }

private:

	ActivityData m_activity_data;
};

#endif

