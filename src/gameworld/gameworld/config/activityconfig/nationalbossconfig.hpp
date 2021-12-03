#ifndef _NATIONALBOSSCONFIG_HPP_
#define _NATIONALBOSSCONFIG_HPP_

#include "servercommon/activitydef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include "gamedef.h"

#include <vector>

struct NBRefreshItem
{
	NBRefreshItem() : world_level(0)
	{
		memset(boss_id_list, 0, sizeof(boss_id_list));
	}

	int world_level;
	
	int boss_id_list[NATIONALBOSS_FRESH_BOSS_COUNT];
	Posi born_pos_list[NATIONALBOSS_FRESH_BOSS_COUNT];
};

struct NBRankReward
{
	NBRankReward() : min_rank(0), max_rank(0) {}

	int min_rank;
	int max_rank;
	ItemConfigData item_cfg;
};

struct NBTaskRewardConfig
{
	const static int CONFIG_ITEM_MAX_COUNT = 128;

	struct ConfigItem
	{
		ConfigItem() : world_level(0), hurt_percent(0), reward_exp(0), reward_coin(0), reward_level(0) {}

		int world_level;
		int hurt_percent;
		int reward_exp;
		int reward_coin;
		int reward_level;
	};

	NBTaskRewardConfig() : cfg_count(0) {}

	int cfg_count;
	ConfigItem cfg_list[CONFIG_ITEM_MAX_COUNT];
};

class NationalBossConfig : public ConfigBase
{
public:
	NationalBossConfig();
	~NationalBossConfig();

	bool Init(const std::string &configname, std::string *err);

	int GetSceneID() { return m_scene_id; }
	int GetRefreshIntervalS() { return m_refresh_interval_s; }
	int GetBuffGold() { return m_buff_gold; }
	int GetBuyGongjiPercent() { return m_buy_gongji_percent; }
	int GetMaxBuyTimes() { return m_max_buy_times; }

	const NBRefreshItem * GetRefreshList(int world_level);
	ItemConfigData * GetRankReward(int rank);

	const NBTaskRewardConfig::ConfigItem * GetTaskReward(int world_level, int hurt_percent);

private:
	int InitPosCfg(PugiXmlNode RootElement);
	int InitRankRewardCfg(PugiXmlNode RootElement);
	int InitOtherCfg(PugiXmlNode RootElement);
	int InitTaskRewardCfg(PugiXmlNode RootElement);

	int m_scene_id;
	int m_refresh_interval_s;
	int m_buff_gold;
	int m_buy_gongji_percent;
	int m_max_buy_times;

	int m_refresh_count;
	NBRefreshItem m_refresh_list[NATIONALBOSS_FRESH_CONFIG_COUNT];

	static const int MAX_REWARD_NUM = 20;
	int m_reward_count;
	NBRankReward m_reward_list[MAX_REWARD_NUM];

	NBTaskRewardConfig m_task_reward_cfg;
};

#endif

