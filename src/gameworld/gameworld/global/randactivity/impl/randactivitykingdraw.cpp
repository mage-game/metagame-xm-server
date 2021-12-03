#include "randactivitykingdraw.hpp"
#include "global/randactivity/randactivitymanager.hpp"
#include "config/activityconfig/randactivityconfig.hpp"
#include "config/sharedconfig/sharedconfig.h"
#include "config/logicconfigmanager.hpp"

RandActivityKingDraw::RandActivityKingDraw(RandActivityManager *rand_activity_manager)
: RandActivity(rand_activity_manager, RAND_ACTIVITY_TYPE_KING_DRAW)
{

}

RandActivityKingDraw::~RandActivityKingDraw()
{

}

void RandActivityKingDraw::Init(const RandActivityData &data)
{
	RandActivity::Init(data);

	UNSTD_STATIC_CHECK(sizeof(m_data_param) <= sizeof(data.data));

	ActivityDataParam* pTemp = (ActivityDataParam*)data.data;
	m_data_param = *pTemp;
}

void RandActivityKingDraw::GetInitParam(RandActivityData *data)
{
	RandActivity::GetInitParam(data);

	UNSTD_STATIC_CHECK(sizeof(m_data_param) <= sizeof(data->data));

	ActivityDataParam* pTemp = (ActivityDataParam*)data->data;
	*pTemp = m_data_param;
}

void RandActivityKingDraw::OnSpecialActivityStatusChange(int from_status, int to_status)
{
	RandActivity::OnSpecialActivityStatusChange(from_status, to_status);

	if (ACTIVITY_STATUS_OPEN == to_status)
	{
		this->ClearServerChouTimes();
		//int open_day = GetDayIndex(SHAREDCONFIG->GetOpenServerConfig().GetServerRealStartTime(), EngineAdapter::Instance().Time()) + 1;
		//for (int level = 0; level < RA_KING_DRAW_LEVEL_COUNT; ++level)
		//{
		//	bool is_get_begin_index = false;
		//	int count = LOGIC_CONFIG->GetRandActivityCfg().GetKingDrwaRewardCount(level);
		//	for (int index = 0; index < count && index < RandActivityKingDrawLevelConfig::KING_DRAW_REWARD_COUNT_PER_LEVEL; ++index)
		//	{
		//		int opengame_day = LOGIC_CONFIG->GetRandActivityCfg().GetKingDrawOpenGameDay(level, index);

		//		if (opengame_day > 0)
		//		{
		//			if (open_day > opengame_day)
		//			{
		//				continue;
		//			}

		//			if (!is_get_begin_index)
		//			{
		//				m_data_param.reward_begin_index[level] = index;
		//				is_get_begin_index = true;
		//				continue;
		//			}

		//			if (index > 0)
		//			{
		//				int last_opengame_day = LOGIC_CONFIG->GetRandActivityCfg().GetKingDrawOpenGameDay(level, index - 1);
		//				if (last_opengame_day > 0 && last_opengame_day != opengame_day)
		//				{
		//					m_data_param.reward_end_index[level] = index;
		//					break;
		//				}
		//			}
		//		}
		//	}
		//}

		//for (int level = 0; level < RA_KING_DRAW_LEVEL_COUNT; ++level)
		//{
		//	bool is_get_begin_index = false;
		//	int count = LOGIC_CONFIG->GetRandActivityCfg().GetKingDrawReturnRewardCfgCount(level);
		//	for (int index = 0; index < count && index < RandActivityKingDrawReturnRewardCfg::KING_DRAW_RETURN_REWARD_COUNT_PER_LEVEL; ++index)
		//	{
		//		const RandActivityKingDrawReturnRewardCfg *cfg = LOGIC_CONFIG->GetRandActivityCfg().GetKingDrawReturnRewardCfg(level, index);
		//		if (NULL == cfg)
		//		{
		//			continue;
		//		}

		//		if (open_day > cfg->opengame_day)
		//		{
		//			continue;
		//		}

		//		if (!is_get_begin_index)
		//		{
		//			m_data_param.return_reward_begin_index[level] = index;
		//			is_get_begin_index = true;
		//			continue;
		//		}

		//		if (index > 0)
		//		{
		//			const RandActivityKingDrawReturnRewardCfg *last_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetKingDrawReturnRewardCfg(level, index - 1);
		//			if (NULL != last_cfg && last_cfg->opengame_day != cfg->opengame_day)
		//			{
		//				m_data_param.return_reward_end_index[level] = index;
		//				break;
		//			}
		//		}
		//	}
		//}
	}
	else if (ACTIVITY_STATUS_CLOSE == to_status)
	{
		m_data_param.Reset();
	}
}

int RandActivityKingDraw::GetRewardBeginIndex(int level)
{
	if (level < 0 || level >= RA_KING_DRAW_LEVEL_COUNT)
	{
		return -1;
	}
	
	return m_data_param.reward_begin_index[level];
}

int RandActivityKingDraw::GetRewardEndIndex(int level)
{
	if (level < 0 || level >= RA_KING_DRAW_LEVEL_COUNT)
	{
		return -1;
	}

	return m_data_param.reward_end_index[level];
}

int RandActivityKingDraw::GetReturnRewardBeginIndex(int level)
{
	if (level < 0 || level >= RA_KING_DRAW_LEVEL_COUNT)
	{
		return -1;
	}

	return m_data_param.return_reward_begin_index[level];
}

int RandActivityKingDraw::GetReturnRewardEndIndex(int level)
{
	if (level < 0 || level >= RA_KING_DRAW_LEVEL_COUNT)
	{
		return -1;
	}

	return m_data_param.return_reward_end_index[level];
}

