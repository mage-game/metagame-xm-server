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
	return; // ���Σ�����û����

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
	return; // ���Σ�����û����

	// ��飺���������ã��Ƿ����Ŀ�ꡢ�Ƿ�����ȡ���������Ƿ��㹻
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

	// ���ģ�������ȡ���
	data->is_reward_fetched = 1;

	// ���裬������Ʒ������
	m_role->GetKnapsack()->Put(cfg->can_fetch_reward, PUT_ROLE_BIG_SMALL_GOAL_REWARD);

	// ����
	this->SendGoalInfo();
}

void GreateSoldierGoal::OnBuyReward(int goal_type)
{
	return; // ���Σ�����û����

	// ��飺���������ã�δ��ȡ����ֻ�ܹ���һ�Σ������ռ䣬Ԫ����
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

	// ���ģ���Ǯ��������ȡ��Ǻ�������ɱ��
	m_role->GetKnapsack()->GetMoney()->UseGold(cfg->cost, "GreateSoldierGoal::OnBuyReward");
	data->is_purchased = 1;
	data->is_finish = 1;

	// ���裬������Ʒ��������������һ������
	m_role->GetKnapsack()->Put(cfg->can_fetch_reward, PUT_ROLE_BIG_SMALL_GOAL_REWARD);

	this->BeginNextGoal(goal_type);

	// ����
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

			// ������Ҳ���һ����һ��Ŀ�꣬�Ƿ���ǰ�����
			if (GREATE_SOLDIER(m_role)->IsActive(next_cfg->need_active_seq))
			{
				next_data->is_finish = 1;
			}
		}
	}
}
