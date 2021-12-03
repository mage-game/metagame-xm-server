#include "paohuantask.hpp"
#include "task/taskpool.h"
#include "task/taskrecorder.h"
#include "engineadapter.h"
#include "servercommon/errornum.h"
#include "servercommon/string/gameworldstr.h"
#include "servercommon/commondata.hpp"
#include "obj/character/role.h"
#include "item/itempool.h"
#include "item/itembase.h"
#include "gamelog.h"
#include "world.h"
#include "config/logicconfigmanager.hpp"
#include "config/joinlimitconfig.hpp"
#include "item/knapsack.h"
#include "other/event/eventhandler.hpp"
#include "other/route/mailroute.hpp"
#include "task/newtaskmanager.h"
#include "other/sublock/sublock.hpp"
#include "protocal/msgrole.h"
#include "protocal/msgnpc.h"
#include "other/itemlevelreward/itemlevelreward.hpp"
#include "other/vip/vip.hpp"
#include "other/vip/vipconfig.hpp"

PaoHuanTask::PaoHuanTask()
	: m_role(NULL), m_check_task_timestamp(0), m_is_rolling(false)
{

}

PaoHuanTask::~PaoHuanTask()
{

}

void PaoHuanTask::Init(Role *role)
{
	m_role = role;
}

void PaoHuanTask::Update(time_t now_second)
{
	JoinLimitUnit limit_unit;
	bool is_limit = LOGIC_CONFIG->GetJoinLimitConfig().GetOtherJoinLimit(JLMT_OTHER_PAOHUAN_TASK, &limit_unit);

	if (!is_limit || m_role->GetLevel() >= limit_unit.level)
	{
		time_t now = EngineAdapter::Instance().Time();
		if (now >= m_check_task_timestamp + 3)
		{
			m_check_task_timestamp = now;

			if (this->CanAcceptTask())
			{
				this->ResetPaohuanTask();
			}
		}
	}
}

void PaoHuanTask::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	if (old_dayid != now_dayid)
	{
		CommonDataParam *common_p = m_role->GetCommonDataParam();

		common_p->paohuan_complete_count = 0;
		common_p->paohuan_getphase_reward_idx = 0;

		int task_id = 0;
		if (m_role->GetTaskManager()->GetTaskIdByType(TASK_TYPE_PAOHUAN, &task_id))
		{
			m_role->GetTaskManager()->GiveupTask(task_id, true);

			ROLE_LOG_QUICK6(LOG_TYPE_GIVEUP_TASK, m_role, TASK_TYPE_PAOHUAN, task_id, NULL, NULL);
		}

		if (this->CanAcceptTask())
		{
			this->ResetPaohuanTask(true);
		}
	}
}

void PaoHuanTask::OnRoleLeaveScene()
{
	// this->CheckPaoHuanRollState();
}

void PaoHuanTask::SendPaohuanTaskInfo(short notify_reason)
{
	CommonDataParam *common_p = m_role->GetCommonDataParam();

	static Protocol::SCPaohuanTaskInfo phti;
	phti.notify_reason = notify_reason;
	phti.complete_times = common_p->paohuan_complete_count;
	phti.fetch_phase_reward_count = common_p->paohuan_getphase_reward_idx;
	
	int task_id = 0;
	bool is_accpet = m_role->GetTaskManager()->GetTaskIdByType(TASK_TYPE_PAOHUAN, &task_id);

	phti.is_accept = (is_accpet ? 1 : 0);
	phti.task_id = static_cast<TaskID>(task_id);

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&phti, sizeof(phti));
}

void PaoHuanTask::OnSkipTask(bool is_skip_batch, TaskID task_id)
{
	//if (is_skip_batch)
	//{
	this->OnOneKeySkipTask();
	//}
	//else
	//{
	//	this->OnSkipOneTask(task_id);
	//}

}

