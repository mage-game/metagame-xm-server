#include "activityguildbonfire.h"
#include "engineadapter.h"
#include "protocal/msgother.h"
#include "global/worldstatus/worldstatus.hpp"
#include "config/logicconfigmanager.hpp"

ActivityGuildBonfire::ActivityGuildBonfire(ActivityManager *activity_manager)
: Activity(activity_manager, ACTIVITY_TYPE_GUILD_BONFIRE)
{

}

ActivityGuildBonfire::~ActivityGuildBonfire()
{

}

