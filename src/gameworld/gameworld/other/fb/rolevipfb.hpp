#ifndef __ROLE_FB_VIP__
#define __ROLE_FB_VIP__

#include "servercommon/fbdef.hpp"
#include "obj/character/attribute.hpp"

class Role;

class RoleVipFB
{
public:
	RoleVipFB();
	~RoleVipFB();

	void SetRole(Role *role) { m_role = role; }

	void Init(Role *role, const VipFBParam &param);
	void GetInitParam(VipFBParam *param);

	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);
	bool CanEnter(int level);
	void OnEnterFB(int level);
	void OnFinish(int level);
	void AutoFBReq(int level);

	bool IsPassLevel(int level);

	void SendAllInfo();

private:
	Role *m_role;

	VipFBParam m_param;
};

#endif
