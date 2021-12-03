#ifndef __XU_KONG_ZHI_MEN_HPP__
#define __XU_KONG_ZHI_MEN_HPP__

class WorldEventManager;

#include "servercommon/worldeventdef.hpp"
#include "global/worldevent/worldevent.hpp"

class WorldEventXukongzhimen: public WorldEvent
{
public:
	WorldEventXukongzhimen(WorldEventManager *world_event_manager);
	virtual ~WorldEventXukongzhimen();

	virtual void Init(const WorldEventParam &ev_param);
	virtual void GetInitParam(WorldEventParam *ev_param);

	virtual void Update(unsigned long interval, time_t now_second);
	virtual void OnLogicStart();						
	virtual void OnLogicStop();

	virtual void OnSetOtherSpecialAppearance(Role *role, int special_appearacne_type);

private:
	void FlushMonster();

	void FlushDoor();
	void ClearDoor();

	struct DoorItem
	{
		DoorItem() { this->Reset(); }

		void Reset() { scene_id = 0; obj_id = INVALID_OBJ_ID; }
		bool IsValid() { return INVALID_OBJ_ID != obj_id; }

		int scene_id;
		ObjID obj_id;
	};

	int m_door_count;
	DoorItem m_door_list[WORLD_EVENT_XUKONGZHIMENG_DOOR_MAX];
	
	time_t m_next_flush_monster_time;
};

#endif // __XU_KONG_ZHI_MEN_HPP__

