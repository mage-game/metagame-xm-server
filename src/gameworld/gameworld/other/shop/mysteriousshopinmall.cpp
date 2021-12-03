#include "mysteriousshopinmall.hpp"
#include "obj/character/role.h"
#include "servercommon/errornum.h"
#include "servercommon/noticenum.h"
#include "world.h"
#include "servercommon/string/gameworldstr.h"
#include "config/logicconfigmanager.hpp"
#include "other/shop/roleshop.hpp"
#include "item/knapsack.h"
#include "servercommon/serverconfig/crossconfig.hpp"

#include <vector>

MysteriousShopInMall::MysteriousShopInMall() : m_role(NULL)
{

}

MysteriousShopInMall::~MysteriousShopInMall()
{

}

void MysteriousShopInMall::Init(Role *role, const MysteriousShopInMallParam &param, const DisCountShopParam &param_1)
{
	m_role = role;
	m_param = param;
	m_discountshop_param = param_1;
}

void MysteriousShopInMall::GetInitParam(MysteriousShopInMallParam *param, DisCountShopParam *param_1)
{
	if (NULL != param)
	{
		*param = m_param;
	}
	if (param_1 != NULL)
	{
		*param_1 = m_discountshop_param;
	}
}

void MysteriousShopInMall::Update(unsigned int now_second)
{
	this->CheckRefreshTime();
}

void MysteriousShopInMall::OnRoleLogin()
{
	this->CheckRefreshTime();
	this->SendMoneyShopItemSeq();

	this->RefreshDiscountShopInfo();
}

void MysteriousShopInMall::MysteriousshopinmallOp(int type, int param1)
{
	switch (type)
	{
	case Protocol::MYSTERIOUSSHOP_IN_MALL_OPERATE_TYPE_MONEY:
		this->OnBuyMoneyShopItemReq(param1);
		break;

	case Protocol::MYSTERIOUSSHOP_IN_MALL_OPERATE_TYPE_REFRESH:
		this->OnRefreshMoneyShopItemReq(1 == param1);
		break;

	case Protocol::MYSTERIOUSSHOP_IN_MALL_OPERATE_INFO_REQ:
		this->SendMoneyShopItemSeq();
		break;

	case Protocol::MYSTERIOUSSHOP_IN_MALL_OPERATE_OPEN_VIEW:
		{
			m_param.client_remind_flag = 0;
			this->SendMoneyShopItemSeq();
		}
		break;
	}
}

void MysteriousShopInMall::OnBuyMoneyShopItemReq(int seq)
{
	const MysteriousShopInMallOtherCfg *other_cfg = LOGIC_CONFIG->GetMysteriousShopInMallCfg().GetMysteriousShopInMallOtherCfg();
	if (NULL == other_cfg)
	{
		return;
	}

	if (seq < 0 || seq >= other_cfg->item_count)
	{
		return;
	}

	const MysteriousShopItem* shop_item_cfg = LOGIC_CONFIG->GetMysteriousShopInMallCfg().GetMysteriousShopItemCfg(m_param.seq_list[seq].seq);
	if (NULL == shop_item_cfg)
	{
		return;
	}

	if (NULL == m_role)
	{
		return;
	}

	Knapsack *knapsack = m_role->GetKnapsack();
	if (NULL == knapsack)
	{
		return;
	}

	if (!knapsack->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	if (MysteriousShopInMallItemInfo::HAD_BUY_STATE == m_param.seq_list[seq].state)
	{
		m_role->NoticeNum(errornum::NT_MYSTERIOUS_SHOP_BUY_LIMIT);
		return;
	}

	// 消耗
	{
		static const double rate = 0.0001;
		long long real_price = static_cast<long long>(shop_item_cfg->price * (rate * shop_item_cfg->dicount));
		Money *money = knapsack->GetMoney();
		if (real_price > 0 && !money->GoldMoreThan(real_price))
		{
			m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
			return;
		}

		if (!knapsack->GetMoney()->UseGold(real_price, "OnMysteriousshopBuy")) return;
	}

	// 给予
	{
		if (!m_role->GetKnapsack()->PutOrMail(shop_item_cfg->item, PUT_REASON_MYSTERIOUSSHOP))
		{
			return;
		}

		if (shop_item_cfg->is_broadcast)
		{
			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_mysteriousshop_buy_good_item,
				m_role->GetUID(), m_role->GetName(), m_role->GetCamp(), shop_item_cfg->dicount / 1000,
				shop_item_cfg->item.item_id);
			if (length > 0)
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE);
			}
		}

		m_param.seq_list[seq].state = MysteriousShopInMallItemInfo::HAD_BUY_STATE;
	}

	m_role->NoticeNum(noticenum::NT_SHOP_BUY_SUCC);
	this->SendMoneyShopItemSeq();
}

