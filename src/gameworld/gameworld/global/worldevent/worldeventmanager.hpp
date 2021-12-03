#ifndef __WORLD_EVENT_MANAGER_HPP__
#define __WORLD_EVENT_MANAGER_HPP__

class WorldEvent;
class Role;
class Monster;
class RoleShadow;

#include "servercommon/worldeventdef.hpp"

class WorldEventManager
{
public:
	static WorldEventManager & Instance();

	void Init(const WorldEventParam &ev_param);
	void GetInitParam(WorldEventParam *ev_param);

	void Update(unsigned long interval, time_t now_second);
	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);
	void OnUserLogin(Role *role);

	void OnMonsterDisapper(Monster *monster);
	void OnKillMonster(Role *role, Monster *monster);
	void OnHurtMonster(Role *role, Monster *monster, long long hurt);
	void OnHurtRoleShadow(Role *role, RoleShadow *monster, long long hurt);
	void OnTouchEventObj(Role *role, int scene_id, ObjID obj_id);
	void OnSetSpecialOtherAppearance(Role *role, int special_appearacne_type);

	void SendCommonInfo(Role *role);

	void ForceChangeLogicState(int event_type, int from_state, int to_state);
	void GmNextEventType(Role *role);
	void GmNextLogicState(Role *role);
	void AddScore(int event_type, int add_score);
	void GmAddScore(Role *role, int add_score);
	int GetCurrEventScore();

	int GetLogicState() { return m_logic_state; }
	int GetEventType() { return m_curr_event_type; }

	void GetNpcLocateInfo(Role *role, int npc_id);

	int RandSpecialAppearanceParam();

private:
	WorldEventManager();
	~WorldEventManager();

	bool IsEvFinish(int ev_type, int score);
	time_t CalcNextLogicTime(int ev_type, int curr_logic_type, time_t curr_logic_timestamp);
	void UpdateEventLogic(unsigned long interval, time_t now_second);

	WorldEvent * GetCurrWorldEvent();

	int m_curr_event_type;

	int m_logic_state;													// 逻辑状态
	time_t m_next_logic_timestamp;										// 下次逻辑时间

	WorldEvent *m_world_event_list[WORLD_EVENT_TYPE_MAX];				// 当前开放的世界事件
};

#endif // __WORLD_EVENT_MANAGER_HPP__

