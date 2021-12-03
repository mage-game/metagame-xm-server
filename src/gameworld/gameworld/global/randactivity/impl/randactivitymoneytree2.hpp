#ifndef __RAND_ACTIVITY_MONEYTREE2_HPP__
#define __RAND_ACTIVITY_MONEYTREE2_HPP__

#include "global/randactivity/randactivity.hpp"

struct Money_tree2_param
{
	Money_tree2_param() : server_total_rock_money_tree_times(0), prize_pool_gold(0) {}

	int server_total_rock_money_tree_times;
	int prize_pool_gold;
};

class RandActivityMoneyTree2 : public RandActivity
{
public:
	RandActivityMoneyTree2(RandActivityManager *rand_activity_manager);
	virtual ~RandActivityMoneyTree2();

	virtual void Init(const RandActivityData &data);
	virtual void GetInitParam(RandActivityData *data);

	int GetServerTotalRockMoneyTreeTimes() { return m_param.server_total_rock_money_tree_times; }
	int GetServerTotalPoolGold() { return m_param.prize_pool_gold; }

	void AddTotalTotalMoneyTreeTimes(int times);
	void AddTotalPoolGold(int num);
	void DecTotalPoolGold(int num);

	virtual void OnSpecialActivityStatusChange(int from_status, int to_status);

private:
	Money_tree2_param  m_param;
};

#endif

