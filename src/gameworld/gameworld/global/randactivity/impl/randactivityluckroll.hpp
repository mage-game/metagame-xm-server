#ifndef __RANDACTIVITY_LUCKY_ROLL_HPP__
#define __RANDACTIVITY_LUCKY_ROLL_HPP__

#include "global/randactivity/randactivity.hpp"

class RandActivityLuckRoll : public RandActivity
{
public:
	RandActivityLuckRoll(RandActivityManager *rand_activity_manager);
	virtual ~RandActivityLuckRoll();

	virtual void Init(const RandActivityData &data);
	virtual void GetInitParam(RandActivityData *data);

	virtual void OnSpecialActivityStatusChange(int from_status, int to_status);
	
	long long GetGoldPoll() { return m_data_param.gold_poll; }
	void AddGoldPoll(long long add_value); 

	void AddRewardRecord(const LuckyRollRewardRecord &record);
	int GetRewardRecordList(LuckyRollRewardRecord reward_record_list[LUCKY_ROLL_REWARD_RECORD_MAX_COUNT]);

private:	
	struct ActivityDataParam
	{
		ActivityDataParam() : last_open_time(0), gold_poll(0) {}

		unsigned int last_open_time;
		long long gold_poll;
	};

	ActivityDataParam m_data_param;

	int m_reward_record_count;
	int m_reward_record_cursor;

	LuckyRollRewardRecord m_reward_record_list[LUCKY_ROLL_REWARD_RECORD_MAX_COUNT];
};

#endif
