#include "traderegister.h"

#include "internalcomm.h"
#include "obj/character/role.h"
#include "scene/scene.h"
#include "scene/scenemanager.h"
#include "servercommon/errornum.h"
#include "protocal/msgtrade.h"
#include "item/itempool.h"
#include "servercommon/internalprotocal/ggprotocal.h"
#include "servercommon/internalprotocal/gglobalprotocal.h"
#include "gamelog.h"
#include "engineadapter.h"
#include "global/trade/p2ptrademanager.h"
#include "gamelog.h"

#include "world.h"
#include "item/knapsack.h"
#include "other/sublock/sublock.hpp"

TradeRegister::TradeRegister()
	: m_role(NULL), m_trade_state(TS_NONE), m_other_trade_state(TS_NONE), m_trade_gold(0), m_trade_coin(0), m_trade_id(0)
{
	memset(m_other_name, 0, sizeof(m_other_name));
	memset(m_other_plat_name, 0, sizeof(m_other_plat_name));
}

TradeRegister::~TradeRegister()
{
	
}

int TradeRegister::GetTradeItemList(TradeListItem trade_item_list[P2P_TRADE_MAX_ITEM_COUNT])
{
	memcpy(trade_item_list, m_trade_item_list, sizeof(m_trade_item_list));

	int valid_count = 0;
	for (int i = 0; i < P2P_TRADE_MAX_ITEM_COUNT; ++ i)
	{
		if (m_trade_item_list[i].Invalid())
		{
			continue;
		}

		++ valid_count;
	}

	return valid_count;
}

bool TradeRegister::BuildTradeRelation(long long trade_id, UserID other_userid)
{
	m_trade_id = trade_id;
	m_trade_state = TS_TRADING;
	m_other_user_id = other_userid;

	return true;
}

bool TradeRegister::Lock()
{
	if (m_trade_state != TS_TRADING) 
	{
		m_role->NoticeNum(errornum::EN_NOT_AT_TRADING);
		return false;
	}

	Knapsack *knapsack = m_role->GetKnapsack(); Money *money = knapsack->GetMoney();

	ItemGridData item_data;

	for (int i = 0; i < P2P_TRADE_MAX_ITEM_COUNT; ++i) // 这里仅做检查（客户端发现格子变化需要取消交易）
	{
		if (!m_trade_item_list[i].Invalid())
		{
			int k_index = m_trade_item_list[i].knapsack_index;
			if (!knapsack->GetItemGridData(k_index, &item_data))
			{
				m_role->NoticeNum(errornum::EN_TRADE_ITEM_IS_CHANGE);
				return false;
			}

			ItemDataWrapper tmp_wrapper; item_data.WrapTo(&tmp_wrapper);
			
			if (tmp_wrapper.num < m_trade_item_list[i].item_wrapper.num)
			{
				m_role->NoticeNum(errornum::EN_TRADE_ITEM_IS_CHANGE);
				return false;
			}

			if (!m_trade_item_list[i].item_wrapper.IsEqual(tmp_wrapper, true))
			{
				m_role->NoticeNum(errornum::EN_TRADE_ITEM_IS_CHANGE);
				return false;
			}
		}
	}

	if (!money->GoldMoreThan(m_trade_gold))
	{
		m_role->NoticeNum(errornum::EN_COIN_NOT_ENOUGH);
		return false;
	}

	if (!money->CoinMoreThan(m_trade_coin))
	{
		m_role->NoticeNum(errornum::EN_COIN_NOT_ENOUGH);
		return false;
	}

	knapsack->Lock(); money->Lock();

	m_trade_state = TS_LOCK;
	this->SendTradeState();

	return true;
}

bool TradeRegister::Affirm()
{
	if (TS_LOCK != m_trade_state) 
	{
		m_role->NoticeNum(errornum::EN_TRADE_LOCK_FIRST);
		return false;
	}

	if (TS_TRADING == m_other_trade_state)
	{
		m_role->NoticeNum(errornum::EN_TRADE_WAIT_FOR_ANOTHER_LOCK);
		return false;
	}

	m_trade_state = TS_AFFIRM;
	this->SendTradeState();

	return true;
}

void TradeRegister::OnCancel()
{
	m_trade_state = TS_CANCEL;

	this->SendTradeState();
}

