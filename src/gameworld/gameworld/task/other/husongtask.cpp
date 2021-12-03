#include "husongtask.hpp"
#include "obj/character/role.h"
#include "task/taskpool.h"
#include "scene/activityshadow/activityshadow.hpp"
#include "gamelog.h"
#include "config/logicconfigmanager.hpp"
#include "other/vip/vipconfig.hpp"
#include "config/joinlimitconfig.hpp"
#include "config/taskconfig/husongconfig.hpp"
#include "scene/scene.h"
#include "servercommon/errornum.h"
#include "servercommon/noticenum.h"
#include "servercommon/string/gameworldstr.h"
#include "world.h"
#include "other/route/mailroute.hpp"
#include "other/event/eventhandler.hpp"
#include "effect/skilleffect/effecthurtdec.hpp"
#include "effect/skilleffect/effectmovespeed.hpp"
#include "item/knapsack.h"
#include "task/newtaskmanager.h"
#include "task/taskrecorder.h"
#include "other/vip/vip.hpp"
#include "other/daycounter/daycounter.hpp"
#include "other/sublock/sublock.hpp"
#include "other/mount/mountmanager.hpp"
#include "other/fightmount/fightmountmanager.hpp"
#include "other/roleactivity/roleactivity.hpp"
#include "other/multimount/multimount.hpp"
#include "item/itempool.h"
#include "other/itemlevelreward/itemlevelreward.hpp"

HusongTask::HusongTask() 
	: m_role(NULL), m_first(true), m_task_timeout_stamp(0)
{

}

HusongTask::~HusongTask()
{

}

void HusongTask::Init(Role *role, const HusongTaskParam &p)
{
	m_role = role; 

	m_husong_info = p;
}

void HusongTask::GetInitParam(HusongTaskParam *p)
{
	*p = m_husong_info;
}

bool HusongTask::CanAcceptHusongTask(bool notify)
{
	JoinLimitUnit limit_unit;
	bool is_limit = LOGIC_CONFIG->GetJoinLimitConfig().GetOtherJoinLimit(JLMT_OTHER_HUSONG_TASK, &limit_unit);

	if (is_limit && m_role->GetLevel() < limit_unit.level)
	{
		if (notify) m_role->NoticeNum(errornum::EN_TASK_ACCEPT_LEVEL);
		return false;
	}

	if (Role::MOVE_MODE_FLY == m_role->GetMoveMode())
	{
		m_role->NoticeNum(errornum::EN_HUSONG_TASK_ACCEPT_FLY_LIMIT);
		return false;
	}

	if (!m_role->GetTaskRecorder()->IsFinished(LOGIC_CONFIG->GetHuSongConfig().GetPreTaskID()))
	{
		if (notify) m_role->NoticeNum(errornum::EN_TASK_ACCEPT_PRE_TASK);
		return false;
	}

	if (0 != m_role->GetScene()->GetSceneKey())
	{
		if (notify) m_role->NoticeNum(errornum::EN_HUSONG_TASK_SCENE_KEY_LIMIT);
		return false;
	}

	int task_id = 0;
	if (m_role->GetTaskManager()->GetTaskIdByType(TASK_TYPE_HUSONG, &task_id))
	{
		return false;
	}

	int max_husong_times = HUSONG_ONEDAY_ACCEPT_MAX + m_role->GetDayCounter()->GetDayCount(DayCounter::DAYCOUNT_ID_HUSONG_TASK_VIP_BUY_COUNT);

	if (!m_role->GetDayCounter()->DayCountCheck(DayCounter::DAYCOUNT_ID_ACCEPT_HUSONG_TASK_COUNT, max_husong_times))
	{
		if (notify)
		{
			int buy_count = m_role->GetDayCounter()->GetDayCount(DayCounter::DAYCOUNT_ID_HUSONG_TASK_VIP_BUY_COUNT);
			int can_buy_count = LOGIC_CONFIG->GetVipConfig().GetAuthParam(m_role->GetVip()->GetVipLevel(), VAT_BUY_HUSONG_TIMES);

			if (buy_count >= can_buy_count)
			{
				m_role->NoticeNum(errornum::EN_HUSONG_TASK_ACCEPT_NO_COUNT);
			}
			else
			{
				m_role->NoticeNum(errornum::EN_HUSONG_TASK_CAN_BUY_LIMIT);
			}
		}

		return false;
	}

	return true;
}

bool HusongTask::LittleHelperCanAcceptHusongTask(bool notify)
{
	JoinLimitUnit limit_unit;
	bool is_limit = LOGIC_CONFIG->GetJoinLimitConfig().GetOtherJoinLimit(JLMT_OTHER_HUSONG_TASK, &limit_unit);

	if (is_limit && m_role->GetLevel() < limit_unit.level)
	{
		if (notify) m_role->NoticeNum(errornum::EN_TASK_ACCEPT_LEVEL);
		return false;
	}

	if (Role::MOVE_MODE_FLY == m_role->GetMoveMode())
	{
		m_role->NoticeNum(errornum::EN_HUSONG_TASK_ACCEPT_FLY_LIMIT);
		return false;
	}

	if (!m_role->GetTaskRecorder()->IsFinished(LOGIC_CONFIG->GetHuSongConfig().GetPreTaskID()))
	{
		if (notify) m_role->NoticeNum(errornum::EN_TASK_ACCEPT_PRE_TASK);
		return false;
	}

	/*if (0 != m_role->GetScene()->GetSceneKey())
	{
		if (notify) m_role->NoticeNum(errornum::EN_HUSONG_TASK_SCENE_KEY_LIMIT);
		return false;
	}*/

	int task_id = 0;
	if (m_role->GetTaskManager()->GetTaskIdByType(TASK_TYPE_HUSONG, &task_id))
	{
		return false;
	}

	int max_husong_times = HUSONG_ONEDAY_ACCEPT_MAX + m_role->GetDayCounter()->GetDayCount(DayCounter::DAYCOUNT_ID_HUSONG_TASK_VIP_BUY_COUNT);

	if (!m_role->GetDayCounter()->DayCountCheck(DayCounter::DAYCOUNT_ID_ACCEPT_HUSONG_TASK_COUNT, max_husong_times))
	{
		if (notify)
		{
			int buy_count = m_role->GetDayCounter()->GetDayCount(DayCounter::DAYCOUNT_ID_HUSONG_TASK_VIP_BUY_COUNT);
			int can_buy_count = LOGIC_CONFIG->GetVipConfig().GetAuthParam(m_role->GetVip()->GetVipLevel(), VAT_BUY_HUSONG_TIMES);

			if (buy_count >= can_buy_count)
			{
				m_role->NoticeNum(errornum::EN_HUSONG_TASK_ACCEPT_NO_COUNT);
			}
			else
			{
				m_role->NoticeNum(errornum::EN_HUSONG_TASK_CAN_BUY_LIMIT);
			}
		}

		return false;
	}

	return true;
}

