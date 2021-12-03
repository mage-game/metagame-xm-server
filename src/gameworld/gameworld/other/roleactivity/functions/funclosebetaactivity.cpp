#include "other/roleactivity/roleactivity.hpp"

#include "world.h"
#include "engineadapter.h"
#include "gamecommon.h"
#include "obj/character/role.h"
#include "obj/character/monster.h"
#include "scene/activityshadow/activityshadow.hpp"

#include "other/event/eventhandler.hpp"
#include "other/route/mailroute.hpp"

#include "servercommon/noticenum.h"
#include "servercommon/errornum.h"
#include "servercommon/string/gameworldstr.h"
#include "servercommon/string/globalstr.h"
#include "servercommon/internalprotocal/gglobalprotocal.h"
#include "globalconfig/globalconfig.h"
#include "config/logicconfigmanager.hpp"
#include "config/sharedconfig/sharedconfig.h"
#include "config/activityconfig/closedbetaconfig.hpp"
#include "global/team/team.hpp"
#include "item/knapsack.h"
#include "other/activedegree/activedegree.hpp"
#include "other/welfare/welfare.hpp"
#include "other/fb/rolefbequip.hpp"

// --------------------------------------------------------------------------------------------------------
// 封测活动
// --------------------------------------------------------------------------------------------------------
void RoleActivity::OnCloseBetaActivityOperaReq(int opera_type, int param_1, int param_2, int param_3)
{
	if (!ActivityShadow::Instance().IsSpecialActivityOpen(SPECIAL_ACTIVITY_TYPE_CLOSEDBETA))
	{
		m_role->NoticeNum(errornum::EN_CLOSE_BETA_ACTIVITY_NOT_OPEN);
		return;
	}

	if (opera_type <= Protocol::CBA_OPERA_TYPE_INVALID || opera_type >= Protocol::CBA_OPERA_TYPE_MAX)
	{
		return;
	}

	bool need_send = false;

	switch(opera_type)
	{
	case Protocol::CBA_OPERA_TYPE_REQ_INFO:
		{
			need_send = true;
		}
		break;

	case Protocol::CBA_OPERA_TYPE_FETCH_LOING_REWARD:
		{
			need_send = this->OnCBAFetchLoginReward();
		}
		break;

	case Protocol::CBA_OPERA_TYPE_FETCH_GUILD_REWARD:
		{
			need_send = this->OnCBAFetchGuildReward();
		}
		break;

	case Protocol::CBA_OPERA_TYPE_FETCH_MARRY_REWARD:
		{
			need_send = this->OnCBAFetchMarryReward();
		}
		break;

	case Protocol::CBA_OPERA_TYPE_FETCH_ONLINE_REWARD:
		{
			need_send = this->OnCBAFetchOnlineReward(param_1);
		}
		break;

	case Protocol::CBA_OPERA_TYPE_FETCH_UPLEVEL_REWARD:
		{
			need_send = this->OnCBAFetchUplevelReward(param_1);
		}
		break;
	case Protocol::CBA_OPERA_TYPE_FETCH_JOIN_ACTIVITY_REWARD:
		{
			need_send = this->OnCBAFetchJoinActivityReward(param_1);
		}
		break;
	}

	if (need_send)
	{
		this->SendCloseBetaActivityInfo();
	}
}

