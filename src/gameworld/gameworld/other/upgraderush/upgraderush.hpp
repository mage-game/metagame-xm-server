#ifndef __UPGRADE_RUSH_H__
#define __UPGRADE_RUSH_H__

#include "upgraderushconfig.hpp"

class Role;

class UpgradeRush
{
public:
	UpgradeRush();
	~UpgradeRush();

	void SetRole(Role *role) { m_role = role; };

	void Init(Role *role, const unsigned int upgrade_return_timestamp_list[UPGRADE_RUSH_MAX_TYPE_COUNT]);
	void GetInitParam(unsigned int upgrade_return_timestamp_list[UPGRADE_RUSH_MAX_TYPE_COUNT]);
	
	void OnRoleLogin();
	void OnUpgradeLevelChange(int upgrade_type, int new_grade);
	
private:

	void SendToClient();
	
	Role  *m_role;
	unsigned int m_upgrade_return_end_time_list[UPGRADE_RUSH_MAX_TYPE_COUNT];
};

#endif

