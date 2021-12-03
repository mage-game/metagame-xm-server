#include "randactivitylighttowerexplore.hpp"

RandActivityLightTowerExplore::RandActivityLightTowerExplore(RandActivityManager *act_manager) 
: RandActivity(act_manager, RAND_ACTIVITY_TYPE_LIGHT_TOWER_EXPLORE)
{
	m_server_draw_times = 0;	
}

RandActivityLightTowerExplore::~RandActivityLightTowerExplore()
{
	
}
void RandActivityLightTowerExplore::Init(const RandActivityData &data)
{
	RandActivity::Init(data);

	UNSTD_STATIC_CHECK((sizeof(m_server_draw_times) + sizeof(ActivityDataParam)) <= sizeof(data.data));

	int* pTemp1 = (int*)data.data;
	m_server_draw_times = *pTemp1;

	ActivityDataParam* pTemp = (ActivityDataParam*)(data.data + sizeof(m_server_draw_times));
	m_data_param = *pTemp;
}

void RandActivityLightTowerExplore::GetInitParam(RandActivityData *data)
{
	RandActivity::GetInitParam(data);

	UNSTD_STATIC_CHECK((sizeof(m_server_draw_times) + sizeof(ActivityDataParam)) <= sizeof(data->data));

	int* pTemp1 = (int*)data->data;
	*pTemp1 = m_server_draw_times;

	ActivityDataParam* pTemp = (ActivityDataParam*)(data->data + sizeof(m_server_draw_times));
	*pTemp = m_data_param;
}

void RandActivityLightTowerExplore::OnSpecialActivityStatusChange(int from_status, int to_status)
{
	RandActivity::OnSpecialActivityStatusChange(from_status, to_status);

	if (ACTIVITY_STATUS_CLOSE == to_status)
	{
		m_server_draw_times = 0;
	}
	
}

void RandActivityLightTowerExplore::AddServerDrawTimes(const int times)
{
	if (times <= 0)
	{
		return;
	}

	int temp = m_server_draw_times + times;
	if (temp > INT_MAX)
	{
		temp = INT_MAX;
	}

	m_server_draw_times = temp;
}
