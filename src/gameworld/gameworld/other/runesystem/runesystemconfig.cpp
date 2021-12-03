#include "runesystemconfig.hpp"
#include "servercommon/configcommon.h"
#include "servercommon/servercommon.h"
#include "obj/character/attribute.hpp"
#include "item/itempool.h"
#include "servercommon/runesystemdef.hpp"

RuneSystemCfg::RuneSystemCfg() : m_rune_awaken_count(0), m_rune_awaken_cost_count(0), m_zhuling_slot_cfg_total_weight(0), m_zhuling_slot_cfg_count(0), m_rune_level_open_cfg_count(0)
{
}

RuneSystemCfg::~RuneSystemCfg()
{
}

bool RuneSystemCfg::Init(const std::string &configname, std::string *err)
{
	PUGI_XML_CONFIG_PRE_LOAD;

	// 其他配置
	PUGI_XML_LOAD_CONFIG("other", InitOtherCfg);
	// 符文等级开启
	PUGI_XML_LOAD_CONFIG("rune_level_open", InitRuneLevelOpenCfg);
	// 符文槽开启
	PUGI_XML_LOAD_CONFIG("rune_slot_open", InitSlotOpenCfg);
	// 符文获取
	PUGI_XML_LOAD_CONFIG("rune_fetch", InitFetchCfg);
	// 符文属性
	PUGI_XML_LOAD_CONFIG("rune_attr", InitAttrCfg);
	// 实物id虚拟id对照
	PUGI_XML_LOAD_CONFIG("real_id_list", InitIdListCfg);
	// 符文合成
	PUGI_XML_LOAD_CONFIG("rune_compose", InitComposeCfg);
	// 宝箱开启
	PUGI_XML_LOAD_CONFIG("open_box", InitOpenBoxCfg);
	// 符文觉醒
	PUGI_XML_LOAD_CONFIG("awaken_type", InitAwakenCfg);
	// 符文觉醒限制
	PUGI_XML_LOAD_CONFIG("awaken_limit", InitAwakenLimitCfg);
	// 符文觉醒花费
	PUGI_XML_LOAD_CONFIG("awaken_cost", InitAwakenCostCfg);
	// 符文注灵
	PUGI_XML_LOAD_CONFIG("fuwen_zhuling", InitZhulingGradeCfg);
	// 符文注灵槽
	PUGI_XML_LOAD_CONFIG("fuwen_zhuling_slot", InitZhulingSlotCfg);

	return true;
}

