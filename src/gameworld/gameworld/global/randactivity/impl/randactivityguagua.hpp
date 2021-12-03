#ifndef __RAND_ACTIVITY_ADDITION_GUAGUA_HPP__
#define __RAND_ACTIVITY_ADDITION_GUAGUA_HPP__

#include "global/randactivity/randactivity.hpp"

class RandActivityGuaGua : public RandActivity
{
public:
	RandActivityGuaGua(RandActivityManager *rand_activity_manager);
	virtual ~RandActivityGuaGua();

	virtual void Init(const RandActivityData &data);
	virtual void GetInitParam(RandActivityData *data);

	int GetRollTimes() { return m_activity_data.server_roll_times; }
	void AddRollTimes() { ++m_activity_data.server_roll_times; }
	void ClearRollTimes() { m_activity_data.server_roll_times = 0; }

	virtual void OnSpecialActivityStatusChange(int from_status, int to_status);

protected:

	struct ActivityData
	{
		ActivityData() : server_roll_times(0) {}

		int server_roll_times;			// 全服抽奖次数
	};

	ActivityData m_activity_data;
};
#endif