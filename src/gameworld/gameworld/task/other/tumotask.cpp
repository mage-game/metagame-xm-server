#include "tumotask.hpp"
#include "task/taskpool.h"
#include "task/taskrecorder.h"
#include "engineadapter.h"
#include "servercommon/errornum.h"
#include "gamecommon.h"
#include "config/logicconfigmanager.hpp"
#include "other/vip/vipconfig.hpp"
#include "config/joinlimitconfig.hpp"
#include "other/event/eventhandler.hpp"
#include "obj/character/role.h"
#include "other/route/mailroute.hpp"
#include "item/itempool.h"
#include "gamelog.h"
#include "item/knapsack.h"
#include "task/newtaskmanager.h"
#include "other/vip/vip.hpp"
#include "other/daycounter/daycounter.hpp"
#include "other/sublock/sublock.hpp"
#include "protocal/msgnpc.h"
#include "other/itemlevelreward/itemlevelreward.hpp"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

DailyTask::DailyTask()
{

}

DailyTask::~DailyTask()
{

}

void DailyTask::Init(Role *role, short daily_task_can_fetch_flag, short daily_task_fetch_flag, unsigned int daily_task_exp_ball_valid_time, long long daily_task_exp_ball_exp)
{
	m_role = role;

	m_daily_task_can_fetch_flag = daily_task_can_fetch_flag;
	m_daily_task_fetch_flag = daily_task_fetch_flag;
	m_daily_task_exp_ball_valid_time = daily_task_exp_ball_valid_time;
	m_daily_task_exp_ball_exp = daily_task_exp_ball_exp;
}

void DailyTask::GetInitParam(short *daily_task_can_fetch_flag, short *daily_task_fetch_flag, unsigned int *daily_task_exp_ball_valid_time, long long *daily_task_exp_ball_exp)
{
	*daily_task_can_fetch_flag = m_daily_task_can_fetch_flag;
	*daily_task_fetch_flag = m_daily_task_fetch_flag;
	*daily_task_exp_ball_valid_time = m_daily_task_exp_ball_valid_time;
	*daily_task_exp_ball_exp = m_daily_task_exp_ball_exp;
}

void DailyTask::Update(time_t now_second)
{
	JoinLimitUnit limit_unit;
	bool is_limit = LOGIC_CONFIG->GetJoinLimitConfig().GetOtherJoinLimit(JLMT_OTHER_DAILY_TASK, &limit_unit);

	if (!is_limit || m_role->GetLevel() >= limit_unit.level)
	{
		time_t now = EngineAdapter::Instance().Time();
		if (now >= m_check_task_timestamp + 3)
		{
			m_check_task_timestamp = now;

			this->ResetDailyTask();
		}
	}

	this->CheckExpBall(now_second);
}

void DailyTask::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	if (old_dayid != now_dayid)
	{
		int task_id = 0;
		if (m_role->GetTaskManager()->GetTaskIdByType(TASK_TYPE_DAILY, &task_id))
		{
			m_role->GetTaskManager()->GiveupTask(task_id, true);

			ROLE_LOG_QUICK6(LOG_TYPE_GIVEUP_TASK, m_role, TASK_TYPE_DAILY, task_id, NULL, NULL);
		}

		this->ResetDailyTask(true);
	}

	m_daily_task_can_fetch_flag = 0;							
	m_daily_task_fetch_flag = 0;
}

void DailyTask::SendTuMoTaskInfo(short notify_reason)
{
	Protocol::SCTuMoTaskInfo tmti;
	tmti.notify_reason = notify_reason;
	tmti.commit_times = m_role->GetDayCounter()->GetDayCount(DayCounter::DAYCOUNT_ID_COMMIT_DAILY_TASK_COUNT);
	
	int task_id = 0;
	tmti.is_accept = (m_role->GetTaskManager()->GetTaskIdByType(TASK_TYPE_DAILY, &task_id) ? 1 : 0);
	tmti.has_fetch_complete_all_reward = (char)m_role->GetDayCounter()->GetDayCount(DayCounter::DAYCOUNT_ID_FETCH_DAILY_COMPLETE_TASK_REWARD_TIMES);
	tmti.task_id = static_cast<TaskID>(task_id);

	tmti.daily_task_can_fetch_flag = m_daily_task_can_fetch_flag;
	tmti.daily_task_fetch_flag = m_daily_task_fetch_flag;
	tmti.daily_task_exp_ball_valid_time = m_daily_task_exp_ball_valid_time;
	tmti.daily_task_exp_ball_exp = m_daily_task_exp_ball_exp;

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&tmti, sizeof(tmti));
}

