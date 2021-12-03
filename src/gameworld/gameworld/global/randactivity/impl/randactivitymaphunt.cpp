#include "randactivitymaphunt.h"

RandActivityMapHunt::RandActivityMapHunt(RandActivityManager *act_mgr)
: RandActivity(act_mgr, RAND_ACTIVITY_TYPE_MAP_HUNT), m_server_total_flush_times(0)
{
	
}

RandActivityMapHunt::~RandActivityMapHunt()
{
	
}

void RandActivityMapHunt::Init(const RandActivityData &data)
{
	RandActivity::Init(data);

	int* pTemp1 = (int*)data.data;
	m_server_total_flush_times = *pTemp1;
}

void RandActivityMapHunt::GetInitParam(RandActivityData *data)
{
	RandActivity::GetInitParam(data);

	int* pTemp1 = (int*)data->data;
	*pTemp1 = m_server_total_flush_times;
}

void RandActivityMapHunt::OnSpecialActivityStatusChange(int from_status, int to_status)
{
	RandActivity::OnSpecialActivityStatusChange(from_status, to_status);
	if (ACTIVITY_STATUS_CLOSE == to_status)
	{
		m_server_total_flush_times = 0;		
	}
}