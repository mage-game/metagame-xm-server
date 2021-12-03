#include "touzijihua.hpp"
#include "obj/character/role.h"
#include "config/touzijihuaconfig.hpp"
#include "config/joinlimitconfig.hpp"
#include "servercommon/errornum.h"
#include "config/logicconfigmanager.hpp"
#include "config/guildconfig.hpp"
#include "other/event/eventhandler.hpp"
#include "gameworld/scene/activityshadow/activityshadow.hpp"
#include "world.h"
#include "servercommon/string/gameworldstr.h"
#include "global/guild/roleguild.hpp"
#include "item/knapsack.h"
#include "other/sublock/sublock.hpp"
#include "other/fb/rolefbequip.hpp"
#include "other/tianshuxunzhu/tianshuxunzhu.hpp"
#include "other/tianshuxunzhu/tianshuxunzhuconfig.hpp"

Touzijihua::Touzijihua() : m_role(NULL), m_active_plan_0(0), m_active_plan_1(0), m_active_plan_2(0), m_active_plan_3(0), m_active_highest_plan(-1), m_plan_0_reward_flag(0), m_plan_1_reward_flag(0),
m_plan_2_reward_flag(0), m_plan_3_reward_flag(0), m_buy_time(0), m_reward_flag(0), m_vip_reward_flag(0), m_reward_gold_bind_flag(0), m_new_plan_first_reward_flag(0),
m_active_fb_plan(0), m_fb_plan_reward_flag(0), m_active_boss_plan(0), m_boss_plan_reward_flag(0), m_kill_boss_num(0)
{
	memset(m_foundation_next_reward_index, 0, sizeof(m_foundation_next_reward_index));			// 0表示未购买
}

Touzijihua::~Touzijihua()
{

}

void Touzijihua::Init(Role *role, char active_plan_0, char active_plan_1, char active_plan_2, char active_plan_3, char active_highest_plan, short plan_0_reward_flag, short plan_1_reward_flag,
					  short plan_2_reward_flag, short plan_3_reward_flag, unsigned int buy_time, int reward_flag, int vip_reward_flag, int reward_gold_bind_flag, char first_reward_flag, const char foundation_next_reward_index[TouzijihuaConfig::MAX_FOUNDATION_TYPE],
					  short active_fb_plan, short fb_plan_reward_flag, short active_boss_plan, short boss_plan_reward_flag, short kill_boss_num, short kill_shenyu_boss_num, short active_shenyu_boss_plan, short shenyu_boss_plan_reward_flag)
{
	m_role = role;
	m_active_plan_0 = active_plan_0;
	m_active_plan_1 = active_plan_1;
	m_active_plan_2 = active_plan_2;
	m_active_plan_3 = active_plan_3;
	m_active_highest_plan = active_highest_plan;
	m_plan_0_reward_flag = plan_0_reward_flag;
	m_plan_1_reward_flag = plan_1_reward_flag;
	m_plan_2_reward_flag = plan_2_reward_flag;
	m_plan_3_reward_flag = plan_3_reward_flag;
	m_buy_time = buy_time;
	m_reward_flag = reward_flag;
	m_vip_reward_flag = vip_reward_flag;
	m_reward_gold_bind_flag = reward_gold_bind_flag;
	m_new_plan_first_reward_flag = first_reward_flag;
	memcpy(m_foundation_next_reward_index, foundation_next_reward_index, sizeof(m_foundation_next_reward_index));
	m_active_fb_plan = active_fb_plan;
	m_fb_plan_reward_flag = fb_plan_reward_flag;
	m_active_boss_plan = active_boss_plan;
	m_boss_plan_reward_flag = boss_plan_reward_flag;
	m_kill_boss_num = kill_boss_num;

	m_kill_shenyu_boss_num = kill_shenyu_boss_num;
	m_active_shenyu_boss_plan = active_shenyu_boss_plan;
	m_shenyu_boss_plan_reward_flag = shenyu_boss_plan_reward_flag;
}

void Touzijihua::GetInitParam(char *active_plan_0, char *active_plan_1, char *active_plan_2, char *active_plan_3, char *active_highest_plan, short *plan_0_reward_flag, short *plan_1_reward_flag,
							  short *plan_2_reward_flag, short *plan_3_reward_flag, unsigned int *buy_time, int *reward_flag, int *vip_reward_flag, int *reward_gold_bind_flag, char *first_reward_flag, char foundation_next_reward_index[TouzijihuaConfig::MAX_FOUNDATION_TYPE],
							  short *active_fb_plan, short *fb_plan_reward_flag, short *active_boss_plan, short *boss_plan_reward_flag, short *kill_boss_num, short *kill_shenyu_boss_num, short *active_shenyu_boss_plan, short *shenyu_boss_plan_reward_flag)
{
	*active_plan_0 = m_active_plan_0;
	*active_plan_1 = m_active_plan_1;
	*active_plan_2 = m_active_plan_2;
	*active_plan_3 = m_active_plan_3;
	*active_highest_plan = m_active_highest_plan;
	*plan_0_reward_flag = m_plan_0_reward_flag;
	*plan_1_reward_flag = m_plan_1_reward_flag;
	*plan_2_reward_flag = m_plan_2_reward_flag;
	*plan_3_reward_flag = m_plan_3_reward_flag;
	*buy_time = m_buy_time;
	*reward_flag = m_reward_flag;
	*vip_reward_flag = m_vip_reward_flag;
	*reward_gold_bind_flag = m_reward_gold_bind_flag;
	*first_reward_flag = m_new_plan_first_reward_flag;
	memcpy(foundation_next_reward_index, m_foundation_next_reward_index, sizeof(m_foundation_next_reward_index));
	*active_fb_plan = m_active_fb_plan;
	*fb_plan_reward_flag = m_fb_plan_reward_flag;
	*active_boss_plan = m_active_boss_plan;
	*boss_plan_reward_flag = m_boss_plan_reward_flag;
	*kill_boss_num = m_kill_boss_num;

	*kill_shenyu_boss_num = m_kill_shenyu_boss_num;
	*active_shenyu_boss_plan = m_active_shenyu_boss_plan;
	*shenyu_boss_plan_reward_flag = m_shenyu_boss_plan_reward_flag;
}

