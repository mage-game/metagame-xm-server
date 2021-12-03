#ifndef __MIGONGXIANFU_TEAM_FB_CONFIG_HPP__
#define __MIGONGXIANFU_TEAM_FB_CONFIG_HPP__

#include "servercommon/serverdef.h"
#include "servercommon/welfaredef.hpp"
#include "servercommon/struct/itemlistparam.h"
#include <map>

#include "servercommon/fbdef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

struct MiGongXianFuLayerItemCfg
{
	MiGongXianFuLayerItemCfg() : mode(0), layer(0), layer_type(0), scene_id(0), enter_pos(0, 0),
		boss_id(0), boss_pos(0, 0), 
		to_prve_weight(0), to_hide_weight(0), to_next_weight(0), to_here_weight(0) {}

	int mode;
	int layer;
	int layer_type;
	int scene_id;
	Posi enter_pos;
	int boss_id;
	Posi boss_pos;

	int to_prve_weight;
	int to_hide_weight;
	int to_next_weight;
	int to_here_weight;

	Posi door_list[MIGONGXIANFU_DOOR_NUM];
};

struct MiGongXianFuModeItemCfg
{
	MiGongXianFuModeItemCfg() : mode(0), free_join_times(0), time(0) {}

	int mode;
	int free_join_times;
	int time;

	ItemConfigData reward_item1;
	ItemConfigData reward_item2;
	ItemConfigData reward_item3;

	MiGongXianFuLayerItemCfg layer_list[MIGONGXIANFU_LAYER_NUM];
};

class MiGongXianFuTeamFbConfig : public ConfigBase
{
public:
	MiGongXianFuTeamFbConfig();
	~MiGongXianFuTeamFbConfig();

	bool Init(std::string path, std::string *err);

	const MiGongXianFuModeItemCfg *GetModeItemCfg(int mode);
	const MiGongXianFuLayerItemCfg *GetLayerItemCfgBySceneId(int scene_id);
	const MiGongXianFuLayerItemCfg *GetLayerItemCfg(int mode, int layer);
	void GetDailyFindReward(int *item_count, ItemConfigData item_list[DailyFindRewardItem::ITEM_MAX_COUNT], int reward_factor = 1);
	
private:
	int InitModeList(PugiXmlNode RootElement);
	int InitLayerList(PugiXmlNode RootElement);
	
	std::map<int, MiGongXianFuModeItemCfg> m_mode_map;
};

#endif


