#ifndef __HUSONG_TASK_HPP__
#define __HUSONG_TASK_HPP__

#include "servercommon/taskdef.hpp"
#include "gamedef.h"
#include "servercommon/maildef.hpp"
#include "protocal/msgnpc.h"

class Role;


class HusongTask
{
public:
	HusongTask();
	~HusongTask();

	void SetRole(Role *role) { m_role = role; }

	void Init(Role *role, const HusongTaskParam &p);
	void GetInitParam(HusongTaskParam *p);

	bool IsHusongStatus() { return 0 != m_husong_info.task_id; }
	bool CanAcceptHusongTask(bool notify);
	bool LittleHelperCanAcceptHusongTask(bool notify);
	void GetHusongInfo(TaskID *task_id, char *color);

	void OnAddTask(TaskID task_id, bool is_init);
	void OnLittleHelperAddTask(TaskID task_id, bool is_init);
	void OnRemoveTask(TaskID task_id, bool is_complete);
	void OnLittleHelperRemoveTask(TaskID task_id, bool is_complete);
	void OnDie(ObjID killer_objid);
	bool SetTaskColor(char color);

	void Update(unsigned int now_second);

	void OnRefreshTaskColor(Protocol::CSRefreshHusongTask *rhtt);
	void OnBuyTimes();
	bool OnBuyTimes(int times);
	int GetTimes();

	bool SetCount(int count, bool only_reduce);

	void OnAddShield();
	bool IsHusongDoubleTime();
	void SendHusongTaskInfo(short notify_reason = 0, bool is_broadcast = false);
	long long GetHusongExpFind();//护送日常找回经验
private:
	bool RefreshColorHelper(bool is_auto_buy, int &color_ret, int &free_times, int &stuff_times);
	void CheckBuffStatus(bool is_add);
	int CalcBaseReward(long long *base_commit_exp, int *base_commit_bind_coin, ItemConfigData reward_item[MAX_ATTACHMENT_ITEM_NUM]);
	
	Role *m_role;

	HusongTaskParam m_husong_info;														// 护送任务信息

	bool m_first;
	time_t m_task_timeout_stamp;														// 护送任务超时时间戳
};

#endif // __HUSONG_TASK_HPP__

