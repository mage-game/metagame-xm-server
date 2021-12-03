#include "equipbaptizeconfig.hpp"
#include "item/itempool.h"
#include "globalconfig/equipconfig.h"
#include "servercommon/configcommon.h"
#include "servercommon/servercommon.h"

EquipBaptizeConfig::EquipBaptizeConfig() : m_baptize_count(0)
{
	memset(m_equip_big_type_list, 0, sizeof(m_equip_big_type_list));
}

EquipBaptizeConfig::~EquipBaptizeConfig()
{
}

bool EquipBaptizeConfig::Init(const std::string &configname, std::string *err)
{
	YY_XML_CONFIG_PRE_LOAD;

	// 其他配置信息
	YY_XML_LOAD_CONFIG("other", InitOtherCfg);
	// 部位开启
	YY_XML_LOAD_CONFIG("part_open", InitOpenCfg);
	// 洗炼消耗
	YY_XML_LOAD_CONFIG("baptize_consume", InitConsumeCfg);
	// 锁定消耗
	YY_XML_LOAD_CONFIG("lock_consume", InitLockCfg);
	// 装备大类型
	YY_XML_LOAD_CONFIG("equip_big_type", InitEquipBigTypeCfg);
	// 洗炼属性
	YY_XML_LOAD_CONFIG("baptize_attr", InitAttrCfg);
	// 套装洗炼
	YY_XML_LOAD_CONFIG("baptize_suit", InitSuitCfg);
	// 传闻
	YY_XML_LOAD_CONFIG("broadcast", InitBroadcastCfg);

	return true;
}

int EquipBaptizeConfig::InitOtherCfg(YY_XML_FUNCTION_NODE_TYPE RootElement)
{
	YY_XML_NODE data_element = YY_XML_FIRST_CHILD_ELEMENT(RootElement, "data");
	if (YY_XML_NODE_IS_EMPTY(data_element))
	{
		return -1000;
	}

	if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "everyday_free_times", m_other_cfg.everyday_free_times) || m_other_cfg.everyday_free_times < 0)
	{
		return -1;
	}

	return 0;
}

int EquipBaptizeConfig::InitOpenCfg(YY_XML_FUNCTION_NODE_TYPE RootElement)
{
	YY_XML_NODE data_element = YY_XML_FIRST_CHILD_ELEMENT(RootElement, "data");
	if (YY_XML_NODE_IS_EMPTY(data_element))
	{
		return -1000;
	}
	
	while (!YY_XML_NODE_IS_EMPTY(data_element))
	{
		int equip_part = -1;
		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "equip_part", equip_part) || equip_part < 0 || equip_part >= EQUIP_BAPTIZE_EQUIP_GRID_MAX_NUM)
		{
			return -1;
		}

		EquipBaptizeOpenConfig &cfg = m_open_cfg_list[equip_part];
		cfg.equip_part = equip_part;

		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "role_level", cfg.role_level) || cfg.role_level < 0 || cfg.role_level > MAX_ROLE_LEVEL)
		{
			return -2;
		}

		data_element = YY_XML_NEXT_SIBLING(data_element);
	}

	return 0;
}

int EquipBaptizeConfig::InitConsumeCfg(YY_XML_FUNCTION_NODE_TYPE RootElement)
{
	YY_XML_NODE data_element = YY_XML_FIRST_CHILD_ELEMENT(RootElement, "data");
	if (YY_XML_NODE_IS_EMPTY(data_element))
	{
		return -1000;
	}

	m_baptize_count = 0;

	while (!YY_XML_NODE_IS_EMPTY(data_element))
	{
		if (m_baptize_count >= EQUIP_BAPTIZE_ONE_PART_MAX_BAPTIZE_NUM)
		{
			return -100;
		}

		int index = -1;
		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "index", index) || index < 0 || index >= EQUIP_BAPTIZE_ONE_PART_MAX_BAPTIZE_NUM || m_baptize_count != index)
		{
			return -1;
		}

		EquipBaptizeConsumeConfig &cfg = m_consume_cfg_list[index];
		cfg.index = index;

		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "consume_gold", cfg.consume_gold) || cfg.consume_gold < 0)
		{
			return -2;
		}

		++ m_baptize_count;
		data_element = YY_XML_NEXT_SIBLING(data_element);
	}

	if (m_baptize_count >= EQUIP_BAPTIZE_ONE_PART_MAX_BAPTIZE_NUM)
	{
		return -101;
	}
	
	return 0;
}

