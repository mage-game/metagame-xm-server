#ifndef __RAND_ACTIVITY_EXTREME_LUCKY_HPP__
#define __RAND_ACTIVITY_EXTREME_LUCKY_HPP__

#include "global/randactivity/randactivity.hpp"

class RandActivityExtremeLucky : public RandActivity
{
public:
	RandActivityExtremeLucky(RandActivityManager *rand_activity_manager);
	virtual ~RandActivityExtremeLucky();

	virtual void Init(const RandActivityData &data);
	virtual void GetInitParam(RandActivityData *data);

	virtual void OnSpecialActivityStatusChange(int from_status, int to_status);

	int GetRewardBeginIndex() { return m_data_param.reward_begin_index; }
	int GetRewardEndIndex() { return m_data_param.reward_end_index; }

	const int GetServerFlushTimes() { return m_data_param.server_flush_times; }
	void AddServerFlushTimes(const int add_times) { m_data_param.server_flush_times += add_times; }

private:
	struct ActivityDataParam
	{
		ActivityDataParam() : reward_begin_index(0), reward_end_index(0) {}

		short reward_begin_index;				// 奖励起始下标
		short reward_end_index;					// 奖励结束下标
		int server_flush_times;					// 全服刷新次数
	};

	ActivityDataParam m_data_param;
};

#endif 