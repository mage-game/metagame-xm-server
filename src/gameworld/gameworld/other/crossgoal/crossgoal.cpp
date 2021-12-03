#include "crossgoal.hpp"
#include "crossserver/config/sharedconfig.h"
#include "gameworld/config/logicconfigmanager.hpp"
#include "gameworld/item/knapsack.h"
#include "servercommon/serverconfig/crossconfig.hpp"
#include "global/guild/guildmanager.hpp"
#include "global/guild/guild.hpp"
#include "global/cross/crossmanager.h"
#include "other/cross/rolecross.h"

CrossGoal::CrossGoal() : m_role(NULL)
{

}

CrossGoal::~CrossGoal()
{

}

void CrossGoal::Init(Role *role, const CrossGoalParam &param)
{
	m_role = role;
	m_param = param;
}

void CrossGoal::GetInitParam(CrossGoalParam *param)
{
	*param = m_param;
}

void CrossGoal::OnUserLogin()
{
	if (!CrossConfig::Instance().IsHiddenServer())
	{
		int open_day_index = GetDayIndex(SHAREDCONFIG->GetOpenServerConfig().GetServerRealStartTime(), EngineAdapter::Instance().Time());
		const CrossGoalOtherCfg & other_cfg = LOGIC_CONFIG->GetCrossGoalConfig().GetOtherCfg();
		if (open_day_index >= other_cfg.open_day_beg && open_day_index <= other_cfg.open_day_end)
		{
			this->CheckCrossGoalStatus();
			this->CheckGuildGoalStatus();
		}
		else
		{
			m_param.Reset();
		}
	}
}

void CrossGoal::Update(unsigned int now_second)
{
}

void CrossGoal::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	if (!CrossConfig::Instance().IsHiddenServer())
	{
		int open_day_index = GetDayIndex(SHAREDCONFIG->GetOpenServerConfig().GetServerRealStartTime(), EngineAdapter::Instance().Time());
		const CrossGoalOtherCfg & other_cfg = LOGIC_CONFIG->GetCrossGoalConfig().GetOtherCfg();
		if (open_day_index >= other_cfg.open_day_beg && open_day_index <= other_cfg.open_day_end)
		{
			this->CheckCrossGoalStatus();
			this->SendInfoToRole();
		}
		else
		{
			m_param.Reset();
		}
	}
}

void CrossGoal::ChangeCrossGoalParam(int cond_type, int param)
{
	switch (cond_type)
	{
	case CROSS_GOAL_COND_KILL_CROSS_BOSS:
	{
		m_param.kill_cross_boss_num += param;
	}
	break;

	case CROSS_GOAL_COND_CROSS_BOSS_ROLE_KILLER:
	{
		m_param.cross_boss_role_killer += param;
	}
	break;

	case CROSS_GOAL_COND_FINISH_BAIZHAN_TASK:
	{
		m_param.finish_baizhan_task_num += param;
	}
	break;

	case CROSS_GOAL_COND_KILL_BAIZHAN_BOSS:
	{
		m_param.kill_baizhan_boss_num += param;
	}
	break;

	default:
		break;
	}
}

void CrossGoal::OperaReq(Protocol::CSCrossGoalOperaReq *req)
{
	switch (req->opera_type)
	{
	case Protocol::CSCrossGoalOperaReq::CROSS_GOAL_INFO_REQ:
	{
		this->SendInfoToRole();
	}
	break;
	case Protocol::CSCrossGoalOperaReq::FETCH_CROSS_GOAL_REWARD_REQ:
	{
		this->FetchRewardByIndex(req->param);
	}
	break;
	case Protocol::CSCrossGoalOperaReq::FETCH_GUILD_GOAL_REWAED_REQ:
	{
		this->FetchGuildRewardByindex(req->param);
	}
	break;

	default:
		break;
	}
}

