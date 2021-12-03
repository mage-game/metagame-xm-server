#ifndef __RANDACTIVITY_LIMIT_BUY_HPP__
#define __RANDACTIVITY_LIMIT_BUY_HPP__

#include "global/randactivity/randactivity.hpp"

class RandActivityLimitBuy : public RandActivity
{
public:
	RandActivityLimitBuy(RandActivityManager *rand_activity_manager);
	virtual ~RandActivityLimitBuy();
};

#endif


