#ifndef __SPECIAL_VIP_FB_H__
#define __SPECIAL_VIP_FB_H__

#include "scene/speciallogic/speciallogic.hpp"

class SpecialVipFb: public SpecialLogic
{
public:
	SpecialVipFb(Scene *scene);
	virtual ~SpecialVipFb();
	virtual void Update(unsigned long interval, time_t now_second);
	virtual bool SpecialRelive(Role *role, int realive_type);
	virtual void OnRoleLeaveScene(Role *role, bool is_logout);
	virtual void OnRoleEnterScene(Role *role);
protected:
	Role * GetOwnerRole();

	void OnFinish(bool is_pass);
	void SyncFBSceneLogicInfo(bool is_active_leave_fb = false);	// 副本信息
	
	ObjID m_owner_objid;										// 副本拥有者对象ID
	UserID m_owner_user_id;										// 副本拥有者用户ID
	
	bool m_is_finish;											// 是否结束
	bool m_is_pass;												// 是否通关

	UInt16 m_monster_id;
};

#endif