void HusongTask::GetHusongInfo(TaskID *task_id, char *color)
{
	if (NULL == task_id || NULL == color) return;

	*task_id = m_husong_info.task_id;
	*color = static_cast<char>(m_husong_info.task_color);
}

void HusongTask::OnAddTask(TaskID task_id, bool is_init)
{
	if (is_init) return;

	const TaskCfg *task = TASKPOOL->GetCfgByTaskID(task_id);
	if (NULL == task)
	{
		return;
	}

	if (TASK_TYPE_HUSONG != task->task_type)
	{
		return;
	}

	int refresh_count = m_husong_info.refresh_count;

	m_husong_info.refresh_count = 0;
	m_husong_info.task_id = task_id; 
	if (m_husong_info.task_color <= TASK_COLOR_INVALID || m_husong_info.task_color >= TASK_COLOR_MAX)
	{
		m_husong_info.task_color = TASK_COLOR_WHITE;
	}

	m_role->GetDayCounter()->DayCountIncrease(DayCounter::DAYCOUNT_ID_ACCEPT_HUSONG_TASK_COUNT);

	if (ActivityShadow::Instance().IsActivtyOpen(ACTIVITY_TYPE_HUSONG))
	{
		m_husong_info.accpet_activitytime = ActivityShadow::Instance().GetActivityBeginTime(ACTIVITY_TYPE_HUSONG);
		//m_role->SetAttackMode(Role::ATTACK_MODE_GUILD, true, false);
	}
	else
	{
		m_husong_info.accpet_activitytime = 0;
	}

	m_task_timeout_stamp = EngineAdapter::Instance().Time() + HUSONG_TASK_LIMIT_TIME_S;
	m_role->GetMountManager()->GoonMount(false, false);
	m_role->GetFightMountManager()->GoonFightMount(false, false);
	m_role->GetMultiMount()->MountOn(false);
	this->CheckBuffStatus(true);

	EventHandler::Instance().OnAcceptHusongTask(m_role);

	this->SendHusongTaskInfo(Protocol::HUSONG_INFO_NOTFIY_REASON_ADD_TASK, true);

	if (TASK_COLOR_ORANGE == m_husong_info.task_color)
	{
		int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_husong_accept_notice, UidToInt(m_role->GetUserId()), m_role->GetName(), m_role->GetCamp());
		if (length > 0)
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_3);
		}
	}

	{
		// 日志
		gamelog::g_log_husong_task.printf(LL_INFO, "HusongTask::Accept user[%d, %s], level[%d], task_id[%d], color[%d], refresh_count[%d]",
			m_role->GetUID(), m_role->GetName(), m_role->GetLevel(), task_id, m_husong_info.task_color, refresh_count);

		ROLE_LOG_QUICK6(LOG_TYPE_HUSONG_TASK_ACCEPT, m_role, m_role->GetLevel(), m_husong_info.task_color, NULL, NULL);
	}
}

void HusongTask::OnLittleHelperAddTask(TaskID task_id, bool is_init)
{
	if (is_init) return;

	const TaskCfg *task = TASKPOOL->GetCfgByTaskID(task_id);
	if (NULL == task)
	{
		return;
	}

	if (TASK_TYPE_HUSONG != task->task_type)
	{
		return;
	}

	int refresh_count = m_husong_info.refresh_count;

	m_husong_info.refresh_count = 0;
	m_husong_info.task_id = task_id;
	if (m_husong_info.task_color <= TASK_COLOR_INVALID || m_husong_info.task_color >= TASK_COLOR_MAX)
	{
		m_husong_info.task_color = TASK_COLOR_WHITE;
	}

	m_role->GetDayCounter()->DayCountIncrease(DayCounter::DAYCOUNT_ID_ACCEPT_HUSONG_TASK_COUNT);

	if (ActivityShadow::Instance().IsActivtyOpen(ACTIVITY_TYPE_HUSONG))
	{
		m_husong_info.accpet_activitytime = ActivityShadow::Instance().GetActivityBeginTime(ACTIVITY_TYPE_HUSONG);
		//m_role->SetAttackMode(Role::ATTACK_MODE_GUILD, true, false);
	}
	else
	{
		m_husong_info.accpet_activitytime = 0;
	}

	m_task_timeout_stamp = EngineAdapter::Instance().Time() + HUSONG_TASK_LIMIT_TIME_S;
	m_role->GetMountManager()->GoonMount(false, false);
	m_role->GetFightMountManager()->GoonFightMount(false, false);
	m_role->GetMultiMount()->MountOn(false);
	this->CheckBuffStatus(true);

	EventHandler::Instance().OnAcceptHusongTask(m_role);

	this->SendHusongTaskInfo(Protocol::HUSONG_INFO_NOTFIY_REASON_LITTLE_HELPER_ADD_TASK, true);

	if (TASK_COLOR_ORANGE == m_husong_info.task_color)
	{
		int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_husong_accept_notice, UidToInt(m_role->GetUserId()), m_role->GetName(), m_role->GetCamp());
		if (length > 0)
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_3);
		}
	}

	{
		// 日志
		gamelog::g_log_husong_task.printf(LL_INFO, "HusongTask::Accept user[%d, %s], level[%d], task_id[%d], color[%d], refresh_count[%d]",
			m_role->GetUID(), m_role->GetName(), m_role->GetLevel(), task_id, m_husong_info.task_color, refresh_count);

		ROLE_LOG_QUICK6(LOG_TYPE_HUSONG_TASK_ACCEPT, m_role, m_role->GetLevel(), m_husong_info.task_color, NULL, NULL);
	}
}

