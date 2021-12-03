#ifndef __TOUZIJIHUA_CONFIG_HPP__
#define __TOUZIJIHUA_CONFIG_HPP__

#include "servercommon/servercommon.h"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include <vector>

enum TOUZI_JIHUA_PLAN_TYPE
{
	TOUZI_JIHUA_PLAN_TYPE_0 = 0,
	TOUZI_JIHUA_PLAN_TYPE_1,
	TOUZI_JIHUA_PLAN_TYPE_2,
	TOUZI_JIHUA_PLAN_TYPE_3,

	TOUZI_JIHUA_PLAN_MAX_TYPE,
};

UNSTD_STATIC_CHECK(4 == TOUZI_JIHUA_PLAN_MAX_TYPE);

struct TouzijihuaRewardConfig
{
	const static int CONFIG_ITEM_MAX_COUNT = 16;

	struct ConfigItem
	{
		ConfigItem() : plan_type(0), seq(0), need_level(0), reward_gold(0), reward_gold_bind(0) {}

		int plan_type;
		int seq;
		int need_level;
		int reward_gold;
		int reward_gold_bind;
	};

	TouzijihuaRewardConfig() : plan_type(0), cfg_count(0)
	{
	}

	int plan_type;

	int cfg_count;
	ConfigItem cfg_list[CONFIG_ITEM_MAX_COUNT];
};

const static int MAX_NEW_TOUZIJIHUA_DAY = 7;
const static int MAX_NEW_TOUZIJIHUA_REWARD_COUNT = 6;
struct NewTouzijihuaRewardConfig
{
	NewTouzijihuaRewardConfig() : day_index(0), reward_item_count(0), vip_reward_item_count(0) {}

	int day_index;
	int reward_item_count;
	ItemConfigData reward_item_list[MAX_NEW_TOUZIJIHUA_REWARD_COUNT];
	int vip_reward_item_count;
	ItemConfigData vip_reward_item_list[MAX_NEW_TOUZIJIHUA_REWARD_COUNT];
	int reward_gold_bind;
};

struct FoundationConfig
{
	FoundationConfig() : seq(0), chongzhi_gold(0), active_level_min(0), active_level_max(0)
	{}

	struct FoundationRewardCfg
	{
		FoundationRewardCfg() : reward_level(0), reward_gold(0)
		{}

		int reward_level;
		int reward_gold;
	};

	int seq;
	int chongzhi_gold;
	int active_level_min;
	int active_level_max;

	std::vector<FoundationRewardCfg> reward_cfg_vec;
};

struct FbPlanConfig
{
	struct FbPlanItem
	{
		FbPlanItem():index(0),pass_level(0),reward_gold_bind(0){}

		int index;
		int pass_level;
		int reward_gold_bind;
	};

	std::vector<FbPlanItem> fb_plan_vec;
};

struct BossPlanConfig
{
	struct BossPlanItem
	{
		BossPlanItem():index(0),kill_num(0),reward_gold_bind(0){}
		int index;
		int kill_num;
		int reward_gold_bind;
	};
	std::vector<BossPlanItem> boss_plan_vec;
};

// 神域boss投资
struct ShenyuBossPlanConfig
{
	struct BossPlanItem
	{
		BossPlanItem() :index(0), kill_num(0), reward_gold_bind(0) {}
		int index;
		int kill_num;
		int reward_gold_bind;
	};
	std::vector<BossPlanItem> boss_plan_vec;
};

class TouzijihuaConfig : public ConfigBase
{
public:

	static constexpr int MAX_FOUNDATION_TYPE = 12;

	TouzijihuaConfig();
	~TouzijihuaConfig();

	bool Init(const std::string &path, std::string *err);

	const TouzijihuaRewardConfig::ConfigItem * GetRewardCfg(int type, int seq);
	int GetPlanPrice(int plan_type);

	int GetNewPlanPrice();
	int GetNewPlanFirstReward();
	const NewTouzijihuaRewardConfig * GetNewRewardCfg(int day_index);

	const FoundationConfig * GetFoundationCfg(int chongzhi_num);
	int GetFoundationRewardGold(int reward_index, int reward_times, int role_level);
	int GetFoundationRewardLimitMaxLevel(int reward_index);
	int GetFoundationCfgCount() { return static_cast<int>(m_foundation_cfg_vec.size()); }

	int GetFbPlanPrice() const;
	int GetBossPlanPrice() const;
	const FbPlanConfig::FbPlanItem * GetFbPlanCfgItem(int index) const;
	const BossPlanConfig::BossPlanItem * GetBossPlanCfgItem(int index) const;
	int GetFbPlanIndex(int pass_level);
	int GetBossPlanIndex(int kill_num);

	//神域boss投资
	int GetShenyuBossPlanPrice() const;
	const ShenyuBossPlanConfig::BossPlanItem * GetShenyuBossPlanCfgItem(int index) const;
	int GetShenyuBossPlanIndex(int kill_num);

private:
	int InitRewardCfg(PugiXmlNode RootElement);
	int InitOtherCfg(PugiXmlNode RootElement);
	int InitNewRewardCfg(PugiXmlNode RootElement);
	int InitFoundationCfg(PugiXmlNode RootElement);
	int InitFbPlanCfg(PugiXmlNode RootElement);
	int InitBossPlanCfg(PugiXmlNode RootElement);
	int InitShenyuBossPlanCfg(PugiXmlNode RootElement);

	TouzijihuaRewardConfig m_reward_cfglist[TOUZI_JIHUA_PLAN_MAX_TYPE];
	int m_plan_price[TOUZI_JIHUA_PLAN_MAX_TYPE];

	int m_new_plan_price = 0;
	int m_new_plan_first_reward = 0;
	NewTouzijihuaRewardConfig m_new_reward_cfg_list[MAX_NEW_TOUZIJIHUA_DAY];

	std::vector<FoundationConfig> m_foundation_cfg_vec;

	int m_fb_plan_price = 0;
	int m_boss_plan_price = 0;
	int m_shenyu_boss_plan_price = 0;
	FbPlanConfig m_fb_plan_cfg;
	BossPlanConfig m_boss_plan_cfg;
	ShenyuBossPlanConfig m_shenyu_boss_plan_cfg;
};

#endif // __TOUZIJIHUA_CONFIG_HPP__