void DailyTask::OnCommitTask(bool is_commit_all, TaskID task_id)
{
	if (is_commit_all)
	{
		this->OnOneKeyFinishAllTumoTask();
	}
	else
	{
		this->OnCommitOneTask(task_id);
	}
}

bool DailyTask::SetCount(int count, bool only_reduce)
{
	if (count < 0 || count > DAILY_ONEDAY_COMMIT_MAX) return false;

	if (only_reduce && count >= m_role->GetDayCounter()->GetDayCount(DayCounter::DAYCOUNT_ID_COMMIT_DAILY_TASK_COUNT)) return false;

	m_role->GetDayCounter()->DayCountSet(DayCounter::DAYCOUNT_ID_COMMIT_DAILY_TASK_COUNT, count);
	this->SendTuMoTaskInfo();

	return true;
}

TaskID DailyTask::GetAcceptabelDailyTask()
{
	if (!this->CanAcceptTask())
	{
		return 0;
	}

	int task_id = 0;
	if (m_role->GetTaskManager()->GetTaskIdByType(TASK_TYPE_DAILY, &task_id))
	{
		return 0;
	}

	if (m_role->GetLevel() <= TASKPOOL->GetFixDailyTaskLevel())
	{
		int fix_first_daily_task = TASKPOOL->GetFixFirstDailyTask();
		if (!m_role->GetTaskRecorder()->IsFinished(fix_first_daily_task))
		{
			task_id = fix_first_daily_task;
		}
		else
		{
			task_id = ++fix_first_daily_task;

			if (task_id - DAILY_ONEDAY_COMMIT_MAX >= fix_first_daily_task)
			{
				task_id = fix_first_daily_task;
			}
		}
	}
	else
	{
		// 随机出不能自动领取的日常任务
		TASKPOOL->RandDailyTaskByRoleLevel(m_role->GetLevel(), &task_id, m_last_dailytask_fb_scene_id);
	}

	const TaskCfg *tumo_task = TASKPOOL->GetCfgByTaskID(task_id);
	if (NULL == tumo_task || TASK_TYPE_DAILY != tumo_task->task_type)
	{
		return 0;
	}

	return task_id;
}

void DailyTask::ResetDailyTask(bool is_silent, TaskID remove_task_id)
{
	if (!this->CanAcceptTask())
	{
		return;
	}
	
	int task_id = 0;
	if (!m_role->GetTaskManager()->GetTaskIdByType(TASK_TYPE_DAILY, &task_id))
	{
		if (m_role->GetLevel() <= TASKPOOL->GetFixDailyTaskLevel())
		{
			int fix_first_daily_task = TASKPOOL->GetFixFirstDailyTask();
			if (0 == remove_task_id)
			{
				task_id = fix_first_daily_task;
			}
			else
			{
				task_id = ++remove_task_id;

				if (task_id - DAILY_ONEDAY_COMMIT_MAX >= fix_first_daily_task)
				{
					task_id = fix_first_daily_task;
				}
			}
		}
		else
		{
			if (!TASKPOOL->RandDailyTaskByRoleLevel(m_role->GetLevel(), &task_id, m_last_dailytask_fb_scene_id)) 
			{
				return;
			}
		}
		
		const TaskCfg *tumo_task = TASKPOOL->GetCfgByTaskID(task_id);
		if (NULL != tumo_task && TASK_TYPE_DAILY == tumo_task->task_type) 
		{
			if (!m_role->GetTaskManager()->AcceptTask(task_id, false, is_silent))
			{
				// donothing
			}
			this->SendTuMoTaskInfo();
		}
		else
		{
			gamelog::g_log_daily_task.printf(LL_INFO, "ResetDailyTask no taskid:%d error", task_id);
		}
	}
}

