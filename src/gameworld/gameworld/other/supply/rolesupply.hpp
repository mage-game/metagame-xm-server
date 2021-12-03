#ifndef __ROLE_SUPPLY_HPP__
#define __ROLE_SUPPLY_HPP__

class Role;

#include "servercommon/roleshopdef.hpp"

class RoleSupply
{
public:
	RoleSupply();
	~RoleSupply();

	void SetRole(Role *role) { m_role = role; }

	void Init(Role *role, const RoleSupplyParam &param);
	void GetInitParam(RoleSupplyParam *param);

	void Update(time_t now_second);

	void OnBuySupplyItem(int supply_type, int index, bool is_use_no_bind_gold);
	void OnSetSupplyRecoverRangePer(int supply_type, int supply_range_per);
	void SendSupplyInfo();
	bool OnUseXuebao(int supply_type, int recover_value);

private:
	Role *m_role;

	RoleSupplyItem m_supply_list[SUPPLY_TYPE_MAX];
};

#endif // __ROLE_SUPPLY_HPP__

