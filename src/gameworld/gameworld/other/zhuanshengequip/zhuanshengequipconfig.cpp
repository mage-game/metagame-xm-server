#include "zhuanshengequipconfig.hpp"
#include "servercommon/configcommon.h"
#include "servercommon/servercommon.h"
#include "item/itempool.h"
#include "config/sharedconfig/sharedconfig.h"
#include "obj/character/attribute.hpp"
#include "engineadapter.h"
#include "servercommon/serverconfig/crossconfig.hpp"

ZhuanShengEquipConfig::ZhuanShengEquipConfig() : m_level_cfg_count(0), m_xiuwei_cfg_count(0), m_chou_section_cfg_max_count(0), m_rand_attr_rate_count(0), m_rand_attr_total_rate(0)
{

}

ZhuanShengEquipConfig::~ZhuanShengEquipConfig()
{

}

bool ZhuanShengEquipConfig::Init(const std::string &configname, std::string *err)
{
	char errinfo[1024] = {0};
	int iRet = 0;

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, configname, "ZhuanShengEquipConfig", *err);

	PugiXmlNode RootElement = document.document_element();

	if (RootElement.empty())
	{
		*err = configname + ": xml root node error.";
		return false;
	}

	{
		// 其他配置信息
		PugiXmlNode root_element = RootElement.child("other");
		if (root_element.empty())
		{
			*err = configname + ": no [other].";
			return false;
		}

		iRet = this->InitOtherCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitOtherCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 转生属性配置
		PugiXmlNode root_element = RootElement.child("zhuansheng_attr_cfg");
		if (root_element.empty())
		{
			*err = configname + ": no [zhuansheng_attr_cfg].";
			return false;
		}

		iRet = this->InitZhuanShenglevelCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitZhuanShenglevelCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 修为兑换配置
		PugiXmlNode root_element = RootElement.child("change_xiuwei_cfg");
		if (root_element.empty())
		{
			*err = configname + ": no [change_xiuwei_cfg].";
			return false;
		}

		iRet = this->InitChangeXiuweiCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitChangeXiuweiCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 随机属性数量配置
		PugiXmlNode root_element = RootElement.child("rand_attr_count");
		if (root_element.empty())
		{
			*err = configname + ": no [rand_attr_count].";
			return false;
		}

		iRet = this->InitRandAttrCountCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo, "%s InitRandAttrCountCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 随机属性概率配置
		PugiXmlNode root_element = RootElement.child("rand_attr_rate");
		if (root_element.empty())
		{
			*err = configname + ": no [rand_attr_rate].";
			return false;
		}

		iRet = this->InitRandAttrRateCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo, "%s InitRandAttrRateCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 随机属性值配置
		PugiXmlNode root_element = RootElement.child("rand_attr_val");
		if (root_element.empty())
		{
			*err = configname + ": no [rand_attr_val].";
			return false;
		}

		iRet = this->InitRandAttrValCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo, "%s InitRandAttrValCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 套装加成属性
		PugiXmlNode root_element = RootElement.child("suit_add_attr");
		if (root_element.empty())
		{
			*err = configname + ": no [suit_add_attr].";
			return false;
		}

		iRet = this->InitEquipSuitAddAttrCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo, "%s InitEquipSuitAddAttrCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

int ZhuanShengEquipConfig::InitOtherCfg(PugiXmlNode RootElement)
{

	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}
	
	if (!PugiGetSubNodeValue(data_element, "common_roll_cost", m_other_cfg.common_roll_cost) || m_other_cfg.common_roll_cost <= 0)
	{
		return -1;
	}

	if (!PugiGetSubNodeValue(data_element, "super_roll_cost", m_other_cfg.super_roll_cost) || m_other_cfg.super_roll_cost <= 0)
	{
		return -2;
	}

	if (!PugiGetSubNodeValue(data_element, "super_roll_cost_item_id", m_other_cfg.super_roll_cost_item_id) || NULL == ITEMPOOL->GetItem(m_other_cfg.super_roll_cost_item_id))
	{
		return -3;
	}

	int cold_down_times_h = 0;
	if (!PugiGetSubNodeValue(data_element, "free_cold_down_time_h", cold_down_times_h) || cold_down_times_h <= 0)
	{
		return -4;
	}

	m_other_cfg.free_cold_down_time = cold_down_times_h * SECOND_PER_HOUR;

	if (!PugiGetSubNodeValue(data_element, "grade_3_attr_add_percent_8", m_other_cfg.grade_3_attr_add_percent_8) || m_other_cfg.grade_3_attr_add_percent_8 <= 0)
	{
		return -5;
	}

	if (!PugiGetSubNodeValue(data_element, "grade_5_attr_add_percent_8", m_other_cfg.grade_5_attr_add_percent_8) || m_other_cfg.grade_5_attr_add_percent_8 <= 0)
	{
		return -6;
	}

	return 0;
}


