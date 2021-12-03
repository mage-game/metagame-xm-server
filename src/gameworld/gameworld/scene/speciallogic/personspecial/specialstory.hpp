#ifndef __SPECIAL_STORYFB_HPP__
#define __SPECIAL_STORYFB_HPP__

#include "scene/speciallogic/speciallogic.hpp"
#include "other/fb/storyfbconfig.hpp"

class SpecialStoryFB : public SpecialLogic
{
public:
	SpecialStoryFB(Scene *scene);
	virtual ~SpecialStoryFB();

	virtual void OnRoleEnterScene(Role *role);
	virtual bool SpecialRelive(Role *role, int realive_type);
	virtual void OnCharacterDie(Character *dead, ObjID killer_id);
	virtual bool OnTimeout();
	virtual bool SpecialCanMountOn(Role *role) { return false; }

protected:
	Role * GetOwnerRole();

	void OnFinish();

	void SyncFBSceneLogicInfo(bool is_active_leave_fb = false);

	ObjID m_owner_objid; 
	UserID m_owner_user_id;

	bool m_is_finish;														// 是否结束
	bool m_is_passed;														// 是否通关

	int m_total_boss_num;
	int m_total_allmonster_num;
	int m_kill_boss_num;
	int m_kill_allmonster_num;
};

#endif // __SPECIAL_EXPFB_HPP__