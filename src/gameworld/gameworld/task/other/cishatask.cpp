#include "cishatask.hpp"
#include "obj/character/role.h"
#include "task/taskpool.h"

static const int CISHA_TASK_LIMIT_TIME_S = 1200;										// 刺杀任务持续时间

CishaTask::CishaTask() : m_role(NULL), m_first(true), m_task_timeout_stamp(0)
{

}

CishaTask::~CishaTask()
{

}

void CishaTask::OnDayChange(int old_dayid, int now_dayid)
{
// 	if (old_dayid == now_dayid)
// 	{
// 		return;
// 	}
// 
// 	this->SendInfo();
}

void CishaTask::Update(unsigned int now_second)
{
// 	if (m_first)
// 	{
// 		m_first = false;
// 
// 		int task_id = 0;
// 		if (m_role->GetTaskManager()->GetTaskIdByType(TASK_TYPE_CISHA, &task_id))
// 		{
// 			const TaskInfo *task_phase = m_role->GetTaskManager()->GetTaskInfo(task_id);
// 			if (NULL != task_phase)
// 			{
// 				m_task_timeout_stamp = task_phase->accept_time + CISHA_TASK_LIMIT_TIME_S;
// 			}
// 		}
// 	}
// 
// 	if (0 != m_task_timeout_stamp && now_second >= m_task_timeout_stamp)
// 	{
// 		m_task_timeout_stamp = 0;
// 
// 		int task_id = 0;
// 		if (m_role->GetTaskManager()->GetTaskIdByType(TASK_TYPE_CISHA, &task_id))
// 		{
// 			m_role->GetTaskManager()->GiveupTask(task_id, true);
// 		}
// 	}
}

void CishaTask::AcceptTask()
{
// 	int camp_type = m_role->GetCamp();
// 	if (camp_type <= CAMP_TYPE_INVALID || camp_type >= CAMP_TYPE_MAX)
// 	{
// 		m_role->NoticeNum(errornum::EN_JOIN_CAMP_FIRST);
// 		return;
// 	}
// 
// 	int task_id = 0;
// 	if (m_role->GetTaskManager()->GetTaskIdByType(TASK_TYPE_CISHA, &task_id))
// 	{
// 		m_role->NoticeNum(errornum::EN_CISHA_TASK_ALREAD_OWN_CISHA_TASK);
// 		return;
// 	}
// 
// 	if (!ActivityShadow::Instance().IsActivtyOpen(ACTIVITY_TYPE_CISHA_TASK))
// 	{
// 		m_role->NoticeNum(errornum::EN_CISHA_TASK_ACTIVITY_OPEN_LIMIT);
// 		return;
// 	}
// 
// 	TaskID rand_task_id = LOGIC_CONFIG->GetCishaTaskCfg().RandOneTask(m_role->GetCamp());
// 	if (INVALID_TASK_ID != rand_task_id)
// 	{
// 		m_role->GetTaskManager()->AcceptTask(rand_task_id, true);
// 
// 		m_task_timeout_stamp = EngineAdapter::Instance().Time() + CISHA_TASK_LIMIT_TIME_S;
// 	}
}

void CishaTask::OnAddTask(TaskID task_id, bool is_init)
{
// 	if (!is_init)
// 	{
// 		const TaskCfg *task_cfg = TASKPOOL->GetCfgByTaskID(task_id);
// 		if (NULL != task_cfg && TASK_TYPE_CISHA == task_cfg->task_type)
// 		{
// 			this->SendInfo();
// 		}
// 	}
}

void CishaTask::OnRemoveTask(TaskID task_id, bool is_complete)
{
// 	const TaskCfg *task_cfg = TASKPOOL->GetCfgByTaskID(task_id);
// 	if (NULL != task_cfg && TASK_TYPE_CISHA == task_cfg->task_type)
// 	{
// 		if (is_complete)
// 		{
// 			m_role->GetDayCounter()->DayCountIncrease(DayCounter::DAYCOUNT_ID_COMMIT_CISHA_TASK_TIMES);
// 
// 			EventHandler::Instance().OnCompleteCampTask(m_role);
// 
// 			ROLE_LOG_QUICK6(LOG_TYPE_CISHA_TASK_COMPLETE, m_role, m_role->GetLevel(), task_id, NULL, NULL);
// 		}
// 
// 		this->SendInfo();
// 	}
}

