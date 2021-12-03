#ifndef __FIND_NPC_HPP__
#define __FIND_NPC_HPP__

class WorldEventManager;

#include "servercommon/worldeventdef.hpp"
#include "global/worldevent/worldevent.hpp"
#include "global/worldevent/worldeventconfig.hpp"

class WorldEventFindNpc : public WorldEvent
{ 
public:
	WorldEventFindNpc(WorldEventManager *world_event_manager);
	virtual ~WorldEventFindNpc();

	virtual void Init(const WorldEventParam &ev_param);
	virtual void GetInitParam(WorldEventParam *ev_param);

	virtual void Update(unsigned long interval, time_t now_second);

	virtual void OnLogicStand();
	virtual void OnLogicStart();					
	virtual void OnLogicStop();
	virtual void OnTouchEventObj(Role *role, int scene_id, ObjID obj_id);
	virtual void OnKillMonster(Role *role, Monster *monster);

	void GetNpcLocateInfo(Role *role, int npc_id);

private:
	struct NPCItem
	{
		NPCItem() { this->Reset(); }

		void Reset()
		{
			npc_id = -1;
			scene_id = 0;
			obj_id = INVALID_OBJ_ID;
		}

		bool IsValid() { return npc_id > 0; }

		int npc_id;
		int scene_id;
		ObjID obj_id;
	};

	NPCItem * FindNpc(int scene_id, ObjID obj_id);
	void ClearAllNPC();
	void RefreshAllNPC();

	NPCItem m_npc_list[WORLD_EVENT_NPC_MAX_COUNT];

	WorldEventNpcPosConfig m_npc_pos_cfg;
};

#endif // __FIND_NPC_HPP__


