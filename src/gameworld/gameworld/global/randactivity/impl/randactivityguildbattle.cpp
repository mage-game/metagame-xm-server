#include "randactivitydaychongzhirank.hpp"
#include "world.h"
#include "global/randactivity/randactivitymanager.hpp"
#include "gameworld/global/randactivity/impl/randactivityguildbattle.hpp"

RandActivityGuildBattle::RandActivityGuildBattle(RandActivityManager *rand_activity_manager)
	: RandActivity(rand_activity_manager, RAND_ACTIVITY_TYPE_GUILDBATTLE)
{

}

RandActivityGuildBattle::~RandActivityGuildBattle()
{

}