void Touzijihua::OnRoleLogin()
{
	this->SendFbBossInfo();
}

void Touzijihua::SendInfo()
{
	Protocol::SCTouZiJiHuaInfo tzjhi;
	tzjhi.active_plan_0 = m_active_plan_0;
	tzjhi.active_plan_1 = m_active_plan_1;
	tzjhi.active_plan_2 = m_active_plan_2;
	tzjhi.new_plan_first_reward_flag = m_new_plan_first_reward_flag;

	tzjhi.plan_0_reward_flag = m_plan_0_reward_flag;
	tzjhi.plan_1_reward_flag = m_plan_1_reward_flag;
	tzjhi.plan_2_reward_flag = m_plan_2_reward_flag;
	tzjhi.active_highest_plan = m_active_highest_plan;
	tzjhi.reserve_sh = 0;

	tzjhi.reserve = 0;
	tzjhi.active_plan_3 = m_active_plan_3;
	tzjhi.plan_3_reward_flag = m_plan_3_reward_flag;

	tzjhi.buy_time = m_buy_time;
	tzjhi.reward_flag = m_reward_flag;
	tzjhi.vip_reward_flag = m_vip_reward_flag;
	tzjhi.reward_gold_bind_flag = m_reward_gold_bind_flag;
	tzjhi.list_len = LOGIC_CONFIG->GetTouzijihuaCfg().GetFoundationCfgCount();
	memcpy(tzjhi.foundation_next_reward_index, m_foundation_next_reward_index, sizeof(char) * TouzijihuaConfig::MAX_FOUNDATION_TYPE);
	
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&tzjhi, sizeof(tzjhi));
}

void Touzijihua::OnFetchReward(int plan_type, int seq)
{
	int reward_gold = 0;
	int reward_bind_gold = 0;

	if (!this->CalcPlanReward(plan_type, seq, &reward_gold, &reward_bind_gold))
	{
		return;
	}

	if (reward_gold <= 0 && reward_bind_gold <= 0)
	{
		return;
	}

 	if (m_role->GetKnapsack()->GetMoney()->Add(reward_gold, reward_bind_gold, 0, 0, "TouzijihuaReward"))
 	{
		switch(plan_type)
		{
		case TOUZI_JIHUA_PLAN_TYPE_0:
			{
				m_plan_0_reward_flag |= (1 << seq);
			}
			break;

		case TOUZI_JIHUA_PLAN_TYPE_1:
			{
				m_plan_0_reward_flag |= (1 << seq);
				m_plan_1_reward_flag |= (1 << seq);
			}
			break;

		case TOUZI_JIHUA_PLAN_TYPE_2:
			{
				m_plan_0_reward_flag |= (1 << seq);
				m_plan_1_reward_flag |= (1 << seq);
				m_plan_2_reward_flag |= (1 << seq);
			}
			break;

		case TOUZI_JIHUA_PLAN_TYPE_3:
			{
				m_plan_0_reward_flag |= (1 << seq);
				m_plan_1_reward_flag |= (1 << seq);
				m_plan_2_reward_flag |= (1 << seq);
				m_plan_3_reward_flag |= (1 << seq);
			}
			break;
		}
 
 		this->SendInfo();
 
 		gamelog::g_log_touzijihua.printf(LL_INFO, "OnFetchReward user[%d, %s] role_level[%d], plan_type[%d], seq[%d], reward_flag_0[%d], reward_flag_1[%d], reward_flag_2[%d], reward_flag_3[%d], reward_gold[%d], reward_gold_bind[%d]",
 			m_role->GetUID(), m_role->GetName(), m_role->GetLevel(), plan_type, seq, m_plan_0_reward_flag, m_plan_1_reward_flag, m_plan_2_reward_flag, m_plan_3_reward_flag, reward_gold, reward_bind_gold);
 	}
}

