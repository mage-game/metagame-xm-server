#include "randactivityarmyday.hpp"

RandActivityArmyDay::RandActivityArmyDay(RandActivityManager *rand_activity_manager)
: RandActivity(rand_activity_manager, RAND_ACTIVITY_TYPE_ARMY_DAY)
{
	m_win_army_side = INVALID_ARMY_SIDE;

	memset(m_all_army_score_list, 0, sizeof(m_all_army_score_list));
}

RandActivityArmyDay::~RandActivityArmyDay()
{
	
}

void RandActivityArmyDay::Init(const RandActivityData &data)
{
	RandActivity::Init(data);

	UNSTD_STATIC_CHECK(sizeof(m_all_army_score_list) <= sizeof(data.data));
	memcpy(m_all_army_score_list, data.data, sizeof(m_all_army_score_list));
}

void RandActivityArmyDay::GetInitParam(RandActivityData *data)
{
	RandActivity::GetInitParam(data);

	UNSTD_STATIC_CHECK(sizeof(m_all_army_score_list) <= sizeof(data->data));
	memcpy(data->data, m_all_army_score_list, sizeof(m_all_army_score_list));
}

void RandActivityArmyDay::OnSpecialActivityStatusChange(int from_status, int to_status)
{
	if (ACTIVITY_STATUS_OPEN == to_status)
	{
		m_win_army_side = INVALID_ARMY_SIDE;

		for (int i = 0; i < RA_ARMY_DAY_ARMY_SIDE_NUM; ++ i)
		{
			memset(m_all_army_score_list, 0, sizeof(m_all_army_score_list));
		}
	}

	if (ACTIVITY_STATUS_CLOSE == to_status)
	{
		this->CalWinAmrySide();
	}

	RandActivity::OnSpecialActivityStatusChange(from_status, to_status); //对CalWinAmrySide()函数依赖
}

void RandActivityArmyDay::OnAddArmyScore(int side, const int score)
{
	if (side < 0 || side >= RA_ARMY_DAY_ARMY_SIDE_NUM)
	{
		return;
	}

	int temp = m_all_army_score_list[side] + score;
	if (temp <= 0)
	{
		m_all_army_score_list[side] = 0;
		return;
	}

	m_all_army_score_list[side] += score;
}

void RandActivityArmyDay::GetAllArmyScoreInfoList(int army_info_list[])
{
	if (NULL == army_info_list)
	{
		return;
	}

	for (int i = 0; i < RA_ARMY_DAY_ARMY_SIDE_NUM; ++ i)
	{
		army_info_list[i] = m_all_army_score_list[i];
	}
}

void RandActivityArmyDay::CalWinAmrySide()
{
	int max_score_count = 0;
	//如果三方分数相同取第一个
	for (int i  = 0; i < RA_ARMY_DAY_ARMY_SIDE_NUM; ++ i)
	{
		if (m_all_army_score_list[i] > max_score_count)
		{
			max_score_count = m_all_army_score_list[i];
			m_win_army_side = i;
		}
	}
}