bool DailyTask::HasLeftCount()
{
	return m_role->GetDayCounter()->DayCountCheck(DayCounter::DAYCOUNT_ID_COMMIT_DAILY_TASK_COUNT, DAILY_ONEDAY_COMMIT_MAX);
}

void DailyTask::OnAddTask(TaskID task_id, bool is_init)
{
	if (is_init)
	{
		return;
	}

	const TaskCfg *task_cfg = TASKPOOL->GetCfgByTaskID(task_id);
	if (NULL != task_cfg && TASK_TYPE_DAILY == task_cfg->task_type)
	{
		this->SendTuMoTaskInfo(Protocol::TUMO_NOTIFY_REASON_ADD_TASK);

		EventHandler::Instance().OnAcceptDailyTask(m_role);
	}
}

void DailyTask::OnRemoveTask(TaskID task_id, bool is_complete)
{
	int trial_trunk_task_id = TASKPOOL->GetDailyPreTask();
	if (trial_trunk_task_id == task_id)
	{
		this->ResetDailyTask();
		this->SendTuMoTaskInfo();
	}

	const TaskCfg *task_cfg = TASKPOOL->GetCfgByTaskID(task_id);
	if (NULL != task_cfg && TASK_TYPE_DAILY == task_cfg->task_type)
	{
		if (is_complete)
		{
			m_last_dailytask_fb_scene_id = task_cfg->c_param1;
			EventHandler::Instance().OnCompleteDailyTask(m_role, 1);

			if (!this->HasLeftCount())
			{
				if (this->SendFinishRewardToUser())
				{
					m_role->GetDayCounter()->DayCountIncrease(DayCounter::DAYCOUNT_ID_FETCH_DAILY_COMPLETE_TASK_REWARD_TIMES);
				}
			}

			this->ResetDailyTask(false, task_id);
		}
		else
		{
			this->SendTuMoTaskInfo(Protocol::TUMO_NOTIFY_REASON_REMOVE_TASK);
		}
	}
}

bool DailyTask::CanAcceptTask(bool notify)
{
	JoinLimitUnit limit_unit;
	bool is_limit = LOGIC_CONFIG->GetJoinLimitConfig().GetOtherJoinLimit(JLMT_OTHER_DAILY_TASK, &limit_unit);

	if (is_limit && m_role->GetLevel() < limit_unit.level)
	{
		if (notify) m_role->NoticeNum(errornum::EN_TASK_ACCEPT_LEVEL);
		return false;
	}

	if (!m_role->GetTaskRecorder()->IsFinished(TASKPOOL->GetDailyPreTask())) 
	{
		if (notify) m_role->NoticeNum(errornum::EN_TUMO_PRETASK_UNCOMPLETE);
		return false;
	}

	if (!this->HasLeftCount())
	{
		if (notify) m_role->NoticeNum(errornum::EN_TUMO_TASK_ACCEPT_NO_COUNT);
		return false;
	}

	int task_id = 0;
	if (m_role->GetTaskManager()->GetTaskIdByType(TASK_TYPE_DAILY, &task_id))
	{
		if (notify) m_role->NoticeNum(errornum::EN_TUMO_ALREADY_HAS_TASK);
		return false;
	}

	return true;
}

bool DailyTask::DoReward(const TaskCfg *task_cfg) 
{
	if (NULL == task_cfg || TASK_TYPE_DAILY != task_cfg->task_type)
	{
		return false;
	}

// 	const DailyTaskReward *task_reward = TASKPOOL->GetDailyReward(m_role->GetLevel());
// 	if (NULL == task_reward)
// 	{
// 		return false;
// 	}
// 	
// 	if (task_reward->exp > 0)
// 	{
// 		m_role->AddExp(task_reward->exp, "DailyTaskReward");
// 	}
// 	if (task_reward->bind_coin > 0)
// 	{
// 		m_role->GetKnapsack()->GetMoney()->AddCoinBind(task_reward->bind_coin, "DailyTaskReward");
// 	}
// 
// 	ROLE_LOG_QUICK10(LOG_TYPE_DAILY_TASK_COMPLETE, m_role, m_role->GetLevel(), task_cfg->task_id, "CommonReward", NULL, 1, task_reward->exp, task_reward->bind_coin, 0);
	return true;
}