bool Touzijihua::CalcPlanReward(int plan_type, int seq, int *reward_gold, int *reward_bind_gold)
{
	if (plan_type < TOUZI_JIHUA_PLAN_TYPE_0 || plan_type >= TOUZI_JIHUA_PLAN_MAX_TYPE)
	{
		return false;
	}

	if (NULL == reward_gold || NULL == reward_bind_gold)
	{
		return false;
	}

	if (plan_type != m_active_highest_plan)
	{
		return false;
	}

	UNSTD_STATIC_CHECK(TouzijihuaRewardConfig::CONFIG_ITEM_MAX_COUNT <= (int)sizeof(short) * 8);

	if (seq < 0 || seq >= (int)sizeof(short) * 8)
	{
		return false;
	}

	switch(plan_type)
	{
	case TOUZI_JIHUA_PLAN_TYPE_0:
		{
			 if (0 == m_active_plan_0)
			 {
				 m_role->NoticeNum(errornum::EN_TOUZIJIHUA_PLAN_NOT_ACTIVE);
				 return false;
			 }

			 const TouzijihuaRewardConfig::ConfigItem *cfg_item = LOGIC_CONFIG->GetTouzijihuaCfg().GetRewardCfg(TOUZI_JIHUA_PLAN_TYPE_0, seq);
			 if (NULL == cfg_item)
			 {
				 return false;
			 }

			 if (m_role->GetLevel() < cfg_item->need_level)
			 {
				 m_role->NoticeNum(errornum::EN_TOUZIJIHUA_FETCH_REWARD_LIMIT);
				 return false;
			 }

			 if (0 != (m_plan_0_reward_flag & (1 << seq)))
			 {
				 m_role->NoticeNum(errornum::EN_TOUZIJIHUA_HAS_FETCH_REWARD);
				 return false;
			 }

			 *reward_gold = cfg_item->reward_gold;
			 *reward_bind_gold = cfg_item->reward_gold_bind;

			 return true;
		}
		break;

	case TOUZI_JIHUA_PLAN_TYPE_1:
		{
			if (0 == m_active_plan_1)
			{
				m_role->NoticeNum(errornum::EN_TOUZIJIHUA_PLAN_NOT_ACTIVE);
				return false;
			}

			const TouzijihuaRewardConfig::ConfigItem *cfg_item_1 = LOGIC_CONFIG->GetTouzijihuaCfg().GetRewardCfg(TOUZI_JIHUA_PLAN_TYPE_1, seq);
			if (NULL == cfg_item_1)
			{
				return false;
			}

			if (m_role->GetLevel() < cfg_item_1->need_level)
			{
				m_role->NoticeNum(errornum::EN_TOUZIJIHUA_FETCH_REWARD_LIMIT);
				return false;
			}

			if (0 != (m_plan_1_reward_flag & (1 << seq)))
			{
				m_role->NoticeNum(errornum::EN_TOUZIJIHUA_HAS_FETCH_REWARD);
				return false;
			}

			*reward_gold = cfg_item_1->reward_gold;
			*reward_bind_gold = cfg_item_1->reward_gold_bind;

			const TouzijihuaRewardConfig::ConfigItem *cfg_item_0 = LOGIC_CONFIG->GetTouzijihuaCfg().GetRewardCfg(TOUZI_JIHUA_PLAN_TYPE_0, seq);
			if (NULL != cfg_item_0)
			{
				if (0 != (m_plan_0_reward_flag & (1 << seq)))
				{
					*reward_gold -= cfg_item_0->reward_gold;
					*reward_bind_gold -= cfg_item_0->reward_gold_bind;
				}
			}

			return true;
		}
		break;

	case TOUZI_JIHUA_PLAN_TYPE_2:
		{
			if (0 == m_active_plan_2)
			{
				m_role->NoticeNum(errornum::EN_TOUZIJIHUA_PLAN_NOT_ACTIVE);
				return false;
			}

			const TouzijihuaRewardConfig::ConfigItem *cfg_item_2 = LOGIC_CONFIG->GetTouzijihuaCfg().GetRewardCfg(TOUZI_JIHUA_PLAN_TYPE_2, seq);
			if (NULL == cfg_item_2)
			{
				return false;
			}

			if (m_role->GetLevel() < cfg_item_2->need_level)
			{
				m_role->NoticeNum(errornum::EN_TOUZIJIHUA_FETCH_REWARD_LIMIT);
				return false;
			}

			if (0 != (m_plan_2_reward_flag & (1 << seq)))
			{
				m_role->NoticeNum(errornum::EN_TOUZIJIHUA_HAS_FETCH_REWARD);
				return false;
			}

			*reward_gold = cfg_item_2->reward_gold;
			*reward_bind_gold = cfg_item_2->reward_gold_bind;

			const TouzijihuaRewardConfig::ConfigItem *cfg_item_1 = LOGIC_CONFIG->GetTouzijihuaCfg().GetRewardCfg(TOUZI_JIHUA_PLAN_TYPE_1, seq);
			if (NULL != cfg_item_1)
			{
				if (0 != (m_plan_1_reward_flag & (1 << seq)))
				{
					*reward_gold -= cfg_item_1->reward_gold;
					*reward_bind_gold -=  cfg_item_1->reward_gold_bind;
				}
				else
				{
					const TouzijihuaRewardConfig::ConfigItem *cfg_item_0 = LOGIC_CONFIG->GetTouzijihuaCfg().GetRewardCfg(TOUZI_JIHUA_PLAN_TYPE_0, seq);
					if (NULL != cfg_item_0)
					{
						if (0 != (m_plan_0_reward_flag & (1 << seq)))
						{
							*reward_gold -= cfg_item_0->reward_gold;
							*reward_bind_gold -= cfg_item_0->reward_gold_bind;
						}
					}
				}
			}
			else
			{
				const TouzijihuaRewardConfig::ConfigItem *cfg_item_0 = LOGIC_CONFIG->GetTouzijihuaCfg().GetRewardCfg(TOUZI_JIHUA_PLAN_TYPE_0, seq);
				if (NULL != cfg_item_0)
				{
					if (0 != (m_plan_0_reward_flag & (1 << seq)))
					{
						*reward_gold -= cfg_item_0->reward_gold;
						*reward_bind_gold -= cfg_item_0->reward_gold_bind;
					}
				}
			}

			return true;
		}
		break;

	case TOUZI_JIHUA_PLAN_TYPE_3:
		{
			if (0 == m_active_plan_3)
			{
				m_role->NoticeNum(errornum::EN_TOUZIJIHUA_PLAN_NOT_ACTIVE);
				return false;
			}

			const TouzijihuaRewardConfig::ConfigItem *cfg_item_3 = LOGIC_CONFIG->GetTouzijihuaCfg().GetRewardCfg(TOUZI_JIHUA_PLAN_TYPE_3, seq);
			if (NULL == cfg_item_3)
			{
				return false;
			}

			if (m_role->GetLevel() < cfg_item_3->need_level)
			{
				m_role->NoticeNum(errornum::EN_TOUZIJIHUA_FETCH_REWARD_LIMIT);
				return false;
			}

			if (0 != (m_plan_3_reward_flag & (1 << seq)))
			{
				m_role->NoticeNum(errornum::EN_TOUZIJIHUA_HAS_FETCH_REWARD);
				return false;
			}

			*reward_gold = cfg_item_3->reward_gold;
			*reward_bind_gold = cfg_item_3->reward_gold_bind;

			const TouzijihuaRewardConfig::ConfigItem *cfg_item_2 = LOGIC_CONFIG->GetTouzijihuaCfg().GetRewardCfg(TOUZI_JIHUA_PLAN_TYPE_2, seq);
			const TouzijihuaRewardConfig::ConfigItem *cfg_item_1 = LOGIC_CONFIG->GetTouzijihuaCfg().GetRewardCfg(TOUZI_JIHUA_PLAN_TYPE_1, seq);
			const TouzijihuaRewardConfig::ConfigItem *cfg_item_0 = LOGIC_CONFIG->GetTouzijihuaCfg().GetRewardCfg(TOUZI_JIHUA_PLAN_TYPE_0, seq);
			if (NULL != cfg_item_2)
			{
				if (0 != (m_plan_2_reward_flag & (1 << seq)))
				{
					*reward_gold -= cfg_item_2->reward_gold;
					*reward_bind_gold -= cfg_item_2->reward_gold_bind;
				}
				else if (NULL != cfg_item_1)
				{
					if (0 != (m_plan_1_reward_flag & (1 << seq)))
					{
						*reward_gold -= cfg_item_1->reward_gold;
						*reward_bind_gold -= cfg_item_1->reward_gold_bind;
					}
					else if (NULL != cfg_item_0)
					{
						if (0 != (m_plan_0_reward_flag & (1 << seq)))
						{
							*reward_gold -= cfg_item_0->reward_gold;
							*reward_bind_gold -= cfg_item_0->reward_gold_bind;
						}
					}
				}
			}
			else if (NULL != cfg_item_1)
			{
				if (0 != (m_plan_1_reward_flag & (1 << seq)))
				{
					*reward_gold -= cfg_item_1->reward_gold;
					*reward_bind_gold -= cfg_item_1->reward_gold_bind;
				}
				else if (NULL != cfg_item_0)
				{
					if (0 != (m_plan_0_reward_flag & (1 << seq)))
					{
						*reward_gold -= cfg_item_0->reward_gold;
						*reward_bind_gold -= cfg_item_0->reward_gold_bind;
					}
				}
			}
			else if (NULL != cfg_item_0)
			{
				if (0 != (m_plan_0_reward_flag & (1 << seq)))
				{
					*reward_gold -= cfg_item_0->reward_gold;
					*reward_bind_gold -= cfg_item_0->reward_gold_bind;
				}
			}

			return true;
		}
		break;

	default:
		return false;
	}

	return false;
}

