#ifndef __ROLEEVALUATE_HPP__
#define __ROLEEVALUATE_HPP__

#include "servercommon/evaluatedef.hpp"

class Role;

class RoleEvaluate
{
public:
	RoleEvaluate();
	~RoleEvaluate();

	void SetRole(Role *role) { m_role = role; }

	void Init(Role *role, const EvaluateParam &param);
	void GetEvaluateParam(EvaluateParam *param);

	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);

	void OnEvaluateRole(int target_user_id);

private:
	Role *m_role;

	int m_uid_list[MAX_EVALUSTE_NUM];
};

#endif

