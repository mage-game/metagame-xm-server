#include "rollmoney.h"
#include "obj/character/role.h"
#include "math.h"
#include "servercommon/errornum.h"
#include "engineadapter.h"

#include "config/logicconfigmanager.hpp"
#include "other/rollmoney/rollmoneyconfig.h"
#include "item/knapsack.h"
#include "other/activedegree/activedegree.hpp"
#include "other/welfare/welfare.hpp"

#include "protocal/msgrole.h"
#include "protocal/msgother.h"

RollMoney::RollMoney() : m_role(NULL), m_gold_roll_times(0), m_coin_roll_times(0), m_gold_fetch_reward_times(0), m_coin_fetch_reward_timies(0), m_day_roll_one_gold_times(0)
{
	memset(m_gold_roll_num_list, 0, sizeof(m_gold_roll_num_list));
	memset(m_coin_roll_num_list, 0, sizeof(m_coin_roll_num_list));
}

RollMoney::~RollMoney()
{

}

void RollMoney::Init(Role *role, short gold_fetch_reward_times, short coin_fetch_reward_tims, char gold_roll_times, char coin_roll_times,
					 char gold_num_0, char gold_num_1, char gold_num_2, char coin_num_0, char coin_num_1, char coin_num_2, char coin_num_3,
					 char coin_num_4, char coin_num_5, char coin_num_6, char day_roll_gold_times)
{
	m_role = role;
	m_gold_fetch_reward_times = gold_fetch_reward_times;
	m_coin_fetch_reward_timies = coin_fetch_reward_tims;
	m_gold_roll_times = gold_roll_times;
	m_coin_roll_times = coin_roll_times;
	m_day_roll_one_gold_times = day_roll_gold_times;
	
	UNSTD_STATIC_CHECK(3 == ROLL_MONEY_GOLD_ROLL_NUM_MAX_COUNT);
	UNSTD_STATIC_CHECK(7 == ROLL_MONEY_COIN_ROLL_NUM_MAX_COUNT);

	m_gold_roll_num_list[0] = gold_num_0;
	m_gold_roll_num_list[1] = gold_num_1;
	m_gold_roll_num_list[2] = gold_num_2;

	m_coin_roll_num_list[0] = coin_num_0;
	m_coin_roll_num_list[1] = coin_num_1;
	m_coin_roll_num_list[2] = coin_num_2;
	m_coin_roll_num_list[3] = coin_num_3;
	m_coin_roll_num_list[4] = coin_num_4;
	m_coin_roll_num_list[5] = coin_num_5;
	m_coin_roll_num_list[6] = coin_num_6;
}

void RollMoney::GetInitParam(short *gold_fetch_reward_times, short *coin_fetch_reward_tims, char *gold_roll_times, char *coin_roll_times,
							 char *gold_num_0, char *gold_num_1, char *gold_num_2, char *coin_num_0, char *coin_num_1, char *coin_num_2, char *coin_num_3,
							 char *coin_num_4, char *coin_num_5, char *coin_num_6, char *day_roll_gold_times)
{
	*gold_fetch_reward_times = static_cast<short>(m_gold_fetch_reward_times);
	*coin_fetch_reward_tims = static_cast<short>(m_coin_fetch_reward_timies);
	*gold_roll_times = static_cast<char>(m_gold_roll_times);
	*coin_roll_times = static_cast<char>(m_coin_roll_times);

	*day_roll_gold_times = m_day_roll_one_gold_times;

	UNSTD_STATIC_CHECK(3 == ROLL_MONEY_GOLD_ROLL_NUM_MAX_COUNT);
	UNSTD_STATIC_CHECK(7 == ROLL_MONEY_COIN_ROLL_NUM_MAX_COUNT);

	*gold_num_0 = m_gold_roll_num_list[0];
	*gold_num_1 = m_gold_roll_num_list[1];
	*gold_num_2 = m_gold_roll_num_list[2];

	*coin_num_0 = m_coin_roll_num_list[0];
	*coin_num_1 = m_coin_roll_num_list[1];
	*coin_num_2 = m_coin_roll_num_list[2];
	*coin_num_3 = m_coin_roll_num_list[3];
	*coin_num_4 = m_coin_roll_num_list[4];
	*coin_num_5 = m_coin_roll_num_list[5];
	*coin_num_6 = m_coin_roll_num_list[6];
}

void RollMoney::OnDayChange(int old_dayid, int now_dayid)
{
	if (old_dayid != now_dayid)
	{
		m_day_roll_one_gold_times = 0;
		this->SendInfo();
	}
}

