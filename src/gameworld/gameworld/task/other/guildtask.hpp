#ifndef __GUILD_TASK_HPP__
#define __GUILD_TASK_HPP__

#include "servercommon/taskdef.hpp"
#include "task/taskpool.h"

class Role;

class GuildTask
{
public:
	GuildTask();
	~GuildTask();

	void SetRole(Role *role) { m_role = role; }
	void Init(Role *role, const GuildTaskParam &p);

	void GetInitParam(GuildTaskParam *p);
	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);

	bool CanAcceptTask(TaskID task_id, bool notify = false);
	void OnAddTask(TaskID task_id, bool is_init);

	void SendGuildTaskInfo();

	void OnRemoveTask(TaskID task_id, bool is_complete);
	bool DoReward(const TaskCfg *task_cfg);
	void GetGuildDoubleReward();

	void OneKeyCompleteTask();													// Ò»¼üÍê³É

	bool AllTaskComplete() { return 0 != m_guild_param.is_finish; }
	int GetCompleteTaskCount() { return m_guild_param.complete_task_count; }

private:
	Role *m_role;
	
	GuildTaskParam m_guild_param;
};

#endif // __GUILD_TASK_HPP__

