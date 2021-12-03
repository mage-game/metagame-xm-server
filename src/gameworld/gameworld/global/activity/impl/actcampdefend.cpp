#include "actcampdefend.hpp"
#include "scene/speciallogic/worldspecial/campdefendmanager.hpp"

ActivityCampDefend::ActivityCampDefend(ActivityManager *activity_manager, int activitytype)
	: Activity(activity_manager, activitytype)
{
	
}

ActivityCampDefend::~ActivityCampDefend()
{

}

void ActivityCampDefend::Update(unsigned long interval, time_t now_second)
{
	if (m_activity_type != CampDefendMgr::Instance().GetActingType() &&
		(CampDefendMgr::Instance().IsCampDefendStandBy() || CampDefendMgr::Instance().IsCampDefending()))
	{
		return;
	}

	Activity::Update(interval, now_second);
}

bool ActivityCampDefend::ForceToNextState()
{
	if (m_activity_type != CampDefendMgr::Instance().GetActingType() &&
		(CampDefendMgr::Instance().IsCampDefendStandBy() || CampDefendMgr::Instance().IsCampDefending()))
	{
		return false;
	}

	return Activity::ForceToNextState();
}