int DailyTask::GetComsumeGoldBind(int reward_type)
{
	const DailyTaskOtherCfg & other_cfg = TASKPOOL->GetDailyTaskOtherCfg();
	int comsume_goldbind = 0;

	switch (reward_type)
	{
	case DAILY_TASK_REWARD_TYPE_NORMAL:
	{
		comsume_goldbind = 0;
	}
	break;

	case DAILY_TASK_REWARD_TYPE_DOUBLE:
	{
		int role_vip_level = m_role->GetVip()->GetVipLevel();
		if (role_vip_level < other_cfg.free_double_vip_level)
		{
			comsume_goldbind = other_cfg.double_gold;
		}	
	}
	break;

	case DAILY_TASK_REWARD_TYPE_TREBLE:
	{
		comsume_goldbind = other_cfg.daily_treble_gold;
	}
	break;

	default:
		break;
	}

	return comsume_goldbind;
}

bool DailyTask::IsCanFetch(int reward_type)
{
	const DailyTaskOtherCfg & other_cfg = TASKPOOL->GetDailyTaskOtherCfg();
	int vip_level = m_role->GetVip()->GetVipLevel();

	if (reward_type == DAILY_TASK_REWARD_TYPE_TREBLE && vip_level < other_cfg.free_double_vip_level)
	{
		return false;
	}

	return true;
}

void DailyTask::CheckExpBall(time_t now_second)
{
	if (0 != m_daily_task_exp_ball_valid_time && static_cast<unsigned int>(now_second) >= m_daily_task_exp_ball_valid_time)
	{

		const DailyTaskOtherCfg & other_cfg = TASKPOOL->GetDailyTaskOtherCfg();
		int vip_level = m_role->GetVip()->GetVipLevel();

		if (vip_level < other_cfg.free_double_vip_level)
		{
			m_daily_task_exp_ball_exp = 0;
			m_daily_task_exp_ball_valid_time = 0;
		}
		else
		{
			m_role->AddExp(m_daily_task_exp_ball_exp, "DailyTask::OnFetchExpBallReward", Role::EXP_ADD_DAILY_TASK_EXP_BALL);

			m_daily_task_exp_ball_exp = 0;
			m_daily_task_exp_ball_valid_time = 0;
		}

		this->SendTuMoTaskInfo(Protocol::TUMO_NOTIFY_REASON_EXP_BALL_CHANGE);
	}
}

void DailyTask::CheckSyncRewardFlag(bool is_onekey)
{
	int task_committimes = m_role->GetDayCounter()->GetDayCount(DayCounter::DAYCOUNT_ID_COMMIT_DAILY_TASK_COUNT);
	if (task_committimes <= 0 || task_committimes > DAILY_ONEDAY_COMMIT_MAX)
	{
		return;
	}

	UNSTD_STATIC_CHECK(DAILY_ONEDAY_COMMIT_MAX < 16);

	SET_BIT(m_daily_task_can_fetch_flag, (task_committimes - 1));

	int reason = is_onekey ? Protocol::TUMO_NOTIFY_REASON_ONE_KEY_FINISH : 0;
	this->SendTuMoTaskInfo(reason);
}

