#ifndef __SPECIAL_WUSHUANG_FB_HPP__
#define __SPECIAL_WUSHUANG_FB_HPP__

#include "scene/speciallogic/speciallogic.hpp"

class SpecialWushuangFB : public SpecialLogic
{
public:
	SpecialWushuangFB(Scene *scene) : SpecialLogic(scene), m_owner_objid(INVALID_OBJ_ID), m_owner_user_id(INVALID_USER_ID), m_is_finish(false), m_is_pass(false), m_kick_out_timestamp(0) {}
	~SpecialWushuangFB() {}

	void OnReqNextLevel();

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

#endif

