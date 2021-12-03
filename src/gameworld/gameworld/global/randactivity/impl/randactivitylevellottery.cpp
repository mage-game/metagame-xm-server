#include "randactivitylevellottery.hpp"
#include "global/randactivity/randactivitymanager.hpp"
#include "obj/character/role.h"

RandActivityLevelLottery::RandActivityLevelLottery(RandActivityManager *rand_activity_manager)
: RandActivity(rand_activity_manager, RAND_ACTIVITY_TYPE_LEVEL_LOTTERY), m_history_count(0), m_history_last_index(-1)
{

}

RandActivityLevelLottery::~RandActivityLevelLottery()
{

}

void RandActivityLevelLottery::OnSpecialActivityStatusChange(int from_status, int to_status)
{
	RandActivity::OnSpecialActivityStatusChange(from_status, to_status);

	if (ACTIVITY_STATUS_OPEN == to_status)
	{
		m_history_last_index = -1;
		m_history_count = 0;
		this->ClearServerChouTimes();
	}
	//else if (ACTIVITY_STATUS_CLOSE == to_status)
	//{
	//}
}

void RandActivityLevelLottery::Init(const RandActivityData &data)
{
	UNSTD_STATIC_CHECK(sizeof(ActivityData) < RandActivityData::DATA_LENGTH);

	RandActivity::Init(data);
	ActivityData* pTemp = (ActivityData*)data.data;
	m_activity_data = *pTemp;
}

void RandActivityLevelLottery::GetInitParam(RandActivityData *data)
{
	UNSTD_STATIC_CHECK(sizeof(ActivityData) < RandActivityData::DATA_LENGTH);

	RandActivity::GetInitParam(data);
	ActivityData* pTemp = (ActivityData*)data->data;
	*pTemp = m_activity_data;
}

int RandActivityLevelLottery::GetLotteryHistoryList(RALevelLotteryHistoryItem *history_list)
{
	if (NULL == history_list)
	{
		return 0;
	}

	if (m_history_last_index < 0 || m_history_last_index >= RA_LEVEL_LOTTERY_HISTORY_MAX_COUNT)
	{
		return 0;
	}

	int real_count = 0;
	for (int index = m_history_last_index + RA_LEVEL_LOTTERY_HISTORY_MAX_COUNT; index > m_history_last_index; -- index)
	{
		if (real_count >= m_history_count)
		{
			break;
		}

		history_list[real_count ++] = m_history_list[index % RA_LEVEL_LOTTERY_HISTORY_MAX_COUNT];
	}

	return real_count;
}

void RandActivityLevelLottery::AddLotteryHistoryItem(Role *role, int reward_index)
{
	if (NULL == role || reward_index < 0)
	{
		return;
	}

	++ m_history_last_index;
	if (m_history_last_index >= RA_LEVEL_LOTTERY_HISTORY_MAX_COUNT)
	{
		m_history_last_index = 0;
	}

	m_history_list[m_history_last_index].reward_index = reward_index;
	m_history_list[m_history_last_index].uid = role->GetUID();
	F_STRNCPY(m_history_list[m_history_last_index].user_name, role->GetName(), sizeof(GameName));

	if (m_history_count < RA_LEVEL_LOTTERY_HISTORY_MAX_COUNT)
	{
		++ m_history_count;
	}
}