void Touzijihua::OnBuyPlan(int plan_type)
{
	if (plan_type < TOUZI_JIHUA_PLAN_TYPE_0 || plan_type >= TOUZI_JIHUA_PLAN_MAX_TYPE)
	{
		return;
	}

	if (plan_type <= m_active_highest_plan)
	{
		return;
	}

	JoinLimitUnit limit_unit;
	bool is_limit = LOGIC_CONFIG->GetJoinLimitConfig().GetOtherJoinLimit(JLMT_OTHER_TOUZHIJIHUA, &limit_unit);
	if (is_limit && m_role->GetLevel() > limit_unit.level)
	{
		m_role->NoticeNum(errornum::EN_TOUZIJIHUA_LEVEL_LIMIT);
		return;
	}
 
 	if (m_role->GetSublock()->CheckHasLock())
 	{
 		return;
 	}
 
 	if (m_role->GetKnapsack()->IsLock())
 	{
 		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
 		return;
 	}

 	switch (plan_type)
 	{
 	case TOUZI_JIHUA_PLAN_TYPE_0:
 		{
			if (0 != m_active_plan_0)
			{
				m_role->NoticeNum(errornum::EN_TOUZIJIHUA_PLAN_HAS_ACTIVE);
				return;
			}

			int plan_price = LOGIC_CONFIG->GetTouzijihuaCfg().GetPlanPrice(TOUZI_JIHUA_PLAN_TYPE_0);
			if (plan_price <= 0) 
			{
				return;
			}

			if (!m_role->GetKnapsack()->GetMoney()->UseGold(plan_price, "BuyTouzijihua"))
			{
				m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
				return;
			}

			m_active_highest_plan = TOUZI_JIHUA_PLAN_TYPE_0;
 			m_active_plan_0 = 1;
 		}
 		break;
 
 	case TOUZI_JIHUA_PLAN_TYPE_1:
 		{
			if (0 != m_active_plan_1)
			{
				m_role->NoticeNum(errornum::EN_TOUZIJIHUA_PLAN_HAS_ACTIVE);
				return;
			}

			int plan_price = LOGIC_CONFIG->GetTouzijihuaCfg().GetPlanPrice(TOUZI_JIHUA_PLAN_TYPE_1);
			int active_hightest_plan_price = LOGIC_CONFIG->GetTouzijihuaCfg().GetPlanPrice(m_active_highest_plan);
			if (active_hightest_plan_price > 0) 
			{
				plan_price -= active_hightest_plan_price; 
			}

			if (plan_price <= 0) 
			{
				return;
			}

			if (!m_role->GetKnapsack()->GetMoney()->UseGold(plan_price, "BuyTouzijihua"))
			{
				m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
				return;
			}

			m_active_highest_plan = TOUZI_JIHUA_PLAN_TYPE_1;
 			m_active_plan_1 = 1;
 		}
 		break;

	case TOUZI_JIHUA_PLAN_TYPE_2:
		{
			if (0 != m_active_plan_2)
			{
				m_role->NoticeNum(errornum::EN_TOUZIJIHUA_PLAN_HAS_ACTIVE);
				return;
			}

			int plan_price = LOGIC_CONFIG->GetTouzijihuaCfg().GetPlanPrice(TOUZI_JIHUA_PLAN_TYPE_2);
			int active_hightest_plan_price = LOGIC_CONFIG->GetTouzijihuaCfg().GetPlanPrice(m_active_highest_plan);
			if (active_hightest_plan_price > 0) 
			{
				plan_price -= active_hightest_plan_price; 
			}

			if (plan_price <= 0) 
			{
				return;
			}

			if (!m_role->GetKnapsack()->GetMoney()->UseGold(plan_price, "BuyTouzijihua"))
			{
				m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
				return;
			}

			m_active_highest_plan = TOUZI_JIHUA_PLAN_TYPE_2;
			m_active_plan_2 = 1;
		}
		break;

	case TOUZI_JIHUA_PLAN_TYPE_3:
		{
			if (0 != m_active_plan_3)
			{
				m_role->NoticeNum(errornum::EN_TOUZIJIHUA_PLAN_HAS_ACTIVE);
				return;
			}

			int plan_price = LOGIC_CONFIG->GetTouzijihuaCfg().GetPlanPrice(TOUZI_JIHUA_PLAN_TYPE_3);
			int active_hightest_plan_price = LOGIC_CONFIG->GetTouzijihuaCfg().GetPlanPrice(m_active_highest_plan);
			if (active_hightest_plan_price > 0)
			{
				plan_price -= active_hightest_plan_price;
			}

			if (plan_price <= 0)
			{
				return;
			}

			if (!m_role->GetKnapsack()->GetMoney()->UseGold(plan_price, "BuyTouzijihua"))
			{
				m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
				return;
			}

			m_active_highest_plan = TOUZI_JIHUA_PLAN_TYPE_3;
			m_active_plan_3 = 1;
		}
		break;
 	}

	{
		// 传闻
		int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_touzi_level,
			m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp());
		if (length > 0)
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL);
		}
	}
 
 	this->SendInfo();
	m_role->GetRoleGuild()->AddGuildRedPaperAuthority(PAPER_TYPE_BUY_LEVEL_INVEST, plan_type);
 
 	EventHandler::Instance().OnBuyTouzijihua(m_role, plan_type);
 
 	gamelog::g_log_touzijihua.printf(LL_INFO, "OnBuyPlan user[%d, %s] role_level[%d], plan_type[%d]",
 		m_role->GetUID(), m_role->GetName(), m_role->GetLevel(), plan_type);
 
 	ROLE_LOG_QUICK6(LOG_TYPE_TOUZIJIHUA, m_role, plan_type, 0, NULL, NULL);
}