int RuneSystemCfg::InitOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1000;
	}

	if (!PugiGetSubNodeValue(data_element, "free_cd", m_other_cfg.free_cd) || m_other_cfg.free_cd <= 0)
	{
		return -1001;
	}

	if (!PugiGetSubNodeValue(data_element, "xunbao_consume_itemid", m_other_cfg.xunbao_consume_itemid) || NULL == ITEMPOOL->GetItem(m_other_cfg.xunbao_consume_itemid))
	{
		return -1002;
	}

	if (!PugiGetSubNodeValue(data_element, "xunbao_one_consume_num", m_other_cfg.xunbao_one_consume_num) || m_other_cfg.xunbao_one_consume_num <= 0)
	{
		return -1003;
	}

	if (!PugiGetSubNodeValue(data_element, "xunbao_ten_consume_num", m_other_cfg.xunbao_ten_consume_num) || m_other_cfg.xunbao_ten_consume_num <= 0)
	{
		return -1004;
	}


	if (!PugiGetSubNodeValue(data_element, "rune_suipian_num_low", m_other_cfg.rune_suipian_num_low) || m_other_cfg.rune_suipian_num_low <= 0)
	{
		return -1005;
	}

	if (!PugiGetSubNodeValue(data_element, "rune_suipian_num_high", m_other_cfg.rune_suipian_num_high) || m_other_cfg.rune_suipian_num_high <= 0 || m_other_cfg.rune_suipian_num_high <= m_other_cfg.rune_suipian_num_low)
	{
		return -1006;
	}

	if (!PugiGetSubNodeValue(data_element, "rune_compose_need_layer", m_other_cfg.rune_compose_need_layer) || m_other_cfg.rune_compose_need_layer <= 0)
	{
		return -1007;
	}

	if (!PugiGetSubNodeValue(data_element, "xunbao_one_magic_crystal", m_other_cfg.xunbao_one_magic_crystal) || m_other_cfg.xunbao_one_magic_crystal <= 0)
	{
		return -1008;
	}

	if (!PugiGetSubNodeValue(data_element, "jinghua_box_min", m_other_cfg.jinghua_box_min) || m_other_cfg.jinghua_box_min <= 0)
	{
		return -1009;
	}

	if (!PugiGetSubNodeValue(data_element, "jinghua_box_max", m_other_cfg.jinghua_box_max) || m_other_cfg.jinghua_box_max < m_other_cfg.jinghua_box_min)
	{
		return -1010;
	}

	if (!PugiGetSubNodeValue(data_element, "rune_box_min", m_other_cfg.rune_box_min) || m_other_cfg.rune_box_min <= 0)
	{
		return -1011;
	}

	if (!PugiGetSubNodeValue(data_element, "rune_box_max", m_other_cfg.rune_box_max) || m_other_cfg.rune_box_max < m_other_cfg.rune_box_min)
	{
		return -1012;
	}

	if (!PugiGetSubNodeValue(data_element, "jinghua_box_magic_crystal", m_other_cfg.jinghua_box_magic_crystal) || m_other_cfg.jinghua_box_magic_crystal < 0)
	{
		return -1013;
	}

	if (!PugiGetSubNodeValue(data_element, "fuwen_box_magic_crystal", m_other_cfg.fuwen_box_magic_crystal) || m_other_cfg.fuwen_box_magic_crystal < 0)
	{
		return -1014;
	}

	if (!PugiGetSubNodeValue(data_element, "first_xubao_rune_id", m_other_cfg.first_xubao_rune_id) || m_other_cfg.first_xubao_rune_id < 0 || (m_other_cfg.first_xubao_rune_id != 0 && NULL == ITEMPOOL->GetItem(m_other_cfg.first_xubao_rune_id)))
	{
		return -1015;
	}

	if (!PugiGetSubNodeValue(data_element, "awaken_baodi", m_other_cfg.awaken_baodi) || m_other_cfg.awaken_baodi <= 0)
	{
		return -1016;
	}

	if (!PugiGetSubNodeValue(data_element, "zhuling_slot_max_value", m_other_cfg.zhuling_slot_max_value) || m_other_cfg.zhuling_slot_max_value <= 0)
	{
		return -1017;
	}

	if (!PugiGetSubNodeValue(data_element, "zhuling_cost", m_other_cfg.zhuling_cost) || m_other_cfg.zhuling_cost <= 0)
	{
		return -1018;
	}

	if (!PugiGetSubNodeValue(data_element, "rune_awake_need_layer", m_other_cfg.rune_awake_need_layer) || m_other_cfg.rune_awake_need_layer < 0)
	{
		return -1019;
	}

	if (!PugiGetSubNodeValue(data_element, "rune_lianhun_need_layer", m_other_cfg.rune_lianhun_need_layer) || m_other_cfg.rune_lianhun_need_layer < 0)
	{
		return -1020;
	}

	if (!PugiGetSubNodeValue(data_element, "total_x_times", m_other_cfg.total_x_times) || m_other_cfg.total_x_times <= 0)
	{
		return -1021;
	}

	if (!PugiGetSubNodeValue(data_element, "total_x1_times", m_other_cfg.total_x1_times) || m_other_cfg.total_x1_times <= 0 || m_other_cfg.total_x_times >= m_other_cfg.total_x1_times)
	{
		return -1022;
	}

	return 0;
}

int RuneSystemCfg::InitSlotOpenCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1000;
	}

	int next_slot = 0;

	while (!data_element.empty())
	{
		RuneSystemSlotOpenCfg cfg;

		if (!PugiGetSubNodeValue(data_element, "open_rune_slot", cfg.open_rune_slot) || cfg.open_rune_slot < 0 || cfg.open_rune_slot >= RUNE_SYSTEM_SLOT_MAX_NUM || cfg.open_rune_slot != next_slot)
		{
			return -1;
		}

		++ next_slot;

		if (!PugiGetSubNodeValue(data_element, "need_pass_layer", cfg.need_pass_layer) || cfg.need_pass_layer < 0)
		{
			return -2;
		}

		m_slot_open_cfg_vec.push_back(cfg);

		data_element = data_element.next_sibling();
	}

	if ((int)m_slot_open_cfg_vec.size() > RUNE_SYSTEM_SLOT_MAX_NUM)
	{
		return -100;
	}

	return 0;
}

