#include "randactivitypromotingposition.hpp"
#include "obj/character/role.h"
#include "config/logicconfigmanager.hpp"
#include "config/activityconfig/randactivityconfig.hpp"
#include "config/sharedconfig/sharedconfig.h"

RandActivityPromotingPosition::RandActivityPromotingPosition(RandActivityManager *rand_activity_manager) 
: RandActivity(rand_activity_manager, RAND_ACTIVITY_TYPE_PROMOTING_POSITION), m_records_count(0), m_records_last_index(-1)
{

}

RandActivityPromotingPosition::~RandActivityPromotingPosition()
{

}

void RandActivityPromotingPosition::Init(const RandActivityData &data)
{
	RandActivity::Init(data);

	UNSTD_STATIC_CHECK(sizeof(m_data_param) <= sizeof(data.data));

	ActivityDataParam* pTemp = (ActivityDataParam*)data.data;
	m_data_param = *pTemp;
}

void RandActivityPromotingPosition::GetInitParam(RandActivityData *data)
{
	RandActivity::GetInitParam(data);

	UNSTD_STATIC_CHECK(sizeof(m_data_param) <= sizeof(data->data));

	ActivityDataParam* pTemp = (ActivityDataParam*)data->data;
	*pTemp = m_data_param;
}

void RandActivityPromotingPosition::OnSpecialActivityStatusChange(int from_status, int to_status)
{
	RandActivity::OnSpecialActivityStatusChange(from_status, to_status);

	if (ACTIVITY_STATUS_OPEN == to_status)
	{
		m_records_count = 0;
		m_records_last_index = -1;
		memset(m_records_list, 0, sizeof(m_records_list));

		m_data_param.server_chou_times = 0;

		bool is_get_begin_index = false;
		int open_day = GetDayIndex(SHAREDCONFIG->GetOpenServerConfig().GetServerRealStartTime(), EngineAdapter::Instance().Time()) + 1;
		for (int index = 0; index < LOGIC_CONFIG->GetRandActivityCfg().GetPromotingPositionRewardCfgCount() && index < RA_PROMOTION_POSITION_REWARD_CFG_MAX_COUNT; ++index)
		{
			const RandActivityPromotingPositionRewardCfg *reward_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetPromotingPositionRewardCfg(index);
			if (reward_cfg == NULL)
			{
				continue;
			}

			if (open_day > reward_cfg->opengame_day)
			{
				continue;
			}

			if (!is_get_begin_index)
			{
				m_data_param.reward_begin_index = index;
				is_get_begin_index = true;
				continue;
			}

			const RandActivityPromotingPositionRewardCfg *last_reward_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetPromotingPositionRewardCfg(index - 1);
			if (index > 0 && NULL != last_reward_cfg && reward_cfg->opengame_day != last_reward_cfg->opengame_day)
			{
				m_data_param.reward_end_index = index;
				break;
			}
		}

		if (0 == m_data_param.reward_end_index)
		{
			m_data_param.reward_end_index = LOGIC_CONFIG->GetRandActivityCfg().GetPromotingPositionRewardCfgCount();
		}

		is_get_begin_index = false;
		for (int index = 0; index < LOGIC_CONFIG->GetRandActivityCfg().GetPromotingPositionOutsideCfgCount() && 
			index < RA_PROMOTING_POSITION_OUTSIDE_REWARD_MAX_SEQ; ++index)
		{
			const RandActivityPromotingPositionConfig *outside_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetPromotingPositionOutsideCfg(index);
			if (outside_cfg == NULL)
			{
				continue;
			}

			if (open_day > outside_cfg->opengame_day)
			{
				continue;
			}

			if (!is_get_begin_index)
			{
				m_data_param.outside_begin_index = index;
				is_get_begin_index = true;
				continue;
			}

			const RandActivityPromotingPositionConfig *last_outside_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetPromotingPositionOutsideCfg(index - 1);
			if (index > 0 && NULL != last_outside_cfg && outside_cfg->opengame_day != last_outside_cfg->opengame_day)
			{
				m_data_param.outside_end_index = index;
				break;
			}
		}

		is_get_begin_index = false;
		for (int index = 0; index < LOGIC_CONFIG->GetRandActivityCfg().GetPromotingPositionInsideCfgCount() &&
			index < RA_PROMOTING_POSITION_INSIDE_REWARD_MAX_SEQ; ++index)
		{
			const RandActivityPromotingPositionConfig *inside_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetPromotingPositionInsideCfg(index);
			if (inside_cfg == NULL)
			{
				continue;
			}

			if (open_day > inside_cfg->opengame_day)
			{
				continue;
			}

			if (!is_get_begin_index)
			{
				m_data_param.inside_begin_index = index;
				is_get_begin_index = true;
				continue;
			}

			const RandActivityPromotingPositionConfig *last_inside_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetPromotingPositionInsideCfg(index - 1);
			if (index > 0 && NULL != last_inside_cfg && inside_cfg->opengame_day != last_inside_cfg->opengame_day)
			{
				m_data_param.inside_end_index = index;
				break;
			}
		}
	}
	else if (ACTIVITY_STATUS_CLOSE == to_status)
	{
		m_data_param.reward_begin_index = 0;
		m_data_param.reward_end_index = 0;
		m_data_param.outside_begin_index = 0;
		m_data_param.outside_end_index = 0;
		m_data_param.inside_begin_index = 0;
		m_data_param.inside_end_index = 0;
	}
}

int RandActivityPromotingPosition::GetRewardRecordsList(RAPromotingPositionRewardRecord *records_list)
{
	if (NULL == records_list)
	{
		return 0;
	}

	if (m_records_last_index < 0 || m_records_last_index >= RA_PROMOTION_POSITION_REWARD_RECORDS_MAX_COUNT)
	{
		return 0;
	}

	int real_count = 0;
	for (int index = m_records_last_index + RA_PROMOTION_POSITION_REWARD_RECORDS_MAX_COUNT; index > m_records_last_index; -- index)
	{
		if (real_count >= m_records_count)
		{
			break;
		}

		records_list[real_count ++] = m_records_list[index % RA_PROMOTION_POSITION_REWARD_RECORDS_MAX_COUNT];
	}

	return real_count;
}

void RandActivityPromotingPosition::AddRewardRecord(Role * role, RAPromotingPositionRewardInfo &reward_info)
{
	if (NULL == role)
	{
		return;
	}

	++ m_records_last_index;
	if (m_records_last_index >= RA_PROMOTION_POSITION_REWARD_RECORDS_MAX_COUNT)
	{
		m_records_last_index = 0;
	}

	m_records_list[m_records_last_index].reward_info = reward_info;
	m_records_list[m_records_last_index].reserve_sh = 0;
	m_records_list[m_records_last_index].uid = role->GetUID();
	F_STRNCPY(m_records_list[m_records_last_index].user_name, role->GetName(), sizeof(GameName));

	if (m_records_count < RA_PROMOTION_POSITION_REWARD_RECORDS_MAX_COUNT)
	{
		++ m_records_count;
	}
}
