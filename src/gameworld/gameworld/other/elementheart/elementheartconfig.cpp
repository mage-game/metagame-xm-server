#include "elementheartconfig.hpp"
#include "config/logicconfigmanager.hpp"
#include "item/itempool.h"

ElementHeartConfig::ElementHeartConfig() : m_element_index_cfg_count(0), m_element_wuxing_cfg_count(0), m_element_wuxing_max(0), m_element_grade_cfg_count(0), m_item_cfg_count(0), m_chout_total_weight(0), 
		m_chou_cfg_count(0), m_equip_max_level(0), m_shop_total_weight(0)
{
	memset(m_charm_inlay_cfg_list, 0, sizeof(m_charm_inlay_cfg_list));
}

ElementHeartConfig::~ElementHeartConfig()
{
}


bool ElementHeartConfig::Init(const std::string &configname, std::string *err)
{
	PUGI_XML_CONFIG_PRE_LOAD;

	PUGI_XML_LOAD_CONFIG("other", InitOtherCfg);
	PUGI_XML_LOAD_CONFIG("element_heart_openc_fg", InitElementHeartOpenCfg);
	PUGI_XML_LOAD_CONFIG("element_heart_level", InitElementHeartWuxingCfg);
	PUGI_XML_LOAD_CONFIG("element_heart_grade", InitElementHeartGradeCfg);
	PUGI_XML_LOAD_CONFIG("item_cfg", InitItemCfg);
	PUGI_XML_LOAD_CONFIG("chou_cfg", InitChouCfg);
	PUGI_XML_LOAD_CONFIG("inlay", InitCharmInlayCfg);
	PUGI_XML_LOAD_CONFIG("element_texture_level", InitElementTextureLevelCfg);
	PUGI_XML_LOAD_CONFIG("element_equip_shop", InitShopCfg);
	PUGI_XML_LOAD_CONFIG("element_equip_shuxing", InitEquipAttrCfg);
	PUGI_XML_LOAD_CONFIG("element_equip_level", InitEquipLevelCfg);
	PUGI_XML_LOAD_CONFIG("xilian_slot_open", InitXilianOpenCfg);
	PUGI_XML_LOAD_CONFIG("lock_consume", InitXilianLockCfg);
	PUGI_XML_LOAD_CONFIG("xilian_type", InitXilianAttrCfg);
	PUGI_XML_LOAD_CONFIG("xilian_consume", InitXilianConsumeCfg);
	PUGI_XML_LOAD_CONFIG("element_shuxing_addition", InitXilianExtraAttrCfg);
	PUGI_XML_LOAD_CONFIG("element_attr_rate", InitElementAttrRateCfg);
	PUGI_XML_LOAD_CONFIG("upgrade_limit", InitElementHeartUpgradeLimitCfg);

	return true;
}

const ElementHeartOpenCfg * ElementHeartConfig::GetElementIndexCfgById(int id) const
{
	if (id < 0 || id >= m_element_index_cfg_count)
	{
		return NULL;
	}

	if (m_element_index_cfg_list[id].id == id)
	{
		return &m_element_index_cfg_list[id];
	}

	return NULL;
}

const ElementHeartWuxingCfg * ElementHeartConfig::GetElementWuxingCfgByWuxing(int wuxingzhi) const
{
	for (int cfg_index = 0; cfg_index < m_element_wuxing_cfg_count; ++ cfg_index)
	{
		const ElementHeartWuxingCfg &cfg = m_element_wuxing_cfg_list[cfg_index];
		if (wuxingzhi >= cfg.wuxing_min && wuxingzhi <= cfg.wuxing_max) 
		{
			return &cfg;
		}
	}

	return NULL;
}

const ElementHeartGradeCfg * ElementHeartConfig::GetElementGradeCfgByGrade(int grade) const
{
	if (grade < 0 || grade >= m_element_grade_cfg_count)
	{
		return NULL;
	}

	if (m_element_grade_cfg_list[grade].grade == grade)
	{
		return &m_element_grade_cfg_list[grade];
	}

	return NULL;
}

const ElementHeartItemCfg * ElementHeartConfig::GetITemCfgById(int id) const
{
	std::map<int, ElementHeartItemCfg>::const_iterator it = m_item_map.find(id);
	if (it != m_item_map.end())
	{
		return &it->second;
	}

	return NULL;
}

const ElementHeartItemCfg * ElementHeartConfig::GetITemCfgByItemId(int id) const
{
	for (int index = 0; index < m_item_cfg_count; ++index)
	{
		if (m_item_cfg_list[index].item_id == id)
		{
			return &m_item_cfg_list[index];
		}
	}

	return NULL;
}

const ElementHeartChouCfg * ElementHeartConfig::GetChouCfgByIndex(int index) const
{
	if (index < 0 || index >= m_chou_cfg_count)
	{
		return NULL;
	}

	return &m_chou_cfg_list[index];
}

const ElementHeartItemCfg * ElementHeartConfig::GetItemCfg(int item_type, int wuxing_type, int quality) const
{
	std::map<int, ElementHeartItemCfg>::const_iterator it = m_item_map.begin();
	for (; it != m_item_map.end(); it++)
	{
		if (it->second.type == item_type && it->second.param1 == wuxing_type && it->second.param3 == quality)
		{
			return &it->second;
		}
	}

	return NULL;
}

