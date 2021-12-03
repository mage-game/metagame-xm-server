#include "tianshenhuticonfig.hpp"

#include "servercommon/configcommon.h"
#include "servercommon/servercommon.h"
#include "config/sharedconfig/sharedconfig.h"
#include "item/itempool.h"

TianshenHutiConfig::TianshenHutiConfig() : m_max_taozhuang_type(0), m_max_equip_id(0), m_discount_cfgcount(0)
{

}

TianshenHutiConfig::~TianshenHutiConfig()
{

}

bool TianshenHutiConfig::Init(const std::string &configname, std::string *err)
{
	PUGI_XML_CONFIG_PRE_LOAD;

	{
		// 其他配置信息
		PUGI_XML_LOAD_CONFIG("other", InitOtherCfg);
		// 套装类型
		PUGI_XML_LOAD_CONFIG("taozhuang_type", InitTaozhuangTypeCfg);
		// 装备属性
		PUGI_XML_LOAD_CONFIG("equip_info", InitEquipInfoCfg);
		// 套装属性
		PUGI_XML_LOAD_CONFIG("taozhuang_info", InitTaozhuangInfoCfg);
		// 折扣配置
		PUGI_XML_LOAD_CONFIG("discount_cfg", InitDiscountCfg);
		// 累抽奖励奖池
		PUGI_XML_LOAD_CONFIG("accumulate_reward_item_cfg", InitAccumulateRewardItemPoolCfg);
		// 累抽奖励
		PUGI_XML_LOAD_CONFIG("accumulate_reward_cfg", InitAccumulateRewardCfg);
		// 套装技能
		PUGI_XML_LOAD_CONFIG("taozhaung_skill_cfg", InitTaoZhuangSkillCfg);
	}

	return true;
}

const TianshenHutiEquipConfig * TianshenHutiConfig::GetEquipConfigWithID(TianshenEquipID equip_id)
{
	if (equip_id > 0 && equip_id <= m_max_equip_id && equip_id <= TIANSHENHUTI_MAX_EQUIP_ID)
	{
		return &m_equip_list[equip_id];
	}

	return nullptr;
}

const TianshenHutiEquipConfig * TianshenHutiConfig::GetRandomEquipConfigWithLevel(int level, int new_equip_slot /* = -1 */)
{
	if (level <= 0 || level > TIANSHENHUTI_MAX_EQUIP_LEVEL)
	{
		return nullptr;
	}

	if (new_equip_slot < 0 || new_equip_slot >= TIANSHENHUTI_EQUIP_MAX_COUNT) // 随机部位
	{
		int rand_base = static_cast<int>(m_level_equip_snapshot[level].size());
		if (rand_base <= 0) 
		{
			return nullptr;
		}

		int rand_index = RandomNum(rand_base);
		std::vector<TianshenHutiEquipConfig*>::iterator it = m_level_equip_snapshot[level].begin();
		std::advance(it, rand_index);
		return *it;
	}
	else // 指定部位
	{
		std::vector<TianshenHutiEquipConfig*> fix_slot_list;

		for (std::vector<TianshenHutiEquipConfig*>::iterator equip_cfg_it = m_level_equip_snapshot[level].begin(); 
			equip_cfg_it != m_level_equip_snapshot[level].end(); ++ equip_cfg_it)
		{
			if ((*equip_cfg_it)->slot_index == new_equip_slot)
			{
				fix_slot_list.push_back(*equip_cfg_it);
			}
		}

		int rand_base = static_cast<int>(fix_slot_list.size());
		if (rand_base <= 0) 
		{
			return nullptr;
		}

		int rand_index = RandomNum(rand_base);
		std::vector<TianshenHutiEquipConfig*>::iterator it = fix_slot_list.begin();
		std::advance(it, rand_index);
		return *it;
	}

	return nullptr;
}

const TianshenHutiTaozhuangConfig * TianshenHutiConfig::GetTaozhuangConfig(int taozhuang_type, int need_taozhuang_num)
{
	long long key = ConvertParamToLongLong(need_taozhuang_num, taozhuang_type);
	std::map<long long, TianshenHutiTaozhuangConfig>::iterator it = m_taozhuang_map.find(key);
	if (it != m_taozhuang_map.end())
	{
		return &it->second;
	}

	return nullptr;
}