int RuneSystemCfg::InitFetchCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1000;
	}

	while (!data_element.empty())
	{
		RuneSystemRuneFetchCfg cfg;

		if (!PugiGetSubNodeValue(data_element, "rune_id", cfg.rune_id) || NULL == ITEMPOOL->GetItem(cfg.rune_id))
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "seq", cfg.seq) || cfg.seq < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "in_layer_open", cfg.in_layer_open) || cfg.in_layer_open < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "one_weight", cfg.one_weight) || cfg.one_weight < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "ten_weight", cfg.ten_weight) || cfg.ten_weight < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "is_red_rune", cfg.is_red_rune) || cfg.is_red_rune < 0 || cfg.is_red_rune > 1)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "red_rune_weight", cfg.red_rune_weight) || cfg.red_rune_weight < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(data_element, "total_x_times_weight", cfg.total_x_times_weight) || cfg.total_x_times_weight < 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(data_element, "total_x1_times_weight", cfg.total_x1_times_weight) || cfg.total_x1_times_weight < 0)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(data_element, "convert_consume_rune_suipian", cfg.convert_consume_rune_suipian) || cfg.convert_consume_rune_suipian < 0)
		{
			return -10;
		}

		RuneSystemRuneFetchCfgMapIt it = m_rune_id_cfg_map.find(cfg.rune_id);
		if (it != m_rune_id_cfg_map.end() && it->second.convert_consume_rune_suipian != cfg.convert_consume_rune_suipian)
		{
			return -100;
		}

		m_rune_id_cfg_map[cfg.rune_id] = cfg;

		m_rune_fetch_weight_vec.emplace_back(cfg);

		data_element = data_element.next_sibling();
	}

	return 0;
}

int RuneSystemCfg::InitAttrCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1000;
	}

	while (!data_element.empty())
	{
		RuneSystemAttrCfg cfg;

		if (!PugiGetSubNodeValue(data_element, "quality", cfg.quality) || cfg.quality < RUNE_QUALITY_KIND_WHITE || cfg.quality >= RUNE_QUALITY_KIND_MAX)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "types", cfg.types) || cfg.types < RUNE_TYPE_GONGJI || cfg.types >= RUNE_TYPE_MAX)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "level", cfg.level) || cfg.level <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "uplevel_need_jinghua", cfg.uplevel_need_jinghua) || cfg.uplevel_need_jinghua < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "attr_type_0", cfg.attr_type_0) || cfg.attr_type_0 < RUNE_ATTR_TYPE_INVALID || cfg.attr_type_0 >= RUNE_ATTR_TYPE_COUNT)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "add_attributes_0", cfg.add_attributes_0) || cfg.add_attributes_0 < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "attr_type_1", cfg.attr_type_1) || cfg.attr_type_1 < RUNE_ATTR_TYPE_INVALID || cfg.attr_type_1 >= RUNE_ATTR_TYPE_COUNT)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(data_element, "add_attributes_1", cfg.add_attributes_1) || cfg.add_attributes_1 < 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(data_element, "dispose_fetch_jinghua", cfg.dispose_fetch_jinghua) || cfg.dispose_fetch_jinghua < 0)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(data_element, "other_capability", cfg.other_capability) || cfg.other_capability < 0)
		{
			return -10;
		}

		if (!PugiGetSubNodeValue(data_element, "is_broadcast", cfg.broadcast))
		{
			return -11;
		}

		long long key = ConvertParamToLongLong(cfg.quality, cfg.types, cfg.level);
		RuneSystemAttrCfgMapIt it = m_attr_cfg_map.find(key);
		if (it != m_attr_cfg_map.end())
		{
			return -100;
		}

		m_attr_cfg_map[key] = cfg;

		data_element = data_element.next_sibling();
	}

	return 0;
}