const ElementHeartLevelCfg * ElementHeartConfig::GetElementTextureLevelCfg(int wuxing_type, int grade) const
{
	if (wuxing_type < ELEMENT_HEART_WUXING_TYPE_JIN || wuxing_type >= ELEMENT_HEART_WUXING_TYPE_MAX) return NULL;
	if (grade < 0 || grade > ELEMENT_TEXTURE_GRADE_CFG_COUNT) return NULL;

	const ElementHeartLevelCfg &element_texture_level_cfg = m_element_texture_level_cfg_list[wuxing_type][grade];
	if (element_texture_level_cfg.wuxing_type == wuxing_type && element_texture_level_cfg.grade == grade)
	{
		return &element_texture_level_cfg;
	}

	return NULL;
}

const int ElementHeartConfig::GetCharmWuxingType(int equipment_index)
{
	if (equipment_index < 0 || equipment_index >= ItemNamespace::MAX_EQUIPMENT_GRID_NUM) return 0;

	return m_charm_inlay_cfg_list[equipment_index];
}

int ElementHeartConfig::InitOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	if (!PugiGetSubNodeValue(data_element, "food_percent_same", m_other_cfg.food_percent_same) || m_other_cfg.food_percent_same < 0)
	{
		return -2;
	}

	if (!PugiGetSubNodeValue(data_element, "food_percent_promote", m_other_cfg.food_percent_promote) || m_other_cfg.food_percent_promote < 0)
	{
		return -3;
	}

	if (!PugiGetSubNodeValue(data_element, "food_percent_restraint", m_other_cfg.food_percent_restraint) || m_other_cfg.food_percent_restraint < 0)
	{
		return -4;
	}

	if (!PugiGetSubNodeValue(data_element, "food_percent_normal", m_other_cfg.food_percent_normal) || m_other_cfg.food_percent_normal < 0)
	{
		return -5;
	}

	if (!PugiGetSubNodeValue(data_element, "change_wuxing_type_need_gold", m_other_cfg.change_wuxing_type_need_gold) || m_other_cfg.change_wuxing_type_need_gold < 0)
	{
		return -6;
	}

	if (!PugiGetSubNodeValue(data_element, "ghost_product_up_seed_need_gold_per_min", m_other_cfg.ghost_product_up_seed_need_gold_per_min) || m_other_cfg.ghost_product_up_seed_need_gold_per_min <= 0)
	{
		return -8;
	}

	if (!PugiGetSubNodeValue(data_element, "one_chou_need_gold", m_other_cfg.one_chou_need_gold) || m_other_cfg.one_chou_need_gold <= 0)
	{
		return -9;
	}

	if (!PugiGetSubNodeValue(data_element, "ten_chou_need_gold", m_other_cfg.ten_chou_need_gold) || m_other_cfg.ten_chou_need_gold <= m_other_cfg.one_chou_need_gold)
	{
		return -10;
	}

	if (!PugiGetSubNodeValue(data_element, "ten_chou_need_item", m_other_cfg.ten_chou_need_item) || !ITEMPOOL->GetItem(m_other_cfg.ten_chou_need_item))
	{
		return -11;
	}

	if (!PugiGetSubNodeValue(data_element, "one_chou_free_chou_times", m_other_cfg.one_chou_free_chou_times) || m_other_cfg.one_chou_free_chou_times < 0)
	{
		return -12;
	}

	if (!PugiGetSubNodeValue(data_element, "inorder_interval", m_other_cfg.inorder_interval) || m_other_cfg.inorder_interval <= 0)
	{
		return -13;
	}
	
	if (!PugiGetSubNodeValue(data_element, "limit_level", m_other_cfg.limit_level) || m_other_cfg.limit_level <= 0)
	{
		return -14;
	}
	
	if (!PugiGetSubNodeValue(data_element, "ghost_product_up_seed_need_item_id", m_other_cfg.ghost_product_up_seed_need_item_id) || !ITEMPOOL->GetItem(m_other_cfg.ghost_product_up_seed_need_item_id))
	{
		return -15;
	}

	if (!PugiGetSubNodeValue(data_element, "one_chou_need_score", m_other_cfg.one_chou_need_score) || m_other_cfg.one_chou_need_score < 0)
	{
		return -16;
	}

	if (!PugiGetSubNodeValue(data_element, "ten_chou_need_score", m_other_cfg.ten_chou_need_score) || m_other_cfg.shop_refresh_need_gold < 0)
	{
		return -17;
	}

	if (!PugiGetSubNodeValue(data_element, "shop_refresh_need_gold", m_other_cfg.shop_refresh_need_gold) || m_other_cfg.ten_chou_need_score < 0)
	{
		return -18;
	}

	if (!PugiGetSubNodeValue(data_element, "shop_refresh_need_gold_bind", m_other_cfg.shop_refresh_need_gold_bind) || m_other_cfg.shop_refresh_need_gold_bind < 0)
	{
		return -19;
	}

	if (!PugiGetSubNodeValue(data_element, "shop_refresh_need_score", m_other_cfg.shop_refresh_need_score) || m_other_cfg.shop_refresh_need_score < 0)
	{
		return -20;
	}

	if (!PugiGetSubNodeValue(data_element, "shop_need_gold_buy_count", m_other_cfg.shop_need_gold_buy_count) || m_other_cfg.shop_need_gold_buy_count < 0)
	{
		return -21;
	}

	if (!PugiGetSubNodeValue(data_element, "shop_refresh_free_times", m_other_cfg.shop_refresh_free_times) || m_other_cfg.shop_refresh_free_times < 0)
	{
		return -22;
	}

	if (!PugiGetSubNodeValue(data_element, "shop_refresh_time_interval", m_other_cfg.shop_refresh_time_interval) || m_other_cfg.shop_refresh_time_interval <= 0)
	{
		return -23;
	}

	if (!PugiGetSubNodeValue(data_element, "one_chou_need_item", m_other_cfg.one_chou_need_item) || !ITEMPOOL->GetItem(m_other_cfg.one_chou_need_item))
	{
		return -24;
	}
	
	return 0;
}

