#include "randactivitymoneytree.hpp"
#include "config/logicconfigmanager.hpp"
#include "config/activityconfig/randactivityconfig.hpp"

RandActivityMoneyTree::RandActivityMoneyTree(RandActivityManager *rand_activity_manager)
: RandActivity(rand_activity_manager, RAND_ACTIVITY_TYPE_MONEY_TREE)
{

}

RandActivityMoneyTree::~RandActivityMoneyTree()
{

}

void RandActivityMoneyTree::Init(const RandActivityData &data)
{
	RandActivity::Init(data);

	
	UNSTD_STATIC_CHECK(sizeof(m_param) <= sizeof(data.data));

	Money_tree_param* pTemp = (Money_tree_param*)data.data;
	m_param = *pTemp;
}

void RandActivityMoneyTree::GetInitParam(RandActivityData *data)
{
	RandActivity::GetInitParam(data);

	UNSTD_STATIC_CHECK(sizeof(m_param) <= sizeof(data->data));

	Money_tree_param* pTemp = (Money_tree_param*)data->data;
	*pTemp = m_param;
}

void RandActivityMoneyTree::AddTotalTotalMoneyTreeTimes(int times)
{
	if (times > 0)
	{
		m_param.server_total_rock_money_tree_times += times;
	}
}

void RandActivityMoneyTree::AddTotalPoolGold(int num)
{
	if (num > 0)
	{
		m_param.prize_pool_gold += num;
	}
}

void RandActivityMoneyTree::DecTotalPoolGold(int num)
{
	if (num > 0)
	{
		m_param.prize_pool_gold -= num;
	}
}

void RandActivityMoneyTree::OnSpecialActivityStatusChange(int from_status, int to_status)
{
	RandActivity::OnSpecialActivityStatusChange(from_status, to_status);

	if (ACTIVITY_STATUS_OPEN == to_status)
	{
		m_param.server_total_rock_money_tree_times = 0;
		m_param.prize_pool_gold = LOGIC_CONFIG->GetRandActivityCfg().GetMoneyTreeInitGold();
	}
}