void PaoHuanTask::ResetPaohuanTask(bool is_silent)
{
	CommonDataParam *common_p = m_role->GetCommonDataParam();

	if (common_p->paohuan_complete_count >= TASKPOOL->GetPaoHuanTaskCount())
	{
		return;
	}

	int task_id = 0;
	if (!m_role->GetTaskManager()->GetTaskIdByType(TASK_TYPE_PAOHUAN, &task_id))
	{
		if (!TASKPOOL->RandPaohuanTaskByRoleLevel(m_role->GetLevel(), &task_id)) 
		{
			return;
		}
		
		const TaskCfg *paohuan_task = TASKPOOL->GetCfgByTaskID(task_id);
		if (NULL != paohuan_task && TASK_TYPE_PAOHUAN == paohuan_task->task_type) 
		{
			if (m_role->GetTaskManager()->AcceptTask(task_id, true, is_silent))
			{
				this->SendPaohuanTaskInfo();
			}
		}
		else
		{
			gamelog::g_log_daily_task.printf(LL_INFO, "ResetPaohuanTask no taskid:%d error", task_id);
		}
	}
}

bool PaoHuanTask::HasLeftCount()
{
	CommonDataParam *common_p = m_role->GetCommonDataParam();

	if (common_p->paohuan_complete_count < PAOHUAN_ONEDAY_COMMIT_MAX)
	{
		return true;
	}

	return false;
}

void PaoHuanTask::OnAddTask(TaskID task_id, bool is_init)
{
	if (is_init)
	{
		return;
	}

	const TaskCfg *task_cfg = TASKPOOL->GetCfgByTaskID(task_id);
	if (NULL != task_cfg && TASK_TYPE_PAOHUAN == task_cfg->task_type)
	{
		this->SendPaohuanTaskInfo(Protocol::PAOHUAN_NOTIFY_REASON_ADD_TASK);

		EventHandler::Instance().OnAcceptPaohuanTask(m_role);
	}
}

void PaoHuanTask::OnRemoveTask(TaskID task_id, bool is_complete)
{
	const TaskCfg *task_cfg = TASKPOOL->GetCfgByTaskID(task_id);
	if (NULL != task_cfg && TASK_TYPE_PAOHUAN == task_cfg->task_type)
	{
		if (is_complete)
		{
			EventHandler::Instance().OnCompletePaohuanTask(m_role, 1);

			CommonDataParam *common_p = m_role->GetCommonDataParam();

			if (common_p->paohuan_complete_count < TASKPOOL->GetPaoHuanTaskCount())
			{
				this->AddCompleteCount();
				this->ResetPaohuanTask();
			}
		}
		
		this->SendPaohuanTaskInfo(Protocol::PAOHUAN_NOTIFY_REASON_REMOVE_TASK);
	}
}

bool PaoHuanTask::CanAcceptTask(bool notify)
{
	JoinLimitUnit limit_unit;

	bool is_limit = LOGIC_CONFIG->GetJoinLimitConfig().GetOtherJoinLimit(JLMT_OTHER_PAOHUAN_TASK, &limit_unit);
	if (is_limit && m_role->GetLevel() < limit_unit.level)
	{
		if (notify) m_role->NoticeNum(errornum::EN_TASK_ACCEPT_LEVEL);
		return false;
	}

	if (!this->HasLeftCount())
	{
		if (notify) m_role->NoticeNum(errornum::EN_PAOHUAN_TASK_ACCEPT_NO_COUNT);
		return false;
	}

	int task_id = 0;
	if (m_role->GetTaskManager()->GetTaskIdByType(TASK_TYPE_PAOHUAN, &task_id))
	{
		if (notify) m_role->NoticeNum(errornum::EN_PAOHUAN_ALREADY_HAS_TASK);
		return false;
	}

	CommonDataParam *common_p = m_role->GetCommonDataParam();

	if (common_p->paohuan_complete_count >= TASKPOOL->GetPaoHuanTaskCount())
	{
		return false;
	}

	return true;
}