int ElementHeartConfig::InitElementHeartOpenCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		if (m_element_index_cfg_count >= ELEMENT_HEART_MAX_COUNT)
		{
			return -1000;
		}

		int id = 0;
		if (!PugiGetSubNodeValue(data_element, "id", id) || id != m_element_index_cfg_count)
		{
			return -1001;
		}

		ElementHeartOpenCfg &cfg = m_element_index_cfg_list[id];
		cfg.id = id;

		if (!PugiGetSubNodeValue(data_element, "condtion_type", cfg.condition_type) || cfg.condition_type < ELEMENT_HEART_OPEN_CONDITION_TYPE_LAST_FEED_LEVEL ||
			cfg.condition_type > ELEMENT_HEART_OPEN_CONDITION_TYPE_GOLD)
		{
			return -1002;
		}

		if (!PugiGetSubNodeValue(data_element, "condtion", cfg.param) || cfg.param < 0)
		{
			return -1003;
		}

		++ m_element_index_cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int ElementHeartConfig::InitElementHeartWuxingCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_element_wuxing_cfg_count = 0;
	int last_wuxing_max = 0;
	while (!data_element.empty())
	{
		if (m_element_wuxing_cfg_count < 0 || m_element_wuxing_cfg_count >= ELEMENT_HEART_LEVEL_CFG_COUNT)
		{
			return -1001;
		}
		ElementHeartWuxingCfg &cfg = m_element_wuxing_cfg_list[m_element_wuxing_cfg_count];

		if (!PugiGetSubNodeValue(data_element, "level", cfg.feed_level) || cfg.feed_level != m_element_wuxing_cfg_count + 1)
		{
			return -1002;
		}
		
		if (!PugiGetSubNodeValue(data_element, "wuxing_min", cfg.wuxing_min) || cfg.wuxing_min <= last_wuxing_max)
		{
			return -1002;
		}

		if (!PugiGetSubNodeValue(data_element, "wuxing_max", cfg.wuxing_max) || cfg.wuxing_max <= cfg.wuxing_min)
		{
			return -1003;
		}
		last_wuxing_max = cfg.wuxing_max;

		if (!PugiGetSubNodeValue(data_element, "num", cfg.product_num) || cfg.product_num <= 0)
		{
			return -1004;
		}

		cfg.total_weight = 0;
		for (int index = 0; index < CHARM_GHOST_PRODUCT_NUM_MAX; ++ index)
		{
			char attr_name[30];
			SNPRINTF(attr_name, sizeof(attr_name), "weight_%d", index);
			if (!PugiGetSubNodeValue(data_element, attr_name, cfg.weight_list[index]) || cfg.weight_list[index] < 0)
			{
				return -1005;
			}

			cfg.total_weight += cfg.weight_list[index];
		}

		if (!PugiGetSubNodeValue(data_element, "maxhp", cfg.maxhp) || cfg.maxhp < 0)
		{
			return -1007;
		}

		if (!PugiGetSubNodeValue(data_element, "gongji", cfg.gongji) || cfg.gongji < 0)
		{
			return -1008;
		}

		if (!PugiGetSubNodeValue(data_element, "fangyu", cfg.fangyu) || cfg.fangyu < 0)
		{
			return -1009;
		}

		if (!PugiGetSubNodeValue(data_element, "mingzhong", cfg.mingzhong) || cfg.mingzhong < 0)
		{
			return -1010;
		}

		if (!PugiGetSubNodeValue(data_element, "shanbi", cfg.shanbi) || cfg.shanbi < 0)
		{
			return -1011;
		}

		if (!PugiGetSubNodeValue(data_element, "baoji", cfg.baoji) || cfg.baoji < 0)
		{
			return -1012;
		}

		if (!PugiGetSubNodeValue(data_element, "jianren", cfg.jianren) || cfg.jianren < 0)
		{
			return -1013;
		}

		++m_element_wuxing_cfg_count;
		data_element = data_element.next_sibling();
	}

	m_element_wuxing_max = last_wuxing_max;

	return 0;
}

