#include "shengesystemconfig.hpp"
#include "servercommon/configcommon.h"
#include "obj/character/attribute.hpp"
#include "item/itempool.h"

ShengeSystemCfg::ShengeSystemCfg() : m_attr_cfg_count(0), m_compose_cfg_count(0), m_choujiang_cfg_count(0), m_choujiang_cfg_total_weight(0), m_choujiang_cfg_total_ten_weight(0),
m_choujiang_cfg_total_ten_special_weight(0),m_first_ten_chou_cfg_count(0), m_first_ten_chou_total_weight(0), m_combination_cfg_count(0), m_item_id_Shenge_cfg_count(0),
m_drop_id_list_count(0), m_drop_id_total_weight(0), m_zhangkong_rand_exp_all_weight(0), m_zhangkong_rand_grade_all_weight(0)
{
	memset(m_suit_list, 0, sizeof(m_suit_list));
}

ShengeSystemCfg::~ShengeSystemCfg()
{
}

bool ShengeSystemCfg::Init(std::string configname, std::string *err)
{
	PUGI_XML_CONFIG_PRE_LOAD;

	// 其他配置
	PUGI_XML_LOAD_CONFIG("other", InitOtherCfg);
	// 神格属性配置
	PUGI_XML_LOAD_CONFIG("attributescfg", InitShengeCfg);
	// 神格分解配置
	PUGI_XML_LOAD_CONFIG("decomposecfg", InitDecomposeCfg);
	// 抽奖配置
	PUGI_XML_LOAD_CONFIG("choujiangcfg", InitChouJiangCfg);
	// 首抽配置
	PUGI_XML_LOAD_CONFIG("first_ten_chou", InitFirstTenChouJiangCfg);
	// 组合配置
	PUGI_XML_LOAD_CONFIG("combination", InitCombinationCfg);
	// 神格实物id对照配置
	PUGI_XML_LOAD_CONFIG("item_id_to_shenge", InitItemIdShengeCfg);
	// 额外掉落配置
	PUGI_XML_LOAD_CONFIG("drop_id_cfg", InitDropIdCfg);
	// 神格等级传闻配置
	PUGI_XML_LOAD_CONFIG("shenge_level_broadcast", InitBroadcastCfg);
	// 神格页开放配置
	PUGI_XML_LOAD_CONFIG("shengepage", InitShengePageCfg);
	// 掌控
	PUGI_XML_LOAD_CONFIG("zhangkong", InitZhangkongLevelCfg);
	// 掌控随机经验
	PUGI_XML_LOAD_CONFIG("zhangkong_rand_exp_weight", InitZhangkongRandExpCfg);
	// 掌控随机格子
	PUGI_XML_LOAD_CONFIG("zhangkong_rand_grade_weight", InitZhangkongRandGridCfg);
	// 神格神躯配置
	PUGI_XML_LOAD_CONFIG("shenqu", InitShengeShenquCfg);
	// 神格神躯洗炼配置
	PUGI_XML_LOAD_CONFIG("shenqu_xilian", InitShengeShenquXilianCfg);
	// 格子升级
	PUGI_XML_LOAD_CONFIG("uplevel_grid", InitUplevelGridCfg);

	PUGI_XML_LOAD_CONFIG("new_suit", InitNewSuitCfg);
	
	return true;
}

bool ShengeSystemCfg:: CheckSeq(int seq)
{
	if (seq < 0 || seq >= this->GetChoujiangCfgCount() || seq >= SHENGE_SYSTEM_CHOUJIANG_MAX_CFG_COUNTS)
	{
		return false;
	}
	return true;
}

const ShengeSystemAttrLevelCfg * ShengeSystemCfg::GetShengeSystemCfgByQualityTypeLevel(int quality, int type, int level) const
{
	if (quality < SHENGE_QUALITY_BLUE || quality >= SHENGE_QUALITY_COUNT || m_attr_cfg_list[quality].quality != quality)
	{
		return NULL;
	}
	const ShengeSystemAttrCfg &cfg = m_attr_cfg_list[quality];

	if (type < SHENGE_TYPE_HP || type >= SHENGE_TYPE_COUNT || cfg.type_cfg_list[type].type != type)
	{
		return NULL;
	}
	const ShengeSystemAttrtypeCfg &type_cfg = cfg.type_cfg_list[type];

	if (level <= 0 || level >= SHENGE_SYSTEM_MAX_SHENGE_LEVEL || type_cfg.leve_cfg[level].level != level)
	{
		return NULL;
	}

	return &type_cfg.leve_cfg[level];
}

int ShengeSystemCfg::GetShengeKind(int quality, int type, int level)
{
	if (quality < SHENGE_QUALITY_BLUE || quality >= SHENGE_QUALITY_COUNT || m_attr_cfg_list[quality].quality != quality)
	{
		return -1;
	}
	ShengeSystemAttrCfg &cfg = m_attr_cfg_list[quality];

	if (type < SHENGE_TYPE_HP || type >= SHENGE_TYPE_COUNT || cfg.type_cfg_list[type].type != type)
	{
		return -1;
	}
	ShengeSystemAttrtypeCfg &type_cfg = cfg.type_cfg_list[type];

	if (level <= 0 || level >= SHENGE_SYSTEM_MAX_SHENGE_LEVEL || type_cfg.leve_cfg[level].level != level)
	{
		return -1;
	}

	return type_cfg.leve_cfg[level].kind;
}

