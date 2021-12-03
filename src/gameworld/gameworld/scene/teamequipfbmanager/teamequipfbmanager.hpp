#ifndef _TEAMEQUIPFB_MANAGER_HPP_
#define _TEAMEQUIPFB_MANAGER_HPP_

#include "servercommon/servercommon.h"

#include <map>
#include <set>
#include "protocal/msgfb.h"

/*
	精英组队装备本
*/

class Role;
class FallingItem;
class ItemDataWrapper;

class TeamEquipFBManager
{
public:
	static TeamEquipFBManager & Instance();

	void Update(unsigned long interval);

	void OnEnterScene(Role *role, int scene_key, int scene_id);
	void OnLeaveScene(Role *role, int scene_key);
	void OnLayerFinish(int scene_key, int scene_id, bool is_pass, bool can_jump, int finish_time);
	void OnPickFallingItem(Role *role, FallingItem *falling_item, ItemDataWrapper *item_wrapper, int scene_key);
	void OnAddPassExp(Role *role, int scene_key, long long addexp);
	int GetTotalFallItem(Role *role, int scene_key, Protocol::SCEquipFBResult::FallItemInfo *total_fall_item_list, const int max_list_count);
	bool GetIsTeamAllOver(int scene_key);
	unsigned int GetTeamKickOutTimestamp(int scene_key);

private:
	TeamEquipFBManager();
	~TeamEquipFBManager();

	struct FBUser
	{
		FBUser() : user_id(INVALID_USER_ID), user_camp(0)
		{
			memset(user_name, 0, sizeof(user_name)); 
			memset(total_fall_item_list, 0, sizeof(total_fall_item_list));
		}

		UserID user_id;
		GameName user_name;
		char user_camp;
		Protocol::SCEquipFBResult::FallItemInfo total_fall_item_list[NEQFB_PICK_FALL_ITEM_RECORD_COUNT];

	};

	struct EquipFBTeam
	{
		EquipFBTeam() : is_allover(false), active_number(0),fb_start_time(0), kick_out_timestamp(0)
		{ 
			memset(total_pass_exp_list, 0, sizeof(total_pass_exp_list)); 
		}

		bool is_allover;
		char active_number;
		FBUser user_list[MAX_TEAM_MEMBER_NUM];
		long long total_pass_exp_list[MAX_TEAM_MEMBER_NUM];
		unsigned int fb_start_time;
		unsigned int kick_out_timestamp;
	};

	void SendTotalPassExp(Role *role, long long total_pass_exp);
	void SyncResultToTeamRank(EquipFBTeam *fbteam, int scene_id, int finish_time);
	long long GetEquipFBRankValueHelper(int layer, int finish_time);

	typedef std::map<int, EquipFBTeam> EquipFBTeamMap;
	typedef std::map<int, EquipFBTeam>::iterator EquipFBTeamMapIt;
	EquipFBTeamMap m_equipfb_team_map;

	std::set<int> m_scenekey_to_delete_set;

	unsigned long m_clear_fb_interval;
};

#endif