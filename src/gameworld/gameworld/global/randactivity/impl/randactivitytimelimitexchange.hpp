#ifndef __RAND_ACTIVITY_TIME_LIMIT_EXCHANGE_HPP__
#define __RAND_ACTIVITY_TIME_LIMIT_EXCHANGE_HPP__
#include "global/randactivity/randactivity.hpp"

class RandActivityTimeLimitExchangeEqui : public RandActivity
{
public:
	 RandActivityTimeLimitExchangeEqui(RandActivityManager *rand_activity_manager);
	 virtual ~RandActivityTimeLimitExchangeEqui();
};

class RandActivityTimeLimitExchangeJL : public RandActivity
{
public:
	RandActivityTimeLimitExchangeJL(RandActivityManager *rand_activity_manager);
	virtual ~RandActivityTimeLimitExchangeJL();
};
#endif