void CrossGoal::FetchRewardByIndex(int index)
{
	int open_day_index = GetDayIndex(m_role->GetServerStartTime(), EngineAdapter::Instance().Time());
	const CrossGoalOtherCfg & other_cfg = LOGIC_CONFIG->GetCrossGoalConfig().GetOtherCfg();
	if (open_day_index < other_cfg.open_day_beg || open_day_index > other_cfg.open_day_end)
	{
		return;
	}

	if (index < 0 || index >= CROSS_GOAL_TASK_COUNT_MAX) return;

	if (m_param.fetch_reward_flag[index] == CORSS_GAOL_REWARD_TYPE_UNDONE)
	{
		this->CheckCrossGoalStatus();
	}

	if (m_param.fetch_reward_flag[index] != CORSS_GAOL_REWARD_TYPE_CAN_FETCH)
	{
		m_role->NoticeNum(errornum::EN_ROLE_GOAL_CAN_NOT_FETCH_REWARD);
		return;
	}

	const CrossGoalItem * goal_item = LOGIC_CONFIG->GetCrossGoalConfig().GetCrossGoalItemByIndex(index);
	if (goal_item == NULL)
	{
		return;
	}
	
	m_role->GetKnapsack()->PutListOrMail(goal_item->goal_reward, PUT_REASON_CROSS_GOAL_REWARD);
	
	m_param.fetch_reward_flag[index] = CORSS_GAOL_REWARD_TYPE_FINISH;

	this->CheckCrossGoalStatus();

	this->SendInfoToRole();

	m_role->NoticeNum(noticenum::NT_MONTH_CARD_REWARD_FETCH_SUCCESS);
	
	if (goal_item->cond_type == CROSS_GOAL_COND_FINISH_ALL_BEFORE)
	{
		int lenth = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), "cross_goal_role_final", m_role->GetUID(), m_role->GetName(), m_role->GetCamp(), goal_item->goal_reward[0].item_id);
		if (lenth > 0)
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gstr::GSTR_BUF, lenth, SYS_MSG_CENTER_ROLL_2);
		}
	}
}

void CrossGoal::FetchGuildRewardByindex(int index)
{
	int open_day_index = GetDayIndex(m_role->GetServerStartTime(), EngineAdapter::Instance().Time());
	const CrossGoalOtherCfg & other_cfg = LOGIC_CONFIG->GetCrossGoalConfig().GetOtherCfg();
	if (open_day_index < other_cfg.open_day_beg || open_day_index > other_cfg.open_day_end)
	{
		return;
	}

	if (index < 0 || index >= CROSS_GOAL_TASK_COUNT_MAX) return;

	if (m_param.guild_fetch_reward_flag[index] == CORSS_GAOL_REWARD_TYPE_UNDONE)
	{
		this->CheckGuildGoalStatus();
	}

	if (m_param.guild_fetch_reward_flag[index] != CORSS_GAOL_REWARD_TYPE_CAN_FETCH)
	{
		m_role->NoticeNum(errornum::EN_ROLE_GOAL_CAN_NOT_FETCH_REWARD);
		return;
	}

	const CrossGoalItem * goal_item = LOGIC_CONFIG->GetCrossGoalConfig().GetGuildGoalItemByIndex(index);
	if (goal_item == NULL)
	{
		return;
	}

	m_role->GetKnapsack()->PutListOrMail(goal_item->goal_reward, PUT_REASON_CROSS_GOAL_REWARD);

	m_param.guild_fetch_reward_flag[index] = CORSS_GAOL_REWARD_TYPE_FINISH;

	this->CheckGuildGoalStatus();

	this->SendInfoToRole();

	m_role->NoticeNum(noticenum::NT_MONTH_CARD_REWARD_FETCH_SUCCESS);
}

void CrossGoal::SendInfoToRole(char guild_notify)
{
	Protocol::SCCrossGoalInfo pro;
	
	memcpy(pro.fetch_reward_flag, m_param.fetch_reward_flag,sizeof(pro.fetch_reward_flag));
	memcpy(pro.guild_fetch_reward_flag, m_param.guild_fetch_reward_flag, sizeof(pro.guild_fetch_reward_flag));
	pro.kill_cross_boss_num = m_param.kill_cross_boss_num;
	pro.cross_boss_role_killer = m_param.cross_boss_role_killer;
	pro.kill_baizhan_boss_num = m_param.kill_baizhan_boss_num;
	pro.finish_baizhan_task_num = m_param.finish_baizhan_task_num;
	pro.guild_kill_cross_boss = 0;
	pro.guild_kill_baizhan_boss = 0;
	pro.reserve = 0;

	Guild* guild = GuildManager::Instance().GetGuild(m_role->GetGuildID());
	if (NULL != guild)
	{
		pro.guild_kill_cross_boss = guild->GetCrossGoalKillCrossBoss();
		pro.guild_kill_baizhan_boss = guild->GetCrossGoalKillBaiZhanBoss();
	}

//	for (int i = 0; i < 10; ++i)
//	{
//		printf("----i:%d,(c:%d,gd:%d)", i,pro.fetch_reward_flag[i], pro.guild_fetch_reward_flag[i]);
//	}
//	printf("u:%d\n", m_role->GetUID());

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&pro, sizeof(pro));
}

