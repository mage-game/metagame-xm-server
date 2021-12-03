#include "randactivityguagua.hpp"
#include "global/randactivity/randactivitymanager.hpp"

RandActivityGuaGua::RandActivityGuaGua(RandActivityManager *rand_activity_manager)
	: RandActivity(rand_activity_manager, RAND_ACTIVITY_TYPE_GUAGUA)
{

}

RandActivityGuaGua::~RandActivityGuaGua()
{

}

void RandActivityGuaGua::Init(const RandActivityData &data)
{
	UNSTD_STATIC_CHECK(sizeof(ActivityData) < RandActivityData::DATA_LENGTH);

	RandActivity::Init(data);
	ActivityData* pTemp = (ActivityData*)data.data;
	m_activity_data = *pTemp;
}

void RandActivityGuaGua::GetInitParam(RandActivityData *data)
{
	UNSTD_STATIC_CHECK(sizeof(ActivityData) < RandActivityData::DATA_LENGTH);

	RandActivity::GetInitParam(data);
	ActivityData* pTemp = (ActivityData*)data->data;
	*pTemp = m_activity_data;
}

void RandActivityGuaGua::OnSpecialActivityStatusChange(int from_status, int to_status)
{
	RandActivity::OnSpecialActivityStatusChange(from_status, to_status);

	if (ACTIVITY_STATUS_OPEN == to_status)
	{
		m_activity_data.server_roll_times = 0;
	}
}
