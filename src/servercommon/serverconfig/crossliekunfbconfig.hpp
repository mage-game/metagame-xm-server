#ifndef __CROSS_LIEKUNFB_CONFIG_HPP__
#define __CROSS_LIEKUNFB_CONFIG_HPP__

#include "servercommon/crossdef.hpp"
#include "servercommon/servercommon.h"
#include "servercommon/activitydef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

#include "servercommon/struct/itemlistparam.h"
#include "servercommon/serverdef.h"

#include <vector>
#include <map>

static const int LIEKUN_MAX_GATHER_ITEM_COUNT = 5;					// 采集获得最多固定物品数
static const int LIEKUN_MAX_ITEM_COUNT = 10;						// 采集获得最多物品数

struct CrossLieKunFbOtherCfg
{
	CrossLieKunFbOtherCfg() : is_open(false), open_level(0), enter_time_limit_s(0), refresh_zone_boss_s(0), 
		refresh_minor_zone_boss_s(0), refresh_main_zone_boss_s(0), relive_need_gold(0), gather_time_s(0), 
		gather_times(0), portal_id(0), portal_item_offset(0), portal_time_s(0), effective_range(0), title_id(0)
	{
	}

	bool is_open;
	int open_level;
	int enter_time_limit_s;
	int refresh_zone_boss_s;
	int refresh_minor_zone_boss_s;
	int refresh_main_zone_boss_s;
	int relive_need_gold;
	int gather_time_s;
	int gather_times;
	ItemConfigData title_item;

	int portal_id;
	Posi portal_flush_pos;
	int portal_item_offset;
	int portal_time_s;
	int effective_range;
	int title_id;
	
};

struct CrossLieKunFbActivityOpenTimeCfg
{
	CrossLieKunFbActivityOpenTimeCfg() : activity_week_day(0), activity_ready_time(0), activity_start_time(0), activity_end_time(0), ready_dur_s(0), open_dur_s(0) {}

	int activity_week_day;													// 活动所在周几
	int activity_ready_time;												// 活动等待时间点
	int activity_start_time;												// 活动开始时间点
	int activity_end_time;													// 活动结束时间点

	int ready_dur_s;														// 活动等待时间
	int open_dur_s;															// 开启持续时间
};

struct CrossLieKunFbZoneCfg
{
	CrossLieKunFbZoneCfg() : zone(0), scene_id(0)
	{
		memset(boss_id_arr, 0, sizeof(boss_id_arr));
	}

	int zone;
	int scene_id;
	Posi enter_pos;
	int boss_id_arr[LIEKUN_BOSS_TYPE_COUNT];
	Posi boss_pos_arr[LIEKUN_BOSS_TYPE_COUNT];
};

struct CrossLieKunRewardPosCfg
{
	CrossLieKunRewardPosCfg() : zone(0), boss_id(0), gather_id(0), gather_item_offset(0) {}

	int zone;
	int boss_id;
	int gather_id;
	Posi gather_flush_pos;
	int gather_item_offset;
};

struct CrossLieKunRewardCfg
{
	CrossLieKunRewardCfg() : gather_id(0), gather_count(0), suipian_drop_rate(0), reward_drop_rate(0) {}
	
	int gather_id;;

	int gather_count;
	ItemConfigData gather_item_list[LIEKUN_MAX_GATHER_ITEM_COUNT];

	int suipian_drop_rate;
	ItemConfigData suipian_item;

	int reward_drop_rate;
	ItemConfigData reward_item;
};

class CrossLieKunFbConfig:public ConfigBase
{
public:
	CrossLieKunFbConfig();
	~CrossLieKunFbConfig();

	static CrossLieKunFbConfig & Instance();
	bool Init(const std::string &path, std::string *err);
	static bool ReadPosConfig(const PugiXmlNode &element, const char *name, Posi & pos);

	const CrossLieKunFbOtherCfg& GetOtherCfg() { return m_other_cfg; }
	CrossLieKunFbActivityOpenTimeCfg *GetActivityOpenTimeInfo(int index);
	int GetOpenActivityConfigList(CrossLieKunFbActivityOpenTimeCfg cfg_list[CROSS_ACTIVITY_OPEN_TIME_CFG_MAX_COUNT]) const;

	int GetZoneIndexBySceneId(int scene_id);
	void GetBossIdList(int scene_id, int boss_id_list[LIEKUN_BOSS_TYPE_COUNT]);
	const CrossLieKunFbZoneCfg *GetCrossLieKunFbZoneCfg(int zone) const;

	const CrossLieKunRewardPosCfg *GetCrossLieKunRewardPosCfgByBossId(int zone, int boss_id) const;
	const CrossLieKunRewardPosCfg *GetCrossLieKunRewardPosCfgByGatherId(int zone, int gather_id) const;
	bool GetCrossLieKunRewardItemList(int gather_id, ItemConfigData gather_item_list[LIEKUN_MAX_ITEM_COUNT], int &item_count, int &suipian_id);

private:
	int InitOtherCfg(PugiXmlNode RootElement);
	int InitActivityOpenTimeCfg(PugiXmlNode RootElement);
	int InitZoneCfg(PugiXmlNode RootElement);
	int InitRewardPosCfg(PugiXmlNode RootElement);
	int InitRewardCfg(PugiXmlNode RootElement);

	bool DayTimeFormatInvalid(int day_time);

	CrossLieKunFbOtherCfg m_other_cfg;

	int m_activity_open_time_cfg_list_count;								// 活动开启时间配置数量
	CrossLieKunFbActivityOpenTimeCfg m_activity_open_time_cfg_list[CROSS_ACTIVITY_OPEN_TIME_CFG_MAX_COUNT];	// 活动开启时间

	int m_zone_cfg_list_count;
	CrossLieKunFbZoneCfg m_zone_cfg_list[LIEKUN_ZONE_TYPE_COUNT];

	std::vector<CrossLieKunRewardPosCfg> m_reward_pos_cfg_list;

	typedef std::map<int, CrossLieKunRewardCfg> CrossLieKunRewardCfgMap;
	typedef std::map<int, CrossLieKunRewardCfg>::const_iterator CrossLieKunRewardCfgMapIt;
	CrossLieKunRewardCfgMap m_reward_cfg_map;
};

#endif // __CROSS_SHENGYINFB_CONFIG_HPP__

