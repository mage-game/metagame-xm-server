#include "activityyizhandaodi.hpp"
#include "config/logicconfigmanager.hpp"
#include "config/activityconfig/yizhandaodiconfig.hpp"

ActivityYiZhanDaoDi::ActivityYiZhanDaoDi(ActivityManager *activity_manager)
	: Activity(activity_manager, ACTIVITY_TYPE_YIZHANDAODI)
{
	
}

ActivityYiZhanDaoDi::~ActivityYiZhanDaoDi()
{

}

bool ActivityYiZhanDaoDi::IsRecordRoomStatus(int *activity_sceneid)
{
	Posi enterpos(0, 0);
	if (NULL != activity_sceneid) *activity_sceneid = LOGIC_CONFIG->GetYiZhanDaoDiConfig().GetEnterPos(&enterpos);
	return true;
}