TianshenEquipID TianshenHutiConfig::RollEquip(int roll_type, const std::set<int> &allow_level_set)
{
	if (roll_type > TIANSHENHUTI_ROLL_TYPE_INVALID && roll_type < TIANSHENHUTI_ROLL_TYPE_MAX && !allow_level_set.empty())
	{
		int total_weight = 0;
		static int roll_weight_list[TIANSHENHUTI_MAX_EQUIP_ID + 1];
		memset(roll_weight_list, 0, sizeof(roll_weight_list));

		for (int i = 0; i <= TIANSHENHUTI_MAX_EQUIP_ID; ++ i)
		{
			const TianshenHutiEquipConfig &equip_cfg = m_equip_list[i];

			if (equip_cfg.IsValid())
			{
				if (allow_level_set.end() != allow_level_set.find(equip_cfg.level) && equip_cfg.level <= TIANSHENHUTI_ROLL_REWARD_EQUIP_MAX_LEVEL)
				{
					total_weight += equip_cfg.roll_weight_list[roll_type];
					roll_weight_list[i] = total_weight;
				}
			}
		}

		if (total_weight > 0)
		{
			int rand_value = RandomNum(total_weight);
			for (int i = 0; i <= TIANSHENHUTI_MAX_EQUIP_ID; ++ i)
			{
				if (rand_value < roll_weight_list[i])
				{
					return m_equip_list[i].equip_id;
				}
			}
		}
	}

	return 0;
}

TianshenHutiDiscountConfig * TianshenHutiConfig::GetDiscountCfg()
{
	const tm *local_tm = EngineAdapter::Instance().LocalTime();
	if (nullptr == local_tm)
	{
		return nullptr;
	}

	int week_day = local_tm->tm_wday;
	if (SUNDAY == week_day)
	{
		week_day = 7;
	}

	int total_weight = week_day * SECOND_PER_DAY + local_tm->tm_hour * SECOND_PER_HOUR + local_tm->tm_min * SECOND_PER_MIN;

	for (int i = 0; i < m_discount_cfgcount && i < TIANSHENHUTI_MAX_DISCOUNT_CFG_COUNT; ++ i)
	{
		TianshenHutiDiscountConfig &free_cfg = m_discount_cfglist[i];

		int cfg_begin_weight = free_cfg.begin_week_day * SECOND_PER_DAY + free_cfg.begin_hour * SECOND_PER_HOUR + free_cfg.begin_minute + SECOND_PER_MIN;
		int cfg_end_weight = free_cfg.end_week_day * SECOND_PER_DAY + free_cfg.end_hour * SECOND_PER_HOUR + free_cfg.end_minute + SECOND_PER_MIN;

		if (total_weight >= cfg_begin_weight && total_weight <= cfg_end_weight)
		{
			return &m_discount_cfglist[i];
		}
	}

	return nullptr;
}

TianshenHutiAccumulateRewardConfig * TianshenHutiConfig::GetAccumulateRewardCfg()
{
	const tm *local_tm = EngineAdapter::Instance().LocalTime();
	if (nullptr == local_tm)
	{
		return nullptr;
	}

	int opengame_day = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerDayIndex() + 1;
	int week_day = local_tm->tm_wday;

	if (week_day != SATURDAY && week_day != SUNDAY)
	{
		return nullptr;
	}

	// 按周六是开服第几天来算
	int diff_day = (SUNDAY == week_day ? 1 : 0);

	opengame_day -= diff_day;
	opengame_day = (opengame_day > 0 ? opengame_day : 1);

	for (auto &cfg : m_accumulate_reward_cfg_map)
	{
		if (cfg.second.opengame_day >= opengame_day)
		{
			return &cfg.second;
		}
	}

	return nullptr;
}

std::vector<TianshenHutiTaoZhuangSkillCfg> & TianshenHutiConfig::GetTaoZhuangSkillCfg()
{
	return m_taozhuang_skill_cfg_vec;
	//for (auto &skill_cfg : m_taozhuang_skill_cfg_vec)
	//{
	//	if (skill_cfg.taozhuang_type == taozhuang_type)
	//	{
	//		return &skill_cfg;
	//	}
	//}
	//return nullptr;
}

int TianshenHutiConfig::GetAccumulateRandomRewardItem(int reward_type)
{
	auto weight_map_cfg_it = m_accumulate_reward_weight_map_cfg.find(reward_type);

	if (m_accumulate_reward_weight_map_cfg.end() != weight_map_cfg_it)
	{
		int total_weight = 0;
		
		for (auto vec_it : weight_map_cfg_it->second)
		{
			total_weight += vec_it.weight;
		}

		if (total_weight <= 0)
		{
			return -1;
		}

		int rand_val = RandomNum(total_weight);

		for (auto vec_it : weight_map_cfg_it->second)
		{
			if (rand_val < vec_it.weight)
			{
				return vec_it.item_id;
			}

			rand_val -= vec_it.weight;
		}

		return -1;
	}

	return -1;
}

