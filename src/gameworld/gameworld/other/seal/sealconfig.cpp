//#include "stdafx.h"

#include "sealconfig.hpp"
#include "item/itempool.h"

SealConfig::SealConfig()
{

}

SealConfig::~SealConfig()
{
}

bool SealConfig::Init(const std::string &configname, std::string *err)
{
	PRE_LOAD_CONFIG;

	//  •”°ø◊ø™∆Ù≈‰÷√
	LOAD_CONFIG("slot_open", InitSlotOpenCfg);
	//  µŒÔid–Èƒ‚id∂‘’’≈‰÷√
	LOAD_CONFIG("real_id_list", InitIdListCfg);
	//  •”°≥ı º Ù–‘≈‰÷√
	LOAD_CONFIG("initial_attr", InitInitialAttrCfg);
	//  •”°≈‰÷√
	LOAD_CONFIG("seal", InitSealItemCfg);
	//  •”°ªÿ ’≥ı ºª˝∑÷≈‰÷√
	LOAD_CONFIG("initial_score", InitInitialScoreCfg);
	//  •”°ª˝∑÷≈‰÷√
	LOAD_CONFIG("score", InitScoreCfg);
	//  •”°«øªØ≈‰÷√
	LOAD_CONFIG("strength", InitStrengthCfg);
	// Ã◊◊∞≈‰÷√
	LOAD_CONFIG("suit", InitSuitCfg);
	// Ã◊◊∞≤øŒª∂‘’’≈‰÷√
	LOAD_CONFIG("suit_part_list", InitPartListCfg);
	//  •ªÍ≈‰÷√
	LOAD_CONFIG("soul", InitSoulCfg);
	//  •ªÍ π”√œﬁ÷∆≈‰÷√
	LOAD_CONFIG("soul_use_limit", InitSoulUseLimitCfg);

	return true;
}

int SealConfig::GetOpenSlotIndex(int role_level)
{
	for (std::vector<SealSlotOpenLevelCfg>::reverse_iterator open_cfg_it = m_slot_open_cfg_list.rbegin(); open_cfg_it != m_slot_open_cfg_list.rend(); ++open_cfg_it)
	{
		if (role_level >= open_cfg_it->role_level)
		{
			return open_cfg_it->slot_index;
		}
	}

	return 0;
}

const SealRealIdCfg * SealConfig::GetRealIdCfg(ItemID seal_id) const
{
	SealRealIdCfgMapIt it = m_seal_id_cfg_map.find(seal_id);
	if (it != m_seal_id_cfg_map.end())
	{
		return &it->second;
	}

	return NULL;
}

const SealInitialAttrCfg * SealConfig::GetInitialAttrCfg(int slot_index, int order, int color)
{
	if ((slot_index <= 0 || slot_index > SEAL_SLOT_MAX_NUM) || order <= 0 || order > SEAL_ORDER_MAX_NUM || color < 0 || color >= RUNE_COLOR_KIND_MAX)
	{
		return NULL;
	}

	for (std::vector<SealInitialAttrCfg>::iterator it = m_initial_attr_cfg_list.begin(); it != m_initial_attr_cfg_list.end(); ++it)
	{
		if (slot_index == (*it).slot_index && order == (*it).order && color == (*it).color)
		{
			return &(*it);
		}
	}

	return NULL;
}

const SealItemCfg * SealConfig::GetSealItemCfg(int slot_index) const
{
	if (slot_index <= 0 || slot_index > SEAL_SLOT_MAX_NUM)
	{
		return NULL;
	}

	SealItemCfgMapIt it = m_seal_item_cfg_map.find(slot_index);
	if (it != m_seal_item_cfg_map.end())
	{
		return &it->second;
	}

	return NULL;
}

const int SealConfig::GetInitialHunScore(int color) const
{
	if (color >= 0 && color < RUNE_COLOR_KIND_MAX)
	{
		return m_initial_score_list[color].hun_score;
	}

	return -1;
}

const int SealConfig::GetInitialJinghuaScore(int color) const
{
	if (color >= 0 && color < RUNE_COLOR_KIND_MAX)
	{
		return m_initial_score_list[color].jinghua_hun_score;
	}

	return -1;
}

const int SealConfig::GetSealScore(int level) const
{
	if (level < 0 || level >= (int)m_score_cfg_list.size())
	{
		return -1;
	}

	for (int i = 0; i < (int)m_score_cfg_list.size(); ++i)
	{
		if (m_score_cfg_list[i].level == level)
		{
			return m_score_cfg_list[i].hun_score;
		}
	}

	return -1;
}

const SealStrengthCfg * SealConfig::GetStrengthCfg(int level) const
{
	if (level <= 0)
	{
		return NULL;
	}

	for (int i = (int)m_strength_cfg_list.size() - 1; i >= 0; --i)
	{
		if (level >= m_strength_cfg_list[i].level)
		{
			return &m_strength_cfg_list[i];
		}
	}

	return NULL;
}

