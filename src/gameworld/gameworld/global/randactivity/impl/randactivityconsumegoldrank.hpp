#ifndef __RANDACTIVITY_CONSUME_GOLD_RANK__
#define __RANDACTIVITY_CONSUME_GOLD_RANK__

#include "global/randactivity/randactivity.hpp"

class RandActivityConsumeGoldRank : public RandActivity
{
public:
	RandActivityConsumeGoldRank(RandActivityManager *rand_activity_manager);
	virtual ~RandActivityConsumeGoldRank();

	virtual void OnSpecialActivityStatusChange(int from_status, int to_status);
};

#endif


