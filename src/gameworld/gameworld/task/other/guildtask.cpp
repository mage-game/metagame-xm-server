#include "guildtask.hpp"
#include "task/taskpool.h"
#include "task/taskrecorder.h"
#include "engineadapter.h"
#include "servercommon/errornum.h"
#include "config/logicconfigmanager.hpp"
#include "other/event/eventhandler.hpp"
#include "obj/character/role.h"
#include "other/route/mailroute.hpp"
#include "item/itempool.h"
#include "item/itembase.h"
#include "item/knapsack.h"
#include "servercommon/string/gameworldstr.h"
#include "gamelog.h"
#include "global/guild/guild.hpp"
#include "global/guild/guildmanager.hpp"
#include "task/newtaskmanager.h"
#include "other/sublock/sublock.hpp"
#include "protocal/msgnpc.h"
#include "other/itemlevelreward/itemlevelreward.hpp"
#include "other/vip/vip.hpp"
#include "other/vip/vipconfig.hpp"

GuildTask::GuildTask() : m_role(NULL)
{

}

GuildTask::~GuildTask()
{

}

void GuildTask::Init(Role *role, const GuildTaskParam &p)
{
	m_role = role;
	m_guild_param = p;
}

void GuildTask::GetInitParam(GuildTaskParam *p)
{
	*p = m_guild_param;
}

void GuildTask::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	if (old_dayid != now_dayid)
	{
		int task_id = INVALID_TASK_ID;
		if (m_role->GetTaskManager()->GetTaskIdByType(TASK_TYPE_GUILD, &task_id))
		{
			m_role->GetTaskManager()->GiveupTask(task_id, true);

			ROLE_LOG_QUICK6(LOG_TYPE_GIVEUP_TASK, m_role, TASK_TYPE_GUILD, task_id, NULL, NULL);
		}

		m_guild_param.Reset();
		this->SendGuildTaskInfo();
	}
}

bool GuildTask::CanAcceptTask(TaskID task_id, bool notify)
{
	if (INVALID_GUILD_ID == m_role->GetGuildID())
	{
		if (notify) m_role->NoticeNum(errornum::EN_XIANMENGZHAN_NO_GUILD);
		return false;
	}

	if (m_role->GetTaskRecorder()->IsFinished(task_id))
	{
		return false;
	}

	if (0 != m_guild_param.is_finish)
	{
		return false;
	}

	if (m_role->GetTaskManager()->HasTaskType(TASK_TYPE_GUILD))
	{
		return false;
	}
	
	const TaskCfg *task_cfg = TASKPOOL->GetCfgByTaskID(task_id);
	if (NULL != task_cfg && TASK_TYPE_GUILD == task_cfg->task_type)
	{
		if (m_guild_param.first_task == 0 || TASKPOOL->IsInGuildTaskList(m_guild_param.first_task, task_id))
		{
			return true;
		}

		// 处理玩家升级后，与当日首次任务不在同一等级段的问题
		if (task_cfg->min_level > 0 && m_role->GetLevel() >= task_cfg->min_level &&
			task_cfg->max_level > 0 && m_role->GetLevel() <= task_cfg->max_level)
		{
			if (task_cfg->pretaskid > 0)
			{
				if (!m_role->GetTaskRecorder()->IsFinished(task_cfg->pretaskid))
				{
					return false;
				}
				else
				{
					return true;
				}
			}

			return true;
		}
	}

	return false;
}

void GuildTask::OnAddTask(TaskID task_id, bool is_init)
{
	if (is_init)
	{
		return;
	}

	const TaskCfg *task_cfg = TASKPOOL->GetCfgByTaskID(task_id);
	if (NULL != task_cfg && TASK_TYPE_GUILD == task_cfg->task_type)
	{
		if (0 == m_guild_param.first_task && TASKPOOL->IsGuildFirstTaskID(task_id))
		{
			m_guild_param.first_task = task_id;

			if (m_role->GetLevel() < TASKPOOL->GetGuildTaskSpecialCountLimitLevel())
			{
				m_guild_param.guild_task_max_count = GuildTaskConfig::GUILD_TASK_SERIES_SPECIAL_LIMIT_COUNT;
			}
			else
			{
				m_guild_param.guild_task_max_count = GuildTaskConfig::GUILD_TASK_SERIES_LIMIT_COUNT;
			}
		}

		EventHandler::Instance().OnAcceptGuildTask(m_role);
	}
}

