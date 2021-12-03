#ifndef __RANDACTIVITY_CRITICAL_STRIKE_HPP__
#define __RANDACTIVITY_CRITICAL_STRIKE_HPP__

#include "global/randactivity/randactivity.hpp"

class RandActivityCriticalStrike : public RandActivity
{
public:
	RandActivityCriticalStrike(RandActivityManager *rand_activity_manager);
	virtual ~RandActivityCriticalStrike();
};

#endif