int ElementHeartConfig::InitElementHeartGradeCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_element_grade_cfg_count = 1;
	while (!data_element.empty())
	{
		if (m_element_grade_cfg_count < 0 || m_element_grade_cfg_count > ELEMENT_HEART_GRADE_CFG_COUNT)
		{
			return -1001;
		}

		int grade = 0;
		if (!PugiGetSubNodeValue(data_element, "grade", grade) || grade != m_element_grade_cfg_count)
		{
			return -1002;
		}

		ElementHeartGradeCfg &cfg = m_element_grade_cfg_list[grade];
		cfg.grade = grade;

		if (!PugiGetSubNodeValue(data_element, "add_texture_percent_attr", cfg.add_texture_percent_attr) || 
			cfg.add_texture_percent_attr < 0 || cfg.add_texture_percent_attr > 10000)
		{
			return -1003;
		}

		if (!PugiGetSubNodeValue(data_element, "product_time_s", cfg.product_time_s) || cfg.product_time_s <= 0)
		{
			return -1004;
		}

		if (!PugiGetSubNodeValue(data_element, "item_id", cfg.item_id) || NULL == ITEMPOOL->GetItem(cfg.item_id))
		{
			return -1005;
		}

		if (!PugiGetSubNodeValue(data_element, "need_item_num", cfg.need_item_num) || cfg.need_item_num <= 0)
		{
			return -1006;
		}

		if (!PugiGetSubNodeValue(data_element, "bless_val_limit", cfg.bless_val_limit) || cfg.bless_val_limit <= 0)
		{
			return -1007;
		}

		if (!PugiGetSubNodeValue(data_element, "maxhp", cfg.maxhp) || cfg.maxhp < 0)
		{
			return -1008;
		}

		if (!PugiGetSubNodeValue(data_element, "gongji", cfg.gongji) || cfg.gongji < 0)
		{
			return -1009;
		}

		if (!PugiGetSubNodeValue(data_element, "fangyu", cfg.fangyu) || cfg.fangyu < 0)
		{
			return -1010;
		}

		if (!PugiGetSubNodeValue(data_element, "mingzhong", cfg.mingzhong) || cfg.mingzhong < 0)
		{
			return -1011;
		}

		if (!PugiGetSubNodeValue(data_element, "shanbi", cfg.shanbi) || cfg.shanbi < 0)
		{
			return -1012;
		}

		if (!PugiGetSubNodeValue(data_element, "baoji", cfg.baoji) || cfg.baoji < 0)
		{
			return -1013;
		}

		if (!PugiGetSubNodeValue(data_element, "jianren", cfg.jianren) || cfg.jianren < 0)
		{
			return -1014;
		}

		++ m_element_grade_cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int ElementHeartConfig::InitElementHeartUpgradeLimitCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int cfg_cout = 0;
	while (!data_element.empty())
	{
		int element_id = 0;
		if (!PugiGetSubNodeValue(data_element, "element_id", element_id) || element_id < 0 || element_id >= ELEMENT_HEART_MAX_COUNT || element_id != cfg_cout)
		{
			return -1;
		}
		
		if (!PugiGetSubNodeValue(data_element, "last_element_level", m_upgrade_limit_list[element_id]) || m_upgrade_limit_list[element_id] < 0 || 
			m_upgrade_limit_list[element_id] > ELEMENT_HEART_GRADE_CFG_COUNT)
		{
			return -2;
		}

		++cfg_cout;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int ElementHeartConfig::InitItemCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_item_cfg_count = 1;
	while (!data_element.empty())
	{
		if (m_item_cfg_count >= ELEMENT_HEART_ITEM_CFG_COUNT)
		{
			return -1001;
		}

		int item_id = 0;
		if (!PugiGetSubNodeValue(data_element, "item_id", item_id) || !ITEMPOOL->GetItem(item_id))
		{
			return -1003;
		}

		ElementHeartItemCfg &cfg = m_item_map[item_id];
		cfg.item_id = item_id;

		if (!PugiGetSubNodeValue(data_element, "type", cfg.type) || cfg.type <= ELEMENT_HEART_ITEM_TYPE_INVALID || cfg.type >= ELEMENT_HEART_ITEM_TYPE_MAX)
		{
			return -1004;
		}

		if (!PugiGetSubNodeValue(data_element, "param1", cfg.param1) || cfg.param1 < 0)
		{
			return -1005;
		}

		if (ELEMENT_HEART_ITEM_TYPE_FOOD == cfg.type)
		{
			if (cfg.param1 < ELEMENT_HEART_WUXING_TYPE_JIN || cfg.param1 >= ELEMENT_HEART_WUXING_TYPE_MAX)
			{
				return -1006;
			}
		}

		if (!PugiGetSubNodeValue(data_element, "param2", cfg.param2) || cfg.param2 < 0)
		{
			return -1007;
		}

		if (!PugiGetSubNodeValue(data_element, "param3", cfg.param3) || cfg.param3 < 0)
		{
			return -1008;
		}

		++m_item_cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int ElementHeartConfig::InitChouCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_chou_cfg_count = 0;
	m_chout_total_weight = 0;
	while (!data_element.empty())
	{
		if (m_chou_cfg_count < 0 || m_chou_cfg_count >= ELEMENT_HEART_ITEM_CFG_COUNT)
		{
			return  -1;
		}

		int seq = 0;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq != m_chou_cfg_count)
		{
			return -2;
		}

		ElementHeartChouCfg &cfg = m_chou_cfg_list[m_chou_cfg_count];
		cfg.seq = seq;

		{
			PugiXmlNode item_element = data_element.child("reward_item");
			if (item_element.empty())
			{
				return -3;
			}

			if (!cfg.reward_item.ReadConfig(item_element))
			{
				return -4;
			}
		}

		if (!PugiGetSubNodeValue(data_element, "weight", cfg.weight) || cfg.weight <= 0)
		{
			return -5;
		}
		m_chout_total_weight += cfg.weight;

		++m_chou_cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int ElementHeartConfig::InitCharmInlayCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int equipment_index = -1;
	int wuxing_type = 0;

	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "e_index", equipment_index) || equipment_index < 0 || equipment_index >= ItemNamespace::MAX_EQUIPMENT_GRID_NUM)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "wuxing_type", wuxing_type) || wuxing_type < ELEMENT_HEART_WUXING_TYPE_JIN || wuxing_type >= ELEMENT_HEART_WUXING_TYPE_MAX)
		{
			return -1;
		}

		m_charm_inlay_cfg_list[equipment_index] = wuxing_type;

		data_element = data_element.next_sibling();
	}

	return 0;
}

