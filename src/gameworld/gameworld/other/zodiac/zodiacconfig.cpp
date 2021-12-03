#include "stdafx.h"

#include "zodiacconfig.hpp"
#include "item/itempool.h"

ZodiacConfig::ZodiacConfig()
{
}

ZodiacConfig::~ZodiacConfig()
{
}

bool ZodiacConfig::Init(const std::string &configname, std::string *err)
{
	PRE_LOAD_CONFIG;

	// ÆäËûÅäÖÃ
	LOAD_CONFIG("other", InitOtherCfg);
	// ÊµÎïidÐéÄâid¶ÔÕÕÅäÖÃ
	LOAD_CONFIG("real_id_list", InitIdListCfg);
	// ¼¤»îÅäÖÃ
	LOAD_CONFIG("activate", InitActivateCfg);
	// Éý¼¶ÅäÖÃ
	LOAD_CONFIG("levelup", InitLevelupCfg);
	// ·Ö½âÅäÖÃ
	LOAD_CONFIG("decompose", InitDecomposeCfg);

	return true;
}

const ZodiacRealIdCfg * ZodiacConfig::GetRealIdCfg(ItemID item_id) const
{
	ZodiacRealIdCfgMapIt it = m_zodiac_id_cfg_map.find(item_id);
	if (it != m_zodiac_id_cfg_map.end())
	{
		return &it->second;
	}

	return NULL;
}

const ZodiacActivateCfg * ZodiacConfig::GetActivateCfg(int zodiac_index, int activate_num) const
{
	if (zodiac_index < 0 || zodiac_index >= ZODIAC_MAX_NUM || activate_num <= 0 || activate_num > ZODIAC_SUIPIAN_MAX_NUM)
	{
		return NULL;
	}

	long long key = ConvertParamToLongLong(zodiac_index, activate_num);
	ZodiacActivateCfgMapIt it = m_activate_cfg_map.find(key);
	if (it != m_activate_cfg_map.end())
	{
		return &it->second;
	}

	return NULL;
}

const ZodiacLevelupCfg * ZodiacConfig::GetLevelupCfg(int zodiac_index, int level) const
{
	if (zodiac_index < 0 || zodiac_index >= ZODIAC_MAX_NUM || level < 0)
	{
		return NULL;
	}

	long long key = ConvertParamToLongLong(zodiac_index, level);
	ZodiacLevelupCfgMapIt it = m_levelup_cfg_map.find(key);
	if (it != m_levelup_cfg_map.end())
	{
		return &it->second;
	}

	return NULL;
}

const ZodiacDecomposeCfg * ZodiacConfig::GetDecomposeCfg(int zodiac_index, int suipian_index) const
{
	if (zodiac_index < 0 || zodiac_index >= ZODIAC_MAX_NUM || suipian_index < 0 || suipian_index >= ZODIAC_SUIPIAN_MAX_NUM)
	{
		return NULL;
	}

	long long key = ConvertParamToLongLong(zodiac_index, suipian_index);
	ZodiacDecomposeCfgMapIt it = m_decompose_cfg_map.find(key);
	if (it != m_decompose_cfg_map.end())
	{
		return &it->second;
	}

	return NULL;
}

int ZodiacConfig::InitOtherCfg(TiXmlElement *RootElement)
{
	TiXmlElement *data_element = RootElement->FirstChildElement("data");
	if (NULL == data_element)
	{
		return -1000;
	}

	if (!GetSubNodeValue(data_element, "open_level", m_other_cfg.open_level) || m_other_cfg.open_level <= 0)
	{
		return -1;
	}

	if (!GetSubNodeValue(data_element, "max_level", m_other_cfg.max_level) || m_other_cfg.max_level <= 0)
	{
		return -1;
	}

	return 0;
}

int ZodiacConfig::InitIdListCfg(TiXmlElement *RootElement)
{
	TiXmlElement *data_element = RootElement->FirstChildElement("data");
	if (NULL == data_element)
	{
		return -1000;
	}

	while (NULL != data_element)
	{
		ZodiacRealIdCfg cfg;

		if (!GetSubNodeValue(data_element, "suipian_id", cfg.suipian_id) || NULL == ITEMPOOL->GetItem(cfg.suipian_id))
		{
			return -1;
		}

		if (!GetSubNodeValue(data_element, "zodiac_index", cfg.zodiac_index) || cfg.zodiac_index < 0 || cfg.zodiac_index >= ZODIAC_MAX_NUM)
		{
			return -2;
		}

		if (!GetSubNodeValue(data_element, "suipian_index", cfg.suipian_index) || cfg.suipian_index < 0 || cfg.suipian_index >= ZODIAC_SUIPIAN_MAX_NUM)
		{
			return -3;
		}
		
		ZodiacRealIdCfgMapIt it = m_zodiac_id_cfg_map.find(cfg.suipian_id);
		if (it != m_zodiac_id_cfg_map.end())
		{
			return -100;
		}

		m_zodiac_id_cfg_map[cfg.suipian_id] = cfg;

		data_element = data_element->NextSiblingElement();
	}

	return 0;
}

