#include "activitymonsterinvade.hpp"
#include "engineadapter.h"
#include "protocal/msgother.h"
#include "global/worldstatus/worldstatus.hpp"

ActivityMonsterInvade::ActivityMonsterInvade(ActivityManager *activity_manager)
	: Activity(activity_manager, ACTIVITY_TYPE_MONSTER_INVADE)
{
	
}

ActivityMonsterInvade::~ActivityMonsterInvade()
{

}

