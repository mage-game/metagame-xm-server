#ifndef __RAND_ACTIVITY_ADDITION_LOTTERY_HPP__
#define __RAND_ACTIVITY_ADDITION_LOTTERY_HPP__

#include "global/randactivity/randactivity.hpp"

class RandActivityLevelLottery : public RandActivity
{
	struct ActivityData
	{
		ActivityData() : server_chou_times(0) {}

		int server_chou_times;			// ȫ���齱����
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
	int m_history_count;													// ��ʷ��¼����
	int m_history_last_index;												// ��ʷ��¼���²���ļ�¼�±�
	RALevelLotteryHistoryItem m_history_list[RA_LEVEL_LOTTERY_HISTORY_MAX_COUNT];

	ActivityData m_activity_data;
};
#endif