const SealSuitCfg * SealConfig::GetSuitCfg(int suit_type, int same_order_num) const
{
	if (suit_type <= 0 || same_order_num <= 0)
	{
		return NULL;
	}

	long long key = ConvertParamToLongLong(suit_type, same_order_num);
	SealSuitCfgMapIt it = m_suit_cfg_map.find(key);
	if (it != m_suit_cfg_map.end())
	{
		return &it->second;
	}

	return NULL;
}

const SealSuitPartCfg * SealConfig::GetSuitPartCfg(int equip_order, int equip_part) const
{
	if (equip_order <= 0 || equip_part <= 0)
	{
		return NULL;
	}

	long long key = ConvertParamToLongLong(equip_order, equip_part);
	SealSuitPartCfgMapIt it = m_suit_part_cfg_map.find(key);
	if (it != m_suit_part_cfg_map.end())
	{
		return &it->second;
	}

	return NULL;
}

const SealSoulCfg * SealConfig::GetSoulCfg(int soul_type) const
{
	if (soul_type <= 0 || soul_type > SEAL_SOUL_MAX_TYPE)
	{
		return NULL;
	}

	return &m_soul_cfg_list[soul_type];
}

const int SealConfig::GetSoulUseLimit(int role_level) const
{
	if (role_level <= 0)
	{
		return -1;
	}

	for (int i = (int)m_soul_use_limit_cfg_list.size() - 1; i >= 0; --i)
	{
		if (role_level >= m_soul_use_limit_cfg_list[i].role_level)
		{
			return m_soul_use_limit_cfg_list[i].use_limit_num;
		}
	}

	return -1;
}

int SealConfig::InitSlotOpenCfg(TiXmlElement * RootElement)
{
	TiXmlElement *data_element = RootElement->FirstChildElement("data");
	if (NULL == data_element)
	{
		return -1000;
	}

	int last_slot_index = 0;

	while (NULL != data_element)
	{
		SealSlotOpenLevelCfg cfg;

		if (!GetSubNodeValue(data_element, "role_level", cfg.role_level) || cfg.role_level <= 0)
		{
			return -1;
		}

		if (!GetSubNodeValue(data_element, "slot_index", cfg.slot_index) || cfg.slot_index <= last_slot_index || cfg.slot_index > SEAL_SLOT_MAX_NUM)
		{
			return -2;
		}
		last_slot_index = cfg.slot_index;

		m_slot_open_cfg_list.push_back(cfg);

		data_element = data_element->NextSiblingElement();
	}

	return 0;
}

int SealConfig::InitIdListCfg(TiXmlElement * RootElement)
{
	TiXmlElement *data_element = RootElement->FirstChildElement("data");
	if (NULL == data_element)
	{
		return -1000;
	}

	while (NULL != data_element)
	{
		SealRealIdCfg cfg;

		if (!GetSubNodeValue(data_element, "seal_id", cfg.seal_id) || NULL == ITEMPOOL->GetItem(cfg.seal_id))
		{
			return -1;
		}

		if (!GetSubNodeValue(data_element, "slot_index", cfg.slot_index) || cfg.slot_index < 0 || cfg.slot_index > SEAL_SLOT_MAX_NUM)
		{
			return -2;
		}

		if (!GetSubNodeValue(data_element, "order", cfg.order) || cfg.order < 0 || cfg.order > SEAL_ORDER_MAX_NUM)
		{
			return -3;
		}

		if (!GetSubNodeValue(data_element, "color", cfg.color) || cfg.color < RUNE_COLOR_KIND_GREEN || cfg.color >= RUNE_COLOR_KIND_MAX)
		{
			return -4;
		}

		SealRealIdCfgMapIt it = m_seal_id_cfg_map.find(cfg.seal_id);
		if (it != m_seal_id_cfg_map.end())
		{
			return -100;
		}

		m_seal_id_cfg_map[cfg.seal_id] = cfg;

		data_element = data_element->NextSiblingElement();
	}

	return 0;
}