void HusongTask::OnRemoveTask(TaskID task_id, bool is_complete)
{
	const TaskCfg *task = TASKPOOL->GetCfgByTaskID(task_id);
	if (NULL == task)
	{
		return;
	}

	if (TASK_TYPE_HUSONG != task->task_type)
	{
		return;
	}

	if (is_complete) 
	{
		long long base_commit_exp = 0; int base_commit_bind_coin = 0;
		ItemConfigData reward_item[MAX_ATTACHMENT_ITEM_NUM];
		this->CalcBaseReward(&base_commit_exp, &base_commit_bind_coin, reward_item);

		//m_role->AddExp(base_commit_exp, "HusongTaskReward", Role::EXP_ADD_REASON_HUSONG);

		//策划需求，修改增加经验规则（2018-1-3）
		double factor = LOGIC_CONFIG->GetHuSongConfig().GetExpFactor();
		double color_factor = (double)LOGIC_CONFIG->GetHuSongConfig().GetTaskRewardFactor(m_husong_info.task_color)/100;
		long long reward_exp = static_cast<long long>(factor * (LOGIC_CONFIG->GetItemLevelRewardConfig().GetExpDanReward(m_role->GetLevel(), 0)) * color_factor);
		m_role->AddExp(reward_exp, "HusongTaskReward", Role::EXP_ADD_REASON_HUSONG);

		m_role->GetKnapsack()->GetMoney()->AddCoinBind(base_commit_bind_coin, "HusongTaskReward");
		m_role->GetKnapsack()->PutListOrMail(reward_item, PUT_REASON_HUSONG_TASK);

		EventHandler::Instance().OnCompleteHusongTask(m_role, m_husong_info.task_color);
	}

	m_role->GetMultiMount()->MountOn(true);

	m_husong_info.Clear();
	this->CheckBuffStatus(false);

	m_task_timeout_stamp = 0;
	
	int notify_reason = is_complete ? Protocol::HUSONG_INFO_NOTIFY_REASON_COMMIT_TASK : Protocol::HUSONG_INFO_NOTFIY_REASON_GIVEUP_TASK;
	
	this->SendHusongTaskInfo(notify_reason, true);

	{
		// 日志
		gamelog::g_log_husong_task.printf(LL_INFO, "HusongTask::Remove user[%d, %s], level[%d], task_id[%d], color[%d], complete[%d]",
			m_role->GetUID(), m_role->GetName(), m_role->GetLevel(), task_id, m_husong_info.task_color, (is_complete ? 1 : 0));
	}
}

void HusongTask::OnLittleHelperRemoveTask(TaskID task_id, bool is_complete)
{
	const TaskCfg *task = TASKPOOL->GetCfgByTaskID(task_id);
	if (NULL == task)
	{
		return;
	}

	if (TASK_TYPE_HUSONG != task->task_type)
	{
		return;
	}

	if (is_complete)
	{
		long long base_commit_exp = 0; int base_commit_bind_coin = 0;
		ItemConfigData reward_item[MAX_ATTACHMENT_ITEM_NUM];
		this->CalcBaseReward(&base_commit_exp, &base_commit_bind_coin, reward_item);

		//m_role->AddExp(base_commit_exp, "HusongTaskReward", Role::EXP_ADD_REASON_HUSONG);

		//策划需求，修改增加经验规则（2018-1-3）
		double factor = LOGIC_CONFIG->GetHuSongConfig().GetExpFactor();
		double color_factor = (double)LOGIC_CONFIG->GetHuSongConfig().GetTaskRewardFactor(m_husong_info.task_color) / 100;
		long long reward_exp = static_cast<long long>(factor * (LOGIC_CONFIG->GetItemLevelRewardConfig().GetExpDanReward(m_role->GetLevel(), 0)) * color_factor);
		m_role->AddExp(reward_exp, "HusongTaskReward", Role::EXP_ADD_REASON_HUSONG);

		m_role->GetKnapsack()->GetMoney()->AddCoinBind(base_commit_bind_coin, "HusongTaskReward");
		m_role->GetKnapsack()->PutListOrMail(reward_item, PUT_REASON_HUSONG_TASK);

		EventHandler::Instance().OnCompleteHusongTask(m_role, m_husong_info.task_color);
	}

	m_role->GetMultiMount()->MountOn(true);

	m_husong_info.Clear();
	this->CheckBuffStatus(false);

	m_task_timeout_stamp = 0;

	int notify_reason = is_complete ? Protocol::HUSONG_INFO_NOTFIY_REASON_LITTLE_HELPER_COMMIT_TASK : Protocol::HUSONG_INFO_NOTFIY_REASON_GIVEUP_TASK;

	this->SendHusongTaskInfo(notify_reason, true);

	{
		// 日志
		gamelog::g_log_husong_task.printf(LL_INFO, "HusongTask::Remove user[%d, %s], level[%d], task_id[%d], color[%d], complete[%d]",
			m_role->GetUID(), m_role->GetName(), m_role->GetLevel(), task_id, m_husong_info.task_color, (is_complete ? 1 : 0));
	}
}

