#ifndef __RAND_ACTIVITY_DAY_CONSUME_RANK_HPP__
#define __RAND_ACTIVITY_DAY_CONSUME_RANK_HPP__

#include "global/randactivity/randactivity.hpp"

class RandActivityDayConsumeRank : public RandActivity
{
public:
	RandActivityDayConsumeRank(RandActivityManager *rand_activity_manager);
	virtual ~RandActivityDayConsumeRank();

	virtual void Init(const RandActivityData &data);
	virtual void GetInitParam(RandActivityData *data);

	virtual void OnSpecialActivityStatusChange(int from_status, int to_status);
	virtual void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);

private:
	void CheckGiveReward(int day, bool ignore_status);

	struct ActivityDataParam
	{
		ActivityDataParam() : give_reward_last_day(0) {}

		int give_reward_last_day;
	};

	ActivityDataParam m_data_param;
};

#endif




