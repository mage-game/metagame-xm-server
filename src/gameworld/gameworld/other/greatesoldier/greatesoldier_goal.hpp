//-------------------------------------------------------------------------------------------

// ����Ŀ��ģ��

//-------------------------------------------------------------------------------------------

#ifndef __GREATE_SOLDIER_GOAL_HPP__
#define __GREATE_SOLDIER_GOAL_HPP__

class Role;
class GreateSoldierGoalData;

// ����Ŀ�ꡣ  W3���Լ��Ĵ�СĿ��ϵͳ���������ûʲô��

class GreateSoldierGoal
{
public:
	GreateSoldierGoal() = default;

	void Init(Role *role, GreateSoldierGoalData *data);

	void OnOpenView();							// ���� View
	void OnActive(int seq);						// ����������
	void OnFetchReward(int goal_type);			// ��ȡĿ�꽱��
	void OnBuyReward(int goal_type);			// ����Ŀ�꽱����Ʒ

	void SendGoalInfo();

private:
	void BeginNextGoal(int goal_type);			// ������һ������

	Role *m_role = nullptr;
	GreateSoldierGoalData* m_data = nullptr;
};

#endif // __GREATE_SOLDIER_GOAL_HPP__