bool PaoHuanTask::DoReward(const TaskCfg *task_cfg) 
{
	if (NULL == task_cfg || TASK_TYPE_PAOHUAN != task_cfg->task_type)
	{
		return false;
	}

	CommonDataParam *common_p = m_role->GetCommonDataParam();

	int reward_nvwashi = 0;
	long long exp = 0;
	ItemConfigData item;
	if (TASKPOOL->GetPaohuanTaskReward(m_role->GetLevel(), common_p->paohuan_complete_count, &reward_nvwashi, exp, item))
	{
		//m_role->AddNvWaShi(reward_nvwashi, "PaoHuanTaskReward");
		//m_role->AddExp(exp, "paohuan_task");

		//策划需求，修改增加经验规则（2018-1-3）
		const DailyTaskOtherCfg & other_cfg = TASKPOOL->GetDailyTaskOtherCfg();
		long long reward_exp = static_cast<long long>(other_cfg.exp_factor_huantask * (LOGIC_CONFIG->GetItemLevelRewardConfig().GetExpDanReward(m_role->GetLevel(), 0)));
		m_role->AddExp(reward_exp, "paohuan_task", Role::EXP_ADD_REASON_DEFAULT);
		m_role->GetKnapsack()->PutOrMail(item, PUT_REASON_PAOHUAN);

		ROLE_LOG_QUICK10(LOG_TYPE_PAOHUAN_TASK_COMPLETE, m_role, m_role->GetLevel(), task_cfg->task_id, "PaoHuanCommonReward", 
			NULL, 1, reward_nvwashi, 0, 0);

		return true;
	}
	
	return false;
}

void PaoHuanTask::OnOneKeySkipTask()
{
	if (m_role->GetLevel() < TASKPOOL->GetPaoHuanSkipTaskLimitLevel())
	{
		return;
	}

	CommonDataParam *common_p = m_role->GetCommonDataParam();
	
	int left_times = TASKPOOL->GetPaoHuanTaskCount() - common_p->paohuan_complete_count;
	if (left_times <= 0)
	{
		m_role->NoticeNum(errornum::EN_PAOHUAN_TASK_NO_TASK);
		return;
	}

	int vip_level = m_role->GetVip()->GetVipLevel();
	int can_do = LOGIC_CONFIG->GetVipConfig().GetAuthParam(vip_level, VAT_PAOHUAN_TASK_ONE_KEY);
	if (0 == can_do)
	{
		m_role->NoticeNum(errornum::EN_VIP_LEVEL_NOT_ENOUGH);
		return;
	}
	
	int phase_commit_times = TASKPOOL->GetPaoHuanOneKeyTaskCount();
	if (left_times < phase_commit_times) 
	{	
		phase_commit_times = left_times;
	}

	const int ONE_TASK_FINISH_NEED_GOLD = TASKPOOL->GetPaoHuanOneTaskFinishGold();
	int need_gold = phase_commit_times * ONE_TASK_FINISH_NEED_GOLD;

	if (m_role->GetSublock()->CheckHasLock()) 
	{
		return;
	}

	if (!m_role->GetKnapsack()->GetMoney()->UseAllGold(need_gold, "OnOneKeyFinishPhasePaohuanTask"))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	int reward_nvwashi = 0;
	int tmp_complete_count = common_p->paohuan_complete_count;
	long long total_reward_exp = 0;
	{
		for (int i = 0; i < phase_commit_times; i++)
		{
			int temp_nvwashi = 0;
			long long exp = 0;
			ItemConfigData item;
			if (TASKPOOL->GetPaohuanTaskReward(m_role->GetLevel(), tmp_complete_count, &temp_nvwashi, exp, item))
			{
				//m_role->AddExp(exp, "paohuan_task");

				//策划需求，修改增加经验规则（2018-1-3）
				const DailyTaskOtherCfg & other_cfg = TASKPOOL->GetDailyTaskOtherCfg();
				long long reward_exp = static_cast<long long>(other_cfg.exp_factor_huantask * (LOGIC_CONFIG->GetItemLevelRewardConfig().GetExpDanReward(m_role->GetLevel(), 0)));
				long long real_exp = 0;
				m_role->AddExp(reward_exp, "paohuan_task", Role::EXP_ADD_REASON_DEFAULT, &real_exp);

				m_role->GetKnapsack()->PutOrMail(item, PUT_REASON_PAOHUAN);
				reward_nvwashi += temp_nvwashi;
				total_reward_exp += real_exp;
				tmp_complete_count++;
			}
		}
	}

	if (reward_nvwashi > 0)
	{
		//m_role->AddNvWaShi(reward_nvwashi, "OnOneKeyFinishPhasePaohuanTask");
	}

	int task_id = 0;
	if (m_role->GetTaskManager()->GetTaskIdByType(TASK_TYPE_PAOHUAN, &task_id))
	{
		m_role->GetTaskManager()->GiveupTask(task_id);

		// 实际上当作完成了任务
		ROLE_LOG_QUICK6(LOG_TYPE_COMMIT_TASK, m_role, TASK_TYPE_PAOHUAN, task_id, NULL, NULL);

		const TaskCfg* task_cfg_p = TASKPOOL->GetCfgByTaskID(task_id);
		if (task_cfg_p)
		{
			m_role->GetTaskManager()->taskCommitRandomReward(task_cfg_p->task_type, left_times, 1, 0, total_reward_exp);
		}
	}

	// 增加完成次数
	for (int i = 0; i < phase_commit_times; i++)
	{
		this->AddCompleteCount();
	}

	EventHandler::Instance().OnCompletePaohuanTask(m_role, phase_commit_times);

	this->ResetPaohuanTask();

	this->SendPaohuanTaskInfo();

	{
		gamelog::g_log_daily_task.printf(LL_INFO, "PaoHuanTask::OnOneKeyFinishPhasePaohuanTask user[%d, %s], complete_times[%d], use_all_gold[%d], reward_nvwashi[%d]",
			m_role->GetUID(), m_role->GetName(), left_times, need_gold, reward_nvwashi);

		ROLE_LOG_QUICK10(LOG_TYPE_PAOHUAN_TASK_COMPLETE, m_role, m_role->GetLevel(), task_id, "OneKeySkipPaohuan", NULL, phase_commit_times, reward_nvwashi, 0, 0);
	}
}

