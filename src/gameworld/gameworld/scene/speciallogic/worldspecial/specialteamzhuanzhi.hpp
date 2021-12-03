#ifndef __SPECIAL_TEAM_ZHUANZHI_HPP__
#define __SPECIAL_TEAM_ZHUANZHI_HPP__

#include "scene/speciallogic/speciallogic.hpp"
#include "protocal/msgactivity.hpp"

#include <set>
#include <map>
#include "servercommon/serverdef.h"

class Role;
class Team;

class SpecialTeamZhuanzhi : public SpecialLogic
{
public:
	SpecialTeamZhuanzhi(Scene *scene);
	virtual ~SpecialTeamZhuanzhi();

	virtual bool SpecialCanMountOn(Role *role) { return false; }
	void OnFBFinish(bool is_passed);
	virtual void Update(unsigned long interval, time_t now_second);
	virtual bool OnTimeout();
	virtual void OnRoleEnterScene(Role *role);
	virtual void OnRoleLeaveScene(Role *role, bool is_logout);

protected:
	Role * GetOwnerRole(ObjID obj_id);
	virtual void OnAllMonsterDead();

	std::set<ObjID> m_member_objid_setlist; 
	std::set<UserID> m_member_user_id_setlist;
	Team *team;

	bool m_is_finish;
	bool m_is_pass;
	time_t m_kick_out_timestamp;
};

#endif // __SPECIAL_TEAM_ZHUANZHI_HPP__


