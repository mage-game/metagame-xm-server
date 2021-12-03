#ifndef __SPECIAL_GUILDQUESTION_HPP__
#define __SPECIAL_GUILDQUESTION_HPP__

#include "scene/speciallogic/speciallogic.hpp"
#include "scene/activityshadow/activityshadow.hpp"

class Role;

class SpecialGuildQuestion : public SpecialLogic
{
public:
	SpecialGuildQuestion(Scene *scene);
	virtual ~SpecialGuildQuestion();

	virtual bool CanDestroy() { return !m_is_open; }

	virtual void Update(unsigned long interval, time_t now_second);

	virtual void OnRoleEnterScene(Role *role);
	virtual void OnRoleLeaveScene(Role *role, bool is_logout);

	virtual bool SpecialCanGather(Role *role, GatherObj *gather);
	virtual bool SpecialIsEnemy(Role *role, Obj *obj) { return false; }
	virtual bool SpecialIsFriend(Role *role, Obj *obj, bool check_alive = true) { return true; }

	virtual bool SpecialCanMountOn(Role *role) { return false; }
	virtual bool SpecialCanCallPet(Role *role) { return false; }
	virtual bool SpecialCanPerformCommonSkill(Role *role) { return false; }

	void OnActivityStart();
	void OnActivityClose();

protected:
	void NotifySystemMsg(char *str_buff, int str_len);
	void RefreshGatherItem();

	bool m_is_init;
	bool m_is_open;
	bool m_is_first;
};

#endif // __SPECIAL_GUILDQUESTION_HPP__

