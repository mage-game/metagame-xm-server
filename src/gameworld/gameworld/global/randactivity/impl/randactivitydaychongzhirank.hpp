#ifndef __RAND_ACTIVITY_DAY_CHONGZHI_RANK_HPP__
#define __RAND_ACTIVITY_DAY_CHONGZHI_RANK_HPP__

#include "global/randactivity/randactivity.hpp"

class RandActivityDayChongzhiRank : public RandActivity
{
public:
	RandActivityDayChongzhiRank(RandActivityManager *rand_activity_manager);
	virtual ~RandActivityDayChongzhiRank();

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