int TianshenHutiConfig::InitOtherCfg(PugiXmlNode RootElement)
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

	if (!PugiGetSubNodeValue(data_element, "batch_roll_cost", m_other_cfg.batch_roll_cost) || m_other_cfg.batch_roll_cost <= 0)
	{
		return -3;
	}

	if (!PugiGetSubNodeValue(data_element, "free_times", m_other_cfg.free_times) || m_other_cfg.free_times < 0)
	{
		return -4;
	}

	if (!PugiGetSubNodeValue(data_element, "free_cd", m_other_cfg.free_cd) || m_other_cfg.free_cd < 0)
	{
		return -5;
	}

	if (!PugiGetSubNodeValue(data_element, "taozhuang_skill_need_level", m_other_cfg.taozhuang_skill_need_level) 
		|| m_other_cfg.taozhuang_skill_need_level <= 0 || m_other_cfg.taozhuang_skill_need_level > TIANSHENHUTI_MAX_EQUIP_LEVEL)
	{
		return -6;
	}

	if (!PugiGetSubNodeValue(data_element, "common_roll_item_id", m_other_cfg.common_roll_item_id) || !ITEMPOOL->GetItem(m_other_cfg.common_roll_item_id))
	{
		return -7;
	}

	if (!PugiGetSubNodeValue(data_element, "super_roll_item_id", m_other_cfg.super_roll_item_id) || !ITEMPOOL->GetItem(m_other_cfg.super_roll_item_id))
	{
		return -8;
	}

	if (!PugiGetSubNodeValue(data_element, "batch_roll_item_id", m_other_cfg.batch_roll_item_id) || !ITEMPOOL->GetItem(m_other_cfg.batch_roll_item_id))
	{
		return -9;
	}

	if (!PugiGetSubNodeValue(data_element, "common_roll_item_num", m_other_cfg.common_roll_item_num) || m_other_cfg.common_roll_item_num <= 0)
	{
		return -10;
	}

	if (!PugiGetSubNodeValue(data_element, "super_roll_item_num", m_other_cfg.super_roll_item_num) || m_other_cfg.super_roll_item_num <= 0)
	{
		return -11;
	}

	if (!PugiGetSubNodeValue(data_element, "batch_roll_item_num", m_other_cfg.batch_roll_item_num) || m_other_cfg.batch_roll_item_num <= 0)
	{
		return -12;
	}

	return 0;
}

int TianshenHutiConfig::InitTaozhuangTypeCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		if (m_max_taozhuang_type > TIANSHENHUTI_MAX_EQUIP_TYPE)
		{
			return -1000;
		}

		int taozhuang_type = 0;
		if (!PugiGetSubNodeValue(data_element, "taozhuang_type", taozhuang_type))
		{
			return -1;
		}

		if (taozhuang_type != m_max_taozhuang_type + 1)
		{
			return -2;
		}

		++ m_max_taozhuang_type;
		data_element = data_element.next_sibling();
	}
	return 0;
}

