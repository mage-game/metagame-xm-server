#ifndef __RAND_ACTIVITY_ADDITION_LOTTERY_HPP__
#define __RAND_ACTIVITY_ADDITION_LOTTERY_HPP__

#include "global/randactivity/randactivity.hpp"

class RandActivityLevelLottery : public RandActivity
{
	struct ActivityData
	{
		ActivityData() : server_chou_times(0) {}

		int server_chou_times;			// 全服抽奖次数
	};

public:
	RandActivityLevelLottery(RandActivityManager *rand_activity_manager);
	virtual ~RandActivityLevelLottery();

	virtual void OnSpecialActivityStatusChange(int from_status, int to_status);
	virtual void Init(const RandActivityData &data);
	virtual void GetInitParam(RandActivityData *data);

	int GetLotteryHistoryList(RALevelLotteryHistoryItem *history_list);
	void AddLotteryHistoryItem(Role * m_role, int reward_index);
	
	const int GetServerChouTimes() { return m_activity_data.server_chou_times; }
	void AddServerChouTimes(const int add_times) { m_activity_data.server_chou_times += add_times; }
	void ClearServerChouTimes() { m_activity_data.server_chou_times = 0; }

protected:
	int m_history_count;													// 历史纪录数量
	int m_history_last_index;												// 历史纪录最新插入的记录下标
	RALevelLotteryHistoryItem m_history_list[RA_LEVEL_LOTTERY_HISTORY_MAX_COUNT];

	ActivityData m_activity_data;
};
#endif