int SealConfig::InitInitialAttrCfg(TiXmlElement *RootElement)
{
	TiXmlElement *data_element = RootElement->FirstChildElement("data");
	if (NULL == data_element)
	{
		return -1000;
	}

	while (NULL != data_element)
	{
		SealInitialAttrCfg cfg;

		if (!GetSubNodeValue(data_element, "slot_index", cfg.slot_index) || cfg.slot_index <= 0 || cfg.slot_index > SEAL_SLOT_MAX_NUM)
		{
			return -1;
		}

		if (!GetSubNodeValue(data_element, "order", cfg.order) || cfg.order <= 0 || cfg.order > SEAL_ORDER_MAX_NUM)
		{
			return -2;
		}

		if (!GetSubNodeValue(data_element, "color", cfg.color) || cfg.color < RUNE_COLOR_KIND_GREEN || cfg.color >= RUNE_COLOR_KIND_MAX)
		{
			return -3;
		}

		if (!GetSubNodeValue(data_element, "gongji", cfg.gongji) || cfg.gongji < 0 || cfg.gongji > INT_MAX)
		{
			return -4;
		}

		if (!GetSubNodeValue(data_element, "pojia", cfg.pojia) || cfg.pojia < 0 || cfg.pojia > INT_MAX)
		{
			return -5;
		}

		if (!GetSubNodeValue(data_element, "yaolichuantou", cfg.yaolichuantou) || cfg.yaolichuantou < 0 || cfg.yaolichuantou > INT_MAX)
		{
			return -6;
		}

		if (!GetSubNodeValue(data_element, "yaolidikang", cfg.yaolidikang) || cfg.yaolidikang < 0 || cfg.yaolidikang > INT_MAX)
		{
			return -7;
		}

		if (!GetSubNodeValue(data_element, "molichuantou", cfg.molichuantou) || cfg.molichuantou < 0 || cfg.molichuantou > INT_MAX)
		{
			return -8;
		}

		if (!GetSubNodeValue(data_element, "molidikang", cfg.molidikang) || cfg.molidikang < 0 || cfg.molidikang > INT_MAX)
		{
			return -9;
		}

		if (!GetSubNodeValue(data_element, "shenlichuantou", cfg.shenlichuantou) || cfg.shenlichuantou < 0 || cfg.shenlichuantou > INT_MAX)
		{
			return -10;
		}

		if (!GetSubNodeValue(data_element, "shenlidikang", cfg.shenlidikang) || cfg.shenlidikang < 0 || cfg.shenlidikang > INT_MAX)
		{
			return -11;
		}

		if (!GetSubNodeValue(data_element, "maxhp", cfg.maxhp) || cfg.maxhp < 0 || cfg.maxhp > INT_MAX)
		{
			return -12;
		}

		if (!GetSubNodeValue(data_element, "fangyu", cfg.fangyu) || cfg.fangyu < 0 || cfg.fangyu > INT_MAX)
		{
			return -13;
		}

		if (!GetSubNodeValue(data_element, "per_baoji", cfg.per_baoji) || cfg.per_baoji < 0 || cfg.per_baoji > INT_MAX)
		{
			return -14;
		}

		if (!GetSubNodeValue(data_element, "baoji", cfg.baoji) || cfg.baoji < 0 || cfg.baoji > INT_MAX)
		{
			return -15;
		}

		if (!GetSubNodeValue(data_element, "per_baoji_jiacheng", cfg.per_baoji_jiacheng) || cfg.per_baoji_jiacheng < 0 || cfg.per_baoji_jiacheng > INT_MAX)
		{
			return -16;
		}

		if (!GetSubNodeValue(data_element, "per_maxhp", cfg.per_maxhp) || cfg.per_maxhp < 0 || cfg.per_maxhp > INT_MAX)
		{
			return -17;
		}

		if (!GetSubNodeValue(data_element, "per_fangyu", cfg.per_fangyu) || cfg.per_fangyu < 0 || cfg.per_fangyu > INT_MAX)
		{
			return -18;
		}

		if (!GetSubNodeValue(data_element, "per_gongji", cfg.per_gongji) || cfg.per_gongji < 0 || cfg.per_gongji > INT_MAX)
		{
			return -19;
		}

		if (!GetSubNodeValue(data_element, "per_pojia", cfg.per_pojia) || cfg.per_pojia < 0 || cfg.per_pojia > INT_MAX)
		{
			return -20;
		}

		if (!GetSubNodeValue(data_element, "per_shanghaijiacheng", cfg.per_shanghaijiacheng) || cfg.per_shanghaijiacheng < 0 || cfg.per_shanghaijiacheng > INT_MAX)
		{
			return -21;
		}

		if (!GetSubNodeValue(data_element, "per_shanghaijianshao", cfg.per_shanghaijianshao) || cfg.per_shanghaijianshao < 0 || cfg.per_shanghaijianshao > INT_MAX)
		{
			return -22;
		}

		if (!GetSubNodeValue(data_element, "per_base_attr_jiacheng", cfg.per_base_attr_jiacheng) || cfg.per_base_attr_jiacheng < 0 || cfg.per_base_attr_jiacheng > INT_MAX)
		{
			return -23;
		}

		if (!GetSubNodeValue(data_element, "mingzhong", cfg.mingzhong) || cfg.mingzhong < 0 || cfg.mingzhong > INT_MAX)
		{
			return -24;
		}

		if (!GetSubNodeValue(data_element, "shanbi", cfg.shanbi) || cfg.shanbi < 0 || cfg.shanbi > INT_MAX)
		{
			return -25;
		}

		if (!GetSubNodeValue(data_element, "jianren", cfg.jianren) || cfg.jianren < 0 || cfg.jianren > INT_MAX)
		{
			return -26;
		}

		m_initial_attr_cfg_list.push_back(cfg);

		data_element = data_element->NextSiblingElement();
	}

	return 0;
}

