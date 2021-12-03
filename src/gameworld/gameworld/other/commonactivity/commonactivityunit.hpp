#ifndef __COMMON_ACTIVITY_UNIT_HPP__
#define __COMMON_ACTIVITY_UNIT_HPP__

class Role;

#include "servercommon/struct/commonactivityparam.hpp"

class CommonActivityUnitManager
{
public:
	CommonActivityUnitManager();
	~CommonActivityUnitManager();

	void SetRole(Role *role) { m_role = role; }
	void Init(Role *role, const CommonActivityParam &common_activity_param);
	void GetInitParam(CommonActivityParam *common_activity_param);

	void OnUserLogin();

	void OnFetchReward(int unit_type, int param1, int param2);
	void SendCommonActivityState();

	void CheckJoinInSameActivity(bool is_notify);

private:
	void OnSwithNewActivity();

	bool FetchRewardForTest(int param1, int patam2);

	Role *m_role;
	CommonActivityParam m_param;
};

#endif

