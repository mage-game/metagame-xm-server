#ifndef __WORLD_EVENT_HPP__
#define __WORLD_EVENT_HPP__

class WorldEventManager;
class Role;
class Monster;
class RoleShadow;

#include "servercommon/worldeventdef.hpp"

class WorldEvent
{
public:
	WorldEvent(WorldEventManager *world_event_manager, int event_type);
	virtual ~WorldEvent();

	virtual void Init(const WorldEventParam &ev_param) = 0;
	virtual void GetInitParam(WorldEventParam *ev_param) = 0;

	virtual void Update(unsigned long interval, time_t now_second) {}

	virtual void OnEventStart() {}
	virtual void OnEventStop();

	virtual void OnMonsterDisapper(Monster *monster) {}
	virtual void OnKillMonster(Role *role, Monster *monster) {}
	virtual void OnHurtMonster(Role *role, Monster *monster, int hurt) {}
	virtual void OnHurtRoleShadow(Role *role, RoleShadow *role_shadow, int hurt) {}
	virtual void OnTouchEventObj(Role *role, int scene_id, ObjID obj_id) {}
	virtual void OnSetOtherSpecialAppearance(Role *role, int special_appearacne_type) {}

	virtual void OnLogicStand();
	virtual void OnLogicStart() {}							// 一次事件会调用很多次世界逻辑
	virtual void OnLogicStop() {}

	int GetEvType() { return m_event_type; }
	int GetScore() { return m_score; }

	void AddScore(int add_score);

protected:
	void ForceEndLogicState();

	WorldEventManager *m_world_event_manger;
	int m_event_type;
	int m_score;
};

#endif // __WORLD_EVENT_HPP__

