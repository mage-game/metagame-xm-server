#ifndef __QUAN_MIN_TA_FANG_HPP__
#define __QUAN_MIN_TA_FANG_HPP__

class WorldEventManager;

#include "servercommon/worldeventdef.hpp"
#include "global/worldevent/worldevent.hpp"

class WorldEventQuanmintafang : public WorldEvent
{
public:
	WorldEventQuanmintafang(WorldEventManager *world_event_manager);
	virtual ~WorldEventQuanmintafang();

	virtual void Init(const WorldEventParam &ev_param);
	virtual void GetInitParam(WorldEventParam *ev_param);

	virtual void OnMonsterDisapper(Monster *monster);
	virtual void OnKillMonster(Role *role, Monster *monster);
	virtual void Update(unsigned long interval, time_t now_second);
	virtual void OnLogicStart();						
	virtual void OnLogicStop();

private:
	void FlushMonster();

	void TowerAddScore();
	void FlushTower();
	void ClearTower();

	struct TowerItem
	{
		TowerItem() { this->Reset(); }

		void Reset() { scene_id = 0; pos = Posi(0, 0); obj_id = INVALID_OBJ_ID; }
		bool IsValid() { return INVALID_OBJ_ID != obj_id; }

		int scene_id;
		Posi pos;
		ObjID obj_id;
	};

	TowerItem m_tower_list[WORLD_EVENT_QUANMINGTAFANG_TOWER_MAX];

	int m_alive_tower_count;

	struct DoorItem
	{
		DoorItem() { this->Reset(); }

		void Reset() { scene_id = 0; obj_id = INVALID_OBJ_ID; belong_tower = -1; }
		bool IsValid() { return INVALID_OBJ_ID != obj_id; }

		int scene_id;
		ObjID obj_id;
		int belong_tower;
	};

	const static int DOOR_MAX_COUNT = WORLD_EVENT_QUANMINGTAFANG_TOWER_MAX * WORLD_EVENT_QUANMINGTAFANG_START_POS_MAX;

	int m_door_count;
	DoorItem m_door_list[DOOR_MAX_COUNT];

	time_t m_next_flush_time;
	time_t m_next_addscore_time;
};

#endif // __QUAN_MIN_TA_FANG_HPP__