void DailyTask::OnFetchTaskReward(int seq, int reward_type)
{
	if (seq < 0 || seq >= DAILY_ONEDAY_COMMIT_MAX || reward_type <= DAILY_TASK_REWARD_TYPE_VALID || reward_type >= DAILY_TASK_REWARD_TYPE_MAX)
	{
		return;
	}

	if (!IS_BIT_SET(m_daily_task_can_fetch_flag, seq))
	{
		m_role->NoticeNum(errornum::EN_TUMO_TASK_NO_COMPLETE);
		return;
	}

	if (IS_BIT_SET(m_daily_task_fetch_flag, seq))
	{
		m_role->NoticeNum(errornum::EN_TUMO_HAS_FETCH_COMPLETE_ALL_REWARD);
		return;
	}

	if (!this->IsCanFetch(reward_type))
	{
		return;
	}

	const DailyTaskReward *task_reward = TASKPOOL->GetDailyReward(m_role->GetLevel());
	if (NULL == task_reward)
	{
		return;
	}

	// 消耗
	int comsume_goldbind = this->GetComsumeGoldBind(reward_type);

	if (comsume_goldbind > 0 && !m_role->GetKnapsack()->GetMoney()->AllGoldMoreThan(comsume_goldbind))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	if (comsume_goldbind > 0 && !m_role->GetKnapsack()->GetMoney()->UseAllGold(comsume_goldbind, "DailyTask::OnFetchTaskReward"))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	// 奖励
	//long long real_reward_exp = task_reward->exp * reward_type;

	//策划需求，修改增加经验规则（2018-1-3）
	//const DailyTaskOtherCfg & other_cfg = TASKPOOL->GetDailyTaskOtherCfg();
	long long real_reward_exp = static_cast<long long>(TASKPOOL->GetDailyTaskOtherCfg().exp_factor_dailytask * (LOGIC_CONFIG->GetItemLevelRewardConfig().GetExpDanReward(m_role->GetLevel(), 0)));

	real_reward_exp *= reward_type;
	if (real_reward_exp > 0)
	{
		m_role->AddExp(real_reward_exp, "DailyTaskReward", Role::EXP_ADD_DAILY_TASK_EXP_BALL);
	}
	if (task_reward->bind_coin > 0)
	{
		m_role->GetKnapsack()->GetMoney()->AddCoinBind(task_reward->bind_coin, "DailyTaskReward");
	}

	// 设置标记
	SET_BIT(m_daily_task_fetch_flag, seq);

	const DailyTaskOtherCfg & other_cfg = TASKPOOL->GetDailyTaskOtherCfg();

	// 经验球
	if (other_cfg.exp_ball_limit > 0)
	{
		if (0 == m_daily_task_exp_ball_valid_time)
		{
			m_daily_task_exp_ball_valid_time = static_cast<unsigned int>(EngineAdapter::Instance().Time() + other_cfg.valid_time_h * SECOND_PER_HOUR);
		}

		m_daily_task_exp_ball_exp += real_reward_exp;
		if (m_daily_task_exp_ball_exp > other_cfg.exp_ball_limit)
		{
			m_daily_task_exp_ball_exp = other_cfg.exp_ball_limit;
		}
	}


	this->SendTuMoTaskInfo(Protocol::TUMO_NOTIFY_REASON_EXP_BALL_CHANGE);

	ROLE_LOG_QUICK10(LOG_TYPE_DAILY_TASK_COMPLETE, m_role, m_role->GetLevel(), seq, "CommonReward", NULL, 1, real_reward_exp, task_reward->bind_coin, 0);
}

void DailyTask::OnFetchExpBallReward()
{
	if (m_daily_task_exp_ball_exp <= 0)
	{
		return;
	}

	const DailyTaskOtherCfg & other_cfg = TASKPOOL->GetDailyTaskOtherCfg();
	int vip_level = m_role->GetVip()->GetVipLevel();

	if (vip_level < other_cfg.free_double_vip_level)
	{
		m_role->NoticeNum(errornum::EN_VIP_LEVEL_NOT_ENOUGH);
		return;
	}

	m_role->AddExp(m_daily_task_exp_ball_exp, "DailyTask::OnFetchExpBallReward", Role::EXP_ADD_DAILY_TASK_EXP_BALL);

	m_daily_task_exp_ball_exp = 0;
	m_daily_task_exp_ball_valid_time = 0;

	this->SendTuMoTaskInfo(Protocol::TUMO_NOTIFY_REASON_EXP_BALL_CHANGE);
}

