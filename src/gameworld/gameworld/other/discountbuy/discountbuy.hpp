#ifndef __DISCOUNT_BUY_HPP__
#define __DISCOUNT_BUY_HPP__

#include "servercommon/struct/discountbuyparam.hpp"

class Role;

class DiscountBuy
{
public:
	DiscountBuy();
	~DiscountBuy();

	void SetRole(Role *role) { m_role = role; }

	void Init(Role *role, const DiscountBuyParam &param);
	void OnLogin();
	void GetInitParam(DiscountBuyParam *param);
	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);
	void OnRoleLevelChange(int to_level);

	void OnBuy(int seq);
	void SendDiscountBuyInfo();
	
private:
	void CheckDiscountBuyOpen();
	void SendNoticeMail(int phase);

	Role *m_role;
	DiscountBuyParam m_param;
	unsigned int m_last_broadcast_time;
};

#endif // __DISCOUNT_BUY_HPP__

