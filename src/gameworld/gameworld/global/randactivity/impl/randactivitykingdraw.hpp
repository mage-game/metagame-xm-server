#ifndef __RAND_ACTIVITY_ADDITION_KING_DRAW_HPP__
#define __RAND_ACTIVITY_ADDITION_KING_DRAW_HPP__

#include "global/randactivity/randactivity.hpp"

class RandActivityKingDraw : public RandActivity
{
public:
	RandActivityKingDraw(RandActivityManager *rand_activity_manager);
	virtual ~RandActivityKingDraw();

	virtual void Init(const RandActivityData &data);
	virtual void GetInitParam(RandActivityData *data);

	virtual void OnSpecialActivityStatusChange(int from_status, int to_status);

	int GetRewardBeginIndex(int level);
	int GetRewardEndIndex(int level);
	int GetReturnRewardBeginIndex(int level);
	int GetReturnRewardEndIndex(int level);
	const int GetServerChouTimes() { return m_data_param.server_chou_times; }
	void AddServerChouTimes(const int add_times) { m_data_param.server_chou_times += add_times; }
	void ClearServerChouTimes() { m_data_param.server_chou_times = 0; }

private:
	struct ActivityDataParam
	{
		void Reset()
		{
			memset(reward_begin_index, 0, sizeof(reward_begin_index));
			memset(reward_end_index, 0, sizeof(reward_end_index));
			memset(return_reward_begin_index, 0, sizeof(return_reward_begin_index));
			memset(return_reward_end_index, 0, sizeof(return_reward_end_index));
			server_chou_times = 0;
		}

		ActivityDataParam()
		{
			this->Reset();
		}

		short reward_begin_index[RA_KING_DRAW_LEVEL_COUNT];					// 奖励起始下标
		short reward_end_index[RA_KING_DRAW_LEVEL_COUNT];					// 奖励结束下标
		short return_reward_begin_index[RA_KING_DRAW_LEVEL_COUNT];			// 返利奖励起始下标
		short return_reward_end_index[RA_KING_DRAW_LEVEL_COUNT];			// 返利奖励结束下标
		int server_chou_times;												// 全服抽奖次数
	};

	ActivityDataParam m_data_param;
};
#endif