int RuneSystemCfg::InitIdListCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1000;
	}

	while (!data_element.empty())
	{
		RuneSystemRealIdCfg cfg;

		if (!PugiGetSubNodeValue(data_element, "rune_id", cfg.rune_id) || NULL == ITEMPOOL->GetItem(cfg.rune_id))
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "quality", cfg.quality) || cfg.quality < RUNE_QUALITY_KIND_WHITE || cfg.quality >= RUNE_QUALITY_KIND_MAX)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "type", cfg.type) || cfg.type < RUNE_TYPE_GONGJI || cfg.type >= RUNE_TYPE_MAX)
		{
			return -3;
		}

		int is_notice = 0;
		if (!PugiGetSubNodeValue(data_element, "is_notice", is_notice) || is_notice < 0)
		{
			return -4;
		}

		if(is_notice > 0)
		{
			cfg.is_notice = true;
		}

		RuneSystemRealIdCfgMapIt it = m_real_id_cfg_map.find(cfg.rune_id);
		if (it != m_real_id_cfg_map.end())
		{
			return -100;
		}

		m_real_id_cfg_map[cfg.rune_id] = cfg;

		data_element = data_element.next_sibling();
	}

	return 0;
}


int RuneSystemCfg::InitComposeCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1000;
	}

	while (!data_element.empty())
	{
		RuneSystemComposeCfg cfg;

		if (!PugiGetSubNodeValue(data_element, "rune1_id", cfg.rune1_id) || NULL == ITEMPOOL->GetItem(cfg.rune1_id))
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "rune2_id", cfg.rune2_id) || NULL == ITEMPOOL->GetItem(cfg.rune2_id))
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "magic_crystal_num", cfg.magic_crystal_num) || cfg.magic_crystal_num <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "get_rune_id", cfg.get_rune_id) || NULL == ITEMPOOL->GetItem(cfg.get_rune_id))
		{
			return -4;
		}

		long long key = ConvertParamToLongLong(cfg.rune1_id, cfg.rune2_id);

		RuneSystemComposeCfgMapIt it = m_compose_cfg_map.find(key);
		if (it != m_compose_cfg_map.end())
		{
			return -100;
		}

		m_compose_cfg_map[key] = cfg;

		data_element = data_element.next_sibling();
	}

	return 0;
}

int RuneSystemCfg::InitOpenBoxCfg(PugiXmlNode  RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		RuneSystemOpenBoxCfg cfg;

		if (!PugiGetSubNodeValue(data_element, "rune_id", cfg.rune_id) || NULL == ITEMPOOL->GetItem(cfg.rune_id))
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "in_layer_open", cfg.in_layer_open) || cfg.in_layer_open < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "weight_rune_box", cfg.weight_rune_box) || cfg.weight_rune_box < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "weight_jinghua_box", cfg.weight_jinghua_box) || cfg.weight_jinghua_box < 0)
		{
			return -4;
		}

		RuneSystemOpenBoxCfgMapIt it = m_open_box_cfg_map.find(cfg.rune_id);
		if (it != m_open_box_cfg_map.end())
		{
			return -100;
		}

		m_open_box_cfg_map[cfg.rune_id] = cfg;

		data_element = data_element.next_sibling();
	}

	return 0;
}

int RuneSystemCfg::InitAwakenCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		RuneSystemAwakenCfg cfg;

		if (!PugiGetSubNodeValue(data_element, "awaken_seq", cfg.awaken_seq) || m_rune_awaken_map.find(cfg.awaken_seq) != m_rune_awaken_map.end())
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "awaken_type", cfg.awaken_type) || cfg.awaken_type <= RUEN_AWAKEN_REWARD_TYPE_INVALID || cfg.awaken_type >= RUEN_AWAKEN_REWARD_TYPE_MAX)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "add_value", cfg.add_value) || cfg.add_value <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "common_pro", cfg.common_pro) || cfg.common_pro < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "diamond_pro", cfg.diamond_pro) || cfg.diamond_pro < 0)
		{
			return -5;
		}

		m_rune_awaken_map[cfg.awaken_seq] = cfg;

		data_element = data_element.next_sibling();
	}

	return 0;
}

