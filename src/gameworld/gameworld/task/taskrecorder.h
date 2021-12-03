
#ifndef TASKRECORDER_H
#define TASKRECORDER_H

#include <set>
#include "gamedef.h"

/*
	��¼��ɫ�����������¼���ֱ��¼���������Ĵ����Լ����������Ĵ���
*/

class Role;

class TaskRecorder
{
public:
	struct TaskItem
	{
		TaskID			task_id;		// ����ID
	};

	TaskRecorder();
	~TaskRecorder();

	void SetRole(Role *role) { m_role = role; }

	bool Init(Role *role, int buff_len, const char *record_buff);
	void GetInitParam(char *record_buff, int *record_buff_len);
	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);

	bool IsEmpty();

	bool IsFinished(TaskID task_id);
	void Complete(TaskID task_id);

	void SendTaskRecorderListToRole();

	void GMClearTrunkTask(TaskID start_task_id);

protected:
	void ResizeTaskIdList(int size);

	void SendTaskRecorderInfo(TaskID task_id);

	void ClearGuildTaskRecord();

	Role		*m_role;

	std::set<TaskID> m_record_set;
};

#endif