int SealConfig::InitSealItemCfg(TiXmlElement * RootElement)
{
	TiXmlElement *data_element = RootElement->FirstChildElement("data");
	if (NULL == data_element)
	{
		return -1000;
	}

	while (NULL != data_element)
	{
		SealItemCfg cfg;

		if (!GetSubNodeValue(data_element, "slot_index", cfg.slot_index) || cfg.slot_index <= 0 || cfg.slot_index > SEAL_SLOT_MAX_NUM)
		{
			return -1;
		}

		if (!GetSubNodeValue(data_element, "gongji", cfg.gongji) || cfg.gongji < 0 || cfg.gongji > INT_MAX)
		{
			return -2;
		}

		if (!GetSubNodeValue(data_element, "pojia", cfg.pojia) || cfg.pojia < 0 || cfg.pojia > INT_MAX)
		{
			return -3;
		}

		if (!GetSubNodeValue(data_element, "yaolichuantou", cfg.yaolichuantou) || cfg.yaolichuantou < 0 || cfg.yaolichuantou > INT_MAX)
		{
			return -4;
		}

		if (!GetSubNodeValue(data_element, "yaolidikang", cfg.yaolidikang) || cfg.yaolidikang < 0 || cfg.yaolidikang > INT_MAX)
		{
			return -5;
		}

		if (!GetSubNodeValue(data_element, "molichuantou", cfg.molichuantou) || cfg.molichuantou < 0 || cfg.molichuantou > INT_MAX)
		{
			return -6;
		}

		if (!GetSubNodeValue(data_element, "molidikang", cfg.molidikang) || cfg.molidikang < 0 || cfg.molidikang > INT_MAX)
		{
			return -7;
		}

		if (!GetSubNodeValue(data_element, "shenlichuantou", cfg.shenlichuantou) || cfg.shenlichuantou < 0 || cfg.shenlichuantou > INT_MAX)
		{
			return -8;
		}

		if (!GetSubNodeValue(data_element, "shenlidikang", cfg.shenlidikang) || cfg.shenlidikang < 0 || cfg.shenlidikang > INT_MAX)
		{
			return -9;
		}

		if (!GetSubNodeValue(data_element, "maxhp", cfg.maxhp) || cfg.maxhp < 0 || cfg.maxhp > INT_MAX)
		{
			return -10;
		}

		if (!GetSubNodeValue(data_element, "fangyu", cfg.fangyu) || cfg.fangyu < 0 || cfg.fangyu > INT_MAX)
		{
			return -11;
		}

		if (!GetSubNodeValue(data_element, "per_baoji", cfg.per_baoji) || cfg.per_baoji < 0 || cfg.per_baoji > INT_MAX)
		{
			return -12;
		}

		if (!GetSubNodeValue(data_element, "baoji", cfg.baoji) || cfg.baoji < 0 || cfg.baoji > INT_MAX)
		{
			return -13;
		}

		if (!GetSubNodeValue(data_element, "per_baoji_jiacheng", cfg.per_baoji_jiacheng) || cfg.per_baoji_jiacheng < 0 || cfg.per_baoji_jiacheng > INT_MAX)
		{
			return -14;
		}

		if (!GetSubNodeValue(data_element, "per_maxhp", cfg.per_maxhp) || cfg.per_maxhp < 0 || cfg.per_maxhp > INT_MAX)
		{
			return -15;
		}

		if (!GetSubNodeValue(data_element, "per_fangyu", cfg.per_fangyu) || cfg.per_fangyu < 0 || cfg.per_fangyu > INT_MAX)
		{
			return -16;
		}

		if (!GetSubNodeValue(data_element, "per_gongji", cfg.per_gongji) || cfg.per_gongji < 0 || cfg.per_gongji > INT_MAX)
		{
			return -17;
		}

		if (!GetSubNodeValue(data_element, "per_pojia", cfg.per_pojia) || cfg.per_pojia < 0 || cfg.per_pojia > INT_MAX)
		{
			return -18;
		}

		if (!GetSubNodeValue(data_element, "per_shanghaijiacheng", cfg.per_shanghaijiacheng) || cfg.per_shanghaijiacheng < 0 || cfg.per_shanghaijiacheng > INT_MAX)
		{
			return -19;
		}

		if (!GetSubNodeValue(data_element, "per_shanghaijianshao", cfg.per_shanghaijianshao) || cfg.per_shanghaijianshao < 0 || cfg.per_shanghaijianshao > INT_MAX)
		{
			return -20;
		}

		SealItemCfgMapIt it = m_seal_item_cfg_map.find(cfg.slot_index);
		if (it != m_seal_item_cfg_map.end())
		{
			return -100;
		}

		m_seal_item_cfg_map[cfg.slot_index] = cfg;

		data_element = data_element->NextSiblingElement();
	}

	return 0;
}

