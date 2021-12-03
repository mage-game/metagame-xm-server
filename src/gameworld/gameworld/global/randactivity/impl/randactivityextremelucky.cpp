#include "randactivityextremelucky.hpp"
#include "config/logicconfigmanager.hpp"
#include "config/activityconfig/randactivityconfig.hpp"
#include "config/sharedconfig/sharedconfig.h"

RandActivityExtremeLucky::RandActivityExtremeLucky(RandActivityManager *rand_activity_manager) : 
RandActivity(rand_activity_manager, RAND_ACTIVITY_TYPE_EXTREME_LUCKY)
{
	
}

RandActivityExtremeLucky::~RandActivityExtremeLucky()
{

}

void RandActivityExtremeLucky::Init(const RandActivityData &data)
{
	RandActivity::Init(data);

	UNSTD_STATIC_CHECK(sizeof(m_data_param) <= sizeof(data.data));

	ActivityDataParam* pTemp = (ActivityDataParam*)data.data;
	m_data_param = *pTemp;
}

void RandActivityExtremeLucky::GetInitParam(RandActivityData *data)
{
	RandActivity::GetInitParam(data);

	UNSTD_STATIC_CHECK(sizeof(m_data_param) <= sizeof(data->data));

	ActivityDataParam* pTemp = (ActivityDataParam*)data->data;
	*pTemp = m_data_param;
}

void RandActivityExtremeLucky::OnSpecialActivityStatusChange(int from_status, int to_status)
{
	RandActivity::OnSpecialActivityStatusChange(from_status, to_status);

	if (ACTIVITY_STATUS_OPEN == to_status)
	{
		m_data_param.server_flush_times = 0;
		//bool is_get_begin_index = false;
		//int open_day = GetDayIndex(SHAREDCONFIG->GetOpenServerConfig().GetServerRealStartTime(), EngineAdapter::Instance().Time()) + 1;
		//for (int index = 0; index < LOGIC_CONFIG->GetRandActivityCfg().GetExtremeLuckyReturnRewardCount() && index < RA_EXTREME_LUCKY_RETURN_REWARD_CFG_COUNT; ++index)
		//{
		//	const RandActivityExtremeLuckyReturnRewardCfg *reward_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetExtremeLuckyReturnRewardCfg(index);
		//	if (reward_cfg == NULL)
		//	{
		//		continue;
		//	}

		//	if (open_day > reward_cfg->opengame_day)
		//	{
		//		continue;
		//	}

		//	if (!is_get_begin_index)
		//	{
		//		m_data_param.reward_begin_index = index;
		//		is_get_begin_index = true;
		//		continue;
		//	}

		//	if (index > 0 && NULL != LOGIC_CONFIG->GetRandActivityCfg().GetExtremeLuckyReturnRewardCfg(index - 1)
		//		&& reward_cfg->opengame_day != LOGIC_CONFIG->GetRandActivityCfg().GetExtremeLuckyReturnRewardCfg(index - 1)->opengame_day)
		//	{
		//		m_data_param.reward_end_index = index;
		//		break;
		//	}
		//}
	}
	else if (ACTIVITY_STATUS_CLOSE == to_status)
	{
		m_data_param.reward_begin_index = 0;
		m_data_param.reward_end_index = 0;
	}
}