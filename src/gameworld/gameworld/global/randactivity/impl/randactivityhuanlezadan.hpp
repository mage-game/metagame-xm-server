#ifndef __RANDACTIVITY_HUANLEZADAN_HPP__
#define __RANDACTIVITY_HUANLEZADAN_HPP__

#include "global/randactivity/randactivity.hpp"

class RandActivityHuanLeZaDan : public RandActivity
{

	struct ActivityData
	{
		ActivityData() : server_chou_times(0) {}

		int server_chou_times;			// 全服抽奖次数
	};

public:
	RandActivityHuanLeZaDan(RandActivityManager *rand_activity_manager);
	virtual ~RandActivityHuanLeZaDan();

	virtual void OnSpecialActivityStatusChange(int from_status, int to_status);
	virtual void Init(const RandActivityData &data);
	virtual void GetInitParam(RandActivityData *data);

	const int GetServerChouTimes() { return m_activity_data.server_chou_times; }
	void AddServerChouTimes(const int add_times) { m_activity_data.server_chou_times += add_times; }
	void ClearServerChouTimes() { m_activity_data.server_chou_times = 0; }

protected:
	ActivityData m_activity_data;
};

#endif