int EquipBaptizeConfig::InitLockCfg(YY_XML_FUNCTION_NODE_TYPE RootElement)
{
	YY_XML_NODE data_element = YY_XML_FIRST_CHILD_ELEMENT(RootElement, "data");
	if (YY_XML_NODE_IS_EMPTY(data_element))
	{
		return -1000;
	}

	while (!YY_XML_NODE_IS_EMPTY(data_element))
	{
		int lock_num = -1;
		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "lock_num", lock_num) || lock_num < 0 || lock_num >= EQUIP_BAPTIZE_ONE_PART_MAX_BAPTIZE_NUM)
		{
			return -1;
		}
		
		EquipBaptizeLockConfig &cfg = m_lock_cfg_list[lock_num];
		cfg.lock_num = lock_num;

		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "consume_stuff_id", cfg.consume_stuff_id) || NULL == ITEMPOOL->GetItem(cfg.consume_stuff_id))
		{
			return -2;
		}

		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "consume_stuff_num", cfg.consume_stuff_num) || cfg.consume_stuff_num <= 0)
		{
			return -3;
		}

		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "purple_stuff_id", cfg.purple_stuff_id) || cfg.purple_stuff_id < 0 || NULL == ITEMPOOL->GetItem(cfg.purple_stuff_id))
		{
			return -4;
		}

		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "purple_stuff_num", cfg.purple_stuff_num) || cfg.purple_stuff_num < 0)
		{
			return -41;
		}

		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "orange_stuff_id", cfg.orange_stuff_id) || cfg.orange_stuff_id < 0 || NULL == ITEMPOOL->GetItem(cfg.orange_stuff_id))
		{
			return -5;
		}

		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "orange_stuff_num", cfg.orange_stuff_num) || cfg.orange_stuff_num < 0)
		{
			return -6;
		}

		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "red_stuff_id", cfg.red_stuff_id) || cfg.red_stuff_id < 0 || NULL == ITEMPOOL->GetItem(cfg.red_stuff_id))
		{
			return -7;
		}

		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "red_stuff_num", cfg.red_stuff_num) || cfg.red_stuff_num < 0)
		{
			return -8;
		}

		data_element = YY_XML_NEXT_SIBLING(data_element);
	}

	return 0;
}

int EquipBaptizeConfig::InitEquipBigTypeCfg(YY_XML_FUNCTION_NODE_TYPE RootElement)
{
	YY_XML_NODE data_element = YY_XML_FIRST_CHILD_ELEMENT(RootElement, "data");
	if (YY_XML_NODE_IS_EMPTY(data_element))
	{
		return -1000;
	}

	int equip_index = -1;
	while (!YY_XML_NODE_IS_EMPTY(data_element))
	{
		equip_index = -1;
		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "equip_part", equip_index) || equip_index < 0 || equip_index >= EQUIP_BAPTIZE_EQUIP_GRID_MAX_NUM)
		{
			return -1;
		}

		EquipBigTypeConfig &cfg = m_equip_big_type_list[equip_index];
		cfg.equip_index = equip_index;

		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "big_type", cfg.equip_big_type) || cfg.equip_big_type < EQUIP_XIANPIN_BIG_TYPE_GONGJI || cfg.equip_big_type >= EQUIP_XIANPIN_BIG_TYPE_MAX)
		{
			return -2;
		}

		data_element = YY_XML_NEXT_SIBLING(data_element);
	}

	return 0;
}

const EquipBigTypeConfig * EquipBaptizeConfig::GetEquipBigType(int equip_index)
{
	if (equip_index < 0 || equip_index >= EQUIP_BAPTIZE_EQUIP_GRID_MAX_NUM)
	{
		return nullptr;
	}

	return &m_equip_big_type_list[equip_index];
}