int SealConfig::InitInitialScoreCfg(TiXmlElement *RootElement)
{
	TiXmlElement *data_element = RootElement->FirstChildElement("data");
	if (NULL == data_element)
	{
		return -1000;
	}

	int next_color = 0;

	while (NULL != data_element)
	{
		int color = -1;
		if (!GetSubNodeValue(data_element, "color", color) || color < RUNE_COLOR_KIND_GREEN || color >= RUNE_COLOR_KIND_MAX || color != next_color)
		{
			return -1;
		}

		SealInitialScoreCfg &cfg = m_initial_score_list[color];
		cfg.color = color;

		if (!GetSubNodeValue(data_element, "hun_score", cfg.hun_score) || cfg.hun_score <= 0)
		{
			return -2;
		}

		if (!GetSubNodeValue(data_element, "jinghua_hun_score", cfg.jinghua_hun_score) || cfg.jinghua_hun_score < 0)
		{
			return -3;
		}

		++next_color;
		data_element = data_element->NextSiblingElement();
	}

	return 0;
}

int SealConfig::InitScoreCfg(TiXmlElement * RootElement)
{
	TiXmlElement *data_element = RootElement->FirstChildElement("data");
	if (NULL == data_element)
	{
		return -1000;
	}

	int next_level = 0;

	while (NULL != data_element)
	{
		SealScoreCfg cfg;

		if (!GetSubNodeValue(data_element, "level", cfg.level) || cfg.level < 0 || cfg.level != next_level)
		{
			return -1;
		}

		if (!GetSubNodeValue(data_element, "hun_score", cfg.hun_score) || cfg.hun_score <= 0)
		{
			return -2;
		}

		m_score_cfg_list.push_back(cfg);

		++next_level;
		data_element = data_element->NextSiblingElement();
	}

	return 0;
}

int SealConfig::InitStrengthCfg(TiXmlElement * RootElement)
{
	TiXmlElement *data_element = RootElement->FirstChildElement("data");
	if (NULL == data_element)
	{
		return -1000;
	}

	while (NULL != data_element)
	{
		SealStrengthCfg cfg;

		if (!GetSubNodeValue(data_element, "level", cfg.level) || cfg.level <= 0)
		{
			return -1;
		}

		if (!GetSubNodeValue(data_element, "gongji", cfg.gongji) || cfg.gongji < 0 || cfg.gongji > INT_MAX)
		{
			return -2;
		}

		if (!GetSubNodeValue(data_element, "pojia", cfg.pojia) || cfg.pojia < 0 || cfg.pojia > INT_MAX)
		{
			return -3;
		}

		if (!GetSubNodeValue(data_element, "yaolichuantou", cfg.yaolichuantou) || cfg.yaolichuantou < 0 || cfg.yaolichuantou > INT_MAX)
		{
			return -4;
		}

		if (!GetSubNodeValue(data_element, "yaolidikang", cfg.yaolidikang) || cfg.yaolidikang < 0 || cfg.yaolidikang > INT_MAX)
		{
			return -5;
		}

		if (!GetSubNodeValue(data_element, "molichuantou", cfg.molichuantou) || cfg.molichuantou < 0 || cfg.molichuantou > INT_MAX)
		{
			return -6;
		}

		if (!GetSubNodeValue(data_element, "molidikang", cfg.molidikang) || cfg.molidikang < 0 || cfg.molidikang > INT_MAX)
		{
			return -7;
		}

		if (!GetSubNodeValue(data_element, "shenlichuantou", cfg.shenlichuantou) || cfg.shenlichuantou < 0 || cfg.shenlichuantou > INT_MAX)
		{
			return -8;
		}

		if (!GetSubNodeValue(data_element, "shenlidikang", cfg.shenlidikang) || cfg.shenlidikang < 0 || cfg.shenlidikang > INT_MAX)
		{
			return -9;
		}

		if (!GetSubNodeValue(data_element, "maxhp", cfg.maxhp) || cfg.maxhp < 0 || cfg.maxhp > INT_MAX)
		{
			return -10;
		}

		if (!GetSubNodeValue(data_element, "fangyu", cfg.fangyu) || cfg.fangyu < 0 || cfg.fangyu > INT_MAX)
		{
			return -11;
		}

		if (!GetSubNodeValue(data_element, "per_baoji", cfg.per_baoji) || cfg.per_baoji < 0 || cfg.per_baoji > INT_MAX)
		{
			return -12;
		}

		if (!GetSubNodeValue(data_element, "baoji", cfg.baoji) || cfg.baoji < 0 || cfg.baoji > INT_MAX)
		{
			return -13;
		}

		if (!GetSubNodeValue(data_element, "per_baoji_jiacheng", cfg.per_baoji_jiacheng) || cfg.per_baoji_jiacheng < 0 || cfg.per_baoji_jiacheng > INT_MAX)
		{
			return -14;
		}

		if (!GetSubNodeValue(data_element, "per_maxhp", cfg.per_maxhp) || cfg.per_maxhp < 0 || cfg.per_maxhp > INT_MAX)
		{
			return -15;
		}

		if (!GetSubNodeValue(data_element, "per_fangyu", cfg.per_fangyu) || cfg.per_fangyu < 0 || cfg.per_fangyu > INT_MAX)
		{
			return -16;
		}

		if (!GetSubNodeValue(data_element, "per_gongji", cfg.per_gongji) || cfg.per_gongji < 0 || cfg.per_gongji > INT_MAX)
		{
			return -17;
		}

		if (!GetSubNodeValue(data_element, "per_pojia", cfg.per_pojia) || cfg.per_pojia < 0 || cfg.per_pojia > INT_MAX)
		{
			return -18;
		}

		if (!GetSubNodeValue(data_element, "per_shanghaijiacheng", cfg.per_shanghaijiacheng) || cfg.per_shanghaijiacheng < 0 || cfg.per_shanghaijiacheng > INT_MAX)
		{
			return -19;
		}

		if (!GetSubNodeValue(data_element, "per_shanghaijianshao", cfg.per_shanghaijianshao) || cfg.per_shanghaijianshao < 0 || cfg.per_shanghaijianshao > INT_MAX)
		{
			return -20;
		}

		m_strength_cfg_list.push_back(cfg);

		data_element = data_element->NextSiblingElement();
	}

	return 0;
}

