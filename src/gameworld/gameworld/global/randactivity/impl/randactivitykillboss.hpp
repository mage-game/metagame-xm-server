#ifndef __RAND_ACTIVITY_KILL_BOSS_HPP__
#define __RAND_ACTIVITY_KILL_BOSS_HPP__

#include "global/randactivity/randactivity.hpp"

class RandActivityKillBoss : public RandActivity
{
public:
	RandActivityKillBoss(RandActivityManager *rand_activity_manager);
	virtual ~RandActivityKillBoss();
};
#endif

