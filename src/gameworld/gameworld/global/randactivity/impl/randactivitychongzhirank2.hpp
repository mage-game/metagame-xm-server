#ifndef __RANDACTIVITY_CHONGZHI2_HPP__
#define __RANDACTIVITY_CHONGZHI2_HPP__

#include "global/randactivity/randactivity.hpp"

class RandActivityChongzhiRank2 : public RandActivity
{
public:
	RandActivityChongzhiRank2(RandActivityManager *rand_activity_manager);
	virtual ~RandActivityChongzhiRank2();

	virtual void OnSpecialActivityStatusChange(int from_status, int to_status);
};

#endif