void PaoHuanTask::OnSkipOneTask(TaskID task_id)
{
	const TaskCfg *task_cfg = TASKPOOL->GetCfgByTaskID(task_id);
	if (NULL == task_cfg)
	{
		return;
	}

	if (NULL == task_cfg || TASK_TYPE_PAOHUAN != task_cfg->task_type)
	{
		return;
	}

	JoinLimitUnit limit_unit;
	bool is_limit = LOGIC_CONFIG->GetJoinLimitConfig().GetOtherJoinLimit(JLMT_OTHER_PAOHUAN_TASK, &limit_unit);
	if (is_limit && m_role->GetLevel() < limit_unit.level) 
	{
		return;
	}

	if (!m_role->GetTaskManager()->HasTask(task_id))
	{
		m_role->NoticeNum(errornum::EN_PAOHUAN_TASK_NO_TASK);
		return;
	}
	
	CommonDataParam *common_p = m_role->GetCommonDataParam();

	int reward_nvwashi = 0;
	long long exp = 0;
	ItemConfigData item;
	if (!TASKPOOL->GetPaohuanTaskReward(m_role->GetLevel(), common_p->paohuan_complete_count, &reward_nvwashi, exp, item))
	{
		return;
	}

	if (m_role->GetSublock()->CheckHasLock()) 
	{
		return;
	}

	const int ONE_TASK_FINISH_NEED_GOLD = TASKPOOL->GetPaoHuanOneTaskFinishGold();
	if (!m_role->GetKnapsack()->GetMoney()->UseGold(ONE_TASK_FINISH_NEED_GOLD, "OneTaskCommitPaoHuan"))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	m_role->GetTaskManager()->GiveupTask(task_id);

	// 实际上是完成了任务
	ROLE_LOG_QUICK6(LOG_TYPE_COMMIT_TASK, m_role, TASK_TYPE_PAOHUAN, task_id, NULL, NULL);

	//m_role->AddNvWaShi(static_cast<int>(reward_nvwashi), "OnCommitOnePaoHuanTask");
	//m_role->AddExp(exp, "paohuan_task");

	//策划需求，修改增加经验规则（2018-1-3）
	const DailyTaskOtherCfg & other_cfg = TASKPOOL->GetDailyTaskOtherCfg();
	long long reward_exp = static_cast<long long>(other_cfg.exp_factor_huantask * (LOGIC_CONFIG->GetItemLevelRewardConfig().GetExpDanReward(m_role->GetLevel(), 0)));
	m_role->AddExp(reward_exp, "paohuan_task", Role::EXP_ADD_REASON_DEFAULT);

	m_role->GetKnapsack()->PutOrMail(item, PUT_REASON_PAOHUAN);

	m_role->GetTaskManager()->OnCommitOneTask(static_cast<TaskID>(task_cfg->task_id));

	EventHandler::Instance().OnCompletePaohuanTask(m_role, 1);
	
	this->AddCompleteCount();
	
	this->ResetPaohuanTask();

	this->SendPaohuanTaskInfo();

	{
		gamelog::g_log_daily_task.printf(LL_INFO, "PaoHuanTask::OnSkipOneTask user[%d, %s], use_all_gold[%d], reward_exp[%d]",
			m_role->GetUID(), m_role->GetName(), ONE_TASK_FINISH_NEED_GOLD, reward_nvwashi);

		ROLE_LOG_QUICK10(LOG_TYPE_PAOHUAN_TASK_COMPLETE, m_role, m_role->GetLevel(), task_cfg->task_id, "SkipOneTask", NULL, 1, reward_nvwashi, 0, 0);
	}
}