int ZodiacConfig::InitActivateCfg(TiXmlElement *RootElement)
{
	TiXmlElement *data_element = RootElement->FirstChildElement("data");
	if (NULL == data_element)
	{
		return -1000;
	}

	while (NULL != data_element)
	{
		ZodiacActivateCfg cfg;

		if (!GetSubNodeValue(data_element, "zodiac_index", cfg.zodiac_index) || cfg.zodiac_index < 0 || cfg.zodiac_index >= ZODIAC_MAX_NUM)
		{
			return -1;
		}

		if (!GetSubNodeValue(data_element, "activate_num", cfg.activate_num) || cfg.activate_num <= 0 || cfg.activate_num > ZODIAC_SUIPIAN_MAX_NUM)
		{
			return -2;
		}

		if (!GetSubNodeValue(data_element, "attr_type", cfg.attr_type) || !CharIntAttrs::IsRoleBaseAttr(cfg.attr_type))
		{
			return -3;
		}

		if (!GetSubNodeValue(data_element, "attr_value", cfg.attr_value) || cfg.attr_value < 0)
		{
			return -4;
		}

		long long key = ConvertParamToLongLong(cfg.zodiac_index, cfg.activate_num);
		ZodiacActivateCfgMapIt it = m_activate_cfg_map.find(key);
		if (it != m_activate_cfg_map.end())
		{
			return -100;
		}

		m_activate_cfg_map[key] = cfg;

		data_element = data_element->NextSiblingElement();
	}

	return 0;
}

int ZodiacConfig::InitLevelupCfg(TiXmlElement *RootElement)
{
	TiXmlElement *data_element = RootElement->FirstChildElement("data");
	if (NULL == data_element)
	{
		return -1000;
	}

	while (NULL != data_element)
	{
		ZodiacLevelupCfg cfg;

		if (!GetSubNodeValue(data_element, "zodiac_index", cfg.zodiac_index) || cfg.zodiac_index < 0 || cfg.zodiac_index >= ZODIAC_MAX_NUM)
		{
			return -1;
		}

		if (!GetSubNodeValue(data_element, "level", cfg.level) || cfg.level < 0)
		{
			return -2;
		}

		if (!GetSubNodeValue(data_element, "jinghua_num", cfg.jinghua_num) || cfg.jinghua_num <= 0)
		{
			return -3;
		}
		std::string attr_type = "attr_type_";
		std::string attr_value = "attr_value_";
		for (int i = 0; i < ZODIAC_ATTR_MAX_NUM; ++i)
		{
			if (!GetSubNodeValue(data_element, attr_type + std::to_string(i), cfg.attr_type[i]) || !CharIntAttrs::IsRoleBaseAttr(cfg.attr_type[i]))
			{
				return -4 - i * 2;
			}
			if (!GetSubNodeValue(data_element, attr_value + std::to_string(i), cfg.attr_value[i]) || cfg.attr_value[i] < 0)
			{
				return -5 - i * 2;
			}
		}

		long long key = ConvertParamToLongLong(cfg.zodiac_index, cfg.level);
		ZodiacLevelupCfgMapIt it = m_levelup_cfg_map.find(key);
		if (it != m_levelup_cfg_map.end())
		{
			return -100;
		}

		m_levelup_cfg_map[key] = cfg;

		data_element = data_element->NextSiblingElement();
	}

	return 0;
}

int ZodiacConfig::InitDecomposeCfg(TiXmlElement *RootElement)
{
	TiXmlElement *data_element = RootElement->FirstChildElement("data");
	if (NULL == data_element)
	{
		return -1000;
	}

	while (NULL != data_element)
	{
		ZodiacDecomposeCfg cfg;

		if (!GetSubNodeValue(data_element, "zodiac_index", cfg.zodiac_index) || cfg.zodiac_index < 0 || cfg.zodiac_index >= ZODIAC_MAX_NUM)
		{
			return -1;
		}

		if (!GetSubNodeValue(data_element, "suipian_index", cfg.suipian_index) || cfg.suipian_index < 0 || cfg.suipian_index >= ZODIAC_SUIPIAN_MAX_NUM)
		{
			return -2;
		}

		if (!GetSubNodeValue(data_element, "jinghua_num", cfg.jinghua_num) || cfg.jinghua_num <= 0)
		{
			return -3;
		}

		long long key = ConvertParamToLongLong(cfg.zodiac_index, cfg.suipian_index);
		ZodiacDecomposeCfgMapIt it = m_decompose_cfg_map.find(key);
		if (it != m_decompose_cfg_map.end())
		{
			return -100;
		}

		m_decompose_cfg_map[key] = cfg;

		data_element = data_element->NextSiblingElement();
	}

	return 0;
}