const ShengeSystemDecomposeQualityCfg * ShengeSystemCfg::GetcomposeCfgByKindAndQuality(int kind, int quality)
{
	if (kind < 0 || kind > m_compose_cfg_count || m_compose_cfg_list[kind].kind != kind)
	{
		return NULL;
	}
	ShengeSystemComposeCfg &cfg = m_compose_cfg_list[kind];

	if (quality < SHENGE_QUALITY_BLUE || quality >= SHENGE_QUALITY_COUNT || cfg.quality_cfg_list[quality].quality != quality)
	{
		return NULL;
	}

	return &cfg.quality_cfg_list[quality];
}

const ShengeSystemChoujiagnCfg * ShengeSystemCfg::GetChoujiangCfgBySeq(int seq) const
{
	if (seq < 0 || seq >= m_choujiang_cfg_count || m_choujiang_cfg_list[seq].seq != seq)
	{
		return NULL;
	}

	return &m_choujiang_cfg_list[seq];
}

const ShengeSystemChoujiagnCfg * ShengeSystemCfg::GetChoujiangCfgByQualityAndType(short quality, short type) const
{
	if (quality < SHENGE_QUALITY_BLUE || quality >= SHENGE_QUALITY_COUNT)
	{
		return NULL;
	}

	if (type < SHENGE_TYPE_HP || type >= SHENGE_TYPE_COUNT)
	{
		return NULL;
	}

	for (int i = 0; i < m_choujiang_cfg_count; i++)
	{
		if (m_choujiang_cfg_list[i].reward_shenge.quality == quality && m_choujiang_cfg_list[i].reward_shenge.type == type)
		{
			return &m_choujiang_cfg_list[i];
		}
	}

	return NULL;
}

const ShengeSystemChoujiagnCfg * ShengeSystemCfg::GetFirstTenChouJiangCfgBySeq(int seq) const
{
	if (seq < 0 || seq >= m_first_ten_chou_cfg_count || m_first_ten_chou_cfg_list[seq].seq != seq)
	{
		return NULL;
	}

	return &m_first_ten_chou_cfg_list[seq];
}

const ShengeSystemCombinationCfg * ShengeSystemCfg::GetCombinationCfgBySeq(int seq) const
{
	if (seq < 0 || seq >= m_combination_cfg_count || m_combination_cfg_list[seq].seq != seq)
	{
		return NULL;
	}

	return &m_combination_cfg_list[seq];
}

const ShengeSystemItemIdShengeCfg * ShengeSystemCfg::GetItemIdShengeCfgByItemId(ItemID item_id) const
{
	for (int index = 0; index < m_item_id_Shenge_cfg_count; ++index)
	{
		if (m_item_id_Shenge_cfg_list[index].item_id == item_id)
		{
			return &m_item_id_Shenge_cfg_list[index];
		}
	}

	return NULL;
}

const ShengeSystemDropIdCfg * ShengeSystemCfg::GetDropIdCfgBySeq(int seq) const
{
	if (seq < 0 || seq >= m_drop_id_list_count || m_drop_id_list[seq].seq != seq)
	{
		return NULL;
	}

	return &m_drop_id_list[seq];
}

const ShengeSystemPageCfg ShengeSystemCfg::GetShengePageCfg(int level)
{
	ShengeSystemPageCfg cfg;

	std::map<int ,ShengeSystemPageCfg>::iterator it = m_page_map.find(level);
	if (it != m_page_map.end())
	{
		return it->second;
	}

	return cfg;
}

int ShengeSystemCfg::InitOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1000;
	}

	if (!PugiGetSubNodeValue(data_element, "one_chou_need_gold", m_other_cfg.one_chou_need_gold) || m_other_cfg.one_chou_need_gold <= 0)
	{
		return -1001;
	}

	if (!PugiGetSubNodeValue(data_element, "ten_chou_need_gold", m_other_cfg.ten_chou_need_gold) || m_other_cfg.ten_chou_need_gold <= m_other_cfg.one_chou_need_gold)
	{
		return -1002;
	}

	if (!PugiGetSubNodeValue(data_element, "free_choujiang_times", m_other_cfg.free_choujiang_times) || m_other_cfg.free_choujiang_times < 0)
	{
		return -1003;
	}

	if (!PugiGetSubNodeValue(data_element, "left_free_chou_cd_s", m_other_cfg.left_free_chou_cd_s) || m_other_cfg.left_free_chou_cd_s <= 0)
	{
		return -1004;
	}


	if (!PugiGetSubNodeValue(data_element, "normal_shenge_type_max_count", m_other_cfg.normal_shenge_type_max_count) || m_other_cfg.normal_shenge_type_max_count <= 0)
	{
		return -1005;
	}


	if (!PugiGetSubNodeValue(data_element, "ultimate_shenge_type_max_count", m_other_cfg.ultimate_shenge_type_max_count) || m_other_cfg.ultimate_shenge_type_max_count <= 0)
	{
		return -1006;
	}

	if (!PugiGetSubNodeValue(data_element, "unactive_attr_add_percen", m_other_cfg.unactive_attr_add_percen) || m_other_cfg.unactive_attr_add_percen <= 0)
	{
		return -1007;
	}

	if (!PugiGetSubNodeValue(data_element, "active_attr_add_percen", m_other_cfg.active_attr_add_percen) || m_other_cfg.active_attr_add_percen <= 0)
	{
		return -1008;
	}

	if (!PugiGetSubNodeValue(data_element, "drop_item_cd_s", m_other_cfg.drop_item_cd_s) || m_other_cfg.drop_item_cd_s < 0)
	{
		return -1009;
	}

	if (!PugiGetSubNodeValue(data_element, "uplevel_zhangkong_itemid", m_other_cfg.uplevel_zhangkong_itemid) || NULL == ITEMPOOL->GetItem(m_other_cfg.uplevel_zhangkong_itemid))
	{
		return -1010;
	}

	if (!PugiGetSubNodeValue(data_element, "uplevel_zhangkong_gold", m_other_cfg.uplevel_zhangkong_gold) || m_other_cfg.uplevel_zhangkong_gold < 0)
	{
		return -1011;
	}

	if (!PugiGetSubNodeValue(data_element, "replacement_id", m_other_cfg.replacement_id) || NULL == ITEMPOOL->GetItem(m_other_cfg.replacement_id))
	{
		return -1012;
	}

	if (!PugiGetSubNodeValue(data_element, "consume_replacement", m_other_cfg.consume_replacement) || m_other_cfg.consume_replacement < 0)
	{
		return -1013;
	}

	if (!PugiGetSubNodeValue(data_element, "open_box_30_use_itemid", m_other_cfg.ten_chou_use_itemid) || NULL == ITEMPOOL->GetItem(m_other_cfg.ten_chou_use_itemid))
	{
		return -1014;
	}

	if (!PugiGetSubNodeValue(data_element, "open_box_30_use_item_num", m_other_cfg.ten_chou_use_item_num) || m_other_cfg.ten_chou_use_item_num < 0)
	{
		return -1015;
	}

	return 0;
}

