#ifndef __MONEY_H__
#define __MONEY_H__

#include "iclockmodule.h"

#include "servercommon/basedef.h"

class Role;

class Money
{
public:
	static const int BINDCOIN_FIRST = 1;						// 优先扣除绑定铜钱
	static const int NOBINDCOIN_FIRST = 2;						// 优先扣除非绑定铜钱

	Money();
	~Money();

	void SetRole(Role *role) { m_role = role; }

	void Init(Role *role, Int64 gold, Int64 gold_bind, Int64 coin, Int64 coin_bind);

	Int64 GetAllGold() const { return m_gold + m_gold_bind; }
	Int64 GetGold() const { return m_gold; }
	Int64 GetGoldBind() const { return m_gold_bind; }
	
	Int64 GetAllCoin() const { return m_coin + m_coin_bind; }
	Int64 GetCoin() const { return m_coin; }
	Int64 GetCoinBind() const { return m_coin_bind; }

	bool GoldMoreThan(Int64 consume_gold);
	bool GoldBindMoreThan(Int64 consume_gold_bind);
	bool AllGoldMoreThan(Int64 consume_all_gold);

	bool CoinMoreThan(Int64 consume_coin);
	bool CoinBindMoreThan(Int64 consume_coin_bind);
	bool AllCoinMoreThan(Int64 consume_all_coin);

	bool UseGold(Int64 consume_gold, const char* type, bool is_consume_sys = true);
	bool UseGoldBind(Int64 consume_gold_bind, const char* type);
	bool UseAllGold(Int64 consume_all_gold, const char *type, bool bind_first = true, Int64 *use_gold = NULL, Int64 *use_gold_bind = NULL, bool is_consume_sys = true);

	bool UseCoin(Int64 consume_coin, const char* type);
	bool UseCoinBind(Int64 consume_coin_bind, const char* type);
	bool UseAllCoin(Int64 consume_all_coin, const char *type, bool bind_first = true, Int64 *use_coin = NULL, Int64 *use_coin_bind = NULL);
	
	bool Add(Int64 gold, Int64 gold_bind, Int64 coin, Int64 coin_bind, const char* type);
	bool AddGold(Int64 gold, const char* type, bool is_chongzhi = false);
	bool AddGoldBind(Int64 gold_bind, const char* type);

	bool AddCoin(Int64 coin, const char *type);
	bool AddCoinBind(Int64 coin_bind, const char* type);

	void GetAccountGoldFromDB(time_t now_second);
	void GetAccountGoldToRole(Int64 gold, int authority_type);

	void Send();

	bool Lock() { if (m_is_lock) return false; return (m_is_lock = true); }
	void UnLock() { m_is_lock = false; }
	bool IsLock() { return m_is_lock; }

	void ForceGetGold();
	unsigned int GetLastGetAccountTime() { return static_cast<unsigned>(m_last_account_get_time); }

	struct QuickLogParam
	{
		int uid = 0;
		GameName user_name = { 0 };
		PlatName plat_name = { 0 };
		long long n1 = 0;	// consume gold
		long long n2 = 0;	// m_gold
		char type[128] = { 0 };
	};
	static void QuickLogByCross(int log_type, QuickLogParam &param);
	void OnUseGoldInCross(bool is_bind, long long use_gold, const char *type, bool is_consume_sys);

private:
	Role	*m_role;

	Int64	m_gold;
	Int64	m_gold_bind;
	Int64	m_coin;
	Int64	m_coin_bind;

	time_t m_last_account_get_time;

	bool m_is_lock;
};

#endif
