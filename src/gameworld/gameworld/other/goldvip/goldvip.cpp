#include "goldvip.hpp"

#include "gamelog.h"
#include "engineadapter.h"
#include "world.h"
#include "protocal/msgrole.h"

#include "globalconfig/globalconfig.h"
#include "obj/character/role.h"
#include "servercommon/noticenum.h"
#include "servercommon/errornum.h"
#include "servercommon/string/gameworldstr.h"
#include "other/route/mailroute.hpp"
#include "scene/scene.h"
#include "other/event/eventhandler.hpp"

#include "other/route/mailroute.hpp"
#include "servercommon/string/globalstr.h"
#include "servercommon/string/gameworldstr.h"

#include "other/daycounter/daycounter.hpp"

#include "item/itempool.h"
#include "config/logicconfigmanager.hpp"
#include "config/guildconfig.hpp"
#include "other/goldvip/goldvipconfig.hpp"
#include "scene/activityshadow/activityshadow.hpp"
#include "global/guild/roleguild.hpp"
#include "item/knapsack.h"
#include "other/roleactivity/roleactivity.hpp"

GoldVip::GoldVip()
	: m_role(NULL)
{
}

GoldVip::~GoldVip()
{

}

void GoldVip::Init(Role *role, const GoldVipParam &gold_vip_param)
{
	m_role = role; 
	m_gold_vip_param = gold_vip_param;
	m_gold_vip_param.day_score = (unsigned short)m_gold_vip_param.day_score;
	
}

void GoldVip::GetOtherInitParam(GoldVipParam *gold_vip_param)
{
	*gold_vip_param = m_gold_vip_param;
}

void GoldVip::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	// 日期不一致就往下判断，做出相应的改变
	if (old_dayid != now_dayid)
	{
		memset(m_gold_vip_param.gold_vip_shop_counts_list, 0, sizeof(m_gold_vip_param.gold_vip_shop_counts_list));

		// 未激活或者以超过激活时间截，则相应数据置0
		if (!this->IsvalidGoldVip())
		{
			m_gold_vip_param.gold_vip_active_timestamp = 0;

			if (1 == m_gold_vip_param.is_active_return_reward)
			{
				m_gold_vip_param.is_active_return_reward = 0;
				//m_gold_vip_param.can_fetch_return_reward = 1;
			}
		}

		// 如果能领取激活奖励，则激活相应数据值
		if(this->IsCanFetchReward())
		{
			m_gold_vip_param.can_fetch_return_reward = 1;
		}

		m_gold_vip_param.day_score = 0;
	
		this->SendGoldVipInfo();
	}
}

void GoldVip::OnConvertShop(int seq)
{
	if (seq < 0 || seq >= GOLD_VIP_CONVERT_SHOP_MAX_COUNT)
	{
		return;
	}

	if (seq > m_gold_vip_param.shop_active_grade_flag + 1)
	{
		return;
	}

	if (!this->IsvalidGoldVip())
	{
		m_role->NoticeNum(errornum::EN_GOLD_VIP_AUTH_LIMIT);
		return;
	}

	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	const GoldVipShopCfg::ConfigItem *cfg_item = LOGIC_CONFIG->GetGoldVipCfg().GetGoldVipShopRewardCfg(seq);
	if (NULL == cfg_item)
	{
		return;
	}

	const GoldVipActiveCfg *active_cfg = LOGIC_CONFIG->GetGoldVipCfg().GetGoldVipActiveCfg();
	if (NULL == active_cfg)
	{
		return;
	}

	if (m_role->GetRoleActivity()->GetDayChongzhi() < active_cfg->active_convert_gold)
	{
		m_role->NoticeNum(errornum::EN_OPEN_GAME_ACTIVITY_TOTAL_CHONGZHI_NOT_ENOUGH);
		return;
	}
	
	if (cfg_item->limit_times > 0 && m_gold_vip_param.gold_vip_shop_counts_list[seq] >= cfg_item->limit_times)
	{
		m_role->NoticeNum(errornum::NT_CONVERTSHOP_CONVERT_COUNT_LIMIT);
		return;
	}

	int total_cost = cfg_item->consume_val * LOGIC_CONFIG->GetGoldVipCfg().GetMultipleCost(seq, m_gold_vip_param.gold_vip_shop_counts_list[seq]);

	if (cfg_item->consume_type == 1)
	{
		if (total_cost > m_gold_vip_param.day_score)
		{
			m_role->NoticeNum(errornum::EN_SCORE_NOT_ENOUGH);
			return;
		}
		m_gold_vip_param.day_score -= total_cost;
	}

	if (cfg_item->consume_type == 2)
	{
		if (!m_role->GetKnapsack()->GetMoney()->UseGold(total_cost, "GoldVipConvertShop"))
		{
			m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
			return;
		}
	}

	m_gold_vip_param.gold_vip_shop_counts_list[seq] += 1;

	if (seq > m_gold_vip_param.shop_active_grade_flag)
	{
		m_gold_vip_param.shop_active_grade_flag += 1;
	}

	m_role->GetKnapsack()->Put(cfg_item->reward_item, PUT_REASON_GOLD_VIP_CONVERT_ITEM);

	m_role->NoticeNum(noticenum::NT_CONVERTSHOP_CONVERT_SUCC);

	this->SendGoldVipInfo();
}

