#include "servercommon/struct/itemlistparam.h"
#include "servercommon/activitydef.hpp"
#include "todaythemerewardconfig.hpp"




bool TodayThemeRewardConfig::Init(const std::string &configname, std::string *err)
{
	PUGI_XML_CONFIG_PRE_LOAD;
	PUGI_XML_LOAD_CONFIG("reward_seq", InitReward);
	return true;
}

int TodayThemeRewardConfig::InitReward(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	int last_seq = -1;
	int seq = 0;
	while (!dataElement.empty())
	{
		if (!PugiGetSubNodeValue(dataElement, "seq", seq) || seq < 0 || seq >= TODAY_THEME_REWARD_MAX_COUNT)
		{
			return -1;
		}
		if (last_seq + 1 != seq)
		{
			return -2;
		}
		last_seq = seq;

		ItemConfigData item_config_data;
		if (!ReadItemConfig(dataElement, "reward_item", item_config_data))
		{
			return -3;
		}

		reward_vec.push_back(item_config_data);

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

const ItemConfigData * TodayThemeRewardConfig::GetRewardConfig(int seq)
{
	if (seq < 0 || static_cast<size_t>(seq) >= reward_vec.size() || seq >= TODAY_THEME_REWARD_MAX_COUNT)
	{
		return nullptr;
	}
	return &reward_vec[seq];
}
