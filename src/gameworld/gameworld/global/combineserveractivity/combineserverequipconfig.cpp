#include "combineserverequipconfig.hpp"
#include "common/tinyxml/tinyxml.h"
#include "servercommon/configcommon.h"
#include "item/itempool.h"

CombineServerActivityEquipConfig::CombineServerActivityEquipConfig() : m_common_max_level(0), m_great_max_level(0)
{

}

CombineServerActivityEquipConfig::~CombineServerActivityEquipConfig()
{

}

bool CombineServerActivityEquipConfig::Init(const std::string &configname, std::string *err)
{
	PUGI_XML_CONFIG_PRE_LOAD;

	// 等级配置
	PUGI_XML_LOAD_CONFIG("common_equip_uplevel", InitCommonEquipLevelCfg);
	// 技能配置
	PUGI_XML_LOAD_CONFIG("great_equip_uplevel", InitGreatEquipLevelCfg);

	return true;
}

const CSACommonEquipUplevelItemCfg * CombineServerActivityEquipConfig::GetCommonlevelItemCfg(int part_index, int level)
{
	if (part_index < 0 || part_index >= CSA_EQUIP_MAX_PART || level <= 0 || level > CSA_EQUIP_MAX_LEVEL)
	{
		return NULL;
	}

	return &m_part_list[part_index].common_equip_uplevel_list[level];
}

const CSAGreatEquipUplevelItemCfg * CombineServerActivityEquipConfig::GetGreatlevelItemCfg(int part_index, int level)
{
	if (part_index < 0 || part_index >= CSA_EQUIP_MAX_PART || level <= 0 || level > CSA_EQUIP_MAX_LEVEL)
	{
		return NULL;
	}

	return &m_part_list[part_index].great_equip_uplevel_list[level];
}

int CombineServerActivityEquipConfig::InitCommonEquipLevelCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElem = RootElement.child("data");
	if (dataElem.empty())
	{
		return -10000;
	}

	int last_part_index = -1;
	int last_level = 0;

	while (!dataElem.empty())
	{
		int part_index = 0;
		if (!PugiGetSubNodeValue(dataElem, "index", part_index) || part_index >= CSA_EQUIP_MAX_PART)
		{
			return -1;
		}

		if (part_index != last_part_index)
		{
			if (1 != part_index - last_part_index)
			{
				return -200;
			}

			last_part_index = part_index;
			last_level = 0;
		}

		CSAPartItemCfg &part_cfg = m_part_list[part_index];
		part_cfg.part_index = part_index;

		int level = 0;
		if (!PugiGetSubNodeValue(dataElem, "level", level) || 1 != level - last_level || level > CSA_EQUIP_MAX_LEVEL)
		{
			return -2;
		}
		last_level = level;

		if (level > m_common_max_level)
		{
			m_common_max_level = level;
		}

		CSACommonEquipUplevelItemCfg &item_cfg = part_cfg.common_equip_uplevel_list[level];
		item_cfg.index = part_index;
		item_cfg.level = level;

		if (!PugiGetSubNodeValue(dataElem, "stuff_id", item_cfg.stuff_id) || NULL == ITEMPOOL->GetItem(item_cfg.stuff_id))
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElem, "stuff_num", item_cfg.stuff_num) || item_cfg.stuff_num <= 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElem, "gongji", item_cfg.gongji) || item_cfg.gongji < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElem, "fangyu", item_cfg.fangyu) || item_cfg.fangyu < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElem, "maxhp", item_cfg.maxhp) || item_cfg.maxhp < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElem, "mingzhong", item_cfg.mingzhong) || item_cfg.mingzhong < 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(dataElem, "shanbi", item_cfg.shanbi) || item_cfg.shanbi < 0)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(dataElem, "baoji", item_cfg.baoji) || item_cfg.baoji < 0)
		{
			return -13;
		}

		if (!PugiGetSubNodeValue(dataElem, "jianren", item_cfg.jianren) || item_cfg.jianren < 0)
		{
			return -14;
		}

		if (!PugiGetSubNodeValue(dataElem, "extra_gongji", item_cfg.extra_gongji) || item_cfg.extra_gongji < 0)
		{
			return -15;
		}

		if (!PugiGetSubNodeValue(dataElem, "extra_fangyu", item_cfg.extra_fangyu) || item_cfg.extra_fangyu < 0)
		{
			return -16;
		}

		if (!PugiGetSubNodeValue(dataElem, "extra_maxhp", item_cfg.extra_maxhp) || item_cfg.extra_maxhp < 0)
		{
			return -17;
		}

		if (!PugiGetSubNodeValue(dataElem, "extra_mingzhong", item_cfg.extra_mingzhong) || item_cfg.extra_mingzhong < 0)
		{
			return -18;
		}

		if (!PugiGetSubNodeValue(dataElem, "extra_shanbi", item_cfg.extra_shanbi) || item_cfg.extra_shanbi < 0)
		{
			return -19;
		}

		if (!PugiGetSubNodeValue(dataElem, "extra_baoji", item_cfg.extra_baoji) || item_cfg.extra_baoji < 0)
		{
			return -23;
		}

		if (!PugiGetSubNodeValue(dataElem, "extra_jianren", item_cfg.extra_jianren) || item_cfg.extra_jianren < 0)
		{
			return -24;
		}

		dataElem = dataElem.next_sibling();
	}

	return 0;
}

