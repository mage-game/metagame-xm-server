#ifndef __SPECIAL_MILLIONAIRE_HPP__
#define __SPECIAL_MILLIONAIRE_HPP__

#include "scene/speciallogic/speciallogic.hpp"

class SpecialMillionaire : public SpecialLogic
{
public:

	SpecialMillionaire(Scene *scene);
	virtual ~SpecialMillionaire();

	void OnActivityStatusChange(int status);

	virtual bool CanDestroy();
	virtual void OnGather(Role *role, int gather_id, ObjID obj_id, const Posi &pos);
	virtual void OnCharacterDie(Character *dead, ObjID killer_id);

	virtual void OnRoleEnterScene(Role *role);
	virtual void OnRoleLeaveScene(Role *role, bool is_logout);

	void OnUseFrozenSkill(Role *role, ObjID target_obj_id);

protected:

	void OnActivityStandby();
	void OnActivityStart();
	void OnActivityClose();

	void SyncGlobalInfoToAll();
	void SyncGlobalInfo(Role *role);
	void SyncRoleInfo(Role *role);
	void NotifySystemMsg(char *str_buff, int str_len, int notice_type);

	bool m_is_finish;
};

#endif // __SPECIAL_MILLIONAIRE_HPP__
