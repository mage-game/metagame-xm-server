#ifndef __RAND_ACTIVITY_PLANTING_TREE_HPP__
#define __RAND_ACTIVITY_PLANTING_TREE_HPP__

#include "global/randactivity/randactivity.hpp"
#include "servercommon/activitydef.hpp"
#include "servercommon/serverdef.h"

#include <map>
#include <set>

class RandActivityPlantingTree : public RandActivity
{
public:
	struct TreeInfo
	{
		TreeInfo() : owner_uid(0), vanish_time(0), watering_times(0)
		{
			memset(owner_name, 0, sizeof(owner_name));
		}

		int owner_uid;
		GameName owner_name;
		unsigned int vanish_time;
		char watering_times;
		std::set<int> watering_role_uid_list;		// 已经浇过树的玩家uid
	};

public:
	RandActivityPlantingTree(RandActivityManager *rand_activity_manager);
	virtual ~RandActivityPlantingTree();

	virtual void OnSpecialActivityStatusChange(int from_status, int to_status);

	virtual void Update(unsigned long interval, time_t now_second);

	void OnPlantingTree(int scene_id, ObjID obj_id, GameName role_name, int role_uid);
	bool OnWateringTree(int scene_id, ObjID obj_id, int role_uid);

	const TreeInfo* GetTreeInfo(long long key);
	int GetRolePlantingTreeCount(int role_uid);

	bool HasRoleWateredTree(int role_uid, long long key);

private:
	std::map<long long, TreeInfo> m_tree_list;				// key是树所在scene_id和树的obj_id的组合
	std::map<int, int> m_role_planting_count_map;			// key是玩家的uid，值是植树次数
};

#endif