bool CishaTask::CanAcceptTask(TaskID task_id, bool notify)
{
	return false;
// 	int camp_type = m_role->GetCamp();
// 	if (camp_type <= CAMP_TYPE_INVALID || camp_type >= CAMP_TYPE_MAX)
// 	{
// 		if (notify) m_role->NoticeNum(errornum::EN_JOIN_CAMP_FIRST);
// 		return false;
// 	}
// 
// 	int tmp_task_id = 0;
// 	if (m_role->GetTaskManager()->GetTaskIdByType(TASK_TYPE_CISHA, &tmp_task_id))
// 	{
// 		if (notify) m_role->NoticeNum(errornum::EN_CISHA_TASK_ALREAD_OWN_CISHA_TASK);
// 		return false;
// 	}
// 
// 	JoinLimitUnit limit_unit;
// 	bool is_limit = LOGIC_CONFIG->GetJoinLimitConfig().GetOtherJoinLimit(JLMT_OTHER_CISHA_TASK, &limit_unit);
// 
// 	if (is_limit && m_role->GetLevel() < limit_unit.level)
// 	{
// 		if (notify) m_role->NoticeNum(errornum::EN_ROLE_LEVEL_NOT_ENOUGH);
// 		return false;
// 	}
// 
// 	int commit_times = m_role->GetDayCounter()->GetDayCount(DayCounter::DAYCOUNT_ID_COMMIT_CISHA_TASK_TIMES);
// 	if (commit_times >= LOGIC_CONFIG->GetCishaTaskCfg().CanCommitTimes())
// 	{
// 		if (notify) m_role->NoticeNum(errornum::EN_CISHA_TASK_ACCEPT_TIMES_LIMIT);
// 		return false;
// 	}
// 
// 	return true;
}

bool CishaTask::DoReward(const TaskCfg *task_cfg)
{
	return false;
// 	const CishaTaskRewardConfig *reward_cfg = LOGIC_CONFIG->GetCishaTaskCfg().GetRewardCfg(m_role->GetLevel());
// 	if (NULL != reward_cfg)
// 	{
// 		m_role->AddExp(reward_cfg->reward_exp, "CishaTaskReward");
// 
// 		{
// 			static MailContentParam contentparam; contentparam.Reset();
// 
// 			const ItemConfigData &reward_item = LOGIC_CONFIG->GetCishaTaskCfg().GetRewardItem();
// 			contentparam.item_list[0].item_id = reward_item.item_id;
// 			contentparam.item_list[0].num = reward_item.num;
// 			contentparam.item_list[0].is_bind = reward_item.is_bind;
// 
// 			int length1 = SNPRINTF(contentparam.subject, sizeof(contentparam.subject), gamestring::g_cisha_task_complete_reward_subject);
// 			int length2 = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_cisha_task_complete_reward_content);
// 
// 			if (length1 > 0 && length2 > 0)
// 			{
// 				MailRoute::MailToUser(m_role->GetUserId() , SYSTEM_MAIL_REASON_INVALID, contentparam);
// 			}
// 		}
// 	}
// 	else
// 	{
// 		// TODO:这种地方要打log
// 	}
// 
// 	return true;
}

void CishaTask::SendInfo()
{
	/*
	Protocol::SCCishaTaskInfo csti;
	csti.commit_times = m_role->GetDayCounter()->GetDayCount(DayCounter::DAYCOUNT_ID_COMMIT_CISHA_TASK_TIMES);

	int task_id = 0;
	if (m_role->GetTaskManager()->GetTaskIdByType(TASK_TYPE_CISHA, &task_id))
	{
		csti.task_id = static_cast<TaskID>(task_id);
	}
	else
	{
		csti.task_id = INVALID_TASK_ID;
	}

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&csti, sizeof(csti));
	*/
}