int RuneSystemCfg::InitAwakenLimitCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_rune_awaken_count = 0;
	int last_level = -1;
	while (!data_element.empty())
	{
		if (m_rune_awaken_count < 0 || m_rune_awaken_count >= RUNESYSTEM_AWAKEN_LIMIT_MAX_COUNT)
		{
			return -1;
		}

		RuneSystemAwakenLimitCfg &limit_cfg = m_rune_awaken_limit_list[m_rune_awaken_count];

		if (!PugiGetSubNodeValue(data_element, "min_level", limit_cfg.min_level) || limit_cfg.min_level < 0 || limit_cfg.min_level != last_level + 1)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "max_level", limit_cfg.max_level) || limit_cfg.max_level < 0 || limit_cfg.max_level < limit_cfg.min_level)
		{
			return -3;
		}
		last_level = limit_cfg.max_level;

		if (!PugiGetSubNodeValue(data_element, "gongji_limit", limit_cfg.gongji_limit) || limit_cfg.gongji_limit < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "fangyu_limit", limit_cfg.fangyu_limit) || limit_cfg.fangyu_limit < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "maxhp_limit", limit_cfg.maxhp_limit) || limit_cfg.maxhp_limit < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "addper_limit", limit_cfg.addper_limit) || limit_cfg.addper_limit < 0)
		{
			return -7;
		}
		
		m_rune_awaken_count++;

		data_element = data_element.next_sibling();
	}

	return 0;
}

int RuneSystemCfg::InitAwakenCostCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_rune_awaken_cost_count = 0;
	int last_times = -1;
	while (!data_element.empty())
	{
		if (m_rune_awaken_cost_count < 0 || m_rune_awaken_cost_count >= RUNESYSTEM_AWAKEN_COST_MAX_COUNT)
		{
			return -1;
		}

		RuneAwakenCost &cost_cfg = m_rune_awaken_cost_list[m_rune_awaken_cost_count];

		if (!PugiGetSubNodeValue(data_element, "min_times", cost_cfg.min_times) || cost_cfg.min_times < 0 || cost_cfg.min_times != last_times + 1)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "max_times", cost_cfg.max_times) || cost_cfg.max_times < 0 || cost_cfg.max_times < cost_cfg.min_times)
		{
			return -3;
		}
		last_times = cost_cfg.max_times;

		if (!PugiGetSubNodeValue(data_element, "gold_cost", cost_cfg.gold_cost) || cost_cfg.gold_cost <= 0)
		{
			return -4;
		}

		PugiXmlNode item_element = data_element.child("common_awaken_item");
		if (item_element.empty())
		{
			return -5;
		}

		if (!cost_cfg.common_awaken_item.ReadConfig(item_element))
		{
			return -6;
		}

		m_rune_awaken_cost_count++;

		data_element = data_element.next_sibling();
	}

	return 0;
}

