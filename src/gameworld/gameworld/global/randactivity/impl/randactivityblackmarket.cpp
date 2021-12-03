#include "randactivityblackmarket.hpp"
#include "global/randactivity/randactivitymanager.hpp"

#include "config/logicconfigmanager.hpp"
#include "config/activityconfig/randactivityconfig.hpp"
#include "item/itempool.h"
#include "world.h"
#include "servercommon/string/gameworldstr.h"
#include "other/route/mailroute.hpp"
#include "obj/character/role.h"
#include "item/knapsack.h"
#include "servercommon/errornum.h"
#include "servercommon/noticenum.h"

RandActivityBlackMarket::RandActivityBlackMarket(RandActivityManager *rand_activity_manager)
: RandActivity(rand_activity_manager, RAND_ACTIVITY_TYPE_BLACK_MARKET)
{

}

RandActivityBlackMarket::~RandActivityBlackMarket()
{

}

void RandActivityBlackMarket::Init(const RandActivityData &data)
{
	RandActivity::Init(data);

	UNSTD_STATIC_CHECK(sizeof(m_data_param) <= sizeof(data.data));

	ActivityDataParam* pTemp = (ActivityDataParam*)data.data;
	m_data_param = *pTemp;
}

void RandActivityBlackMarket::GetInitParam(RandActivityData *data)
{
	RandActivity::GetInitParam(data);

	UNSTD_STATIC_CHECK(sizeof(m_data_param) <= sizeof(data->data));

	ActivityDataParam* pTemp = (ActivityDataParam*)data->data;
	*pTemp = m_data_param;
}

void RandActivityBlackMarket::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	if (old_dayid == now_dayid) return;

	this->CheckGiveItem();
	this->RefreshItemInfoList();
}

void RandActivityBlackMarket::OnSpecialActivityStatusChange(int from_status, int to_status)
{
	RandActivity::OnSpecialActivityStatusChange(from_status, to_status);

	if (ACTIVITY_STATUS_OPEN == to_status)
	{
		this->RefreshItemInfoList();
	}
	else if (ACTIVITY_STATUS_CLOSE == to_status)
	{
		this->CheckGiveItem();
	}
}

void RandActivityBlackMarket::GetItemInfoList(RABlackMarketItemInfo item_info_list[RA_BLACK_MARKET_MAX_ITEM_COUNT])
{
	if (NULL == item_info_list)
	{
		return;
	}
	else
	{
		memcpy(item_info_list, m_data_param.item_info_list, sizeof(item_info_list[0]) * RA_BLACK_MARKET_MAX_ITEM_COUNT);
	}
}

bool RandActivityBlackMarket::OnRoleOfferItem(Role *role, int index, int price)
{
	if (NULL == role || index < 0 || index >= RA_BLACK_MARKET_MAX_ITEM_COUNT)
	{
		return false;
	}

	int day = GetDayIndex(static_cast<time_t>(this->GetBeginTime()), EngineAdapter::Instance().Time());
	const RandActivityBlackMarketItemCfg::ItemConfig *item_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetBlackMarketItemCfg(day, index);
	if (NULL == item_cfg)
	{
		return false;
	}

	// 最小所需元宝
	int need_least_gold = 0;
	if (!m_data_param.item_info_list[index].is_offered)
	{
		need_least_gold = item_cfg->init_gold;
	}
	else
	{
		need_least_gold = m_data_param.item_info_list[index].cur_price + item_cfg->min_add_gold;
	}

	if (price < need_least_gold)
	{
		role->NoticeNum(errornum::EN_RA_BLACK_MARKET_OFFER_GOLD_PRICE_LIMIT);
		return false;
	}

	// 最后才花钱
	if (!role->GetKnapsack()->GetMoney()->UseGold(price, "RandActivityBlackMarket::OnRoleOfferItem", false))
	{
		role->NoticeNum(errornum::EN_RA_BLACK_MARKET_CANT_OFFER_ITEM);
		return false;
	}

	// 先返还元宝给原来的竞拍者
	if (m_data_param.item_info_list[index].buyer_uid > 0)
	{
		// 如果是自己竞拍自己，直接返还，否则邮件返还
		if (role->GetUID() == m_data_param.item_info_list[index].buyer_uid)
		{
			role->GetKnapsack()->GetMoney()->AddGold(m_data_param.item_info_list[index].cur_price, "RandActivityBlackMarket::OnRoleOfferItem");
		}
		else
		{
			static MailContentParam contentparam; contentparam.Reset();
			
			contentparam.AddVirtualItem(0, MAIL_VIRTUAL_ITEM_GOLD, m_data_param.item_info_list[index].cur_price);

			int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_black_market_return_gold_content, item_cfg->item.item_id);
			if (length > 0)
			{
				MailRoute::MailToUser(IntToUid(m_data_param.item_info_list[index].buyer_uid), SYSTEM_MAIL_REASON_INVALID, contentparam);
			}
		}
	}

	// 设置新的买家
	m_data_param.item_info_list[index].SetItemInfo(item_cfg->seq, 1, price, role->GetUID());
	role->NoticeNum(noticenum::NT_RA_BLACK_MARKET_OFFER_ITEM_SUCC);

	return true;
}

