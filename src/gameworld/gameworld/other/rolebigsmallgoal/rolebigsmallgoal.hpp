#ifndef __ROLE_BIG_SMALL_GOAL_HPP__
#define __ROLE_BIG_SMALL_GOAL_HPP__

#include "servercommon/struct/rolebigsmallgoalparam.hpp"

class Role;

class RoleBigSmallGoal
{
public:
	RoleBigSmallGoal() : m_role(NULL) {}
	~RoleBigSmallGoal() {}

	void SetRole(Role *role) { m_role = role; }
	void InitParam(Role *role, const RoleBigSmallGoalRewardParam &param);
	void GetInitParam(RoleBigSmallGoalRewardParam *param) const;

	void OnOpenFunc(int system_type);
	void OnRoleLevelChange(int level);
	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);
	bool CanOpen(int system_type, int goal_type);				//能否开启大小目标系统

	void OnSystemOperaCheck(int system_type, int oper_type, int param_1, int param_2 = 0);
	void OnRoleBigSmallGoalOpera(int opera_type, int param_1, int param_2);
	bool ActiveRoleBigSmallGoalSpecialAttr(int system_type, int param_1);
	void RoleBigSmallGoalAddSpecialAttr(int system_type, CharIntAttrs &attrs_add);
	int GetRoleBigSmallGoalAddAttr(int system_type);
	void OnGetItem(int item_id, int num);
	bool IsFuncActive(int system_type) { return m_param.goal_info_list[system_type].open_func_time_stamp != 0; }
	void OnRoleLogin();
private:

	void CheckGoalProgress(int system_type, int goal_type, int oper_type, int param_1, int param_2 = 0);
	void CheckGoalActive(int system_type, int goal_type);
	void SendGoalInfo(int system_type);
	void FetchGoalRewardReq(int system_type, bool use_gold);
	void FetchGoalReward(int system_type, int goal_type, bool use_gold);
	void CheckBroadcast(int system_type, int goal_type);

	Role *m_role;
	RoleBigSmallGoalRewardParam m_param;
};

#endif // !__ROLE_BIG_SMALL_GOAL_HPP__
