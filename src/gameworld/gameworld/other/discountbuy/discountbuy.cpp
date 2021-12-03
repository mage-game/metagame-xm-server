#include "discountbuy.hpp"

#include "engineadapter.h"
#include "obj/character/role.h"
#include "config/logicconfigmanager.hpp"
#include "other/discountbuy/discountbuyconfig.hpp"
#include "servercommon/errornum.h"
#include "scene/activityshadow/activityshadow.hpp"
#include "world.h"
#include "servercommon/string/gameworldstr.h"
#include "item/knapsack.h"
#include "other/sublock/sublock.hpp"
#include "config/sharedconfig/sharedconfig.h"
#include "servercommon/serverconfig/openserverconfig.hpp"
#include "servercommon/serverconfig/crossconfig.hpp"
#include "other/route/mailroute.hpp"

DiscountBuy::DiscountBuy()
	: m_role(NULL), m_last_broadcast_time(0)
{
	
}

DiscountBuy::~DiscountBuy()
{
}

void DiscountBuy::Init(Role *role, const DiscountBuyParam &param)
{
	m_role = role;
	m_param = param;
}

void DiscountBuy::OnLogin()
{
	this->CheckDiscountBuyOpen();
}

void DiscountBuy::GetInitParam(DiscountBuyParam *param)
{
	if (NULL != param)
	{
		*param = m_param;
	}
}

void DiscountBuy::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	if (old_dayid != now_dayid)
	{
		this->CheckDiscountBuyOpen();
	}
}

void DiscountBuy::OnRoleLevelChange(int to_level)
{
	this->CheckDiscountBuyOpen();
}

void DiscountBuy::OnBuy(int seq)
{
	const DiscountBuyItemConfig::ConfigItem * cfg_item = LOGIC_CONFIG->GetDiscountBuyCfg().GetItemCfg(seq);
	if (NULL == cfg_item)
	{
		return;
	}

	if (cfg_item->phase < 0 || cfg_item->phase >= DISCOUNT_BUY_PHASE_MAX_COUNT ||
		cfg_item->item_seq < 0 || cfg_item->item_seq >= DISCOUNT_BUY_ITEM_PER_PHASE || cfg_item->price < 0)		//策划需求，一折抢购可配置免费，2019.3.29
	{
		return;
	}

	DiscountBuyPhase &phase = m_param.phase_list[cfg_item->phase];

	if (EngineAdapter::Instance().Time() > phase.close_timestamp)
	{
		m_role->NoticeNum(errornum::EN_DISCOUNT_BUY_CLOSE_TIME_LIMIT);
		return;
	}

	if (phase.buy_count_list[cfg_item->item_seq] >= cfg_item->buy_limit_count)
	{
		m_role->NoticeNum(errornum::EN_DISCOUNT_BUY_BUY_COUNT_LIMIT);
		return;
	}

	if (m_role->GetSublock()->CheckHasLock())
	{
		return;
	}

	if (!m_role->GetKnapsack()->GetMoney()->GoldMoreThan(cfg_item->price))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	if (cfg_item->price > 0)		//策划需求，一折抢购可配置免费，2019.3.29
	{
		if (!m_role->GetKnapsack()->GetMoney()->UseGold(cfg_item->price, "DiscountBuyItem"))
		{
			m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
			return;
		}
	}

	++ phase.buy_count_list[cfg_item->item_seq];

	m_role->GetKnapsack()->Put(cfg_item->item, PUT_REASON_DISCOUNT_BUY);

	{
		if (cfg_item->price > 0)	//策划需求，免费的不传闻
		{
			// 传闻
			static const unsigned int INTERVAL = 120;
			if (static_cast<unsigned int>(EngineAdapter::Instance().Time()) > m_last_broadcast_time + INTERVAL)
			{
				int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_discount_buy,
					m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), cfg_item->phase);
				if (length > 0) 
				{
					World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE);
				}

				m_last_broadcast_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
			}
		}
	}

	gamelog::g_log_other.printf(LL_INFO, "DiscountBuy user[%d, %s], level[%d], use_gold[%d], seq[%d], reward_item[%s]", 
		m_role->GetUID(), m_role->GetName(), m_role->GetLevel(), cfg_item->price, seq, ItemConfigDataLog(&cfg_item->item, NULL));
	
	ROLE_LOG_QUICK6(LOG_TYPE_DISCOUNT_BUY, m_role, seq, cfg_item->price, ItemConfigDataLog(&cfg_item->item, NULL), NULL);

	this->SendDiscountBuyInfo();
}

void DiscountBuy::SendDiscountBuyInfo()
{
	Protocol::SCDiscountBuyInfo dbi;

	for (int i = 0; i < DISCOUNT_BUY_PHASE_MAX_COUNT; ++ i)
	{
		dbi.phase_list[i].close_timestamp = m_param.phase_list[i].close_timestamp;

		for (int j = 0; j < DISCOUNT_BUY_ITEM_PER_PHASE; ++ j)
		{
			dbi.phase_list[i].buy_count_list[j] = m_param.phase_list[i].buy_count_list[j];
		}
	}

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&dbi, sizeof(dbi));
}

void DiscountBuy::CheckDiscountBuyOpen()
{
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}
	int phase_cfg_count = LOGIC_CONFIG->GetDiscountBuyCfg().GetPhaseCfgCount();

	bool need_send = false;

	for (int i = 0; i < phase_cfg_count && i < DISCOUNT_BUY_PHASE_MAX_COUNT; ++ i)
	{
		if (m_param.phase_list[i].close_timestamp > 0) // 已经激活，不需要再计算
		{
			continue;
		}

		const DiscountBuyPhaseConfig::ConfigItem *cfg_item = LOGIC_CONFIG->GetDiscountBuyCfg().GetPhaseCfg(i);
		if (NULL == cfg_item)
		{
			continue;
		}
		
		int day_index = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerDayIndex() + 1;
		
		if (cfg_item->begin_open_day > day_index || cfg_item->end_open_day < day_index)
		{
			continue;
		}

		if (m_role->GetLevel() < cfg_item->active_level)			// 后续的没必要继续检查，读配置的时候保证激活等级是递增的
		{
			break;
		}

		long long now = static_cast<long long>(EngineAdapter::Instance().Time());

		m_param.phase_list[i].close_timestamp = static_cast<unsigned int>(now + cfg_item->last_time_s);

		need_send = true;

		this->SendNoticeMail(i);
	}

	if (need_send)
	{
		this->SendDiscountBuyInfo();
	}
}

void DiscountBuy::SendNoticeMail(int phase)
{
	const DiscountBuyPhaseConfig::ConfigItem *cfg_item = LOGIC_CONFIG->GetDiscountBuyCfg().GetPhaseCfg(phase);
	if (NULL == cfg_item)
	{
		return;
	}

	static MailContentParam contentparam; contentparam.Reset();
	int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_discount_buy_mail_content, phase);
	if (length <= 0)
	{
		return;
	}

	contentparam.item_list[0].item_id = cfg_item->notice_mail_item.item_id;
	contentparam.item_list[0].is_bind = cfg_item->notice_mail_item.is_bind;
	contentparam.item_list[0].num = cfg_item->notice_mail_item.num;

	MailRoute::MailToRole(m_role, SYSTEM_MAIL_REASON_INVALID, contentparam);
}
