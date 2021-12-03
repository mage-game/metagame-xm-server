#ifndef __SPECIAL_XINMO_FB_HPP__
#define __SPECIAL_XINMO_FB_HPP__

#include "scene/speciallogic/speciallogic.hpp"
#include <map>
#include "monster/monsterinitparam.h"

class SpecialXinMoFb : public SpecialLogic
{
public:
	SpecialXinMoFb(Scene *scene);
	virtual ~SpecialXinMoFb();

	virtual bool SpecialCanMountOn(Role *role) { return false; }
	virtual bool SpecialCanGuWu() { return true; }

	virtual void Update(unsigned long interval, time_t now_second);

	virtual void OnRoleEnterScene(Role *role);
	virtual void OnLeaveFB(Role *role);
	virtual void OnCharacterDie(Character *dead, ObjID killer_id);

	void OnFBFinish(bool is_passed);

protected:
	Role * GetOwnerRole(ObjID obj_id);

	bool FlushMonster();
	void SyncFBSceneLogicInfo(bool is_active_leave_fb = false);

	ObjID m_owner_objid;
	UserID m_owner_user_id;

	bool m_is_finish;
	bool m_is_pass;
	bool m_is_flushboss;

	time_t m_kick_out_timestamp;
};

#endif // __SPECIAL_XINMO_FB_HPP__