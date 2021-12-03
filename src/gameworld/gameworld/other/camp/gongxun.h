#ifndef _GONG_XUN_RANK_HPP_
#define _GONG_XUN_RANK_HPP_
#include "other/camp/campconfig.hpp"
#include  "servercommon/campdef.hpp"
class  Role;

class GongXun
{
public:
	GongXun();
	~GongXun();

	void SetRole(Role *role) { m_role = role; }
	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);
	void CheckGongXunGiveReward();

protected:
	Role *m_role;
};

#endif