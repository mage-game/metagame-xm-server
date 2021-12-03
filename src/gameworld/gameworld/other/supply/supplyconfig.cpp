#include "supplyconfig.hpp"

#include "servercommon/configcommon.h"

SupplyConfig::SupplyConfig() : m_not_fight_rate(0), m_recover_per(0)
{

}

SupplyConfig::~SupplyConfig()
{

}

bool SupplyConfig::Init(const std::string &path, std::string *err)
{
	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "SupplyConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (!RootElement)
	{
		*err = path + ": xml root node error.";
		return false;
	}

	{
		PugiXmlNode supply_item_list_element = RootElement.child("supply_item_list");
		if (supply_item_list_element.empty())
		{
			*err = path + ": xml error in node [supply_item_list].";
			return false;
		}

		PugiXmlNode DataElement = supply_item_list_element.child("data");
		if (DataElement.empty())
		{
			*err = path + ": xml error in node [supply_item_list->data].";
			return false;
		}

		while (!DataElement.empty())
		{
			int supply_type = -1;
			if (!PugiGetSubNodeValue(DataElement, "supply_type", supply_type) || supply_type < 0 || supply_type >= SUPPLY_TYPE_MAX)
			{
				*err = path + ": xml error in node [supply_item_list->data->supply_type].";
				return false;
			}

			if (m_supply_cfg_list[supply_type].item_count >= MAX_SUPPLY_ITEM_COUNT)
			{
				*err = path + ": xml error in node [supply_item_list->data->supply_type] more than limit.";
				return false;
			}

			SupplyItem &supply_item = m_supply_cfg_list[supply_type].supply_item_list[m_supply_cfg_list[supply_type].item_count];

			if (!PugiGetSubNodeValue(DataElement, "price", supply_item.price) || supply_item.price <= 0)
			{
				*err = path + ": xml error in node [supply_item_list->data->price].";
				return false;
			}

			if (!PugiGetSubNodeValue(DataElement, "supply_value", supply_item.value) || supply_item.value <= 0)
			{
				*err = path + ": xml error in node [supply_item_list->data->supply_value].";
				return false;
			}

			int is_use_gold = 0;
			if (!PugiGetSubNodeValue(DataElement, "is_use_gold", is_use_gold) || is_use_gold < 0)
			{
				*err = path + ": xml error in node [supply_item_list->data->is_use_gold].";
				return false;
			}

			supply_item.is_use_gold = (0 != is_use_gold);

			++ m_supply_cfg_list[supply_type].item_count;
			DataElement = DataElement.next_sibling();
		}
	}
	
	{
		PugiXmlNode supply_recover_value_list_element = RootElement.child("supply_recover_value_list");
		if (supply_recover_value_list_element.empty())
		{
			*err = path + ": xml error in node [supply_recover_value_list].";
			return false;
		}

		PugiXmlNode DataElement = supply_recover_value_list_element.child("data");
		if (DataElement.empty())
		{
			*err = path + ": xml error in node [supply_recover_value_list->data].";
			return false;
		}

		while (!DataElement.empty())
		{
			int supply_type = -1;
			if (!PugiGetSubNodeValue(DataElement, "supply_type", supply_type) || supply_type < 0 || supply_type >= SUPPLY_TYPE_MAX)
			{
				*err = path + ": xml error in node [supply_recover_value_list->data->supply_type].";
				return false;
			}

			int level = 0;
			if (!PugiGetSubNodeValue(DataElement, "level", level) || level <= 0 || supply_type > MAX_ROLE_LEVEL)
			{
				*err = path + ": xml error in node [supply_recover_value_list->data->level].";
				return false;
			}

			if (!PugiGetSubNodeValue(DataElement, "recover_value", m_supply_cfg_list[supply_type].recover_value_list[level]) || m_supply_cfg_list[supply_type].recover_value_list[level] <= 0)
			{
				*err = path + ": xml error in node [supply_recover_value_list->data->recover_value].";
				return false;
			}

			if (level -1 > 0 && (m_supply_cfg_list[supply_type].recover_value_list[level - 1] <= 0 || 
				m_supply_cfg_list[supply_type].recover_value_list[level] < m_supply_cfg_list[supply_type].recover_value_list[level - 1]))
			{
				*err = path + ": xml error in node [supply_recover_value_list->data->level] not continully.";
				return false;
			}

			DataElement = DataElement.next_sibling();
		}
	}

	{
		PugiXmlNode supply_item_list_element = RootElement.child("supply_interval");
		if (supply_item_list_element.empty())
		{
			*err = path + ": xml error in node [supply_interval].";
			return false;
		}

		PugiXmlNode DataElement = supply_item_list_element.child("data");
		if (DataElement.empty())
		{
			*err = path + ": xml error in node [supply_interval->data].";
			return false;
		}

		while (!DataElement.empty())
		{
			int supply_type = -1;
			if (!PugiGetSubNodeValue(DataElement, "supply_type", supply_type) || supply_type < 0 || supply_type >= SUPPLY_TYPE_MAX)
			{
				*err = path + ": xml error in node [supply_interval->data->supply_type].";
				return false;
			}

			SupplyTypeCfg &supply_cfg = m_supply_cfg_list[supply_type];

			if (!PugiGetSubNodeValue(DataElement, "interval", supply_cfg.recover_interval_s) || supply_cfg.recover_interval_s <= 0)
			{
				*err = path + ": xml error in node [supply_interval->data->interval].";
				return false;
			}

			DataElement = DataElement.next_sibling();
		}
	}

	{
		PugiXmlNode other_element = RootElement.child("other");
		if (other_element.empty())
		{
			*err = path + ": xml error in node [other].";
			return false;
		}

		PugiXmlNode data_element = other_element.child("data");
		if (data_element.empty())
		{
			*err = path + ": xml error in node [other->data].";
			return false;
		}

		if (!PugiGetSubNodeValue(data_element, "not_fight_rate", m_not_fight_rate) || m_not_fight_rate < 0)
		{
			*err = path + ": xml error in node [other->data->not_fight_rate].";
			return false;
		}

		if (!PugiGetSubNodeValue(data_element, "recover_per", m_recover_per) || m_recover_per < 0)
		{
			*err = path + ": xml error in node [other->data->recover_per].";
			return false;
		}
	}

	return true;
}

const SupplyItem * SupplyConfig::GetSupplyItem(int supply_type, int index)
{
	if (supply_type < 0 || supply_type >= SUPPLY_TYPE_MAX || 
		index < 0 || index >= m_supply_cfg_list[supply_type].item_count)
	{
		return NULL;
	}

	return &m_supply_cfg_list[supply_type].supply_item_list[index];
}

int SupplyConfig::GetSupplyRecoverValue(int supply_type, int level)
{
	if (supply_type < 0 || supply_type >= SUPPLY_TYPE_MAX || 
		level <= 0 || level > MAX_ROLE_LEVEL)
	{
		return 0;
	}

	return m_supply_cfg_list[supply_type].recover_value_list[level];
}

int SupplyConfig::GetSupplyRecoverInterval(int supply_type)
{
	if (supply_type < 0 || supply_type >= SUPPLY_TYPE_MAX)
	{
		return 99999999;
	}

	return m_supply_cfg_list[supply_type].recover_interval_s;
}

