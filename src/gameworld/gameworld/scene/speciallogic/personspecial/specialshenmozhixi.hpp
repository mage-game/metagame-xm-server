#ifndef __SPECIAL_SHENMOZHIXI_HPP__
#define __SPECIAL_SHENMOZHIXI_HPP__

#include "scene/speciallogic/speciallogic.hpp"

class SpecialShenmozhixi : public SpecialLogic
{
public:
	SpecialShenmozhixi(Scene *scene);
	virtual ~SpecialShenmozhixi();

	virtual void Update(unsigned long interval, time_t now_second);
	virtual bool SpecialCanMountOn(Role *role) { return false; }

	virtual void OnRoleEnterScene(Role *role);

protected:	
	virtual void OnAllMonsterDead();

	bool m_is_finish;

	time_t kick_user_time;

	ObjID m_user_objid;
};

#endif