bool TradeRegister::PutItem(int register_index, int knapsack_index, int item_num)
{
	if (m_role->GetSublock()->CheckHasLock()) return false;

	if (register_index < 0 || register_index >= P2P_TRADE_MAX_ITEM_COUNT) 
	{
		m_role->NoticeNum(errornum::EN_TRADE_PUTITEM_MAX_COUNT);
		return false;
	}

	if (item_num <= 0) return false;

	if (TS_TRADING != m_trade_state)
	{
		if (m_trade_state == TS_LOCK || m_trade_state == TS_AFFIRM) // 已经锁定
		{
			m_role->NoticeNum(errornum::EN_TRADE_LOCK_STATE_CAN_NOT_CHANGE);
			return false;
		}

		// m_role->NoticeNum(errornum::EN_NOT_AT_TRADING); // 不在交易状态

		return false;
	}

	if (-1 == knapsack_index)
	{
		if (m_trade_item_list[register_index].Invalid()) return false;

		short old_knapsack_index = static_cast<short>(m_trade_item_list[register_index].knapsack_index);

		m_trade_item_list[register_index].Reset();

		// 发送通知
		Protocol::SCTradeItem ti;
		ti.is_me = 0;
		ti.trade_index = register_index;
		ti.knapsack_index = old_knapsack_index;
		ti.reserve_sh = 0;
		ti.item_id = ItemBase::INVALID_ITEM_ID;
		ti.num = 0;
		ti.invalid_time = 0;

		EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&ti, sizeof(ti));
		ti.is_me = 1;
		World::GetInstWorld()->GetSceneManager()->SendToUser(m_other_user_id, (const char*)&ti, sizeof(ti));
	}
	else
	{
		if (!m_trade_item_list[register_index].Invalid()) return false;

		for (int i = 0; i < P2P_TRADE_MAX_ITEM_COUNT; ++i)
		{
			if (m_trade_item_list[i].knapsack_index == knapsack_index)
			{
				return false;
			}
		}

		Knapsack *knapsack = m_role->GetKnapsack();

		ItemGridData item_data;
		if (!knapsack->GetItemGridData(knapsack_index, &item_data))
		{
			m_role->NoticeNum(errornum::EN_KNAPSACK_LOAD_ERROR);
			return false;
		}

		if (!ItemBase::CanTrade(&item_data))
		{
			m_role->NoticeNum(errornum::EN_ITEM_CANNOT_TRADE);
			return false;
		}

		if (item_data.num < item_num)
		{
			m_role->NoticeNum(errornum::EN_ITEM_NUM_NOT_ENOUGH);
			return false;
		}

		m_trade_item_list[register_index].knapsack_index = knapsack_index;
		item_data.WrapTo(&m_trade_item_list[register_index].item_wrapper);
		m_trade_item_list[register_index].item_wrapper.num = item_num;

		// 发送通知给双方
 		this->SendTradeItemInfo(register_index);
	}
	
	return true;
}

bool TradeRegister::PutMoney(int gold, int coin)
{
	if (m_role->GetSublock()->CheckHasLock()) return false;

	if (TS_TRADING != m_trade_state)
	{
		if (m_trade_state == TS_LOCK || m_trade_state == TS_AFFIRM) // 已经锁定
		{
			m_role->NoticeNum(errornum::EN_TRADE_LOCK_STATE_CAN_NOT_CHANGE);
			return false;
		}

		m_role->NoticeNum(errornum::EN_NOT_AT_TRADING); // 不在交易状态

		return false;
	}
	
	Money *money = m_role->GetKnapsack()->GetMoney();

	if (gold < 0 || !money->GoldMoreThan(gold))		// 元宝数量出错或余额不足
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return false;
	}

	if (coin < 0 || !money->CoinMoreThan(coin))
	{
		m_role->NoticeNum(errornum::EN_COIN_NOT_ENOUGH);
		return false;
	}

	m_trade_gold = gold;
	m_trade_coin = coin;

	// 发送通知
	Protocol::SCTradeGold tc;
	tc.gold = gold;
	tc.coin = coin;
	tc.is_me = 0;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&tc, sizeof(tc));
	tc.is_me = 1;
	World::GetInstWorld()->GetSceneManager()->SendToUser(m_other_user_id, (const char*)&tc, sizeof(tc));

	return true;
}

