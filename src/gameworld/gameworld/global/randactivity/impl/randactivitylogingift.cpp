#include "randactivitylogingift.hpp"
#include "global/randactivity/randactivitymanager.hpp"

RandActivityLoginGift::RandActivityLoginGift(RandActivityManager *rand_activity_manager)
	: RandActivity(rand_activity_manager, RAND_ACTIVITY_TYPE_LOGIN_GIFT)
{

}

RandActivityLoginGift::RandActivityLoginGift(RandActivityManager *rand_activity_manager, int activity_type)
	: RandActivity(rand_activity_manager, activity_type)
{

}

RandActivityLoginGift::~RandActivityLoginGift()
{
 
}


