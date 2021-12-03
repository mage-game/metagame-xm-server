#include "greatesoldier_goal.hpp"
#include "protocal/msggreatesoldier.h"
#include "greatesoldier.hpp"
#include "greatesoldierconfig.hpp"
#include "config/logicconfigmanager.hpp"
#include "item/knapsack.h"

void GreateSoldierGoal::Init(Role *role, GreateSoldierGoalData *data)
{
	m_role = role;
	m_data = data;
}

void GreateSoldierGoal::OnOpenView()
{
	GreateSoldierGoalData::DataItem &first_goal_data = m_data->data_list[GreateSoldierGoalData::GOAL_TYPE_1];

	if (first_goal_data.end_time > 0 || first_goal_data.is_finish) return;

	const GreateSoldierGoalConfig * cfg = LOGIC_CONFIG->GetGreateSoldierConfig().GetGreateSoldierGoalConfig(GreateSoldierGoalData::GOAL_TYPE_1);
	if (!cfg) return;

	first_goal_data.end_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + SECOND_PER_DAY * cfg->need_days;

	this->SendGoalInfo();
}

void GreateSoldierGoal::OnActive(int seq)
{
	return; // 屏蔽，现在没用了

	unsigned int now_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	for (int goal_type = 0; goal_type < GreateSoldierGoalData::GOAL_TYPE_MAX; ++goal_type)
	{
		const GreateSoldierGoalConfig *cfg = LOGIC_CONFIG->GetGreateSoldierConfig().GetGreateSoldierGoalConfig(goal_type);
		if (!cfg) continue;

		GreateSoldierGoalData::DataItem *data = &m_data->data_list[goal_type];
		if (data->is_finish) continue;

		if (now_time > data->end_time) continue;

		if (seq == cfg->need_active_seq)
		{
			data->is_finish = 1;
			
			this->BeginNextGoal(goal_type);

			this->SendGoalInfo();
		}
	}
}

void GreateSoldierGoal::OnFetchReward(int goal_type)
{
	return; // 屏蔽，现在没用了

	// 检查：参数、配置，是否完成目标、是否已领取过、背包是否足够
	if (!GreateSoldierGoalData::IsValidGoalType(goal_type))
	{
		m_role->NoticeNum(errornum::EN_PARAMETER_ERROR);
		return;
	}

	const GreateSoldierGoalConfig *cfg = LOGIC_CONFIG->GetGreateSoldierConfig().GetGreateSoldierGoalConfig(goal_type);
	if (!cfg)
	{
		m_role->NoticeNum(errornum::EN_CONFIG_ERROR);
		return;
	}

	GreateSoldierGoalData::DataItem *data = &m_data->data_list[goal_type];
	if (!data->is_finish)
	{
		//m_role->NoticeNum(errornum::EN_REWARD_FETCH_CONDITION_LIMIT);
		return;
	}

	if (data->is_reward_fetched)
	{
		//m_role->NoticeNum(errornum::EN_REWARD_ALREADY_FETCHED);
		return;
	}

	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	// 消耗，设置领取标记
	data->is_reward_fetched = 1;

	// 给予，发放物品到背包
	m_role->GetKnapsack()->Put(cfg->can_fetch_reward, PUT_ROLE_BIG_SMALL_GOAL_REWARD);

	// 发送
	this->SendGoalInfo();
}

void GreateSoldierGoal::OnBuyReward(int goal_type)
{
	return; // 屏蔽，现在没用了

	// 检查：参数，配置，未领取过，只能购买一次，背包空间，元宝数
	if (!GreateSoldierGoalData::IsValidGoalType(goal_type))
	{
		m_role->NoticeNum(errornum::EN_PARAMETER_ERROR);
		return;
	}

	const GreateSoldierGoalConfig *cfg = LOGIC_CONFIG->GetGreateSoldierConfig().GetGreateSoldierGoalConfig(goal_type);
	if (!cfg)
	{
		m_role->NoticeNum(errornum::EN_CONFIG_ERROR);
		return;
	}

	GreateSoldierGoalData::DataItem *data = &m_data->data_list[goal_type];
	if (data->is_purchased)
	{
		//m_role->NoticeNum(errornum::EN_REWARD_ALREADY_FETCHED);
		return;
	}

	if (data->is_reward_fetched)
	{
		//m_role->NoticeNum(errornum::EN_REWARD_ALREADY_FETCHED);
		return;
	}

	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	if (!m_role->GetKnapsack()->GetMoney()->GoldMoreThan(cfg->cost))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	// 消耗，花钱，设置领取标记和任务完成标记
	m_role->GetKnapsack()->GetMoney()->UseGold(cfg->cost, "GreateSoldierGoal::OnBuyReward");
	data->is_purchased = 1;
	data->is_finish = 1;

	// 给予，发放物品到背包，开启下一个任务
	m_role->GetKnapsack()->Put(cfg->can_fetch_reward, PUT_ROLE_BIG_SMALL_GOAL_REWARD);

	this->BeginNextGoal(goal_type);

	// 发送
	this->SendGoalInfo();
}

void GreateSoldierGoal::SendGoalInfo()
{
	Protocol::SCGreateSoldierGoalInfo msg;
	msg.goal_data = *m_data;
	SEND_TO_ROLE(m_role, msg);
}

void GreateSoldierGoal::BeginNextGoal(int goal_type)
{
	if (GreateSoldierGoalData::IsValidGoalType(goal_type + 1))
	{
		const GreateSoldierGoalConfig *next_cfg = LOGIC_CONFIG->GetGreateSoldierConfig().GetGreateSoldierGoalConfig(goal_type + 1);
		if (next_cfg)
		{
			GreateSoldierGoalData::DataItem *next_data = &m_data->data_list[goal_type + 1];
			next_data->end_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + SECOND_PER_DAY * next_cfg->need_days;

			// 开启后，也检查一下下一个目标，是否提前完成了
			if (GREATE_SOLDIER(m_role)->IsActive(next_cfg->need_active_seq))
			{
				next_data->is_finish = 1;
			}
		}
	}
}
