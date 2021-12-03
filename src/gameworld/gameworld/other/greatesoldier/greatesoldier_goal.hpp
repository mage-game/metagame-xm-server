//-------------------------------------------------------------------------------------------

// 名将目标模块

//-------------------------------------------------------------------------------------------

#ifndef __GREATE_SOLDIER_GOAL_HPP__
#define __GREATE_SOLDIER_GOAL_HPP__

class Role;
class GreateSoldierGoalData;

// 名将目标。  W3有自己的大小目标系统，所以这个没什么用

class GreateSoldierGoal
{
public:
	GreateSoldierGoal() = default;

	void Init(Role *role, GreateSoldierGoalData *data);

	void OnOpenView();							// 当打开 View
	void OnActive(int seq);						// 当激活名将
	void OnFetchReward(int goal_type);			// 领取目标奖励
	void OnBuyReward(int goal_type);			// 购买目标奖励物品

	void SendGoalInfo();

private:
	void BeginNextGoal(int goal_type);			// 开启下一个任务

	Role *m_role = nullptr;
	GreateSoldierGoalData* m_data = nullptr;
};

#endif // __GREATE_SOLDIER_GOAL_HPP__