int SealConfig::InitSuitCfg(TiXmlElement * RootElement)
{
	TiXmlElement *data_element = RootElement->FirstChildElement("data");
	if (NULL == data_element)
	{
		return -1000;
	}

	while (NULL != data_element)
	{
		SealSuitCfg cfg;

		if (!GetSubNodeValue(data_element, "suit_type", cfg.suit_type) || cfg.suit_type < SEAL_SUIT_MIN_TYPE || cfg.suit_type > SEAL_SUIT_MAX_TYPE)
		{
			return -1;
		}

		if (!GetSubNodeValue(data_element, "same_order_num", cfg.same_order_num) || cfg.same_order_num <= 0)
		{
			return -2;
		}

		if (!GetSubNodeValue(data_element, "gongji", cfg.gongji) || cfg.gongji < 0 || cfg.gongji > INT_MAX)
		{
			return -3;
		}

		if (!GetSubNodeValue(data_element, "pojia", cfg.pojia) || cfg.pojia < 0 || cfg.pojia > INT_MAX)
		{
			return -4;
		}

		if (!GetSubNodeValue(data_element, "yaolichuantou", cfg.yaolichuantou) || cfg.yaolichuantou < 0 || cfg.yaolichuantou > INT_MAX)
		{
			return -5;
		}

		if (!GetSubNodeValue(data_element, "yaolidikang", cfg.yaolidikang) || cfg.yaolidikang < 0 || cfg.yaolidikang > INT_MAX)
		{
			return -6;
		}

		if (!GetSubNodeValue(data_element, "molichuantou", cfg.molichuantou) || cfg.molichuantou < 0 || cfg.molichuantou > INT_MAX)
		{
			return -7;
		}

		if (!GetSubNodeValue(data_element, "molidikang", cfg.molidikang) || cfg.molidikang < 0 || cfg.molidikang > INT_MAX)
		{
			return -8;
		}

		if (!GetSubNodeValue(data_element, "shenlichuantou", cfg.shenlichuantou) || cfg.shenlichuantou < 0 || cfg.shenlichuantou > INT_MAX)
		{
			return -9;
		}

		if (!GetSubNodeValue(data_element, "shenlidikang", cfg.shenlidikang) || cfg.shenlidikang < 0 || cfg.shenlidikang > INT_MAX)
		{
			return -10;
		}

		if (!GetSubNodeValue(data_element, "maxhp", cfg.maxhp) || cfg.maxhp < 0 || cfg.maxhp > INT_MAX)
		{
			return -11;
		}

		if (!GetSubNodeValue(data_element, "fangyu", cfg.fangyu) || cfg.fangyu < 0 || cfg.fangyu > INT_MAX)
		{
			return -12;
		}

		if (!GetSubNodeValue(data_element, "per_baoji", cfg.per_baoji) || cfg.per_baoji < 0 || cfg.per_baoji > INT_MAX)
		{
			return -13;
		}

		if (!GetSubNodeValue(data_element, "baoji", cfg.baoji) || cfg.baoji < 0 || cfg.baoji > INT_MAX)
		{
			return -14;
		}

		if (!GetSubNodeValue(data_element, "per_baoji_jiacheng", cfg.per_baoji_jiacheng) || cfg.per_baoji_jiacheng < 0 || cfg.per_baoji_jiacheng > INT_MAX)
		{
			return -15;
		}

		if (!GetSubNodeValue(data_element, "per_maxhp", cfg.per_maxhp) || cfg.per_maxhp < 0 || cfg.per_maxhp > INT_MAX)
		{
			return -16;
		}

		if (!GetSubNodeValue(data_element, "per_fangyu", cfg.per_fangyu) || cfg.per_fangyu < 0 || cfg.per_fangyu > INT_MAX)
		{
			return -17;
		}

		if (!GetSubNodeValue(data_element, "per_gongji", cfg.per_gongji) || cfg.per_gongji < 0 || cfg.per_gongji > INT_MAX)
		{
			return -18;
		}

		if (!GetSubNodeValue(data_element, "per_pojia", cfg.per_jingzhun) || cfg.per_jingzhun < 0 || cfg.per_jingzhun > INT_MAX)
		{
			return -19;
		}

		if (!GetSubNodeValue(data_element, "per_shanghaijiacheng", cfg.per_pofang) || cfg.per_pofang < 0 || cfg.per_pofang > INT_MAX)
		{
			return -20;
		}

		if (!GetSubNodeValue(data_element, "per_shanghaijianshao", cfg.per_mianshang) || cfg.per_mianshang < 0 || cfg.per_mianshang > INT_MAX)
		{
			return -21;
		}

		if (!GetSubNodeValue(data_element, "per_base_attr_jiacheng", cfg.per_base_attr_jiacheng) || cfg.per_base_attr_jiacheng < 0 || cfg.per_base_attr_jiacheng > INT_MAX)
		{
			return -22;
		}

		if (!GetSubNodeValue(data_element, "per_strength_attr_jiacheng", cfg.per_strength_attr_jiacheng) || cfg.per_strength_attr_jiacheng < 0 || cfg.per_strength_attr_jiacheng > INT_MAX)
		{
			return -23;
		}

		if (!GetSubNodeValue(data_element, "mingzhong", cfg.mingzhong) || cfg.mingzhong < 0 || cfg.mingzhong > INT_MAX)
		{
			return -24;
		}

		if (!GetSubNodeValue(data_element, "shanbi", cfg.shanbi) || cfg.shanbi < 0 || cfg.shanbi > INT_MAX)
		{
			return -25;
		}

		if (!GetSubNodeValue(data_element, "jianren", cfg.jianren) || cfg.jianren < 0 || cfg.jianren > INT_MAX)
		{
			return -26;
		}

		if (!GetSubNodeValue(data_element, "skill_zengshang_per", cfg.skill_zengshang_per) || cfg.skill_zengshang_per < 0 || cfg.skill_zengshang_per > INT_MAX)
		{
			return -27;
		}

		long long key = ConvertParamToLongLong(cfg.suit_type, cfg.same_order_num);
		SealSuitCfgMapIt it = m_suit_cfg_map.find(key);
		if (it != m_suit_cfg_map.end())
		{
			return -100;
		}

		m_suit_cfg_map[key] = cfg;

		data_element = data_element->NextSiblingElement();
	}

	return 0;
}

