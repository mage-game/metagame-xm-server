#ifndef _TODAY_THEME_REWARD_CONFIG_HPP_
#define _TODAY_THEME_REWARD_CONFIG_HPP_

#include "servercommon/pugixml/pugixml_adapter.hpp"
#include "servercommon/configbase.h"


class TodayThemeRewardConfig : public ConfigBase
{
public:
	TodayThemeRewardConfig() {}

	~TodayThemeRewardConfig() {}

	bool Init(const std::string &configname, std::string *err);

	const ItemConfigData* GetRewardConfig(int seq);

private:
	
	int InitReward(PugiXmlNode RootElement);
	std::vector<ItemConfigData> reward_vec;
};

#endif