void GuildTask::SendGuildTaskInfo()
{
	Protocol::SCGuildTaskInfo gti;
	gti.double_reward_flag = m_guild_param.double_reward_flag;
	gti.is_finish = m_guild_param.is_finish;
	gti.complete_task_count = m_guild_param.complete_task_count;
	gti.guild_task_max_count = m_guild_param.guild_task_max_count;

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&gti, sizeof(gti));
}

void GuildTask::OnRemoveTask(TaskID task_id, bool is_complete)
{
	const TaskCfg *task_cfg = TASKPOOL->GetCfgByTaskID(task_id);
	if (NULL != task_cfg && TASK_TYPE_GUILD == task_cfg->task_type)
	{
		if (is_complete)
		{
			m_role->GetTaskRecorder()->Complete(task_id);
			++ m_guild_param.complete_task_count;

			if (task_id == TASKPOOL->GetEndTaskByFirstTask(m_guild_param.first_task) || m_guild_param.complete_task_count >= m_guild_param.guild_task_max_count)
			{
				m_guild_param.is_finish = 1;
				
				// 公会图腾经验
				{
					Guild *role_guild = GuildManager::Instance().GetGuild(m_role->GetUserId());
					if (NULL != role_guild)
					{
						GuildOtherConfig &guild_other_cfg = LOGIC_CONFIG->GetGuildConfig().GetGuildOtherConfig();
						if (role_guild->GetTotemExpToday() < guild_other_cfg.finish_task_totem_exp_reward * role_guild->GetGuildMaxMemberCount())
						{
							role_guild->AddTotemExp(guild_other_cfg.finish_task_totem_exp_reward);
							role_guild->AddTotemExpToday(guild_other_cfg.finish_task_totem_exp_reward);
						}
					}
				}
				// 完成公会任务奖励
				{
					const ItemConfigData &reward_item = TASKPOOL->GetGuildTaskAllCompleteReward();
					m_role->GetKnapsack()->PutOrMail(reward_item, PUT_REASON_GUILD_TASK_ALL_COMPLETE);
				}
			}

			this->SendGuildTaskInfo();

			EventHandler::Instance().OnCompleteGuildTask(m_role);

			ROLE_LOG_QUICK6(LOG_TYPE_GUILD_TASK_COMPLETE, m_role, m_role->GetLevel(), 1, "CommitTask", NULL);
		}
	}
}

bool GuildTask::DoReward(const TaskCfg *task_cfg)
{
	if (NULL == task_cfg || TASK_TYPE_GUILD != task_cfg->task_type)
	{
		return false;
	}

	const GuildTaskRewardCfg *guild_task_reward_p = TASKPOOL->GetGuildReward(m_role->GetLevel());
	if (NULL == guild_task_reward_p)
	{
		return false;
	}
	
	if (!m_role->GetKnapsack()->CheckEmptyGridNoLessThan(guild_task_reward_p->item_count))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return false;
	}

	//策划需求，修改增加经验规则（2018-1-3）
	const DailyTaskOtherCfg & other_cfg = TASKPOOL->GetDailyTaskOtherCfg();
	long long reward_exp = static_cast<long long>(other_cfg.exp_factor_guildtask * (LOGIC_CONFIG->GetItemLevelRewardConfig().GetExpDanReward(m_role->GetLevel(), 0)));

	if (m_guild_param.guild_task_max_count == GuildTaskConfig::GUILD_TASK_SERIES_SPECIAL_LIMIT_COUNT)
	{
		//m_role->AddExp(guild_task_reward_p->exp * 2, "GuildTaskReward");
		m_role->AddExp(reward_exp * 2, "GuildTaskReward", Role::EXP_ADD_REASON_DEFAULT);
	}
	else
	{
		//m_role->AddExp(guild_task_reward_p->exp, "GuildTaskReward");
		m_role->AddExp(reward_exp, "GuildTaskReward", Role::EXP_ADD_REASON_DEFAULT);
	}

	int lucky_prob = LOGIC_CONFIG->GetGuildConfig().GetGuildLuckyFactor(m_role->GetGuildLuckyColor(), GUILD_LUCKY_FACTOR_TYPE_GUILDTASK);
	
	{
		// 仙盟运势加成计算
		int extra_gongxian = static_cast<int>((float)guild_task_reward_p->gongxian * lucky_prob / 100.0f);

		int add_gongxian = guild_task_reward_p->gongxian + extra_gongxian;

		if (m_guild_param.guild_task_max_count == GuildTaskConfig::GUILD_TASK_SERIES_SPECIAL_LIMIT_COUNT)
		{
			add_gongxian *= 2;
		}

		m_role->AddGuildGongxian(add_gongxian, "GuildTaskReward");

		ItemConfigData item_list[GuildTaskRewardCfg::GUILD_TASK_REWARD_ITEM_MAX];

		for (int i = 0; i < guild_task_reward_p->item_count && i < GuildTaskRewardCfg::GUILD_TASK_REWARD_ITEM_MAX; ++ i)
		{
			item_list[i] = guild_task_reward_p->item_list[i];

			const ItemBase *itembase = ITEMPOOL->GetItem(item_list[i].item_id);
			if (NULL != itembase)
			{
				item_list[i].num = static_cast<int>(guild_task_reward_p->item_list[i].num * (1.0f + lucky_prob / 100.0f));

				if (m_guild_param.guild_task_max_count == GuildTaskConfig::GUILD_TASK_SERIES_SPECIAL_LIMIT_COUNT)
				{
					item_list[i].num *= 2; 
				}

				if (item_list[i].num > itembase->GetPileLimit())
				{
					item_list[i].num = itembase->GetPileLimit();
				}
			}
		}

		if (guild_task_reward_p->item_count > 0)
		{
			m_role->GetKnapsack()->PutList(guild_task_reward_p->item_count, item_list, PUT_REASON_GUILD_TASK_REWARD);
		}
	}

	return true;
}

