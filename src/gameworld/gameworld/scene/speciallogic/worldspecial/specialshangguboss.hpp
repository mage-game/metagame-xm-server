#ifndef _SPECIAL_SHANGGU_BOSS_HPP_
#define _SPECIAL_SHANGGU_BOSS_HPP_

#include "scene/speciallogic/speciallogic.hpp"
#include "protocal/msgother.h"

class SpecialShangGuBoss : public SpecialLogic
{
	static const int KICK_OUT_NEED_MAX_NUQI = 100; // 达到一定怒气就踢出去

	struct RoleFbInfo
	{
		RoleFbInfo(): angry_value(0), next_add_nuqi_time(0), kick_out_time(0)
		{
		}

		int angry_value;
		time_t next_add_nuqi_time;	// 下一次定时加怒气时间
		time_t kick_out_time;		// 踢出副本的时间
	};

public:
	SpecialShangGuBoss(Scene *scene);
	virtual ~SpecialShangGuBoss();

	virtual bool CanDestroy() { return false; }

	virtual void Update(unsigned long interval, time_t now_second);

	virtual void OnRoleEnterScene(Role *role);
	virtual void OnRoleLeaveScene(Role *role, bool is_logout);

	virtual void OnCharacterDie(Character *dead, ObjID killer_id);
	virtual void OnGather(Role *role, int gather_id, ObjID obj_id, const Posi &pos);
	virtual void OnPickFallingItem(Role *role, FallingItem *falling_item);

	virtual bool SpecialCanReAlive(Role *role, int realive_type);
	void AddAngryValueToDropOwner(Role *role, unsigned short monster_id);

private:

	void AddAngryValueToRole(int uid, int add_nuqi);
	void SendInfoToRole(Role *role);

	std::map<int, RoleFbInfo> m_role_info_map; // 玩家怒气值
};

#endif // _SPECIAL_SHANGGU_BOSS_HPP_
