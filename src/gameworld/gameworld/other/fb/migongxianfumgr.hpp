#ifndef __MIGONGXIANFUMGR_HPP__
#define __MIGONGXIANFUMGR_HPP__

#include <map>
#include "protocal/msgfb.h"
#include "servercommon/fbdef.hpp"
class Role;

struct MgxfTeamFbLogicData
{
	MgxfTeamFbLogicData() 
	{
		this->Reset();
	}

	void Reset()
	{
		is_finish = false;
		is_pass = false;
		is_entered_hide = false;
		time_out_stamp = 0;
		start_time = 0;

		mode = 0;
		team_index = 0;
		kill_hide_boss_num = 0;
		kill_end_boos_num = 0;
	}

	bool is_finish;
	bool is_pass;
	bool is_entered_hide;
	unsigned int time_out_stamp;
	unsigned int start_time;

	int mode;
	int team_index;
	int kill_hide_boss_num;
	int kill_end_boos_num;
	
	struct LayerItem
	{
		LayerItem ()
		{
			this->Reset();
		}

		void Reset()
		{
			layer = 0;
			memset(door_status_list, 0, sizeof(door_status_list));
		}

		int layer;
		int door_status_list[MIGONGXIANFU_DOOR_NUM];
	};
	
	LayerItem layer_list[MIGONGXIANFU_LAYER_NUM];
};

class MiGongXianFuMgr
{
public:
	static MiGongXianFuMgr & Instance();
	void OnDestroyTeam(int team_index);
	void OnAddTeam(int team_index);

	MgxfTeamFbLogicData *GetLogicData(int team_index);
	MgxfTeamFbLogicData::LayerItem *GetLayerItem(int team_index, int layer);
	int GetChangeSceneKey(int team_index, int layer);

	int OnTouchDoor(int team_index, int layer, int door_index);
	bool CheckMustToHide(int team_index);

	void PutPassReward(int team_index);
	void SyncSceneLogicInfo(int team_index);

private:
	MiGongXianFuMgr(); 
	~MiGongXianFuMgr(); 

	typedef std::map<int, MgxfTeamFbLogicData> LogicDataMap;
	LogicDataMap m_data_map;
};

#endif
