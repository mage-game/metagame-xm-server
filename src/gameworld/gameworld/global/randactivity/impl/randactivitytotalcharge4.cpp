#include "randactivitytotalcharge4.hpp"



RandActivityTotalCharge4::RandActivityTotalCharge4(RandActivityManager *rand_activity_manager)
	: RandActivity(rand_activity_manager, RAND_ACTIVITY_TYPE_TOTAL_CHARGE_4)
{
}

RandActivityTotalCharge4::~RandActivityTotalCharge4()
{

}

void RandActivityTotalCharge4::Init(const RandActivityData &data)
{
	RandActivity::Init(data);
	UNSTD_STATIC_CHECK(sizeof(m_charge4_data_param) <= sizeof(data.data));

	ChargeRandActivityDataParam* pTemp = (ChargeRandActivityDataParam*)data.data;
	m_charge4_data_param = *pTemp;
}

void RandActivityTotalCharge4::GetInitParam(RandActivityData *data)
{
	RandActivity::GetInitParam(data);
	UNSTD_STATIC_CHECK(sizeof(m_charge4_data_param) <= sizeof(data->data));

	ChargeRandActivityDataParam* pTemp = (ChargeRandActivityDataParam*)data->data;
	*pTemp = m_charge4_data_param;
}

void RandActivityTotalCharge4::OnUseReward()
{
	if (m_charge4_data_param.activity_reward_remain < MAX_REWARD)
	{
		m_charge4_data_param.activity_reward_remain++;
	}
	else 
	{
		return;
	}
}

void RandActivityTotalCharge4::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	if (old_dayid != now_dayid)
	{
		m_charge4_data_param.activity_reward_remain = 0;
	}
}

short RandActivityTotalCharge4::GetRemainReward()
{
	return m_charge4_data_param.activity_reward_remain;
}