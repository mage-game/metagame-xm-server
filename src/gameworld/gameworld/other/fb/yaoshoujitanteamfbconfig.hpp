#ifndef __YAOSHOUJITAN_TEAM_FB_CONFIG_HPP__
#define __YAOSHOUJITAN_TEAM_FB_CONFIG_HPP__

#include "servercommon/serverdef.h"
#include "servercommon/welfaredef.hpp"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include <vector>

struct YaoShouJiTanTeamFBModeConfig
{
	YaoShouJiTanTeamFBModeConfig() : mode(0), scene_id(0), enter_pos(0, 0), free_join_times(0), 
		boss_id(0), boss_pos(0, 0), change_time_gap(0), boss_js_per(0) {}

	int mode;
	int scene_id;
	Posi enter_pos;
	int free_join_times;
	int boss_id;
	Posi boss_pos;
	int change_time_gap;
	int boss_js_per;

	ItemConfigData reward_item1;
	ItemConfigData reward_item2;
	ItemConfigData reward_item3;
};

class YaoShouJiTanTeamFbConfig : public ConfigBase
{
public:
	YaoShouJiTanTeamFbConfig();
	~YaoShouJiTanTeamFbConfig();

	bool Init(std::string path, std::string *err);

	const YaoShouJiTanTeamFBModeConfig *GetFbModeCfg(int mode);
	int GetModeBySceneId(int scene_id);

	void GetDailyFindReward(int *item_count, ItemConfigData item_list[DailyFindRewardItem::ITEM_MAX_COUNT], int reward_factor = 1);

private:
	int InitModeList(PugiXmlNode RootElement);
	
	std::vector<YaoShouJiTanTeamFBModeConfig> m_teamfb_mode_vec;
};

#endif


