//#include "stdafx.h"

#include "shengqiconfig.hpp"
#include "item/itempool.h"
#include <servercommon/configcommon.h>

ShengqiConfig::ShengqiConfig()
{
}

ShengqiConfig::~ShengqiConfig()
{
}

bool ShengqiConfig::Init(const std::string &configname, std::string *err)
{
	PRE_LOAD_CONFIG;

	// ∆‰À˚≈‰÷√
	LOAD_CONFIG("other", InitOtherCfg);
	//  •∆˜≈‰÷√
	LOAD_CONFIG("shengqi", InitShengqiCfg);
	//  •∆˜ø™∆ÙÃıº˛≈‰÷√
	LOAD_CONFIG("open_condition", InitOpenConditionCfg);
	//  •∆˜∏Ω¡È≈‰÷√
	LOAD_CONFIG("spirit", InitSpiritCfg);
	//  •∆˜«øªØ≈‰÷√
	LOAD_CONFIG("strength", InitStrengthCfg);
	// ∑÷Ω‚≈‰÷√
	LOAD_CONFIG("decompose", InitDecomposeCfg);

	return true;
}

const ShengqiCfg * ShengqiConfig::GetShengqiCfg(int shengqi_index) const
{
	if (shengqi_index < 0 || shengqi_index >= SHENGQI_MAX_NUM)
	{
		return NULL;
	}

	return &m_shengqi_cfg_list[shengqi_index];
}

const ShengqiOpenConditionCfg * ShengqiConfig::GetOpenConditionCfg(int shengqi_index) const
{
	if (shengqi_index < 0 || shengqi_index >= SHENGQI_MAX_NUM)
	{
		return NULL;
	}

	ShengqiOpenConditionCfgMapIt it = m_open_condition_cfg_map.find(shengqi_index);
	if (it != m_open_condition_cfg_map.end())
	{
		return &it->second;
	}

	return NULL;
}

const ShengqiSpiritCfg * ShengqiConfig::GetSpiritCfg(int shengqi_index) const
{
	if (shengqi_index < 0 || shengqi_index >= SHENGQI_MAX_NUM)
	{
		return NULL;
	}

	ShengqiSpiritCfgMapIt it = m_spirit_cfg_map.find(shengqi_index);
	if (it != m_spirit_cfg_map.end())
	{
		return &it->second;
	}

	return NULL;
}

const ShengqiStrengthCfg * ShengqiConfig::GetStrengthCfg(int shengqi_index, int level) const
{
	if ((shengqi_index < 0 || shengqi_index >= SHENGQI_MAX_NUM) || (level < 0 && level > m_other_cfg.strength_max))
	{
		return NULL;
	}

	long long key = ConvertParamToLongLong(shengqi_index, level);
	ShengqiStrengthCfgMapIt it = m_strength_cfg_map.find(key);
	if (it != m_strength_cfg_map.end())
	{
		return &it->second;
	}

	return NULL;
}

int ShengqiConfig::GetShengqiIndexByItem(ItemID item_id)
{
	std::map<ItemID, int>::iterator it = m_item_to_shengqi_type_map.find(item_id);
	if (it != m_item_to_shengqi_type_map.end())
	{
		return it->second;
	}

	return -1;
}

ItemID ShengqiConfig::GetItemByShengqiIndex(int shengqi_index)
{
	if (shengqi_index < 0 || shengqi_index >= SHENGQI_MAX_NUM)
	{
		return -1;
	}

	return m_item_to_shengqi_type_map[shengqi_index];
}

const ShengqiDecomposeCfg * ShengqiConfig::GetDecomposeCfg(ItemID stuff_id) const
{
	if (stuff_id < 0) return NULL;

	for (auto &decompose_iter : m_decompose_map)
	{
		if (decompose_iter.first == stuff_id)
		{
			return &decompose_iter.second;
		}
	}

	return NULL;
}

