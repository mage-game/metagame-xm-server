#include "servercommon/configcommon.h"
#include "upgradecardbuyconfig.hpp"
#include "item/itempool.h"

UpgradeCardBuyConfig::UpgradeCardBuyConfig()
{

}

UpgradeCardBuyConfig::~UpgradeCardBuyConfig()
{

}

bool UpgradeCardBuyConfig::Init(const std::string & configname, std::string * err)
{
	PRE_LOAD_CONFIG;

	LOAD_CONFIG("buy_cfg", InitUpgradeCardBuyConfig);

	LOAD_CONFIG("other", InitOther);

	return true;
}

UpgradeCardInfo * UpgradeCardBuyConfig::GetUpgradeCardInfo(int activity_id, int opengame_day, short grade)
{
	auto it = m_upgrade_card_buy_info_map.find(activity_id);
	if (m_upgrade_card_buy_info_map.end() != it)
	{
		for (auto &info_vec_it : it->second)
		{
			if (opengame_day <= info_vec_it.opengame_day && grade <= info_vec_it.grade)
			{
				return &info_vec_it;
			}
		}
	}

	return nullptr;
}

UpgradeCardInfo * UpgradeCardBuyConfig::GetUpgradeCardInfo(int activity_id, int opengame_day, short grade, int theme)
{
	auto it = m_upgrade_card_buy_info_map.find(activity_id);
	if (m_upgrade_card_buy_info_map.end() != it)
	{
		for (auto &info_vec_it : it->second)
		{
			if (opengame_day <= info_vec_it.opengame_day && grade <= info_vec_it.grade && info_vec_it.act_theme == theme)
			{
				return &info_vec_it;
			}
		}
	}

	return nullptr;
}

int UpgradeCardBuyConfig::InitUpgradeCardBuyConfig(TiXmlElement * root_element)
{
	TiXmlElement *data_element = root_element->FirstChildElement("data");
	if (nullptr == data_element)
	{
		return -100000;
	}

	int activity_id = 0;

	int check_key = 0;
	std::map<int, int> check_key_sort_map;

	while (data_element)
	{
		if (!GetSubNodeValue(data_element, "related_activity_s", activity_id) || activity_id <= 0)
		{
			return -1;
		}
		std::vector<UpgradeCardInfo> &cfg_vec = m_upgrade_card_buy_info_map[activity_id];

		UpgradeCardInfo cfg;

		if (!GetSubNodeValue(data_element, "open_game_day", cfg.opengame_day) || cfg.opengame_day <= 0)
		{
			return -2;
		}

		if (!GetSubNodeValue(data_element, "grade", cfg.grade) || cfg.grade <= 0)
		{
			return -3;
		}

		if (!GetSubNodeValue(data_element, "price", cfg.need_gold) || cfg.need_gold <= 0)
		{
			return -4;
		}

		if (!GetSubNodeValue(data_element, "item_id", cfg.item_id) || nullptr == ITEMPOOL->GetItem(cfg.item_id))
		{
			return -5;
		}

		if (!GetSubNodeValue(data_element, "act_theme", cfg.act_theme) || cfg.act_theme <= 0)
		{
			return -6;
		}

		auto check_it = check_key_sort_map.find(activity_id);
		check_key = cfg.opengame_day * 1000 + cfg.act_theme * 10 + cfg.grade;
		if (check_key_sort_map.end() != check_it)
		{
			// ±£Ö¤ÓÐÐò
			if (check_key <= check_it->second)
			{
				return -100;
			}
		}
		check_key_sort_map[activity_id] = check_key;

		cfg_vec.push_back(cfg);

		data_element = data_element->NextSiblingElement();
	}

	check_key_sort_map.clear();

	return 0;
}


int UpgradeCardBuyConfig::InitOther(TiXmlElement * root_element)
{
	TiXmlElement *data_element = root_element->FirstChildElement("data");
	if (nullptr == data_element)
	{
		return -100000;
	}

	if (!GetSubNodeValue(data_element, "limit_opengame_day", m_other_cfg.opengame_day_limit) || m_other_cfg.opengame_day_limit <= 0)
	{
		return -1;
	}

	if (!GetSubNodeValue(data_element, "limit_opengame_day_2", m_other_cfg.opengame_day_limit_2) || m_other_cfg.opengame_day_limit_2 <= 0)
	{
		return -2;
	}

	return 0;
}
