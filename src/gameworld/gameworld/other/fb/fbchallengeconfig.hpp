#ifndef __FB_CHALLENGE_CONFIG_HPP__
#define __FB_CHALLENGE_CONFIG_HPP__

#include "servercommon/struct/itemlistparam.h"
#include "servercommon/maildef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

static const int CHALLENGE_FB_BUY_COST_MAX_COUNT = 20;

struct ChallengeFBRewardConfig
{
	ChallengeFBRewardConfig()
	{}

	unsigned int min_time;
	unsigned int max_time;
	ItemConfigData reward_item_list[MAX_ATTACHMENT_ITEM_NUM];
};

struct ChallengeFBLevelCfg
{
	const static int LAYER_PER_LEVEL = 10;
	const static int MONSTER_MAX_COUNT = 10;
	const static int CONFIG_ITEM_MAX_COUNT = 30;
	const static int REWARD_DEGREE = 3;

	struct Layer
	{
		Layer() : level(0), layer(0), scene_id(0), boss_id(0), boss_pos(0, 0), zhanli(0)
		{}

		int level;
		int layer;
		int scene_id;

		int boss_id;
		Posi boss_pos;
		int zhanli;
	};

	struct ConfigItem
	{
		ConfigItem() : level(0), role_level_limit(0), birth_pos(0, 0), start_scene_id(0), end_scene_id(0) {}

		int level;
		int role_level_limit;
		Posi birth_pos;
		int start_scene_id;
		int end_scene_id;

		Layer layer_list[LAYER_PER_LEVEL];
		ItemConfigData find_reward[MAX_ATTACHMENT_ITEM_NUM];
	};

	ChallengeFBLevelCfg() : cfg_count(0) {}

	int cfg_count;
	ConfigItem cfg_list[CONFIG_ITEM_MAX_COUNT];
	ChallengeFBRewardConfig layer_reward_list[CONFIG_ITEM_MAX_COUNT][REWARD_DEGREE];
};

struct ChallengeFBOtherCfg
{
	ChallengeFBOtherCfg(): day_free_times(0), auto_need_star(0), buy_max_times(0), max_layer(0), add_role_gongji_per(0)
	{}

	int day_free_times;
	int auto_need_star;
	int buy_max_times;
	int max_layer;
	int add_role_gongji_per;
};

struct ChallengeFBBuyCostCfg
{
	ChallengeFBBuyCostCfg():buy_times(0), gold_cost(0)
	{}

	int buy_times;
	int gold_cost;
};

class ChallengeFBConfig : public ConfigBase
{
public:
	ChallengeFBConfig();
	~ChallengeFBConfig();

	bool Init(const std::string &path, std::string * err);

	const ChallengeFBLevelCfg::ConfigItem * GetLevelCfg(int level);
	const ChallengeFBLevelCfg::ConfigItem * GetLevelCfgBySceneID(int scene_id, int *belong_layer);
	const ChallengeFBLevelCfg::ConfigItem * GetLevelCfgByStartSceneID(int scene_id);
	const ChallengeFBLevelCfg::ConfigItem * GetLevelCfgByEndSceneID(int scene_id);

	const ChallengeFBLevelCfg::Layer * GetLayerCfgBySceneID(int scene_id);
	const ChallengeFBLevelCfg::Layer * GetLayerCfg(int level, int layer);

	int GetDayFreeTimes() { return m_other_cfg.day_free_times; }
	int GetAutoNeedStar() { return m_other_cfg.auto_need_star; }
	int GetDayMaxBuyTimes() { return m_other_cfg.buy_max_times; }
	int GetMaxLayer() { return m_other_cfg.max_layer; }
	int GetCapAddGongjiPer() { return m_other_cfg.add_role_gongji_per; }

	ItemConfigData * GetRewardItemList(int level, unsigned int pass_time, int &reward_flag);
	ItemConfigData * GetRewardItemList(int level, int reward_flag);

	const ChallengeFBBuyCostCfg* GetBuyCostCfg(int buy_times);

private:
	int InitLayerCfg(PugiXmlNode RootElement);
	int InitLevelRewardCfg(PugiXmlNode RootElement);
	int InitOtherCfg(PugiXmlNode RootElement);
	int InitBuyCostCfg(PugiXmlNode RootElement);

	ChallengeFBOtherCfg m_other_cfg;
	ChallengeFBLevelCfg m_level_cfg;

	int m_buy_cost_count;
	ChallengeFBBuyCostCfg m_buy_cost_cfg[CHALLENGE_FB_BUY_COST_MAX_COUNT];
};

#endif // __FB_CHALLENGE_CONFIG_HPP__