void GuildTask::GetGuildDoubleReward()
{
	if (INVALID_GUILD_ID == m_role->GetGuildID())
	{
		m_role->NoticeNum(errornum::EN_XIANMENGZHAN_NO_GUILD);
		return;
	}

	if (0 == m_guild_param.is_finish || 0 != m_guild_param.double_reward_flag || m_guild_param.complete_task_count <= 0)
	{
		m_role->NoticeNum(errornum::EN_GUILD_GET_REWARD_FAILED);
		return;
	}

	if (m_role->GetSublock()->CheckHasLock())
	{
		return;
	}

	const GuildTaskRewardCfg *guild_reward_p = TASKPOOL->GetGuildReward(m_role->GetLevel());
	if (NULL == guild_reward_p) 
	{
		return;
	}
	if (guild_reward_p->exp <= 0 && guild_reward_p->item_count <= 0 && guild_reward_p->gongxian <= 0)
	{
		return;
	}

	int guildlucky_prob = LOGIC_CONFIG->GetGuildConfig().GetGuildLuckyFactor(m_role->GetGuildLuckyColor(), GUILD_LUCKY_FACTOR_TYPE_GUILDTASK);

	int reward_num_list[GuildTaskRewardCfg::GUILD_TASK_REWARD_ITEM_MAX] = {0};

	// 算出每种物品该奖励多少个 (包括仙盟运势产生的额外奖励)
	for (int i = 0; i < guild_reward_p->item_count; ++i)
	{
		reward_num_list[i] = guild_reward_p->item_list[i].num * m_guild_param.complete_task_count;
		reward_num_list[i] = static_cast<int>(reward_num_list[i] * (1.0f + (float)guildlucky_prob / 100.0f)); 
	}

	// 组织好奖励物品的数据格式
	int put_item_count = 0;
	ItemConfigData put_item_list[MAX_GUILD_REWARD_ITEM_COUNT];

	for (int i = 0; i < guild_reward_p->item_count; ++i)
	{
		const ItemBase *item_base_p = ITEMPOOL->GetItem(guild_reward_p->item_list[i].item_id);
		if (NULL == item_base_p)
		{
			return;
		}

		int item_num_limit = item_base_p->GetPileLimit();
		if (item_num_limit <= 0)
		{
			return;
		}

		while (reward_num_list[i] > 0)
		{
			if (put_item_count >= MAX_GUILD_REWARD_ITEM_COUNT) 
			{
				return;
			}

			if (reward_num_list[i] >= item_num_limit)
			{
				put_item_list[put_item_count] = guild_reward_p->item_list[i];
				put_item_list[put_item_count].num = item_num_limit;
				reward_num_list[i] -= item_num_limit;

				++ put_item_count;
			}
			else
			{
				put_item_list[put_item_count] = guild_reward_p->item_list[i];
				put_item_list[put_item_count].num = reward_num_list[i];
				reward_num_list[i] = 0;

				++ put_item_count;
			}			
		}
	}

	if (!m_role->GetKnapsack()->CheckEmptyGridNoLessThan(put_item_count))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	int guild_double_reward_cost = TASKPOOL->GetGuildDoubleRewardCost();
	if (!m_role->GetKnapsack()->GetMoney()->UseAllGold(guild_double_reward_cost, "GuildDoubleRewardCost"))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	m_guild_param.double_reward_flag = 1;

	//m_role->AddExp(guild_reward_p->exp * m_guild_param.complete_task_count, "GuildDoubleReward");

	//策划需求，修改增加经验规则（2018-1-3）
	const DailyTaskOtherCfg & other_cfg = TASKPOOL->GetDailyTaskOtherCfg();
	long long reward_exp = static_cast<long long>(other_cfg.exp_factor_guildtask * (LOGIC_CONFIG->GetItemLevelRewardConfig().GetExpDanReward(m_role->GetLevel(), 0)));
	m_role->AddExp(reward_exp * m_guild_param.complete_task_count, "GuildDoubleReward", Role::EXP_ADD_REASON_DEFAULT);

	{
		// 仙盟运势有加成 物品上面已有计算 

		m_role->GetKnapsack()->PutList(put_item_count, put_item_list, PUT_REASON_GUILD_TASK_REWARD);

		int gongxian_reward = static_cast<int>(guild_reward_p->gongxian * (1.0f + (float)guildlucky_prob / 100.0f) * m_guild_param.complete_task_count);
		m_role->AddGuildGongxian(gongxian_reward, "GuildDoubleReward");
	}


	this->SendGuildTaskInfo();
}

