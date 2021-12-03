#ifndef __ROLEYAOJIANG__
#define __ROLEYAOJIANG__

#include "servercommon/yaojiangdef.hpp"

class Role;

class RoleYaojiang
{
public:
	RoleYaojiang();
	~RoleYaojiang();

	void SetRole(Role *role) { m_role = role; }

	void Init(Role *role);

	void OnYaojiangReq(int yaojiang_type);

private:
	Role *m_role;
};

#endif // __ROLEYAOJIANG__

