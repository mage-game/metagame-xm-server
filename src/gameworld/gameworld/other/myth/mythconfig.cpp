#include "mythconfig.hpp"
#include "item/itempool.h"

MythConfig::MythConfig() : baoji_total_weight(0)
{
}

MythConfig::~MythConfig()
{
}

bool MythConfig::Init(const std::string &configname, std::string *err)
{
	PRE_LOAD_CONFIG;

	// ÆªÕÂÅäÖÃ
	LOAD_CONFIG("chapter", InitChpaterLevelConfig);

	// ¹²ÃùÅäÖÃ
	LOAD_CONFIG("resonance", InitResonanceConfig);
	
	// ¹²ÃùµÈ¼¶ÏÞÖÆ
	LOAD_CONFIG("resonance_open_limit", InitResonanceOpenLimitConfig);

	//// ·Ö½âÅäÖÃ
	//LOAD_CONFIG("decompose", InitDecomposeConfig);

	//// ÁìÎòÅäÖÃ
	//LOAD_CONFIG("digestion", InitDigestionConfig);

	//// ±©»÷ÅäÖÃ
	//LOAD_CONFIG("baoji", InitBaojiConfig);

	//// Éñ»êÅäÖÃ
	//LOAD_CONFIG("god_soul", InitSoulConfig);

	//// Ëæ»úÐÇÊýÅäÖÃ
	//LOAD_CONFIG("rand_attr_count", InitRandStarConfig);

	//// Ëæ»úÊôÐÔÅäÖÃ
	//LOAD_CONFIG("rand_attr", InitRandAttrConfig);

	//// ºÏ³É
	//LOAD_CONFIG("synthesis_exchange", InitComposeConfig);
	
	return true;
}

int MythConfig::InitChpaterLevelConfig(TiXmlElement *RootElement)
{
	TiXmlElement *data_element = RootElement->FirstChildElement("data");
	if (NULL == data_element)
	{
		return -1000;
	}

	int last_chpater_id = 0;
	int last_level = 0;
	while (NULL != data_element)
	{
		int chpater_id = 0;
		if (!GetSubNodeValue(data_element, "chpater_id", chpater_id) || chpater_id <= 0 || chpater_id > MAX_MYTH_CHAPTER_ID ||
			(last_chpater_id + 1 != chpater_id && last_chpater_id != chpater_id))
		{
			return -1;
		}

		MythChapterConfig &chpater_cfg = m_chpater_list[chpater_id];
		chpater_cfg.chapter_id = chpater_id;

		if (last_chpater_id != chpater_id && last_chpater_id != 0)
		{
			last_level = 0;
		}
		last_chpater_id = chpater_id;
		
		int level = 0;
		if (!GetSubNodeValue(data_element, "level", level) || level <= 0 || level > MAX_MYTH_CHPATER_LEVEL || level != last_level + 1)
		{
			return -2;
		}
		chpater_cfg.max_level = level;
		last_level = level;

		MythChapterLevelConfig &level_cfg = chpater_cfg.level_list[level];

		TiXmlElement *item_element = data_element->FirstChildElement("stuff_id1");
		if (NULL == item_element)
		{
			return -3;
		}

		if (!level_cfg.stuff_id1.ReadConfig(item_element))
		{
			return -4;
		}

		//TiXmlElement *item_element2 = data_element->FirstChildElement("stuff_id2");
		//if (NULL == item_element2)
		//{
		//	return -5;
		//}

		//if (!level_cfg.stuff_id2.ReadConfig(item_element2))
		//{
		//	return -6;
		//}

		int is_broadcast = 0;
		if (!GetSubNodeValue(data_element, "is_broadcast", is_broadcast))
		{
			return -7;
		}
		level_cfg.is_broadcast = (0 != is_broadcast);
		
		if (!GetSubNodeValue(data_element, "maxhp", level_cfg.maxhp) || level_cfg.maxhp < 0)
		{
			return -101;
		}

		if (!GetSubNodeValue(data_element, "gongji", level_cfg.gongji) || level_cfg.gongji < 0)
		{
			return -102;
		}

		if (!GetSubNodeValue(data_element, "fangyu", level_cfg.fangyu) || level_cfg.fangyu < 0)
		{
			return -103;
		}

		if (!GetSubNodeValue(data_element, "mingzhong", level_cfg.mingzhong) || level_cfg.mingzhong < 0)
		{
			return -104;
		}

		if (!GetSubNodeValue(data_element, "shanbi", level_cfg.shanbi) || level_cfg.shanbi < 0)
		{
			return -105;
		}

		if (!GetSubNodeValue(data_element, "baoji", level_cfg.baoji) || level_cfg.baoji < 0)
		{
			return -106;
		}

		if (!GetSubNodeValue(data_element, "jianren", level_cfg.jianren) || level_cfg.jianren < 0)
		{
			return -107;
		}

		if (!GetSubNodeValue(data_element, "per_pofang", level_cfg.pofang_per) || level_cfg.pofang_per < 0)
		{
			return -108;
		}

		if (!GetSubNodeValue(data_element, "per_mianshang", level_cfg.mianshang_per) || level_cfg.mianshang_per < 0)
		{
			return -109;
		}

		data_element = data_element->NextSiblingElement();
	}

	return 0;
}