int ElementHeartConfig::InitElementTextureLevelCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int wuxing_type = 0;
	int last_wuxing_type = 0;
	int grade = 0;
	int last_grade = 0;

	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "wuxing_type", wuxing_type) || wuxing_type < ELEMENT_HEART_WUXING_TYPE_JIN || wuxing_type >= ELEMENT_HEART_WUXING_TYPE_MAX)
		{
			return -1;
		}

		if (wuxing_type != last_wuxing_type)
		{
			last_grade = 0;
		}
		last_wuxing_type = wuxing_type;

		if (!PugiGetSubNodeValue(data_element, "grade", grade) || grade != last_grade + 1 || grade > ELEMENT_TEXTURE_GRADE_CFG_COUNT)
		{
			return -2;
		}
		last_grade = grade;

		ElementHeartLevelCfg &element_texture_level_cfg = m_element_texture_level_cfg_list[wuxing_type][grade];
		element_texture_level_cfg.wuxing_type = wuxing_type;
		element_texture_level_cfg.grade = grade;

		if (!PugiGetSubNodeValue(data_element, "exp_limit", element_texture_level_cfg.exp_limit) || element_texture_level_cfg.exp_limit <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "maxhp", element_texture_level_cfg.maxhp) || element_texture_level_cfg.maxhp < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "gongji", element_texture_level_cfg.gongji) || element_texture_level_cfg.gongji < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "fangyu", element_texture_level_cfg.fangyu) || element_texture_level_cfg.fangyu < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "mingzhong", element_texture_level_cfg.mingzhong) || element_texture_level_cfg.mingzhong < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(data_element, "shanbi", element_texture_level_cfg.shanbi) || element_texture_level_cfg.shanbi < 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(data_element, "baoji", element_texture_level_cfg.baoji) || element_texture_level_cfg.baoji < 0)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(data_element, "jianren", element_texture_level_cfg.jianren) || element_texture_level_cfg.jianren < 0)
		{
			return -10;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

const ElementHeartShopConfig * ElementHeartConfig::GetShopCfg(int shop_seq)
{
	if (shop_seq >= 0 && shop_seq < (int)m_shop_cfg_list.size())
	{
		return &m_shop_cfg_list[shop_seq];
	}

	return NULL;
}

int ElementHeartConfig::InitEquipLevelCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_equip_max_level = -1;
	while (!data_element.empty())
	{
		if (m_equip_max_level >= ELEMENT_HEART_EQUIP_MAX_LEVEL)
		{
			return -1000;
		}

		int real_level = 0;
		if (!PugiGetSubNodeValue(data_element, "real_level", real_level) || real_level != m_equip_max_level + 1)
		{
			return -1;
		}

		ElementHeartEquipLevelCfg &level_cfg = m_equip_level_cfg_list[real_level];
		level_cfg.real_level = real_level;
		m_equip_max_level = real_level;

		if (!PugiGetSubNodeValue(data_element, "attr_total_percent", level_cfg.attr_total_percent) || level_cfg.attr_total_percent < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "comsume_item_id", level_cfg.comsume_item_id) || level_cfg.comsume_item_id < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "comsume_item_num", level_cfg.comsume_item_num) || level_cfg.comsume_item_num <= 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "upgrade_progress", level_cfg.upgrade_progress) || level_cfg.upgrade_progress <= 0)
		{
			return -5;
		}

		char element_name[20];
		for (int grid_index = 0; grid_index < ELEMENT_HEART_EQUIP_MAX_COUNT; ++grid_index)
		{
			SNPRINTF(element_name, sizeof(element_name), "grid_%d_id", grid_index);
			if (PugiGetSubNodeValue(data_element, element_name, level_cfg.item_id_list[grid_index]))
			{
				if (level_cfg.item_id_list[grid_index] <= 0 || NULL == ITEMPOOL->GetItem(level_cfg.item_id_list[grid_index]))
				{
					return -6;
				}
			}
		}

		if (!PugiGetSubNodeValue(data_element, "maxhp", level_cfg.maxhp) || level_cfg.maxhp < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(data_element, "gongji", level_cfg.gongji) || level_cfg.gongji < 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(data_element, "fangyu", level_cfg.fangyu) || level_cfg.fangyu < 0)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(data_element, "mingzhong", level_cfg.mingzhong) || level_cfg.mingzhong < 0)
		{
			return -10;
		}

		if (!PugiGetSubNodeValue(data_element, "shanbi", level_cfg.shanbi) || level_cfg.shanbi < 0)
		{
			return -11;
		}

		if (!PugiGetSubNodeValue(data_element, "baoji", level_cfg.baoji) || level_cfg.baoji < 0)
		{
			return -12;
		}

		if (!PugiGetSubNodeValue(data_element, "jianren", level_cfg.jianren) || level_cfg.jianren < 0)
		{
			return -13;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int ElementHeartConfig::InitEquipAttrCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_equip_max_level = -1;
	while (!data_element.empty())
	{
		ElemenEquipAttrConfig temp_cfg;
		if (!PugiGetSubNodeValue(data_element, "item_id", temp_cfg.item_id) || NULL == ITEMPOOL->GetItem(temp_cfg.item_id))
		{
			return -1;
		}

		{
			PugiXmlNode item_element = data_element.child("reward_item");
			if (item_element.empty())
			{
				return -4;
			}

			if (!temp_cfg.reward_item.ReadConfig(item_element))
			{
				return -5;
			}
		}

		if (!PugiGetSubNodeValue(data_element, "maxhp", temp_cfg.maxhp) || temp_cfg.maxhp < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "gongji", temp_cfg.gongji) || temp_cfg.gongji < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "fangyu", temp_cfg.fangyu) || temp_cfg.fangyu < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "mingzhong", temp_cfg.mingzhong) || temp_cfg.mingzhong < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(data_element, "shanbi", temp_cfg.shanbi) || temp_cfg.shanbi < 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(data_element, "baoji", temp_cfg.baoji) || temp_cfg.baoji < 0)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(data_element, "jianren", temp_cfg.jianren) || temp_cfg.jianren < 0)
		{
			return -10;
		}

		m_equip_attr_vec.push_back(temp_cfg);

		data_element = data_element.next_sibling();
	}

	return 0;
}