void HusongTask::OnDie(ObjID killer_objid)
{
	if (!this->IsHusongStatus()) return;
	if (NULL == m_role->GetScene()) return;
	if (m_husong_info.Invalid()) return;

	Role *rob_role = m_role->GetScene()->GetPrincipal(killer_objid);
	if (NULL != rob_role)
	{
		long long useless_base_commit_exp = 0; int base_commit_bind_coin = 0;
		ItemConfigData reward_item[MAX_ATTACHMENT_ITEM_NUM];
		int item_count = this->CalcBaseReward(&useless_base_commit_exp, &base_commit_bind_coin, reward_item);

		static MailContentParam contentparam; contentparam.Reset();

		{	// 被劫者奖励
			//long long reward_exp = m_role->GetAddonExp(static_cast<long long>(base_commit_exp / 2), Role::EXP_ADD_REASON_HUSONG);

			//策划需求，修改增加经验规则（2018-1-3）
			double factor = LOGIC_CONFIG->GetHuSongConfig().GetExpFactor();
			double color_factor = (double)LOGIC_CONFIG->GetHuSongConfig().GetTaskRewardFactor(m_husong_info.task_color) / 100;
			double be_rob_exp_factor = (double)LOGIC_CONFIG->GetHuSongConfig().GetBeRobExpFactor()/100;
			long long base_commit_exp = static_cast<long long>(factor * (LOGIC_CONFIG->GetItemLevelRewardConfig().GetExpDanReward(m_role->GetLevel(), 0)) * color_factor * be_rob_exp_factor);
			int add_percent = 0;
			long long reward_exp = m_role->GetAddonExp(base_commit_exp, Role::EXP_ADD_REASON_HUSONG, &add_percent);

			contentparam.Reset();
			contentparam.AddVirtualItem(0, MAIL_VIRTUAL_ITEM_EXP, reward_exp);
			contentparam.AddVirtualItem(1, MAIL_VIRTUAL_ITEM_BIND_COIN, base_commit_bind_coin / 2);

			for(int i = 0; i < item_count && i < MAX_ATTACHMENT_ITEM_NUM; i++)
			{
				if ((reward_item[i].num / 2) > 0 && NULL != ITEMPOOL->GetItem(reward_item[i].item_id))
				{
					contentparam.AddItem(reward_item[i].item_id, reward_item[i].num / 2, reward_item[i].is_bind);
				}
			}

			int length1 = SNPRINTF(contentparam.subject, sizeof(contentparam.subject), gamestring::g_husong_berob_mail_subject);
			int length2 = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_husong_berob_mail_content, 
				UidToInt(rob_role->GetUserId()), rob_role->GetName(), rob_role->GetCamp());
			if (length1 > 0 && length2 > 0)
			{
				MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
			}
		}

		// 劫镖者奖励
		int LEVEL_DIFF = LOGIC_CONFIG->GetHuSongConfig().GetRobNoRewardLevelLimit();  // 等级差
		int reward_level = rob_role->GetLevel() - LEVEL_DIFF;

		if (m_role->GetLevel() >= reward_level
			&& rob_role->GetDayCounter()->DayCountCheck(DayCounter::DAYCOUNT_ID_HUSONG_ROB_COUNT, LOGIC_CONFIG->GetHuSongConfig().GetMaxRobCount()))
		{
			rob_role->GetDayCounter()->DayCountIncrease(DayCounter::DAYCOUNT_ID_HUSONG_ROB_COUNT);

			//long long reward_exp = m_role->GetAddonExp(static_cast<long long>(base_commit_exp * 0.5f), Role::EXP_ADD_REASON_HUSONG);

			//策划需求，修改增加经验规则（2018-1-3）
			double factor = LOGIC_CONFIG->GetHuSongConfig().GetExpFactor();
			double color_factor = (double)LOGIC_CONFIG->GetHuSongConfig().GetTaskRewardFactor(m_husong_info.task_color) / 100;
			double rob_exp_factor = (double)LOGIC_CONFIG->GetHuSongConfig().GetRobExpFactor() / 100;
			long long base_commit_exp = static_cast<long long>(factor * (LOGIC_CONFIG->GetItemLevelRewardConfig().GetExpDanReward(m_role->GetLevel(), 0)) * color_factor * rob_exp_factor);
			int add_percent = 0;
			long long reward_exp = m_role->GetAddonExp(base_commit_exp, Role::EXP_ADD_REASON_HUSONG, &add_percent);

			contentparam.Reset();
			contentparam.AddVirtualItem(0, MAIL_VIRTUAL_ITEM_EXP, reward_exp);
			contentparam.AddVirtualItem(1, MAIL_VIRTUAL_ITEM_BIND_COIN, (int)(base_commit_bind_coin * 0.5f));

			for(int i = 0; i < item_count && i < MAX_ATTACHMENT_ITEM_NUM; i++)
			{
				if (static_cast<int>(reward_item[i].num * 0.5f) > 0 && NULL != ITEMPOOL->GetItem(reward_item[i].item_id))
				{
					contentparam.AddItem(reward_item[i].item_id, static_cast<int>(reward_item[i].num * 0.5f), reward_item[i].is_bind);
				}
			}

			int length1 = SNPRINTF(contentparam.subject, sizeof(contentparam.subject), gamestring::g_husong_rob_mail_subject);
			int length2 = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_husong_rob_mail_content, 
				UidToInt(m_role->GetUserId()), m_role->GetName(), m_role->GetCamp());
			if (length1 > 0 && length2 > 0)
			{
				MailRoute::MailToUser(rob_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
			}

			// 发传闻
			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_husong_rob_notice, 
				UidToInt(rob_role->GetUserId()), rob_role->GetName(), rob_role->GetCamp(), UidToInt(m_role->GetUserId()), m_role->GetName(), m_role->GetCamp());
			if (length > 0)
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_ACTIVITY_SPECIAL);
			}

			{
				// 日志
				gamelog::g_log_husong_task.printf(LL_INFO, "HusongTask::Rob user[%d, %s], level[%d], robber[%d, %s], robber_level[%d], color[%d]",
					m_role->GetUID(), m_role->GetName(), m_role->GetLevel(), 
					rob_role->GetUID(), rob_role->GetName(), rob_role->GetLevel(), m_husong_info.task_color);

				ROLE_LOG_QUICK6(LOG_TYPE_HUSONG_TASK_ROB, rob_role, rob_role->GetLevel(), m_husong_info.task_color, NULL, NULL);
				ROLE_LOG_QUICK6(LOG_TYPE_HUSONG_TASK_BEROB, m_role, m_role->GetLevel(), m_husong_info.task_color, NULL, NULL);
			}
		}
		else
		{
			if (m_role->GetLevel() < reward_level)
			{
				// 打劫者等级太高，没有奖励，通知邮件
				contentparam.Reset();
				int length1 = SNPRINTF(contentparam.subject, sizeof(contentparam.subject), gamestring::g_husong_rob_mail_subject);
				int length2 = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_husong_rob_mail_level_limit_content,
					LEVEL_DIFF);
				if (length1 > 0 && length2 > 0)
				{
					MailRoute::MailToUser(rob_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
				}
			}
			else if (!rob_role->GetDayCounter()->DayCountCheck(DayCounter::DAYCOUNT_ID_HUSONG_ROB_COUNT, LOGIC_CONFIG->GetHuSongConfig().GetMaxRobCount()))
			{
				// 打劫者打劫次数已满，没有奖励，通知邮件
				contentparam.Reset();
				int length1 = SNPRINTF(contentparam.subject, sizeof(contentparam.subject), gamestring::g_husong_rob_mail_subject);
				int length2 = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_husong_rob_mail_times_limit_content,
					LOGIC_CONFIG->GetHuSongConfig().GetMaxRobCount());
				if (length1 > 0 && length2 > 0)
				{
					MailRoute::MailToUser(rob_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
				}
			}
		}

		m_role->GetTaskManager()->GiveupTask(m_husong_info.task_id, true);

		// 实际上当完成任务
		ROLE_LOG_QUICK6(LOG_TYPE_COMMIT_TASK, m_role, TASK_TYPE_HUSONG, m_husong_info.task_id, NULL, NULL);

		EventHandler::Instance().OnRobHusong(rob_role);
	}
}

