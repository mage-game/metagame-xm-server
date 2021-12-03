#ifndef __SHEN_MO_ZHI_XI_HPP__
#define __SHEN_MO_ZHI_XI_HPP__

class WorldEventManager;

#include "servercommon/worldeventdef.hpp"
#include "global/worldevent/worldevent.hpp"
#include "global/worldevent/worldeventconfig.hpp"

class WorldEventShenmozhiji : public WorldEvent
{
public:
	WorldEventShenmozhiji(WorldEventManager *world_event_manager);
	virtual ~WorldEventShenmozhiji();

	virtual void Init(const WorldEventParam &ev_param);
	virtual void GetInitParam(WorldEventParam *ev_param);

	virtual void OnLogicStart();					
	virtual void OnLogicStop();

	virtual void OnTouchEventObj(Role *role, int scene_id, ObjID obj_id);

private:
	struct PortalItem
	{
		PortalItem() { this->Reset(); }

		bool IsValid() { return 0 != scene_id; }
		void Reset() { scene_id = 0; obj_id = INVALID_OBJ_ID; }

		int scene_id;
		ObjID obj_id;
	};

	PortalItem * FindPortal(int scene_id, ObjID obj_id);
	void ClearAllPortal();
	void RefreshAllPortal();

	PortalItem m_portal_item_list[WORLD_EVENT_SHENMOZHIXI_PORTAL_MAX_COUNT];
	WorldEventPosCfg m_protal_pos_cfg;
};

#endif // __SHEN_MO_ZHI_XI_HPP__

