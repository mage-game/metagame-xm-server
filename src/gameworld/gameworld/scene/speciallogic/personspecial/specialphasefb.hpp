#ifndef __SPECIAL_PHASEFB_HPP__
#define __SPECIAL_PHASEFB_HPP__

#include "scene/speciallogic/speciallogic.hpp"

class SpecialPhaseFB : public SpecialLogic
{
public:
	SpecialPhaseFB(Scene *scene);
	virtual ~SpecialPhaseFB();

	virtual void Update(unsigned long interval, time_t now_second);
	virtual void OnRoleEnterScene(Role *role);
	virtual void OnLeaveFB(Role *role);
	virtual bool SpecialRelive(Role *role, int realive_type);
	virtual void OnCharacterDie(Character *dead, ObjID killer_id);
	virtual bool OnTimeout();
	virtual bool SpecialCanMountOn(Role *role) { return false; }
	virtual void OnAddObj(Obj *obj);
	virtual void SpecialCancelMonsterStatic();
	virtual bool CanDestroy() { return m_is_finish; }
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
	int m_is_cancel_monster_static;
	unsigned int m_pass_kick_out_time;
};

#endif // __SPECIAL_EXPFB_HPP__


