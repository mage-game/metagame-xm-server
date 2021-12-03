#include "taskrecorder.h"
#include "engineadapter.h"
#include "protocal/msgnpc.h"
#include "obj/character/role.h"
#include "task/taskpool.h"
#include "task/newtaskmanager.h"

TaskRecorder::TaskRecorder()
	: m_role(NULL)
{

}

TaskRecorder::~TaskRecorder()
{
}

bool TaskRecorder::Init(Role *role, int buff_len, const char *record_buff)
{
	m_role = role;

	if (NULL != record_buff && buff_len >= (int)sizeof(short))
	{
		short total_count = *(short *)record_buff;
		record_buff += sizeof(short);
		buff_len -= sizeof(short);

		for (int i = 0; i < total_count; ++i)
		{
			if (buff_len <= 0)
			{
				break;
			}

			short task_id = *(short *)record_buff;
			record_buff += sizeof(short);
			buff_len -= sizeof(short);

			m_record_set.insert(task_id);
		}
	}

	return true;
}

void TaskRecorder::GetInitParam(char *record_buff, int *record_buff_len)
{
	if (NULL == record_buff || NULL == record_buff_len) return;

	*record_buff_len = 0; 

	short total_count = (short)(m_record_set.size());
	if (total_count > MAX_TASK_RECORD_NUM)
	{
		total_count = MAX_TASK_RECORD_NUM;
	}

	*(short *)record_buff = total_count;
	record_buff += sizeof(short);
	*record_buff_len += sizeof(short);

	int tmp_count = 0;
	for (std::set<TaskID>::iterator iter = m_record_set.begin(); 
		iter != m_record_set.end() && tmp_count < total_count;
		++iter)
	{
		*(short *)record_buff = *iter;
		record_buff += sizeof(short);
		*record_buff_len += sizeof(short);
	}
}

void TaskRecorder::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	if (old_dayid != now_dayid)
	{
		this->ClearGuildTaskRecord();
		m_role->GetTaskManager()->OnGuildTaskStateChange();
	}
}

void TaskRecorder::Complete(TaskID task_id)
{
	m_record_set.insert(task_id);
	this->SendTaskRecorderInfo(task_id);
}

bool TaskRecorder::IsFinished(TaskID task_id)
{
	std::set<TaskID>::iterator iter = m_record_set.find(task_id);
	if (iter != m_record_set.end())
	{
		return true;
	}	

	return false;
}


void TaskRecorder::SendTaskRecorderListToRole()
{
	static Protocol::SCTaskRecorderList trl;

	int count = 0;
	for (std::set<TaskID>::iterator it = m_record_set.begin(); m_record_set.end() != it; ++ it)
	{
		if (count < MAX_TASK_RECORDER_INFO_NUM)
		{
			trl.task_recorder_info_item[count] = *it;
			++ count;
		}
		else
		{
			break;
		}
	}
	trl.count = count;

	unsigned int sendlen = sizeof(trl) - (MAX_TASK_RECORDER_INFO_NUM - count) * sizeof(trl.task_recorder_info_item[0]);
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&trl, sendlen);
}

void TaskRecorder::SendTaskRecorderInfo(TaskID task_id)
{
	Protocol::SCTaskRecorderInfo tri;
	tri.completed_taskid = task_id;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&tri, sizeof(Protocol::SCTaskRecorderInfo));
}

void TaskRecorder::GMClearTrunkTask(TaskID start_task_id)
{
	for (std::set<TaskID>::iterator it = m_record_set.begin(); it != m_record_set.end();)
	{
		const TaskCfg *task = TASKPOOL->GetCfgByTaskID(*it);
		if (NULL != task && TASK_TYPE_TRUNK == task->task_type && *it >= start_task_id)
		{
			m_record_set.erase(it ++);
		}
		else
		{
			++ it;
		}
	}

	this->SendTaskRecorderListToRole();
}

bool TaskRecorder::IsEmpty()
{
	if (m_record_set.empty())
	{
		return true;
	}

	return false;
}


void TaskRecorder::ClearGuildTaskRecord()
{
	for (std::set<TaskID>::iterator it = m_record_set.begin(); it != m_record_set.end();)
	{
		const TaskCfg *task_cfg = TASKPOOL->GetCfgByTaskID(*it);
		if (NULL != task_cfg && TASK_TYPE_GUILD == task_cfg->task_type)
		{
			m_record_set.erase(it ++);
		}
		else
		{
			++ it;
		}
	}

	this->SendTaskRecorderListToRole();
}