void DailyTask::FetchCompleteAllTaskReward()
{
	if (this->HasLeftCount())
	{
		m_role->NoticeNum(errornum::EN_TUMO_COMPLETE_ALL_TASK_FIRST);
		return;
	}

	if (!m_role->GetDayCounter()->DayCountCheck(DayCounter::DAYCOUNT_ID_FETCH_DAILY_COMPLETE_TASK_REWARD_TIMES, 1))
	{
		m_role->NoticeNum(errornum::EN_TUMO_HAS_FETCH_COMPLETE_ALL_REWARD);
		return;
	}

	const DailyTaskReward *reward_cfg = TASKPOOL->GetDailyReward(m_role->GetLevel());
	if (NULL == reward_cfg)
	{
		return;
	}

	m_role->GetDayCounter()->DayCountIncrease(DayCounter::DAYCOUNT_ID_FETCH_DAILY_COMPLETE_TASK_REWARD_TIMES);

	m_role->GetKnapsack()->PutListOrMail(reward_cfg->complete_all_daily_reward_itemlist, PUT_REASON_DAILY_TASK_REWARD);

	if (reward_cfg->complete_all_daily_reward_coin > 0)
	{
		m_role->GetKnapsack()->GetMoney()->AddCoinBind(reward_cfg->complete_all_daily_reward_coin, "CompleteAllDailyReward");
	}

	this->SendTuMoTaskInfo();
}