int SealConfig::InitPartListCfg(TiXmlElement *RootElement)
{
	TiXmlElement *data_element = RootElement->FirstChildElement("data");
	if (NULL == data_element)
	{
		return -1000;
	}

	while (NULL != data_element)
	{
		SealSuitPartCfg cfg;

		if (!GetSubNodeValue(data_element, "suit_type", cfg.suit_type) || cfg.suit_type <= 0)
		{
			return -1;
		}

		if (!GetSubNodeValue(data_element, "equip_order", cfg.equip_order) || cfg.equip_order <= 0)
		{
			return -2;
		}

		if (!GetSubNodeValue(data_element, "equip_part", cfg.equip_part) || cfg.equip_part < 0)
		{
			return -3;
		}

		long long key = ConvertParamToLongLong(cfg.equip_order, cfg.equip_part);
		SealSuitPartCfgMapIt it = m_suit_part_cfg_map.find(key);
		if (it != m_suit_part_cfg_map.end())
		{
			return -100;
		}

		m_suit_part_cfg_map[key] = cfg;

		data_element = data_element->NextSiblingElement();
	}

	return 0;
}

int SealConfig::InitSoulCfg(TiXmlElement * RootElement)
{
	TiXmlElement *data_element = RootElement->FirstChildElement("data");
	if (NULL == data_element)
	{
		return -1000;
	}

	int next_level = 1;

	while (NULL != data_element)
	{
		int soul_type = -1;
		if (!GetSubNodeValue(data_element, "soul_type", soul_type) || soul_type < SEAL_SOUL_MIN_TYPE || soul_type > SEAL_SOUL_MAX_TYPE)
		{
			return -1;
		}

		SealSoulCfg &cfg = m_soul_cfg_list[soul_type];
		cfg.soul_type = soul_type;

		if (!GetSubNodeValue(data_element, "soul_id", cfg.soul_id) || NULL == ITEMPOOL->GetItem(cfg.soul_id))
		{
			return -2;
		}

		if (!GetSubNodeValue(data_element, "gongji", cfg.gongji) || cfg.gongji < 0 || cfg.gongji > INT_MAX)
		{
			return -3;
		}

		if (!GetSubNodeValue(data_element, "pojia", cfg.pojia) || cfg.pojia < 0 || cfg.pojia > INT_MAX)
		{
			return -4;
		}

		if (!GetSubNodeValue(data_element, "yaolichuantou", cfg.yaolichuantou) || cfg.yaolichuantou < 0 || cfg.yaolichuantou > INT_MAX)
		{
			return -5;
		}

		if (!GetSubNodeValue(data_element, "yaolidikang", cfg.yaolidikang) || cfg.yaolidikang < 0 || cfg.yaolidikang > INT_MAX)
		{
			return -6;
		}

		if (!GetSubNodeValue(data_element, "molichuantou", cfg.molichuantou) || cfg.molichuantou < 0 || cfg.molichuantou > INT_MAX)
		{
			return -7;
		}

		if (!GetSubNodeValue(data_element, "molidikang", cfg.molidikang) || cfg.molidikang < 0 || cfg.molidikang > INT_MAX)
		{
			return -8;
		}

		if (!GetSubNodeValue(data_element, "shenlichuantou", cfg.shenlichuantou) || cfg.shenlichuantou < 0 || cfg.shenlichuantou > INT_MAX)
		{
			return -9;
		}

		if (!GetSubNodeValue(data_element, "shenlidikang", cfg.shenlidikang) || cfg.shenlidikang < 0 || cfg.shenlidikang > INT_MAX)
		{
			return -10;
		}

		if (!GetSubNodeValue(data_element, "maxhp", cfg.maxhp) || cfg.maxhp < 0 || cfg.maxhp > INT_MAX)
		{
			return -11;
		}

		if (!GetSubNodeValue(data_element, "fangyu", cfg.fangyu) || cfg.fangyu < 0 || cfg.fangyu > INT_MAX)
		{
			return -12;
		}

		if (!GetSubNodeValue(data_element, "per_baoji", cfg.per_baoji) || cfg.per_baoji < 0 || cfg.per_baoji > INT_MAX)
		{
			return -13;
		}

		if (!GetSubNodeValue(data_element, "baoji", cfg.baoji) || cfg.baoji < 0 || cfg.baoji > INT_MAX)
		{
			return -14;
		}

		if (!GetSubNodeValue(data_element, "per_baoji_jiacheng", cfg.per_baoji_jiacheng) || cfg.per_baoji_jiacheng < 0 || cfg.per_baoji_jiacheng > INT_MAX)
		{
			return -15;
		}

		if (!GetSubNodeValue(data_element, "per_maxhp", cfg.per_maxhp) || cfg.per_maxhp < 0 || cfg.per_maxhp > INT_MAX)
		{
			return -16;
		}

		if (!GetSubNodeValue(data_element, "per_fangyu", cfg.per_fangyu) || cfg.per_fangyu < 0 || cfg.per_fangyu > INT_MAX)
		{
			return -17;
		}

		if (!GetSubNodeValue(data_element, "per_gongji", cfg.per_gongji) || cfg.per_gongji < 0 || cfg.per_gongji > INT_MAX)
		{
			return -18;
		}

		if (!GetSubNodeValue(data_element, "per_pojia", cfg.per_pojia) || cfg.per_pojia < 0 || cfg.per_pojia > INT_MAX)
		{
			return -19;
		}

		if (!GetSubNodeValue(data_element, "per_shanghaijiacheng", cfg.per_shanghaijiacheng) || cfg.per_shanghaijiacheng < 0 || cfg.per_shanghaijiacheng > INT_MAX)
		{
			return -20;
		}

		if (!GetSubNodeValue(data_element, "per_shanghaijianshao", cfg.per_shanghaijianshao) || cfg.per_shanghaijianshao < 0 || cfg.per_shanghaijianshao > INT_MAX)
		{
			return -21;
		}

		if (!GetSubNodeValue(data_element, "per_base_attr_jiacheng", cfg.per_base_attr_jiacheng) || cfg.per_base_attr_jiacheng < 0 || cfg.per_base_attr_jiacheng > INT_MAX)
		{
			return -22;
		}

		if (!GetSubNodeValue(data_element, "per_strength_attr_jiacheng", cfg.per_strength_attr_jiacheng) || cfg.per_strength_attr_jiacheng < 0 || cfg.per_strength_attr_jiacheng > INT_MAX)
		{
			return -23;
		}

		++next_level;
		data_element = data_element->NextSiblingElement();
	}

	return 0;
}

int SealConfig::InitSoulUseLimitCfg(TiXmlElement * RootElement)
{
	TiXmlElement *data_element = RootElement->FirstChildElement("data");
	if (NULL == data_element)
	{
		return -1000;
	}

	while (NULL != data_element)
	{
		SealSoulUseLimitCfg cfg;

		if (!GetSubNodeValue(data_element, "role_level", cfg.role_level) || cfg.role_level <= 0)
		{
			return -1;
		}

		if (!GetSubNodeValue(data_element, "use_limit_num", cfg.use_limit_num) || cfg.use_limit_num <= 0)
		{
			return -2;
		}

		m_soul_use_limit_cfg_list.push_back(cfg);
		data_element = data_element->NextSiblingElement();
	}

	return 0;
}
