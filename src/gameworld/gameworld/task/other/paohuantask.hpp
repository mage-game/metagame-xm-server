#ifndef __PAOHUAN_TASK_HPP__
#define __PAOHUAN_TASK_HPP__

#include "task/taskpool.h"

class Role;

class PaoHuanTask
{
public:
	PaoHuanTask();
	~PaoHuanTask();

	void SetRole(Role *role) { m_role = role; }

	void Init(Role *role);

	void Update(time_t now_second);
	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);
	void OnRoleLeaveScene();

	void SendPaohuanTaskInfo(short notify_reason = 0);
	void OnSkipTask(bool is_skip_batch, TaskID task_id);

	void OnAddTask(TaskID task_id, bool is_init);
	void OnRemoveTask(TaskID task_id, bool is_complete);
	bool CanAcceptTask(bool notify = false);
	bool DoReward(const TaskCfg *task_cfg);

	void StartPaoHuanRoll();
	void OnGetPhaseRewardRollReq(short client_click_index);
	void CheckPaoHuanRollState();
	void SendPaoHuanRollInfo(short reason, short hit_seq = -1, short clint_click_index = -1);
	int GetRandRollItemIndex();

	int getDayCompleteCount();
	void OnOneKeySkipTask();							// Ò»¼üÍê³É			

private:
	void ResetPaohuanTask(bool is_silent = false);
	bool HasLeftCount();
	void OnSkipOneTask(TaskID task_id);
	void AddCompleteCount();

	Role *m_role;

	time_t m_check_task_timestamp;

	bool m_is_rolling;
	PaoHuanRollItem m_roll_list[PAOHUAN_ROLLPOOL_TOTAL_COUNT];
};

#endif // __PAOHUAN_TASK_HPP__

