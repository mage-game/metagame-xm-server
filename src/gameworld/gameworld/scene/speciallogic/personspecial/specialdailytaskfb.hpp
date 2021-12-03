#ifndef __DAILY_TASK_FB_H__
#define __DAILY_TASK_FB_H__

#include "scene/speciallogic/speciallogic.hpp"
#include "other/fb/dailytaskfbcofig.hpp"

class SpecialDailyTaskFb: public SpecialLogic
{
public:
	SpecialDailyTaskFb(Scene *scene);
	virtual ~SpecialDailyTaskFb();

	virtual void OnRoleEnterScene(Role *role);
	virtual void OnRoleLeaveScene(Role *role, bool is_logout);
	virtual void OnCharacterDie(Character *dead, ObjID killer_id);

	virtual void Update(unsigned long interval, time_t now_second);
	virtual void OnAddObj(Obj *obj);
	virtual void SpecialCancelMonsterStatic();

protected:

	void SyncFBSceneLogicInfo(bool is_active_leave_fb = false);	// 副本信息

	int m_owner_uid;				// 副本拥有者用户uid
	int m_daily_task_fb_type;
	bool m_is_pass;
	int m_fb_level;

	int m_finish_param;
	bool m_is_cancel_monster_static;

};

#endif