void MysteriousShopInMall::OnRefreshMoneyShopItemReq(bool is_fresh_all)
{
	if (NULL == m_role)
	{
		return;
	}

	if (CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}

	Knapsack *knapsack = m_role->GetKnapsack();
	if (NULL == knapsack)
	{
		return;
	}

	const MysteriousShopInMallOtherCfg *other_cfg = LOGIC_CONFIG->GetMysteriousShopInMallCfg().GetMysteriousShopInMallOtherCfg();
	if (NULL == other_cfg) return;

	// 消耗
	if (m_param.today_free_count > 0 && !is_fresh_all)
	{
		m_param.today_free_count--;
	}
	else if (other_cfg->can_use_item_refresh)
	{
		if (knapsack->IsLock())
		{
			m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
			return;
		}

		int need_item_num = is_fresh_all ? other_cfg->all_consume_replacement : other_cfg->consume_replacement;
		int bag_itme_num = m_role->GetKnapsack()->Count(other_cfg->replacement_id);
		int real_consume_item_num = need_item_num > bag_itme_num ? bag_itme_num : need_item_num;
		int left_num = need_item_num > bag_itme_num ? need_item_num - bag_itme_num : 0;

		if (left_num > 0)
		{
			int total_need_gold = left_num * other_cfg->consume_diamond;
			if (total_need_gold > 0 && !m_role->GetKnapsack()->GetMoney()->GoldMoreThan(total_need_gold))
			{
				m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
				return;
			}

			if (!knapsack->GetMoney()->UseGold(total_need_gold, "MysteriousShopRefresh")) return;
		}	

		if (real_consume_item_num > 0 && !m_role->GetKnapsack()->ConsumeItem(other_cfg->replacement_id, real_consume_item_num, "MysteriousShopRefresh"))
		{
			m_role->NoticeNum(errornum::EN_FUNC_STUFF_NOT_ENOUGH);
			return;
		}
	}
	else
	{	// 消耗
		Money *money = knapsack->GetMoney();
		if (NULL == money)
		{
			return;
		}

		int consume_gold = is_fresh_all ? other_cfg->all_consume_diamond : other_cfg->consume_diamond;

		if (consume_gold > 0 && !money->GoldMoreThan(consume_gold))
		{
			m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
			return;
		}

		if (!knapsack->GetMoney()->UseGold(consume_gold, "MysteriousShopRefresh")) return;
	}

	// 给予
	m_role->GetRoleShop()->AddChestShopMysteriousShopCredit(0, "Chestshop");	//现在不加积分
	this->RefreshMoneyShopItem(is_fresh_all);
}

void MysteriousShopInMall::CheckRefreshTime()
{
	unsigned int current_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	if (current_time > m_param.next_refresh_item_time)
	{
		unsigned int today_zero_time = static_cast<unsigned int>(GetZeroTime(EngineAdapter::Instance().Time()));

		const MysteriousShopFlushTimeCfg *pre_flush_time_cfg = LOGIC_CONFIG->GetMysteriousShopInMallCfg().GetFlushTimeCfg(m_param.next_refresh_item_time);
		if (nullptr == pre_flush_time_cfg)
		{
			return;
		}

		const MysteriousShopFlushTimeCfg *flush_time_cfg = LOGIC_CONFIG->GetMysteriousShopInMallCfg().GetFlushTimeCfg(current_time);
		if (nullptr == flush_time_cfg)
		{
			return;
		}

		if (m_param.next_refresh_item_time <= 0)
		{
			this->RefreshMoneyShopItem(true);
		}

		unsigned int tmp_next_refresh_time = today_zero_time + flush_time_cfg->refresh_time;
		m_param.next_refresh_item_time = (tmp_next_refresh_time <= m_param.next_refresh_item_time) ? tmp_next_refresh_time + SECOND_PER_DAY : tmp_next_refresh_time;

		bool is_auto_fresh = pre_flush_time_cfg->is_auto_refresh;
		if (is_auto_fresh)
		{
			this->RefreshMoneyShopItem(true);
		}
		else
		{
			m_param.today_free_count = pre_flush_time_cfg->give_free_refesh_count;
		}

		m_param.client_remind_flag = 1;
		this->SendMoneyShopItemSeq();
	}
}

int MysteriousShopInMall::GetIndexBySeq(int seq)
{
	const MysteriousShopInMallOtherCfg *other_cfg = LOGIC_CONFIG->GetMysteriousShopInMallCfg().GetMysteriousShopInMallOtherCfg();
	if (NULL == other_cfg)
	{
		return -1;
	}

	for (int i = 0; i < other_cfg->item_count; i++)
	{
		if (m_param.seq_list[i].seq == seq)
		{
			return i;
		}
	}

	return -1;
}

void MysteriousShopInMall::RefreshMoneyShopItem(bool is_all)
{
	if (NULL == m_role)
	{
		return;
	}

	const MysteriousShopInMallOtherCfg *other_cfg = LOGIC_CONFIG->GetMysteriousShopInMallCfg().GetMysteriousShopInMallOtherCfg();
	if (NULL == other_cfg)
	{
		return;
	}

	static int seq_list[MYSTERIOUSSHOP_ITEM_MAX_COUNT];
	memset(seq_list, -1, sizeof(seq_list));

	int fresh_item_count = is_all ? other_cfg->item_count : 1;

	LOGIC_CONFIG->GetMysteriousShopInMallCfg().GetRandSeqList(m_role, seq_list, fresh_item_count);

	for (int i = 0; i < fresh_item_count; i++)
	{
		if (seq_list[i] < 0)
		{
			return;
		}
		else
		{
			m_param.seq_list[i].seq = seq_list[i];
		}
		m_param.seq_list[i].state = MysteriousShopInMallItemInfo::HAD_REFRESH_STATE;
	}

	this->SendMoneyShopItemSeq();
}