int EquipBaptizeConfig::InitAttrCfg(YY_XML_FUNCTION_NODE_TYPE RootElement)
{
	YY_XML_NODE data_element = YY_XML_FIRST_CHILD_ELEMENT(RootElement, "data");
	if (YY_XML_NODE_IS_EMPTY(data_element))
	{
		return -1000;
	}

	while (!YY_XML_NODE_IS_EMPTY(data_element))
	{
		EquipBaptizeAttrConfig cfg;

		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "special_type", cfg.special_type) || cfg.special_type < EQUIP_BAPTIZE_SPECIAL_TYPE_NONE || cfg.special_type >= EQUIP_BAPTIZE_SPECIAL_TYPE_MAX)
		{
			return -10000;
		}

		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "seq", cfg.seq) || cfg.seq <= 0 || cfg.seq >= CharIntAttrs::CHARINTATTR_TYPE_MAX)
		{
			return -1;
		}

		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "big_type", cfg.big_type) || cfg.big_type < EQUIP_XIANPIN_BIG_TYPE_GONGJI || cfg.big_type >= EQUIP_XIANPIN_BIG_TYPE_MAX)
		{
			return -2;
		}

		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "weight", cfg.weight) || cfg.weight < 0)
		{
			return -3;
		}

		std::string attr_string;
		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "attr_type", attr_string))
		{
			return -4;
		}
		if (!CharIntAttrs::GetAttrTypeFromAttrString(attr_string, &cfg.attr_type) || cfg.attr_type < 0 || cfg.attr_type >= CharIntAttrs::CHARINTATTR_TYPE_MAX)
		{
			return -5;
		}

		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "white_weight", cfg.color_list[WHITE].color_weight) || cfg.color_list[WHITE].color_weight < 0)
		{
			return -6;
		}
		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "blue_weight", cfg.color_list[BLUE].color_weight) || cfg.color_list[BLUE].color_weight < 0)
		{
			return -7;
		}
		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "purple_weight", cfg.color_list[PURPLE].color_weight) || cfg.color_list[PURPLE].color_weight < 0)
		{
			return -8;
		}
		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "orange_weight", cfg.color_list[ORANGE].color_weight) || cfg.color_list[ORANGE].color_weight < 0)
		{
			return -9;
		}
		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "red_weight", cfg.color_list[RED].color_weight) || cfg.color_list[RED].color_weight < 0)
		{
			return -10;
		}

		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "white_value_low", cfg.color_list[WHITE].attr_val_low) || cfg.color_list[WHITE].attr_val_low <= 0)
		{
			return -11;
		}
		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "white_value_high", cfg.color_list[WHITE].attr_val_high) || cfg.color_list[WHITE].attr_val_high <= 0)
		{
			return -12;
		}
		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "blue_value_low", cfg.color_list[BLUE].attr_val_low) || cfg.color_list[BLUE].attr_val_low <= 0)
		{
			return -13;
		}
		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "blue_value_high", cfg.color_list[BLUE].attr_val_high) || cfg.color_list[BLUE].attr_val_high <= 0)
		{
			return -14;
		}
		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "purple_value_low", cfg.color_list[PURPLE].attr_val_low) || cfg.color_list[PURPLE].attr_val_low <= 0)
		{
			return -15;
		}
		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "purple_value_high", cfg.color_list[PURPLE].attr_val_high) || cfg.color_list[PURPLE].attr_val_high <= 0)
		{
			return -16;
		}
		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "orange_value_low", cfg.color_list[ORANGE].attr_val_low) || cfg.color_list[ORANGE].attr_val_low <= 0)
		{
			return -17;
		}
		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "orange_value_high", cfg.color_list[ORANGE].attr_val_high) || cfg.color_list[ORANGE].attr_val_high <= 0)
		{
			return -18;
		}
		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "red_value_low", cfg.color_list[RED].attr_val_low) || cfg.color_list[RED].attr_val_low <= 0)
		{
			return -19;
		}
		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "red_value_high", cfg.color_list[RED].attr_val_high) || cfg.color_list[RED].attr_val_high <= 0)
		{
			return -20;
		}

		long long key = ConvertParamToLongLong(cfg.seq, cfg.big_type);
		EquipBaptizeAttrConfigMapIt it = m_attr_cfg_map[cfg.special_type].find(key);
		if (it != m_attr_cfg_map[cfg.special_type].end())
		{
			return - 100;
		}
		m_attr_cfg_map[cfg.special_type][key] = cfg;


		EquipBaptizeAttrConfigMapIt map_it = m_recalc_attr_map[cfg.special_type].find(cfg.seq);
		if (map_it != m_recalc_attr_map[cfg.special_type].end())
		{
			return - 101;
		}
		m_recalc_attr_map[cfg.special_type][cfg.seq] = cfg;

		data_element = YY_XML_NEXT_SIBLING(data_element);
	}

	return 0;
}