int ZhuanShengEquipConfig::InitZhuanShenglevelCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_level_cfg_count = 0;

	while(!data_element.empty())
	{
		if (m_level_cfg_count >= ZHUANSHENG_ATTR_LEVEL_LIMIT)
		{
			return -20;
		}

		int level = -1;
		if (!PugiGetSubNodeValue(data_element, "level", level) || level != m_level_cfg_count)
		{
			return -1;
		}
	
		ZhuanshengLevelConfig &level_cfg = m_level_cfg_list[level];
		level_cfg.level = level;

		if (!PugiGetSubNodeValue(data_element, "consume_xiuwei", level_cfg.consume_xiuwei) || level_cfg.consume_xiuwei <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "maxhp", level_cfg.maxhp) || level_cfg.maxhp < 0)
		{
			return -101;
		}

	
		if (!PugiGetSubNodeValue(data_element, "extra_exp_add", level_cfg.extra_exp_add) || level_cfg.extra_exp_add < 0)
		{
			return -1011;
		}

		if (!PugiGetSubNodeValue(data_element, "gongji", level_cfg.gongji) || level_cfg.gongji < 0)
		{
			return -102;
		}

		if (!PugiGetSubNodeValue(data_element, "fangyu", level_cfg.fangyu) || level_cfg.fangyu < 0)
		{
			return -103;
		}

		if (!PugiGetSubNodeValue(data_element, "mingzhong", level_cfg.mingzhong) || level_cfg.mingzhong < 0)
		{
			return -104;
		}

		if (!PugiGetSubNodeValue(data_element, "shanbi", level_cfg.shanbi) || level_cfg.shanbi < 0)
		{
			return -105;
		}

		if (!PugiGetSubNodeValue(data_element, "baoji", level_cfg.baoji) || level_cfg.baoji < 0)
		{
			return -106;
		}

		if (!PugiGetSubNodeValue(data_element, "jianren", level_cfg.jianren) || level_cfg.jianren < 0)
		{
			return -107;
		}

		++ m_level_cfg_count;
		data_element = data_element.next_sibling();
	}
	
	return 0;

}

int ZhuanShengEquipConfig::InitChangeXiuweiCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_xiuwei_cfg_count = 0;

	while (!data_element.empty())
	{
		int seq = -1;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq != m_xiuwei_cfg_count)
		{
			return -1;
		}
		
		ZhuanshengXiuweiConfig &xiuwei_cfg = m_xiuwei_cfg_list[seq];
		xiuwei_cfg.seq = seq;

		if (!PugiGetSubNodeValue(data_element, "consume_gold", xiuwei_cfg.consume_gold) || xiuwei_cfg.consume_gold < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "consume_coin", xiuwei_cfg.consume_coin) || xiuwei_cfg.consume_coin < 0)
		{
			return -3;
		}

		if (xiuwei_cfg.consume_coin <= 0 && xiuwei_cfg.consume_gold <= 0)		// 必须消耗元宝或者铜币
		{
			return -5;
		}

		if (xiuwei_cfg.consume_coin > 0 && xiuwei_cfg.consume_gold > 0)		// 只能消耗其中一个
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "reward_xiuwei", xiuwei_cfg.reward_xiuwei) || xiuwei_cfg.reward_xiuwei <= 0)
		{
			return -7;
		}

		++ m_xiuwei_cfg_count;

		data_element = data_element.next_sibling();
	}

	return 0;
}

