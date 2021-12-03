#include "littlepetconfig.hpp"
#include "item/itempool.h"
#include <servercommon/configcommon.h>

LittlePetCfg::LittlePetCfg() : m_base_cfg_count(0), m_chou_cfg_count(0), m_chou_total_weight(0), m_exchange_cfg_count(0)
{
}

LittlePetCfg::~LittlePetCfg()
{
}

bool LittlePetCfg::Init(std::string configname, std::string *err)
{
	PUGI_XML_CONFIG_PRE_LOAD;

	//小宠物其他配置
	PUGI_XML_LOAD_CONFIG("other", InitOtherCfg);

	//小宠物各品质属性生成配置
//	PUGI_XML_LOAD_CONFIG("quality_cfg", InitQualityCfg);

	//小宠物基础配置
	PUGI_XML_LOAD_CONFIG("little_pet", InitLittlePetBaseCfg);

	//小宠物强化属性生成配置
//	PUGI_XML_LOAD_CONFIG("qianghua_cfg", InitStrengthenCfg);

	//小宠物抽奖配置
	PUGI_XML_LOAD_CONFIG("chou_cfg", InitChouCfg);

	//兑换配置
	PUGI_XML_LOAD_CONFIG("exchange", InitExchangeCfg);

	//小宠物喂养配置
	PUGI_XML_LOAD_CONFIG("little_pet_feed", InitFeedCfg);

	// 小宠物装备配置
	PUGI_XML_LOAD_CONFIG("little_pet_equipment_uplevel", InitEquipUplevelCfg);

	return true;
}

int	LittlePetCfg::InitOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	if (!PugiGetSubNodeValue(data_element, "feed_need_gold", m_other_cfg.feed_need_gold) || m_other_cfg.feed_need_gold <= 0)
	{
		return -1004;
	}

	if (!PugiGetSubNodeValue(data_element, "per_interact_count", m_other_cfg.per_pet_interact_times) || m_other_cfg.per_pet_interact_times <= 0)
	{
		return -1005;
	}

	if (!PugiGetSubNodeValue(data_element, "total_interact_count", m_other_cfg.per_user_total_interact_times) || m_other_cfg.per_user_total_interact_times <= 0)
	{
		return -1006;
	}

	if (!PugiGetSubNodeValue(data_element, "feed_clear_interval_h", m_other_cfg.feed_clear_interval_h) || m_other_cfg.feed_clear_interval_h <= 0)
	{
		return -1007;
	}

	if (!PugiGetSubNodeValue(data_element, "fangsheng_grid_add_num", m_other_cfg.fangsheng_grid_add_num) || m_other_cfg.fangsheng_grid_add_num < 0)
	{
		return -1008;
	}

	if (!PugiGetSubNodeValue(data_element, "feed_value", m_other_cfg.feed_value) || m_other_cfg.feed_value <= 0)
	{
		return -1009;
	}

	if (!PugiGetSubNodeValue(data_element, "per_interact_add_score", m_other_cfg.per_interact_add_score) || m_other_cfg.per_interact_add_score <= 0)
	{
		return -1010;
	}

	if (!PugiGetSubNodeValue(data_element, "one_chou_consume_gold", m_other_cfg.one_chou_consume_gold) || m_other_cfg.one_chou_consume_gold <= 0)
	{
		return -1011;
	}

	if (!PugiGetSubNodeValue(data_element, "ten_chou_consume_gold", m_other_cfg.ten_chou_consume_gold) || m_other_cfg.ten_chou_consume_gold <= 0)
	{
		return -1012;
	}

	if (!PugiGetSubNodeValue(data_element, "free_chou_interval_h", m_other_cfg.free_chou_interval_h) || m_other_cfg.free_chou_interval_h <= 0)
	{
		return -1013;
	}

	if (!PugiGetSubNodeValue(data_element, "rename_consume", m_other_cfg.rename_consume) || m_other_cfg.rename_consume <= 0)
	{
		return -1014;
	}

	{
		PugiXmlNode item_data = data_element.child("first_reward_item_cfg");
		if (item_data.empty())
		{
			return -1015;
		}
		if (!m_other_cfg.first_reward_item_cfg.ReadConfig(item_data))
		{
			return -1016;
		}
	}

	if (!PugiGetSubNodeValue(data_element, "choujiang_get_score", m_other_cfg.choujiang_get_score) || m_other_cfg.choujiang_get_score < 0)
	{
		return -1017;
	}

	if (!PugiGetSubNodeValue(data_element, "replacement_id", m_other_cfg.replacement_id) || NULL == ITEMPOOL->GetItem(m_other_cfg.replacement_id))
	{
		return -1018;
	}

	if (!PugiGetSubNodeValue(data_element, "consume_replacement", m_other_cfg.consume_replacement) || m_other_cfg.consume_replacement < 0)
	{
		return -1019;
	}

	if (!PugiGetSubNodeValue(data_element, "open_box_30_use_itemid", m_other_cfg.ten_chou_use_itemid) || NULL == ITEMPOOL->GetItem(m_other_cfg.ten_chou_use_itemid))
	{
		return -1020;
	}

	if (!PugiGetSubNodeValue(data_element, "open_box_30_use_item_num", m_other_cfg.ten_chou_use_item_num) || m_other_cfg.ten_chou_use_item_num < 0)
	{
		return -1021;
	}

	return 0;
}

