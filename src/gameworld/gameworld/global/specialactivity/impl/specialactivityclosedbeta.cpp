#include "specialactivityclosedbeta.hpp"

#include "servercommon/activitydef.hpp"

class SpecialActivityManager;

SpecialActivityClosedBeta::SpecialActivityClosedBeta(SpecialActivityManager *special_activity_manager)
	: SpecialActivity(special_activity_manager, SPECIAL_ACTIVITY_TYPE_CLOSEDBETA)
{

}

SpecialActivityClosedBeta::~SpecialActivityClosedBeta()
{

}