int ZhuanShengEquipConfig::InitChouCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int last_start_day = -1;	// 上一个时段的开启天
	int last_end_day = 0;	// 上一个时段的结束天

	m_chou_section_cfg_max_count = 0;

	while (!data_element.empty())
	{
		if (m_chou_section_cfg_max_count > ZHUANSHENG_EQUIP_SECTION_MAX_COUNT)
		{
			return -20;
		}

		int section_start = 0;
		if (!PugiGetSubNodeValue(data_element, "section_start", section_start) || section_start <= 0)
		{
			return -1;
		}
		
		// 配置开始天数从1开始
		if (-1 == last_start_day && 1 !=section_start)
		{
			return -2;
		}

		int section_end = -1;
		if (!PugiGetSubNodeValue(data_element, "section_end", section_end) || section_end < 0)
		{
			return -3;
		}

		if (0 == section_end) // 0 代表无穷
		{
			section_end = INT_MAX;
		}

		if (section_end <= section_start)
		{
			return -4;
		}

		if (section_start != last_start_day || section_end != last_end_day)
		{
			// 两个时间段要相连 切换时段
			if (section_start != last_end_day + 1)
			{
				return -10;
			}

			++ m_chou_section_cfg_max_count;
		}

		ZhuanshengChouSectionConfig &section_cfg = m_chou_section_cfg_list[m_chou_section_cfg_max_count - 1];

		if (section_cfg.chou_cfg_count > ZHUANSHENG_CHOU_CFG_LIMIT)
		{
			return -21;
		}

		ZhuanshengChouConfig &chou_cfg = section_cfg.chou_cfg_list[section_cfg.chou_cfg_count];

		section_cfg.section_start = section_start;
		section_cfg.section_end = section_end;

		last_start_day = section_start;
		last_end_day = section_end;

		if (!PugiGetSubNodeValue(data_element, "seq", chou_cfg.seq) || chou_cfg.seq != section_cfg.chou_cfg_count)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "reward_equip_id", chou_cfg.reward_equip_id) || chou_cfg.reward_equip_id <= 0)
		{
			return -13;
		}

		if (!PugiGetSubNodeValue(data_element, "weight", chou_cfg.weight) || chou_cfg.weight < 0)
		{
			return -15;
		}

		section_cfg.total_weight += chou_cfg.weight;
		++ section_cfg.chou_cfg_count;

		data_element = data_element.next_sibling();
	}

	return 0;
}

int ZhuanShengEquipConfig::InitRandAttrCountCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		int type = -1;
		if (!PugiGetSubNodeValue(data_element, "type", type) || ZHUANSHENG_EQUIP_COLOR_TYPE_MIN >= type|| ZHUANSHENG_EQUIP_COLOR_TYPE_MAX <= type)
		{
			return -1;
		}
		int index = type - 1;

		if (!PugiGetSubNodeValue(data_element, "rand_attr_0_rate", m_rand_attr_count_cfg_list[index].rand_attr_0_rate) || m_rand_attr_count_cfg_list[index].rand_attr_0_rate < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "rand_attr_1_rate", m_rand_attr_count_cfg_list[index].rand_attr_1_rate) || m_rand_attr_count_cfg_list[index].rand_attr_1_rate < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "rand_attr_2_rate", m_rand_attr_count_cfg_list[index].rand_attr_2_rate) || m_rand_attr_count_cfg_list[index].rand_attr_2_rate < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "rand_attr_3_rate", m_rand_attr_count_cfg_list[index].rand_attr_3_rate) || m_rand_attr_count_cfg_list[index].rand_attr_3_rate < 0)
		{
			return -5;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int ZhuanShengEquipConfig::InitRandAttrRateCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int count = 0;
	while (!data_element.empty())
	{
		if (count >= MAX_ZHUANSHENG_EQUIP_RAND_ATTR_MAX)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "attr_type", m_rand_attr_rate_list[count].attr_type))
		{
			return -2;
		}
		
		if (m_rand_attr_rate_list[count].attr_type < ZRS_TYPE_ADDEXP_MIN || m_rand_attr_rate_list[count].attr_type >= ZRS_TYPE_ADDEXP_MAX)
		{
			if (!CharIntAttrs::IsRoleBaseAttr(m_rand_attr_rate_list[count].attr_type))
			{
				return -20;
			}
		}

		if (!PugiGetSubNodeValue(data_element, "rate", m_rand_attr_rate_list[count].rate) || m_rand_attr_rate_list[count].rate <= 0)
		{
			return -3;
		}

		m_rand_attr_total_rate += m_rand_attr_rate_list[count].rate;

		++ count;
		data_element = data_element.next_sibling();
	}

	m_rand_attr_rate_count = count;

	return 0;
}

