#ifndef _ROLEEQUIPFB_MANAGER_HPP_
#define _ROLEEQUIPFB_MANAGER_HPP_

#include "servercommon/servercommon.h"
#include "other/fb/neqfbconfig.hpp"

#include <map>

class Role;

class RoleEquipFBManager
{
public:
	static RoleEquipFBManager & Instance();

	void Update(unsigned long interval);

	void OnEnterScene(Role *role, int scene_key, int scene_id);
	void OnLeaveScene(Role *role, int scene_key);

	void CheckRollState(Role *role);
	void OnNeqRollReq(Role *role);
	void SendNeqRollInfo(Role *role, short reason, short hit_seq);
	void StartRoll(Role *role, int star, int chapter, int level);

private:
	RoleEquipFBManager();
	~RoleEquipFBManager();

	char * RollItemQuickLog(const NeqRollItemToRole &roll_item);
	char * RollItemListQuickLog(NeqRollItemToRole roll_list[NEQFB_ROLLPOOL_TOTAL_COUNT]);
	int RandRollItemHelper(int already_roll_times, bool hit_list[NEQFB_ROLLPOOL_TOTAL_COUNT], NeqRollItemToRole roll_list[NEQFB_ROLLPOOL_TOTAL_COUNT]);

	struct RoleRollInfo
	{
		int max_free_roll_times;									// 临时变量不保存
		int free_rolled_times;									// 临时变量不保存
		int gold_rolled_times;									// 临时变量不保存
		int roll_chapter;											// 临时变量不保存
		int roll_level;											// 临时变量不保存

		NeqRollItemToRole roll_list[NEQFB_ROLLPOOL_TOTAL_COUNT];
		bool hit_list[NEQFB_ROLLPOOL_TOTAL_COUNT];
	};

	typedef std::map<int, RoleRollInfo> RoleRollInfoMap;
	typedef std::map<int, RoleRollInfo>::iterator RoleRollInfoMapIt;

	RoleRollInfoMap m_role_roll_info_map;
};

#endif