void Touzijihua::OnNewFetchReward(bool is_vip, int day_index)
{
	if (0 == m_buy_time)
	{
		m_role->NoticeNum(errornum::EN_TOUZIJIHUA_PLAN_NOT_ACTIVE);
		return;
	}

	const NewTouzijihuaRewardConfig * cfg = LOGIC_CONFIG->GetTouzijihuaCfg().GetNewRewardCfg(day_index);
	if (NULL == cfg)
	{
		return;
	}

	int now_day_index = GetDayIndex(m_buy_time, EngineAdapter::Instance().Time());
	if (day_index > now_day_index)
	{
		m_role->NoticeNum(errornum::EN_TOUZIJIHUA_TIME_LIMIT);
		return;
	}

	if (m_reward_gold_bind_flag & (1 << day_index))
	{
		m_role->NoticeNum(errornum::EN_TOUZIJIHUA_HAS_FETCH_REWARD);
		return;
	}

	if (m_role->GetKnapsack()->GetMoney()->AddGoldBind(cfg->reward_gold_bind, "Touzijihua"))
	{
		m_reward_gold_bind_flag |= (1 << day_index);
		this->SendInfo();
	}
}

void Touzijihua::OnNewFetchFirstReward()
{
	if (0 == m_buy_time)
	{
		m_role->NoticeNum(errornum::EN_TOUZIJIHUA_PLAN_NOT_ACTIVE);
		return;
	}

	int first_reward = LOGIC_CONFIG->GetTouzijihuaCfg().GetNewPlanFirstReward();
	if (0 >= first_reward)
	{
		return;
	}

	int now_day_index = GetDayIndex(m_buy_time, EngineAdapter::Instance().Time());
	if (0 > now_day_index)
	{
		return;
	}

	if (1 == m_new_plan_first_reward_flag)
	{
		m_role->NoticeNum(errornum::EN_TOUZIJIHUA_HAS_FETCH_REWARD);
		return;
	}

	if (m_role->GetKnapsack()->GetMoney()->AddGoldBind(first_reward, "Touzijihua"))
	{
		m_new_plan_first_reward_flag = 1;
		this->SendInfo();
	}
}