const ElementHeartEquipLevelCfg * ElementHeartConfig::GetEquipLevelCfg(int level)
{
	if (level < 0 || level > m_equip_max_level)
	{
		return NULL;
	}

	return &m_equip_level_cfg_list[level];
}

int ElementHeartConfig::InitShopCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_shop_cfg_list.clear();
	m_shop_total_weight = 0;
	while (!data_element.empty())
	{
		ElementHeartShopConfig shop_cfg;
		if (!PugiGetSubNodeValue(data_element, "seq", shop_cfg.seq) || shop_cfg.seq != (int)m_shop_cfg_list.size())
		{
			return -1;
		}

		{
			PugiXmlNode item_element = data_element.child("reward_item");
			if (item_element.empty())
			{
				return -2;
			}

			if (!shop_cfg.reward_item.ReadConfig(item_element))
			{
				return -3;
			}
		}

		if (!PugiGetSubNodeValue(data_element, "weight", shop_cfg.weight) || shop_cfg.weight < 0)
		{
			return -4;
		}
		m_shop_total_weight += shop_cfg.weight;

		if (!PugiGetSubNodeValue(data_element, "cost_gold_bind", shop_cfg.cost_gold_bind) || shop_cfg.cost_gold_bind < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "cost_gold", shop_cfg.cost_gold) || shop_cfg.cost_gold < 0)
		{
			return -6;
		}

		m_shop_cfg_list.push_back(shop_cfg);
		data_element = data_element.next_sibling();
	}

	if ((int)m_shop_cfg_list.size() < ELEMENT_SHOP_ITEM_COUNT)
	{
		return -2000;
	}

	return 0;
}

void ElementHeartConfig::GetRandomShopItem(ElementHeartShopItem shop_item_list[ELEMENT_SHOP_ITEM_COUNT]) const
{
	int total_weight = m_shop_total_weight;
	std::vector<ElementHeartShopConfig> cfg_list = m_shop_cfg_list;

	for (int item_index = 0; item_index < ELEMENT_SHOP_ITEM_COUNT; ++item_index)
	{
		int rand_value = RandomNum(total_weight);
		for (std::vector<ElementHeartShopConfig>::iterator cfg_it = cfg_list.begin(); cfg_it != cfg_list.end(); ++cfg_it)
		{
			if (rand_value < cfg_it->weight)
			{
				shop_item_list[item_index].shop_seq = cfg_it->seq;
				shop_item_list[item_index].has_buy = 0;
				shop_item_list[item_index].need_gold_buy = 1;

				break;
			}

			rand_value -= cfg_it->weight;
		}
	}
}

const ElemenEquipAttrConfig * ElementHeartConfig::GetEquipAttrCfg(int item_id)
{
	std::vector<ElemenEquipAttrConfig>::iterator it = m_equip_attr_vec.begin();
	for (; it != m_equip_attr_vec.end(); it ++)
	{
		if (item_id == it->item_id)
		{
			return &(*it);
		}
	}

	return NULL;
}

int ElementHeartConfig::InitXilianLockCfg(PugiXmlNode RootElement)
{
 	PugiXmlNode data_element = RootElement.child("data");
 	if (data_element.empty())
 	{
 		return -10000;
 	}
 
 	while (!data_element.empty())
 	{
 		int comsume_color = 0;
 		if (!PugiGetSubNodeValue(data_element, "consume_color", comsume_color) || comsume_color < 0 )
 		{
 			return -1;
 		}
 
 		ElementHeartXilianLockCfg lock_consume_cfg;
 		lock_consume_cfg.comsume_color = comsume_color;
 
 		//int lock_num = 0;
 		if (!PugiGetSubNodeValue(data_element, "lock_num", lock_consume_cfg.lock_num) || lock_consume_cfg.lock_num < 0 || lock_consume_cfg.lock_num > ELEMENT_HEART_MAX_XILIAN_SLOT)
 		{
 			return -2;
 		}
 
 		//ElementHeartXilianLockCfg &cfg = m_xilian_lock_cfg[lock_num];
 		//cfg.lock_num = lock_num;
 
 		//{
 		//	PugiXmlNode item_element = data_element.child("lock_comsume_item");
 		//	if (item_element.empty())
 		//	{
 		//		return -4;
 		//	}
 
 		//	if (!cfg.lock_comsume_item.ReadConfig(item_element) && cfg.lock_comsume_item.item_id != 0)
 		//	{
 		//		return -5;
 		//	}
 		//}
 
 		{
 			PugiXmlNode item_element = data_element.child("lock_comsume_item");
 			if (item_element.empty())
 			{
 				return -3;
 			}
 
 			if (!lock_consume_cfg.lock_comsume_item.ReadConfig(item_element) && lock_consume_cfg.lock_comsume_item.item_id != 0)
 			{
 				return -4;
 			}
 		}
 
 		m_lock_consume_vct.push_back(lock_consume_cfg);
 		data_element = data_element.next_sibling();
 	}

	return 0;
}