void RoleActivity::SendCloseBetaActivityInfo()
{
	if (!ActivityShadow::Instance().IsSpecialActivityOpen(SPECIAL_ACTIVITY_TYPE_CLOSEDBETA)) 
	{
		return;
	}

	Protocol::SCCloseBetaActivityInfo cbai;
	cbai.has_fetch_login_reward = m_activity_info.cba_has_fetch_login_reward;
	cbai.has_fetch_guild_reward = m_activity_info.cba_has_fetch_guild_reward;
	cbai.has_fetch_marry_reward = m_activity_info.cba_has_fetch_marry_reward;
	cbai.fetch_online_reward_flag = m_activity_info.cba_fetch_online_reward_flag;
	cbai.fetch_uplevel_reward_flag = m_activity_info.cba_fetch_uplevel_reward_flag;
	cbai.join_activity_flag = m_activity_info.cba_join_activity_flag;
	cbai.fetch_activity_reward_flag = m_activity_info.cba_fetch_activity_reward_flag;
	cbai.expfb_satisfy_reward_cond_flag = m_activity_info.cba_expfb_satisfy_reward_cond_flag;
	cbai.expfb_fetch_reward_flag = m_activity_info.cba_expfb_fetch_reward_flag;
	cbai.equipfb_satisfy_reward_cond_flag = m_activity_info.cba_equipfb_satisfy_reward_cond_flag;
	cbai.equipfb_fetch_reward_flag = m_activity_info.cba_equipfb_fetch_reward_flag;
	cbai.tdfb_satisfy_reward_cond_flag = m_activity_info.cba_tdfb_satisfy_reward_cond_flag;
	cbai.tdfb_fetch_reward_flag = m_activity_info.cba_tdfb_fetch_reward_flag;
	cbai.challengefb_satisfy_reward_cond_flag = m_activity_info.cba_challengefb_satisfy_reward_cond_flag;
	cbai.challengefb_fetch_reeward_flag = m_activity_info.cba_challengefb_fetch_reward_flag;
	cbai.total_login_days = m_role->GetWelfare()->GetTotalLoginDays();
	cbai.online_time = m_role->GetActiveDegree()->GetDayOnlineTime();
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&cbai, sizeof(cbai));
}
void RoleActivity::OnPassEquipFB(int chapter, int level)
{
	{
		// 封测活动
		if (ActivityShadow::Instance().IsSpecialActivityOpen(SPECIAL_ACTIVITY_TYPE_CLOSEDBETA))
		{
			int max_star = m_role->GetFBEquip()->GetNeqMaxStar(chapter, level);
			const CloseBetaEquipFBCfg::ConfigItem *cfg_item = LOGIC_CONFIG->GetCloseBetaCfg().GetEquipFBCfg(chapter, level);
			if (NULL != cfg_item && max_star >= cfg_item->need_star)
			{
				int old_flag = m_activity_info.cba_equipfb_satisfy_reward_cond_flag;

				m_activity_info.cba_equipfb_satisfy_reward_cond_flag |= (1 << cfg_item->seq);

				if (old_flag != m_activity_info.cba_equipfb_satisfy_reward_cond_flag)
				{
					this->SendCloseBetaActivityInfo();
				}
			}
		}
	}
}


bool RoleActivity::OnCBAFetchMarryReward()
{
	if (0 != m_activity_info.cba_has_fetch_marry_reward)
	{
		m_role->NoticeNum(errornum::EN_CLOSE_BETA_HAS_FETCH_REWARD);
		return false;
	}

	if (0 == m_role->GetLastMarryTime())
	{
		m_role->NoticeNum(errornum::EN_CLOSE_BETA_HAS_NOT_MARRY);
		return false;
	}

	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return false;
	}

	m_activity_info.cba_has_fetch_marry_reward = 1;

	m_role->GetKnapsack()->Put(LOGIC_CONFIG->GetCloseBetaCfg().GetOtherCfg().marry_reward_item, PUT_REASON_CLOSE_BETA_ACTIVITY);

	return true;
}

bool RoleActivity::OnCBAFetchGuildReward()
{
	if (0 != m_activity_info.cba_has_fetch_guild_reward)
	{
		m_role->NoticeNum(errornum::EN_CLOSE_BETA_HAS_FETCH_REWARD);
		return false;
	}

	if (INVALID_GUILD_ID == m_role->GetGuildID())
	{
		m_role->NoticeNum(errornum::EN_CLOSE_BETA_JOIN_GUILD_FIRST);
		return false;
	}

	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return false;
	}

	m_activity_info.cba_has_fetch_guild_reward = 1;

	m_role->GetKnapsack()->Put(LOGIC_CONFIG->GetCloseBetaCfg().GetOtherCfg().guild_reward_item, PUT_REASON_CLOSE_BETA_ACTIVITY);

	return true;
}