void Touzijihua::OnNewBuy()
{
	if (0 != m_buy_time)
	{
		for(int i = 0; i < MAX_NEW_TOUZIJIHUA_DAY; i++)
		{
			if (0 == (m_reward_gold_bind_flag & 1 << i))
			{
				m_role->NoticeNum(errornum::EN_TOUZIJIHUA_PLAN_HAS_ACTIVE);
				return;
			}
		}

		if (0 == m_new_plan_first_reward_flag)
		{
			m_role->NoticeNum(errornum::EN_TOUZIJIHUA_PLAN_HAS_ACTIVE);
			return;
		}
	}

	if (m_role->GetSublock()->CheckHasLock())
	{
		return;
	}

	if (m_role->GetKnapsack()->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	int plan_price = LOGIC_CONFIG->GetTouzijihuaCfg().GetNewPlanPrice();
	if (plan_price <= 0)
	{
		return;
	}

	if (!m_role->GetKnapsack()->GetMoney()->UseGold(plan_price, "BuyTouzijihuaNew"))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	m_buy_time = (unsigned int)EngineAdapter::Instance().Time();
	m_reward_gold_bind_flag = 0;
	m_new_plan_first_reward_flag = 0;

	m_role->GetRoleGuild()->AddGuildRedPaperAuthority(PAPER_TYPE_BUY_MONTH_INVEST, 0);

	{
		// 传闻
		int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_touzi_week_card,
			m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp());
		if (length > 0)
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL);
		}
	}

	this->SendInfo();
}

void Touzijihua::GmReset()
{
	m_active_plan_0 = 0;
	m_active_plan_1 = 0;
	m_active_plan_2 = 0;
	m_active_plan_3 = 0;
	m_plan_0_reward_flag = 0;
	m_plan_1_reward_flag = 0;
	m_plan_2_reward_flag = 0;
	m_plan_3_reward_flag = 0;

	m_buy_time = 0;
	m_reward_flag = 0;
	m_vip_reward_flag = 0;
	m_reward_gold_bind_flag = 0;
	m_new_plan_first_reward_flag = 0;

	m_active_fb_plan =0;
	m_fb_plan_reward_flag = 0;

	m_active_boss_plan = 0;
	m_boss_plan_reward_flag = 0;
	m_kill_boss_num = 0;

	this->SendInfo();
	this->SendFbBossInfo();
}

void Touzijihua::GmAddDay(int day)
{
	if (m_buy_time > 0)
	{
		m_buy_time -= (day * 24 * 3600);
		this->SendInfo();
	}
}

bool Touzijihua::OnBuyFoundation(int chongzhi_num)
{
	const FoundationConfig *cfg = LOGIC_CONFIG->GetTouzijihuaCfg().GetFoundationCfg(chongzhi_num);
	if (nullptr == cfg)
	{
		return false;
	}

	if (cfg->seq < 0 || cfg->seq >= TouzijihuaConfig::MAX_FOUNDATION_TYPE)
	{
		return false;
	}

	if (m_role->GetLevel() < cfg->active_level_min || m_role->GetLevel() > cfg->active_level_max)
	{
		m_role->NoticeNum(errornum::EN_TOUZIJIHUA_FOUNDATION_BUY_LEVEL_LIMIT);
		return false;
	}

	if (m_foundation_next_reward_index[cfg->seq] > 0)
	{
		m_role->NoticeNum(errornum::EN_TOUZIJIHUA_FOUNDATION_HAD_BUY);
		return false;
	}

	m_foundation_next_reward_index[cfg->seq] = 1;

	m_role->GetKnapsack()->GetMoney()->AddGold(cfg->chongzhi_gold, "OnBuyFoundation");

	this->SendInfo();

	return true;
}

void Touzijihua::OnFetchFoundationReward(int reward_index)
{
	if (reward_index < 0 || reward_index >= TouzijihuaConfig::MAX_FOUNDATION_TYPE)
	{
		return;
	}

	int reward_gold = LOGIC_CONFIG->GetTouzijihuaCfg().GetFoundationRewardGold(reward_index, m_foundation_next_reward_index[reward_index] - 1, m_role->GetLevel());
	if (reward_gold <= 0)
	{
		m_role->NoticeNum(errornum::EN_TOUZIJIHUA_FOUNDATION_FETCH_LEVEL_LIMIT);
		return;
	}

	++m_foundation_next_reward_index[reward_index];

	m_role->GetKnapsack()->GetMoney()->AddGold(reward_gold, "OnFetchFoundationReward");

	this->SendInfo();
}