const ElementHeartXilianLockCfg * ElementHeartConfig::GetXilianLockCfg(int consume_color, int lock_num)
{
	if (lock_num < 0 || lock_num >= ELEMENT_HEART_MAX_XILIAN_SLOT) return NULL;

	//ElementHeartXilianLockCfg &cfg = m_xilian_lock_cfg[lock_num];
	//if (cfg.lock_num == lock_num)
	//{
	//	return &cfg;
	//}

	for (int i = 0; i < static_cast<int>(m_lock_consume_vct.size()); i++)
	{
		if (m_lock_consume_vct[i].comsume_color == consume_color && m_lock_consume_vct[i].lock_num == lock_num)
		{
			return &m_lock_consume_vct[i];
		}
	}

	return NULL;
}

int ElementHeartConfig::InitXilianConsumeCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	memset(m_xilian_consuem_total_weight, 0, sizeof(m_xilian_consuem_total_weight));
	while (!data_element.empty())
	{
		int color = 0;
		if (!PugiGetSubNodeValue(data_element, "comsume_color", color) || color < EH_COUMME_COLOR_INVALID || color >= EH_COUMME_COLOR_MAX)
		{
			return -1;
		}

		int &xilian_total_weight = m_xilian_consuem_total_weight[color];
		ElementHeartXilianComsumeCfg &cfg = m_xilian_consume_cfg[color];
		cfg.comsume_color = color;

		{
			PugiXmlNode item_element = data_element.child("consume_item");
			if (item_element.empty())
			{
				return -4;
			}

			if (!cfg.consume_item.ReadConfig(item_element) && cfg.consume_item.item_id != 0)
			{
				return -5;
			}

			if (!PugiGetSubNodeValue(data_element, "max_star_number", cfg.max_star_number) || cfg.max_star_number < 0)
			{
				return -6;
			}
		}

		char tem_str[64] = { 0 };
		for (int i = 0; i < ELEMENT_HEART_XILIAN_MAX_STAR_COUNT; i++)
		{
			sprintf(tem_str, "star_weight_%d", i);
			if (!PugiGetSubNodeValue(data_element, tem_str, cfg.star_weight[i]) || cfg.star_weight[i] < 0)
			{
				return -200;
			}

			xilian_total_weight += cfg.star_weight[i];
		}

		if (xilian_total_weight <= 0)
		{
			return -300;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int ElementHeartConfig::InitXilianOpenCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int last_id = -1;
	int last_slot = -1;
	while (!data_element.empty())
	{
		int element_id = 0;
		int slot = 0;
		if (!PugiGetSubNodeValue(data_element, "element_id", element_id) || element_id < 0 || element_id >= ELEMENT_HEART_MAX_COUNT || (last_id != element_id && element_id != last_id + 1))
		{
			return -1;
		}

		if (last_id != element_id)
		{
			last_slot = -1;
		}
		last_id = element_id;

		if (!PugiGetSubNodeValue(data_element, "slot_id", slot) || slot < 0 || slot >= ELEMENT_HEART_MAX_XILIAN_SLOT || slot != last_slot + 1)
		{
			return -2;
		}
		last_slot = slot;

		int &feed_level = m_xilian_open_level_cfg[element_id][slot];
		if (!PugiGetSubNodeValue(data_element, "feed_level", feed_level) || feed_level < 0)
		{
			return -3;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int ElementHeartConfig::InitXilianAttrCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		int element_id = 0;
		int slot = 0;
		if (!PugiGetSubNodeValue(data_element, "element_id", element_id) || element_id < 0 || element_id >ELEMENT_HEART_MAX_COUNT)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "xilian_solt", slot) || slot < 0 || slot > ELEMENT_HEART_MAX_XILIAN_SLOT)
		{
			return -3;
		}
		long long key = ConvertParamToLongLong(element_id, slot);

		if (m_xilian_attr_cfg_map.find(key) != m_xilian_attr_cfg_map.end())
		{
			return -4;
		}

		EHXilianShuxingCfg &cfg = m_xilian_attr_cfg_map[key];

		if (!PugiGetSubNodeValue(data_element, "shuxing_type", cfg.attr_type) || cfg.attr_type <= EH_XL_SHUXING_TYPE_INVALID || cfg.attr_type >= EH_XL_SHUXING_TYPE_MAX)
		{
			return -5;
		}

		char tem_str[64] = { 0 };
		for (int i = 0; i < ELEMENT_HEART_XILIAN_MAX_STAR_COUNT; i++)
		{
			sprintf(tem_str, "star_min_%d", i);
			if (!PugiGetSubNodeValue(data_element, tem_str, cfg.star_min[i]) || cfg.star_min[i] < 0)
			{
				return -6;
			}

			sprintf(tem_str, "star_max_%d", i);
			if (!PugiGetSubNodeValue(data_element, tem_str, cfg.star_max[i]) || cfg.star_max[i] < 0)
			{
				return -7;
			}
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int ElementHeartConfig::InitXilianExtraAttrCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		int attr_type = 0;
		if (!PugiGetSubNodeValue(data_element, "element_shuxing_type", attr_type) || attr_type < ELEMENT_HEART_WUXING_TYPE_JIN || attr_type >= ELEMENT_HEART_WUXING_TYPE_MAX)
		{
			return -1;
		}

		EHXilianExtraNeedCfg need_cfg;
		if (!PugiGetSubNodeValue(data_element, "need_element_shuxing_count", need_cfg.need_count) || need_cfg.need_count < 0 || need_cfg.need_count > ELEMENT_HEART_MAX_XILIAN_SLOT)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "add_percent", need_cfg.add_percent) || need_cfg.add_percent < 0)
		{
			return -3;
		}

		m_extra_cfg_vec[attr_type].push_back(need_cfg);

		data_element = data_element.next_sibling();
	}

	return 0;
}

