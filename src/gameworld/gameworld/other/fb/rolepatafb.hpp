#ifndef __ROLE_FB_PATA__
#define __ROLE_FB_PATA__

#include "obj/character/attribute.hpp"

class Role;

class RolePataFB
{
public:
	RolePataFB();
	~RolePataFB();

	void SetRole(Role *role) { m_role = role; }

	void Init(Role *role, short pass_level, short today_level);
	void GetInitParam(short *pass_level, short *today_level);

	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);
	void OnEnterScene(int scene_id);
	void OnRoleLogin();
	bool CanEnter(int level);
	void OnFinish(int level);
	void AutoFBReq();

	int GetPassLevel() { return m_pass_level; }
	int GetTodayLevel() { return m_today_level; }

	void SendAllInfo();

	void GMSetPassLevel(int level) { m_pass_level = level; }
	void GMSetTodayLevel(int level) { m_today_level = level; }

private:
	Role *m_role;

	short m_pass_level;
	short m_today_level;
};

#endif


