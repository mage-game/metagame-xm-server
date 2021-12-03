#ifndef __KILL_RANDOM_BOSS_HPP__
#define __KILL_RANDOM_BOSS_HPP__

class WorldEventManager;

#include "servercommon/worldeventdef.hpp"
#include "global/worldevent/worldevent.hpp"

class WorldEventKillRandBoss : public WorldEvent
{
public:
	WorldEventKillRandBoss(WorldEventManager *world_event_manager);
	virtual ~WorldEventKillRandBoss();

	virtual void Init(const WorldEventParam &ev_param);
	virtual void GetInitParam(WorldEventParam *ev_param);

	virtual void OnHurtMonster(Role *role, Monster *monster, int hurt);

	virtual void OnLogicStart();					
	virtual void OnLogicStop();

	virtual void Update(unsigned long interval, time_t now_second);

private:
	void ClearBoss();
	void FlushBoss();

	struct BossItem
	{
		BossItem() { this->Reset(); }

		void Reset() { scene_id = 0; objid = INVALID_OBJ_ID; boss_id = 0; }
		bool IsVaild() { return INVALID_OBJ_ID != objid; }

		int scene_id;
		ObjID objid;
		int boss_id;
	};
	
	int m_boss_count;
	BossItem m_boss_list[WORLD_EVENT_RANDOM_FLUSH_BOSS_MAX];		// ´æÔÚµÄboss

	time_t m_last_check_boss_alive_timestamp;
};


#endif // __KILL_RANDOM_BOSS_HPP__

