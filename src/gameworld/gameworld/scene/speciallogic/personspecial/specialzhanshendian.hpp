#ifndef _SPECIALZHANSHENDIAN_HPP_
#define _SPECIALZHANSHENDIAN_HPP_

#include "scene/speciallogic/speciallogic.hpp"

class SpecialZhanShenDian : public SpecialLogic
{
public:
	SpecialZhanShenDian(Scene *scene);
	virtual ~SpecialZhanShenDian();

	virtual bool SpecialCanMountOn(Role *role) { return false; }
	virtual void OnRoleEnterScene(Role *role);
	virtual bool OnTimeout();
	virtual void Update(unsigned long interval, time_t now_second);

	void OnReqNextLevel();

protected:
	Role * GetOwnerRole();

	void OnFinish(bool is_pass);
	virtual void OnAllMonsterDead();

	ObjID m_owner_objid;										// 副本拥有者对象ID
	UserID m_owner_user_id;										// 副本拥有者用户ID

	bool m_is_finish;											// 是否结束 
	bool m_is_pass;												// 是否通关

	UInt16 m_monster_id; 
};

#endif


