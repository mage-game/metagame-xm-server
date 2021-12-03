#include "zhuanzhitask.hpp"

#include "task/taskpool.h"
#include "task/taskrecorder.h"

#include "engineadapter.h"
#include "servercommon/errornum.h"
#include "config/logicconfigmanager.hpp"
#include "other/event/eventhandler.hpp"
#include "servercommon/noticenum.h"

#include "obj/character/role.h"

#include "other/route/mailroute.hpp"
#include "item/itempool.h"

#include "servercommon/string/gameworldstr.h"
#include "gamelog.h"
#include "config/joinlimitconfig.hpp"
#include "../newtaskmanager.h"
#include "config/zhuanzhiconfig.hpp"

ZhuanzhiTask::ZhuanzhiTask() : m_role(NULL)
{

}

ZhuanzhiTask::~ZhuanzhiTask()
{

}

void ZhuanzhiTask::Init(Role *role, const ZhuanzhiTaskParam &p)
{
	m_role = role;
	m_zhuanzhitask_param = p;
}

void ZhuanzhiTask::GetInitParam(ZhuanzhiTaskParam *p)
{
	*p = m_zhuanzhitask_param;
}

bool ZhuanzhiTask::CanAcceptTask(TaskID task_id, bool notify)
{
	JoinLimitUnit limit_unit;
	bool is_limit = LOGIC_CONFIG->GetJoinLimitConfig().GetOtherJoinLimit(JLMT_OTHER_ZHUANZHI_TASK, &limit_unit);

	if (is_limit && m_role->GetLevel() < limit_unit.level)
	{
		if (notify) m_role->NoticeNum(errornum::EN_TASK_ACCEPT_LEVEL);
		return false;
	}

	if (m_role->GetTaskRecorder()->IsFinished(task_id))
	{
		return false;
	}

	const TaskCfg *task_cfg = TASKPOOL->GetCfgByTaskID(task_id);
	if (NULL != task_cfg && TASK_TYPE_ZHUANZHI == task_cfg->task_type)
	{
		if (m_zhuanzhitask_param.first_task == 0 || TASKPOOL->IsInZhuanzhiTaskList(m_zhuanzhitask_param.first_task, task_id))
		{
			return true;
		}
	}

	return false;
}

void ZhuanzhiTask::OnAddTask(TaskID task_id, bool is_init)
{
	if (is_init)
	{
		return;
	}

	const TaskCfg *task_cfg = TASKPOOL->GetCfgByTaskID(task_id);
	if (NULL != task_cfg && TASK_TYPE_ZHUANZHI == task_cfg->task_type)
	{
		if (0 == m_zhuanzhitask_param.first_task && TASKPOOL->IsZhuanzhiFirstTaskID(task_id))
		{
			m_zhuanzhitask_param.first_task = task_id;
		}

	}
}

void ZhuanzhiTask::OnRemoveTask(TaskID task_id, bool is_complete)
{
	TaskID temp_task_id = TASKPOOL->GetZhuanzhiEndTaskByFirstTask(m_zhuanzhitask_param.first_task);

	const TaskCfg *task_cfg = TASKPOOL->GetCfgByTaskID(task_id);
	if (NULL != task_cfg && TASK_TYPE_ZHUANZHI == task_cfg->task_type)
	{
		if (is_complete)
		{
			m_role->GetTaskRecorder()->Complete(task_id);
			++ m_zhuanzhitask_param.complete_task_count;

			m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_ZHUANZHI);

			if (temp_task_id == task_id)
			{
				m_zhuanzhitask_param.first_task = 0;
				m_role->Zhuanzhi();
				m_role->GetTaskManager()->OnZhuanzhiTaskStateChange();
			}

		}
	}
}
