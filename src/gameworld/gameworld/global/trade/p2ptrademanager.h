#ifndef __P2PTRADEMANAGER_H__
#define __P2PTRADEMANAGER_H__

#include <map>
#include "servercommon/internalprotocal/gglobalprotocal.h"

class P2PTrade;
class GlobalServer;

const static int TRADE_LEVEL_LIMIT = 31;

class P2PTradeManager
{
public:
	static P2PTradeManager & Instance();

	P2PTradeManager();
	~P2PTradeManager();

	void OnReqTrade(Role *role, int target_uid);
	void OnReqTradeAgree(Role *role, int target_uid);
	bool PutItem(Role *role, int register_index, int knapcack_index, int item_num);
	bool PutMoney(Role *role, int gold, int coin);
	void OnReqTradeLock(Role *role);
	void OnReqTradeAffirm(Role *role);
	void OnReqCancel(Role *role, const char *reason);
	void OnUserLeaveScene(Role *role);

private:
	typedef std::map<UserID, long long> P2PUserTradeMap;
	typedef std::map<UserID, long long>::iterator P2PUserTradeMapIt;
	P2PUserTradeMap m_p2p_user_trade_map;

	typedef std::map<long long, P2PTrade> P2PTradeMap;
	typedef std::map<long long, P2PTrade>::iterator P2PTradeMapIt;
	P2PTradeMap m_p2ptrade_map;

	int m_p2ptrade_seq;
};

#endif