void RollMoney::OnOperaReq(int opera_type)
{
	bool need_send = false;

	switch(opera_type)
	{
	case Protocol::ROLL_MONEY_OPERA_TYPE_QUERY:
		{
			need_send = true;
		}
		break;

	case Protocol::ROLL_MONEY_OPERA_TYPE_ROLL_GOLD:
		{
			need_send = this->OnRollGoldNum();
		}
		break;

// 	case Protocol::ROLL_MONEY_OPERA_TYPE_ROLL_COIN:
// 		{
// 			need_send = this->OnRollCoinNum();
// 		}
// 		break;

	case Protocol::ROLL_MONEY_OPERA_TYPE_FETCH_GOLD_REWARD:
		{
			need_send = this->OnFetchGoldReward();
		}
		break;

// 	case Protocol::ROLL_MONEY_OPERA_TYPE_FETCH_COIN_REWARD:
// 		{
// 			need_send = this->OnFetchCoinReward();
// 		}
// 		break;
	}

	if (need_send)
	{
		this->SendInfo();
	}
}

void RollMoney::SendInfo()
{
	Protocol::SCRollMoneyInfo rmi;
	rmi.fetch_gold_reward_times = m_gold_fetch_reward_times;
	rmi.fetch_coin_reward_times = m_coin_fetch_reward_timies;
	rmi.gold_roll_times = m_gold_roll_times;
	rmi.coin_roll_times = m_coin_roll_times;
	rmi.day_roll_one_gold_times = m_day_roll_one_gold_times;
	rmi.reserve_ch = 0;
	rmi.reserve_sh = 0;
	memcpy(rmi.gold_roll_num_list, m_gold_roll_num_list, sizeof(rmi.gold_roll_num_list));
	memcpy(rmi.coin_roll_num_list, m_coin_roll_num_list, sizeof(rmi.coin_roll_num_list));
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&rmi, sizeof(rmi));
}

bool RollMoney::OnRollGoldNum()
{
	if (m_gold_roll_times < 0 || m_gold_roll_times > ROLL_MONEY_GOLD_ROLL_NUM_MAX_COUNT) // ·À´íÐÞÕý
	{
		m_gold_roll_times = 0;
	}

	if (m_gold_roll_times >= ROLL_MONEY_GOLD_ROLL_NUM_MAX_COUNT)
	{
		m_role->NoticeNum(errornum::EN_ROLL_MONEY_ROLL_TIMES_LIMIT);
		return false;
	}

	if (m_gold_fetch_reward_times >= ROLL_MONEY_FETCH_GOLD_REWARD_MAX_TIEMS)
	{
		m_role->NoticeNum(errornum::EN_ROLL_MONEY_HAS_FETCH_ALL_REWARD);
		return false;
	}

	if (m_day_roll_one_gold_times >= ROLL_MONEY_DAILY_ROLL_GOLD_MAX_TIMES)
	{
		m_role->NoticeNum(errornum::EN_ROLL_MONEY_DAILY_ROLL_TIMES_LIMIT);
		return false;
	}

	const RollMoneyOtherCfg &other_cfg = LOGIC_CONFIG->GetRollMoneyCfg().GetOtherCfg();
	int need_active_degree = other_cfg.active_degree_need * (m_day_roll_one_gold_times + 1);

	if (m_role->GetActiveDegree()->GetTotalDegree() < need_active_degree)
	{
		m_role->NoticeNum(errornum::EN_ROLL_MONEY_ACTIVE_DEDREE_LIMIT);
		return false;
	}

	int result_num = -1;
	{
		const RollMoneyRollBindGoldCfg &roll_bind_gold_cfg = LOGIC_CONFIG->GetRollMoneyCfg().GetRollGoldCfg();
		const RollMoneyRollBindGoldCfg::ConfigItem &cfg_item = roll_bind_gold_cfg.cfg_item_list[m_gold_roll_times];
		int total_weight = 0;

		for (int i = 0; i < ROLL_MONEY_NUM_MAX_COUNT; ++ i)
		{
			total_weight += cfg_item.num_weight[i];
		}

		if (total_weight <= 0)
		{
			return false;
		}

		int rand_value = RandomNum(total_weight);

		for (int i = 0; i < ROLL_MONEY_NUM_MAX_COUNT; ++ i)
		{
			if (rand_value < cfg_item.num_weight[i])
			{
				result_num = i;
				break;
			}

			rand_value -= cfg_item.num_weight[i];
		}
	}
	if (result_num < 0 || result_num >= ROLL_MONEY_NUM_MAX_COUNT)
	{
		return false;
	}

	m_gold_roll_num_list[m_gold_roll_times] = result_num;
	++ m_gold_roll_times;
	++ m_day_roll_one_gold_times;

	return true;
}