void DailyTask::OnOneKeyFinishAllTumoTask()
{
	JoinLimitUnit limit_unit;
	bool is_limit = LOGIC_CONFIG->GetJoinLimitConfig().GetOtherJoinLimit(JLMT_OTHER_DAILY_TASK, &limit_unit);

	if (is_limit && m_role->GetLevel() < limit_unit.level) 
	{
		return;
	}

	int vip_level = m_role->GetVip()->GetVipLevel();
	int can_do = LOGIC_CONFIG->GetVipConfig().GetAuthParam(vip_level, VAT_FINISH_ALL_DAILY_TASK);
	if (0 == can_do)
	{
		m_role->NoticeNum(errornum::EN_VIP_LEVEL_NOT_ENOUGH);
		return;
	}

	if (!m_role->GetTaskRecorder()->IsFinished(TASKPOOL->GetDailyPreTask())) 
	{
		m_role->NoticeNum(errornum::EN_TUMO_PRETASK_UNCOMPLETE);
		return;
	}

	const DailyTaskReward *task_reward = TASKPOOL->GetDailyReward(m_role->GetLevel());
	if (NULL == task_reward)
	{
		return;
	}

	int task_committimes = m_role->GetDayCounter()->GetDayCount(DayCounter::DAYCOUNT_ID_COMMIT_DAILY_TASK_COUNT);

	int left_times = DAILY_ONEDAY_COMMIT_MAX - task_committimes;
	if (left_times <= 0)
	{
		m_role->NoticeNum(errornum::EN_TUMO_TASK_NO_TASK);
		return;
	}

	if (left_times > DAILY_ONEDAY_COMMIT_MAX)
	{
		left_times = DAILY_ONEDAY_COMMIT_MAX;
	}

	// 根据vip等级获取奖励类型和所需绑元单价
	const DailyTaskOtherCfg & other_cfg = TASKPOOL->GetDailyTaskOtherCfg();

	int single_need_gold = vip_level < other_cfg.free_double_vip_level ? other_cfg.double_gold : other_cfg.daily_treble_gold;
	int reward_type = vip_level < other_cfg.free_double_vip_level ? DAILY_TASK_REWARD_TYPE_DOUBLE : DAILY_TASK_REWARD_TYPE_TREBLE;
	int need_gold = left_times * single_need_gold;

	if (m_role->GetSublock()->CheckHasLock()) 
	{
		return;
	}
	if (!m_role->GetKnapsack()->GetMoney()->UseAllGold(need_gold, "OneKeyFinishTumo"))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	//long long reward_exp = task_reward->exp * reward_type * left_times;

	//策划需求，修改增加经验规则（2018-1-3）
	//const DailyTaskOtherCfg & other_cfg = TASKPOOL->GetDailyTaskOtherCfg();
	long long base_reward_exp = static_cast<long long>(TASKPOOL->GetDailyTaskOtherCfg().exp_factor_dailytask * (LOGIC_CONFIG->GetItemLevelRewardConfig().GetExpDanReward(m_role->GetLevel(), 0)));
	long long reward_exp = base_reward_exp * reward_type * left_times;
	long long real_exp = 0;
	if (reward_exp > 0)
	{
		m_role->AddExp(reward_exp, "OneKeyDailyTaskReward", Role::EXP_ADD_DAILY_TASK_EXP_BALL, &real_exp);
	}

	int reward_bind_coin = task_reward->bind_coin * reward_type * left_times;
	if (reward_bind_coin > 0)
	{
		m_role->GetKnapsack()->GetMoney()->AddCoinBind(reward_bind_coin, "OneKeyDailyTaskReward");
	}

	int task_id = 0;
	if (m_role->GetTaskManager()->GetTaskIdByType(TASK_TYPE_DAILY, &task_id))
	{
		m_role->GetTaskManager()->GiveupTask(task_id);

		// 实际上当作完成了任务
		ROLE_LOG_QUICK6(LOG_TYPE_COMMIT_TASK, m_role, TASK_TYPE_DAILY, task_id, NULL, NULL);

		m_role->GetTaskManager()->taskCommitRandomReward(TASK_TYPE_DAILY, left_times, 1, 0, real_exp);
	}

	EventHandler::Instance().OnCompleteDailyTask(m_role, left_times);

	for (int i = 0; i < left_times; ++ i)
	{
		int tmp_task_id = 0;
		if (TASKPOOL->RandDailyTaskByRoleLevel(m_role->GetLevel(), &tmp_task_id, m_last_dailytask_fb_scene_id)) 
		{
			m_role->GetTaskManager()->OnCommitOneTask(static_cast<TaskID>(tmp_task_id));
		}
	}

	// 设置奖励标记
	for (int seq = 0; seq < DAILY_ONEDAY_COMMIT_MAX; seq ++)
	{
		SET_BIT(m_daily_task_can_fetch_flag, seq);
		SET_BIT(m_daily_task_fetch_flag, seq);
	}

	if (!this->HasLeftCount())
	{
		if (this->SendFinishRewardToUser())
		{
			m_role->GetDayCounter()->DayCountIncrease(DayCounter::DAYCOUNT_ID_FETCH_DAILY_COMPLETE_TASK_REWARD_TIMES);
		}
	}

	// 设置经验球
	if (other_cfg.exp_ball_limit > 0)
	{
		if (0 == m_daily_task_exp_ball_valid_time)
		{
			m_daily_task_exp_ball_valid_time = static_cast<unsigned int>(EngineAdapter::Instance().Time() + other_cfg.valid_time_h * SECOND_PER_HOUR);
		}

		m_daily_task_exp_ball_exp += reward_exp;
		if (m_daily_task_exp_ball_exp > other_cfg.exp_ball_limit)
		{
			m_daily_task_exp_ball_exp = other_cfg.exp_ball_limit;
		}
	}

	this->SendTuMoTaskInfo(Protocol::TUMO_NOTIFY_REASON_ONE_KEY_FINISH);

	{
		gamelog::g_log_daily_task.printf(LL_INFO, "DailyTask::OnOneKeyFinishAllTumoTask user[%d, %s], complete_times[%d], use_all_gold[%d], reward_exp[%lld]",
			m_role->GetUID(), m_role->GetName(), left_times, need_gold, reward_exp);

		ROLE_LOG_QUICK10(LOG_TYPE_DAILY_TASK_COMPLETE, m_role, m_role->GetLevel(), task_id, "OneKeyComplete", NULL, left_times, reward_exp, reward_bind_coin, 0);
	}
}