bool TradeRegister::CheckCanTrade(Role *other_role)
{
	if (NULL == other_role)
	{
		return false;
	}

	// 检查交易对象是否正确
	if (other_role->GetUserId() != m_other_user_id)
	{
		return false;
	}

	if (TS_AFFIRM != m_trade_state)
	{
		return false;
	}

	// 检查背包数量是否足够
	TradeListItem other_trade_item_list[P2P_TRADE_MAX_ITEM_COUNT];
	int other_item_num = other_role->GetTradeRegister()->GetTradeItemList(other_trade_item_list);

	if (other_item_num > m_role->GetKnapsack()->GetCurrKnapscakEmptyGridNum())
	{
		m_role->NoticeNum(errornum::EN_TRADE_FAILED_OWN_BAG_HAS_FULL);
		other_role->NoticeNum(errornum::EN_TRADE_FAILED_ANOTHER_BAG_HAS_FULL);

		return false;
	}

	// 检查交换的物品
	Knapsack *knapsack = m_role->GetKnapsack();

	for (int i = 0; i < P2P_TRADE_MAX_ITEM_COUNT; ++i) // 这里仅做检查（客户端发现格子变化需要取消交易）
	{
		if (!m_trade_item_list[i].Invalid())
		{
			ItemGridData item_data;
			int k_index = m_trade_item_list[i].knapsack_index;

			if (!knapsack->GetItemGridData(k_index, &item_data))
			{
				m_role->NoticeNum(errornum::EN_TRADE_ITEM_IS_CHANGE);
				return false;
			}

			ItemDataWrapper tmp_wrapper; item_data.WrapTo(&tmp_wrapper);

			if (tmp_wrapper.num < m_trade_item_list[i].item_wrapper.num)
			{
				m_role->NoticeNum(errornum::EN_TRADE_ITEM_IS_CHANGE);
				return false;
			}

			if (!m_trade_item_list[i].item_wrapper.IsEqual(tmp_wrapper, true))
			{
				m_role->NoticeNum(errornum::EN_TRADE_ITEM_IS_CHANGE);
				return false;
			}
		}
	}

	if (m_trade_gold > knapsack->GetMoney()->GetGold())
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return false;
	}

	if (m_trade_coin > knapsack->GetMoney()->GetCoin())
	{
		m_role->NoticeNum(errornum::EN_COIN_NOT_ENOUGH);
		return false;
	}


	return true;
}

bool TradeRegister::DoTrade(Role *other_role)
{
	if (NULL == other_role)
	{
		return false;
	}

	Knapsack *knapsack = m_role->GetKnapsack(); 

	// 给予的物品
	int trade_in_gold = other_role->GetTradeRegister()->GetTradeGold();
	int trade_in_coin = other_role->GetTradeRegister()->GetTradeCoin();

	TradeListItem trade_in_item_list[P2P_TRADE_MAX_ITEM_COUNT];
	int trade_in_item_num = other_role->GetTradeRegister()->GetTradeItemList(trade_in_item_list);

	knapsack->UnLock(); knapsack->GetMoney()->UnLock();

	if (!knapsack->CheckEmptyGridNoLessThan(trade_in_item_num))
	{
		return false;
	}

	// 消耗
	if (m_trade_state != TS_AFFIRM)
	{
		return false;
	}

	knapsack->GetMoney()->UseGold(m_trade_gold, "TradeOut", false);
	knapsack->GetMoney()->UseCoin(m_trade_coin, "TradeOut");

	int trade_out_item = 0;
	for (int i = 0; i < P2P_TRADE_MAX_ITEM_COUNT; i++)	
	{
		if (m_trade_item_list[i].Invalid()) continue;

		const ItemDataWrapper &item_wrapper = m_trade_item_list[i].item_wrapper;
		knapsack->ConsumeItemByIndex(m_trade_item_list[i].knapsack_index, item_wrapper.num, "TradeOut", true, false);

		++ trade_out_item;
	}

	if (trade_in_gold > 0)
	{
		knapsack->GetMoney()->AddGold(trade_in_gold, "TradeIn");
	}

	if (trade_in_coin > 0)
	{
		knapsack->GetMoney()->AddCoin(trade_in_coin, "TradeIn");
	}

	std::string item_log_str = "";
	static char item_wrapper_log[LOG_BASE_LENGTH] = { 0 };

	for (int i = 0; i < P2P_TRADE_MAX_ITEM_COUNT; i++)	
	{
		if (trade_in_item_list[i].Invalid()) continue;

		knapsack->Put(trade_in_item_list[i].item_wrapper, PUT_REASON_P2PTRADE);

		const ItemDataWrapper *item_wrapper = &trade_in_item_list[i].item_wrapper;
		SNPRINTF(item_wrapper_log, LOG_BASE_LENGTH,
			"item_id:%d, num:%d, is_bind:%d;",
			(int)item_wrapper->item_id,
			(int)item_wrapper->num,
			(int)item_wrapper->is_bind);
		item_log_str += item_wrapper_log;
	}

	knapsack->Lock(); knapsack->GetMoney()->Lock();

	m_trade_state = TS_AFFIRM_SUCC;

	gamelog::g_log_p2ptrade.printf(LL_INFO, "user do trade [trade_id:%lld] role[uid:%d, role_name:%s], other_role[uid:%d, role_name:%s],"
		"trade_out_gold: %d, trade_out_coin: %d, trade_out_item_list[item_num:%d, item1: [%s], item2: [%s], item3: [%s], item4: [%s], item5: [%s]"
		"trade_in_gold: %d, trade_in_coin: %d, trade_in_item_list[item_num:%d, item1: [%s], item2: [%s], item3: [%s], item4: [%s], item5: [%s]",
		m_trade_id, m_role->GetUID(), m_role->GetName(), other_role->GetUID(), other_role->GetName(), 
		m_trade_gold, m_trade_coin, trade_out_item, ItemDataWrapperLog(&m_trade_item_list[0].item_wrapper, 0), ItemDataWrapperLog(&m_trade_item_list[1].item_wrapper, 1), 
		ItemDataWrapperLog(&m_trade_item_list[2].item_wrapper, 2), ItemDataWrapperLog(&m_trade_item_list[3].item_wrapper, 3), ItemDataWrapperLog(&m_trade_item_list[4].item_wrapper, 4),
		trade_in_gold, trade_in_coin, trade_in_item_num, ItemDataWrapperLog(&trade_in_item_list[0].item_wrapper, 0), ItemDataWrapperLog(&trade_in_item_list[1].item_wrapper, 1), 
		ItemDataWrapperLog(&trade_in_item_list[2].item_wrapper, 2), ItemDataWrapperLog(&trade_in_item_list[3].item_wrapper, 3), ItemDataWrapperLog(&trade_in_item_list[4].item_wrapper, 4));

	ROLE_LOG_QUICK10(LOG_TYPE_TRADE_SUCC, m_role, trade_in_gold, other_role->GetUID(), other_role->GetName(), item_log_str.c_str(), 0, 0, 0, 0);

	return true;
}