bool HusongTask::SetTaskColor(char color)
{
	if (color <= TASK_COLOR_INVALID || color >= TASK_COLOR_MAX)
	{
		return false;
	}
	m_husong_info.task_color = color;
	this->SendHusongTaskInfo();
	return true;
}

void HusongTask::Update(unsigned int now_second)
{
	if (m_first)
	{
		m_first = false;

		int task_id = 0;
		if (m_role->GetTaskManager()->GetTaskIdByType(TASK_TYPE_HUSONG, &task_id))
		{
			m_husong_info.task_id = task_id;

			const TaskInfo *task_phase = m_role->GetTaskManager()->GetTaskInfo(m_husong_info.task_id);
			if (NULL != task_phase)
			{
				m_task_timeout_stamp = task_phase->accept_time + HUSONG_TASK_LIMIT_TIME_S;
			}
		}
		else
		{
			m_husong_info.task_id = INVALID_TASK_ID;
			this->CheckBuffStatus(false);
		}

		// this->SendHusongTaskInfo(Protocol::HUSONG_INFO_NOTFIY_REASON_DEFAULT, true);
	}

	if (0 != m_task_timeout_stamp && now_second >= m_task_timeout_stamp)
	{
		long long useless_base_commit_exp = 0; int base_commit_bind_coin = 0;
		ItemConfigData reward_item[MAX_ATTACHMENT_ITEM_NUM];
		int item_count = this->CalcBaseReward(&useless_base_commit_exp, &base_commit_bind_coin, reward_item);

		// 缓存护送的数据
		TaskID cache_task_id = m_husong_info.task_id;
		char cache_task_color = m_husong_info.task_color;

		if (m_role->GetTaskManager()->GiveupTask(m_husong_info.task_id, true))
		{
			static MailContentParam contentparam; contentparam.Reset();

			{	// 超时奖励一半
				//long long reward_exp = m_role->GetAddonExp(static_cast<long long>(base_commit_exp / 2), Role::EXP_ADD_REASON_DEFAULT);

				//策划需求，修改增加经验规则（2018-1-3）
				double factor = LOGIC_CONFIG->GetHuSongConfig().GetExpFactor();
				double color_factor = (double)LOGIC_CONFIG->GetHuSongConfig().GetTaskRewardFactor(cache_task_color) / 100;
				long long base_commit_exp = static_cast<long long>(factor * (LOGIC_CONFIG->GetItemLevelRewardConfig().GetExpDanReward(m_role->GetLevel(), 0)) * color_factor);
				int add_percent = 0;
				long long reward_exp = m_role->GetAddonExp(static_cast<long long>(base_commit_exp / 2), Role::EXP_ADD_REASON_DEFAULT, &add_percent);

				contentparam.AddVirtualItem(0, MAIL_VIRTUAL_ITEM_EXP, reward_exp);
				contentparam.AddVirtualItem(1, MAIL_VIRTUAL_ITEM_BIND_COIN, base_commit_bind_coin / 2);

				for(int i = 0; i < item_count && i < MAX_ATTACHMENT_ITEM_NUM; i++)
				{
					if ((reward_item[i].num / 2) > 0 && NULL != ITEMPOOL->GetItem(reward_item[i].item_id))
					{
						contentparam.AddItem(reward_item[i].item_id, reward_item[i].num / 2, reward_item[i].is_bind);
					}
				}
					
				int length1 = SNPRINTF(contentparam.subject, sizeof(contentparam.subject), gamestring::g_husong_overtime_mail_subject);
				int length2 = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_husong_overtime_mail_content);
				if (length1 > 0 && length2 > 0)
				{
					MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
				}
			}

			// 实际上当完成任务
			ROLE_LOG_QUICK6(LOG_TYPE_COMMIT_TASK, m_role, TASK_TYPE_HUSONG, cache_task_id, NULL, NULL);
		}
	}
}

