#ifndef __TRADEREGISTER_H__
#define __TRADEREGISTER_H__

#include "item/itemgriddata.h"
#include "servercommon/internalprotocal/ggprotocal.h"

class Role;

struct TradeListItem 
{
	TradeListItem() : knapsack_index(-1){}

	int knapsack_index;
	ItemDataWrapper item_wrapper;

	void Reset() { knapsack_index = -1; item_wrapper.Reset(); }
	bool Invalid() { return -1 == knapsack_index ; }
};

class TradeRegister
{
public:
	TradeRegister();
	~TradeRegister();

	void SetRole(Role *role) { m_role = role; }

	int GetTradeState() { return m_trade_state; };
	UserID GetOtherUserID() { return m_other_user_id; }
	int GetTradeGold() { return m_trade_gold; }
	int GetTradeCoin() { return m_trade_coin; }
	int GetTradeItemList(TradeListItem trade_item_list[P2P_TRADE_MAX_ITEM_COUNT]);
	bool HasAffirm() { return TS_AFFIRM == m_trade_state; }
	
	bool BuildTradeRelation(long long trade_id, UserID other_userid);
	bool Lock();
	bool Affirm();
	void OnCancel();

	bool PutItem(int register_index, int knapcack_index, int item_num);
	bool PutMoney(int gold, int coin);
	bool CheckCanTrade(Role *other_role);
	bool DoTrade(Role *other_role);
	
	void SyncOtherUserInfo();
	void Clear();

private:
	// void TradeLog(const char *show_str);
	void SendTradeItemInfo(int register_index);
	void SendTradeState();

	Role *m_role;

	int m_trade_state;
	UserID m_other_user_id;
	GameName m_other_name;
	PlatName m_other_plat_name;
	int m_other_trade_state;

	TradeListItem m_trade_item_list[P2P_TRADE_MAX_ITEM_COUNT];
	int	m_trade_gold;
	int m_trade_coin;

	long long m_trade_id;
};

#endif // __TRADEREGISTER_H__

