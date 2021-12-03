#ifndef __DATING_TASK_HPP__
#define __DATING_TASK_HPP__

#include "servercommon/taskdef.hpp"
#include "protocal/msgnpc.h"

class Role;

class DatingTask
{
public:
	DatingTask();
	~DatingTask();

	void SetRole(Role *role) { m_role = role; }

	bool CanAcceptDatingTask();

	void OnAddTask(TaskID task_id, bool is_complete, bool is_init);
	void OnRemoveTask(TaskID task_id, bool is_complete);

private: 

	Role *m_role;

	DatingTaskParam m_dating_task_info;
};

#endif	// __DATING_TASK_HPP__

