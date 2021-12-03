#ifndef __RANDACTIVITY_XIANMENG_BIPIN_HPP__
#define __RANDACTIVITY_XIANMENG_BIPIN_HPP__

#include "global/randactivity/randactivity.hpp"

class RandActivityXianmengBipin : public RandActivity
{
public:
	RandActivityXianmengBipin(RandActivityManager *rand_activity_manager);
	virtual ~RandActivityXianmengBipin();

	virtual void OnSpecialActivityStatusChange(int from_status, int to_status);
};

#endif


