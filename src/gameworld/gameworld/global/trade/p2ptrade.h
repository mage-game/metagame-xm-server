#ifndef __P2PTRADE_H__
#define __P2PTRADE_H__

#include "servercommon/internalprotocal/ggprotocal.h"
#include "servercommon/internalprotocal/gglobalprotocal.h"

struct P2PTradeUser
{
public:
	P2PTradeUser() 
	{

	}
	~P2PTradeUser() {}

	UserID user_id;
};

class P2PTrade
{
public:
	P2PTrade() : m_trade_id(0) {} 
	P2PTrade(long long trade_id, const UserID &user_id, const UserID &other_user_id);
	~P2PTrade();

	long long GetTradeID() { return m_trade_id; }

	P2PTradeUser * GetP2PTradeUser(const UserID &user_id);
	P2PTradeUser * GetOtherP2PTradeUser(const UserID &user_id);

private:
	P2PTradeUser m_user_list[2];
	long long m_trade_id;
};

#endif // __P2PTRADE_H__


