#ifndef __SPECIAL_PERSONAL_ZHUANZHI_HPP__
#define __SPECIAL_PERSONAL_ZHUANZHI_HPP__

#include "scene/speciallogic/speciallogic.hpp"
#include "protocal/msgactivity.hpp"

#include <map>
#include "servercommon/serverdef.h"

class Role;

class SpecialPersonalZhuanzhi : public SpecialLogic
{
public:
	SpecialPersonalZhuanzhi(Scene *scene);
	virtual ~SpecialPersonalZhuanzhi();

	virtual bool SpecialCanMountOn(Role *role) { return false; }
	void OnFBFinish(bool is_passed);
	virtual void Update(unsigned long interval, time_t now_second);
	virtual bool OnTimeout();
	virtual void OnRoleEnterScene(Role *role);

protected:
	Role * GetOwnerRole();
	virtual void OnAllMonsterDead();

	ObjID m_owner_objid; 
	UserID m_owner_user_id;

	bool m_is_finish;
	bool m_is_pass;
	time_t m_kick_out_timestamp;
};

#endif // __SPECIAL_PERSONAL_ZHUANZHI_HPP__


