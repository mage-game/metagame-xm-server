#ifndef __RAND_ACTIVITY_GUILD_BATTLE_HPP__
#define __RAND_ACTIVITY_GUILD_BATTLE_HPP__

#include "global/randactivity/randactivity.hpp"

class RandActivityGuildBattle : public RandActivity
{
public:
	RandActivityGuildBattle(RandActivityManager *rand_activity_manager);
	virtual ~RandActivityGuildBattle();
};

#endif