int ShengqiConfig::InitOtherCfg(TiXmlElement *RootElement)
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

	if (!GetSubNodeValue(data_element, "open_gold", m_other_cfg.open_gold) || m_other_cfg.open_gold <= 0)
	{
		return -2;
	}

	if (!GetSubNodeValue(data_element, "spirit_max", m_other_cfg.spirit_max) || m_other_cfg.spirit_max <= 0 || m_other_cfg.spirit_max > SHENGQI_SPIRIT_MAX)
	{
		return -3;
	}

	if (!GetSubNodeValue(data_element, "strength_max", m_other_cfg.strength_max) || m_other_cfg.strength_max <= 0 || m_other_cfg.strength_max > SHENGQI_STRENGT_MAX)
	{
		return -4;
	}

	if (!GetSubNodeValue(data_element, "per_one_spirit_strength", m_other_cfg.per_one_spirit_strength) || m_other_cfg.per_one_spirit_strength <= 0)
	{
		return -5;
	}

	if (!GetSubNodeValue(data_element, "per_two_spirit_strength", m_other_cfg.per_two_spirit_strength) || m_other_cfg.per_two_spirit_strength <= 0)
	{
		return -6;
	}

	if (!GetSubNodeValue(data_element, "per_three_spirit_strength", m_other_cfg.per_three_spirit_strength) || m_other_cfg.per_three_spirit_strength <= 0)
	{
		return -7;
	}

	if (!GetSubNodeValue(data_element, "per_spirit_default", m_other_cfg.per_spirit_default) || m_other_cfg.per_spirit_default <= 0 || m_other_cfg.per_spirit_default >= SHENGQI_SPIRIT_MAX)
	{
		return -8;
	}

	return 0;
}

int ShengqiConfig::InitShengqiCfg(TiXmlElement *RootElement)
{
	TiXmlElement *data_element = RootElement->FirstChildElement("data");
	if (NULL == data_element)
	{
		return -1000;
	}

	int next_index = 0;

	while (NULL != data_element)
	{
		int index = -1;
		if (!GetSubNodeValue(data_element, "index", index) || index < 0 || index >= SHENGQI_MAX_NUM || index != next_index)
		{
			return -1;
		}

		ShengqiCfg &shengqi_cfg = m_shengqi_cfg_list[index];
		shengqi_cfg.index = index;

		if (!GetSubNodeValue(data_element, "id", shengqi_cfg.id) /*|| NULL == ITEMPOOL->GetItem(shengqi_cfg.id)*/)
		{
			return -2;
		}

		if (!GetSubNodeValue(data_element, "series", shengqi_cfg.series) || shengqi_cfg.series < 0)
		{
			return -3;
		}
		m_item_to_shengqi_type_map[shengqi_cfg.id] = index;

		++next_index;
		data_element = data_element->NextSiblingElement();
	}

	return 0;
}

int ShengqiConfig::InitOpenConditionCfg(TiXmlElement *RootElement)
{
	TiXmlElement *data_element = RootElement->FirstChildElement("data");
	if (NULL == data_element)
	{
		return -1000;
	}

	while (NULL != data_element)
	{
		ShengqiOpenConditionCfg cfg;

		if (!GetSubNodeValue(data_element, "index", cfg.index) || cfg.index < 0 || cfg.index >= SHENGQI_MAX_NUM)
		{
			return -1;
		}

		if (!GetSubNodeValue(data_element, "open_level", cfg.open_level) || cfg.open_level < 0)
		{
			return -2;
		}

		if (!GetSubNodeValue(data_element, "stuff_id", cfg.stuff_id) || cfg.stuff_id < 0)
		{
			return -3;
		}

		if (!GetSubNodeValue(data_element, "stuff_num", cfg.stuff_num) || cfg.stuff_num < 0)
		{
			return -4;
		}

		ShengqiOpenConditionCfgMapIt it = m_open_condition_cfg_map.find(cfg.index);
		if (it != m_open_condition_cfg_map.end())
		{
			return -100;
		}

		m_open_condition_cfg_map[cfg.index] = cfg;

		data_element = data_element->NextSiblingElement();
	}

	return 0;
}