int ShengeSystemCfg::InitShengeCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1000;
	}

	m_attr_cfg_count = 0;

	while (!data_element.empty())
	{
		int quality = 0;
		if (!PugiGetSubNodeValue(data_element, "quality", quality) || (quality != m_attr_cfg_count && quality != m_attr_cfg_count + 1))
		{
			return -1001;
		}

		if (quality >= SHENGE_QUALITY_COUNT)
		{
			return -1002;
		}

		ShengeSystemAttrCfg &cfg = m_attr_cfg_list[quality];
		cfg.quality = quality;
		m_attr_cfg_count = quality;

		int type = 0;
		if (!PugiGetSubNodeValue(data_element, "types", type))
		{
			return -1003;
		}

		if (type >= SHENGE_TYPE_COUNT )
		{
			return -2001;
		}

		ShengeSystemAttrtypeCfg &type_cfg = cfg.type_cfg_list[type];
		type_cfg.type = type;

		int level = 0;
		if (!PugiGetSubNodeValue(data_element, "level", level) || level < 0 || level >= SHENGE_SYSTEM_MAX_SHENGE_LEVEL)
		{
			return -1005;
		}
		ShengeSystemAttrLevelCfg &level_cfg = type_cfg.leve_cfg[level];
		level_cfg.level = level;

		if (!PugiGetSubNodeValue(data_element, "kind", level_cfg.kind) || level_cfg.kind < 0)
		{
			return -1006;
		}

		if (!PugiGetSubNodeValue(data_element, "next_level_need_marrow_score", level_cfg.next_level_need_marrow_score) || level_cfg.next_level_need_marrow_score < 0)
		{
			return -1007;
		}

		if (!PugiGetSubNodeValue(data_element, "return_score", level_cfg.return_score) || level_cfg.return_score <= 0)
		{
			return -1008;
		}

		if (!PugiGetSubNodeValue(data_element, "capbility", level_cfg.capbility) || level_cfg.capbility < 0)
		{
			return -1009;
		}

		if (!PugiGetSubNodeValue(data_element, "quyu", level_cfg.quyu) || level_cfg.quyu < 0)
		{
			return -10010;
		}

		char attr_name[100];
		for (int index = 0; index < ShengeSystemAttrLevelCfg::MAX_ATTR_COUNTS; ++index)
		{
			ShengeSystemAttrSingelCfg &attr_cfg = level_cfg.base_attr[index];


			SNPRINTF(attr_name, 100, "attr_type_%d", index);
			if (!PugiGetSubNodeValue(data_element, attr_name, attr_cfg.attr_type) || attr_cfg.attr_type < SHENGE_SYSTEM_ATTR_TYPE_MAXHP || attr_cfg.attr_type >= SHENGE_SYSTEM_ATTR_TYPE_COUNT)
			{
				return -1200;
			}

			SNPRINTF(attr_name, 100, "add_attributes_%d", index);
			if (!PugiGetSubNodeValue(data_element, attr_name, attr_cfg.attr_value) || attr_cfg.attr_value < 0)
			{
				return -1201;
			}
		}

		data_element = data_element.next_sibling();
	}

	++m_attr_cfg_count;
	if (m_attr_cfg_count != SHENGE_QUALITY_COUNT)
	{
		return -9999;
	}

	return 0;
}

int ShengeSystemCfg::InitDecomposeCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1000;
	}

	m_compose_cfg_count = 0;
	int last_quality = -1;

	while (!data_element.empty())
	{
		int kind = 0;
		if (!PugiGetSubNodeValue(data_element, "kind", kind) || (kind != m_compose_cfg_count && kind != m_compose_cfg_count + 1))
		{
			return -1001;
		}

		if (kind >= SHENGE_SYSTEM_MAX_KINDS)
		{
			return -1002;
		}

		if (kind == m_compose_cfg_count + 1)
		{
			last_quality = -1;
		}

		ShengeSystemComposeCfg &cfg = m_compose_cfg_list[kind];
		cfg.kind = kind;
		m_compose_cfg_count = kind;

		int quality = 0;
		if (!PugiGetSubNodeValue(data_element, "quality", quality) || quality != last_quality + 1)
		{
			return -1003;
		}

		if (quality >= SHENGE_QUALITY_COUNT)
		{
			return -1004;
		}

		ShengeSystemDecomposeQualityCfg &quality_cfg = cfg.quality_cfg_list[quality];
		quality_cfg.quality = quality;
		last_quality = quality;

		if (!PugiGetSubNodeValue(data_element, "composite_prob", quality_cfg.composite_prob) || quality_cfg.composite_prob < 0)
		{
			return -1005;
		}

		data_element = data_element.next_sibling();
	}

	if (last_quality + 1 != SHENGE_QUALITY_COUNT)
	{
		return -9998;
	}
	return 0;
}

