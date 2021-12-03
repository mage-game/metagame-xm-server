#include "randactivitycracybuy.hpp"
#include "world.h"
#include "global/randactivity/randactivitymanager.hpp"
#include "config/logicconfigmanager.hpp"
#include "global/rank/rankmanager.hpp"
#include "other/route/mailroute.hpp"
#include "servercommon/string/gameworldstr.h"
#include "item/itempool.h"
#include "global/usercache/usercache.hpp"
#include "gamelog.h"

RandActivityCracyBuyToAll::RandActivityCracyBuyToAll(RandActivityManager *rand_activity_manager)
	: RandActivity(rand_activity_manager, RAND_ACTIVITY_TYPE_CRACYBUY)
{
	
}

RandActivityCracyBuyToAll::~RandActivityCracyBuyToAll()
{

}

void RandActivityCracyBuyToAll::Init(const RandActivityData &data)
{
	RandActivity::Init(data);

	UNSTD_STATIC_CHECK(sizeof(m_data_param) <= sizeof(data.data));

	ActivityDataParam* pTemp = (ActivityDataParam*)data.data;
	m_data_param = *pTemp;
}

void RandActivityCracyBuyToAll::GetInitParam(RandActivityData *data)
{
	RandActivity::GetInitParam(data);

	UNSTD_STATIC_CHECK(sizeof(m_data_param) <= sizeof(data->data));

	ActivityDataParam* pTemp = (ActivityDataParam*)data->data;
	*pTemp = m_data_param;
}

void RandActivityCracyBuyToAll::OnSpecialActivityStatusChange(int from_status, int to_status)
{
	RandActivity::OnSpecialActivityStatusChange(from_status, to_status);

	if (ACTIVITY_STATUS_OPEN == to_status)
	{
		memset(m_data_param.m_to_all_buy_limit, 0, sizeof(m_data_param.m_to_all_buy_limit));
	}
	else if (ACTIVITY_STATUS_CLOSE == to_status)
	{
	}
}

void RandActivityCracyBuyToAll::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	RandActivity::OnDayChange(old_dayid, now_dayid);

	if (old_dayid == now_dayid) return;

	memset(m_data_param.m_to_all_buy_limit, 0, sizeof(m_data_param.m_to_all_buy_limit));
}