int MythConfig::InitResonanceConfig(TiXmlElement *RootElement)
{
	TiXmlElement *data_element = RootElement->FirstChildElement("data");
	if (NULL == data_element)
	{
		return -1000;
	}

	int last_chpater_id = 0;
	int last_level = 0;
	while (NULL != data_element)
	{
		int chpater_id = 0;
		if (!GetSubNodeValue(data_element, "chpater", chpater_id) || chpater_id <= 0 || chpater_id > MAX_MYTH_CHAPTER_ID ||
			(last_chpater_id + 1 != chpater_id && last_chpater_id != chpater_id))
		{
			return -1;
		}

		MythResonanceConfig &chpater_cfg = m_resonance_list[chpater_id];
		chpater_cfg.chpater_id = chpater_id;

		if (last_chpater_id != chpater_id && last_chpater_id != 0)
		{
			last_level = 0;
		}
		last_chpater_id = chpater_id;

		int level = 0;
		if (!GetSubNodeValue(data_element, "level", level) || level <= 0 || level > MAX_MYTH_CHPATER_LEVEL || level != last_level + 1)
		{
			return -2;
		}
		chpater_cfg.max_level = level;
		last_level = level;

		MythResonanceLevelConfig &level_cfg = chpater_cfg.level_list[level];
		level_cfg.level = level;

		TiXmlElement *item_element = data_element->FirstChildElement("stuff_id");
		if (NULL == item_element)
		{
			return -3;
		}

		if (!level_cfg.stuff.ReadConfig(item_element))
		{
			return -4;
		}

		for (int index = 0; index < 3; ++index)
		{
			int &soul_type = level_cfg.position_soul_list[index];

			char name[30] = {};
			SNPRINTF(name, sizeof(name), "position_%d", index + 1);
			if (!GetSubNodeValue(data_element, name, soul_type) || soul_type < 0)
			{
				return -10000 - index;
			}
		}

		for (int index = 1; index <= 6; ++index)
		{
			int &weight = level_cfg.soul_weight_list[index];

			char name[30] = {};
			SNPRINTF(name, sizeof(name), "soul_weight_%d", index);
			if (!GetSubNodeValue(data_element, name, weight) || weight < 0)
			{
				return -1000 - index;
			}

			level_cfg.weight_total += weight;
		}

		if (level_cfg.weight_total <= 0)
		{
			return -5;
		}

		for (int index = 0; index < MAX_MYTH_RESONANCE_ATTR_NUM; ++index)
		{
			MythResonanceLevelConfig::ResonanceItem &resonace_cfg = level_cfg.resonance_attr_list[index];

			char name[30] = {};
			SNPRINTF(name, sizeof(name), "resonance_type%d", index + 1);
			if (!GetSubNodeValue(data_element, name, resonace_cfg.attr_type) || resonace_cfg.attr_type < 0)
			{
				return -1000 - index;
			}

			char name2[30] = {};
			SNPRINTF(name2, sizeof(name2), "resonance_val%d", index + 1);
			if (!GetSubNodeValue(data_element, name2, resonace_cfg.attr_val) || resonace_cfg.attr_val < 0)
			{
				return -2000 - index;
			}
		}

		data_element = data_element->NextSiblingElement();
	}

	return 0;
}