int ShengeSystemCfg::InitChouJiangCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1000;
	}

	m_choujiang_cfg_total_weight = 0;
	m_choujiang_cfg_count = 0;
	while (!data_element.empty())
	{
		if (m_choujiang_cfg_count >= SHENGE_SYSTEM_CHOUJIANG_MAX_CFG_COUNTS)
		{
			return -9999;
		}

		int seq = 0;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq != m_choujiang_cfg_count)
		{
			return -1001;
		}

		ShengeSystemChoujiagnCfg &cfg = m_choujiang_cfg_list[seq];
		cfg.seq = seq;

		if (!PugiGetSubNodeValue(data_element, "weight", cfg.weight) || cfg.weight < 0)
		{
			return -1002;
		}

		m_choujiang_cfg_total_weight += cfg.weight;

		if (!PugiGetSubNodeValue(data_element, "quality", cfg.reward_shenge.quality) || cfg.reward_shenge.quality < 0)
		{
			return -1003;
		}

		if (!PugiGetSubNodeValue(data_element, "types", cfg.reward_shenge.type) || cfg.reward_shenge.type < 0)
		{
			return -1004;
		}

		if (!PugiGetSubNodeValue(data_element, "is_msg", cfg.is_msg) || cfg.is_msg < 0)
		{
			return -1005;
		}

		if (!PugiGetSubNodeValue(data_element, "ten_weight", cfg.ten_weight) || cfg.ten_weight < 0)
		{
			return -1006;
		}

		m_choujiang_cfg_total_ten_weight += cfg.ten_weight;

		if (SHENGE_SYSTEM_TEN_CHOU_MUST_GET_QUALITY == cfg.reward_shenge.quality)
		{
			m_choujiang_cfg_total_ten_special_weight += cfg.ten_weight;
		}

		++m_choujiang_cfg_count;
		data_element = data_element.next_sibling();
	}

	if (m_choujiang_cfg_total_weight <= 0 || m_choujiang_cfg_total_ten_weight <= 0 || m_choujiang_cfg_total_ten_special_weight <= 0)
	{
		return -9999;
	}

	return 0;
}

int ShengeSystemCfg::InitFirstTenChouJiangCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1000;
	}

	m_first_ten_chou_cfg_count = 0;
	m_first_ten_chou_total_weight = 0;
	while (!data_element.empty())
	{
		if (m_first_ten_chou_cfg_count >= SHENGE_SYSTEM_FIRST_TEN_CHOUJIANG_MAX_CFG_COUNTS)
		{
		}
		int seq = 0;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq != m_first_ten_chou_cfg_count)
		{
			return -1001;
		}

		ShengeSystemChoujiagnCfg &cfg = m_first_ten_chou_cfg_list[seq];
		cfg.seq = seq;

		if (!PugiGetSubNodeValue(data_element, "weight", cfg.weight) || cfg.weight < 0)
		{
			return -1002;
		}

		m_first_ten_chou_total_weight += cfg.weight;

		if (!PugiGetSubNodeValue(data_element, "quality", cfg.reward_shenge.quality) || cfg.reward_shenge.quality < 0)
		{
			return -1003;
		}

		if (!PugiGetSubNodeValue(data_element, "types", cfg.reward_shenge.type) || cfg.reward_shenge.type < 0)
		{
			return -1004;
		}

		++m_first_ten_chou_cfg_count;
		data_element = data_element.next_sibling();
	}

	if (m_first_ten_chou_total_weight <= 0)
	{
		return -9999;
	}

	return 0;
}

