#include "crossequipconfig.hpp"
#include "servercommon/configcommon.h"
#include "item/itempool.h"

CrossEquipConfig::CrossEquipConfig() : m_max_grade(0)
{
}

CrossEquipConfig::~CrossEquipConfig()
{
}


bool CrossEquipConfig::Init(const std::string &configname, std::string *err)
{
	PRE_LOAD_CONFIG;

	LOAD_CONFIG("douqi_grade", InitCrossEquipDouqi);

	LOAD_CONFIG("douqi_exchange", InitCrossEquipDouqiXiulian);

	LOAD_CONFIG("douqidan", InitCrossEquipDouqiDan);

	LOAD_CONFIG("roll", InitCrossEquipRoll);

//	LOAD_CONFIG("roll_item_list", InitCrossEquipRollItem);

	LOAD_CONFIG("equip_info", InitCrossEquipEquipInfo);

	// Ì××°ÊôÐÔ
	LOAD_CONFIG("suit_attr", InitSuitAttrCfg);

	return true;
}

const CrossEquipDouqiCfg * CrossEquipConfig::GetCrossEquipDouqi(short douqi_grade)
{
	auto douqi_map = m_cross_equip_douqi_map.find(douqi_grade);
	
	if (m_cross_equip_douqi_map.end() != douqi_map)
	{
		return &(douqi_map->second);
	}

	return nullptr;
}

const CrossEquipDouqiXiulianCfg * CrossEquipConfig::GetCrossEquipDouqiXiulianCfg(short douqi_grade, short today_xiulian_times)
{
	auto douqi_map = m_cross_equip_douqi_map.find(douqi_grade);

	if (m_cross_equip_douqi_map.end() != douqi_map)
	{
		for (auto &xiulian_vec_it : douqi_map->second.douqi_xiulian_vec)
		{
			if (today_xiulian_times == xiulian_vec_it.xiulian_times)
			{
				return &(xiulian_vec_it);
			}
		}
	}

	return nullptr;
}

const CrossEquipDouqiDanCfg * CrossEquipConfig::GetCrossEquipDouqiDan(short douqidan_type)
{
	auto douqidan_map_it = m_cross_equip_douqidan_map.find(douqidan_type);

	if (m_cross_equip_douqidan_map.end() != douqidan_map_it)
	{
		return &(douqidan_map_it->second);
	}

	return nullptr;
}

const CrossEquipRollCfg * CrossEquipConfig::GetCrossEquipRoll(short roll_type)
{
	auto roll_map_it = m_cross_equip_roll_map.find(roll_type);

	if (m_cross_equip_roll_map.end() != roll_map_it)
	{
		return &(roll_map_it->second);
	}

	return nullptr;
}

bool CrossEquipConfig::GetCrossEquipRollReward(short roll_type, ItemConfigData *reward_cfg)
{
	auto roll_map_it = m_cross_equip_roll_map.find(roll_type);

	if (m_cross_equip_roll_map.end() != roll_map_it)
	{
		int total_weight = 0;

		for (auto vec_it : roll_map_it->second.roll_item_vec)
		{
			total_weight += vec_it.weight;
		}

		if (total_weight <= 0)
		{
			return false;
		}

		int rand_val = RandomNum(total_weight);

		for (auto vec_it : roll_map_it->second.roll_item_vec)
		{
			if (rand_val < vec_it.weight)
			{
				*reward_cfg = vec_it.item_cfg;
				return true;
			}
			rand_val -= vec_it.weight;
		}
	}

	return false;
}

short CrossEquipConfig::GetEquipGrade(ItemID equip_id)
{
	auto grade_map_it = m_cross_equip_equip_need_grade_map.find(equip_id);
	if (m_cross_equip_equip_need_grade_map.end() != grade_map_it)
	{
		return grade_map_it->second;
	}

	return -1;
}

const CrossEquipLianzhiCfg * CrossEquipConfig::GetLianzhiCfg(short prof, short grade, short equip_index)
{
	long long key = ConvertParamToLongLong(prof, grade, equip_index);
	auto lianzhi_map_it = m_cross_equip_lianzhi_info_map.find(key);
	if (m_cross_equip_lianzhi_info_map.end() != lianzhi_map_it)
	{
		return &(lianzhi_map_it->second);
	}

	return nullptr;
}

