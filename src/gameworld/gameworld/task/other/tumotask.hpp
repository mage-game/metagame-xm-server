#ifndef __TUMO_TASK_HPP__
#define __TUMO_TASK_HPP__

#include "task/taskpool.h"

class Role;

class DailyTask
{
public:
	DailyTask();
	~DailyTask();

	void SetRole(Role *role) { m_role = role; }
	void Init(Role *role, short daily_task_can_fetch_flag, short daily_task_fetch_flag, unsigned int daily_task_exp_ball_valid_time, long long daily_task_exp_ball_exp);
	void GetInitParam(short *daily_task_can_fetch_flag, short *daily_task_fetch_flag, unsigned int *daily_task_exp_ball_valid_time, long long *daily_task_exp_ball_exp);

	void Update(time_t now_second);
	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);

	void SendTuMoTaskInfo(short notify_reason = 0);
	void OnCommitTask(bool is_commit_all, TaskID task_id);

	void OnAddTask(TaskID task_id, bool is_init);
	void OnRemoveTask(TaskID task_id, bool is_complete);
	bool CanAcceptTask(bool notify = false);
	bool DoReward(const TaskCfg *task_cfg);									// 已无用

	int GetComsumeGoldBind(int reward_type);								// 获取不同奖励类型消耗的绑元
	bool IsCanFetch(int reward_type);										// 能否领取某种类型奖励
	void CheckExpBall(time_t now_second);									// 检查清除或自动领取经验球
	void CheckSyncRewardFlag(bool is_onekey);								// 完成任务时设置标记

	void OnFetchTaskReward(int seq, int reward_type);						 // 领取任务奖励
	void OnFetchExpBallReward();											 // 领取经验球奖励
	void FetchCompleteAllTaskReward();

	bool SetCount(int count, bool only_reduce);

	bool AllTaskComplete() { return !this->HasLeftCount(); }

	TaskID GetAcceptabelDailyTask();										  // 获取能接取的任务
	void OnOneKeyFinishAllTumoTask();										  // 一键完成

private:
	void ResetDailyTask(bool is_silent = false, TaskID remove_task_id = 0);   // 接取新的日常任务
	bool HasLeftCount();
	void OnCommitOneTask(TaskID task_id);
	bool SendFinishRewardToUser();

	Role *m_role = nullptr;

	time_t m_check_task_timestamp = 0;										// 下次检查任务时间戳
	TaskID m_last_dailytask_fb_scene_id = 0;								// 上一次完成日常副本场景id

	short m_daily_task_can_fetch_flag = 0;									// 日常任务可领取标记
	short m_daily_task_fetch_flag = 0;										// 日常任务领取标记
	unsigned int m_daily_task_exp_ball_valid_time = 0;						// 经验球清空时间
	long long m_daily_task_exp_ball_exp = 0;								// 经验球当前经验
};

#endif // __TUMO_TASK_HPP__