int ShengeSystemCfg::InitCombinationCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1000;
	}

	m_combination_cfg_count = 0;
	while (!data_element.empty())
	{
		if (m_combination_cfg_count >= SHENGE_SYSTEM_COMBINATION_MAX_CFG_COUNTS)
		{
			return -1001;
		}

		int seq = 0;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq != m_combination_cfg_count)
		{
			return -1002;
		}
		ShengeSystemCombinationCfg &cfg = m_combination_cfg_list[seq];
		cfg.seq = seq;

		for (int type = 0; type < SHENGE_TYPE_COUNT; ++ type)
		{
			int &tmp = cfg.type[type];
			int &quality_tmp = cfg.quality[type];

			char attr_name[100];
			SNPRINTF(attr_name, 100, "type_%d", type);
			if (!PugiGetSubNodeValue(data_element, attr_name, tmp) || tmp < 0 || tmp > SHENGE_SYSTEM_CUR_MAX_SHENGE_GRID)
			{
				return -1200;
			}

			char quality_attr_name[100];
			SNPRINTF(quality_attr_name, 100, "quality_%d", type);
			if (!PugiGetSubNodeValue(data_element, quality_attr_name, quality_tmp) || quality_tmp < 0 || quality_tmp >= SHENGE_TYPE_COUNT)
			{
				return -1300;
			}
		}

		for (int type = 0; type < SHENGE_SYSTEM_COMBINATION_PERCEN_CFG; ++type)
		{
			ShengeSystemCombinationCfg::AddPerCenCfg &tmp = cfg.add_percen_cfg_list[type];
			char attr_name[100];
			SNPRINTF(attr_name, 100, "attr_type_%d", type);
			if (!PugiGetSubNodeValue(data_element, attr_name, tmp.attr_tyep) || tmp.attr_tyep < SHENGE_SYSTEM_ATTR_TYPE_MAXHP || tmp.attr_tyep > SHENGE_SYSTEM_ATTR_TYPE_COUNT)
			{
				return -1201;
			}

			SNPRINTF(attr_name, 100, "add_attr_percen_%d", type);
			if (!PugiGetSubNodeValue(data_element, attr_name, tmp.attr_value) || tmp.attr_value < 0)
			{
				return -1202;
			}
		}

		for (int type = 0; type < SHENGE_SYSTEM_COMBINATION_SPECIAL_EFFECT_CFG; ++type)
		{
			ShengeSystemCombinationCfg::SpecialEffectCfg &tmp = cfg.special_cfg_list[type];
			char attr_name[100];
			SNPRINTF(attr_name, 100, "effect_type_%d", type);
			if (!PugiGetSubNodeValue(data_element, attr_name, tmp.ecffect_type) || tmp.ecffect_type < SHENGE_SPECIAL_EFFECT_TYPE_ADD_EXP || tmp.ecffect_type > SHENGE_SPECIAL_EFFECT_TYPE_COUNT)
			{
				return -1203;
			}

			SNPRINTF(attr_name, 100, "percen_value_%d", type);
			if (!PugiGetSubNodeValue(data_element, attr_name, tmp.percen_value) || tmp.percen_value < 0 || tmp.percen_value > 10000)
			{
				return -1204;
			}
		}

		++m_combination_cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int ShengeSystemCfg::InitItemIdShengeCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1000;
	}

	while (!data_element.empty())
	{
		if (m_item_id_Shenge_cfg_count >= SHENGE_SYSTEM_ITEM_ID_SHENGE_MAX_CFG)
		{
			return -1001;
		}

		ShengeSystemItemIdShengeCfg &cfg = m_item_id_Shenge_cfg_list[m_item_id_Shenge_cfg_count];

		if (!PugiGetSubNodeValue(data_element, "item_id", cfg.item_id) || NULL == ITEMPOOL->GetItem(cfg.item_id))
		{
			return -1003;
		}

		if (!PugiGetSubNodeValue(data_element, "quality", cfg.quality) || cfg.quality < SHENGE_QUALITY_BLUE || cfg.quality > SHENGE_QUALITY_COUNT)
		{
			return -1004;
		}

		if (!PugiGetSubNodeValue(data_element, "types", cfg.type) || cfg.type < SHENGE_TYPE_HP || cfg.type > SHENGE_TYPE_COUNT)
		{
			return -1005;
		}

		++m_item_id_Shenge_cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int ShengeSystemCfg::InitDropIdCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1000;
	}

	m_drop_id_list_count = 0;
	m_drop_id_total_weight = 0;
	while (!data_element.empty())
	{
		if (m_drop_id_list_count >= SHENGE_SYSTEM_DROP_ID_MAX_CFG)
		{
			return -1001;
		}

		int seq = 0;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq != m_drop_id_list_count)
		{
			return -1002;
		}
		ShengeSystemDropIdCfg &cfg = m_drop_id_list[seq];
		cfg.seq = seq;

		if (!PugiGetSubNodeValue(data_element, "weight", cfg.weight) || cfg.weight < 0)
		{
			return -1004;
		}
		m_drop_id_total_weight += cfg.weight;

		if (!PugiGetSubNodeValue(data_element, "drop_id", cfg.drop_id) || cfg.drop_id <= 0)
		{
			return -1005;
		}

		++m_drop_id_list_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int ShengeSystemCfg::InitBroadcastCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1000;
	}

	while (!data_element.empty())
	{
		int shenge_level = 0;
		if (!PugiGetSubNodeValue(data_element, "shenge_level", shenge_level) || shenge_level <= 0)
		{
			return -1001;
		}

		bool is_broadcast;
		if (!PugiGetSubNodeValue(data_element, "is_broadcast", is_broadcast))
		{
			return -1002;
		}

		m_is_broadcast_map[shenge_level] = is_broadcast;

		data_element = data_element.next_sibling();
	}

	return 0;
}

int ShengeSystemCfg::InitShengePageCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1000;
	}

	while (!data_element.empty())
	{
		ShengeSystemPageCfg cfg;

		if (!PugiGetSubNodeValue(data_element, "shengepage", cfg.page) || cfg.page < 0 || cfg.page >= SHENGE_SYSTEM_MAX_SHENGE_PAGE)
		{
			return -1001;
		}

		int level = 0;
		if (!PugiGetSubNodeValue(data_element, "shengepage_open", level) || level < 0)
		{
			return -1002;
		}

		if (!PugiGetSubNodeValue(data_element, "is_broadcast", cfg.is_broadcast))
		{
			return -1003;
		}

		m_page_map[level] = cfg;

		data_element = data_element.next_sibling();
	}

	return 0;
}