int ZhuanShengEquipConfig::InitRandAttrValCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int last_level = 0;
	int last_color = 0;
	while (!data_element.empty())
	{
		int equip_level = 0;
		if (!PugiGetSubNodeValue(data_element, "equip_level", equip_level) || equip_level <= 0 || equip_level > MAX_ROLE_LEVEL)
		{
			return -3;
		}

		if (last_level != equip_level)
		{
			last_color = 0;
		}

		int equip_color = 0;
		if (!PugiGetSubNodeValue(data_element, "equip_color", equip_color) || ZHUANSHENG_EQUIP_COLOR_TYPE_MIN >= equip_color || ZHUANSHENG_EQUIP_COLOR_TYPE_MAX <= equip_color)
		{
			return -4;
		}

		if (last_color != equip_color && equip_color != last_color + 1)
		{
			return -5;
		}

		last_color = equip_color;
		last_level = equip_level;

		ZhuanShengEquipRandAttrValCfg &rand_val_cfg = m_rand_attr_val_cfg_map[equip_level];
		ZhuanShengEquipColorAttrValCfg &val_cfg = rand_val_cfg.color_attr_list[equip_color - 1];
		val_cfg.equip_color = equip_color;
		if (val_cfg.attr_count >= MAX_ZHUANSHENG_EQUIP_RAND_ATTR_MAX)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "attr_type", val_cfg.attr_val_list[val_cfg.attr_count].attr_type))
		{
			return -7;
		}

		if (val_cfg.attr_val_list[val_cfg.attr_count].attr_type < ZRS_TYPE_ADDEXP_MIN || val_cfg.attr_val_list[val_cfg.attr_count].attr_type >= ZRS_TYPE_ADDEXP_MAX)
		{
			if (!CharIntAttrs::IsRoleBaseAttr(val_cfg.attr_val_list[val_cfg.attr_count].attr_type))
			{
				return -50;
			}
		}

		if (!PugiGetSubNodeValue(data_element, "attr_value_max", val_cfg.attr_val_list[val_cfg.attr_count].attr_val_max) || val_cfg.attr_val_list[val_cfg.attr_count].attr_val_max <= 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(data_element, "attr_value_min", val_cfg.attr_val_list[val_cfg.attr_count].attr_val_min) || val_cfg.attr_val_list[val_cfg.attr_count].attr_val_min <= 0)
		{
			return -9;
		}

		++ val_cfg.attr_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int ZhuanShengEquipConfig::InitEquipSuitAddAttrCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		int grade = 0;
		if (!PugiGetSubNodeValue(data_element, "grade", grade) || grade <= 0 || grade > ZHUANSHENG_EQUIP_GRADE_LIMIT)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "maxhp", m_equip_suit_add_attr_list[grade].max_hp) || m_equip_suit_add_attr_list[grade].max_hp < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "gongji", m_equip_suit_add_attr_list[grade].gongji) || m_equip_suit_add_attr_list[grade].gongji < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "fangyu", m_equip_suit_add_attr_list[grade].fangyu) || m_equip_suit_add_attr_list[grade].fangyu < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "min_gongji", m_equip_suit_add_attr_list[grade].min_gongji) || m_equip_suit_add_attr_list[grade].min_gongji < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "baoji", m_equip_suit_add_attr_list[grade].baoji) || m_equip_suit_add_attr_list[grade].baoji < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "per_pofang", m_equip_suit_add_attr_list[grade].per_pofang) || m_equip_suit_add_attr_list[grade].per_pofang < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(data_element, "per_mianshang", m_equip_suit_add_attr_list[grade].per_mianshang) || m_equip_suit_add_attr_list[grade].per_mianshang < 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(data_element, "per_to_role_shang", m_equip_suit_add_attr_list[grade].per_to_role_shang) || m_equip_suit_add_attr_list[grade].per_to_role_shang < 0 || m_equip_suit_add_attr_list[grade].per_to_role_shang >= 10000)
		{
			return -9;
		}
		
		if (!PugiGetSubNodeValue(data_element, "per_to_role_mianshang", m_equip_suit_add_attr_list[grade].per_to_role_mianshang) || m_equip_suit_add_attr_list[grade].per_to_role_mianshang < 0 || m_equip_suit_add_attr_list[grade].per_to_role_mianshang >= 10000)
		{
			return -10;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

const ZhuanshengLevelConfig * ZhuanShengEquipConfig::GetLevelCfg(int level)
{
	if (level < 0 || level >= ZHUANSHENG_ATTR_LEVEL_LIMIT || level >= m_level_cfg_count)
	{
		return NULL;
	}

	return &m_level_cfg_list[level];

}


const ZhuanshengXiuweiConfig * ZhuanShengEquipConfig::GetXiuweiCfg(int times)
{
	if (times < 0 || times >= m_xiuwei_cfg_count || times >= ZHUANSHENG_XIUWEI_CFG_LIMIT)
	{
		return NULL;
	}

	return &m_xiuwei_cfg_list[times];
}

const ZhuanshengChouConfig * ZhuanShengEquipConfig::GetRandomChouResult()
{
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return NULL;
	}
	time_t now_time = EngineAdapter::Instance().Time();
	time_t open_server_timestamp = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerZeroTime();
	int act_real_open_day = GetDayIndex(open_server_timestamp, now_time) + 1; // 开启时的开服天数，1~N

	if (act_real_open_day <= 0)
	{
		act_real_open_day = 1;
	}

	
	for (int i = 0; i <= m_chou_section_cfg_max_count; ++ i)
	{
		if (act_real_open_day >= m_chou_section_cfg_list[i].section_start &&
			act_real_open_day <= m_chou_section_cfg_list[i].section_end &&
			m_chou_section_cfg_list[i].total_weight > 0)
		{
			int rand_weight = RandomNum(m_chou_section_cfg_list[i].total_weight);

			for (int index = 0; index < m_chou_section_cfg_list[i].chou_cfg_count; ++ index)
			{
				if (rand_weight < m_chou_section_cfg_list[i].chou_cfg_list[index].weight)
				{
					return &m_chou_section_cfg_list[i].chou_cfg_list[index];
				}

				rand_weight -= m_chou_section_cfg_list[i].chou_cfg_list[index].weight;
			}
		}
	}

	return NULL;
}

