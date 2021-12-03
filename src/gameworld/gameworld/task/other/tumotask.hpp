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
	bool DoReward(const TaskCfg *task_cfg);									// ������

	int GetComsumeGoldBind(int reward_type);								// ��ȡ��ͬ�����������ĵİ�Ԫ
	bool IsCanFetch(int reward_type);										// �ܷ���ȡĳ�����ͽ���
	void CheckExpBall(time_t now_second);									// ���������Զ���ȡ������
	void CheckSyncRewardFlag(bool is_onekey);								// �������ʱ���ñ��

	void OnFetchTaskReward(int seq, int reward_type);						 // ��ȡ������
	void OnFetchExpBallReward();											 // ��ȡ��������
	void FetchCompleteAllTaskReward();

	bool SetCount(int count, bool only_reduce);

	bool AllTaskComplete() { return !this->HasLeftCount(); }

	TaskID GetAcceptabelDailyTask();										  // ��ȡ�ܽ�ȡ������
	void OnOneKeyFinishAllTumoTask();										  // һ�����

private:
	void ResetDailyTask(bool is_silent = false, TaskID remove_task_id = 0);   // ��ȡ�µ��ճ�����
	bool HasLeftCount();
	void OnCommitOneTask(TaskID task_id);
	bool SendFinishRewardToUser();

	Role *m_role = nullptr;

	time_t m_check_task_timestamp = 0;										// �´μ������ʱ���
	TaskID m_last_dailytask_fb_scene_id = 0;								// ��һ������ճ���������id

	short m_daily_task_can_fetch_flag = 0;									// �ճ��������ȡ���
	short m_daily_task_fetch_flag = 0;										// �ճ�������ȡ���
	unsigned int m_daily_task_exp_ball_valid_time = 0;						// ���������ʱ��
	long long m_daily_task_exp_ball_exp = 0;								// ������ǰ����
};

#endif // __TUMO_TASK_HPP__