void PaoHuanTask::AddCompleteCount()
{
	CommonDataParam *common_p = m_role->GetCommonDataParam();

	if (common_p->paohuan_complete_count >= TASKPOOL->GetPaoHuanTaskCount())
	{
		return;
	}

	++ common_p->paohuan_complete_count;
	if (common_p->paohuan_complete_count >= PAOHUAN_ONEDAY_COMMIT_MAX)
	{
		int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_paohuan_task_finish_notice, UidToInt(m_role->GetUserId()), m_role->GetName(), (int)m_role->GetCamp());
		if (length > 0)
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_3, 0, 0);
		}
	}

	if (0 == common_p->paohuan_complete_count % 10)
	{
		// this->StartPaoHuanRoll();
	}
}

void PaoHuanTask::StartPaoHuanRoll()
{
	CommonDataParam *common_p = m_role->GetCommonDataParam();

	this->CheckPaoHuanRollState();

	int max_getphase_reward_idx = common_p->paohuan_complete_count / 10; 
	if (common_p->paohuan_getphase_reward_idx >= max_getphase_reward_idx)
	{
		return;
	}

	memset(m_roll_list, 0, sizeof(m_roll_list));

	bool ret = TASKPOOL->GetPaoHuanRollPool(m_roll_list, common_p->paohuan_getphase_reward_idx);
	if (!ret)
	{
		return;
	}

	m_is_rolling = true;

	static Protocol::SCPaoHuanRollPool phrp;
	for (int i = 0; i < PAOHUAN_ROLLPOOL_TOTAL_COUNT; ++ i)
	{
		phrp.roll_item_list[i].item_id = m_roll_list[i].item_cfg.item_id;
		phrp.roll_item_list[i].is_bind = (m_roll_list[i].item_cfg.is_bind == true ? 1 : 0);
		phrp.roll_item_list[i].num = m_roll_list[i].item_cfg.num;
	}
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&phrp, sizeof(phrp));

	this->SendPaoHuanRollInfo(Protocol::SCPaoHuanRollInfo::RIREASON_START);
}