int ShengeSystemCfg::InitZhangkongLevelCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -100000;
	}

	int last_grid = 0;
	int last_level = -1;
	
	int last_grade = 0;
	int last_star = 0;

	while (!dataElement.empty())
	{
		int grid = -1;
		if (!PugiGetSubNodeValue(dataElement, "grid", grid) || grid < 0 || grid >= SHENGE_SYSTEM_QUALITY_MAX_VALUE)
		{
			return -1;
		}
		if (grid == last_grid + 1)
		{
			last_grid = grid;
			last_level = -1;

			last_grade = 0;
			last_star = 0;
		}

		int level = -1;
		if (!PugiGetSubNodeValue(dataElement, "level", level) || level < 0 || level > SHENGE_SYSTEM_ZHANGKONG_MAX_LEVEL || level != last_level + 1)
		{
			return -1;
		}
		last_level = level;

		ZhangkongLevelCfg::ConfigItem &cfg = m_zhangkong_level_cfg[grid].cfg_list[level];
		cfg.grid = grid;
		cfg.level = level;

		if (!PugiGetSubNodeValue(dataElement, "grade", cfg.grade) || cfg.grade < 0 || cfg.grade >= SHENGE_SYSTEM_ZHANGKONG_MAX_GRADE)
		{
			return -2;
		}

		if (cfg.grade == last_grade + 1)
		{
			last_grade = cfg.grade;
			last_star = 0;
		}
		
		if (!PugiGetSubNodeValue(dataElement, "star", cfg.star) || cfg.star < 0 || cfg.star > SHENGE_SYSTEM_ZHANGKONG_MAX_STAR || (cfg.star != last_star + 1 && 0 != cfg.star))
		{
			return -3;
		}
		last_star = cfg.star;

		if (!PugiGetSubNodeValue(dataElement, "exp", cfg.exp) || cfg.exp < 0) 
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "gongji_pro", cfg.gongji_pro) || cfg.gongji_pro < 0) 
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElement, "fangyu_pro", cfg.fangyu_pro) || cfg.fangyu_pro < 0) 
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(dataElement, "maxhp_pro", cfg.maxhp_pro) || cfg.maxhp_pro < 0) 
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(dataElement, "shanbi_pro", cfg.shanbi_pro) || cfg.shanbi_pro < 0) 
		{
			return -10;
		}

		if (!PugiGetSubNodeValue(dataElement, "baoji_pro", cfg.baoji_pro) || cfg.baoji_pro < 0)
		{
			return -11;
		}

		if (!PugiGetSubNodeValue(dataElement, "kangbao_pro", cfg.kangbao_pro) || cfg.kangbao_pro < 0) 
		{
			return -12;
		}

		if (!PugiGetSubNodeValue(dataElement, "mingzhong_pro", cfg.mingzhong_pro) || cfg.mingzhong_pro < 0) 
		{
			return -13;
		}

		if (!PugiGetSubNodeValue(dataElement, "shenge_pro", cfg.shenge_pro) || cfg.shenge_pro < 0)
		{
			return -14;
		}

		if (!PugiGetSubNodeValue(dataElement, "is_broadcast", cfg.is_broadcast) || cfg.is_broadcast < 0)
		{
			return -15;
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

ZhangkongLevelCfg::ConfigItem * ShengeSystemCfg::GetZhangkongCfgByGridAndLevel(int grid, int level)
{
	if (grid < 0 || grid >= SHENGE_SYSTEM_QUALITY_MAX_VALUE || level < 0 || level > SHENGE_SYSTEM_ZHANGKONG_MAX_LEVEL)
	{
		return NULL;
	}

	ZhangkongLevelCfg::ConfigItem &cfg = m_zhangkong_level_cfg[grid].cfg_list[level];
	if (cfg.grid != grid || cfg.level != level) return NULL;
	
	return &m_zhangkong_level_cfg[grid].cfg_list[level];
}

int ShengeSystemCfg::InitZhangkongRandExpCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1000;
	}

	int list_count = 0;
	while (!data_element.empty())
	{
		if (list_count >= SHENGE_SYSTEM_ZHANGKONG_RAND_EXP_COUNT)
		{
			return -1001;
		}

		ZhangkongRandExpCfg &cfg = m_zhangkong_rand_exp_cfg_list[list_count];

		if (!PugiGetSubNodeValue(data_element, "weight", cfg.weight) || cfg.weight <= 0)
		{
			return -1002;
		}

		m_zhangkong_rand_exp_all_weight += cfg.weight;

		if (!PugiGetSubNodeValue(data_element, "exp", cfg.exp) || cfg.exp <= 0)
		{
			return -1003;
		}

		++list_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int ShengeSystemCfg::GetZhangkongRandExp()
{
	int rand_weight = RandomNum(m_zhangkong_rand_exp_all_weight);

	for (int i = 0; i < SHENGE_SYSTEM_ZHANGKONG_RAND_EXP_COUNT; i++)
	{
		if (rand_weight < m_zhangkong_rand_exp_cfg_list[i].weight)
		{
			return m_zhangkong_rand_exp_cfg_list[i].exp;
		}

		rand_weight -= m_zhangkong_rand_exp_cfg_list[i].weight;
	}

	return 0;
}

int ShengeSystemCfg::InitZhangkongRandGridCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1000;
	}

	int list_count = 0;
	while (!data_element.empty())
	{
		if (list_count >= SHENGE_SYSTEM_QUALITY_MAX_VALUE)
		{
			return -1001;
		}

		ZhangkongRandGradeCfg &cfg = m_zhangkong_rand_grade_cfg_list[list_count];

		if (!PugiGetSubNodeValue(data_element, "weight", cfg.weight) || cfg.weight <= 0)
		{
			return -1002;
		}

		m_zhangkong_rand_grade_all_weight += cfg.weight;

		if (!PugiGetSubNodeValue(data_element, "grid", cfg.grid) || cfg.grid < 0)
		{
			return -1003;
		}

		++list_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int ShengeSystemCfg::InitShengeShenquCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1000;
	}

	while (!data_element.empty())
	{
		ShengeSystemShenquCfg cfg;
		if (!PugiGetSubNodeValue(data_element, "shenqu_id", cfg.shenqu_id) || cfg.shenqu_id < 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "role_level", cfg.role_level) || cfg.role_level <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "stuff_id", cfg.stuff_id) || NULL == ITEMPOOL->GetItem(cfg.stuff_id))
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "value_percent", cfg.value_percent) || cfg.value_percent < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "fighting_capacity", cfg.fighting_capacity) || cfg.fighting_capacity < 0)
		{
			return -5;
		}

		char name[100];
		for (int i = 0; i < SHENGE_SYSTEM_SHENGESHENQU_XILIAN_SLOT_MAX_NUM; ++ i)
		{
			ShengeSystemShenquCfg::StuffNumCfg & stuff_num_cfg = cfg.stuff_num_list[i];

			SNPRINTF(name, 100, "stuff_num_%d", i);
			if (!PugiGetSubNodeValue(data_element, name, stuff_num_cfg.stuff_num) || stuff_num_cfg.stuff_num < 0)
			{
				return -(1000 + i);
			}
		}

		m_shenge_shenqu_cfg.push_back(cfg);
		data_element = data_element.next_sibling();
	}

	return 0;
}