void RandActivityBlackMarket::CheckGiveItem()
{
	for (int index = 0; index < RA_BLACK_MARKET_MAX_ITEM_COUNT; ++ index)
	{
		int day = GetDayIndex(static_cast<time_t>(this->GetBeginTime()), EngineAdapter::Instance().Time() - 3); // 由于切天结算，避免读取到下一天的配置，时间做一下偏移
		const RandActivityBlackMarketItemCfg::ItemConfig *item_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetBlackMarketItemCfg(day, index);
		if (NULL != item_cfg)
		{
			if (m_data_param.item_info_list[index].buyer_uid > 0)
			{
				// 传闻
				Role *final_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(m_data_param.item_info_list[index].buyer_uid));
				if (NULL != final_role)
				{
					int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_rand_activity_black_market_final_get_reward, 
						final_role->GetUID(), final_role->GetName(), (int)final_role->GetCamp(), m_data_param.item_info_list[index].cur_price, item_cfg->item.item_id);
					if (length > 0) 
					{
						World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL);
					}
				}

				// 邮件发放奖励
				{
					static MailContentParam contentparam; contentparam.Reset();
					const ItemBase *reward_item_base = ITEMPOOL->GetItem(item_cfg->item.item_id);
					if (NULL != reward_item_base)
					{
						contentparam.item_list[0].item_id = item_cfg->item.item_id;
						contentparam.item_list[0].num = item_cfg->item.num;
						contentparam.item_list[0].is_bind = (item_cfg->item.is_bind) ? 1 : 0;
						contentparam.item_list[0].invalid_time = reward_item_base->CalInvalidTime(); 
					}    
					int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_black_market_give_item_content, item_cfg->item.item_id);
					if (length > 0)
					{
						MailRoute::MailToUser(IntToUid(m_data_param.item_info_list[index].buyer_uid), SYSTEM_MAIL_REASON_INVALID, contentparam);
						m_data_param.item_info_list[index].buyer_uid = 0;
					}
				}
			}
		}
	}
}

void RandActivityBlackMarket::RefreshItemInfoList()
{
	int day = GetDayIndex(static_cast<time_t>(this->GetBeginTime()), EngineAdapter::Instance().Time());

	for (int index = 0; index < RA_BLACK_MARKET_MAX_ITEM_COUNT; ++ index)
	{
		m_data_param.item_info_list[index].Reset();

		const RandActivityBlackMarketItemCfg::ItemConfig *item_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetBlackMarketItemCfg(day, index);
		if (item_cfg != NULL)
		{
			m_data_param.item_info_list[index].SetItemInfo(item_cfg->seq, 0, item_cfg->init_gold, 0);
		}
	}
}
