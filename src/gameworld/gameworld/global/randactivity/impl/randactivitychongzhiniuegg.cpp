#include "randactivitychongzhiniuegg.hpp"
#include "obj/character/role.h"

RandActivityChongzhiNiuEgg::RandActivityChongzhiNiuEgg(RandActivityManager *rand_activity_manager)
: RandActivity(rand_activity_manager, RAND_ACTIVITY_TYPE_CHONGZHI_NIU_EGG), m_server_total_niu_egg_times(0), m_history_count(0), m_history_last_index(0)
{

}

RandActivityChongzhiNiuEgg::~RandActivityChongzhiNiuEgg()
{

}

void RandActivityChongzhiNiuEgg::Init(const RandActivityData &data)
{
	RandActivity::Init(data);

	UNSTD_STATIC_CHECK(sizeof(m_server_total_niu_egg_times) <= sizeof(data.data));

	int* pTemp = (int*)data.data;
	m_server_total_niu_egg_times = *pTemp;
}

void RandActivityChongzhiNiuEgg::GetInitParam(RandActivityData *data)
{
	RandActivity::GetInitParam(data);

	UNSTD_STATIC_CHECK(sizeof(m_server_total_niu_egg_times) <= sizeof(data->data));

	int* pTemp = (int*)data->data;
	*pTemp = m_server_total_niu_egg_times;

}

void RandActivityChongzhiNiuEgg::AddTotalNiuEggTimes(int times)
{
	if (times > 0)
	{
		m_server_total_niu_egg_times += times;
	}
}
void RandActivityChongzhiNiuEgg::OnSpecialActivityStatusChange(int from_status, int to_status)
{
	RandActivity::OnSpecialActivityStatusChange(from_status, to_status);

	if (ACTIVITY_STATUS_OPEN == to_status)
	{
		m_server_total_niu_egg_times = 0;
	}
	else if (ACTIVITY_STATUS_CLOSE == to_status)
	{

		m_history_count = 0;

		for (int i = 0; i < RA_NIU_EGG_CHOU_HISTORY_MAX_COUNT; ++ i)
		{
			m_history_list[i].Reset();
		}
	}

}

int RandActivityChongzhiNiuEgg::GetNiueggChouHistoryList(RAChongzhiNiuEggHistoryItem *history_list)
{
	if (NULL == history_list)
	{
		return 0;
	}

	if (m_history_last_index < 0 || m_history_last_index >= RA_NIU_EGG_CHOU_HISTORY_MAX_COUNT)
	{
		return 0;
	}

	int real_count = 0;
	for (int index = m_history_last_index + RA_NIU_EGG_CHOU_HISTORY_MAX_COUNT; index > m_history_last_index; -- index)
	{
		if (real_count >= m_history_count)
		{
			break;
		}

		history_list[real_count ++] = m_history_list[index % RA_NIU_EGG_CHOU_HISTORY_MAX_COUNT];
	}

	return real_count;
}

void RandActivityChongzhiNiuEgg::AddNiuEggChouHistoryItem(Role *role, int reward_req)
{
	if (NULL == role || reward_req < 0 || reward_req >= RA_NIU_EGG_CHOU_REWARD_CFG_COUNT_LIMIT)
	{
		return;
	}

	++ m_history_last_index;
	if (m_history_last_index >= RA_NIU_EGG_CHOU_HISTORY_MAX_COUNT)
	{
		m_history_last_index = 0;
	}

	m_history_list[m_history_last_index].reward_req = reward_req;
	m_history_list[m_history_last_index].uid = role->GetUID();
	F_STRNCPY(m_history_list[m_history_last_index].user_name, role->GetName(), sizeof(GameName));

	if (m_history_count < RA_NIU_EGG_CHOU_HISTORY_MAX_COUNT)
	{
		++ m_history_count;
	}
}