void DailyTask::OnCommitOneTask(TaskID task_id)
{
	const TaskCfg *task_cfg = TASKPOOL->GetCfgByTaskID(task_id);
	if (NULL == task_cfg)
	{
		return;
	}

	if (NULL == task_cfg || TASK_TYPE_DAILY != task_cfg->task_type)
	{
		return;
	}

	JoinLimitUnit limit_unit;
	bool is_limit = LOGIC_CONFIG->GetJoinLimitConfig().GetOtherJoinLimit(JLMT_OTHER_DAILY_TASK, &limit_unit);

	if (is_limit && m_role->GetLevel() < limit_unit.level) 
	{
		return;
	}

	bool task_isfinish = false;
	if (!m_role->GetTaskManager()->HasTask(task_id, &task_isfinish))
	{
		m_role->NoticeNum(errornum::EN_TUMO_TASK_NO_TASK);
		return;
	}
	if (!task_isfinish)
	{
		m_role->NoticeNum(errornum::EN_TUMO_TASK_NO_COMPLETE);
		return;
	}

	const DailyTaskReward *task_reward = TASKPOOL->GetDailyReward(m_role->GetLevel());
	if (NULL == task_reward)
	{
		return;
	}

	if (m_role->GetSublock()->CheckHasLock()) 
	{
		return;
	}

	int need_gold = TASKPOOL->GetDailyFinishDoubleGold();

	if (!m_role->GetKnapsack()->GetMoney()->UseGold(need_gold, "OneTaskCommitTumo"))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	m_role->GetTaskManager()->GiveupTask(task_id);
	
	m_role->GetTaskManager()->taskCommitRandomReward(TASK_TYPE_DAILY, 1, 1);
	// 实际上是完成了任务
	ROLE_LOG_QUICK6(LOG_TYPE_COMMIT_TASK, m_role, TASK_TYPE_DAILY, task_id, NULL, NULL);

	//long long reward_exp = task_reward->exp * DAILY_GOLD_COMMIT_REWARD_TIMES;

	//策划需求，修改增加经验规则（2018-1-3）
	const DailyTaskOtherCfg & other_cfg = TASKPOOL->GetDailyTaskOtherCfg();
	long long reward_exp = static_cast<long long>(other_cfg.exp_factor_dailytask * (LOGIC_CONFIG->GetItemLevelRewardConfig().GetExpDanReward(m_role->GetLevel(), 0)));
	reward_exp *= DAILY_GOLD_COMMIT_REWARD_TIMES;
	if (reward_exp > 0)
	{
		m_role->AddExp(reward_exp, "OneTaskDailyTaskReward", Role::EXP_ADD_DAILY_TASK_EXP_BALL);
	}

	int reward_bind_coin = task_reward->bind_coin * DAILY_GOLD_COMMIT_REWARD_TIMES;
	if (reward_bind_coin > 0)
	{
		m_role->GetKnapsack()->GetMoney()->AddCoinBind(reward_bind_coin, "OneTaskDailyTaskReward");
	}

	m_role->GetTaskManager()->OnCommitOneTask(static_cast<TaskID>(task_cfg->task_id));

	EventHandler::Instance().OnCompleteDailyTask(m_role, 1);

	if (!this->HasLeftCount())
	{	
		if (this->SendFinishRewardToUser())
		{
			m_role->GetDayCounter()->DayCountIncrease(DayCounter::DAYCOUNT_ID_FETCH_DAILY_COMPLETE_TASK_REWARD_TIMES);
		}
	}

	this->ResetDailyTask(false, task_id);

	{
		gamelog::g_log_daily_task.printf(LL_INFO, "DailyTask::OnCommitOneTask user[%d, %s], use_all_gold[%d], reward_exp[%lld] reward_bind_coin[%d]",
			m_role->GetUID(), m_role->GetName(), need_gold, reward_exp, reward_bind_coin);

		ROLE_LOG_QUICK10(LOG_TYPE_DAILY_TASK_COMPLETE, m_role, m_role->GetLevel(), task_cfg->task_id, "DoubleReward", NULL, 1, reward_exp, reward_bind_coin, 0);
	}
}


bool DailyTask::SendFinishRewardToUser()
{
	if (!m_role->GetDayCounter()->DayCountCheck(DayCounter::DAYCOUNT_ID_FETCH_DAILY_COMPLETE_TASK_REWARD_TIMES, 1))
	{
		return false;
	}

	const DailyTaskReward *task_reward = TASKPOOL->GetDailyReward(m_role->GetLevel());
	if (NULL == task_reward)
	{
		return false;
	}

	static MailContentParam contentparam; contentparam.Reset();
	contentparam.coin_bind = task_reward->complete_all_daily_reward_coin;

	m_role->GetKnapsack()->PutListOrMail(task_reward->complete_all_daily_reward_itemlist, PUT_REASON_DAILY_TASK_REWARD);

	return true;
}