void CrossGoal::CheckCrossGoalStatus()
{
	for (int i = 0; i < CROSS_GOAL_TASK_COUNT_MAX; i++)
	{
		if (m_param.fetch_reward_flag[i] >= CORSS_GAOL_REWARD_TYPE_CAN_FETCH)
		{
			continue;
		}

		const CrossGoalItem * goal_item = LOGIC_CONFIG->GetCrossGoalConfig().GetCrossGoalItemByIndex(i);
		if(goal_item ==NULL) continue;

		if (this->CheckCondFinish(goal_item))
		{
			m_param.fetch_reward_flag[i] = CORSS_GAOL_REWARD_TYPE_CAN_FETCH;
		}
	}
}

bool CrossGoal::CheckGuildGoalStatus(bool notify_client)
{
	bool have_act = false;
	Guild* guild = GuildManager::Instance().GetGuild(m_role->GetGuildID());
	if (NULL != guild)
	{
		int flag = guild->GetGuildCrossGoalFlag();
		for (int i = 0; i < CROSS_GOAL_TASK_COUNT_MAX; i++)
		{
			if (m_param.guild_fetch_reward_flag[i] >= CORSS_GAOL_REWARD_TYPE_CAN_FETCH)
			{
				continue;
			}

			if(0 != (flag & (1<<i)))
			{
				m_param.guild_fetch_reward_flag[i] = CORSS_GAOL_REWARD_TYPE_CAN_FETCH;
				have_act = true;
			}
		}
	}

	if (have_act && notify_client)
	{
		this->SendInfoToRole();
	}

	return have_act;
}

bool CrossGoal::CheckCondFinish(const CrossGoalItem * goal_item)
{
	if (goal_item == NULL) return false;

	bool can_fetch = false;
	switch (goal_item->cond_type)
	{
	case CROSS_GOAL_COND_KILL_CROSS_BOSS:
	{
		if (m_param.kill_cross_boss_num >= goal_item->cond_param[0])
		{
			can_fetch = true;
		}
	}
	break;
	case CROSS_GOAL_COND_CROSS_BOSS_ROLE_KILLER:
	{
		if (m_param.cross_boss_role_killer >= goal_item->cond_param[0])
		{
			can_fetch = true;
		}
	}
	break;

	case CROSS_GOAL_COND_FINISH_ALL_BEFORE:
	{
		bool all_finish = true;
		for (unsigned int i = 0; i < goal_item->cond_param.size(); i++)
		{
			if (goal_item->cond_param[i] < 0 || goal_item->cond_param[i] >= CROSS_GOAL_TASK_COUNT_MAX)
			{
				all_finish = false;
				break;
			}

			if (m_param.fetch_reward_flag[goal_item->cond_param[i]] < CORSS_GAOL_REWARD_TYPE_FINISH)
			{
				all_finish = false;
				break;
			}
		}
		can_fetch = all_finish;
	}
	break;

	case CROSS_GOAL_COND_FINISH_BAIZHAN_TASK:
	{
		if (m_param.finish_baizhan_task_num >= goal_item->cond_param[0])
		{
			can_fetch = true;
		}
	}
	break;

	case CROSS_GOAL_COND_KILL_BAIZHAN_BOSS:
	{
		if (m_param.kill_baizhan_boss_num >= goal_item->cond_param[0])
		{
			can_fetch = true;
		}
	}
	break;

	default:
		break;
	}

	return can_fetch;
}