void HusongTask::OnRefreshTaskColor(Protocol::CSRefreshHusongTask* rhtt)
{
	if (!this->CanAcceptHusongTask(true))
	{
		return;
	}

	if (m_role->GetSublock()->CheckHasLock()) return;

	if (m_role->GetKnapsack()->IsLock())													
	{	
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	if (TASK_COLOR_ORANGE == m_husong_info.task_color)
	{
		m_role->NoticeNum(errornum::EN_HUSONG_TASK_COLOR_IS_RED);
		return;
	}
	
	if (m_husong_info.task_color <= TASK_COLOR_INVALID || m_husong_info.task_color >= TASK_COLOR_MAX) m_husong_info.task_color = TASK_COLOR_WHITE;

	if (0 != rhtt->is_autoflush && m_husong_info.task_color >= rhtt->to_color) // 若一键刷新 但目标颜色不合法
	{
		m_role->NoticeNum(errornum::EN_HUSONG_TASK_COLOR_TYPE_SATISFY);
		return;
	}

	if (m_role->GetTaskManager()->HasTask(m_husong_info.task_id)) return;

	int task_accepttimes = m_role->GetDayCounter()->GetDayCount(DayCounter::DAYCOUNT_ID_ACCEPT_HUSONG_TASK_COUNT);
	int can_accept_times = HUSONG_ONEDAY_ACCEPT_MAX + m_role->GetDayCounter()->GetDayCount(DayCounter::DAYCOUNT_ID_HUSONG_TASK_VIP_BUY_COUNT);

	if (task_accepttimes >= can_accept_times)
	{
		int buy_count = m_role->GetDayCounter()->GetDayCount(DayCounter::DAYCOUNT_ID_HUSONG_TASK_VIP_BUY_COUNT);
		int can_buy_count = LOGIC_CONFIG->GetVipConfig().GetAuthParam(m_role->GetVip()->GetVipLevel(), VAT_BUY_HUSONG_TIMES);

		if (buy_count >= can_buy_count)
		{
			m_role->NoticeNum(errornum::EN_HUSONG_TASK_ACCEPT_NO_COUNT);
		}
		else
		{
			m_role->NoticeNum(errornum::EN_HUSONG_TASK_CAN_BUY_LIMIT);
		}

		return;
	}

	int old_color = m_husong_info.task_color, old_fresh_count = m_husong_info.refresh_count;

	// 计算
	int free_times = 0, stuff_times = 0, color_ret = m_husong_info.task_color;
	{
		if (0 == rhtt->is_autoflush)
		{
			this->RefreshColorHelper(0 != rhtt->is_auto_buy ,color_ret, free_times, stuff_times);
		}
		else
		{
			int max_refresh_times = LOGIC_CONFIG->GetHuSongConfig().GetRefreshToColorMaxTimes(rhtt->to_color);
			int try_count = 0;
			while(try_count < max_refresh_times && color_ret < rhtt->to_color)
			{
				if (!this->RefreshColorHelper(0 != rhtt->is_auto_buy ,color_ret, free_times, stuff_times)) break;
				++ try_count;
			}

			if (try_count >= max_refresh_times && color_ret < rhtt->to_color)
			{
				color_ret = rhtt->to_color;
			}
		}
	}

	if (free_times + stuff_times <= 0) // 什么都不能用的时候，说明真的没办法消耗了
	{
		if (0 != rhtt->is_auto_buy)
		{
			m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		}
		else
		{
			m_role->NoticeItemLack(LOGIC_CONFIG->GetHuSongConfig().GetFlushItemId(), 1);
		}

		return;
	}

	Int64 final_use_bind_gold = 0, final_use_gold = 0;

	ItemExtern::ItemConsumeList consume_list; consume_list.Reset();
	{
		// 消耗
		if (free_times > 0)
		{
			int set_count = m_role->GetDayCounter()->GetDayCount(DayCounter::DAYCOUNT_ID_HUSONG_REFRESH_COLOR_FREE_TIMES) + free_times;
			m_role->GetDayCounter()->DayCountSet(DayCounter::DAYCOUNT_ID_HUSONG_REFRESH_COLOR_FREE_TIMES, set_count);
		}

		if (stuff_times > 0)
		{
			ItemExtern::ItemConsumeConfig stuff(LOGIC_CONFIG->GetHuSongConfig().GetFlushItemId(), 0 != rhtt->is_auto_buy, stuff_times);

			if (!ItemExtern::GetConsumeListByOrder(m_role, false, 1, &stuff, &consume_list))
			{
				m_role->NoticeNum(errornum::EN_FUNC_STUFF_NOT_ENOUGH);
				return;
			}

			if (!m_role->GetKnapsack()->GetMoney()->AllGoldMoreThan(consume_list.need_gold_num))
			{
				m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
				return;
			}

			// 先算出最后会消耗的元宝类型和数量
			if (m_role->GetKnapsack()->GetMoney()->GoldBindMoreThan(consume_list.need_gold_num))
			{
				final_use_bind_gold = consume_list.need_gold_num;
			}
			else
			{
				final_use_bind_gold = m_role->GetKnapsack()->GetMoney()->GetGoldBind();
				final_use_gold = consume_list.need_gold_num - final_use_bind_gold;
			}

			if (consume_list.need_gold_num > 0)
			{
				long long consume_gold = consume_list.need_gold_num, consume_gold_bind = 0;
				m_role->GetKnapsack()->GetMoney()->UseAllGold(consume_list.need_gold_num, "FlushHuSongTaskColor");

				EventHandler::Instance().OnShopBuyItem(m_role, consume_list.buy_count, consume_list.buy_list, consume_gold, consume_gold_bind);
			}

			if (consume_list.count > 0)
			{
				m_role->GetKnapsack()->ConsumeItemByIndexList(consume_list.count, consume_list.index_list, consume_list.num_list, "FlushHuSongTaskColor");
			}
		}
	}
		
	{
		// 给予
		if (color_ret > m_husong_info.task_color)
		{
			m_role->NoticeNum(noticenum::NT_HUSONG_REFRESH_COLOR_SUCC);

			// 成功才会提示消耗了什么
			if (consume_list.count > 0 || consume_list.need_gold_num > 0)
			{
				Protocol::SCHusongConsumeInfo hi;
				hi.token_num = consume_list.bind_item_num + consume_list.nobind_item_num;
				hi.bind_gold_num = static_cast<int>(final_use_bind_gold);
				hi.gold_num = static_cast<int>(final_use_gold);

				EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&hi, sizeof(hi));
			}
		}
		else
		{
			m_role->NoticeNum(errornum::EN_HUSONG_TASK_REFRESH_COLOR_FAIL);
		}

		m_husong_info.task_color = color_ret;
		m_husong_info.is_first_refresh = 1;	
		m_husong_info.refresh_count += (free_times + stuff_times);
		this->SendHusongTaskInfo(Protocol::HUSONG_INFO_NOTFIY_REASON_FLUSHCOLOR);
	}

	gamelog::g_log_husong_task.printf(LL_INFO, "OnRefreshTaskColor user[%d, %s], old_color[%d], new_color[%d], refresh_count[%d], "
		"free_times[%d], stuff_times[%d], consume_stuff[%d], all_gold[%d], old_refresh_count[%d], new_refresh_count[%d]",
		m_role->GetUID(), m_role->GetName(), old_color, m_husong_info.task_color, free_times + stuff_times, 
		free_times, stuff_times, consume_list.bind_item_num + consume_list.nobind_item_num, consume_list.need_gold_num, old_fresh_count, m_husong_info.refresh_count);
}
bool HusongTask::SetCount(int count, bool only_reduce)
{
	if (count < 0 || count > HUSONG_ONEDAY_ACCEPT_MAX) return false;

	if (only_reduce && count >= m_role->GetDayCounter()->GetDayCount(DayCounter::DAYCOUNT_ID_ACCEPT_HUSONG_TASK_COUNT)) return false;

	m_role->GetDayCounter()->DayCountSet(DayCounter::DAYCOUNT_ID_ACCEPT_HUSONG_TASK_COUNT, count);

	return true;
}

