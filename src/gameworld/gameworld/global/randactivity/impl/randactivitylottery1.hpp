#ifndef __RAND_ACTIVITY_LOTTERY_1_HPP__
#define __RAND_ACTIVITY_LOTTERY_1_HPP__

#include "global/randactivity/randactivity.hpp"

class RandActivityLottery1 : public RandActivity
{
	struct ActivityData
	{
		ActivityData() : server_chou_times(0) {}

		int server_chou_times;			// 全服抽奖次数
	};

public:
	RandActivityLottery1(RandActivityManager *rand_activity_manager);
	virtual ~RandActivityLottery1();

	virtual void Init(const RandActivityData &data);
	virtual void GetInitParam(RandActivityData *data);
	virtual void OnSpecialActivityStatusChange(int from_status, int to_status);

	const int GetServerChouTimes() { return m_activity_data.server_chou_times; }
	void AddServerChouTimes(const int add_times) { m_activity_data.server_chou_times += add_times; }
	void ClearServerChouTimes() { m_activity_data.server_chou_times = 0; }

protected:

	ActivityData m_activity_data;

};
#endif

