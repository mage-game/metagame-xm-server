#ifndef __ROLE_PRECIOUS_BOSS_HPP__
#define __ROLE_PRECIOUS_BOSS_HPP__

#include "servercommon/fbdef.hpp"

class Role;

class RolePreciousBoss
{
public:
	RolePreciousBoss();
	~RolePreciousBoss();

	void Init(Role *role, const PreciousBossParam &param);
	void GetInitParam(PreciousBossParam *param);
	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);

	bool AddPreciousBossTaskCondition(int task_type, int max_count, int add_count = 1);
	bool SkipTask();

	PreciousBossParam::TaskParam * GetPreciousBossParamTaskList(int &count) { count = PreciousBossParam::PRECIOUS_BOSS_TASK_TYPE_MAX; return m_param.task_list; }


private:

	Role *m_role;
	PreciousBossParam m_param;
};

#endif // __ROLE_PRECIOUS_BOSS_HPP__