int CombineServerActivityEquipConfig::InitGreatEquipLevelCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElem = RootElement.child("data");
	if (dataElem.empty())
	{
		return -10000;
	}

	int last_part_index = -1;
	int last_level = 0;

	while (!dataElem.empty())
	{
		int part_index = 0;
		if (!PugiGetSubNodeValue(dataElem, "index", part_index) || part_index >= CSA_EQUIP_MAX_PART)
		{
			return -1;
		}

		if (part_index != last_part_index)
		{
			if (1 != part_index - last_part_index)
			{
				return -200;
			}

			last_part_index = part_index;
			last_level = 0;
		}

		CSAPartItemCfg &part_cfg = m_part_list[part_index];
		part_cfg.part_index = part_index;

		int level = 0;
		if (!PugiGetSubNodeValue(dataElem, "level", level) || 1 != level - last_level || level > CSA_EQUIP_MAX_LEVEL)
		{
			return -2;
		}
		last_level = level;

		if (level > m_great_max_level)
		{
			m_great_max_level = level;
		}

		CSAGreatEquipUplevelItemCfg &item_cfg = part_cfg.great_equip_uplevel_list[level];
		item_cfg.index = part_index;
		item_cfg.level = level;

		if (!PugiGetSubNodeValue(dataElem, "stuff_id", item_cfg.stuff_id) || NULL == ITEMPOOL->GetItem(item_cfg.stuff_id))
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElem, "stuff_num", item_cfg.stuff_num) || item_cfg.stuff_num <= 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElem, "gongji", item_cfg.gongji) || item_cfg.gongji < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElem, "fangyu", item_cfg.fangyu) || item_cfg.fangyu < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElem, "maxhp", item_cfg.maxhp) || item_cfg.maxhp < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElem, "mingzhong", item_cfg.mingzhong) || item_cfg.mingzhong < 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(dataElem, "shanbi", item_cfg.shanbi) || item_cfg.shanbi < 0)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(dataElem, "baoji", item_cfg.baoji) || item_cfg.baoji < 0)
		{
			return -13;
		}

		if (!PugiGetSubNodeValue(dataElem, "jianren", item_cfg.jianren) || item_cfg.jianren < 0)
		{
			return -14;
		}

		if (!PugiGetSubNodeValue(dataElem, "pvp_shanghai_jiacheng", item_cfg.pvp_shanghai_jiacheng) || item_cfg.jianren < 0)
		{
			return -15;

		}

		if (!PugiGetSubNodeValue(dataElem, "pvp_shanghai_jianmian", item_cfg.pvp_shanghai_jianmian) || item_cfg.jianren < 0)
		{
			return -16;
		}

		dataElem = dataElem.next_sibling();
	}

	return 0;
}