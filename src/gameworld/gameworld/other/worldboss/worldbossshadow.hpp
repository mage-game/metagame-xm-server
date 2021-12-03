#ifndef _WORLDBOSSSHADOW_HPP_
#define _WORLDBOSSSHADOW_HPP_

#include "servercommon/internalprotocal/gglobalprotocal.h"
#include <set>
#include <map>
#include "gameworld/gameworld/obj/character/character.h"
#include "gameworld/gameworld/obj/character/monster.h"
#include "gameworld/gameworld/global/worldboss/worldbossmanager.hpp"

class WorldBossShadow
{
public:
	static WorldBossShadow & Instance();

	void Update(unsigned long interval);
	
	void OnBossRefresh(WorldBossManager::WorldBossInfo &world_boss);

	void OnAttack(Role *attacker, Monster *monster, Attribute &delta_hp);
	void OnBeenAttack(Role *attacker, Monster *monster, UInt16 skill_id, Attribute delta_hp, bool from_skill);
	void OnMonsterDie(int monster_id, int killer_uid, GameName killer_name);

	bool IsWorldBossAndAlive(int boss_id);

private:
	WorldBossShadow();
	~WorldBossShadow();

	std::set<int> m_boss_set;

	unsigned int m_last_update_time;
};

#endif