int TianshenHutiConfig::InitEquipInfoCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		int equip_id = 0;
		if (!PugiGetSubNodeValue(data_element, "equip_id", equip_id) || equip_id != m_max_equip_id + 1 || equip_id > TIANSHENHUTI_MAX_EQUIP_ID)
		{
			return -1;
		}
		m_max_equip_id = equip_id;

		TianshenHutiEquipConfig &equip_cfg = m_equip_list[equip_id];
		equip_cfg.equip_id = equip_id;

		if (!PugiGetSubNodeValue(data_element, "slot_index", equip_cfg.slot_index) || equip_cfg.slot_index < 0 || equip_cfg.slot_index >= TIANSHENHUTI_EQUIP_MAX_COUNT)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "level", equip_cfg.level) || equip_cfg.level <= 0 || equip_cfg.level > TIANSHENHUTI_MAX_EQUIP_LEVEL)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "level_taozhuang_type", equip_cfg.level_taozhuang_type) || 
			equip_cfg.level_taozhuang_type <= 0 || equip_cfg.level_taozhuang_type > (m_max_taozhuang_type * TIANSHENHUTI_MAX_EQUIP_LEVEL))
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "taozhuang_type", equip_cfg.taozhuang_type) 
			|| equip_cfg.taozhuang_type <= 0 || equip_cfg.taozhuang_type > m_max_taozhuang_type || equip_cfg.taozhuang_type > TIANSHENHUTI_MAX_EQUIP_TYPE)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "maxhp", equip_cfg.maxhp) || equip_cfg.maxhp < 0)
		{
			return -100;
		}

		if (!PugiGetSubNodeValue(data_element, "gongji", equip_cfg.gongji) || equip_cfg.gongji < 0)
		{
			return -101;
		}

		if (!PugiGetSubNodeValue(data_element, "fangyu", equip_cfg.fangyu) || equip_cfg.fangyu < 0)
		{
			return -102;
		}

		if (!PugiGetSubNodeValue(data_element, "mingzhong", equip_cfg.mingzhong) || equip_cfg.mingzhong < 0)
		{
			return -103;
		}

		if (!PugiGetSubNodeValue(data_element, "shanbi", equip_cfg.shanbi) || equip_cfg.shanbi < 0)
		{
			return -104;
		}

		if (!PugiGetSubNodeValue(data_element, "baoji", equip_cfg.baoji) || equip_cfg.baoji < 0)
		{
			return -105;
		}

		if (!PugiGetSubNodeValue(data_element, "jianren", equip_cfg.jianren) || equip_cfg.jianren < 0)
		{
			return -106;
		}

		int common_roll_weight = 0;
		if (!PugiGetSubNodeValue(data_element, "common_roll_weight", common_roll_weight) || common_roll_weight < 0)
		{
			return -10;
		}

		int super_rollweight = 0;
		if (!PugiGetSubNodeValue(data_element, "super_rollweight", super_rollweight) || super_rollweight < 0)
		{
			return -11;
		}

		int batch_roll_weight = 0;
		if (!PugiGetSubNodeValue(data_element, "batch_roll_weight", batch_roll_weight) || batch_roll_weight < 0)
		{
			return -12;
		}

		equip_cfg.roll_weight_list[TIANSHENHUTI_ROLL_TYPE_COMMON] = common_roll_weight;
		equip_cfg.roll_weight_list[TIANSHENHUTI_ROLL_TYPE_SUPER] = super_rollweight;
		equip_cfg.roll_weight_list[TIANSHENHUTI_ROLL_TYPE_BATCH] = batch_roll_weight;

		m_level_equip_snapshot[equip_cfg.level].push_back(&equip_cfg);
		data_element = data_element.next_sibling();
	}

	bool batch_roll_level_four_zero = true;  // 连抽时必出某阶装备
	bool all_weight_zero[TIANSHENHUTI_ROLL_TYPE_MAX];
	for (int i = 0; i < TIANSHENHUTI_ROLL_TYPE_MAX; ++ i)
	{
		all_weight_zero[i] = true;
	}

	for (int level = 1; level <= TIANSHENHUTI_ROLL_REWARD_EQUIP_MAX_LEVEL; ++ level)
	{
		for (std::vector<TianshenHutiEquipConfig*>::iterator it = m_level_equip_snapshot[level].begin(); it != m_level_equip_snapshot[level].end(); ++ it)
		{
			if ((*it)->roll_weight_list[TIANSHENHUTI_ROLL_TYPE_COMMON] > 0)
			{
				all_weight_zero[TIANSHENHUTI_ROLL_TYPE_COMMON] = false;
			}
			if ((*it)->roll_weight_list[TIANSHENHUTI_ROLL_TYPE_SUPER] > 0)
			{
				all_weight_zero[TIANSHENHUTI_ROLL_TYPE_SUPER] = false;
			}
			if ((*it)->roll_weight_list[TIANSHENHUTI_ROLL_TYPE_BATCH] > 0)
			{
				all_weight_zero[TIANSHENHUTI_ROLL_TYPE_BATCH] = false;

				if (TIANSHENHUTI_ROLL_BATCH_FIX_EQUIP_LEVEL == (*it)->level)  // 连抽必须要有一个必出阶数的权重
				{
					batch_roll_level_four_zero = false;
				}
			}
		}
	}

	for (int i = TIANSHENHUTI_ROLL_TYPE_COMMON; i < TIANSHENHUTI_ROLL_TYPE_MAX; ++ i)
	{
		if (all_weight_zero[i])
		{
			return -40000;
		}
	}

	if (batch_roll_level_four_zero)  // 连抽必出阶数的权重一定要大于0
	{
		return -40001;
	}

	return 0;
}

