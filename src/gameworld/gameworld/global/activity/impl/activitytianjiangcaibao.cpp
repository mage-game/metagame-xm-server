#include "activitytianjiangcaibao.h"
#include "engineadapter.h"
#include "protocal/msgother.h"
#include "global/worldstatus/worldstatus.hpp"
#include "config/logicconfigmanager.hpp"
#include "config/activityconfig/tianjiangcaibaoconfig.h"

ActivityTianJianbCaiBao::ActivityTianJianbCaiBao(ActivityManager *activity_manager)
: Activity(activity_manager, ACTIVITY_TYPE_TIANJIANGCAIBAO)
{

}

ActivityTianJianbCaiBao::~ActivityTianJianbCaiBao()
{

}

void ActivityTianJianbCaiBao::OnActivityStatusChange(int from_status, int to_status)
{
	if (ACTIVITY_STATUS_CLOSE == to_status)
	{
		WorldStatus::Instance().OnActivityFinish(ACTIVITY_FIND_TYPE_TIANJIANGCAIBAO);
	}

	Activity::OnActivityStatusChange(from_status, to_status);
}

bool ActivityTianJianbCaiBao::IsRecordRoomStatus(int *activity_sceneid)
{
	if (NULL != activity_sceneid)
	{
		*activity_sceneid = LOGIC_CONFIG->GetTianJiangCaiBaoConfig().GetTianJiangCaiBaoEnterInfo();
	}

	return true;
}