void Touzijihua::OnBuyFbPlan()
{
	if (0 != m_active_fb_plan)
	{
		return;
	}

	int fb_plan_price = LOGIC_CONFIG->GetTouzijihuaCfg().GetFbPlanPrice();
	if (fb_plan_price <= 0)
	{
		return;
	}

	if (!m_role->GetKnapsack()->GetMoney()->UseGold(fb_plan_price, "OnBuyFbPlan"))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	m_active_fb_plan = 1;

	{
		// 传闻
		int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_touzi_fb_plan,
			m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp());
		if (length > 0)
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL);
		}
	}

	this->CheckActiveFbReward();
	this->OnFetchFbPlanReward(0,true);
	this->SendFbBossInfo();
	
	TianShuXunZhu::CheckShiXueTianShu(m_role, CHENGZHANG_TIANSHU_TYPE_TOUTZHIJIHUA);

	gamelog::g_log_touzijihua.printf(LL_INFO, "OnBuyFbPlan user[%d, %s] role_level[%d], useGold[%d]",
		m_role->GetUID(), m_role->GetName(), m_role->GetLevel(), fb_plan_price);
}

void Touzijihua::OnFetchFbPlanReward(int index, bool is_buy)
{
	const FbPlanConfig::FbPlanItem * plan_item = LOGIC_CONFIG->GetTouzijihuaCfg().GetFbPlanCfgItem(index);
	if (plan_item == NULL)
	{
		return;
	}

	if (0 !=(m_fb_plan_reward_flag & (1 << index)))
	{
		m_role->NoticeNum(errornum::EN_TOUZIJIHUA_HAS_FETCH_REWARD);
		return;
	}

	if (0 == (m_active_fb_plan & (1 << index)))
	{
		m_role->NoticeNum(errornum::EN_TOUZIJIHUA_PLAN_NOT_ACTIVE);
		return;
	}

	m_role->GetKnapsack()->GetMoney()->AddGoldBind(plan_item->reward_gold_bind, "OnFetchFbPlanReward");

	m_fb_plan_reward_flag |= (1 << index);

	if (!is_buy)
	{
		this->SendFbBossInfo();
	}
	gamelog::g_log_touzijihua.printf(LL_INFO, "OnFetchFbPlanReward user[%d, %s] role_level[%d], addbindGold[%d]",
		m_role->GetUID(), m_role->GetName(), m_role->GetLevel(), plan_item->reward_gold_bind);

}
void Touzijihua::OnBuyBossPlan()
{
	if (0 != m_active_boss_plan)
	{
		return ;
	}
	
	int boss_plan_price = LOGIC_CONFIG->GetTouzijihuaCfg().GetBossPlanPrice();
	if (boss_plan_price <= 0)
	{
		return;
	}

	if (!m_role->GetKnapsack()->GetMoney()->UseGold(boss_plan_price, "OnBuyBossPlan"))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	m_active_boss_plan = 1;

	{
		// 传闻
		int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_touzi_boss_plan,
			m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp());
		if (length > 0)
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL);
		}
	}

	this->CheckActiveBossReward();
	this->OnFetchBossPlanReward(0, true);
	this->SendFbBossInfo();
	TianShuXunZhu::CheckShiXueTianShu(m_role, CHENGZHANG_TIANSHU_TYPE_TOUTZHIJIHUA);

	gamelog::g_log_touzijihua.printf(LL_INFO, "OnBuyBossPlan user[%d, %s] role_level[%d], useGold[%d]",
		m_role->GetUID(), m_role->GetName(), m_role->GetLevel(), boss_plan_price);


}
void Touzijihua::OnFetchBossPlanReward(int index, bool is_buy)
{
	const BossPlanConfig::BossPlanItem * plan_item = LOGIC_CONFIG->GetTouzijihuaCfg().GetBossPlanCfgItem(index);
	if (plan_item == NULL)
	{
		return;
	}

	if (0 != (m_boss_plan_reward_flag & (1 << index)))
	{
		m_role->NoticeNum(errornum::EN_TOUZIJIHUA_HAS_FETCH_REWARD);
		return;
	}
	if (0 == (m_active_boss_plan & (1 << index)))
	{
		m_role->NoticeNum(errornum::EN_TOUZIJIHUA_PLAN_NOT_ACTIVE);
		return;
	}

	m_role->GetKnapsack()->GetMoney()->AddGoldBind(plan_item->reward_gold_bind, "OnFetchBossPlanReward");

	m_boss_plan_reward_flag |= (1 << index);

	if (!is_buy)
	{
		this->SendFbBossInfo();
	}
	gamelog::g_log_touzijihua.printf(LL_INFO, "OnFetchBossPlanReward user[%d, %s] role_level[%d], addbindGold[%d]",
		m_role->GetUID(), m_role->GetName(), m_role->GetLevel(), plan_item->reward_gold_bind);
}
void Touzijihua::OnPassFb()
{
	this->CheckActiveFbReward();
	this->SendFbBossInfo();
}

