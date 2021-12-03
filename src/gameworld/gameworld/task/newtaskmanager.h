#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include "gamedef.h"
#include "servercommon/taskdef.hpp"
#include "taskpool.h"
#include <set>

class Role;

class NewTaskManager
{
public:
	NewTaskManager();
	~NewTaskManager();

	void SetRole(Role *role) { m_role = role; }

	bool Init(Role *role, const TaskParam &taskparam);
	void GetOtherInitParam(TaskParam *param);
	
	void Update(time_t now_second);

	bool CanAcceptTask(TaskID task_id, bool notify = false);
	bool CanCommitTask(TaskID task_id, bool notify = false, bool *has_task = NULL);

	bool AcceptTask(TaskID task_id, bool notify = false, bool is_silent = false, bool is_check_can_accept = true);
	bool CommitTask(TaskID task_id, bool notify = true, bool is_silent = false, bool is_force = false);
	bool GiveupTask(TaskID task_id, bool is_silent = false);

	bool IsComplete(TaskInfo *task_info, const TaskCfg *task_cfg = NULL);
	bool IsComplete(TaskID task_id);

	void OnKillMonster(int scene_id, const Posi &pos, int monster_id, int level);
	void OnAddKillMonsterTask(int monster_type);
	void OnTalkToNPC(UInt16 npc_id);
	void OnGather(int gather_id);
	void OnEnterScene(int scene_id);
	void OnCommitOneTask(TaskID task_id);
	void OnPassFB(int fb_type);
	void OnPassFBLayer(int fb_type, int param = 0);
	void OnDoOperator(int operator_type);
	void OnCheckSatisfyStatus(int satisfy_status_type);
	void OnUpLevel(int old_level, int level);
	void OnGuildTaskStateChange();
	void OnZhuanzhiTaskStateChange();
	void OnEnterSceneType(int scene_type);
	void OnPickUpItem(ItemID item_id, int item_num);
	void OnPassSpecialTask(int type);
	void OnPassDailyTaskFB(int scene_id);
	void OnBianshenTalk(int type, int param);
	bool OnTaskConsumeItem(TaskID task_id);
	void OnGetItem(ItemID item_id, int item_num);

	void SendTaskListToRole();
	void SendAcceptableToRole();

	bool HasTask(TaskID task_id, bool *is_complete = NULL, unsigned int *accept_time = NULL);
	bool HasTaskType(int task_type);
	bool GetTaskIdByType(int task_type, int *task_id);
	bool HasTaskBianshen(int appearn, int param);   // 是否有未完成的变身任务

	bool ForceCompleteTask(TaskID task_id);
	void GMClearTrunkTask(TaskID start_task_id, bool is_clear_zhi_xian, bool add_start_task);
	void GMAddTask(TaskID task_id);
	void GMCommitTask(TaskID task_id);
	void GMCommitAllTask();

	TaskInfo * GetTaskInfo(TaskID task_id);

	void OnRoleInitFinish();
	void CheckCishaTaskValid(bool is_init);

	void AddBianshenTaskEffect();
	void RemoveBianshenTaskEffetc();

	void AddFlyTaskEffect();
	void RemoveFlyTaskEffetc();

	void AddDownSpeedTaskEffect();
	void RemoveDownSpeedTaskEffetc();
	
	void taskCommitRandomReward(int task_type, int count, int one_key, int param1 = 0, long long param2 = 0);

	bool GetCurZhuanzhiTaskid(int *task_id);
	void GetTaskDropIdList(std::set<UInt16>* drip_id_list);
private:
	Role			*m_role;
	bool			m_first;

	TaskParam		m_task_param;
	std::set<int>	m_acceptable_set;

	void SendTaskInfo(TaskInfo *taskinfo, int reason, const TaskCfg *task_cfg, bool is_silent = false);

	void OnAddTask(TaskID task_id, bool is_init);
	void OnRemoveTask(TaskID task_id, bool is_complete);
	void OnTaskChange(TaskID task_id, bool is_complete);

	bool RemoveTask(TaskID task_id, bool is_complete, bool is_silent);

	void CalcAcceptable(int reason, TaskID task_id, int up_level = 0);

	bool PreOperate(const TaskCfg *task_cfg, bool is_reverse = false);
	bool DoReward(const TaskCfg *task_cfg);
	bool DoRewardTRunk(const TaskCfg *task_cfg);

	void CheckProgress(int t_index, int task_condtion, int param1, int param2);
	bool CheckSatisfyStatus(int satisfy_type, int param1, int param2);
	void AdjustDoOperatorTimes(int task_id);
	void CheckTaskTypeStatus(int task_id);
	void CheckEnterSceneStatus(int task_id);
	void CheckOwnItemStatus(int task_id);

	enum CALC_ACC_REASON
	{
		CALC_ACC_REASON_INIT = 0,
		CALC_ACC_REASON_UPLEVEL,
		CALC_ACC_REASON_COMMIT,
		CAL_ACC_REASON_GUILD_TASK_STATE_CHANGE,
		CALC_ACC_REASON_ZHUANZHI_TASK_STATE_CHANGE,
	};
};

#endif
