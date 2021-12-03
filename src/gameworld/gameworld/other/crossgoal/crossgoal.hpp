#ifndef __CROSS_GOLD_HPP__
#define __CROSS_GOLD_HPP__

#include "servercommon/crossgoaldef.hpp"
#include "other/crossgoal/crossgoalconfig.hpp"

class Role;

class CrossGoal
{
public:
	CrossGoal();
	~CrossGoal();

	void SetRole(Role *role) { m_role = role; }
	void Init(Role *role, const CrossGoalParam &param);
	void GetInitParam(CrossGoalParam *param);
	void OnUserLogin();

	void Update(unsigned int now_second);
	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);

	void ChangeCrossGoalParam(int cond_type,int param);

	void OperaReq(Protocol::CSCrossGoalOperaReq *req);

	void FetchRewardByIndex(int index);

	void FetchGuildRewardByindex(int index);

	void SendInfoToRole(char guild_notify = 0);

	bool CheckGuildGoalStatus(bool notify_client = false);

private:

	void CheckCrossGoalStatus();
	bool CheckCondFinish(const CrossGoalItem * goal_item);//个人目标

	Role *m_role;
	CrossGoalParam m_param;
};

#endif