int MythConfig::InitDecomposeConfig(TiXmlElement *RootElement)
{
	TiXmlElement *data_element = RootElement->FirstChildElement("data");
	if (NULL == data_element)
	{
		return -1000;
	}

	while (NULL != data_element)
	{	
		ItemID item_id = 0;
		if (!GetSubNodeValue(data_element, "item_id", item_id) || m_decompose_map.find(item_id) != m_decompose_map.end() || NULL == ITEMPOOL->GetItem(item_id))
		{
			return -1;
		}

		int gain_soul_essence = 0;
		if (!GetSubNodeValue(data_element, "gain_soul_essence", gain_soul_essence) || gain_soul_essence <= 0)
		{
			return -2;
		}

		m_decompose_map[item_id] = gain_soul_essence;
		data_element = data_element->NextSiblingElement();
	}

	return 0;
}

int MythConfig::InitDigestionConfig(TiXmlElement *RootElement)
{
	TiXmlElement *data_element = RootElement->FirstChildElement("data");
	if (NULL == data_element)
	{
		return -1000;
	}

	int last_chpater_id = 0;
	int last_level = -1;
	while (NULL != data_element)
	{
		int chpater_id = 0;
		if (!GetSubNodeValue(data_element, "chpater_id", chpater_id) || chpater_id <= 0 || chpater_id > MAX_MYTH_CHAPTER_ID ||
			(last_chpater_id + 1 != chpater_id && last_chpater_id != chpater_id))
		{
			return -1;
		}

		MythDigestionConfig &chpater_cfg = m_digestion_list[chpater_id];
		chpater_cfg.chpater_id = chpater_id;

		if (last_chpater_id != chpater_id && last_chpater_id != 0)
		{
			last_level = -1;
		}
		last_chpater_id = chpater_id;

		int level = 0;
		if (!GetSubNodeValue(data_element, "digestion_level", level) || level < 0 || level > MAX_MYTH_DIGESTION_LEVEL || level != last_level + 1)
		{
			return -2;
		}
		last_level = level;
		chpater_cfg.max_level = level;

		DigestionLevelConfig &level_cfg = chpater_cfg.level_list[level];
		
		if (!GetSubNodeValue(data_element, "single_essence", level_cfg.single_essence) ||  level_cfg.single_essence <= 0)
		{
			return -3;
		}

		if (!GetSubNodeValue(data_element, "need_essence", level_cfg.need_essence) || level_cfg.need_essence <= 0)
		{
			return -4;
		}

		int ret = level_cfg.attr.ReadConfig(data_element);
		if (ret < 0)
		{
			return -5;
		}

		data_element = data_element->NextSiblingElement();
	}

	return 0;
}

int MythConfig::InitBaojiConfig(TiXmlElement *RootElement)
{
	TiXmlElement *data_element = RootElement->FirstChildElement("data");
	if (NULL == data_element)
	{
		return -1000;
	}

	baoji_total_weight = 0;
	while (NULL != data_element)
	{
		MythBaoJIConfig temp_cfg;
		if (!GetSubNodeValue(data_element, "baoji_multiple", temp_cfg.baoji_multiple) || temp_cfg.baoji_multiple < 0 || temp_cfg.baoji_multiple > MAX_MYTH_BAOJI_MULTIPLE)
		{
			return -1;
		}

		if (!GetSubNodeValue(data_element, "baoji_weight", temp_cfg.baoji_weight) || temp_cfg.baoji_weight < 0 || temp_cfg.baoji_weight > 10000)
		{
			return -2;
		}
		baoji_total_weight += temp_cfg.baoji_weight;

		m_baoji_vec.push_back(temp_cfg);
		data_element = data_element->NextSiblingElement();
	}

	if (baoji_total_weight <= 0)
	{
		return -3;
	}
	
	return 0;
}