void PaoHuanTask::OnGetPhaseRewardRollReq(short client_click_index)
{
	this->StartPaoHuanRoll();

	if (!m_is_rolling)
	{
		return;
	}

	CommonDataParam *common_p = m_role->GetCommonDataParam();

	int max_getphase_reward_idx = common_p->paohuan_complete_count / 10; 
	if (common_p->paohuan_getphase_reward_idx >= max_getphase_reward_idx)
	{
		return;
	}

	int rand_roll_index = this->GetRandRollItemIndex();
	if (rand_roll_index < 0 || rand_roll_index >= PAOHUAN_ROLLPOOL_TOTAL_COUNT)
	{
		return;
	}

	PaoHuanRollItem &result_item = m_roll_list[rand_roll_index];

	m_role->GetKnapsack()->PutOrMail(result_item.item_cfg, PUT_REASON_PAOHUAN_ROLL);

	this->SendPaoHuanRollInfo(Protocol::SCPaoHuanRollInfo::RIREASON_ROLL, rand_roll_index, client_click_index);
	
	++ common_p->paohuan_getphase_reward_idx;
	
	this->SendPaohuanTaskInfo(Protocol::PAOHUAN_NOTIFY_REASON_FETCH_REWARD);

	m_is_rolling = false;
}

void PaoHuanTask::CheckPaoHuanRollState()
{
	if (!m_is_rolling)
	{
		return;
	}

	CommonDataParam *common_p = m_role->GetCommonDataParam();

	int rand_roll_index = this->GetRandRollItemIndex();
	if (rand_roll_index < 0 || rand_roll_index >= PAOHUAN_ROLLPOOL_TOTAL_COUNT)
	{
		return;
	}

	// 发送邮件
	{
		static MailContentParam contentparam; contentparam.Reset();

		const ItemBase *item_base = ITEMPOOL->GetItem(m_roll_list[rand_roll_index].item_cfg.item_id);
		if (NULL == item_base)
		{
			return;
		}

		contentparam.item_list[0].item_id =m_roll_list[rand_roll_index].item_cfg.item_id;
		contentparam.item_list[0].num = m_roll_list[rand_roll_index].item_cfg.num;
		contentparam.item_list[0].is_bind = m_roll_list[rand_roll_index].item_cfg.is_bind ? 1 : 0;
		contentparam.item_list[0].invalid_time = item_base->CalInvalidTime();

		int length_1 = SNPRINTF(contentparam.subject, sizeof(contentparam.subject), gamestring::g_paohuan_task_phase_subject);
		int length_2 = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_paohuan_task_phase_content);
		if (length_1 > 0 && length_2 > 0)
		{
			MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
		}
	}

	++ common_p->paohuan_getphase_reward_idx;

	this->SendPaoHuanRollInfo(Protocol::SCPaoHuanRollInfo::RIREASON_END);

	this->SendPaohuanTaskInfo(Protocol::PAOHUAN_NOTIFY_REASON_FETCH_REWARD);

	m_is_rolling = false;
}

void PaoHuanTask::SendPaoHuanRollInfo(short reason, short hit_seq, short clint_click_index)
{
	CommonDataParam *common_p = m_role->GetCommonDataParam();

	static Protocol::SCPaoHuanRollInfo cmd;
	cmd.reason = reason;
	cmd.hit_seq = static_cast<char>(hit_seq);
	cmd.phase = static_cast<char>(common_p->paohuan_getphase_reward_idx);
	cmd.clint_click_index = clint_click_index;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&cmd, sizeof(cmd));
}

int PaoHuanTask::GetRandRollItemIndex()
{
	int total_weight = 0;
	for (int i = 0; i < PAOHUAN_ROLLPOOL_TOTAL_COUNT; ++ i)
	{
		total_weight += m_roll_list[i].weight;
	}

	if (total_weight <= 0)
	{
		return -1;
	}

	int rand_value = RandomNum(total_weight);

	for (int i = 0; i < PAOHUAN_ROLLPOOL_TOTAL_COUNT; ++ i)
	{
		if (rand_value <= m_roll_list[i].weight)
		{
			return i;
		}

		rand_value -= m_roll_list[i].weight;
	}

	return -1;
}

int PaoHuanTask::getDayCompleteCount()
{ 
	return  m_role->GetCommonDataParam()->paohuan_complete_count; 
}

