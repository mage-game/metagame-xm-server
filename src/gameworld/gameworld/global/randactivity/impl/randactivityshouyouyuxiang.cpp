#include "randactivityshouyouyuxiang.hpp"
#include "config/logicconfigmanager.hpp"
#include "global/rank/rankmanager.hpp"
#include "item/itempool.h"
#include "global/guild/guildmanager.hpp"
#include "servercommon/string/gameworldstr.h"
#include "global/guild/guild.hpp"

RandActivityShouYouYuXiang::RandActivityShouYouYuXiang(RandActivityManager *rand_activity_manager) 
	: RandActivity(rand_activity_manager, RAND_ACTIVITY_TYPE_SHOUYOU_YUXIANG)
{

}

RandActivityShouYouYuXiang::~RandActivityShouYouYuXiang()
{

}

void RandActivityShouYouYuXiang::Init(const RandActivityData &data)
{
	RandActivity::Init(data);

	UNSTD_STATIC_CHECK(sizeof(m_data_param) <= sizeof(data.data));

	ActivityDataParam* pTemp = (ActivityDataParam*)data.data;
	m_data_param = *pTemp;
}

void RandActivityShouYouYuXiang::GetInitParam(RandActivityData *data)
{
	RandActivity::GetInitParam(data);

	UNSTD_STATIC_CHECK(sizeof(m_data_param) <= sizeof(data->data));

	ActivityDataParam* pTemp = (ActivityDataParam*)data->data;
	*pTemp = m_data_param;
}

void RandActivityShouYouYuXiang::OnSpecialActivityStatusChange(int from_status, int to_status)
{
	RandActivity::OnSpecialActivityStatusChange(from_status, to_status);
	
	if (ACTIVITY_STATUS_CLOSE == to_status)
	{
		m_data_param.total_flower_num = 0;
	}
}

void RandActivityShouYouYuXiang::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	m_data_param.total_flower_num = 0;
}

void RandActivityShouYouYuXiang::AddFlowerNum(int add_flower_num)
{
	if (m_data_param.total_flower_num + add_flower_num > TWO_BILLION)
	{
		m_data_param.total_flower_num = TWO_BILLION;
	}
	else
	{
		m_data_param.total_flower_num += add_flower_num;
	}
}


