#include "activityguildquestion.hpp"
#include "scene/activityshadow/activityshadow.hpp"
#include "world.h"
#include "config/sharedconfig/sharedconfig.h"

ActivityGuildQuestion::ActivityGuildQuestion(ActivityManager *activity_manager)
	: Activity(activity_manager, ACTIVITY_TYPE_GUILD_QUESTION)
{

}

ActivityGuildQuestion::~ActivityGuildQuestion()
{

}
