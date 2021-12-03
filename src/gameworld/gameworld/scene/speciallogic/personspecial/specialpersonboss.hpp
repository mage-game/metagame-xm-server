#ifndef __SPECIAL_PERSON_BOSS_HPP__
#define __SPECIAL_PERSON_BOSS_HPP__

#include "scene/speciallogic/speciallogic.hpp"

class SpecialPersonBoss : public SpecialLogic
{
public:
	SpecialPersonBoss(Scene *scene);
	virtual ~SpecialPersonBoss();

	virtual bool SpecialCanMountOn(Role *role) { return false; }
	virtual bool SpecialCanGuWu() { return true; }

	virtual void Update(unsigned long interval, time_t now_second);

	virtual void OnRoleEnterScene(Role *role);
	virtual void OnLeaveFB(Role *role);
	virtual void OnCharacterDie(Character *dead, ObjID killer_id);
	virtual bool OnTimeout();

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

	time_t m_flush_boss_timestamp;
	time_t m_kick_out_timestamp;
};

#endif // __SPECIAL_PERSON_BOSS_HPP__