int ShengeSystemCfg::InitShengeShenquXilianCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1000;
	}

	while (!data_element.empty())
	{
		ShengeSystemXilianAttrCfg cfg;
		if (!PugiGetSubNodeValue(data_element, "shenqu_id", cfg.shenqu_id) || cfg.shenqu_id < 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "point_type", cfg.point_type) || cfg.point_type < 0)
		{
			return -2;
		}

		char attr_name[100];
		for (int index = 0; index < SHENGE_SYSTEM_XILIAN_BAODI_MAX_COUNT; ++index)
		{
			ShengeSystemXilianAttrCfg::BaodiCfg & baodi_cfg = cfg.baodi_count_cfg[index];

			SNPRINTF(attr_name, 100, "baodi_count_%d", index);
			if (!PugiGetSubNodeValue(data_element, attr_name, baodi_cfg.baodi_count) || baodi_cfg.baodi_count < 0)
			{
				return -(1000 + index);
			}

			SNPRINTF(attr_name, 100, "criti_per_%d", index);
			if (!PugiGetSubNodeValue(data_element, attr_name, baodi_cfg.criti_per) || baodi_cfg.criti_per < 0 || baodi_cfg.criti_per > 100)
			{
				return -(1100 + index);
			}

		}

		for (int base_value_index = 0; base_value_index < SHENGE_SYSTEM_SHENGESHENQU_ATTR_MAX_NUM; ++base_value_index)
		{
			ShengeSystemXilianAttrCfg::AttrInfo & value_cfg = cfg.attr_list[base_value_index];
			value_cfg.value_type = base_value_index;

			SNPRINTF(attr_name, 100, "weight_%d", base_value_index);
			if (!PugiGetSubNodeValue(data_element, attr_name, value_cfg.weight) || value_cfg.weight < 0)
			{
				return -(1200 + base_value_index);				// base_value_index 为属性值类型
			}
			cfg.total_weight += value_cfg.weight;

			SNPRINTF(attr_name, 100, "min_%d", base_value_index);
			if (!PugiGetSubNodeValue(data_element, attr_name, value_cfg.value_min) || value_cfg.value_min < 0)
			{
				return -(1300 + base_value_index);				// base_value_index 为属性值类型
			}

			SNPRINTF(attr_name, 100, "max_%d", base_value_index);
			if (!PugiGetSubNodeValue(data_element, attr_name, value_cfg.value_max) || value_cfg.value_max < value_cfg.value_min)
			{
				return -(1400 + base_value_index);
			}

			SNPRINTF(attr_name, 100, "criti_value_%d", base_value_index);
			if (!PugiGetSubNodeValue(data_element, attr_name, value_cfg.criti_value) || value_cfg.criti_value < value_cfg.value_min || value_cfg.criti_value > value_cfg.value_max)
			{
				return -(1500 + base_value_index);
			}
		}

		m_shenge_shenqu_xilian_cfg.push_back(cfg);
		data_element = data_element.next_sibling();
	}

	return 0;
}

int ShengeSystemCfg::InitUplevelGridCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1000;
	}

	while (!data_element.empty())
	{
		int index = 0;
		if (!PugiGetSubNodeValue(data_element, "index", index) || index < 0 || index >= SHENGE_SYSTEM_QUALITY_MAX_VALUE)
		{
			return -1;
		}

		UplevelGridCfg cfg;
		cfg.index = index;

		if (!PugiGetSubNodeValue(data_element, "item_id", cfg.stuff_id) || cfg.stuff_id < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "quality_range", cfg.quality_range) || cfg.quality_range < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "quality_range_2", cfg.quality_range_2) || cfg.quality_range_2 < 0)
		{
			return -4;
		}

		m_shenge_uplevel_grid_cfg.push_back(cfg);

		data_element = data_element.next_sibling();
	}

	return 0;
}

