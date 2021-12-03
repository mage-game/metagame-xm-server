#include "sevendayloginconfig.hpp"
#include "servercommon/configcommon.h"
#include "item/itempool.h"

SevenDayLoginConfig::SevenDayLoginConfig() : m_max_reward_day(0)
{
	memset(m_reward_item_list, 0, sizeof(m_reward_item_list));
}

SevenDayLoginConfig::~SevenDayLoginConfig()
{

}

bool SevenDayLoginConfig::Init(const std::string &path, std::string *err)
{
	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "SevenDayLoginConfig", *err);

	PugiXmlNode root_element = document.document_element();
	if (root_element.empty())
	{
		*err = path + ": xml root node error.";
		return false;
	}

	{
		PugiXmlNode reward_element = root_element.child("reward");
		if (reward_element.empty())
		{
			*err = path + ": xml node error in [config->reward].";
			return false;
		}

		PugiXmlNode data_element = reward_element.child("data");
		if (data_element.empty())
		{
			*err = path + ": xml node error in [config->reward->data] empty.";
			return false;
		}

		m_max_reward_day = 0;

		while (!data_element.empty())
		{
			int day = 0;
			if (!PugiGetSubNodeValue(data_element, "login_daycount", day) || day <= 0 || day > SEVEN_DAY_LOGIN_MAX_REWARD_DAY || day != m_max_reward_day + 1)
			{
				*err = path + ": xml node error in [config->reward->data->login_daycount].";
				return false;
			}

			m_max_reward_day = day;

			PugiXmlNode item_element = data_element.child("reward_item");
			if (item_element.empty())
			{
				*err = path + ": xml node error in [config->reward->data->reward_item]";
				return false;
			}
			if (!m_reward_item_list[m_max_reward_day].ReadConfig(item_element))
			{
				*err = path + ": xml node error in [config->reward->data->reward_item]";
				return false;
			}

			data_element = data_element.next_sibling();
		}
	}

	return true;
}

const ItemConfigData * SevenDayLoginConfig::getRewardItem(int day)
{
	if (day <= 0 || day > m_max_reward_day || day > SEVEN_DAY_LOGIN_MAX_REWARD_DAY) return NULL;

	return &m_reward_item_list[day];
}