void GuildTask::OneKeyCompleteTask()
{
	if (m_guild_param.is_finish)
	{
		m_role->NoticeNum(errornum::EN_GUILD_TASK_ALL_COMPLETE);
		return;
	}

	if (INVALID_GUILD_ID == m_role->GetGuildID())
	{
		m_role->NoticeNum(errornum::EN_XIANMENGZHAN_NO_GUILD);
		return;
	}

	int left_count = m_guild_param.guild_task_max_count - m_guild_param.complete_task_count;
	if (left_count <= 0)
	{
		return;
	}

	int vip_level = m_role->GetVip()->GetVipLevel();
	int can_do = LOGIC_CONFIG->GetVipConfig().GetAuthParam(vip_level, VAT_GUILD_TASK_ONE_KEY);
	if (0 == can_do)
	{
		m_role->NoticeNum(errornum::EN_VIP_LEVEL_NOT_ENOUGH);
		return;
	}

	int need_gold = TASKPOOL->GetGuildTaskOneKeyCostPerTask() * left_count;
	if (need_gold <= 0)
	{
		return;
	}

	if (!m_role->GetKnapsack()->GetMoney()->AllGoldMoreThan(need_gold))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	const GuildTaskRewardCfg *guild_task_reward_p = TASKPOOL->GetGuildReward(m_role->GetLevel());
	if (NULL == guild_task_reward_p)
	{
		return;
	}

	int guildlucky_prob = LOGIC_CONFIG->GetGuildConfig().GetGuildLuckyFactor(m_role->GetGuildLuckyColor(), GUILD_LUCKY_FACTOR_TYPE_GUILDTASK);

	const int REWARD_ITEM_MAX_COUNT = 256;

	int item_count = 0;
	ItemConfigData reward_item_list[REWARD_ITEM_MAX_COUNT];

	for (int i = 0; i < guild_task_reward_p->item_count && i < GuildTaskRewardCfg::GUILD_TASK_REWARD_ITEM_MAX; ++ i)
	{
		const ItemBase *item = ITEMPOOL->GetItem(guild_task_reward_p->item_list[i].item_id);
		if (NULL ==  item)
		{
			return;
		}

		int pile_num = item->GetPileLimit();
		int reward_num = static_cast<int>(guild_task_reward_p->item_list[i].num * (1.0f + (float)guildlucky_prob / 100.0f) * left_count);

		if (m_guild_param.guild_task_max_count == GuildTaskConfig::GUILD_TASK_SERIES_SPECIAL_LIMIT_COUNT)
		{
			reward_num *= 2;
		}
		
		while (reward_num > 0 && item_count < REWARD_ITEM_MAX_COUNT)
		{
			int reward_item_num = 0;

			if (reward_num >= pile_num)
			{
				reward_item_num = pile_num;
				reward_num -= pile_num;
			}
			else
			{
				reward_item_num = reward_num;
				reward_num = 0;
			}

			if (0 == reward_item_num) 
			{
				return;
			}

			reward_item_list[item_count].num = reward_item_num;
			reward_item_list[item_count].item_id = guild_task_reward_p->item_list[i].item_id;
			reward_item_list[item_count].is_bind = guild_task_reward_p->item_list[i].is_bind;
			
			++ item_count;
 		}
	}

	if (!m_role->GetKnapsack()->CheckEmptyGridNoLessThan(item_count))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	if (!m_role->GetKnapsack()->GetMoney()->UseAllGold(need_gold, "OneKeyCompleteTask"))
	{
		return;
	}

	m_guild_param.is_finish = 1;
	m_guild_param.complete_task_count = m_guild_param.guild_task_max_count;

	//long long add_exp = guild_task_reward_p->exp;

	//策划需求，修改增加经验规则（2018-1-3）
	const DailyTaskOtherCfg & other_cfg = TASKPOOL->GetDailyTaskOtherCfg();
	long long add_exp = static_cast<long long>(other_cfg.exp_factor_guildtask * (LOGIC_CONFIG->GetItemLevelRewardConfig().GetExpDanReward(m_role->GetLevel(), 0)));

	int gongxian = guild_task_reward_p->gongxian;
	if (m_guild_param.guild_task_max_count == GuildTaskConfig::GUILD_TASK_SERIES_SPECIAL_LIMIT_COUNT)
	{
		add_exp *= 2;
		gongxian *= 2;
	}

	long long real_add_exp = 0;
	m_role->AddExp(add_exp * left_count, "OneKeyCompleteTask", Role::EXP_ADD_REASON_DEFAULT, &real_add_exp);

	int total_gongxian = static_cast<int>(gongxian * (1.0f + (float)guildlucky_prob / 100.0f) * left_count);

	int task_id = 0;
	if (m_role->GetTaskManager()->GetTaskIdByType(TASK_TYPE_GUILD, &task_id))
	{
		m_role->GetTaskManager()->GiveupTask(task_id);

		ROLE_LOG_QUICK6(LOG_TYPE_COMMIT_TASK, m_role, TASK_TYPE_GUILD, task_id, NULL, NULL);

		m_role->GetTaskManager()->taskCommitRandomReward(TASK_TYPE_GUILD, left_count, 1, total_gongxian, real_add_exp);
	}
	else
	{
		m_role->GetTaskManager()->OnGuildTaskStateChange();
	}

	{
		// 仙盟运势有加成 物品上面已有计算 

		m_role->AddGuildGongxian(total_gongxian,
			"OneKeyCompleteTask");

		// 物品上面已经计算
		if (item_count > 0)
		{
			m_role->GetKnapsack()->PutList(item_count, reward_item_list, PUT_REASON_GUILD_TASK_REWARD);
		}
	}

	// 公会图腾经验
	{
		Guild *role_guild = GuildManager::Instance().GetGuild(m_role->GetUserId());
		if (NULL != role_guild)
		{
			GuildOtherConfig &guild_other_cfg = LOGIC_CONFIG->GetGuildConfig().GetGuildOtherConfig();
			if (role_guild->GetTotemExpToday() < guild_other_cfg.finish_task_totem_exp_reward * role_guild->GetGuildMaxMemberCount())
			{
				if (m_guild_param.guild_task_max_count == GuildTaskConfig::GUILD_TASK_SERIES_SPECIAL_LIMIT_COUNT)
				{
					role_guild->AddTotemExp(guild_other_cfg.finish_task_totem_exp_reward * 2);
					role_guild->AddTotemExpToday(guild_other_cfg.finish_task_totem_exp_reward * 2);
				}
				else
				{
					role_guild->AddTotemExp(guild_other_cfg.finish_task_totem_exp_reward);
					role_guild->AddTotemExpToday(guild_other_cfg.finish_task_totem_exp_reward);
				}
			}
		}
	}

	// 一键完成奖励
	{
		const ItemConfigData &reward_item = TASKPOOL->GetGuildTaskAllCompleteReward();
		m_role->GetKnapsack()->PutOrMail(reward_item, PUT_REASON_GUILD_TASK_ALL_COMPLETE);
	}
	
	for (int i = 0; i < left_count; ++ i)
	{
		m_role->GetTaskManager()->OnCommitOneTask(TASKPOOL->GetGuildExampleTaskID());
		EventHandler::Instance().OnCompleteGuildTask(m_role);
	}

	this->SendGuildTaskInfo();

	ROLE_LOG_QUICK6(LOG_TYPE_GUILD_TASK_COMPLETE, m_role, m_role->GetLevel(), left_count, "OneKeyCompleteTask", NULL);

	{
		// 日志
		std::string get_item_log;
		ItemConfigDataListLog(get_item_log, item_count, reward_item_list);

		gamelog::g_log_guild_task.printf(LL_INFO, "OnKeyCompleteTask user[%d, %s], role_level[%d], complete_count[%d], use_all_gold[%d], add_exp[%lld], add_gongxian[%d], add_item[%s]", 
			m_role->GetUID(), m_role->GetName(), m_role->GetLevel(), left_count, need_gold, guild_task_reward_p->exp * left_count, guild_task_reward_p->gongxian * left_count, get_item_log.c_str());
	}
}