bool CrossEquipConfig::IsCalcSuit(int equip_order, int color)
{
	for (int i = (int)m_suit_attr_vec.size() - 1; i >= 0; --i)
	{
		if (equip_order == m_suit_attr_vec[i].order &&
			color == m_suit_attr_vec[i].color
			)
		{
			return true;
		}
	}

	return false;
}

const CrossEquipSuitAttrCfg * CrossEquipConfig::GetSuitAttrCfg(int equip_order, int color, int same_order_num)
{
	if (equip_order <= 0 || equip_order > CROSS_EQUIP_MAX_ORDER || 
		color <= 0 || color >= Equipment::I_COLOR_MAX || 
		same_order_num <= 0 || same_order_num > CROSS_EQUIP_INDEX_MAX)
	{
		return NULL;
	}

	for (int i = (int)m_suit_attr_vec.size() - 1; i >= 0; --i)
	{
		if (equip_order == m_suit_attr_vec[i].order &&
			color == m_suit_attr_vec[i].color &&
			same_order_num >= m_suit_attr_vec[i].need_count
			)
		{
			return &m_suit_attr_vec[i];
		}
	}

	return NULL;
}

int CrossEquipConfig::InitCrossEquipDouqi(TiXmlElement * root_element)
{
	TiXmlElement *data_element = root_element->FirstChildElement("data");
	if (nullptr == data_element)
	{
		return -100000;
	}

	short grade = 0, last_grade = -1;
	while (data_element)
	{
		if (!GetSubNodeValue(data_element, "douqi_grade", grade) || (last_grade + 1) != grade)
		{
			return -1;
		}
		last_grade = grade;

		CrossEquipDouqiCfg &cfg = m_cross_equip_douqi_map[grade];
		cfg.douqi_grade = grade;

		if (!GetSubNodeValue(data_element, "equip_level", cfg.equip_level) || cfg.equip_level < 0)
		{
			return -2;
		}

		if (!GetSubNodeValue(data_element, "maxhp", cfg.maxhp) || cfg.maxhp < 0) return -101;

		if (!GetSubNodeValue(data_element, "gongji", cfg.gongji) || cfg.gongji < 0) return -102;

		if (!GetSubNodeValue(data_element, "fangyu", cfg.fangyu) || cfg.fangyu < 0) return -103;

		if (!GetSubNodeValue(data_element, "mingzhong", cfg.mingzhong) || cfg.mingzhong < 0) return -104;

		if (!GetSubNodeValue(data_element, "shanbi", cfg.shanbi) || cfg.shanbi < 0) return -105;

		if (!GetSubNodeValue(data_element, "baoji", cfg.baoji) || cfg.baoji < 0) return -106;

		if (!GetSubNodeValue(data_element, "jianren", cfg.jianren) || cfg.jianren < 0) return -107;

		int broadcast = 0;
		if (!GetSubNodeValue(data_element, "is_broadcast", broadcast) || broadcast < 0)
		{
			return -6;
		}
		cfg.is_broadcast = (broadcast > 0 ? true : false);

		if (!GetSubNodeValue(data_element, "need_exp", cfg.upgrade_need_exp) || cfg.upgrade_need_exp <= 0)
		{
			return -7;
		}

		data_element = data_element->NextSiblingElement();
	}

	m_max_grade = grade;

	return 0;
}

int CrossEquipConfig::InitCrossEquipDouqiXiulian(TiXmlElement * root_element)
{
	TiXmlElement *data_element = root_element->FirstChildElement("data");
	if (nullptr == data_element)
	{
		return -100000;
	}

	short last_grade = 0, grade = 0, exchange_times = 0, last_exchange_times = 0;
	int need_mojing = 0;
	int reward_exp = 0;
	while (data_element)
	{
		if (!GetSubNodeValue(data_element, "douqi_grade", grade) || !IsValidCrossEquipDouqiGrade(grade))
		{
			return -1;
		}

		if (last_grade != grade)
		{
			last_exchange_times = 0;
		}
		last_grade = grade;

		CrossEquipDouqiCfg &cfg = m_cross_equip_douqi_map[grade];

		if (!GetSubNodeValue(data_element, "mojing_exchange_times", exchange_times) || exchange_times <= 0 || last_exchange_times + 1 != exchange_times)
		{
			return -2;
		}
		last_exchange_times = exchange_times;

		if (!GetSubNodeValue(data_element, "need_mojing", need_mojing) || need_mojing <= 0)
		{
			return -3;
		}

		if (!GetSubNodeValue(data_element, "reward_exp", reward_exp) || reward_exp <= 0)
		{
			return -4;
		}

		cfg.douqi_xiulian_vec.emplace_back(grade, exchange_times, need_mojing, reward_exp);

		data_element = data_element->NextSiblingElement();
	}

	return 0;
}