const ZhuanShengEquipSuitAddAttr * ZhuanShengEquipConfig::GetEquipSuitAddAttr(int grade)
{
	if (grade <= 0 || grade > ZHUANSHENG_EQUIP_GRADE_LIMIT)
	{
		return NULL;
	}

	return &m_equip_suit_add_attr_list[grade];
}

int ZhuanShengEquipConfig::GetZhuanshengRandAttrCount(int type)
{
	if (ZHUANSHENG_EQUIP_COLOR_TYPE_MIN >= type || ZHUANSHENG_EQUIP_COLOR_TYPE_MAX <= type)
	{
		return 0;
	}

	int index = type - 1;
	int total_rate = m_rand_attr_count_cfg_list[index].rand_attr_0_rate + m_rand_attr_count_cfg_list[index].rand_attr_1_rate + 
		m_rand_attr_count_cfg_list[index].rand_attr_2_rate + m_rand_attr_count_cfg_list[index].rand_attr_3_rate;

	if (total_rate <= 0)
	{
		return 0;
	}

	int rand_rate = RandomNum(total_rate);

	if (rand_rate < m_rand_attr_count_cfg_list[index].rand_attr_0_rate)
	{
		return 0;
	}

	rand_rate -= m_rand_attr_count_cfg_list[index].rand_attr_0_rate;
	if (rand_rate < m_rand_attr_count_cfg_list[index].rand_attr_1_rate)
	{
		return 1;
	}

	rand_rate -= m_rand_attr_count_cfg_list[index].rand_attr_1_rate;
	if (rand_rate < m_rand_attr_count_cfg_list[index].rand_attr_2_rate)
	{
		return 2;
	}

	rand_rate -= m_rand_attr_count_cfg_list[index].rand_attr_2_rate;
	if (rand_rate < m_rand_attr_count_cfg_list[index].rand_attr_3_rate)
	{
		return 3;
	}

	return 0;
}

