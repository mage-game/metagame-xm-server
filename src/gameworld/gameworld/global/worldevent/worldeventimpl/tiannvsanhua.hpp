#ifndef __TIAN_NV_SAN_HUA_HPP__
#define __TIAN_NV_SAN_HUA_HPP__

class WorldEventManager;

#include "servercommon/worldeventdef.hpp"
#include "global/worldevent/worldevent.hpp"

class WorldEventTiannvshanhua : public WorldEvent
{
public:
	WorldEventTiannvshanhua(WorldEventManager *world_event_manager);
	virtual ~WorldEventTiannvshanhua();

	virtual void Init(const WorldEventParam &ev_param);
	virtual void GetInitParam(WorldEventParam *ev_param);

	virtual void Update(unsigned long interval, time_t now_second);

	virtual void OnLogicStart();				
	virtual void OnLogicStop();

private:
	void CheckRefreshFallingItem(time_t now_second);
	void CalMonsterMovePos(time_t now_second);
	void RefreshAllMonster();
	void ClearAllMonster();

	struct MonsterItem
	{
		MonsterItem() { this->Reset(); }

		bool IsValid() { return INVALID_OBJ_ID != obj_id; }

		void Reset() { obj_id = INVALID_OBJ_ID; curr_move_pos_index = 0; rest_time_s = 0; }


		ObjID obj_id;
		int curr_move_pos_index;
		int rest_time_s;
	};

	MonsterItem m_monster_item[WORLD_EVENT_TIANNVSHANHUA_MONSTER_COUNT];
	time_t m_last_refresh_falling_item_timestamp;
	time_t m_last_cal_monster_move_pos_timestamp;
};


#endif // __TIAN_NV_SAN_HUA_HPP__