int ShengqiConfig::InitSpiritCfg(TiXmlElement *RootElement)
{
	TiXmlElement *data_element = RootElement->FirstChildElement("data");
	if (NULL == data_element)
	{
		return -1000;
	}

	while (NULL != data_element)
	{
		ShengqiSpiritCfg cfg;

		if (!GetSubNodeValue(data_element, "index", cfg.index) || cfg.index < 0 || cfg.index >= SHENGQI_MAX_NUM)
		{
			return -1;
		}

		if (!GetSubNodeValue(data_element, "spirit_stuff_id", cfg.spirit_stuff_id) || NULL == ITEMPOOL->GetItem(cfg.spirit_stuff_id))
		{
			return -2;
		}

		if (!GetSubNodeValue(data_element, "spirit_stuff_num", cfg.spirit_stuff_num) || cfg.spirit_stuff_num <= 0)
		{
			return -3;
		}

		if (!GetSubNodeValue(data_element, "per_strength_min", cfg.per_strength_min) || cfg.per_strength_min <= 0)
		{
			return -4;
		}

		if (!GetSubNodeValue(data_element, "per_strength_max", cfg.per_strength_max) || cfg.per_strength_max <= 0)
		{
			return -5;
		}

		if (!GetSubNodeValue(data_element, "attr_type_1", cfg.attr_type[0]) || cfg.attr_type[0] < 0)
		{
			return -6;
		}

		if (!GetSubNodeValue(data_element, "attr_total_value_1", cfg.attr_total_value[0]) || cfg.attr_total_value[0] <= 0)
		{
			return -7;
		}

		if (!GetSubNodeValue(data_element, "attr_type_2", cfg.attr_type[1]) || cfg.attr_type[1] < 0)
		{
			return -8;
		}

		if (!GetSubNodeValue(data_element, "attr_total_value_2", cfg.attr_total_value[1]) || cfg.attr_total_value[1] <= 0)
		{
			return -9;
		}

		if (!GetSubNodeValue(data_element, "attr_type_3", cfg.attr_type[2]) || cfg.attr_type[2] < 0)
		{
			return -10;
		}

		if (!GetSubNodeValue(data_element, "attr_total_value_3", cfg.attr_total_value[2]) || cfg.attr_total_value[2] <= 0)
		{
			return -11;
		}

		if (!GetSubNodeValue(data_element, "attr_type_4", cfg.attr_type[3]) || cfg.attr_type[3] < 0)
		{
			return -12;
		}

		if (!GetSubNodeValue(data_element, "attr_total_value_4", cfg.attr_total_value[3]) || cfg.attr_total_value[3] <= 0)
		{
			return -13;
		}

		ShengqiSpiritCfgMapIt it = m_spirit_cfg_map.find(cfg.index);
		if (it != m_spirit_cfg_map.end())
		{
			return -100;
		}

		m_spirit_cfg_map[cfg.index] = cfg;

		data_element = data_element->NextSiblingElement();
	}

	return 0;
}

