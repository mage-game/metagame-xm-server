#ifndef __ZHUANZHI_TASK_HPP__
#define __ZHUANZHI_TASK_HPP__

#include "servercommon/taskdef.hpp"
#include "engineadapter.h"
#include "protocal/msgnpc.h"
#include "task/taskpool.h"

class Role;

class ZhuanzhiTask
{
public:
	ZhuanzhiTask();
	~ZhuanzhiTask();

	void SetRole(Role *role) { m_role = role; }
	void Init(Role *role, const ZhuanzhiTaskParam &p);

	void GetInitParam(ZhuanzhiTaskParam *p);

	bool CanAcceptTask(TaskID task_id, bool notify = false);
	void OnAddTask(TaskID task_id, bool is_init);

	int GetZhuanzhiTaskCompleteCount() { return m_zhuanzhitask_param.complete_task_count; }

	void OnRemoveTask(TaskID task_id, bool is_complete);

private:
	Role *m_role;

	ZhuanzhiTaskParam m_zhuanzhitask_param;
};

#endif // __ZHUANZHI_TASK_HPP__