int MythConfig::InitSoulConfig(TiXmlElement *RootElement)
{
	TiXmlElement *data_element = RootElement->FirstChildElement("data");
	if (NULL == data_element)
	{
		return -10000;
	}

	while (NULL != data_element)
	{
		ItemID item_id = 0;
		if (!GetSubNodeValue(data_element, "item_id", item_id) || NULL == ITEMPOOL->GetItem(item_id))
		{
			return -1;
		}

		if (m_soul_god_map.find(item_id) != m_soul_god_map.end())
		{
			return -2;
		}

		MythSoulGodConfig &temp_cfg = m_soul_god_map[item_id];

		if (!GetSubNodeValue(data_element, "soul_type", temp_cfg.soul_type) || temp_cfg.soul_type <= 0 || temp_cfg.soul_type > MAX_MYTH_SOUL_SLOT)
		{
			return -3;
		}

		if (!GetSubNodeValue(data_element, "quality", temp_cfg.quality) || temp_cfg.quality <= 0 || temp_cfg.soul_type > MAX_MYTH_SOUL_QUALITY)
		{
			return -4;
		}

		int ret = temp_cfg.attr.ReadConfig(data_element);
		if (ret < 0)
		{
			return -4;
		}

		data_element = data_element->NextSiblingElement();
	}

	return 0;
}

int MythConfig::InitRandStarConfig(TiXmlElement *RootElement)
{
	TiXmlElement* data_element = RootElement->FirstChildElement("data");
	if (NULL == data_element)
	{
		return -1000;
	}

	int last_quality = 0;
	while (NULL != data_element)
	{
		int quality = 0;
		if (!GetSubNodeValue(data_element, "quality", quality) || quality != last_quality + 1 || quality <= 0 || quality > MAX_MYTH_SOUL_QUALITY)
		{
			return -1;
		}
		last_quality = quality;

		RandStarConfig &cfg = m_soul_star_cfg_list[quality];
		
		if (!GetSubNodeValue(data_element, "rand_attr_num", cfg.rand_attr_num) || cfg.rand_attr_num <= 0)
		{
			return -2;
		}

		for (int index = 0; index <= 3; ++index)
		{
			int &weight = cfg.star_weight_list[index];

			char name[30] = {};
			SNPRINTF(name, sizeof(name), "weight_%d", index);
			if (!GetSubNodeValue(data_element, name, weight) || weight < 0)
			{
				return -1000 - index;
			}

			cfg.weight_total += weight;
		}

		if (cfg.weight_total <= 0)
		{
			return -4;
		}

		data_element = data_element->NextSiblingElement();
	}

	return 0;
}

int MythConfig::InitRandAttrConfig(TiXmlElement *RootElement)
{
	TiXmlElement *data_element = RootElement->FirstChildElement("data");
	if (NULL == data_element)
	{
		return -1000;
	}

	while (NULL != data_element)
	{
		int quality = 0;
		if (!GetSubNodeValue(data_element, "quality", quality) || quality <= 0 || quality > MAX_MYTH_SOUL_QUALITY)
		{
			return -1;
		}

		SoulAttrConfig &quality_cfg = m_soul_attr_list[quality];
		quality_cfg.quality = quality;

		int is_star_attr = 0;
		if (!GetSubNodeValue(data_element, "is_star_attr", is_star_attr) || (is_star_attr != 0 && is_star_attr != 1))
		{
			return -2;
		}

		SoulRandAttrConfig *attr_cfg;
		if (0 == is_star_attr) // ÆÕÍ¨ÊôÐÔ
		{
			if (quality_cfg.normal_count >= MAX_MYTH_RAND_ATTR_NORMAL_COUNT)
			{
				return -3;
			}

			attr_cfg = &quality_cfg.normal_list[quality_cfg.normal_count++];
		}
		else // ÕäÏ¡ÊôÐÔ
		{
			if (quality_cfg.rare_count >= MAX_MYTH_RAND_ATTR_RARE_COUNT)
			{
				return -4;
			}

			attr_cfg = &quality_cfg.rare_list[quality_cfg.rare_count++];
		}

		if (!GetSubNodeValue(data_element, "attr_type", attr_cfg->attr_type) || attr_cfg->attr_type <= MYTH_ATTR_TYPE_INVALID_MIN || attr_cfg->attr_type >= MYTH_ATTR_TYPE_INVALID_MAX)
		{
			return -5;
		}

		if (!GetSubNodeValue(data_element, "attr_value", attr_cfg->attr_val) || attr_cfg->attr_val < 0)
		{
			return -6;
		}

		data_element = data_element->NextSiblingElement();
	}

	return 0;
}

