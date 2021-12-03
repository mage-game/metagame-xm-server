#ifndef __SPECIAL_MIGONGXIANFU_FB_HPP__
#define __SPECIAL_MIGONGXIANFU_FB_HPP__

#include "scene/speciallogic/speciallogic.hpp"
#include "gameworld/other/fb/migongxianfumgr.hpp"

class SpecialMiGongXianFuTeamFb : public SpecialLogic
{
public:
	SpecialMiGongXianFuTeamFb(Scene *scene);
	~SpecialMiGongXianFuTeamFb();

	virtual void Update(unsigned long interval, time_t now_second);
	virtual bool SpecialCanGuWu() { return true; }

	virtual void OnRoleEnterScene(Role *role);
	virtual bool SpecialIsEnemy(Role *role, Obj *obj);
	void OnTouchDoor(Role *role, int layer, int door_index);
	virtual void OnRemoveObj(Obj *obj);
	virtual void OnLeaveFB(Role *role);
	virtual bool SpecialCanMountOn(Role *role){return false;}

	MgxfTeamFbLogicData *GetLogicData();

private:
	bool IsInTeam(int uid);
	void OnFinish(bool is_pass);

	bool m_is_init;
	int m_layer;
	int m_team_index;
	//MgxfTeamFbLogicData *m_logic_data;
};

#endif

