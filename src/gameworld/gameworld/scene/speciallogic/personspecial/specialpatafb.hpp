#ifndef __SPECIAL_PATA_FB_H__
#define __SPECIAL_PATA_FB_H__

#include "scene/speciallogic/speciallogic.hpp"

class SpecialPataFb: public SpecialLogic
{
public:
	SpecialPataFb(Scene *scene);
	virtual ~SpecialPataFb();

	virtual bool SpecialCanMountOn(Role *role) { return false; }
	virtual void OnRoleEnterScene(Role *role);
	virtual void OnRoleLeaveScene(Role *role, bool is_logout);
	virtual bool OnTimeout();
	virtual bool SpecialRelive(Role *role, int realive_type);
	virtual void Update(unsigned long interval, time_t now_second);
	virtual bool SpecialCanGuWu() { return true; } 
	virtual void OnCharacterDie(Character *dead, ObjID killer_id);

	void OnReqNextLevel();

protected:
	Role * GetOwnerRole();

	void OnFinish(bool is_pass);
	void SyncFBSceneLogicInfo(bool is_active_leave_fb = false);					// 副本信息

	virtual void OnAllMonsterDead();
	
	ObjID m_owner_objid;										// 副本拥有者对象ID
	UserID m_owner_user_id;										// 副本拥有者用户ID
	
	bool m_is_finish;											// 是否结束 
	bool m_is_pass;												// 是否通关

	UInt16 m_monster_id; 
	unsigned int m_die_timestamp;
};

#endif