int TianshenHutiConfig::InitTaozhuangInfoCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		TianshenHutiTaozhuangConfig taozhuang_cfg;

		if (!PugiGetSubNodeValue(data_element, "level_taozhuang_type", taozhuang_cfg.taozhuang_type) || 
			taozhuang_cfg.taozhuang_type <= 0 || taozhuang_cfg.taozhuang_type > (m_max_taozhuang_type * TIANSHENHUTI_MAX_EQUIP_LEVEL))
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "level_taozhuang_num", taozhuang_cfg.need_taozhuang_num) || taozhuang_cfg.need_taozhuang_num <= 0 || taozhuang_cfg.need_taozhuang_num > TIANSHENHUTI_EQUIP_MAX_COUNT)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "skill_num", taozhuang_cfg.skill_num) || taozhuang_cfg.skill_num <= 0 || taozhuang_cfg.skill_num > 3)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "static_injure", taozhuang_cfg.static_injure) || taozhuang_cfg.static_injure < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "rate_injure", taozhuang_cfg.rate_injure) || taozhuang_cfg.rate_injure < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "maxhp", taozhuang_cfg.maxhp) || taozhuang_cfg.maxhp < 0)
		{
			return -100;
		}

		if (!PugiGetSubNodeValue(data_element, "gongji", taozhuang_cfg.gongji) || taozhuang_cfg.gongji < 0)
		{
			return -101;
		}

		if (!PugiGetSubNodeValue(data_element, "fangyu", taozhuang_cfg.fangyu) || taozhuang_cfg.fangyu < 0)
		{
			return -102;
		}

		if (!PugiGetSubNodeValue(data_element, "mingzhong", taozhuang_cfg.mingzhong) || taozhuang_cfg.mingzhong < 0)
		{
			return -103;
		}

		if (!PugiGetSubNodeValue(data_element, "shanbi", taozhuang_cfg.shanbi) || taozhuang_cfg.shanbi < 0)
		{
			return -104;
		}

		if (!PugiGetSubNodeValue(data_element, "baoji", taozhuang_cfg.baoji) || taozhuang_cfg.baoji < 0)
		{
			return -105;
		}

		if (!PugiGetSubNodeValue(data_element, "jianren", taozhuang_cfg.jianren) || taozhuang_cfg.jianren < 0)
		{
			return -106;
		}

		if (!PugiGetSubNodeValue(data_element, "per_gongji", taozhuang_cfg.per_gongji) || taozhuang_cfg.per_gongji < 0)
		{
			return -107;
		}

		if (!PugiGetSubNodeValue(data_element, "per_fangyu", taozhuang_cfg.per_fangyu) || taozhuang_cfg.per_fangyu < 0)
		{
			return -108;
		}

		if (!PugiGetSubNodeValue(data_element, "per_shengming", taozhuang_cfg.per_maxhp) || taozhuang_cfg.per_maxhp < 0)
		{
			return -109;
		}

		long long key = ConvertParamToLongLong(taozhuang_cfg.need_taozhuang_num, taozhuang_cfg.taozhuang_type);
		if (m_taozhuang_map.find(key) != m_taozhuang_map.end())
		{
			return -1000;
		}

		m_taozhuang_map[key] = taozhuang_cfg;
		data_element = data_element.next_sibling();
	}

	return 0;
}

bool DayTimeFormatInvalid(int day_time)
{
	int hour = day_time / 100;
	int minute = day_time % 100;

	return (hour < 0 || hour >= 24 || minute < 0 || minute >= 60);
}