int ShengeSystemCfg::InitNewSuitCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{

		int color = 0;
		if (!PugiGetSubNodeValue(data_element, "quality", color) || color < 0 || color >= SHENGE_QUALITY_COUNT)
		{
			return -97;
		}

		ShengeSystemNewSuitCfg &suit_cfg = m_suit_list[color];

		int need_count = 0;
		if (!PugiGetSubNodeValue(data_element, "need_count", need_count) || need_count < 0 )
		{
			return -98;
		}

		if (suit_cfg.suit_count < 0 || suit_cfg.suit_count >= SHENGE_SYSTEM_SUIT_CFG_MAX_COUNT)
		{
			return -99;
		}

		ShengeSystemNewSuitAttrCfg &attr_cfg = suit_cfg.attr[suit_cfg.suit_count];

		attr_cfg.need_count = need_count;

		if (!PugiGetSubNodeValue(data_element, "maxhp", attr_cfg.maxhp) || attr_cfg.maxhp < 0)
		{
			return -100;
		}

		if (!PugiGetSubNodeValue(data_element, "gongji", attr_cfg.gongji) || attr_cfg.gongji < 0)
		{
			return -101;
		}

		if (!PugiGetSubNodeValue(data_element, "fangyu", attr_cfg.fangyu) || attr_cfg.fangyu < 0)
		{
			return -102;
		}

		if (!PugiGetSubNodeValue(data_element, "mingzhong", attr_cfg.mingzhong) || attr_cfg.mingzhong < 0)
		{
			return -103;
		}

		if (!PugiGetSubNodeValue(data_element, "shanbi", attr_cfg.shanbi) || attr_cfg.shanbi < 0)
		{
			return -104;
		}

		if (!PugiGetSubNodeValue(data_element, "baoji", attr_cfg.baoji) || attr_cfg.baoji < 0)
		{
			return -105;
		}

		if (!PugiGetSubNodeValue(data_element, "jianren", attr_cfg.jianren) || attr_cfg.jianren < 0)
		{
			return -106;
		}

		if (!PugiGetSubNodeValue(data_element, "per_gongji", attr_cfg.per_gongji) || attr_cfg.per_gongji < 0)
		{
			return -108;
		}

		if (!PugiGetSubNodeValue(data_element, "per_maxhp", attr_cfg.per_maxhp) || attr_cfg.per_maxhp < 0)
		{
			return -109;
		}

		if (!PugiGetSubNodeValue(data_element, "per_fangyu", attr_cfg.per_fangyu) || attr_cfg.per_fangyu < 0)
		{
			return -110;
		}

		if (!PugiGetSubNodeValue(data_element, "per_mianshang", attr_cfg.per_mianshang) || attr_cfg.per_mianshang < 0)
		{
			return -111;
		}

		++suit_cfg.suit_count;

		data_element = data_element.next_sibling();
	}

	return 0;
}

int ShengeSystemCfg::GetZhangkongRandGrid()
{
	int rand_weight = RandomNum(m_zhangkong_rand_grade_all_weight);

	for (int i = 0; i < SHENGE_SYSTEM_QUALITY_MAX_VALUE; i++)
	{
		if (rand_weight < m_zhangkong_rand_grade_cfg_list[i].weight)
		{
			return m_zhangkong_rand_grade_cfg_list[i].grid;
		}

		rand_weight -= m_zhangkong_rand_grade_cfg_list[i].weight;
	}

	return 0;
}

int ShengeSystemCfg::GetZhangkongGridRandWeight(int grid_index)
{
	if (grid_index < 0 || grid_index >= static_cast<int>(static_array_size(m_zhangkong_rand_grade_cfg_list)))
	{
		return 0;
	}
	return m_zhangkong_rand_grade_cfg_list[grid_index].weight;
}

const ShengeSystemShenquCfg * ShengeSystemCfg::GetShenquCfg(int shenqu_id)
{
	if (shenqu_id < 0) return NULL;

	for (int i = 0; i < (int)m_shenge_shenqu_cfg.size(); ++ i)
	{
		if (shenqu_id == m_shenge_shenqu_cfg[i].shenqu_id)
		{
			return &m_shenge_shenqu_cfg[i];
		}
	}

	return NULL;
}

const ShengeSystemXilianAttrCfg * ShengeSystemCfg::GetXilianCfg(int shenqu_id, int point_type)
{
	if (shenqu_id < 0 || shenqu_id >= SHENGE_SYSTEM_SHENGESHENQU_MAX_NUM || point_type < 0 || point_type >= SHENGE_SYSTEM_SHENGESHENQU_ATTR_MAX_NUM) return NULL;

	for (int i = 0; i < (int)m_shenge_shenqu_xilian_cfg.size(); ++ i)
	{
		if (m_shenge_shenqu_xilian_cfg[i].shenqu_id == shenqu_id && m_shenge_shenqu_xilian_cfg[i].point_type == point_type)
		{
			return &m_shenge_shenqu_xilian_cfg[i];
		}
	}
	return NULL;
}

const UplevelGridCfg * ShengeSystemCfg::GetUplevelGridCfg(int index) const
{
	if (index < 0 || index >= SHENGE_SYSTEM_QUALITY_MAX_VALUE) return NULL;

	for (auto &uplevel_iter : m_shenge_uplevel_grid_cfg)
	{
		if (uplevel_iter.index == index)
		{
			return &uplevel_iter;
		}
	}

	return NULL;
}

const ShengeSystemNewSuitAttrCfg * ShengeSystemCfg::GetSuitCfg(int color, int count) const
{
	if (color < 0 || color >= SHENGE_QUALITY_COUNT)
	{
		return NULL;
	}

	if (m_suit_list[color].suit_count > 0)
	{
		for (int i = m_suit_list[color].suit_count - 1; i >= 0 && i < SHENGE_SYSTEM_SUIT_CFG_MAX_COUNT; i--)
		{
			if (count >= m_suit_list[color].attr[i].need_count)
			{
				return &m_suit_list[color].attr[i];
			}
		}
	}

	return NULL;
}