void HusongTask::OnAddShield()
{
	if (NULL == m_role) return;

	if (0 != m_husong_info.is_use_hudun)
	{
		m_role->NoticeNum(errornum::EN_GUILD_BATTLE_HAS_USE_SHILD);
		return;
	}

	m_husong_info.is_use_hudun = 1;

	int buff_continue_time = LOGIC_CONFIG->GetHuSongConfig().GetShieldTime();
	int buff_dec_hurt_per = LOGIC_CONFIG->GetHuSongConfig().GetShieldDecHurtPer();

	// 使自己受到伤害减少60%,持续5秒
	EffectDecHurt *effect_dec_hurt = new EffectDecHurt(m_role->GetId(), 1, buff_continue_time, 1 ,EffectBase::PRODUCT_METHOD_SKILL);
	effect_dec_hurt->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_3);
	effect_dec_hurt->SetDecPercent(buff_dec_hurt_per);
	m_role->AddEffect(effect_dec_hurt);

	this->SendHusongTaskInfo();

}

void HusongTask::SendHusongTaskInfo(short notify_reason, bool is_broadcast)
{
	Protocol::SCHusongInfo hi;
	hi.notfiy_reason = notify_reason;

	hi.obj_id = m_role->GetId();
	hi.task_id = m_husong_info.task_id;
	hi.task_color = m_husong_info.task_color;
	hi.accept_in_activityTime = m_husong_info.accpet_activitytime > 0 ? 1 : 0;
	hi.is_use_hudun = m_husong_info.is_use_hudun > 0 ? 1 : 0;
	hi.reserve_1 = 0;
	hi.reserve_2 = 0;

	if (is_broadcast)
	{
		if (NULL != m_role->GetScene()) m_role->GetScene()->GetZoneMatrix()->NotifyAreaMsg(m_role->GetObserHandle(), (void*)&hi, sizeof(hi));
	}
	else 
	{
		EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&hi, sizeof(hi));
	}
}

bool HusongTask::RefreshColorHelper(bool is_auto_buy, int &color_ret, int &free_times, int &stuff_times)
{
	int free_times_left = LOGIC_CONFIG->GetHuSongConfig().GetFreeRefreshTimes() - m_role->GetDayCounter()->GetDayCount(DayCounter::DAYCOUNT_ID_HUSONG_REFRESH_COLOR_FREE_TIMES);
	if (free_times_left < 0) 
	{
		free_times_left = 0;
	}

	if (free_times_left <= free_times)
	{
		ItemExtern::ItemConsumeList consume_list;
		ItemExtern::ItemConsumeConfig stuff(LOGIC_CONFIG->GetHuSongConfig().GetFlushItemId(), is_auto_buy, stuff_times + 1);

		if (!ItemExtern::GetConsumeListByOrder(m_role, false, 1, &stuff, &consume_list, false))
		{
			return false;
		}

		if (!m_role->GetKnapsack()->GetMoney()->AllGoldMoreThan(consume_list.need_gold_num))
		{
			return false;
		}

		++ stuff_times;
	}
	else
	{
		++ free_times;
	}

	if (color_ret <= TASK_COLOR_INVALID || color_ret >= TASK_COLOR_MAX) color_ret = TASK_COLOR_WHITE;

	const HuSongConfig::TaskColorFlushProbItem *flush_prob = LOGIC_CONFIG->GetHuSongConfig().GetTaskColorFlushProb(color_ret);
	if (NULL == flush_prob) return false;

	if (m_husong_info.IsFirstRefresh())																	
	{
		// 刷新任务颜色 第一次刷结果必定是橙色
		color_ret = TASK_COLOR_ORANGE;
	}
	else
	{
		int rand_value = RandomNum(100);
		for (int i = TASK_COLOR_WHITE; i < TASK_COLOR_MAX; ++i)
		{
			if (rand_value < flush_prob->color_problist[i])
			{
				color_ret = i;
				break;
			}

			rand_value -= flush_prob->color_problist[i];
		}
	}
	if (color_ret <= TASK_COLOR_INVALID || color_ret >= TASK_COLOR_MAX) color_ret = TASK_COLOR_WHITE;

	return true;
}

void HusongTask::CheckBuffStatus(bool is_add)
{
	const int HUSONG_SKILL_ID = 1;

	if (is_add)
	{
		if (this->IsHusongStatus())
		{
			const int HUSONG_BUFF_LAST_TIMES_MS = 7200 * 1000;

			// 目标减速
			int dec_speed_percent = LOGIC_CONFIG->GetHuSongConfig().GetSpeedDecPer();
			EffectMoveSpeed *effect_speed = new EffectMoveSpeed(m_role->GetId(), HUSONG_SKILL_ID, HUSONG_BUFF_LAST_TIMES_MS, EBT_CHIHUAN, 1, EffectBase::PRODUCT_METHOD_HUSONG);
			effect_speed->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);
			int value = static_cast<int>(m_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MOVE_SPEED) * (dec_speed_percent * ROLE_ATTR_PER));
			effect_speed->SetMoveSpeed(-value, -dec_speed_percent / 100);
			m_role->AddEffect(effect_speed);
		}
	}
	else
	{
		m_role->GetEffectList()->FindAndRemoveEffect(EffectBase::EFFECT_TYPE_MOVESPEED, EBT_CHIHUAN, INVALID_OBJ_ID, EffectBase::PRODUCT_METHOD_HUSONG, HUSONG_SKILL_ID);
	}
}

