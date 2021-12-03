#ifndef __ITEM_LEVEL_REWARD_H__
#define __ITEM_LEVEL_REWARD_H__

#include "servercommon/configcommon.h"
#include "servercommon/configbase.h"

struct ItemLevelRewardConfigItem
{
	static const int EXP_DAN_MAX_TYPE_COUNT = 1;

	ItemLevelRewardConfigItem() : level(0)
	{
		memset(exp_dan_exp, 0, sizeof(exp_dan_exp));
	}

	int level;
	long long exp_dan_exp[EXP_DAN_MAX_TYPE_COUNT];
};

class ItemLevelRewardConfig : public ConfigBase
{
public:
	ItemLevelRewardConfig();
	~ItemLevelRewardConfig();

	bool Init(const std::string &configname, std::string *err);

	const long long GetExpDanReward(int role_level, int type);

private:
	int InitLevelRewardCfg(TiXmlElement *RootElement);

	ItemLevelRewardConfigItem m_level_reward_cfg_list[MAX_ROLE_LEVEL + 1];

};

#endif