void TradeRegister::SyncOtherUserInfo()
{
	if (INVALID_USER_ID != m_other_user_id)
	{
		Role *other_role = World::GetInstWorld()->GetSceneManager()->GetRole(m_other_user_id);
		if (NULL != other_role)
		{
			other_role->GetName(m_other_name);
			other_role->GetPlatName(m_other_plat_name);
			m_other_trade_state = other_role->GetTradeRegister()->GetTradeState();
			this->SendTradeState();

			return;
		}
	}

	P2PTradeManager::Instance().OnReqCancel(m_role, "SynOtherUserInfoFail");
}

void TradeRegister::Clear()
{
	if (m_trade_state == TS_NONE) return;

	m_trade_state = TS_NONE;
	m_other_user_id = INVALID_USER_ID;
	m_other_trade_state = TS_NONE;
	memset(m_other_name, 0, sizeof(m_other_name));
	memset(m_other_plat_name, 0, sizeof(m_other_plat_name));

	for (int i = 0; i < P2P_TRADE_MAX_ITEM_COUNT; ++i)
	{
		m_trade_item_list[i].Reset();
	}

	m_trade_gold = 0;
	m_trade_coin = 0;
	m_trade_id = 0;

	m_role->GetKnapsack()->UnLock(); m_role->GetKnapsack()->GetMoney()->UnLock();

	this->SendTradeState();
}

void TradeRegister::SendTradeItemInfo(int register_index)
{
	if (register_index < 0 || register_index >= P2P_TRADE_MAX_ITEM_COUNT) return;

	const ItemDataWrapper &item_wrapper = m_trade_item_list[register_index].item_wrapper;
	if (item_wrapper.Invalid()) return;

	if (item_wrapper.has_param) 
	{
		static Protocol::SCTradeItemParam tip;
		tip.trade_index = register_index;
		tip.item_wrapper = item_wrapper;
		tip.knapsack_index = m_trade_item_list[register_index].knapsack_index;
		tip.reserve_sh = 0;

		tip.is_me = 0;
		EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&tip, sizeof(tip));
		tip.is_me = 1;
		World::GetInstWorld()->GetSceneManager()->SendToUser(m_other_user_id, (const char*)&tip, sizeof(tip));
	}
	else
	{
		Protocol::SCTradeItem ti;

		ti.trade_index = register_index;
		ti.item_id = item_wrapper.item_id;
		ti.num = item_wrapper.num;
		ti.invalid_time = item_wrapper.invalid_time;
		ti.knapsack_index = m_trade_item_list[register_index].knapsack_index;
		ti.reserve_sh = 0;

		ti.is_me = 0;
		EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&ti, sizeof(ti));
		ti.is_me = 1;
		World::GetInstWorld()->GetSceneManager()->SendToUser(m_other_user_id, (const char*)&ti, sizeof(ti));
	}
}

void TradeRegister::SendTradeState()
{
	Protocol::SCTradeState ts;
	ts.trade_state = m_trade_state;
	ts.other_trade_state = m_other_trade_state;
	ts.other_uid = UidToInt(m_other_user_id);
	F_STRNCPY(ts.other_name, m_other_name, sizeof(GameName));
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&ts, sizeof(ts));
}

