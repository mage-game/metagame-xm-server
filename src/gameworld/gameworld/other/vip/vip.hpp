#ifndef __VIP_HPP__
#define __VIP_HPP__

#include "servercommon/serverdef.h"
#include "servercommon/vipdef.hpp"
#include "protocal/msgother.h"
#include "obj/character/attribute.hpp"

class Role;

class Vip
{
public: 
	Vip();
	~Vip();

	void SetRole(Role *role) { m_role = role; }

	void Init(Role *role, const VipParam &vip_param); 
	void GetOtherInitParam(VipParam *vip_param);	

	void Update(time_t now_second);

	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);

	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);

	void SetVipLevel(int vip_level, bool is_time_limit_vip = false);
	int  GetVipLevel();
	bool AddVipExp(int inc_exp);
	bool IsTimeLimitVip();
	unsigned int GetTimeLimitVipTime() { return m_vip_param.time_limit_vip_time; }

	void FetchLevelReward(int seq);
	void FetchVipWeekReward();
	void FetchTimeLimitVip();
	void SendVipInfo(bool is_broadcast = true);	

	void OnWeekChange();

private:
	bool UpgradeVipLevel(int to_vip_level);
	
	Role			*m_role;
	CharIntAttrs	m_attrs_add;							// 各属性加成数值

	VipParam		m_vip_param;
};

#endif // __VIP_HPP__



