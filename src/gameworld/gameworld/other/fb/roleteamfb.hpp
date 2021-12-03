#ifndef __ROLE_TEAM_FB_HPP__
#define __ROLE_TEAM_FB_HPP__

class Role;

class RoleTeamFB
{
public:
	RoleTeamFB();
	~RoleTeamFB();

	void SetRole(Role *role) { m_role = role; }

	void Init(Role *role);
	void GetInitParam();

	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);
	void SendInfo();

private:
	Role *m_role;
};

#endif

