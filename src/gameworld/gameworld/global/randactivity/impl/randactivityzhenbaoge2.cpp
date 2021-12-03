#include "randactivityzhenbaoge2.hpp"

RandActivityZhenBaoGe2::RandActivityZhenBaoGe2(RandActivityManager *rand_activity_manager) : 
RandActivity(rand_activity_manager, RAND_ACTIVITY_TYPE_ZHENBAOGE2), m_server_reflush_times(0)
{

}

RandActivityZhenBaoGe2::~RandActivityZhenBaoGe2()
{

}

void RandActivityZhenBaoGe2::Init(const RandActivityData &data)
{
	RandActivity::Init(data);

	UNSTD_STATIC_CHECK(sizeof(m_server_reflush_times) <= sizeof(data.data));

	int* pTemp = (int*)data.data;
	m_server_reflush_times = *pTemp;
}

void RandActivityZhenBaoGe2::GetInitParam(RandActivityData *data)
{
	RandActivity::GetInitParam(data);

	UNSTD_STATIC_CHECK(sizeof(m_server_reflush_times) <= sizeof(data->data));

	int* pTemp = (int*)data->data;
	*pTemp = m_server_reflush_times;
}

void RandActivityZhenBaoGe2::OnSpecialActivityStatusChange(int from_status, int to_status)
{
	RandActivity::OnSpecialActivityStatusChange(from_status, to_status);

	if (ACTIVITY_STATUS_OPEN == to_status)
	{
		m_server_reflush_times = 0;
	}
}

void RandActivityZhenBaoGe2::AddServerReflushTimes()
{
	m_server_reflush_times += 1;
}


