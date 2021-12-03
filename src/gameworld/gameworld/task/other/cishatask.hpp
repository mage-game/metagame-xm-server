#ifndef __CISHA_TASK_HPP__
#define __CISHA_TASK_HPP__

#include "gamedef.h"
#include "servercommon/taskdef.hpp"

class Role;
struct TaskCfg;

class CishaTask
{
public:
	CishaTask();
	~CishaTask();

	void SetRole(Role *role) { m_role = role; }

	void OnDayChange(int old_dayid, int now_dayid);
	void Update(unsigned int now_second);

	void AcceptTask();
	void OnAddTask(TaskID task_id, bool is_init);
	void OnRemoveTask(TaskID task_id, bool is_complete);
	bool CanAcceptTask(TaskID task_id, bool notify = false);
	bool DoReward(const TaskCfg *task_cfg);

	void SendInfo();

private:
	Role *m_role;

	bool m_first;
	time_t m_task_timeout_stamp;
};

#endif