int MythConfig::InitComposeConfig(TiXmlElement *RootElement)
{
	TiXmlElement *data_element = RootElement->FirstChildElement("data");
	if (NULL == data_element)
	{
		return -1000;
	}

	int last_seq = -1;
	while (NULL != data_element)
	{
		int seq = 0;
		if (!GetSubNodeValue(data_element, "seq", seq) || seq != last_seq + 1)
		{
			return -100000;
		}
		last_seq = seq;

		MythComposeCfg &cfg = m_compose_map[seq];
		if (!GetSubNodeValue(data_element, "before_item_id", cfg.before_item_id))
		{
			return -100001;
		}

		if (!GetSubNodeValue(data_element, "after_item_id", cfg.after_item_id))
		{
			return -100002;
		}

		if (!GetSubNodeValue(data_element, "need_start_num", cfg.need_start_num) || cfg.need_start_num < 0)
		{
			return -100003;
		}

		if (!GetSubNodeValue(data_element, "give_start_num", cfg.give_start_num) || cfg.give_start_num < 0)
		{
			return -100004;
		}

		int is_need_item = 0;
		if (!GetSubNodeValue(data_element, "is_need_item", is_need_item) || is_need_item < 0)
		{
			return -100005;
		}
		cfg.is_need_item = is_need_item != 0;

		int is_broatcast = 0;
		if (!GetSubNodeValue(data_element, "is_broadcast", is_broatcast) || is_broatcast < 0)
		{
			return -100006;
		}
		cfg.is_broatcast = is_broatcast != 0;

		if (cfg.is_need_item)
		{
			if (!GetSubNodeValue(data_element, "item_id", cfg.after_item_id) || !ITEMPOOL->GetItem(cfg.after_item_id))
			{
				return -100007;
			}

			if (!GetSubNodeValue(data_element, "item_num", cfg.item_num) || cfg.item_num <= 0)
			{
				return -100008;
			}
		}

		int is_broadcast = 0;
		if (!GetSubNodeValue(data_element, "is_broadcast", is_broadcast))
		{
			return -100009;
		}

		cfg.is_broadcast = (0 != is_broadcast);

		data_element = data_element->NextSiblingElement();
	}

	return 0;
}

int MythConfig::InitResonanceOpenLimitConfig(TiXmlElement *RootElement)
{
	TiXmlElement *data_element = RootElement->FirstChildElement("data");
	if (NULL == data_element)
	{
		return -1000;
	}

	int last_chpater_id = 0;
	int last_resonance_open_limit_level = 0;
	while (NULL != data_element)
	{
		int chpater_id = 0;
		if (!GetSubNodeValue(data_element, "chpater_id", chpater_id) || chpater_id < 0 || chpater_id > MAX_MYTH_CHAPTER_ID ||
			(last_chpater_id + 1 != chpater_id && last_chpater_id != chpater_id))
		{
			return -1;
		}

		if (last_chpater_id != chpater_id && last_chpater_id != 0)
		{
			last_resonance_open_limit_level = 0;
		}
		last_chpater_id = chpater_id;

		MythResonanceOpenLimitCfg &resonance_open_limit_cfg = m_resonance_open_limit_list[chpater_id];
		resonance_open_limit_cfg.chpater_id = chpater_id;

		int resonance_open_limit_level = 0;
		if (!GetSubNodeValue(data_element, "level", resonance_open_limit_level) 
			|| (last_resonance_open_limit_level + 1 != resonance_open_limit_level)
			|| (resonance_open_limit_level <= 0) 
			|| (resonance_open_limit_level > MAX_MYTH_RESONANCE_LEVEL))			
		{
			return -2;
		}
		last_resonance_open_limit_level = resonance_open_limit_level;

		MythResonanceOpenLimitItem &level_open_limit_cfg = resonance_open_limit_cfg.level_open_limit[resonance_open_limit_level];
		level_open_limit_cfg.resonance_level = resonance_open_limit_level;

		if (!GetSubNodeValue(data_element, "grid_level", level_open_limit_cfg.chpater_level)
			|| (level_open_limit_cfg.chpater_level <= 0)
			|| (level_open_limit_cfg.chpater_level > MAX_MYTH_CHPATER_LEVEL))
		{
			return -3;
		}

		data_element = data_element->NextSiblingElement();
	}

	return 0;
}