int ShengqiConfig::InitStrengthCfg(TiXmlElement *RootElement)
{
	TiXmlElement *data_element = RootElement->FirstChildElement("data");
	if (NULL == data_element)
	{
		return -1000;
	}

	while (NULL != data_element)
	{
		ShengqiStrengthCfg cfg;

		if (!GetSubNodeValue(data_element, "index", cfg.index) || cfg.index < 0 || cfg.index >= SHENGQI_MAX_NUM)
		{
			return -1;
		}

		if (!GetSubNodeValue(data_element, "level", cfg.level) || cfg.level <= 0)
		{
			return -2;
		}

		if (!GetSubNodeValue(data_element, "strength_stuff_id", cfg.strength_stuff_id) || NULL == ITEMPOOL->GetItem(cfg.strength_stuff_id))
		{
			return -3;
		}

		if (!GetSubNodeValue(data_element, "strength_stuff_num", cfg.strength_stuff_num) || cfg.strength_stuff_num <= 0)
		{
			return -4;
		}

		if (!GetSubNodeValue(data_element, "maxhp", cfg.maxhp) || cfg.maxhp < 0 || cfg.maxhp > INT_MAX)
		{
			return -2;
		}

		if (!GetSubNodeValue(data_element, "gongji", cfg.gongji) || cfg.gongji < 0 || cfg.gongji > INT_MAX)
		{
			return -3;
		}

		if (!GetSubNodeValue(data_element, "fangyu", cfg.fangyu) || cfg.fangyu < 0 || cfg.fangyu > INT_MAX)
		{
			return -4;
		}

		//if (!GetSubNodeValue(data_element, "fa_fangyu", cfg.fa_fangyu) || cfg.fa_fangyu < 0 || cfg.fa_fangyu > INT_MAX)
		//{
		//	return -5;
		//}

		//if (!GetSubNodeValue(data_element, "pojia", cfg.pojia) || cfg.pojia < 0 || cfg.pojia > INT_MAX)
		//{
		//	return -6;
		//}

		if (!GetSubNodeValue(data_element, "per_baoji", cfg.per_baoji) || cfg.per_baoji < 0 || cfg.per_baoji > INT_MAX)
		{
			return -5;
		}

		if (!GetSubNodeValue(data_element, "per_jingzhuan", cfg.per_jingzhuan) || cfg.per_jingzhuan < 0 || cfg.per_jingzhuan > INT_MAX)
		{
			return -6;
		}

		if (!GetSubNodeValue(data_element, "mingzhong", cfg.mingzhong) || cfg.mingzhong < 0 || cfg.mingzhong > INT_MAX)
		{
			return -7;
		}

		if (!GetSubNodeValue(data_element, "shanbi", cfg.shanbi) || cfg.shanbi < 0 || cfg.shanbi > INT_MAX)
		{
			return -8;
		}

		if (!GetSubNodeValue(data_element, "baoji", cfg.baoji) || cfg.baoji < 0 || cfg.baoji > INT_MAX)
		{
			return -9;
		}

		if (!GetSubNodeValue(data_element, "jianren", cfg.jianren) || cfg.jianren < 0 || cfg.jianren > INT_MAX)
		{
			return -10;
		}

		if (!GetSubNodeValue(data_element, "per_kangbao", cfg.per_kangbao) || cfg.per_kangbao < 0 || cfg.per_kangbao > INT_MAX)
		{
			return -11;
		}

		if (!GetSubNodeValue(data_element, "skill_jianshang_per", cfg.skill_jianshang_per) || cfg.skill_jianshang_per < 0 || cfg.skill_jianshang_per > INT_MAX)
		{
			return -12;
		}

		if (!GetSubNodeValue(data_element, "shanbi_per", cfg.shanbi_per) || cfg.shanbi_per < 0 || cfg.shanbi_per > INT_MAX)
		{
			return -13;
		}

		if (!GetSubNodeValue(data_element, "per_mianshang", cfg.per_mianshang) || cfg.per_mianshang < 0 || cfg.per_mianshang > INT_MAX)
		{
			return -14;
		}

		if (!GetSubNodeValue(data_element, "mingzhong_per", cfg.mingzhong_per) || cfg.mingzhong_per < 0 || cfg.mingzhong_per > INT_MAX)
		{
			return -15;
		}

		if (!GetSubNodeValue(data_element, "gedang_per", cfg.gedang_per) || cfg.gedang_per < 0 || cfg.gedang_per > INT_MAX)
		{
			return -16;
		}

		if (!GetSubNodeValue(data_element, "per_pofang", cfg.per_pofang) || cfg.per_pofang < 0 || cfg.per_pofang > INT_MAX)
		{
			return -17;
		}

		long long key = ConvertParamToLongLong(cfg.index, cfg.level);
		ShengqiStrengthCfgMapIt it = m_strength_cfg_map.find(key);
		if (it != m_strength_cfg_map.end())
		{
			return -100;
		}

		m_strength_cfg_map[key] = cfg;

		data_element = data_element->NextSiblingElement();
	}

	return 0;
}

int ShengqiConfig::InitDecomposeCfg(TiXmlElement *RootElement)
{
	TiXmlElement *dataElement = RootElement->FirstChildElement("data");
	if (NULL == dataElement)
	{
		return -10000;
	}


	while (NULL != dataElement)
	{
		ItemID stuff_id = 0;
		if (!GetSubNodeValue(dataElement, "stuff_id", stuff_id) || stuff_id < 0)
		{
			return -1;
		}

		if (m_decompose_map.find(stuff_id) != m_decompose_map.end())
		{
			return -100;
		}

		ShengqiDecomposeCfg &cfg = m_decompose_map[stuff_id];
		cfg.stuff_id = stuff_id;

		{
			TiXmlElement *item_element = dataElement->FirstChildElement("return_item");
			if (NULL == item_element)
			{
				return -2;
			}

			if (!cfg.return_item.ReadConfig(item_element))
			{
				return -101;
			}
		}

		dataElement = dataElement->NextSiblingElement();
	}

	return 0;
}
