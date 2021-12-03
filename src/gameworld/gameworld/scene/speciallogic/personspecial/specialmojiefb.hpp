#ifndef __SPECIAL_MOJIEFB_HPP__
#define __SPECIAL_MOJIEFB_HPP__

#include "scene/speciallogic/speciallogic.hpp"

class SpecialMoJieFB : public SpecialLogic
{
public:
	SpecialMoJieFB(Scene *scene);
	virtual ~SpecialMoJieFB();

	virtual void OnRoleEnterScene(Role *role);
	virtual void OnCharacterDie(Character *dead, ObjID killer_id);
	virtual bool SpecialCanMountOn(Role *role) { return false; }

protected:
	Role * GetOwnerRole();

	void OnFinish();

	void SyncFBSceneLogicInfo(bool is_active_leave_fb = false);

	ObjID m_owner_objid; 
	UserID m_owner_user_id;

	bool m_is_finish;														// �Ƿ����
	bool m_is_passed;														// �Ƿ�ͨ��

	int m_fb_id;

};


#endif // __SPECIAL_MOJIEFB_HPP__