#include "randactivitymine.hpp"
#include "config/logicconfigmanager.hpp"
#include "config/activityconfig/randactivityconfig.hpp"
#include "world.h"
#include "servercommon/string/gameworldstr.h"
#include "config/sharedconfig/sharedconfig.h"

RandActivityMine::RandActivityMine(RandActivityManager *rand_activity_manager)
: RandActivity(rand_activity_manager, RAND_ACTIVITY_TYPE_MINE)
{

}

RandActivityMine::~RandActivityMine()
{

}

void RandActivityMine::Init(const RandActivityData &data)
{
	RandActivity::Init(data);

	UNSTD_STATIC_CHECK(sizeof(m_data_param) <= sizeof(data.data));

	ActivityDataParam* pTemp = (ActivityDataParam*)data.data;
	m_data_param = *pTemp;
}

void RandActivityMine::GetInitParam(RandActivityData *data)
{
	RandActivity::GetInitParam(data);

	UNSTD_STATIC_CHECK(sizeof(m_data_param) <= sizeof(data->data));

	ActivityDataParam* pTemp = (ActivityDataParam*)data->data;
	*pTemp = m_data_param;
}

void RandActivityMine::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	//if (old_dayid == now_dayid) return;

	//int max_reward_seq = LOGIC_CONFIG->GetRandActivityCfg().GetMineServerRewardCfgCount() - 1;
	//const RandActivityMineServerRewardConfig *max_reward_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetMineServerRewardCfg(max_reward_seq);
	//if (max_reward_cfg == NULL)
	//{
	//	return;
	//}

	//// 换矿次数超过最大次数，则清零
	//if (m_data_param.cur_total_refresh_times >= max_reward_cfg->total_refresh_times)
	//{
	//	m_data_param.cur_total_refresh_times = 0;
	//	m_data_param.is_pre_day_clear_refresh_times = 1;
	//}
	//else
	//{
	//	m_data_param.is_pre_day_clear_refresh_times = 0;
	//}
}

void RandActivityMine::OnSpecialActivityStatusChange(int from_status, int to_status)
{
	RandActivity::OnSpecialActivityStatusChange(from_status, to_status);

	if (ACTIVITY_STATUS_OPEN == to_status)
	{
		m_data_param.cur_total_refresh_times = 0;
		//m_data_param.cur_total_refresh_times = 0;
		//m_data_param.is_pre_day_clear_refresh_times = 0;

		//bool is_get_begin_index = false;
		//int open_day = GetDayIndex(SHAREDCONFIG->GetOpenServerConfig().GetServerRealStartTime(), EngineAdapter::Instance().Time()) + 1;
		//for (int index = 0; index < LOGIC_CONFIG->GetRandActivityCfg().GetMineServerRewardCfgCount() && index < RA_MINE_REWARD_MAX_COUNT; ++index)
		//{
		//	const RandActivityMineServerRewardConfig *reward_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetMineServerRewardCfg(index);
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

		//	if (index > 0 && NULL != LOGIC_CONFIG->GetRandActivityCfg().GetMineServerRewardCfg(index - 1)
		//		&& reward_cfg->opengame_day != LOGIC_CONFIG->GetRandActivityCfg().GetMineServerRewardCfg(index - 1)->opengame_day)
		//	{
		//		m_data_param.reward_end_index = index;
		//		break;
		//	}
		//}

		//is_get_begin_index = false;
		//for (int index = 0; index < LOGIC_CONFIG->GetRandActivityCfg().GetMineInfoCfgCount() && index < RA_MINE_MAX_CFG_COUNT; ++index)
		//{
		//	const RandActivityMineInfoConfig *mine_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetMineInfoCfg(index);
		//	if (mine_cfg == NULL)
		//	{
		//		continue;
		//	}

		//	if (open_day > mine_cfg->opengame_day)
		//	{
		//		continue;
		//	}

		//	if (!is_get_begin_index)
		//	{
		//		m_data_param.refresh_begin_index = index;
		//		is_get_begin_index = true;
		//		continue;
		//	}

		//	const RandActivityMineInfoConfig *last_mine_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetMineInfoCfg(index - 1);
		//	if (index > 0 && NULL != last_mine_cfg && mine_cfg->opengame_day != last_mine_cfg->opengame_day)
		//	{
		//		m_data_param.refresh_end_index = index;
		//		break;
		//	}
		//}
	}
	else if (ACTIVITY_STATUS_CLOSE == to_status)
	{
		m_data_param.reward_begin_index = 0;
		m_data_param.reward_end_index = 0;
		m_data_param.refresh_begin_index = 0;
		m_data_param.refresh_end_index = 0;
	}
}

void RandActivityMine::AddTotalRefreshTimes()
{
	//int next_reward_level = 0;	// 下一个全服奖励的级别

	//for (int index = 0; index < LOGIC_CONFIG->GetRandActivityCfg().GetMineServerRewardCfgCount(); ++ index)
	//{
	//	const RandActivityMineServerRewardConfig *reward_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetMineServerRewardCfg(index);
	//	if (reward_cfg == NULL)
	//	{
	//		continue;
	//	}

	//	if (m_data_param.cur_total_refresh_times < reward_cfg->total_refresh_times)
	//	{
	//		next_reward_level = index;
	//		break;
	//	}
	//}

	++ m_data_param.cur_total_refresh_times;

	//const RandActivityMineServerRewardConfig *next_reward_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetMineServerRewardCfg(next_reward_level);
	//if (next_reward_cfg == NULL)
	//{
	//	return;
	//}

	//if (m_data_param.cur_total_refresh_times == next_reward_cfg->total_refresh_times)
	//{
	//	// 达到了新级别的换矿次数，发传闻
	//	int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_rand_activity_mine_server_reward_content, 
	//		m_data_param.cur_total_refresh_times, LOGIC_CONFIG->GetRandActivityCfg().GetMineFetchServerRewardLevel());
	//	if (length > 0) 
	//	{
	//		World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE);
	//	}
	//}
}
