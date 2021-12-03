#ifndef __MONSTER_DROP_SUM_HPP__
#define __MONSTER_DROP_SUM_HPP__

#include <map>
#include <vector>

#include "gamedef.h"
#include "servercommon/basedef.h"

class Role;

class MonsterDropSum
{
public:
	static MonsterDropSum & Instance();

	void Update(unsigned long interval);

	void OnKillMonster(int monster_id);
	void OnMonsterDropItem(int monster_id, ItemID item_id, int num);

private:
	MonsterDropSum();
	~MonsterDropSum();

	static const int LOG_TIME_INTERVAL = 3600;

	typedef std::map<ItemID, int> DropItemMap;
	typedef std::map<ItemID, int>::iterator DropItemMapIt;

	struct MonsterDropInfo
	{
		MonsterDropInfo() : monster_die_count(0) {}

		int monster_die_count;
		DropItemMap drop_item_map;
	};

	typedef std::map<int, MonsterDropInfo> MonsterDropMap;
	typedef std::map<int, MonsterDropInfo>::iterator MonsterDropMapIt;

	MonsterDropMap m_monster_drop_map;

	unsigned int m_last_log_time;
	unsigned int m_last_drop_time;
};

#endif // __MONSTER_DROP_SUM_HPP__

