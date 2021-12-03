#include "platvipconfig.hpp"
#include "servercommon/configcommon.h"
#include "item/itempool.h"

PlatVipConfig::PlatVipConfig()
{
}

PlatVipConfig::~PlatVipConfig()
{
}

bool PlatVipConfig::Init(const std::string &path, std::string *err)
{
	TiXmlDocument document;
	if (path == "" || !document.LoadFile(path))
	{
		*err = path + ": Load PlatVipConfig Error,\n " + document.ErrorDesc();
		return false;
	}

	TiXmlElement *root_element = document.RootElement();
	if (NULL == root_element)
	{
		*err = path + ": xml root node error.";
		return false;
	}

	{
		TiXmlElement *reward_element = root_element->FirstChildElement("reward");
		if (NULL == reward_element)
		{
			*err = path + ": xml node error in [config->reward].";
			return false;
		}

		TiXmlElement *data_element = reward_element->FirstChildElement("data");
		if (NULL == data_element)
		{
			*err = path + ": xml node error in [config->reward->data] empty.";
			return false;
		}

		while (NULL != data_element)
		{
			int plat_id = 0, reward_type = 0;

			if (!GetSubNodeValue(data_element, "plat_id", plat_id) || plat_id < 0 || plat_id >= PLAT_WELFARE_MAX)
			{
				*err = path + ": xml node error in [config->reward->data->plat_id].";
				return false;
			}

			if (!GetSubNodeValue(data_element, "reward_type", reward_type) || reward_type < 1 || reward_type >= PLAT_VIP_REWARD_MAX)
			{
				*err = path + ": xml node error in [config->reward->data->reward_type].";
				return false;
			}

			TiXmlElement *item_element = data_element->FirstChildElement("level_reward_item");
			if (NULL == item_element)
			{
				*err = path + ": xml node error in [config->reward->data->level_reward_item]";
				return false;
			}

			if (m_reward_item_list[plat_id][reward_type][0].item_id > 0) 
			{
				*err = path + ": xml node error in [config->reward->data->level_reward_item REPEAT]";
			}

			if (!m_reward_item_list[plat_id][reward_type][0].ReadConfig(item_element))
			{
				*err = path + ": xml node error in [config->reward->data->level_reward_item READ_CONFIG]";
				return false;
			}

			item_element = data_element->FirstChildElement("login_reward_item");
			if (NULL == item_element)
			{
				*err = path + ": xml node error in [config->reward->data->login_reward_item]";
				return false;
			}

			if (m_reward_item_list[plat_id][reward_type][1].item_id > 0) 
			{
				*err = path + ": xml node error in [config->reward->data->login_reward_item REPEAT]";
			}

			if (!m_reward_item_list[plat_id][reward_type][1].ReadConfig(item_element))
			{
				*err = path + ": xml node error in [config->reward->data->login_reward_item] READ_CONFIG";
				return false;
			}

			data_element = data_element->NextSiblingElement();
		}
	}

	return true;
}

const ItemConfigData * PlatVipConfig::GetReward(int fetch_type, int plat_id, int vip_level, int vip_type, int *reward_type)
{
	if (fetch_type < 0 || fetch_type >= PLAT_VIP_REWARD_MAX || plat_id < 0 || plat_id >= PLAT_WELFARE_MAX || 
		vip_level < 0 || vip_level > PLAT_VIP_REWARD_LEVEL || vip_type < 0 || vip_type >= PLAT_VIP_REWARD_360_BOOST_BALL || NULL == reward_type) 
	{
		return NULL;
	}

	ItemConfigData *item_data = NULL;
	*reward_type = -1;

	if (PLAT_VIP_REWARD_LOGIN == fetch_type) 
	{
		item_data = &m_reward_item_list[plat_id][vip_level][1];
		*reward_type = PLAT_VIP_REWARD_LOGIN;
	}
	else if (fetch_type > PLAT_VIP_REWARD_LOGIN && fetch_type <= PLAT_VIP_REWARD_LEVEL)
	{
		item_data = &m_reward_item_list[plat_id][vip_level][0];
		*reward_type = vip_level;
	}
	else if (fetch_type > PLAT_VIP_REWARD_LEVEL && fetch_type < PLAT_VIP_REWARD_360_BOOST_BALL)
	{
		item_data = &m_reward_item_list[plat_id][vip_type][0];
		*reward_type = vip_type;
	} 
	else if (PLAT_VIP_REWARD_360_BOOST_BALL == fetch_type) 
	{
		if (PLAT_WELFARE_360 == plat_id) 
		{
			item_data = &m_reward_item_list[plat_id][fetch_type][0];
			*reward_type = fetch_type;
		}
	}

	return item_data;
}