int LittlePetCfg::InitQualityCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int last_quality_type = -1;

	while (!data_element.empty())
	{
		int quality_type = 0;
		if (!PugiGetSubNodeValue(data_element, "quality_type", quality_type) || quality_type != last_quality_type + 1)
		{
			return -1101;
		}

		if (quality_type < 0 || quality_type >= LITTLEPET_MAX_QUALITY_TYPE)
		{
			return -1102;
		}

		last_quality_type = quality_type;

		LittlePetQualityCfg & quality_cfg = m_quality_cfg[quality_type];
		quality_cfg.quality_type = quality_type;

		if (!PugiGetSubNodeValue(data_element, "grid_num", quality_cfg.grid_num) || quality_cfg.grid_num < 0)
		{
			return -1103;
		}

		if (!PugiGetSubNodeValue(data_element, "max_feed_degree", quality_cfg.max_feed_degree) || quality_cfg.max_feed_degree < 0)
		{
			return -1104;
		}

		if (!PugiGetSubNodeValue(data_element, "qianghua_need_item_id", quality_cfg.qianghua_need_item_id) || NULL == ITEMPOOL->GetItem(quality_cfg.qianghua_need_item_id))
		{
			return -1105;
		}

		if (!PugiGetSubNodeValue(data_element, "qianghua_need_item_num", quality_cfg.qianghua_need_item_num) || quality_cfg.qianghua_need_item_num <= 0)
		{
			return -1106;
		}

		if (!PugiGetSubNodeValue(data_element, "is_specail", quality_cfg.is_specail) || quality_cfg.is_specail < 0)
		{
			return -1107;
		}
		if (!PugiGetSubNodeValue(data_element, "add_attr_percent", quality_cfg.add_attr_percent) || quality_cfg.add_attr_percent < 0)
		{
			return -1108;
		}

		char attr_name[100];
		for (int base_value_index = 0; base_value_index < LITTLEPET_VALUE_TYPE_MAX_NUM; ++base_value_index)
		{
			LittlePetBirthValueCfg & birth_cfg = quality_cfg.base_value_cfg[base_value_index];
			birth_cfg.value_type = base_value_index;

			SNPRINTF(attr_name, 100, "min_%d", base_value_index);
			if (!PugiGetSubNodeValue(data_element, attr_name, birth_cfg.value_min) || birth_cfg.value_min < 0)
			{
				return -1200;
			}

			SNPRINTF(attr_name, 100, "max_%d", base_value_index);
			if (!PugiGetSubNodeValue(data_element, attr_name, birth_cfg.value_max) || birth_cfg.value_max < birth_cfg.value_min)
			{
				return -1201;
			}
		}

		data_element = data_element.next_sibling();
	}

	if (LITTLEPET_MAX_QUALITY_TYPE != last_quality_type)
	{
		return -1300;
	}

	return 0;
}