int EquipBaptizeConfig::InitSuitCfg(YY_XML_FUNCTION_NODE_TYPE RootElement)
{
	YY_XML_NODE data_element = YY_XML_FIRST_CHILD_ELEMENT(RootElement, "data");
	if (YY_XML_NODE_IS_EMPTY(data_element))
	{
		return -1000;
	}

	int equip_part = -1;
	while (!YY_XML_NODE_IS_EMPTY(data_element))
	{
		equip_part = -1;
		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "equip_part", equip_part) || equip_part < 0 || equip_part >= EQUIP_BAPTIZE_EQUIP_GRID_MAX_NUM)
		{
			return -1;
		}

		EquipBaptizeSuitAttrConfig &suit_attr = m_baptize_suit_attr_list[equip_part];
		suit_attr.equip_part = equip_part;

		EquipBaptizeSuitAttrConfig::EquipPartAttrCfg cfg;

		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "baptize_color", cfg.baptize_color) || cfg.baptize_color < 0 || cfg.baptize_color >= EQUIP_BAPTIZE_ATTR_COLOR_MAX_NUM)
		{
			return -2;
		}

		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "baptize_count", cfg.baptize_count) || cfg.baptize_count <= 0 || cfg.baptize_count >= EQUIP_BAPTIZE_ONE_PART_MAX_BAPTIZE_NUM)
		{
			return -3;
		}

		std::string attr_string;
		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "attr_type", attr_string))
		{
			return -4;
		}
		if (!CharIntAttrs::GetAttrTypeFromAttrString(attr_string, &cfg.attr_type) || cfg.attr_type < 0 || cfg.attr_type >= CharIntAttrs::CHARINTATTR_TYPE_MAX)
		{
			return -5;
		}

		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "attr_value", cfg.attr_value) || cfg.attr_value <= 0)
		{
			return -6;
		}

		suit_attr.equip_attr_cfg.push_back(cfg);
		data_element = YY_XML_NEXT_SIBLING(data_element);
	}

	return 0;
}

Attribute EquipBaptizeConfig::GetBaptizeSuitAttrValue(int equip_index, int baptize_color_num[EQUIP_BAPTIZE_ATTR_COLOR_MAX_NUM], int &baptize_color)
{
	if (equip_index < 0 || equip_index >= EQUIP_BAPTIZE_EQUIP_GRID_MAX_NUM)
	{
		return 0;
	}

	EquipBaptizeSuitAttrConfig &suit_attr = m_baptize_suit_attr_list[equip_index];

	int size = suit_attr.equip_attr_cfg.size();
	if (size <= 0) return 0;

	for (int i = size - 1; i >= 0; --i)
	{
		int color = suit_attr.equip_attr_cfg[i].baptize_color;
		if (color < 0 || color >= EQUIP_BAPTIZE_ATTR_COLOR_MAX_NUM)
		{
			continue;
		}

		if (baptize_color_num[color] >= suit_attr.equip_attr_cfg[i].baptize_count)
		{
			baptize_color = color;
			return suit_attr.equip_attr_cfg[i].attr_value;
		}
	}

	return 0;
}

int EquipBaptizeConfig::GetBaptizeSuitAttrType(int equip_index, int baptize_color)
{
	if (equip_index < 0 || equip_index >= EQUIP_BAPTIZE_EQUIP_GRID_MAX_NUM)
	{
		return 0;
	}

	EquipBaptizeSuitAttrConfig &suit_attr = m_baptize_suit_attr_list[equip_index];
	int size = suit_attr.equip_attr_cfg.size();
	if (size <= 0) return 0;

	for (int i = size - 1; i >= 0; --i)
	{
		if (baptize_color == suit_attr.equip_attr_cfg[i].baptize_color)
		{
			return suit_attr.equip_attr_cfg[i].attr_type;
		}
	}

	return 0;
}

int EquipBaptizeConfig::InitBroadcastCfg(YY_XML_FUNCTION_NODE_TYPE RootElement)
{
	YY_XML_NODE data_element = YY_XML_FIRST_CHILD_ELEMENT(RootElement, "data");
	if (YY_XML_NODE_IS_EMPTY(data_element))
	{
		return -1000;
	}

	while (!YY_XML_NODE_IS_EMPTY(data_element))
	{
		EquipBaptizeBroadcastConfig cfg;
		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "seq", cfg.seq) || cfg.seq < 0)
		{
			return -1;
		}

		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "orange_num", cfg.orange_num) || cfg.orange_num < 0 || cfg.orange_num > EQUIP_BAPTIZE_ONE_PART_MAX_BAPTIZE_NUM)
		{
			return -2;
		}

		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "red_num", cfg.red_num) || cfg.red_num < 0 || cfg.red_num > EQUIP_BAPTIZE_ONE_PART_MAX_BAPTIZE_NUM)
		{
			return -3;
		}

		int is_broadcast = 0;
		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "is_broadcast", is_broadcast))
		{
			return -4;
		}
		cfg.is_broadcast = (0 != is_broadcast);

		m_broadcast_cfg_vec.push_back(cfg);

		data_element = YY_XML_NEXT_SIBLING(data_element);
	}

	return 0;
}

