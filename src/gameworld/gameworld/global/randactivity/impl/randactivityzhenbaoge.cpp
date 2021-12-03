#include "randactivityzhenbaoge.hpp"

RandActivityZhenBaoGe::RandActivityZhenBaoGe(RandActivityManager *rand_activity_manager) : RandActivity(rand_activity_manager, RAND_ACTIVITY_TYPE_ZHENBAOGE), m_server_reflush_times(0)
{

}

RandActivityZhenBaoGe::~RandActivityZhenBaoGe()
{

}

void RandActivityZhenBaoGe::Init(const RandActivityData &data)
{
	RandActivity::Init(data);

	UNSTD_STATIC_CHECK(sizeof(m_server_reflush_times) <= sizeof(data.data));

	int* pTemp = (int*)data.data;
	m_server_reflush_times = *pTemp;
}

void RandActivityZhenBaoGe::GetInitParam(RandActivityData *data)
{
	RandActivity::GetInitParam(data);

	UNSTD_STATIC_CHECK(sizeof(m_server_reflush_times) <= sizeof(data->data));

	int* pTemp = (int*)data->data;
	*pTemp = m_server_reflush_times;
}

void RandActivityZhenBaoGe::OnSpecialActivityStatusChange(int from_status, int to_status)
{
	RandActivity::OnSpecialActivityStatusChange(from_status, to_status);

	if (ACTIVITY_STATUS_OPEN == to_status)
	{
		m_server_reflush_times = 0;
	}
}

void RandActivityZhenBaoGe::AddServerReflushTimes()
{
	m_server_reflush_times += 1;
}