int RuneSystemCfg::InitZhulingGradeCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}
	bool has_index[RUNE_SYSTEM_SLOT_MAX_NUM];
	memset(has_index, false, sizeof(has_index));
	while (!data_element.empty())
	{
		RuneZhulingGradeConfig cfg;
		if (!PugiGetSubNodeValue(data_element, "index", cfg.fuwen_index) || cfg.fuwen_index < 0 || cfg.fuwen_index >= RUNE_SYSTEM_SLOT_MAX_NUM)
		{
			return -10001;
		}
		if (!has_index[cfg.fuwen_index])
		{
			has_index[cfg.fuwen_index] = true;
		}
		int grade = 0;
		if (!PugiGetSubNodeValue(data_element, "grade", grade) || grade < 0)
		{
			return -10002;
		}
		cfg.grade = grade;

		if (!PugiGetSubNodeValue(data_element, "need_bless", cfg.up_grade_need_bless) || cfg.up_grade_need_bless <= 0)
		{
			return -10003;
		}

		if (PugiGetSubNodeValue(data_element, "maxhp", cfg.maxhp) && cfg.maxhp < 0)
		{
			return -101;
		}
		if (PugiGetSubNodeValue(data_element, "gongji", cfg.gongji) && cfg.gongji < 0)
		{
			return -102;
		}
		if (PugiGetSubNodeValue(data_element, "fangyu", cfg.fangyu) && cfg.fangyu < 0)
		{
			return -103;
		}
		if (PugiGetSubNodeValue(data_element, "mingzhong", cfg.mingzhong) && cfg.mingzhong < 0)
		{
			return -104;
		}
		if (PugiGetSubNodeValue(data_element, "shanbi", cfg.shanbi) && cfg.shanbi < 0)
		{
			return -105;
		}
		if (PugiGetSubNodeValue(data_element, "baoji", cfg.baoji) && cfg.baoji < 0)
		{
			return -107;
		}
		if (PugiGetSubNodeValue(data_element, "jianren", cfg.jianren) && cfg.jianren < 0)
		{
			return -108;
		}

		if (PugiGetSubNodeValue(data_element, "special_add", cfg.special_add) && cfg.special_add < 0)
		{
			return -109;
		}

		if (PugiGetSubNodeValue(data_element, "is_broadcast", cfg.is_broadcast) && cfg.is_broadcast < 0)
		{
			return -110;
		}

		m_zhuling_grade_cfg_list.push_back(cfg);

		data_element = data_element.next_sibling();
	}
	//必须先读符文槽的配置再读本配置,这里检查是否已经读过
	if (m_slot_open_cfg_vec.size() == 0)
	{
		return -111;
	}
	//检查注灵的索引与符文槽的索引是否都配置过,避免计算属性的时候,因为读不到注灵的配置而跳过计算
	for (size_t i = 0; i < m_slot_open_cfg_vec.size() && i < RUNE_SYSTEM_SLOT_MAX_NUM; ++i)
	{
		if (!has_index[i])
		{
			return -1000 - i;
		}
	}
	return 0;
}

int RuneSystemCfg::InitZhulingSlotCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_zhuling_slot_cfg_total_weight = 0;
	m_zhuling_slot_cfg_count = 0;

	while (!data_element.empty())
	{
		if (m_zhuling_slot_cfg_count >= RUNESYSTEM_ZHULING_MAX_SLOT_CFG)
		{
			return -10001;
		}

		int index = 0;
		if (!PugiGetSubNodeValue(data_element, "index", index) || index != m_zhuling_slot_cfg_count)
		{
			return -10002;
		}

		RuneZhulingSlotConfig &cfg = m_zhuling_slot_cfg_list[index];
		if (!PugiGetSubNodeValue(data_element, "weight", cfg.weight) || cfg.weight <= 0)
		{
			return -10003;
		}
		m_zhuling_slot_cfg_total_weight += cfg.weight;

		if (!PugiGetSubNodeValue(data_element, "param1", cfg.param1) || cfg.param1 == 0)
		{
			return -10004;
		}

		if (!PugiGetSubNodeValue(data_element, "type", cfg.type) || cfg.type < 0)
		{
			return -10005;
		}

		++m_zhuling_slot_cfg_count;
		data_element = data_element.next_sibling();
	}

	if (m_zhuling_slot_cfg_total_weight <= 0 || m_zhuling_slot_cfg_count <= 0)
	{
		return -99999;
	}

	return 0;
}