int CrossEquipConfig::InitCrossEquipDouqiDan(TiXmlElement * root_element)
{
	TiXmlElement *data_element = root_element->FirstChildElement("data");
	if (nullptr == data_element)
	{
		return -100000;
	}

	int douqidan_type = 0; 
	while (data_element)
	{
		if (!GetSubNodeValue(data_element, "douqidan_type", douqidan_type) 
			|| douqidan_type < CROSS_EQUIP_DOUQIDAN_TYPE_BEGIN || douqidan_type >= CROSS_EQUIP_DOUQIDAN_TYPE_MAX)
		{
			return -1;
		}

		CrossEquipDouqiDanCfg &cfg = m_cross_equip_douqidan_map[douqidan_type];
		cfg.douqidan_type = douqidan_type;

		if (!GetSubNodeValue(data_element, "reward_exp", cfg.reward_exp) || cfg.reward_exp <= 0)
		{
			return -2;
		}

		if (!GetSubNodeValue(data_element, "day_used_limit", cfg.used_times_per_day) || cfg.used_times_per_day <= 0)
		{
			return -3;
		}

		data_element = data_element->NextSiblingElement();
	}

	return 0;
}

int CrossEquipConfig::InitCrossEquipRoll(TiXmlElement * root_element)
{
	TiXmlElement *data_element = root_element->FirstChildElement("data");
	if (nullptr == data_element)
	{
		return -100000;
	}

	short roll_type = 0;
	while (data_element)
	{
		if (!GetSubNodeValue(data_element, "reward_type", roll_type)
			|| roll_type < CROSS_EQUIP_ROLL_TYPE_BEGIN || roll_type >= CROSS_EQUIP_ROLL_TYPE_MAX)
		{
			return -1;
		}

		CrossEquipRollCfg &cfg = m_cross_equip_roll_map[roll_type];
		cfg.roll_type = roll_type;

		if (!GetSubNodeValue(data_element, "open_need_grade", cfg.need_grade) || !IsValidCrossEquipDouqiGrade(cfg.need_grade))
		{
			return -2;
		}

		char temp_str[32] = { 0 };
		for (int i = CROSS_EQUIP_ROLL_COUNTS_TYPE_BEGIN + 1; i < CROSS_EQUIP_ROLL_COUNTS_TYPE_MAX; ++i)
		{
			sprintf(temp_str, "roll_times_%d", i);
			if (!GetSubNodeValue(data_element, temp_str, cfg.roll_times[i]) || cfg.roll_times[i] <= 0)
			{
				return -(10 * i + 3);
			}

			sprintf(temp_str, "consume_%d", i);
			if (!GetSubNodeValue(data_element, temp_str, cfg.consume[i]) || cfg.consume[i] <= 0)
			{
				return -(10 * i + 4);
			}

			sprintf(temp_str, "reward_score_%d", i);
			if (!GetSubNodeValue(data_element, temp_str, cfg.reward_chuanshi_score[i]) || cfg.reward_chuanshi_score[i] <= 0)
			{
				return -(10 * i + 5);
			}
		}
		data_element = data_element->NextSiblingElement();
	}

	return 0;
}

