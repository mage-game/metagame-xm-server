#include "randactivitytianmingdivination.hpp"
#include "config/logicconfigmanager.hpp"
#include "config/activityconfig/randactivityconfig.hpp"
#include "config/sharedconfig/sharedconfig.h"

RandActivityTianMingDivination::RandActivityTianMingDivination(RandActivityManager *rand_activity_manager) :RandActivity(rand_activity_manager, RAND_ACTIVITY_TYPE_TIANMING_DIVINATION)
{

}

RandActivityTianMingDivination::~RandActivityTianMingDivination()
{

}
void RandActivityTianMingDivination::Init(const RandActivityData &data)
{
	RandActivity::Init(data);

	UNSTD_STATIC_CHECK(sizeof(m_data_param) <= sizeof(data.data));

	ActivityDataParam* pTemp = (ActivityDataParam*)data.data;
	m_data_param = *pTemp;
}

void RandActivityTianMingDivination::GetInitParam(RandActivityData *data)
{
	RandActivity::GetInitParam(data);

	UNSTD_STATIC_CHECK(sizeof(m_data_param) <= sizeof(data->data));

	ActivityDataParam* pTemp = (ActivityDataParam*)data->data;
	*pTemp = m_data_param;
}

void RandActivityTianMingDivination::OnSpecialActivityStatusChange(int from_status, int to_status)
{
	RandActivity::OnSpecialActivityStatusChange(from_status, to_status);

	if (ACTIVITY_STATUS_OPEN == to_status)
	{
		bool is_get_begin_index = false;
		int open_day = GetDayIndex(SHAREDCONFIG->GetOpenServerConfig().GetServerRealStartTime(), EngineAdapter::Instance().Time()) + 1;

		is_get_begin_index = false;
		for (int index = 0; index < RAND_ACTIVITY_TIANMING_DIVINATION_REWARD_MAX_SEQ; ++index)
		{
			const RandActivityTianMingDivinationRewardConfig *cfg = LOGIC_CONFIG->GetRandActivityCfg().GetTianMingDivinationRewardCfg(index);
			if (cfg == NULL)
			{
				continue;
			}

			if (open_day > cfg->opengame_day)
			{
				continue;
			}

			if (!is_get_begin_index)
			{
				m_data_param.refresh_begin_index = index;
				is_get_begin_index = true;
				continue;
			}

			const RandActivityTianMingDivinationRewardConfig *last_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetTianMingDivinationRewardCfg(index - 1);
			if (index > 0 && NULL != last_cfg && cfg->opengame_day != last_cfg->opengame_day)
			{
				m_data_param.refresh_end_index = index;
				break;
			}
		}

		m_data_param.server_chou_times = 0;
	}
	else if (ACTIVITY_STATUS_CLOSE == to_status)
	{
		m_data_param.refresh_begin_index = 0;
		m_data_param.refresh_end_index = 0;
	}
}