int TianshenHutiConfig::InitDiscountCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int last_time_weight = 0;

	while (!data_element.empty())
	{
		if (m_discount_cfgcount >= TIANSHENHUTI_MAX_DISCOUNT_CFG_COUNT)
		{
			return -1;
		}

		int seq = 0;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq != m_discount_cfgcount)
		{
			return -2;
		}
		TianshenHutiDiscountConfig &discount_cfg = m_discount_cfglist[seq];
		discount_cfg.seq = seq;

		if (!PugiGetSubNodeValue(data_element, "begin_day", discount_cfg.begin_week_day) || discount_cfg.begin_week_day <= 0 || discount_cfg.begin_week_day > 7)
		{
			return -3;
		}
		if (!PugiGetSubNodeValue(data_element, "end_day", discount_cfg.end_week_day) || discount_cfg.end_week_day <= 0 || discount_cfg.end_week_day > 7)
		{
			return -4;
		}

		int begin_time = 0;
		if (!PugiGetSubNodeValue(data_element, "begin_time", begin_time) || DayTimeFormatInvalid(begin_time))
		{
			return -5;
		}

		int end_time = 0;
		if (!PugiGetSubNodeValue(data_element, "end_time", end_time) || DayTimeFormatInvalid(end_time))
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "discount_percent", discount_cfg.discount_percent) || discount_cfg.discount_percent < 0 || discount_cfg.discount_percent > 100)
		{
			return -7;
		}

		discount_cfg.begin_hour = begin_time / 100;
		discount_cfg.begin_minute = begin_time % 100;
		discount_cfg.end_hour = end_time / 100;
		discount_cfg.end_minute = end_time % 100;

		int begin_time_weight = discount_cfg.begin_week_day * SECOND_PER_DAY + discount_cfg.begin_hour * SECOND_PER_HOUR + discount_cfg.begin_minute * SECOND_PER_MIN;
		int end_time_weight = discount_cfg.end_week_day * SECOND_PER_DAY + discount_cfg.end_hour * SECOND_PER_HOUR + discount_cfg.end_minute * SECOND_PER_MIN;

		if (end_time_weight <= begin_time_weight)
		{
			return -7;
		} 

		if (begin_time_weight <= last_time_weight)
		{
			return -8;
		}
		last_time_weight = end_time_weight;

		++ m_discount_cfgcount;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int TianshenHutiConfig::InitAccumulateRewardItemPoolCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int reward_type = 0;
	ItemID reward_item = 0;
	short weight = 0;

	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "reward_type", reward_type))
		{
			return -1;
		}

		std::vector<TianshenHutiAccumulateRewardWeightCfg> &weight_vec = m_accumulate_reward_weight_map_cfg[reward_type];
		if (!PugiGetSubNodeValue(data_element, "reward_item", reward_item) || nullptr == ITEMPOOL->GetItem(reward_item))
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "weight", weight) || weight < 0)
		{
			return -3;
		}

		weight_vec.emplace_back(reward_item, weight);

		data_element = data_element.next_sibling();
	}

	return 0;
}

int TianshenHutiConfig::InitAccumulateRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int opengame_day = 0;
	int last_opengame_day = -1;
	int index = 0;
	int last_index = -1;
	int last_accumulate_times = 0;
	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day <= 0 || opengame_day < last_opengame_day)
		{
			return -2;
		}
		TianshenHutiAccumulateRewardConfig &cfg = m_accumulate_reward_cfg_map[opengame_day];
		cfg.opengame_day = opengame_day;

		if (!PugiGetSubNodeValue(data_element, "index", index) || index < 0 || index <= last_index || index >= TianshenHutiAccumulateRewardConfig::ACCMULATE_REWARD_INDEX_MAX)
		{
			return -3;
		}

		TianshenHutiAccumulateRewardConfig::TianshenHutiAccumulateRewardItem item_cfg;
		item_cfg.index = index;

		if (!PugiGetSubNodeValue(data_element, "accumulate_times", item_cfg.accumulate_times)
			|| item_cfg.accumulate_times <= 0 || item_cfg.accumulate_times <= last_accumulate_times)
		{
			return -4;
		}
		if (opengame_day != last_opengame_day)
		{
			last_index = -1;
			last_accumulate_times = 0;
		}
		else
		{
			last_index = index;
			last_accumulate_times = item_cfg.accumulate_times;
		}

		int item_count = ItemConfigData::ReadConfigList(data_element, "reward", item_cfg.item_list, MAX_ATTACHMENT_ITEM_NUM);
		if (item_count < 0)
		{
			return -10 + item_count;
		}

		if (!PugiGetSubNodeValue(data_element, "reward_type", item_cfg.reward_type))
		{
			return -5;
		}

		cfg.reward_vec.emplace_back(item_cfg);
	
		data_element = data_element.next_sibling();
	}

	return 0;
}

int TianshenHutiConfig::InitTaoZhuangSkillCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		TianshenHutiTaoZhuangSkillCfg cfg;
		if (!PugiGetSubNodeValue(data_element, "taozhuang_type", cfg.taozhuang_type) || cfg.taozhuang_type > TIANSHENHUTI_MAX_EQUIP_TYPE)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "active_skill_id", cfg.active_skill_id))
		{
			return -3;
		}

		m_taozhuang_skill_cfg_vec.emplace_back(cfg);

		data_element = data_element.next_sibling();
	}

	return 0;
}



