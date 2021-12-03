#ifndef __RANDACTIVITY_FISHING_HPP__
#define __RANDACTIVITY_FISHING_HPP__

#include "global/randactivity/randactivity.hpp"

class RandActivityFishing : public RandActivity
{
public:
	RandActivityFishing(RandActivityManager *rand_activity_manager);
	virtual ~RandActivityFishing();

	virtual void OnSpecialActivityStatusChange(int from_status, int to_status);
};

#endif