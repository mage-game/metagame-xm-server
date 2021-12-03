#ifndef __GOLDTURNTABLE_CONFIG_HPP__
#define __GOLDTURNTABLE_CONFIG_HPP__

#include "servercommon/configcommon.h"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

class Role;

struct GoldTurntableOtherCfg
{
	GoldTurntableOtherCfg() : per_transform(0), sigle_cj_gold(0), min_pool(0) {}

	int per_transform;
	int sigle_cj_gold;
	long long min_pool;
};

struct GoldTurntableRewardPoolCfg
{
	GoldTurntableRewardPoolCfg() : index(0), rewrad_gold(0), jianchi_per(0), weight(0), broadcast(0) {}

	int index;
	int rewrad_gold;	
	int jianchi_per;
	int weight;
	int broadcast;

	ItemConfigData item;
};

class GoldTurntableConfig : public ConfigBase
{
public:
	GoldTurntableConfig();
	~GoldTurntableConfig();

	bool Init(const std::string &path, std::string *err);

	GoldTurntableRewardPoolCfg *RandRewardCfg();
	GoldTurntableOtherCfg *GetOtherCfg() { return &m_other_cfg; }

private:
	int InitOtherConfig(PugiXmlNode element);
	int InitRewardPoolConfig(PugiXmlNode element);

	GoldTurntableOtherCfg m_other_cfg;

	static const int GOLD_TURNTABLE_POOL_COUNT = 20;

	int m_total_weight;
	int m_reward_pool_count;
	GoldTurntableRewardPoolCfg m_reward_pool_cfg_list[GOLD_TURNTABLE_POOL_COUNT];
};

#endif