#ifndef __RUNE_TOWER_CONFIG_HPP__
#define __RUNE_TOWER_CONFIG_HPP__

#include "servercommon/runesystemdef.hpp"
#include "common/tinyxml/tinyxml.h"
#include "servercommon/configcommon.h"
#include "servercommon/maildef.hpp"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

#include <vector>

static const int RUNE_TOWER_FB_LAYER_MAX = 500;
static const int RUNE_TOWER_RAND_REWARD_CFG_MAX = 100;
static const int RUNE_TOWER_RAND_WEGHT_INDEX_MAX = 18;
static const int RUNE_TOWER_RAND_REWARD_MAX_COUNT = 7;

struct RuneTowerOtherCfg
{
	RuneTowerOtherCfg() : offline_time_max(0), offline_time_free(0), offline_time_free_count(0), add_hp_per(0), add_gongji_per(0),
		pass_reward_min(0), pass_reward_max(0), add_role_gongji_per(0){}

	int offline_time_max;
	int offline_time_free;
	int offline_time_free_count;
	int add_hp_per;
	int add_gongji_per;
	int pass_reward_min;
	int pass_reward_max;
	int add_role_gongji_per;
};

struct RuneTowerLayerCfg
{
	RuneTowerLayerCfg() : fb_layer(0), level_limit(0), scene_id(0), enter_pos(0, 0), monster_id(0), monster_num(0),
		monster_pos(0, 0), monster_range(0), monster_rune_exp_rate(0), monster_rune_exp_min(0), monster_rune_exp_max(0),
		first_reward_rune_exp(0), normal_reward_rune_exp(0), capability(0), kill_monster_speed(0) {}

	int fb_layer;
	int level_limit;
	int scene_id;
	Posi enter_pos;

	int monster_id;
	int monster_num;
	Posi monster_pos;
	int monster_range;
	int monster_rune_exp_rate;
	int monster_rune_exp_min;
	int monster_rune_exp_max;

	int first_reward_rune_exp;
	ItemConfigData first_reward_item_list[MAX_ATTACHMENT_ITEM_NUM];
	int normal_reward_rune_exp;
	ItemConfigData normal_reward_item_list[MAX_ATTACHMENT_ITEM_NUM];

	int capability;
	float kill_monster_speed;
};

struct RuneTowerRandWeightCfg
{
	RuneTowerRandWeightCfg() : weight_index(0), begin_layer(0), end_layer(0) {}

	int weight_index;
	int begin_layer;
	int end_layer;
};

struct RuneTowerRandRewardCfg
{
	RuneTowerRandRewardCfg() : index(0), rune_id(0)
	{
		memset(weight, 0, sizeof(weight));
	}

	int index;
	int rune_id;
	int weight[RUNE_TOWER_RAND_WEGHT_INDEX_MAX];
};

class RuneTowerConfig : public ConfigBase
{
public:
	RuneTowerConfig();
	~RuneTowerConfig();

	bool Init(const std::string &configname, std::string *err);

	const RuneTowerOtherCfg & GetOtherCfg() const { return m_other_cfg; }
	const RuneTowerLayerCfg * GetRuneTowerLayerCfg(int layer) const;
	const int GetRandNum() const;
	const int GetRandWeightIndexByLayer(int layer)const;
	const int GetRandRewardByLayer(int layer)const;
	void GetRandRewardList(int layer, int reward_list[RUNE_TOWER_RAND_REWARD_MAX_COUNT], int &count);
private:
	int InitOtherCfg(PugiXmlNode RootElement);
	int InitLayerCfg(PugiXmlNode RootElement);
	int InitRandRewardCfg(PugiXmlNode RootElement);
	int InitRandWeightCfg(PugiXmlNode RootElement);

	RuneTowerOtherCfg m_other_cfg;
	RuneTowerLayerCfg m_layer_list[RUNE_TOWER_FB_LAYER_MAX + 1];

	RuneTowerRandWeightCfg m_rand_weight_cfg[RUNE_TOWER_RAND_WEGHT_INDEX_MAX];
	RuneTowerRandRewardCfg m_rand_reward_cfg[RUNE_TOWER_RAND_REWARD_CFG_MAX];
};

#endif