bool RoleActivity::OnCBAFetchJoinActivityReward(int seq)
{
	UNSTD_STATIC_CHECK(CloseBetaJoinActivityRewardCfg::CONFIG_ITEM_MAX_COUNT <= (int)sizeof(m_activity_info.cba_fetch_activity_reward_flag) * 8);

	if (seq < 0 || seq >= (int)sizeof(m_activity_info.cba_fetch_activity_reward_flag) * 8)
	{
		return false;
	}

	if (0 != (m_activity_info.cba_fetch_activity_reward_flag & (1 << seq)))
	{
		m_role->NoticeNum(errornum::EN_CLOSE_BETA_HAS_FETCH_REWARD);
		return false;
	}

	if (0 == (m_activity_info.cba_join_activity_flag & (1 << seq)))
	{
		m_role->NoticeNum(errornum::EN_CLOSE_BETA_FETCH_REWARD_LIMIT);
		return false;
	}

	const CloseBetaJoinActivityRewardCfg::ConfigItem *cfg_item = LOGIC_CONFIG->GetCloseBetaCfg().GetJoinActivityCfgBySeq(seq);
	if (NULL == cfg_item)
	{
		return false;
	}

	m_activity_info.cba_fetch_activity_reward_flag |= (1 << seq);

	m_role->GetKnapsack()->GetMoney()->AddGold(cfg_item->reward_gold, "CloseBetaJoinActivityReward");

	return true;
}

bool RoleActivity::OnCBAFetchOnlineReward(int seq)
{
	UNSTD_STATIC_CHECK(CloseBetaOnlineTimeRewardCfg::ONLINE_REWARD_MAX_COUNT <= (int)sizeof(m_activity_info.cba_fetch_online_reward_flag) * 8);

	if (seq < 0 || seq >= CloseBetaOnlineTimeRewardCfg::ONLINE_REWARD_MAX_COUNT)
	{
		return false;
	}

	if (0 != (m_activity_info.cba_fetch_online_reward_flag & (1 << seq)))
	{
		m_role->NoticeNum(errornum::EN_CLOSE_BETA_HAS_FETCH_REWARD);
		return false;
	}

	const CloseBetaOnlineTimeRewardCfg::ConfigItem *cfg_item = LOGIC_CONFIG->GetCloseBetaCfg().GetOnlineTimeCfgBySeq(seq);
	if (NULL == cfg_item)
	{
		return false;
	}

	if (m_role->GetActiveDegree()->GetDayOnlineTime() < cfg_item->need_online_time_s)
	{
		m_role->NoticeNum(errornum::EN_CLOSE_BETA_ONLINE_TIMES_LIMIT);
		return false;
	}

	m_activity_info.cba_fetch_online_reward_flag |= (1 << seq);

	m_role->GetKnapsack()->GetMoney()->AddGold(cfg_item->reward_gold, "CloseBetaOnlineReward");

	return true;
}

bool RoleActivity::OnCBAFetchUplevelReward(int seq)
{
	UNSTD_STATIC_CHECK(CloseBetaUplevelRewardCfg::CONFIG_ITEM_MAX_COUNT <= (int)sizeof(m_activity_info.cba_fetch_uplevel_reward_flag) * 8);

	if (seq < 0 || seq >= (int)sizeof(m_activity_info.cba_fetch_uplevel_reward_flag) * 8)
	{
		return false;
	}

	if (0 != (m_activity_info.cba_fetch_uplevel_reward_flag & (1 << seq)))
	{
		m_role->NoticeNum(errornum::EN_CLOSE_BETA_HAS_FETCH_REWARD);
		return false;
	}

	const CloseBetaUplevelRewardCfg::ConfigItem *cfg_item = LOGIC_CONFIG->GetCloseBetaCfg().GetUplevelRewardCfg(seq);
	if (NULL == cfg_item)
	{
		return false;
	}

	if (m_role->GetLevel() < cfg_item->need_level)
	{
		m_role->NoticeNum(errornum::EN_CLOSE_BETA_FETCH_REWARD_LIMIT);
		return false;
	}

	m_activity_info.cba_fetch_uplevel_reward_flag |= (1 << seq);

	m_role->GetKnapsack()->GetMoney()->AddGold(cfg_item->reward_gold, "CloseBetaUpleveReward");

	return true;
}

bool RoleActivity::OnCBAFetchLoginReward()
{
	if (0 != m_activity_info.cba_has_fetch_login_reward)
	{
		m_role->NoticeNum(errornum::EN_CLOSE_BETA_HAS_FETCH_REWARD);
		return false;
	}

	int total_login_days = m_role->GetWelfare()->GetTotalLoginDays();
	const CloseBetaLoginRewardCfg::ConfigItem *cfg_item = LOGIC_CONFIG->GetCloseBetaCfg().GetLoginRewardCfg(total_login_days);
	if (NULL == cfg_item)
	{
		return false;
	}

	m_activity_info.cba_has_fetch_login_reward = 1;

	m_role->GetKnapsack()->GetMoney()->AddGold(cfg_item->reward_gold, "CloseBetaLoginReward");

	return true;
}