bool RollMoney::OnRollCoinNum()
{
	if (m_coin_roll_times < 0 || m_coin_roll_times > ROLL_MONEY_COIN_ROLL_NUM_MAX_COUNT) // ·À´íÐÞÕý
	{
		m_coin_roll_times = 0;
	}

	if (m_coin_roll_times >= ROLL_MONEY_COIN_ROLL_NUM_MAX_COUNT)
	{
		m_role->NoticeNum(errornum::EN_ROLL_MONEY_ROLL_TIMES_LIMIT);
		return false;
	}

	if (m_coin_roll_times >= m_role->GetWelfare()->GetTotalLoginDays())
	{
		m_role->NoticeNum(errornum::EN_ROLL_MONEY_LOGIN_DAYS_NOT_ENOUGH);
		return false;
	}

	if (m_coin_fetch_reward_timies >= ROLL_MONEY_FETCH_COIN_REWARD_MAX_TIMES)
	{
		m_role->NoticeNum(errornum::EN_ROLL_MONEY_HAS_FETCH_ALL_REWARD);
		return false;
	}
	
	int result_num = -1;
	{
		const RollMoneyRollCoinCfg &roll_coin_cfg = LOGIC_CONFIG->GetRollMoneyCfg().GetRollCoinCfg();
		const RollMoneyRollCoinCfg::ConfigItem &cfg_item = roll_coin_cfg.cfg_item_list[m_coin_roll_times];

		int total_weight = 0;

		for (int i = 0; i < ROLL_MONEY_NUM_MAX_COUNT; ++ i)
		{
			total_weight += cfg_item.num_weight[i];
		}

		if (total_weight <= 0)
		{
			return false;
		}

		int rand_value = RandomNum(total_weight);

		for (int i = 0; i < ROLL_MONEY_NUM_MAX_COUNT; ++ i)
		{
			if (rand_value < cfg_item.num_weight[i])
			{
				result_num = i;
				break;
			}

			rand_value -= cfg_item.num_weight[i];
		}
	}
	if (result_num < 0)
	{
		return false;
	}

	m_coin_roll_num_list[m_coin_roll_times] = result_num;
	++ m_coin_roll_times;

	return true;
}

bool RollMoney::OnFetchGoldReward()
{
	if (m_gold_fetch_reward_times >= ROLL_MONEY_FETCH_GOLD_REWARD_MAX_TIEMS)
	{
		return false;
	}

	if (m_gold_roll_times < ROLL_MONEY_GOLD_ROLL_NUM_MAX_COUNT)
	{
		return false;
	}
	
	UNSTD_STATIC_CHECK(3 == ROLL_MONEY_GOLD_ROLL_NUM_MAX_COUNT);

	int reward_gold = (m_gold_roll_num_list[0] * 10 + m_gold_roll_num_list[1]) * m_gold_roll_num_list[2];

	gamelog::g_log_touzijihua.printf(LL_INFO, "OnFetchGoldReward user[%d, %s] reward_gold[%d], fetch_reward_times[%d], num_0[%d], num_1[%d], num_2[%d]",
		m_role->GetUID(), m_role->GetName(), reward_gold, m_gold_fetch_reward_times + 1, m_gold_roll_num_list[0], m_gold_roll_num_list[1], m_gold_roll_num_list[2]);

	++ m_gold_fetch_reward_times;
	m_gold_roll_times = 0;

	memset(m_gold_roll_num_list, 0, sizeof(m_gold_roll_num_list));

	m_role->GetKnapsack()->GetMoney()->AddGoldBind(reward_gold, "RollMoneyGold");

	return true;
}

bool RollMoney::OnFetchCoinReward()
{
	if (m_coin_fetch_reward_timies >= ROLL_MONEY_FETCH_COIN_REWARD_MAX_TIMES)
	{
		return false;
	}

	if (m_coin_roll_times < ROLL_MONEY_COIN_ROLL_NUM_MAX_COUNT)
	{
		return false;
	}

	UNSTD_STATIC_CHECK(7 == ROLL_MONEY_COIN_ROLL_NUM_MAX_COUNT);

	int reward_coin = 0;

	for (int i = 0; i < ROLL_MONEY_COIN_ROLL_NUM_MAX_COUNT - 1; ++ i)
	{
		reward_coin += static_cast<int>(m_coin_roll_num_list[i] * pow(10.0, ROLL_MONEY_COIN_ROLL_NUM_MAX_COUNT - 2 - i));
	}

	reward_coin *= m_coin_roll_num_list[ROLL_MONEY_COIN_ROLL_NUM_MAX_COUNT - 1];

	gamelog::g_log_touzijihua.printf(LL_INFO, "OnFetchCoinReward user[%d, %s] reward_coin[%d], fetch_reward_times[%d], num_0[%d], num_1[%d], num_2[%d]"
		"num_3[%d], num_4[%d], num_5[%d], num_6[%d]",
		m_role->GetUID(), m_role->GetName(), reward_coin, m_coin_fetch_reward_timies + 1, m_coin_roll_num_list[0], m_coin_roll_num_list[1], m_coin_roll_num_list[2],
		m_coin_roll_num_list[3], m_coin_roll_num_list[4], m_coin_roll_num_list[5], m_coin_roll_num_list[6]);

	++ m_coin_fetch_reward_timies;
	m_coin_roll_times = 0;
	memset(m_coin_roll_num_list, 0, sizeof(m_coin_roll_num_list));

	m_role->GetKnapsack()->GetMoney()->AddCoinBind(reward_coin, "RollMoneyCoin");

	return true;
}