int HusongTask::CalcBaseReward(long long *base_commit_exp, int *base_commit_bind_coin ,ItemConfigData reward_item[MAX_ATTACHMENT_ITEM_NUM])
{
	int item_count = 0;
	const HuSongConfig::TaskRewardCfg *task_reward = LOGIC_CONFIG->GetHuSongConfig().GetTaskRewardCfg(m_role->GetLevel());
	if (NULL != task_reward)
	{
		int factor = LOGIC_CONFIG->GetHuSongConfig().GetTaskRewardFactor(m_husong_info.task_color);

		if (NULL != base_commit_exp) 
		{
			*base_commit_exp = static_cast<long long>(task_reward->commit_exp * 1.0 * factor / 100);
		}

		if (NULL != base_commit_bind_coin)
		{
			*base_commit_bind_coin = static_cast<int>(task_reward->commit_bind_coin * 1.0 * factor / 100);
		}

		for(int i = 0; i < MAX_ATTACHMENT_ITEM_NUM; i++)
		{
			int item_num = static_cast<int>(task_reward->reward_item[i].num * 1.0 * factor / 100);

			if (m_role->GetHusongTask()->IsHusongDoubleTime())
			{
				item_num *= 2;
			}
			
			if (NULL == ITEMPOOL->GetItem(task_reward->reward_item[i].item_id) || item_num <= 0)
			{
				continue;
			}

			reward_item[item_count].item_id = task_reward->reward_item[i].item_id;
			reward_item[item_count].num = item_num;
			reward_item[item_count].is_bind = task_reward->reward_item[i].is_bind;
			item_count++;
		}
	}

	return item_count;
}

void HusongTask::OnBuyTimes()
{
	if (m_role->GetSublock()->CheckHasLock())
	{
		return;
	}

	if (m_role->GetKnapsack()->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	int buy_count = m_role->GetDayCounter()->GetDayCount(DayCounter::DAYCOUNT_ID_HUSONG_TASK_VIP_BUY_COUNT);
	int can_buy_count = LOGIC_CONFIG->GetVipConfig().GetAuthParam(m_role->GetVip()->GetVipLevel(), VAT_BUY_HUSONG_TIMES);

	if (buy_count >= can_buy_count)
	{
		m_role->NoticeNum(errornum::EN_HUSONG_TASK_VIP_BUY_LIMIT);
		return;
	}

	int need_gold = LOGIC_CONFIG->GetHuSongConfig().GetBuyTimesCost(buy_count + 1);
	if (need_gold <= 0)
	{
		return;
	}

	if (!m_role->GetKnapsack()->GetMoney()->UseAllGold(need_gold, "BuyHusongTimes"))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	m_role->GetRoleActivity()->AddTurntableScore(need_gold);

	m_role->GetDayCounter()->DayCountIncrease(DayCounter::DAYCOUNT_ID_HUSONG_TASK_VIP_BUY_COUNT);
}

bool HusongTask::OnBuyTimes(int times)
{
	if (times <= 0)
	{
		return false;
	}
	if (m_role->GetSublock()->CheckHasLock())
	{
		return false;
	}

	if (m_role->GetKnapsack()->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	const int buy_count = m_role->GetDayCounter()->GetDayCount(DayCounter::DAYCOUNT_ID_HUSONG_TASK_VIP_BUY_COUNT);
	const int can_buy_count = LOGIC_CONFIG->GetVipConfig().GetAuthParam(m_role->GetVip()->GetVipLevel(), VAT_BUY_HUSONG_TIMES);

	if (buy_count >= can_buy_count)
	{
		m_role->NoticeNum(errornum::EN_HUSONG_TASK_VIP_BUY_LIMIT);
		return false;
	}
	const int remain_buy_count = can_buy_count - buy_count;	//剩余购买次数
	const int now_buy_times = times > remain_buy_count ? remain_buy_count : times;
	int need_gold = 0;
	for (int i = buy_count; i < buy_count + now_buy_times; ++i)
	{
		int local_need_gold = LOGIC_CONFIG->GetHuSongConfig().GetBuyTimesCost(i + 1);
		if (local_need_gold <= 0)
		{
			return false;
		}
		need_gold += local_need_gold;
	}

	if (!m_role->GetKnapsack()->GetMoney()->UseAllGold(need_gold, "BuyHusongTimes"))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return false;
	}

	m_role->GetRoleActivity()->AddTurntableScore(need_gold);

	m_role->GetDayCounter()->DayCountSet(DayCounter::DAYCOUNT_ID_HUSONG_TASK_VIP_BUY_COUNT, buy_count + now_buy_times);
	return true;
}

int HusongTask::GetTimes()
{
	const int max_husong_times = HUSONG_ONEDAY_ACCEPT_MAX + m_role->GetDayCounter()->GetDayCount(DayCounter::DAYCOUNT_ID_HUSONG_TASK_VIP_BUY_COUNT);
	const int used_husong_time = m_role->GetDayCounter()->GetDayCount(DayCounter::DAYCOUNT_ID_ACCEPT_HUSONG_TASK_COUNT);
	return max_husong_times - used_husong_time;
}

bool HusongTask::IsHusongDoubleTime()
{
	return 0 != m_husong_info.accpet_activitytime && ActivityShadow::Instance().IsActivtyOpen(ACTIVITY_TYPE_HUSONG);
}

long long HusongTask::GetHusongExpFind()
{
	double factor = LOGIC_CONFIG->GetHuSongConfig().GetExpFactor();
	double color_factor = (double)LOGIC_CONFIG->GetHuSongConfig().GetTaskRewardFactor(1) / 100;
	long long reward_exp = static_cast<long long>(factor * (LOGIC_CONFIG->GetItemLevelRewardConfig().GetExpDanReward(m_role->GetLevel(), 0)) * color_factor);
	return reward_exp;
}
