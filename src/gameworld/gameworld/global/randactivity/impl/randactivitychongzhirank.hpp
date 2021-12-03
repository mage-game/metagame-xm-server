#ifndef __RANDACTIVITY_CHONGZHI_HPP__
#define __RANDACTIVITY_CHONGZHI_HPP__

#include "global/randactivity/randactivity.hpp"

class RandActivityChongzhiRank : public RandActivity
{
public:
	RandActivityChongzhiRank(RandActivityManager *rand_activity_manager);
	virtual ~RandActivityChongzhiRank();

	virtual void OnSpecialActivityStatusChange(int from_status, int to_status);
};

#endif