void Touzijihua::CheckActiveFbReward()
{
	if ( 0 == (m_active_fb_plan & 1))
	{
		return;
	}

	int pass_level = m_role->GetFBEquip()->GetNeqFBPassMaxLevel();
	int index = LOGIC_CONFIG->GetTouzijihuaCfg().GetFbPlanIndex(pass_level);
	if (index <= 0)
	{
		return;
	}

	for (; index > 0; --index)
	{
		m_active_fb_plan |= (1 << index);
	}
	return;
}

void Touzijihua::OnKillBoss()
{
	m_kill_boss_num++;
	this->CheckActiveBossReward();
	this->SendFbBossInfo();
}

void Touzijihua::CheckActiveBossReward()
{
	if (0 == (m_active_boss_plan & 1))
	{
		return ;
	}

	int index = LOGIC_CONFIG->GetTouzijihuaCfg().GetBossPlanIndex(m_kill_boss_num);
	if (index <= 0)
	{
		return;
	}
	for (; index > 0; --index)
	{
		m_active_boss_plan |= (1 << index);
	}

	return;
}

void Touzijihua::OnBuyShenyuBossPlan()
{
	if (0 != m_active_shenyu_boss_plan)
	{
		return;
	}

	int shenyu_boss_plan_price = LOGIC_CONFIG->GetTouzijihuaCfg().GetShenyuBossPlanPrice();
	if (shenyu_boss_plan_price <= 0)
	{
		return;
	}

	if (!m_role->GetKnapsack()->GetMoney()->UseGold(shenyu_boss_plan_price, "OnBuyShenyuBossPlan"))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	m_active_shenyu_boss_plan = 1;

	{
		// 传闻
		int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_touzi_shenyu_boss_plan,
			m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp());
		if (length > 0)
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL);
		}
	}

	this->CheckActiveShenyuBossReward();
	this->OnFetchShenyuBossPlanReward(0, true);
	this->SendFbBossInfo();

	gamelog::g_log_touzijihua.printf(LL_INFO, "OnBuyShenyuBossPlan user[%d, %s] role_level[%d], useGold[%d]",
		m_role->GetUID(), m_role->GetName(), m_role->GetLevel(), shenyu_boss_plan_price);
}

void Touzijihua::OnFetchShenyuBossPlanReward(int index, bool is_buy /*= false*/)
{
	const ShenyuBossPlanConfig::BossPlanItem * plan_item = LOGIC_CONFIG->GetTouzijihuaCfg().GetShenyuBossPlanCfgItem(index);
	if (plan_item == NULL)
	{
		return;
	}

	if (0 != (m_shenyu_boss_plan_reward_flag & (1 << index)))
	{
		m_role->NoticeNum(errornum::EN_TOUZIJIHUA_HAS_FETCH_REWARD);
		return;
	}
	if (0 == (m_active_shenyu_boss_plan & (1 << index)))
	{
		m_role->NoticeNum(errornum::EN_TOUZIJIHUA_PLAN_NOT_ACTIVE);
		return;
	}

	m_role->GetKnapsack()->GetMoney()->AddGoldBind(plan_item->reward_gold_bind, "OnFetchShenyuBossPlanReward");

	m_shenyu_boss_plan_reward_flag |= (1 << index);

	if (!is_buy)
	{
		this->SendFbBossInfo();
	}
	gamelog::g_log_touzijihua.printf(LL_INFO, "OnFetchShenyuBossPlanReward user[%d, %s] role_level[%d], addbindGold[%d]",
		m_role->GetUID(), m_role->GetName(), m_role->GetLevel(), plan_item->reward_gold_bind);
}

void Touzijihua::OnKillShenyuBoss()
{
	m_kill_shenyu_boss_num++;
	this->CheckActiveShenyuBossReward();
	this->SendFbBossInfo();
}

void Touzijihua::CheckActiveShenyuBossReward()
{
	if (0 == (m_active_shenyu_boss_plan & 1))
	{
		return;
	}

	int index = LOGIC_CONFIG->GetTouzijihuaCfg().GetShenyuBossPlanIndex(m_kill_shenyu_boss_num);
	if (index <= 0)
	{
		return;
	}
	for (; index > 0; --index)
	{
		m_active_shenyu_boss_plan |= (1 << index);
	}

	return;
}

void Touzijihua::SendFbBossInfo()
{
	Protocol::SCTouzijihuaFbBossInfo info;
	info.active_fb_plan = m_active_fb_plan;
	info.fb_plan_reward_flag = m_fb_plan_reward_flag;
	info.fb_pass_level = m_role->GetFBEquip()->GetNeqFBPassMaxLevel();
	info.active_boss_plan = m_active_boss_plan;
	info.boss_plan_reward_flag = m_boss_plan_reward_flag;
	info.kill_boss_num = m_kill_boss_num;
	info.active_shenyu_boss_plan = m_active_shenyu_boss_plan;
	info.shenyu_boss_plan_reward_flag = m_shenyu_boss_plan_reward_flag;
	info.kill_shenyu_boss_num = m_kill_shenyu_boss_num;

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&info, sizeof(info));

	//printf("---fbbossplan:fb:%d,%d boss: %d,%d,%d", m_active_fb_plan, m_fb_plan_reward_flag, m_active_boss_plan, m_boss_plan_reward_flag, m_kill_boss_num);
}