int LittlePetCfg::InitLittlePetBaseCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_base_cfg_count = 0;

	while (!data_element.empty())
	{
		if (m_base_cfg_count < 0 || m_base_cfg_count >= LITTLEPET_MAX_NUM)
		{
			return -1001;
		}

		int little_pet_id = -1;

		if (!PugiGetSubNodeValue(data_element, "id", little_pet_id) || little_pet_id != m_base_cfg_count + 1)
		{
			return -1003;
		}

		LittlePetBaseCfg &pet_cfg = m_base_cfg[little_pet_id];
		pet_cfg.little_pet_id = little_pet_id;

		if (!PugiGetSubNodeValue(data_element, "active_item_id", pet_cfg.active_item_id) || NULL == ITEMPOOL->GetItem(pet_cfg.active_item_id))
		{
			return -1004;
		}

		if (!PugiGetSubNodeValue(data_element, "quality_type", pet_cfg.quality_type) || pet_cfg.quality_type < 0 || pet_cfg.quality_type >= LITTLEPET_MAX_QUALITY_TYPE)
		{
			return -1005;
		}

		if (!PugiGetSubNodeValue(data_element, "using_img_id", pet_cfg.using_img_id) || pet_cfg.using_img_id < 0)
		{
			return -1006;
		}

		{
			PugiXmlNode item_data = data_element.child("return_item");
			if (item_data.empty())
			{
				return -1101;
			}
			if (!pet_cfg.return_item.ReadConfig(item_data))
			{
				return -1102;
			}
		}

		std::string name;
		if (!PugiGetSubNodeValue(data_element, "name", name))
		{
			return -5;
		}
		if (name.length() >= sizeof(pet_cfg.pet_name))
		{
			return -6;
		}
		memcpy(pet_cfg.pet_name, name.c_str(), (int)name.length());
		pet_cfg.pet_name[name.length()] = '\0';

		{
			char attr_name[64] = { 0 };

			for (int base_value_index = 0; base_value_index < LITTLEPET_VALUE_TYPE_MAX_NUM; ++base_value_index)
			{
				SNPRINTF(attr_name, 64, "attr_value_%d", base_value_index);
				if (!PugiGetSubNodeValue(data_element, attr_name, pet_cfg.attr_list[base_value_index]) || pet_cfg.attr_list[base_value_index] < 0)
				{
					return -1200;
				}
			}
		}

		++m_base_cfg_count;
		data_element = data_element.next_sibling();
	}

	if (m_base_cfg_count <= 0)
	{
		return -1100;
	}

	return 0;
}

