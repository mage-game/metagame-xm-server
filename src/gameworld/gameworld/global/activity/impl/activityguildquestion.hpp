#ifndef __ACTIVITY_GUILD_QUESTION_HPP__
#define __ACTIVITY_GUILD_QUESTION_HPP__

#include "global/activity/activity.hpp"

class ActivityGuildQuestion : public Activity
{
public:
	ActivityGuildQuestion(ActivityManager *activity_manager);
	virtual ~ActivityGuildQuestion();
};

#endif // __ACTIVITY_GUILD_QUESTION_HPP__
