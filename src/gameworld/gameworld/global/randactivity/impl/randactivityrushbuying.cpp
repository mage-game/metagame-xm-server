#include "world.h"
#include "servercommon/string/gameworldstr.h"
#include "config/logicconfigmanager.hpp"

#include "randactivityrushbuying.hpp"
#include "config/activityconfig/randactivityconfig.hpp"

RandActivityRushBuying::RandActivityRushBuying(RandActivityManager *rand_activity_manager) : RandActivity(rand_activity_manager, RAND_ACTIVITY_TYPE_RUSH_BUYING)
{

}

RandActivityRushBuying::~RandActivityRushBuying()
{

}

void RandActivityRushBuying::OnSpecialActivityStatusChange(int from_status, int to_status)
{
	RandActivity::OnSpecialActivityStatusChange(from_status, to_status);

	if (ACTIVITY_STATUS_OPEN == to_status)
	{
		// 活动开始，刷新购买阶段(防止发传闻)，购买次数
		m_data_param.cur_phase = LOGIC_CONFIG->GetRandActivityCfg().GetRushBuyingPhase(EngineAdapter::Instance().Time());
		memset(m_data_param.server_item_buy_times, 0, sizeof(m_data_param.server_item_buy_times));
	}
}

void RandActivityRushBuying::Init(const RandActivityData &data)
{
	RandActivity::Init(data);

	UNSTD_STATIC_CHECK(sizeof(m_data_param) <= sizeof(data.data));

	ActivityDataParam* pTemp = (ActivityDataParam*)data.data;
	m_data_param = *pTemp;
}

void RandActivityRushBuying::GetInitParam(RandActivityData *data)
{
	RandActivity::GetInitParam(data);

	UNSTD_STATIC_CHECK(sizeof(m_data_param) <= sizeof(data->data));

	ActivityDataParam* pTemp = (ActivityDataParam*)data->data;
	*pTemp = m_data_param;
}

void RandActivityRushBuying::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	if (ACTIVITY_STATUS_OPEN != m_rand_activity_status)
	{
		return;
	}

	// 换天后，强制刷新购买阶段，购买次数
	m_data_param.cur_phase = LOGIC_CONFIG->GetRandActivityCfg().GetRushBuyingPhase(EngineAdapter::Instance().Time());
	memset(m_data_param.server_item_buy_times, 0, sizeof(m_data_param.server_item_buy_times));

	if (m_data_param.cur_phase >= 0)
	{
		int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_rand_activity_rush_buying_begin, RAND_ACTIVITY_TYPE_RUSH_BUYING);
		if (length > 0) 
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL_2);
		}
	}
}

void RandActivityRushBuying::Update(unsigned long interval, time_t now_second)
{
	RandActivity::Update(interval, now_second);

	if (ACTIVITY_STATUS_OPEN != m_rand_activity_status)
	{
		return;
	}

	int cur_phase = LOGIC_CONFIG->GetRandActivityCfg().GetRushBuyingPhase(now_second);

	// 刷新阶段，购买次数
	if (m_data_param.cur_phase != cur_phase)
	{
		m_data_param.cur_phase = cur_phase;
		memset(m_data_param.server_item_buy_times, 0, sizeof(m_data_param.server_item_buy_times));

		if (m_data_param.cur_phase >= 0)
		{
			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_rand_activity_rush_buying_begin, RAND_ACTIVITY_TYPE_RUSH_BUYING);
			if (length > 0)
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL_2);
			}
		}
	}
}

void RandActivityRushBuying::GetServerItemBuyTimes(short item_buy_times_list[], int list_len)
{
	if (NULL == item_buy_times_list)
	{
		return;
	}

	for (int index = 0; index < list_len && index < RA_RUSH_BUYING_SALES_ITEMS_COUNT; ++ index)
	{
		item_buy_times_list[index] = m_data_param.server_item_buy_times[index];
	}
}

int RandActivityRushBuying::GetServerItemBuyTimes(int index)
{
	int items_count = LOGIC_CONFIG->GetRandActivityCfg().GetRushBuyingSaleItemsCount(m_data_param.cur_phase);

	// 下标错误，返回最大购买次数，表明不可购买非法物品
	if (index < 0 || index >= items_count || index >= RA_RUSH_BUYING_SALES_ITEMS_COUNT)
	{
		return RA_RUSH_BUYING_MAX_SERVER_BUYING_TIMES;
	}

	return m_data_param.server_item_buy_times[index];
}

void RandActivityRushBuying::AddServerItemBuyTimes(int index)
{
	int items_count = LOGIC_CONFIG->GetRandActivityCfg().GetRushBuyingSaleItemsCount(m_data_param.cur_phase);

	if (index < 0 || index >= items_count || index >= RA_RUSH_BUYING_SALES_ITEMS_COUNT)
	{
		return;
	}

	++ m_data_param.server_item_buy_times[index];
}

