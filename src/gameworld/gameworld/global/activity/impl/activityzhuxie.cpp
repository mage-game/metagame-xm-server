#include "activityzhuxie.hpp"
#include "engineadapter.h"
#include "protocal/msgother.h"
#include "global/worldstatus/worldstatus.hpp"
#include "config/logicconfigmanager.hpp"
#include "config/activityconfig/zhuxieconfig.hpp"

ActivityZhuXie::ActivityZhuXie(ActivityManager *activity_manager)
	: Activity(activity_manager, ACTIVITY_TYPE_ZHUXIE)
{
	
}

ActivityZhuXie::~ActivityZhuXie()
{

}

void ActivityZhuXie::OnActivityStatusChange(int from_status, int to_status)
{
	if (ACTIVITY_STATUS_CLOSE == to_status)
	{

	}

	Activity::OnActivityStatusChange(from_status, to_status);
}

bool ActivityZhuXie::IsRecordRoomStatus(int *activity_sceneid)
{
	if (NULL != activity_sceneid)
	{
		*activity_sceneid = LOGIC_CONFIG->GetZhuXieConfig().GetZhuXieFBEnterInfo(NULL);
	}

	return true;
}

