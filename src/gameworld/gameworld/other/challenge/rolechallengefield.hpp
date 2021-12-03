#ifndef __ROLE_CHALLENGEFILE_HPP__
#define __ROLE_CHALLENGEFILE_HPP__

class Role;

#include "obj/character/attribute.hpp"

class RoleChallengeField
{
public:
	RoleChallengeField();
	virtual ~RoleChallengeField();

	void SetRole(Role *role) { m_role = role; }

	void SendRoleCFBestRankBreakInfo();
	void RoleCFBestRankBreak();
	void ReCalcAttr(CharIntAttrs& base_add, bool is_recalc);

protected:
	Role *m_role;
	CharIntAttrs m_attrs_add;												// 各属性加成数值
};

#endif

