#ifndef __RAND_ACTIVITY_TOMORROW_REWARD_HPP__
#define __RAND_ACTIVITY_TOMORROW_REWARD_HPP__

#include "global/randactivity/randactivity.hpp"

class RandActivityTomorrowReward : public RandActivity
{
public:
	RandActivityTomorrowReward(RandActivityManager *rand_activity_manager);
	virtual ~RandActivityTomorrowReward();
};

#endif