const EquipBaptizeOpenConfig * EquipBaptizeConfig::GetOpenCfg(int equip_part)
{
	if (equip_part < 0 || equip_part >= EQUIP_BAPTIZE_EQUIP_GRID_MAX_NUM)
	{
		return NULL;
	}

	return &m_open_cfg_list[equip_part];
}

const EquipBaptizeConsumeConfig * EquipBaptizeConfig::GetConsumeCfg(int index)
{
	if (index < 0 || index >= EQUIP_BAPTIZE_ONE_PART_MAX_BAPTIZE_NUM || index >= m_baptize_count)
	{
		return NULL;
	}

	return &m_consume_cfg_list[index];
}

const EquipBaptizeLockConfig * EquipBaptizeConfig::GetLockCfg(int lock_num)
{
	if (lock_num < 0 || lock_num >= EQUIP_BAPTIZE_ONE_PART_MAX_BAPTIZE_NUM || lock_num >= m_baptize_count)
	{
		return NULL;
	}

	return &m_lock_cfg_list[lock_num];
}

const EquipBaptizeAttrConfig * EquipBaptizeConfig::GetAttrCfg(int seq, int big_type)
{
	if (seq <= 0 || seq >= CharIntAttrs::CHARINTATTR_TYPE_MAX || big_type < EQUIP_XIANPIN_BIG_TYPE_GONGJI || big_type >= EQUIP_XIANPIN_BIG_TYPE_MAX)
	{
		return NULL;
	}

	long long key = ConvertParamToLongLong(seq, big_type);
	EquipBaptizeAttrConfigMapIt it = m_attr_cfg_map[EQUIP_BAPTIZE_SPECIAL_TYPE_NONE].find(key);
	if (it != m_attr_cfg_map[EQUIP_BAPTIZE_SPECIAL_TYPE_NONE].end())
	{
		return &it->second;
	}

	return NULL;
}

const EquipBaptizeAttrConfig * EquipBaptizeConfig::GetReCalcAttrCfg(int special_type, int seq)
{
	if (special_type < EQUIP_BAPTIZE_SPECIAL_TYPE_NONE || special_type >= EQUIP_BAPTIZE_SPECIAL_TYPE_MAX ||
		seq <= 0 || seq >= CharIntAttrs::CHARINTATTR_TYPE_MAX)
	{
		return NULL;
	}

	EquipBaptizeAttrConfigMapIt it = m_recalc_attr_map[special_type].find(seq);
	if (it != m_recalc_attr_map[special_type].end())
	{
		return &it->second;
	}

	return NULL;
}

int EquipBaptizeConfig::GetAttrColorBySeqAndValue(int seq, int attr_value)
{
	if (seq <= 0 || seq >= CharIntAttrs::CHARINTATTR_TYPE_MAX)
	{
		return EQUIP_BAPTIZE_ATTR_COLOR_TYPE_INVALID;
	}

	EquipBaptizeAttrConfigMapIt map_it = m_recalc_attr_map[EQUIP_BAPTIZE_SPECIAL_TYPE_NONE].find(seq);
	if (map_it != m_recalc_attr_map[EQUIP_BAPTIZE_SPECIAL_TYPE_NONE].end())
	{
		for (int i = 0; i < EQUIP_BAPTIZE_ATTR_COLOR_MAX_NUM; ++i)
		{
			if (attr_value >= map_it->second.color_list[i].attr_val_low && attr_value <= map_it->second.color_list[i].attr_val_high)
			{
				return i;
			}
		}
	}

	return EQUIP_BAPTIZE_ATTR_COLOR_TYPE_INVALID;
}

const EquipBaptizeBroadcastConfig * EquipBaptizeConfig::GetBroadcastCfg(int orange_num, int red_num)
{
	if (orange_num < 0 || red_num < 0 || orange_num > EQUIP_BAPTIZE_ONE_PART_MAX_BAPTIZE_NUM || red_num > EQUIP_BAPTIZE_ONE_PART_MAX_BAPTIZE_NUM)
	{
		return NULL;
	}

	for (int i = 0; i < (int)m_broadcast_cfg_vec.size(); ++i)
	{
		if (orange_num == m_broadcast_cfg_vec[i].orange_num && red_num == m_broadcast_cfg_vec[i].red_num)
		{
			return &m_broadcast_cfg_vec[i];
		}
	}

	return NULL;
}