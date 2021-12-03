#ifndef __SPECIAL_FB_GUIDE_H__
#define __SPECIAL_FB_GUIDE_H__

#include "scene/speciallogic/speciallogic.hpp"
#include "other/fb/fbguideconfig.hpp"
#include "protocal/msgfb.h"

class SpecialFbGuide: public SpecialLogic
{
 public:
	SpecialFbGuide(Scene *scene);
	virtual ~SpecialFbGuide();

	virtual void OnRoleLeaveScene(Role *role, bool is_logout);

	void OnFinish(Role *role, int monster_x, int monster_y, int monster_id);
	void OnCreateGather(Role *role, int gather_x, int gather_y, int gather_id, int gather_time);
	void SetObjPos(Role * role, Protocol::CSFunOpenSetObjToPos *cmd);

protected:
	bool m_is_finish;
};

#endif
