#include "randactivityhuanleyaojiang2.hpp"

RandActivityHuanLeYaoJiang2::RandActivityHuanLeYaoJiang2(RandActivityManager *rand_activity_manager) : RandActivity(rand_activity_manager, RAND_ACTIVITY_TYPE_HUANLE_YAOJIANG2)
{

}

RandActivityHuanLeYaoJiang2::~RandActivityHuanLeYaoJiang2()
{

}

void RandActivityHuanLeYaoJiang2::OnSpecialActivityStatusChange(int from_status, int to_status)
{
	RandActivity::OnSpecialActivityStatusChange(from_status, to_status);

	if (ACTIVITY_STATUS_OPEN == to_status)
	{
		m_activity_data.server_chou_times = 0;
	}
	else if (ACTIVITY_STATUS_CLOSE == to_status)
	{
	}
}

void RandActivityHuanLeYaoJiang2::Init(const RandActivityData & data)
{
	UNSTD_STATIC_CHECK(sizeof(ActivityData) < RandActivityData::DATA_LENGTH);

	RandActivity::Init(data);
	ActivityData* pTemp = (ActivityData*)data.data;
	m_activity_data = *pTemp;
}

void RandActivityHuanLeYaoJiang2::GetInitParam(RandActivityData * data)
{
	UNSTD_STATIC_CHECK(sizeof(ActivityData) < RandActivityData::DATA_LENGTH);

	RandActivity::GetInitParam(data);
	ActivityData* pTemp = (ActivityData*)data->data;
	*pTemp = m_activity_data;
}