void GoldVip::OnActiveGoldVip()
{
	if (0 != m_gold_vip_param.gold_vip_active_timestamp)
	{
		return;
	}

	const GoldVipActiveCfg *active_cfg = LOGIC_CONFIG->GetGoldVipCfg().GetGoldVipActiveCfg();
	if (NULL == active_cfg)
	{
		return;
	}

	if (m_role->GetLevel() < active_cfg->need_level)
	{
		m_role->NoticeNum(errornum::EN_XIANMENGZHAN_ROLE_LEVEL_LIMIT);
		return;
	}

	if (!m_role->GetKnapsack()->GetMoney()->UseGold(active_cfg->need_gold, "GoldVipActive"))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	m_gold_vip_param.gold_vip_active_timestamp = (unsigned int)EngineAdapter::Instance().Time();
	m_gold_vip_param.is_active_return_reward = 1;

	m_role->GetRoleGuild()->AddGuildRedPaperAuthority(PAPER_TYPE_BUY_GOLD_VIP, 0);

	m_role->NoticeNum(noticenum::NT_GOLD_VIP_ACTIVE_SUCC);

	// 激活传闻
	int len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_gold_vip_active_content, m_role->GetUID(), m_role->GetName(),
		(int)m_role->GetCamp());
	if (len > 0) 
	{
		World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, len, SYS_MSG_CENTER_NOTICE);
	}

	this->SendGoldVipInfo();
}

void GoldVip::OnFetchRetrunReward()
{
	if (!this->IsvalidGoldVip())
	{
		return;
	}

	if (!this->IsCanFetchReward())
	{
		return;
	}

	if (m_gold_vip_param.can_fetch_return_reward != 1)
	{
		return;
	}

	const GoldVipActiveCfg *active_cfg = LOGIC_CONFIG->GetGoldVipCfg().GetGoldVipActiveCfg();
	if (NULL == active_cfg)
	{
		return;
	}

	m_role->GetKnapsack()->GetMoney()->AddGoldBind(active_cfg->return_gold, "GoldVipRetrunReward");

	m_gold_vip_param.can_fetch_return_reward = 0;
	this->SendGoldVipInfo();
}

void GoldVip::CheckAddScore(int chongzhi_gold)
{
	if (!this->IsvalidGoldVip())
	{
		return;
	}

	const GoldVipActiveCfg *active_cfg = LOGIC_CONFIG->GetGoldVipCfg().GetGoldVipActiveCfg();
	if (NULL == active_cfg)
	{
		return;
	}

	if (active_cfg->convert_rate > 0)
	{
		m_gold_vip_param.day_score += chongzhi_gold / active_cfg->convert_rate;
		this->SendGoldVipInfo();
	}

	return;
}

// 是否激活黄金会员，已激活或者激活时间截未超过限定（现在是永久）返回true,否则返回false
bool GoldVip::IsvalidGoldVip()
{
	if (m_gold_vip_param.gold_vip_active_timestamp == 0)
	{
		return false;
	}

	int interval_days = GetDayIndex(m_gold_vip_param.gold_vip_active_timestamp, EngineAdapter::Instance().Time()) + 1;
	const GoldVipActiveCfg *active_cfg = LOGIC_CONFIG->GetGoldVipCfg().GetGoldVipActiveCfg();
	if (NULL == active_cfg)
	{
		return false;
	}

	if (active_cfg->continue_days < interval_days)
	{
		return false;
	}

	return true;
}


void GoldVip::SendGoldVipInfo()
{
	static Protocol::SCGoldVipInfo gvi;

	memcpy(gvi.gold_vip_shop_counts_list, m_gold_vip_param.gold_vip_shop_counts_list, sizeof(gvi.gold_vip_shop_counts_list));
	gvi.gold_vip_active_timestamp = m_gold_vip_param.gold_vip_active_timestamp;
	gvi.day_score = m_gold_vip_param.day_score;
	gvi.shop_active_grade_flag = m_gold_vip_param.shop_active_grade_flag;
	gvi.can_fetch_return_reward = m_gold_vip_param.can_fetch_return_reward;
	gvi.is_not_first_fetch_return_reward = m_gold_vip_param.is_not_first_fetch_return_reward;
	gvi.reserver_1 = 0;
	
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&gvi, sizeof(gvi));
}

// 判断是否可以领取奖励,不可领取返回false，可领取返回true
bool GoldVip::IsCanFetchReward()
{
	if (m_gold_vip_param.gold_vip_active_timestamp == 0)
	{
		return false;
	}

	int interval_days = GetDayIndex(m_gold_vip_param.gold_vip_active_timestamp, EngineAdapter::Instance().Time()) + 1;
	const GoldVipActiveCfg *active_cfg = LOGIC_CONFIG->GetGoldVipCfg().GetGoldVipActiveCfg();
	if (NULL == active_cfg)
	{
		return false;
	}

	// 如果激活后没有超过领取奖励时间，不能领取
	if (active_cfg->count_down > interval_days)
	{
		return false;
	}

	return true;
}
