#ifndef __ROLE_FB_PERSONBOSS__
#define __ROLE_FB_PERSONBOSS__

#include "obj/character/attribute.hpp"
#include "servercommon/fbdef.hpp"

class Role;

class RolePersonBossFB
{
public:
	RolePersonBossFB();
	~RolePersonBossFB();

	void SetRole(Role *role) { m_role = role; }

	void Init(Role *role, const PersonBossParam &param);
	void GetInitParam(PersonBossParam *param);
	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);
	void OnRoleLogin();

	int GetTodayEnterByLayer(int layer);
	void AddTodayEnterTimesByLayer(int layer);

	void ReqPersonBossInfo();
private:
	void SendPersonBossInfo();

	Role *m_role;
	PersonBossParam m_param;
};

#endif


