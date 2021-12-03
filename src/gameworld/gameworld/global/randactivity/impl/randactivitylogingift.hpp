#ifndef __RANDACTIVITY_LOGIN_GIFT__
#define __RANDACTIVITY_LOGIN_GIFT__

#include "global/randactivity/randactivity.hpp"

class RandActivityLoginGift : public RandActivity
{
public:
	RandActivityLoginGift(RandActivityManager *rand_activity_manager);
	RandActivityLoginGift(RandActivityManager *rand_activity_manager, int activity_type);
	virtual ~RandActivityLoginGift();
};

#endif