void MysteriousShopInMall::SendMoneyShopItemSeq()
{
	const MysteriousShopInMallOtherCfg *other_cfg = LOGIC_CONFIG->GetMysteriousShopInMallCfg().GetMysteriousShopInMallOtherCfg();
	if (NULL == other_cfg)
	{
		return;
	}

	static Protocol::SCSendMysteriosshopItemInfo shop_seq_info;
	shop_seq_info.next_shop_item_refresh_time = m_param.next_refresh_item_time;
	shop_seq_info.item_count = other_cfg->item_count;
	shop_seq_info.client_remind_flag = (int)m_param.client_remind_flag;
	shop_seq_info.today_free_count = m_param.today_free_count;
	memcpy(shop_seq_info.item, m_param.seq_list, sizeof(m_param.seq_list[0]) * other_cfg->item_count);
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&shop_seq_info, 
		sizeof(shop_seq_info) - sizeof(m_param.seq_list[0]) * (MYSTERIOUSSHOP_ITEM_MAX_COUNT - other_cfg->item_count));
}

void MysteriousShopInMall::OnDiscountShopBuy(int seq, int num)
{
	if (seq < 0 || seq >= DISCOUNTSHOP_ITEM_MAX_COUNT)
	{
		return;
	}

	const DiscountShopItem* shop_item_cfg = LOGIC_CONFIG->GetMysteriousShopInMallCfg().GetDiscountShopItemCfg(seq);
	if (NULL == shop_item_cfg)
	{
		return;
	}

	Knapsack *knapsack = m_role->GetKnapsack();
	if (NULL == knapsack)
	{
		return;
	}

	if (!knapsack->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	if (m_discountshop_param.item_buy_count[seq] + num > shop_item_cfg->buy_limit)
	{
		return;
	}

	// 消耗
	{
		int need_gold = num * shop_item_cfg->price;
		Money *money = knapsack->GetMoney();
		if (need_gold > 0 && !money->GoldMoreThan(need_gold))
		{
			m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
			return;
		}

		if (!knapsack->GetMoney()->UseGold(need_gold, "OnDiscountshopBuy")) return;
	}

	// 给予
	{
		ItemConfigData item;
		item.item_id = shop_item_cfg->item.item_id;
		item.num = shop_item_cfg->item.num * num;
		item.is_bind = shop_item_cfg->item.is_bind;
		if (!m_role->GetKnapsack()->PutOrMail(item, PUT_REASON_DISCOUNTSHOP_BUY))
		{
			return;
		}

		m_discountshop_param.item_buy_count[seq] += num;
	}

	m_role->NoticeNum(noticenum::NT_SHOP_BUY_SUCC);
	this->SendDiscountShopInfo();
}

void MysteriousShopInMall::RefreshDiscountShopInfo()
{
	if (!m_discountshop_param.today_is_refresh)
	{
		memset(m_discountshop_param.item_buy_count, 0, sizeof(m_discountshop_param.item_buy_count));

		m_discountshop_param.today_refresh_level = m_role->GetLevel();
		m_discountshop_param.today_is_refresh = 1;
	}

	this->SendDiscountShopInfo();
}

void MysteriousShopInMall::SendDiscountShopInfo()
{
	Protocol::SCSendDiscounthopItemInfo shop_info;

	int item_count = 0;
	int seq_list[DISCOUNTSHOP_ITEM_MAX_COUNT];
	memset(seq_list, 0, sizeof(seq_list));

	item_count = LOGIC_CONFIG->GetMysteriousShopInMallCfg().GetDiscountShopSeqList(seq_list, m_discountshop_param.today_refresh_level);

	shop_info.item_count = item_count;
	shop_info.today_refresh_level = m_discountshop_param.today_refresh_level;

	for (int i = 0; i < item_count && i < DISCOUNTSHOP_ITEM_MAX_COUNT; i++)
	{
		shop_info.item_list[i].seq = seq_list[i];

		if (seq_list[i] < 0 || seq_list[i] >= DISCOUNTSHOP_ITEM_MAX_COUNT)
		{
			continue;
		}
		shop_info.item_list[i].today_buy_count = m_discountshop_param.item_buy_count[seq_list[i]];
	}

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&shop_info,
		sizeof(shop_info) - sizeof(shop_info.item_list[0]) * (DISCOUNTSHOP_ITEM_MAX_COUNT - item_count));
}

void MysteriousShopInMall::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	m_discountshop_param.Reset();

	this->RefreshDiscountShopInfo();
}
