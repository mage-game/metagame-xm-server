#include "jinjiesysrewardconfig.hpp"
#include "servercommon/configcommon.h"

bool JinjiesysRewardConfig::Init(std::string configname, std::string *err)
{
	PRE_LOAD_CONFIG;

	LOAD_CONFIG("item_cfg", InitItemCfg);
	LOAD_CONFIG("attr_cfg", InitAttrCfg);

	return true;
}

std::vector<JinjieRewardItemCfg *> JinjiesysRewardConfig::GetActiveItemCfgVec(int grade, int system_type)
{
	std::vector<JinjieRewardItemCfg *> vec;
	for (int i = 0; i < REWARD_TYPE_MAX; ++i)
	{
		if (CanUseCfg(i, system_type))
		{
			if (grade >= m_reward_item_cfg_list[i][system_type].grade)
			{
				vec.push_back(&m_reward_item_cfg_list[i][system_type]);
			}
		}
	}

	return vec;
}

const JinjieRewardItemCfg * JinjiesysRewardConfig::GetItemCfg(int reward_type, int system_type)
{
	if (!CanUseCfg(reward_type, system_type))
	{
		return nullptr;
	}

	return &m_reward_item_cfg_list[reward_type][system_type];
}

const JinjieRewardAttrCfg * JinjiesysRewardConfig::GetAttrCfg(int system_type)
{
	if (!CanUseCfg(REWARD_TYPE_SPECIAL_IMG, system_type))
	{
		return nullptr;
	}

	return &m_attr_cfg_list[system_type];
}

int JinjiesysRewardConfig::InitItemCfg(TiXmlElement *RootElement)
{
	TiXmlElement *dataElement = RootElement->FirstChildElement("data");
	if (nullptr == dataElement)
	{
		return -10000;
	}

	while (nullptr != dataElement)
	{
		int reward_type = -1;
		if (!GetSubNodeValue(dataElement, "reward_type", reward_type) || reward_type < 0 || reward_type >= REWARD_TYPE_MAX)
		{
			return -100;
		}

		int sys_type = -1;
		if (!GetSubNodeValue(dataElement, "system_type", sys_type) || sys_type < 0 || sys_type >= JINJIE_TYPE_MAX)
		{
			return -1;
		}

		JinjieRewardItemCfg &cfg = m_reward_item_cfg_list[reward_type][sys_type];
		cfg.reward_type = reward_type;
		cfg.system_type = sys_type;

		if (GetSubNodeValue(dataElement, "openserver_day", cfg.openserver_day))
		{
			if (cfg.openserver_day < 0)
			{
				return -3;
			}
		}

		if (!GetSubNodeValue(dataElement, "duration_time", cfg.duration_time_s) || cfg.duration_time_s <= 0)
		{
			return -4;
		}
		cfg.duration_time_s *= SECOND_PER_HOUR;

		if (!GetSubNodeValue(dataElement, "grade", cfg.grade) || cfg.grade <= 0)
		{
			return -5;
		}

		if (!GetSubNodeValue(dataElement, "cost", cfg.cost) || cfg.cost < 0)	// cost = 0 意味着不能交换
		{
			return -6;
		}

		TiXmlElement *item_element = dataElement->FirstChildElement("reward_item");
		if (nullptr == item_element)
		{
			return -7;
		}

		if (!cfg.reward_item.ReadConfig(item_element))
		{
			return -8;
		}

		int param_0 = -1;
		if (GetSubNodeValue(dataElement, "param_0", param_0))
		{
			if (param_0 >= 0)
			{
				cfg.param_0 = param_0;
			}
			else
			{
				return -9;
			}
		}

		if (!GetSubNodeValue(dataElement, "open_level", cfg.open_level) || cfg.open_level < 0)
		{
			return -10;
		}

		dataElement = dataElement->NextSiblingElement();
	}

	return 0;
}

int JinjiesysRewardConfig::InitAttrCfg(TiXmlElement *RootElement)
{
	TiXmlElement *dataElement = RootElement->FirstChildElement("data");
	if (nullptr == dataElement)
	{
		return -10000;
	}

	while (nullptr != dataElement)
	{
		int sys_type = -1;
		if (!GetSubNodeValue(dataElement, "system_type", sys_type) || sys_type < 0 || sys_type >= JINJIE_TYPE_MAX)
		{
			return -1;
		}

		JinjieRewardAttrCfg &cfg = m_attr_cfg_list[sys_type];
		if (!GetSubNodeValue(dataElement, "add_per", cfg.add_per) || cfg.add_per < 0)
		{
			return -2;
		}

		dataElement = dataElement->NextSiblingElement();
	}

	return 0;
}