int CrossEquipConfig::InitCrossEquipRollItem(TiXmlElement * root_element)
{
	TiXmlElement *data_element = root_element->FirstChildElement("data");
	if (nullptr == data_element)
	{
		return -100000;
	}

	short roll_type = 0;
	int weight = 0;
	while (data_element)
	{
		if (!GetSubNodeValue(data_element, "reward_type", roll_type)
			|| roll_type < CROSS_EQUIP_ROLL_TYPE_BEGIN || roll_type >= CROSS_EQUIP_ROLL_TYPE_MAX)
		{
			return -1;
		}

		CrossEquipRollCfg &cfg = m_cross_equip_roll_map[roll_type];

		TiXmlElement *reward_item = data_element->FirstChildElement("reward_item");
		if (nullptr == reward_item)
		{
			return -3;
		}

		ItemConfigData reward_item_cfg;
		if (!reward_item_cfg.ReadConfig(reward_item))
		{
			return -4;
		}

		if (!GetSubNodeValue(data_element, "weight", weight) || weight <= 0)
		{
			return -5;
		}

		cfg.roll_item_vec.emplace_back(reward_item_cfg, weight);

		data_element = data_element->NextSiblingElement();
	}

	return 0;
}

int CrossEquipConfig::InitCrossEquipEquipInfo(TiXmlElement * root_element)
{
	TiXmlElement *data_element = root_element->FirstChildElement("data");
	if (nullptr == data_element)
	{
		return -100000;
	}

	ItemID equip_id = 0;
	short grade = 0;
	short prof = 0;
	int need_fragment = 0;
	short equip_index = 0;
	while (data_element)
	{
		if (!GetSubNodeValue(data_element, "equip_id", equip_id) || nullptr == ITEMPOOL->GetItem(equip_id))
		{
			return -1;
		}

		if (!GetSubNodeValue(data_element, "order", grade) || grade < 0)
		{
			return -2;
		}

		m_cross_equip_equip_need_grade_map.insert(std::make_pair(equip_id, grade));

		if (!GetSubNodeValue(data_element, "prof", prof) || prof <= PROF_TYPE_INVALID || prof > PROF_TYPE_PROF_NO_LIMIT)
		{
			return -3;
		}

		if (!GetSubNodeValue(data_element, "need_fragment", need_fragment) || need_fragment < 0)
		{
			return -4;
		}

		if (!GetSubNodeValue(data_element, "equip_index", equip_index) || equip_index < 0 || equip_index >= CROSS_EQUIP_INDEX_MAX)
		{
			return -5;
		}


		if (need_fragment > 0)
		{
			long long key = ConvertParamToLongLong(prof, grade, equip_index);
			if (m_cross_equip_lianzhi_info_map.end() != m_cross_equip_lianzhi_info_map.find(key))
			{
				return -6;
			}
			m_cross_equip_lianzhi_info_map.insert(std::make_pair(key, CrossEquipLianzhiCfg(equip_id, need_fragment)));
		}

		data_element = data_element->NextSiblingElement();
	}

	return 0;
}

int CrossEquipConfig::InitSuitAttrCfg(TiXmlElement *RootElement)
{
	TiXmlElement *data_element = RootElement->FirstChildElement("data");
	if (nullptr == data_element)
	{
		return -100000;
	}

	while (data_element)
	{
		CrossEquipSuitAttrCfg cfg;

		if (!GetSubNodeValue(data_element, "order", cfg.order) || cfg.order <= 0 || cfg.order >= ItemPool::MAX_EQUIPMENT_ORDER)
		{
			return -1;
		}

		if (!GetSubNodeValue(data_element, "color", cfg.color) || cfg.color <= 0 || cfg.color >= Equipment::I_COLOR_MAX)
		{
			return -2;
		}

		if (!GetSubNodeValue(data_element, "need_count", cfg.need_count) || cfg.need_count <= 0)
		{
			return -3;
		}

		int iRet = cfg.attr_cfg.ReadConfig(data_element);
		if (iRet < 0)
		{
			return iRet;
		}

		if (!GetSubNodeValue(data_element, "per_mingzhong", cfg.per_mingzhong) || cfg.per_mingzhong < 0)
		{
			return -110;
		}

		if (!GetSubNodeValue(data_element, "per_shanbi", cfg.per_shanbi) || cfg.per_shanbi < 0)
		{
			return -111;
		}

		if (!GetSubNodeValue(data_element, "skill_zengshang_per", cfg.skill_zengshang_per) || cfg.skill_zengshang_per < 0)
		{
			return -112;
		}

		if (!GetSubNodeValue(data_element, "skill_jianshang_per", cfg.skill_jianshang_per) || cfg.skill_jianshang_per < 0)
		{
			return -113;
		}

		m_suit_attr_vec.push_back(cfg);

		data_element = data_element->NextSiblingElement();
	}

	return 0;
}
