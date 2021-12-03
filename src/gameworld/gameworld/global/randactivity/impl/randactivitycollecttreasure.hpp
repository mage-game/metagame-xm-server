#ifndef __RANDACTIVITY_COLLECT_TREASURE_HPP__
#define __RANDACTIVITY_COLLECT_TREASURE_HPP__

#include "global/randactivity/randactivity.hpp"

class RandActivityCollectTreasure : public RandActivity
{
public:
	RandActivityCollectTreasure(RandActivityManager *rand_activity_manager);
	virtual ~RandActivityCollectTreasure();
};

#endif


