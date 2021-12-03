
#include "itemlevelreward.hpp"
#include "servercommon/configcommon.h"

ItemLevelRewardConfig::ItemLevelRewardConfig()
{
}

ItemLevelRewardConfig::~ItemLevelRewardConfig()
{
}

bool ItemLevelRewardConfig::Init(const std::string &configname, std::string *err)
{
	PRE_LOAD_CONFIG;

	LOAD_CONFIG("level_reward_list", InitLevelRewardCfg);

	return true;
}

const long long ItemLevelRewardConfig::GetExpDanReward(int role_level, int type)
{
	if (role_level <= 0 || role_level > MAX_ROLE_LEVEL || type < 0 || type >= ItemLevelRewardConfigItem::EXP_DAN_MAX_TYPE_COUNT)
	{
		return 0;
	}

	return m_level_reward_cfg_list[role_level].exp_dan_exp[type];
}

int ItemLevelRewardConfig::InitLevelRewardCfg(TiXmlElement * RootElement)
{
	TiXmlElement *data_element = RootElement->FirstChildElement("data");
	if (NULL == data_element)
	{
		return -1000;
	}

	int pre_level = 0;
	while (NULL != data_element)
	{
		int level = 0;
		if (!GetSubNodeValue(data_element, "level", level) || level != pre_level + 1 || level > MAX_ROLE_LEVEL)
		{
			return -1;
		}
		pre_level = level;

		ItemLevelRewardConfigItem &cfg = m_level_reward_cfg_list[level];

		if (!GetSubNodeValue(data_element, "exp_0", cfg.exp_dan_exp[0]) || cfg.exp_dan_exp[0] <= 0)
		{
			return -2;
		}

		//if (!GetSubNodeValue(data_element, "exp_dan_exp_2", cfg.exp_dan_exp[1]) || cfg.exp_dan_exp[1] < 0)
		//{
		//	return -3;
		//}

		//if (!GetSubNodeValue(data_element, "exp_dan_exp_3", cfg.exp_dan_exp[2]) || cfg.exp_dan_exp[2] < 0)
		//{
		//	return -4;
		//}

		//if (!GetSubNodeValue(data_element, "exp_dan_exp_4", cfg.exp_dan_exp[3]) || cfg.exp_dan_exp[3] < 0)
		//{
		//	return -5;
		//}

		//if (!GetSubNodeValue(data_element, "exp_dan_exp_5", cfg.exp_dan_exp[4]) || cfg.exp_dan_exp[4] < 0)
		//{
		//	return -6;
		//}

		//if (!GetSubNodeValue(data_element, "exp_dan_exp_6", cfg.exp_dan_exp[5]) || cfg.exp_dan_exp[5] < 0)
		//{
		//	return -7;
		//}

		data_element = data_element->NextSiblingElement();
	}

	return 0;
}