void ZhuanShengEquipConfig::GetZhuanshengRandAttrType(int attr_count, char rand_attr_type_list[ItemParamDataStruct::RAND_ATTR_NUM])
{
	if (attr_count <= 0 || attr_count > ItemParamDataStruct::RAND_ATTR_NUM)
	{
		return;
	}

	ZhuanShengEquipRandAttrRate temp_rand_attr_rate_list[MAX_ZHUANSHENG_EQUIP_RAND_ATTR_MAX];
	memcpy(temp_rand_attr_rate_list, m_rand_attr_rate_list, sizeof(m_rand_attr_rate_list));
	int temp_rand_attr_total_rate = m_rand_attr_total_rate;

	for (int i = 0; i < attr_count; i++)
	{
		if (temp_rand_attr_total_rate <= 0)
		{
			return;
		}

		int rand_rate = RandomNum(temp_rand_attr_total_rate);

		for (int j = 0; j < m_rand_attr_rate_count && j < MAX_ZHUANSHENG_EQUIP_RAND_ATTR_MAX; j++)
		{
			if (rand_rate <= temp_rand_attr_rate_list[j].rate)
			{
				rand_attr_type_list[i] = temp_rand_attr_rate_list[j].attr_type;
				temp_rand_attr_total_rate -= temp_rand_attr_rate_list[j].rate;
				temp_rand_attr_rate_list[j].rate = 0;

				break;
			}
			else
			{
				rand_rate -= temp_rand_attr_rate_list[j].rate;
			}
		}
	}
}

int ZhuanShengEquipConfig::GetZhuanshengRandAttrValue(int equip_level, int equip_color, int attr_type)
{
	std::map<int, ZhuanShengEquipRandAttrValCfg>::iterator it = m_rand_attr_val_cfg_map.find(equip_level);
	if (it == m_rand_attr_val_cfg_map.end())
	{
		return 0;
	}

	if (ZHUANSHENG_EQUIP_COLOR_TYPE_MIN >= equip_color || ZHUANSHENG_EQUIP_COLOR_TYPE_MAX <= equip_color)
	{
		return 0;
	}

	ZhuanShengEquipColorAttrValCfg &val_cfg = it->second.color_attr_list[equip_color - 1];
	for (int i = 0; i < val_cfg.attr_count && i < MAX_ZHUANSHENG_EQUIP_RAND_ATTR_MAX; i++)
	{
		if (val_cfg.attr_val_list[i].attr_type == attr_type)
		{
			int attr_value = val_cfg.attr_val_list[i].attr_val_min;
			int delta_val = val_cfg.attr_val_list[i].attr_val_max - val_cfg.attr_val_list[i].attr_val_min;
			if (delta_val > 0)
			{
				attr_value += RandomNum(delta_val);
			}

			return attr_value;
		}
	}

	return 0;
}


