#ifndef __CLONE_ROLE_HPP__
#define __CLONE_ROLE_HPP__

#include "obj/character/role.h"
#include "obj/character/roleshadow.hpp"
#include "servercommon/struct/global/challengeuserparam.hpp"

class RoleClone
	: public RoleShadow
{
public:
	RoleClone(int role_shadow_type);
	void Init(Role * role, float per, int continue_time, ObjID enemy_id);

	virtual	void Update(unsigned long interval, time_t now_second, unsigned int now_dayid);

	void *operator new(size_t c);
	void operator delete(void *m);

private:
	ChallengeParam ConstructChallengeParam(Role *role, RoleAppearance &role_appearance, float per = 1);

	int m_end_time;
	ObjID m_enemy_obj_id;
};

#endif //__CLONE_ROLE_HPP__