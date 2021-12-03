#ifndef __GOLD_VIP_HPP__
#define __GOLD_VIP_HPP__

#include "servercommon/serverdef.h"
#include "servercommon/goldvipdef.hpp"
#include "protocal/msgother.h"
#include "obj/character/attribute.hpp"

class Role;

class GoldVip
{
public: 
	GoldVip();
	~GoldVip();

	void SetRole(Role *role) { m_role = role; }

	void Init(Role *role, const GoldVipParam &gold_vip_param); 
	void GetOtherInitParam(GoldVipParam *gold_vip_param);	

	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);

	void OnActiveGoldVip();
	void OnFetchRetrunReward();
	bool IsvalidGoldVip();
	void CheckAddScore(int chongzhi_gold);
	unsigned int GetActiveGoldViptime() { return m_gold_vip_param.gold_vip_active_timestamp; } 
	void OnConvertShop(int seq);
	void SendGoldVipInfo();
	bool IsCanFetchReward();

private:
	Role			*m_role;

	GoldVipParam	m_gold_vip_param;
};

#endif // __GOLD_VIP_HPP__