int LittlePetCfg::InitStrengthenCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int last_quality_type = -1;
	int point_index = 0;
	while (!data_element.empty())
	{
		int quality_type = 0;
		if (!PugiGetSubNodeValue(data_element, "quality_type", quality_type) || quality_type < 0 || quality_type > LITTLEPET_MAX_QUALITY_TYPE)
		{
			return -9901;
		}

		if (last_quality_type != quality_type)
		{
			point_index = 0;
		}

		if (point_index >= LITTLEPET_QIANGHUAPOINT_CURRENT_NUM)
		{
			return -9903;
		}

		last_quality_type = quality_type;
		LittlePetQualityCfg & quality_cfg = m_quality_cfg[quality_type];

		int  point_type = 0;
		if (!PugiGetSubNodeValue(data_element, "point_type", point_type) || point_type != point_index)
		{
			return -1101;
		}

		if (point_type < 0 || point_type > LITTLEPET_QIANGHUAPOINT_CURRENT_NUM)
		{
			return -1102;
		}

		LittlePetStrengthenCfg &strengthen_cfg = quality_cfg.strengthen_cfg[point_type];
		strengthen_cfg.point_type = point_type;

		if (!PugiGetSubNodeValue(data_element, "baodi_count", strengthen_cfg.baodi_count) || strengthen_cfg.baodi_count < 0)
		{
			return -1103;
		}

		char attr_name[100];
		for (int base_value_index = 0; base_value_index < LITTLEPET_VALUE_TYPE_MAX_NUM; ++base_value_index)
		{
			LittlePetStrengthenValueCfg & value_cfg = strengthen_cfg.strengthen_value_cfg[base_value_index];
			value_cfg.value_type = base_value_index;

			SNPRINTF(attr_name, 100, "weight_%d", base_value_index);
			if (!PugiGetSubNodeValue(data_element, attr_name, value_cfg.weight) || value_cfg.weight < 0)
			{
				return -(1200 + base_value_index);				// base_value_index 为属性值类型
			}
			strengthen_cfg.total_weight += value_cfg.weight;

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
			if (!PugiGetSubNodeValue(data_element, attr_name, value_cfg.value_criticality) || value_cfg.value_criticality < value_cfg.value_min || value_cfg.value_criticality > value_cfg.value_max)
			{
				return -(1500 + base_value_index);
			}
		}

		++point_index;
		data_element = data_element.next_sibling();
	}

	for (int quality_type = 0; quality_type <= LITTLEPET_MAX_QUALITY_TYPE; ++quality_type)
	{
		LittlePetQualityCfg & quality_cfg = m_quality_cfg[quality_type];

		for (int point_type = 0; point_type < LITTLEPET_QIANGHUAPOINT_CURRENT_NUM; ++ point_type)
		{
			LittlePetStrengthenCfg &strengthen_cfg = quality_cfg.strengthen_cfg[point_type];
			if (strengthen_cfg.point_type < 0)
			{
				return -1600;
			}
		}
	}

	return 0;
}

bool LittlePetCfg::CheckSeq(int seq)
{
	if (seq < 0 || seq >= m_chou_cfg_count)
	{
		return false;
	}
	return true;
}

int LittlePetCfg::InitChouCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_chou_cfg_count = 0;
	m_chou_total_weight = 0;

	while (!data_element.empty())
	{
		if (m_chou_cfg_count >= LITTLEPET_CHOU_CFG_MAX_COUNT)
		{
			return -1200;
		}

		int seq = -1;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq < 0 || seq != m_chou_cfg_count)
		{
			return -1001;
		}
		LittlePetChouJiangCfg::ChouJiangCfg &chou_cfg = m_chou_cfg.chou_cfg_list[seq];
		chou_cfg.seq = seq;

		if (!PugiGetSubNodeValue(data_element, "weight", chou_cfg.weight) || chou_cfg.weight < 0)
		{
			return -1002;
		}

		if (!PugiGetSubNodeValue(data_element, "is_broadcast", chou_cfg.is_broadcast) || chou_cfg.is_broadcast < 0)
		{
			return -1003;
		}

		m_chou_total_weight += chou_cfg.weight;

		{
			PugiXmlNode item_data = data_element.child("reward_item");
			if (item_data.empty())
			{
				return -1100;
			}
			if (!chou_cfg.reward_item_cfg.ReadConfig(item_data))
			{
				return -1101;
			}
		}

		++m_chou_cfg_count;
		data_element = data_element.next_sibling();
	}

	if (m_chou_total_weight <= 0)
	{
		return -1200;
	}

	return 0;
}