int ElementHeartConfig::InitElementAttrRateCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		int element_id = 0;
		if (!PugiGetSubNodeValue(data_element, "element_id", element_id) || element_id < 0 || element_id >= ELEMENT_HEART_MAX_COUNT)
		{
			return -1;
		}

		AttrRateItem temp_cfg;
		if (!PugiGetSubNodeValue(data_element, "element_attr_type", temp_cfg.attr_type) || temp_cfg.attr_type < 0 || temp_cfg.attr_type >= ELEMENT_HEART_WUXING_TYPE_MAX)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "weight", temp_cfg.weight) || temp_cfg.weight < 0)
		{
			return -3;
		}

		m_element_rate_list[element_id].total_weight += temp_cfg.weight;
		m_element_rate_list[element_id].attr_list.push_back(temp_cfg);

		data_element = data_element.next_sibling();
	}

	for (int i = 0; i < ELEMENT_HEART_MAX_COUNT; i ++)
	{
		if (100 != m_element_rate_list[i].total_weight)
		{
			return -4;
		}
	}

	return 0;
}

const int ElementHeartConfig::GetExtraAttrAddPercent(int attr_type, int count)
{
	if (attr_type < ELEMENT_HEART_WUXING_TYPE_JIN || attr_type >= ELEMENT_HEART_WUXING_TYPE_MAX
		|| count <= 0 || count > ELEMENT_HEART_MAX_XILIAN_SLOT)
	{
		return 0;
	}

	int add_percent = 0;
	std::vector<EHXilianExtraNeedCfg>::const_iterator it = m_extra_cfg_vec[attr_type].begin();
	for (;it != m_extra_cfg_vec[attr_type].end(); it ++)
	{
		if (count >= it->need_count)
		{
			add_percent = it->add_percent;
		}
	}

	return add_percent;
}

const int ElementHeartConfig::GetXiLianSlotOpenLevel(int element_id, int slot_id)
{
	if (element_id < 0 || element_id >= ELEMENT_HEART_MAX_COUNT || slot_id < 0 || slot_id >= ELEMENT_HEART_MAX_XILIAN_SLOT)
	{
		return 0;
	}

	return m_xilian_open_level_cfg[element_id][slot_id];
}

const EHXilianShuxingCfg *ElementHeartConfig::GetXiLianAttrCfg(int element_id, int slot_id)
{
	if (element_id < 0 || element_id >= ELEMENT_HEART_MAX_COUNT || slot_id < 0 || slot_id >= ELEMENT_HEART_MAX_XILIAN_SLOT)
	{
		return NULL;
	}

	long long key = ConvertParamToLongLong(element_id, slot_id);
	std::map<long long, EHXilianShuxingCfg>::iterator it = m_xilian_attr_cfg_map.find(key);
	if (it != m_xilian_attr_cfg_map.end())
	{
		return &it->second;
	}

	return NULL;
}

const ElementHeartXilianComsumeCfg * ElementHeartConfig::GetXilianConsumeCfg(int color)
{
	if (color < EH_COUMME_COLOR_INVALID || color >= EH_COUMME_COLOR_MAX) return NULL;

	ElementHeartXilianComsumeCfg &cfg = m_xilian_consume_cfg[color];
	if (cfg.comsume_color == color)
	{
		return &cfg;
	}

	return NULL;
}

const int ElementHeartConfig::GetXilianConsuemeTotalWeight(int color)
{
	if (color < EH_COUMME_COLOR_INVALID || color >= EH_COUMME_COLOR_MAX) return 0;

	return m_xilian_consuem_total_weight[color];
}

const ElementAttrRateCfg *ElementHeartConfig::GetElementAttrRateCfg(int element_id)
{
	if (element_id < 0 || element_id >= ELEMENT_HEART_MAX_COUNT)
	{
		return NULL;
	}

	return &m_element_rate_list[element_id];
}

const int ElementHeartConfig::GetRandElementAttr(int element_id)
{
	if (element_id < 0 || element_id >= ELEMENT_HEART_MAX_COUNT)
	{
		return 0;
	}

	const ElementAttrRateCfg &temp_cfg = m_element_rate_list[element_id];
	if (m_element_rate_list[element_id].total_weight <= 0)
	{
		return 0;
	}

	int rand_val = RandomNum(m_element_rate_list[element_id].total_weight);
	for (std::vector<AttrRateItem>::const_iterator it = temp_cfg.attr_list.begin(); it != temp_cfg.attr_list.end(); it++)
	{
		if (it->weight > rand_val)
		{
			return it->attr_type;
		}

		rand_val -= it->weight;
	}

	return 0;
}

const int ElementHeartConfig::GetElementUpgradeLimit(int element_id)
{
	if (element_id < 0 || element_id >= ELEMENT_HEART_MAX_COUNT)
	{
		return -1;
	}

	return m_upgrade_limit_list[element_id];
}