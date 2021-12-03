#include "p2ptrade.h"
#include "engineadapter.h"

P2PTrade::P2PTrade(long long trade_id, const UserID &user_id, const UserID &other_user_id)
{
	UNSTD_STATIC_CHECK(2 == sizeof(m_user_list) / sizeof(m_user_list[0]));

	m_user_list[0].user_id = user_id;
	m_user_list[1].user_id = other_user_id;

	m_trade_id = trade_id;
}

P2PTrade::~P2PTrade()
{
}

P2PTradeUser * P2PTrade::GetP2PTradeUser(const UserID &user_id)
{
	if (m_user_list[0].user_id == user_id) return &m_user_list[0];
	else if (m_user_list[1].user_id == user_id) return &m_user_list[1];
	return NULL;
}

P2PTradeUser * P2PTrade::GetOtherP2PTradeUser(const UserID &user_id)
{
	if (m_user_list[0].user_id == user_id) return &m_user_list[1];
	else if (m_user_list[1].user_id == user_id) return &m_user_list[0];
	return NULL;
}