int LittlePetCfg::InitExchangeCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_exchange_cfg_count = 0;

	while (!data_element.empty())
	{
		if (m_exchange_cfg_count >= LITTLEPET_EXCHANGE_CFG_MAX_NUM)
		{
			return -1200;
		}

		int seq = -1;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq < 0 || seq != m_exchange_cfg_count)
		{
			return -1001;
		}

		LittlePetExchangeCfg &exchange_cfg = m_exchang_cfg[seq];
		exchange_cfg.seq = seq;

		if (!PugiGetSubNodeValue(data_element, "need_score", exchange_cfg.need_score) || exchange_cfg.need_score < 0)
		{
			return -1002;
		}

		{
			PugiXmlNode item_data = data_element.child("exchage_item");
			if (item_data.empty())
			{
				return -1100;
			}
			if (!m_exchang_cfg[seq].exchange_item_cfg.ReadConfig(item_data))
			{
				return -1101;
			}
		}

		++m_exchange_cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int LittlePetCfg::InitFeedCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int feed_level = 0;

	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "feed_level", feed_level) || feed_level < 0 || feed_level > LITTLEPET_FEED_LEVEL_MAX)
		{
			return -1;
		}

		int quality_type = -1;
		if (!PugiGetSubNodeValue(data_element, "quality_type", quality_type) || quality_type < 0 || quality_type >= LITTLEPET_MAX_QUALITY_TYPE)
		{
			return -10;
		}

		LittlePetFeedCfg &feed_cfg = m_feed_cfg_list[quality_type][feed_level];
		feed_cfg.feed_level = feed_level;
		feed_cfg.quality_type = quality_type;

		if (!PugiGetSubNodeValue(data_element, "max_hp", feed_cfg.max_hp) || feed_cfg.max_hp < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "fangyu", feed_cfg.fangyu) || feed_cfg.fangyu < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "gongji", feed_cfg.gongji) || feed_cfg.gongji < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "base_attr_add_per", feed_cfg.base_attr_add_per) || feed_cfg.base_attr_add_per < 0)
		{
			return -5;
		}

		char temp_node_name[64] = { 0 };
		for (int i = 0; i < LITTLEPET_MAX_QUALITY_TYPE; ++i)
		{
			sprintf(temp_node_name, "stuff_id_%d", i);
			if (!PugiGetSubNodeValue(data_element, temp_node_name, feed_cfg.stuff_id_list[i]) || feed_cfg.stuff_id_list[i] <= 0)
			{
				return -6;
			}

			sprintf(temp_node_name, "stuff_num_%d", i);
			if (!PugiGetSubNodeValue(data_element, temp_node_name, feed_cfg.stuff_num_list[i]) || feed_cfg.stuff_num_list[0] < 0)
			{
				return -7;
			}

			sprintf(temp_node_name, "stuff_bind_%d", i);
			if (!PugiGetSubNodeValue(data_element, temp_node_name, feed_cfg.stuff_bind_list[i]) || feed_cfg.stuff_bind_list[0] < 0 || feed_cfg.stuff_bind_list[0] > 1)
			{
				return -11;
			}

			if (feed_cfg.stuff_id_list[i] > 0 && (nullptr == ITEMPOOL->GetItem(feed_cfg.stuff_id_list[i]) || feed_cfg.stuff_num_list[i] <= 0))
			{
				return -8;
			}
		}

		// 一个材料都没有不可能
		//if (feed_cfg.stuff_id_list[0] <= 0)
		//{
		//	return -9;
		//}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int LittlePetCfg::InitEquipUplevelCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int equip_index = 0;
	int level = 0;

	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "equip_index", equip_index) || equip_index < 0 || equip_index >= LITTLEPET_EQUIP_INDEX_MAX_NUM)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "level", level) || level < 0 || level > LITTLEPET_EQUIP_LEVEL_MAX)
		{
			return -2;
		}

		LittlePetEquipUplevelCfg &equip_uplevel_cfg = m_equip_uplevel_cfg_list[equip_index][level];
		equip_uplevel_cfg.equip_index = equip_index;
		equip_uplevel_cfg.level = level;

		if (!PugiGetSubNodeValue(data_element, "stuff_id", equip_uplevel_cfg.stuff_id) || nullptr == ITEMPOOL->GetItem(equip_uplevel_cfg.stuff_id))
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "stuff_num", equip_uplevel_cfg.stuff_num) || equip_uplevel_cfg.stuff_num <= 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "maxhp", equip_uplevel_cfg.maxhp) || equip_uplevel_cfg.maxhp < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "gongji", equip_uplevel_cfg.gongji) || equip_uplevel_cfg.gongji < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "fangyu", equip_uplevel_cfg.fangyu) || equip_uplevel_cfg.fangyu < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(data_element, "base_attr_add_per", equip_uplevel_cfg.base_attrs_add_per) || equip_uplevel_cfg.base_attrs_add_per < 0)
		{
			return -8;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int LittlePetCfg::GetQualityTypeByPetId(int id)
{
	if (id <= 0 || id >= LITTLEPET_MAX_NUM)
	{
		return  -1;
	}

	return m_base_cfg[id].quality_type;
}

const LittlePetBaseCfg *LittlePetCfg::GetBaseCfgById(int id)
{
	if (id <= 0 || id > m_base_cfg_count || id >= LITTLEPET_MAX_NUM)
	{
		return NULL;
	}

	return &m_base_cfg[id];
}

const LittlePetBaseCfg *LittlePetCfg::GetBaseCfgByItemID(ItemID item_id)
{
	for (int id = 0; id <= m_base_cfg_count && id < LITTLEPET_MAX_NUM; ++id)
	{
		if (m_base_cfg[id].active_item_id == (int)item_id)
		{
			return &m_base_cfg[id];
		}
	}

	return NULL;
}

const LittlePetBaseCfg * LittlePetCfg::GetBaseCfgByImgId(int img_id)
{
	for (int id = 0; id < LITTLEPET_MAX_NUM; ++id)
	{
		if (m_base_cfg[id].using_img_id == img_id)
		{
			return &m_base_cfg[id];
		}
	}

	return NULL;
}

const LittlePetQualityCfg * LittlePetCfg::GetLittlePetQualityCfg(int quality_type)
{
	if (quality_type < 0 || quality_type > LITTLEPET_MAX_QUALITY_TYPE)
	{
		return NULL;
	}

	if (m_quality_cfg[quality_type].quality_type != quality_type)
	{
		return NULL;
	}

	return &m_quality_cfg[quality_type];
}

const LittlePetStrengthenCfg *LittlePetCfg::GetLittlePetStrengthenCfg(int quality_type, int point_type)
{
	if (quality_type < 0 || quality_type > LITTLEPET_MAX_QUALITY_TYPE)
	{
		return NULL;
	}

	if (point_type < 0 || point_type > LITTLEPET_VALUE_TYPE_MAX_NUM)
	{
		return NULL;
	}

	return &m_quality_cfg[quality_type].strengthen_cfg[point_type];
}

const LittlePetChouJiangCfg &LittlePetCfg::GetLittlePetChouCfg()
{
	return m_chou_cfg;
}

const LittlePetExchangeCfg *LittlePetCfg::GetLittlePetExchangeCfg(int seq)
{
	if (seq < 0 || seq > m_exchange_cfg_count || seq >= LITTLEPET_EXCHANGE_CFG_MAX_NUM)
	{
		return NULL;
	}

	return &m_exchang_cfg[seq];
}

const LittlePetFeedCfg * LittlePetCfg::GetFeedCfg(int quality_type, int feed_level)
{
	if (quality_type < 0 || quality_type >= LITTLEPET_MAX_QUALITY_TYPE) return nullptr;
	if (feed_level < 0 || feed_level > LITTLEPET_FEED_LEVEL_MAX) return nullptr;

	if (m_feed_cfg_list[quality_type][feed_level].feed_level != feed_level) return nullptr;

	return &m_feed_cfg_list[quality_type][feed_level];
}

const LittlePetEquipUplevelCfg * LittlePetCfg::GetEquipmentUplevelCfg(int equip_index, int level)
{
	if (equip_index < 0 || equip_index >= LITTLEPET_EQUIP_INDEX_MAX_NUM)
	{
		return nullptr;
	}

	if (level < 0 || level > LITTLEPET_EQUIP_LEVEL_MAX)
	{
		return nullptr;
	}

	LittlePetEquipUplevelCfg &equip_uplevel_cfg = m_equip_uplevel_cfg_list[equip_index][level];

	if (equip_uplevel_cfg.equip_index != equip_index || equip_uplevel_cfg.level != level)
	{
		return nullptr;
	}

	return &equip_uplevel_cfg;
}