int RuneSystemCfg::InitRuneLevelOpenCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int last_rune_level = 0;

	while (!data_element.empty())
	{
		if (m_rune_level_open_cfg_count >= RUNE_SYSTEM_RUNE_MAX_LEVEL)
		{
			return -10001;
		}

		RuneLevelOpenCfg &rune_level_open_cfg = rune_level_open_cfg_list[m_rune_level_open_cfg_count];

		if (!PugiGetSubNodeValue(data_element, "rune_level", rune_level_open_cfg.rune_level) || rune_level_open_cfg.rune_level <= last_rune_level)
		{
			return -1;
		}

		last_rune_level = rune_level_open_cfg.rune_level;

		if (!PugiGetSubNodeValue(data_element, "need_rune_tower_layer", rune_level_open_cfg.need_rune_tower_layer) || rune_level_open_cfg.need_rune_tower_layer < 0)
		{
			return -2;
		}

		++m_rune_level_open_cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

const RuneSystemSlotOpenCfg * RuneSystemCfg::GetSlotOpenCfg(int pass_layer)
{
	if (pass_layer < 0)
	{
		return NULL;
	}

	for (int i = (int)m_slot_open_cfg_vec.size() - 1; i >= 0; --i)
	{
		if (pass_layer >= m_slot_open_cfg_vec[i].need_pass_layer)
		{
			return &m_slot_open_cfg_vec[i];
		}
	}

	return NULL;
}

const RuneSystemRuneFetchCfg * RuneSystemCfg::GetRuneIdCfg(ItemID rune_id)
{
	RuneSystemRuneFetchCfgMapIt it = m_rune_id_cfg_map.find(rune_id);
	if (it != m_rune_id_cfg_map.end())
	{
		return &it->second;
	}

	return NULL;
}

const RuneSystemAttrCfg * RuneSystemCfg::GetAttrCfg(int quality, int type, int level)
{
	if (quality  < RUNE_QUALITY_KIND_WHITE || quality >= RUNE_QUALITY_KIND_MAX ||
		type < RUNE_TYPE_GONGJI || type >= RUNE_TYPE_MAX || level <= 0)
	{
		return NULL;
	}

	long long key = ConvertParamToLongLong(quality, type, level);
	RuneSystemAttrCfgMapIt it = m_attr_cfg_map.find(key);
	if (it != m_attr_cfg_map.end())
	{
		return &it->second;
	}

	return NULL;
}

const RuneSystemRealIdCfg * RuneSystemCfg::GetRealIdCfg(ItemID rune_id)
{
	RuneSystemRealIdCfgMapIt it = m_real_id_cfg_map.find(rune_id);
	if (it != m_real_id_cfg_map.end())
	{
		return &it->second;
	}

	return NULL;
}

const RuneSystemComposeCfg * RuneSystemCfg::GetComposeCfg(ItemID rune1_id, ItemID rune2_id)
{
	long long key = ConvertParamToLongLong(rune1_id, rune2_id);
	RuneSystemComposeCfgMapIt it = m_compose_cfg_map.find(key);
	if (it != m_compose_cfg_map.end())
	{
		return &it->second;
	}

	return NULL;
}

void RuneSystemCfg::GetOpenRuneId(int pass_layer, std::set<ItemID> &rune_id_set)
{
	for (RuneSystemRuneFetchCfgMapIt it = m_rune_id_cfg_map.begin(); it != m_rune_id_cfg_map.end(); ++it)
	{
		if (it->second.in_layer_open <= pass_layer)
		{
			rune_id_set.insert(it->first);
		}
	}
}

void RuneSystemCfg::GetRuneFetchWeightCfg(int pass_layer, std::vector<RuneSystemRuneFetchCfg> &rune_id_weight_vec)
{
	for (auto &it : m_rune_fetch_weight_vec)
	{
		if (it.in_layer_open <= pass_layer)
		{
			rune_id_weight_vec.emplace_back(it);
		}
	}
}

ItemID RuneSystemCfg::GetOpenBoxGetItemId(int box_type, int level_limit)
{
	if (box_type <= RUNE_BOX_TYPE_INVALID || box_type >= RUNE_BOX_TYPE_MAX)
	{
		return 0;
	}

	if (int(m_open_box_cfg_map.size()) <= 0)
	{
		return 0;
	}

	switch (box_type)
	{
	case RUNE_BOX_TYPE_JINGHUA:
	{
		int total_weight = 0;
		for (RuneSystemOpenBoxCfgMapIt it = m_open_box_cfg_map.begin(); it != m_open_box_cfg_map.end(); ++it)
		{
			if (it->second.in_layer_open <= level_limit)
			{
				total_weight += it->second.weight_jinghua_box;
			}
		}

		if (total_weight <= 0)
		{
			return 0;
		}

		int rand_num = RandomNum(total_weight);

		for (RuneSystemOpenBoxCfgMapIt it = m_open_box_cfg_map.begin(); it != m_open_box_cfg_map.end(); ++it)
		{
			if (it->second.in_layer_open <= level_limit)
			{
				if (rand_num < it->second.weight_jinghua_box)
				{
					return it->first;
				}
				else
				{
					rand_num -= it->second.weight_jinghua_box;
				}
			}
		}
	}
	break;

	case RUNE_BOX_TYPE_RUNE_AND_JINGHUA:
	{
		int total_weight = 0;
		for (RuneSystemOpenBoxCfgMapIt it = m_open_box_cfg_map.begin(); it != m_open_box_cfg_map.end(); ++it)
		{
			if (it->second.in_layer_open <= level_limit)
			{
				total_weight += it->second.weight_rune_box;
			}
		}

		if (total_weight <= 0)
		{
			return 0;
		}

		int rand_num = RandomNum(total_weight);

		for (RuneSystemOpenBoxCfgMapIt it = m_open_box_cfg_map.begin(); it != m_open_box_cfg_map.end(); ++it)
		{
			if (it->second.in_layer_open <= level_limit)
			{
				if (rand_num < it->second.weight_rune_box)
				{
					return it->first;
				}
				else
				{
					rand_num -= it->second.weight_rune_box;
				}
			}
		}
	}
	break;

	default:
		break;
	}

	return 0;
}

int RuneSystemCfg::GetLevelConsumeTotalJinghua(int quality, int type, int level)
{
	int jinghua = 0;
	for (int i = 1; i <= level; ++i)
	{
		long long key = ConvertParamToLongLong(quality, type, i);
		RuneSystemAttrCfgMapIt it = m_attr_cfg_map.find(key);
		if (it != m_attr_cfg_map.end())
		{
			jinghua += it->second.uplevel_need_jinghua;
		}
	}

	return jinghua;
}

int RuneSystemCfg::GetRuneIdByQualityAndTypeCfg(int quality, int type)
{
	ItemID rune_id = 0;
	for (RuneSystemRealIdCfgMapIt it = m_real_id_cfg_map.begin(); it != m_real_id_cfg_map.end(); ++it)
	{
		if (quality == it->second.quality && type == it->second.type)
		{
			rune_id = it->first;
		}
	}

	return rune_id;
}

RuneSystemAwakenLimitCfg * RuneSystemCfg::GetAwakenLimitByLevel(int level)
{
	if (level < 0) return NULL;

	for(int i = 0; i < m_rune_awaken_count && i < RUNESYSTEM_AWAKEN_LIMIT_MAX_COUNT; i++)
	{
		RuneSystemAwakenLimitCfg &limit_cfg = m_rune_awaken_limit_list[i];

		if (limit_cfg.min_level <= level && level <= limit_cfg.max_level)
		{
			return &limit_cfg;
		}
	}

	return NULL;
}

RuneAwakenCost * RuneSystemCfg::GetAwakenCostByTimes(int times)
{
	if (times < 0) return NULL;

	for (int i = 0; i < m_rune_awaken_cost_count && i < RUNESYSTEM_AWAKEN_COST_MAX_COUNT; i++)
	{
		RuneAwakenCost &cost_cfg = m_rune_awaken_cost_list[i];

		if (cost_cfg.min_times <= times && times <= cost_cfg.max_times)
		{
			return &cost_cfg;
		}
	}

	return NULL;
}

const RuneZhulingGradeConfig * RuneSystemCfg::GetZhulingGradeCfg(int fuwen_index, int grade) const
{
	if (grade < 0 || fuwen_index < 0 || fuwen_index >= RUNE_SYSTEM_SLOT_MAX_NUM) return NULL;

	for (int i = 0; i < (int)m_zhuling_grade_cfg_list.size(); ++ i)
	{
		if (m_zhuling_grade_cfg_list[i].fuwen_index == fuwen_index && m_zhuling_grade_cfg_list[i].grade == grade)
		{
			return &m_zhuling_grade_cfg_list[i];
		}
	}

	return NULL;
}

const RuneZhulingSlotConfig * RuneSystemCfg::GetZhulingSlotCfg(int index) const
{
	if (index < 0 || index >= m_zhuling_slot_cfg_count)
	{
		return NULL;
	}

	return  &m_zhuling_slot_cfg_list[index];
}

const RuneLevelOpenCfg * RuneSystemCfg::GetRuneLevelOpenCfg(int rune_level) const
{
	for (int i = 0; i < m_rune_level_open_cfg_count && i < RUNE_SYSTEM_RUNE_MAX_LEVEL; ++i)
	{
		if (rune_level < rune_level_open_cfg_list[i].rune_level)
		{
			return &rune_level_open_cfg_list[i];
		}
	}

	return nullptr;
}