const MythComposeCfg * MythConfig::GetComposeCfg(int seq)
{
	std::map<int, MythComposeCfg>::iterator it = m_compose_map.find(seq);
	if (it != m_compose_map.end())
	{
		return &it->second;
	}

	return NULL;
}

int MythConfig::GetDecomposeSoulEssence(ItemID item_id)
{
	std::map<ItemID, int>::iterator it = m_decompose_map.find(item_id);
	if (it == m_decompose_map.end())
	{
		return -1;
	}

	return it->second;
}

int MythConfig::GetBaoJiMultiple()
{
	if (0 == baoji_total_weight)
	{
		return 1;
	}
	int rand_weight = rand() % baoji_total_weight;

	std::vector<MythBaoJIConfig>::iterator it = m_baoji_vec.begin();
	for (; it != m_baoji_vec.end(); it ++)
	{
		if (it->baoji_weight >= rand_weight)
		{
			return it->baoji_multiple;
		}

		rand_weight -= it->baoji_weight;
	}

	return 1;
}

const MythChapterLevelConfig * MythConfig::GetChpaterLevelCfg(int chpater_id, int level)
{	
	if (chpater_id <= 0 || chpater_id > MAX_MYTH_CHAPTER_ID || level <= 0 || level > MAX_MYTH_CHPATER_LEVEL)
	{
		return NULL;
	}

	if (level > m_chpater_list[chpater_id].max_level)
	{
		return NULL;
	}

	return &m_chpater_list[chpater_id].level_list[level];
}

const MythResonanceLevelConfig * MythConfig::GetResonanceCfg(int chpater_id, int level)
{
	if (chpater_id <= 0 || chpater_id > MAX_MYTH_CHAPTER_ID || level <= 0 || level > MAX_MYTH_RESONANCE_LEVEL)
	{     
		return NULL;
	}

	if (level > m_resonance_list[chpater_id].max_level)
	{
		return NULL;
	}

	return &m_resonance_list[chpater_id].level_list[level];
}

const DigestionLevelConfig * MythConfig::GetDigestionCfg(int chpater_id, int level)
{
	if (chpater_id <= 0 || chpater_id > MAX_MYTH_CHAPTER_ID || level < 0 || level > MAX_MYTH_DIGESTION_LEVEL)
	{
		return NULL;
	}

	if (level > m_digestion_list[chpater_id].max_level)
	{
		return NULL;
	}

	return &m_digestion_list[chpater_id].level_list[level];
}

const MythSoulGodConfig * MythConfig::GetSoulGodCfgByItem(ItemID item_id)
{
	std::map<ItemID, MythSoulGodConfig>::iterator it = m_soul_god_map.find(item_id);
	if (it != m_soul_god_map.end())
	{
		return &it->second;
	}

	return NULL;
}

const SoulAttrConfig *MythConfig::GetSoulAttrCfg(int quality)
{
	if (quality <= 0 || quality > MAX_MYTH_SOUL_QUALITY)
	{
		return NULL;
	}

	return &m_soul_attr_list[quality];
}

const RandStarConfig *MythConfig::GetStarCfg(int quality)
{
	if (quality <= 0 || quality > MAX_MYTH_SOUL_QUALITY)
	{
		return NULL;
	}

	return &m_soul_star_cfg_list[quality];
}

