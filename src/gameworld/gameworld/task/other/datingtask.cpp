#include "datingtask.hpp"

#include "obj/character/role.h"
#include "task/taskpool.h"
#include "gamelog.h"
#include "other/daycounter/daycounter.hpp"

DatingTask::DatingTask()
	: m_role(NULL)
{

}

DatingTask::~DatingTask()
{

}

bool DatingTask::CanAcceptDatingTask()
{
	//if (m_role->GetTaskManager()->HasTaskType(Task::TT_DATING))
	{
		return false;
	}

	if (!m_role->GetDayCounter()->DayCountCheck(DayCounter::DAYCOUNT_ID_DATING_INVITE, 1))
	{
		return false;
	}

	return true;
}

void DatingTask::OnAddTask(TaskID task_id, bool is_complete, bool is_init)
{
	/*const Task *task = TASKPOOL->GetCfgByTaskID(task_id);
	if (NULL == task || Task::TT_DATING != task->GetTaskType()) return;

	if (!is_init)
	{
		m_dating_task_info.task_id = task_id;
	}*/
}

void DatingTask::OnRemoveTask(TaskID task_id, bool is_complete)
{
	if (task_id == m_dating_task_info.task_id)
	{
		m_dating_task_info.Reset();
	}
}