const SoulRandAttrConfig* MythConfig::GetRandomStarAttr(int quality, std::set<int> &attr_list)
{
	if (quality <= 0 || quality > MAX_MYTH_SOUL_QUALITY)
	{
		return NULL;
	}

	std::vector<int> tmp_atrr_set;
	for (int i = 0; i < m_soul_attr_list[quality].rare_count && i < MAX_MYTH_SOUL_RAND_ATTR_COUNT; ++i)
	{
		if (attr_list.find(m_soul_attr_list[quality].rare_list[i].attr_type) != attr_list.end())
		{
			continue;
		}

		tmp_atrr_set.push_back(i);
	}

	if (tmp_atrr_set.size() <= 0)
	{
		return NULL;
	}

	int ra_index = RandomNum(static_cast<int>(tmp_atrr_set.size()));
	return &m_soul_attr_list[quality].rare_list[tmp_atrr_set[ra_index]];
}

const SoulRandAttrConfig* MythConfig::GetRandomNormalAttr(int quality, std::set<int> &attr_list)
{
	if (quality <= 0 || quality > MAX_MYTH_SOUL_QUALITY)
	{
		return NULL;
	}

	std::vector<int> tmp_atrr_set;
	for (int i = 0; i < m_soul_attr_list[quality].normal_count && i < MAX_MYTH_SOUL_RAND_ATTR_COUNT; ++i)
	{
		if (attr_list.find(m_soul_attr_list[quality].normal_list[i].attr_type) != attr_list.end())
		{
			continue;
		}

		tmp_atrr_set.push_back(i);
	}

	if (tmp_atrr_set.size() <= 0)
	{
		return NULL;
	}

	int ra_index = RandomNum(static_cast<int>(tmp_atrr_set.size()));
	return &m_soul_attr_list[quality].normal_list[tmp_atrr_set[ra_index]];
}

bool MythConfig::CheckAttrStarCount(const MythSoulGodItem &tmp_item, int star_count, int quality)
{
	if (star_count <= 0 || quality <= 0 || quality > MAX_MYTH_SOUL_QUALITY) return false;

	const SoulAttrConfig &temp = m_soul_attr_list[quality];

	int count = 0;
	for (int i = 0; i < MAX_MYTH_STAR_SOUL; i++)
	{
		for (int j = 0; j < temp.rare_count && j < MAX_MYTH_RAND_ATTR_RARE_COUNT; j ++)
		{
			if (temp.rare_list[j].attr_type == tmp_item.attr_list[i].attr_type)
			{
				count++;
				break;
			}
		}
	}

	return (count == star_count);
}

void MythConfig::GetRandStarCount(int quality, int &attr_count, int &star_count)
{
	if (quality <= 0 || quality > MAX_MYTH_SOUL_QUALITY)
	{
		return;
	}

	const RandStarConfig &tmp_cfg = m_soul_star_cfg_list[quality];
	if (0 == tmp_cfg.weight_total)
	{
		return;
	}
	int rand_weight = rand() % tmp_cfg.weight_total;
	for (int i = 0; i <= MAX_MYTH_STAR_SOUL; i++)
	{
		if (rand_weight <= tmp_cfg.star_weight_list[i])
		{
			star_count = i;
			break;
		}

		rand_weight -= tmp_cfg.star_weight_list[i];
	};

	attr_count = tmp_cfg.rand_attr_num;
}

int MythConfig::GetAttrStarCount(const MythSoulGodItem &tmp_item, int quality)
{
	if (quality <= 0 || quality > MAX_MYTH_SOUL_QUALITY) return 0;

	const SoulAttrConfig &temp = m_soul_attr_list[quality];

	int count = 0;
	for (int i = 0; i < MAX_MYTH_STAR_SOUL; i++)
	{
		for (int j = 0; j < temp.rare_count && j < MAX_MYTH_RAND_ATTR_RARE_COUNT; j++)
		{
			if (temp.rare_list[j].attr_type == tmp_item.attr_list[i].attr_type)
			{
				count++;
				break;
			}
		}
	}

	return count;
}

const MythResonanceOpenLimitItem * MythConfig::GetResonanceOpenLimitCfg(int chpater_id, int resonance_level)
{
	if (chpater_id <= 0 || chpater_id > MAX_MYTH_CHAPTER_ID || resonance_level <= 0 || resonance_level > MAX_MYTH_RESONANCE_LEVEL)
	{
		return NULL;
	}

	return &m_resonance_open_limit_list[chpater_id].level_open_limit[resonance_level];
}