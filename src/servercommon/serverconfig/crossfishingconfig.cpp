

#include "crossfishingconfig.hpp"
#include "servercommon/configcommon.h"
#include "servercommon/serverconfig/configstruct.h"
#include <limits>
#include <set>

CrossFishingConfig::CrossFishingConfig() : m_activity_open_time_cfg_list_count(0), m_fish_cfg_count(0), m_fish_combination_cfg_count(0), m_fish_bait_cfg_count(0),
m_treasure_cfg_count(0), m_steal_buy_count(0), m_fishing_score_reward_cfg_count(0)
{

}

CrossFishingConfig::~CrossFishingConfig()
{

}

CrossFishingConfig & CrossFishingConfig::Instance()
{
	static CrossFishingConfig _instance;
	return _instance;
}

bool CrossFishingConfig::Init(const std::string &path, std::string *err)
{
	int iRet = 0;
	char errinfo[1024] = { 0 };

	TiXmlDocument document;
	if (path == "" || !document.LoadFile(path))
	{
		*err = path + "：Load CrossFishingConfig Error,\n" + document.ErrorDesc();
		return false;
	}

	TiXmlElement *RootElement = document.RootElement();
	if (NULL == RootElement)
	{
		*err = path + ": xml root node error.";
		return false;
	}

	{
		// 其他配置信息
		TiXmlElement *other_element = RootElement->FirstChildElement("other");
		if (NULL == other_element)
		{
			*err = path + ": no [other].";
			return false;
		}

		iRet = this->InitOtherCfg(other_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitOtherCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 鱼配置
		TiXmlElement *other_element = RootElement->FirstChildElement("fish");
		if (NULL == other_element)
		{
			*err = path + ": no [fish].";
			return false;
		}

		iRet = this->InitFishCfg(other_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitFishCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 组合配置
		TiXmlElement *other_element = RootElement->FirstChildElement("combination");
		if (NULL == other_element)
		{
			*err = path + ": no [combination].";
			return false;
		}

		iRet = this->InitCombinationCfg(other_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitCombinationCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 鱼饵配置
		TiXmlElement *other_element = RootElement->FirstChildElement("fish_bait");
		if (NULL == other_element)
		{
			*err = path + ": no [fish_bait].";
			return false;
		}

		iRet = this->InitFishBaitCfg(other_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitFishBaitCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 事件配置
		TiXmlElement *other_element = RootElement->FirstChildElement("event");
		if (NULL == other_element)
		{
			*err = path + ": no [event].";
			return false;
		}

		iRet = this->InitEventCfg(other_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitEventCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 宝箱配置
		TiXmlElement *other_element = RootElement->FirstChildElement("treasure");
		if (NULL == other_element)
		{
			*err = path + ": no [treasure].";
			return false;
		}

		iRet = this->InitTreasureCfg(other_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitTreasureCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 偷鱼次数购买配置
		TiXmlElement *other_element = RootElement->FirstChildElement("steal_count_buy");
		if (NULL == other_element)
		{
			*err = path + ": no [steal_count_buy].";
			return false;
		}

		iRet = this->InitStealCountBuyCfg(other_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitStealCountBuyCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 积分奖励
		TiXmlElement *score_element = RootElement->FirstChildElement("score_reward");
		if (NULL == score_element)
		{
			*err = path + ": no [score_reward].";
			return false;
		}

		iRet = this->InitScoreRewardCfg(score_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitScoreRewardCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 活动开启时间
		TiXmlElement *open_element = RootElement->FirstChildElement("activity_open_time");
		if (NULL == open_element)
		{
			*err = path + ": no [activity_open_time].";
			return false;
		}

		iRet = this->InitActivityOpenTimeCfg(open_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitActivityOpenTimeCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 传闻配置
		TiXmlElement *open_element = RootElement->FirstChildElement("broadcast_cfg");
		if (NULL == open_element)
		{
			*err = path + ": no [broadcast_cfg].";
			return false;
		}

		iRet = this->InitBroadcastCfg(open_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitBroadcastCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

int CrossFishingConfig::InitOtherCfg(TiXmlElement *RootElement)
{
	TiXmlElement *data_element = RootElement->FirstChildElement("data");
	if (NULL == data_element)
	{
		return -10000;
	}

	int is_open = 0;
	if (!GetSubNodeValue(data_element, "is_open", is_open) || is_open < 0)
	{
		return -1;
	}

	m_other_cfg.is_open = is_open > 0 ? true : false;

	if (!GetSubNodeValue(data_element, "open_level", m_other_cfg.open_level) || m_other_cfg.open_level < 0)
	{
		return -1;
	}

	if (!GetSubNodeValue(data_element, "sceneid", m_other_cfg.sceneid) || m_other_cfg.sceneid <= 0)
	{
		return -2;
	}

	if (!GetSubNodeValue(data_element, "enter_pos_x", m_other_cfg.enter_pos.x) || m_other_cfg.enter_pos.x < 0)
	{
		return -3;
	}

	if (!GetSubNodeValue(data_element, "enter_pos_y", m_other_cfg.enter_pos.y) || m_other_cfg.enter_pos.y < 0)
	{
		return -4;
	}

	if (!GetSubNodeValue(data_element, "event_max_cd_s", m_other_cfg.event_max_cd_s) || m_other_cfg.event_max_cd_s <= 0)
	{
		return -5;
	}

	if (!GetSubNodeValue(data_element, "event_min_cd_s", m_other_cfg.event_min_cd_s) || m_other_cfg.event_min_cd_s <= 0 || m_other_cfg.event_min_cd_s >= m_other_cfg.event_max_cd_s)
	{
		return -6;
	}

	if (!GetSubNodeValue(data_element, "pull_count_down_s", m_other_cfg.pull_count_down_s) || m_other_cfg.pull_count_down_s <= 0)
	{
		return -7;
	}

	if (!GetSubNodeValue(data_element, "reduce_cd_percent_ingroup_2", m_other_cfg.reduce_cd_percent_ingroup_2) || m_other_cfg.reduce_cd_percent_ingroup_2 < 0 || m_other_cfg.reduce_cd_percent_ingroup_2 >= 100)
	{
		return -8;
	}

	if (!GetSubNodeValue(data_element, "reduce_cd_percent_ingroup_3", m_other_cfg.reduce_cd_percent_ingroup_3) || m_other_cfg.reduce_cd_percent_ingroup_3 < 0 || m_other_cfg.reduce_cd_percent_ingroup_3 >= 100)
	{
		return -9;
	}

	if (!GetSubNodeValue(data_element, "get_bigfish_rate_1", m_other_cfg.get_bigfish_rate_1) || m_other_cfg.get_bigfish_rate_1 <= 0)
	{
		return -10;
	}

	if (!GetSubNodeValue(data_element, "get_bigfish_rate_2", m_other_cfg.get_bigfish_rate_2) || m_other_cfg.get_bigfish_rate_2 <= 0)
	{
		return -11;
	}

	if (!GetSubNodeValue(data_element, "get_bigfish_rate_3", m_other_cfg.get_bigfish_rate_3) || m_other_cfg.get_bigfish_rate_3 <= 0)
	{
		return -12;
	}

	if (!GetSubNodeValue(data_element, "give_bait_count", m_other_cfg.give_bait_count) || m_other_cfg.give_bait_count < 0)
	{
		return -13;
	}

	if (!GetSubNodeValue(data_element, "max_rubber_count", m_other_cfg.max_robbed_count) || m_other_cfg.max_robbed_count <= 0)
	{
		return -14;
	}

	if (!GetSubNodeValue(data_element, "net_get_fish_max_count", m_other_cfg.net_get_fish_max_count) || m_other_cfg.net_get_fish_max_count <= 0)
	{
		return -15;
	}

	if (!GetSubNodeValue(data_element, "oil_reduce_event_cd_s", m_other_cfg.oil_reduce_event_cd_s) || m_other_cfg.oil_reduce_event_cd_s <= 0)
	{
		return -16;
	}

	if (!GetSubNodeValue(data_element, "oil_special_status_duration", m_other_cfg.special_status_oil_duration_s) || m_other_cfg.special_status_oil_duration_s <= 0)
	{
		return -17;
	}

	if (!GetSubNodeValue(data_element, "auto_fishing_need_gold", m_other_cfg.auto_fishing_need_gold) || m_other_cfg.auto_fishing_need_gold <= 0)
	{
		return -18;
	}

	if (!GetSubNodeValue(data_element, "steal_count", m_other_cfg.steal_count) || m_other_cfg.steal_count <= 0)
	{
		return -19;
	}

	if (!GetSubNodeValue(data_element, "be_stealed_count", m_other_cfg.be_stealed_count) || m_other_cfg.be_stealed_count <= 0)
	{
		return -20;
	}

	if (!GetSubNodeValue(data_element, "steal_succ_rate", m_other_cfg.steal_succ_rate) || m_other_cfg.steal_succ_rate <= 0)
	{
		return -21;
	}

	if (!GetSubNodeValue(data_element, "player_fishing_range", m_other_cfg.player_fishing_range) || m_other_cfg.player_fishing_range <= 0)
	{
		return -22;
	}

	if (!GetSubNodeValue(data_element, "fishing_gather_id", m_other_cfg.fishing_gather_id) || m_other_cfg.fishing_gather_id <= 0)
	{
		return -23;
	}

	if (!GetSubNodeValue(data_element, "spear_get_fish_max_count", m_other_cfg.spear_get_fish_max_count) || m_other_cfg.spear_get_fish_max_count <= 0)
	{
		return -24;
	}

	if (!GetSubNodeValue(data_element, "fishing_gather_times", m_other_cfg.fishing_gather_times) || m_other_cfg.fishing_gather_times < 1000)
	{
		return -25;
	}

	if (!GetSubNodeValue(data_element, "rank_title_id", m_other_cfg.rank_title_id) || m_other_cfg.rank_title_id < 0)
	{
		return -26;
	}

	if (!GetSubNodeValue(data_element, "rank_title_item_id", m_other_cfg.rank_title_item_id) || m_other_cfg.rank_title_item_id < 0)
	{
		return -27;
	}


	return 0;
}

int CrossFishingConfig::InitFishCfg(TiXmlElement *RootElement)
{
	TiXmlElement *data_element = RootElement->FirstChildElement("data");
	if (NULL == data_element)
	{
		return -10000;
	}

	m_fish_cfg_count = 0;

	while (NULL != data_element)
	{
		if (m_fish_cfg_count >= FISHING_FISH_TYPE_MAX_COUNT)
		{
			return -1000;
		}

		int type = 0;
		if (!GetSubNodeValue(data_element, "type", type) || type != m_fish_cfg_count + 1)
		{
			return -1;
		}

		CrossFishingFishConfig &cfg = m_fish_cfg_list[type];
		cfg.type = type;

		if (!GetSubNodeValue(data_element, "score", cfg.score) || cfg.score < 0)
		{
			return -2;
		}

		if (!GetSubNodeValue(data_element, "be_stealed_rate", cfg.be_stealed_rate) || cfg.be_stealed_rate < 0)
		{
			return -4;
		}

		if (!GetSubNodeValue(data_element, "is_rare", cfg.is_rare) || cfg.is_rare < 0)
		{
			return -5;
		}

		if (!GetSubNodeValue(data_element, "steal_score", cfg.steal_score) || cfg.steal_score < 0)
		{
			return -6;
		}

		++m_fish_cfg_count;
		data_element = data_element->NextSiblingElement();
	}

	return 0;
}

int CrossFishingConfig::InitCombinationCfg(TiXmlElement* RootElement)
{
	TiXmlElement *data_element = RootElement->FirstChildElement("data");
	if (NULL == data_element)
	{
		return -10000;
	}

	m_fish_combination_cfg_count = 0;

	while (NULL != data_element)
	{
		if (m_fish_combination_cfg_count > FISHING_FISH_LEAST_COUNT_CFG_MAX_COUNT)
		{
			return -1000;
		}

		const char fish_type_field_base[] = "fish_type_";
		char real_fish_type_count_field[sizeof(fish_type_field_base) + 1] = { 0 };
		for (int i = FISHING_FISH_TYPE_QIUDAOYU; i < FISHING_FISH_TYPE_COUNT; ++i)
		{
			SNPRINTF(real_fish_type_count_field, sizeof(real_fish_type_count_field), "%s%d", fish_type_field_base, i);
			int &least_count = m_fish_least_count_cfg_list[m_fish_combination_cfg_count].least_count_list[i];
			if (GetSubNodeValue(data_element, real_fish_type_count_field, least_count))
			{
				if (least_count < 0)
				{
					return -1;
				}
			}
		}

		TiXmlElement* item_element = data_element->FirstChildElement("reward_item");
		if (nullptr == item_element)
		{
			return -100;
		}

		if (!m_fish_least_count_cfg_list[m_fish_combination_cfg_count].reward_item.ReadConfigNoCheck(item_element))
		{
			return -2;
		}

		if (!GetSubNodeValue(data_element, "reward_score", m_fish_least_count_cfg_list[m_fish_combination_cfg_count].reward_score) || m_fish_least_count_cfg_list[m_fish_combination_cfg_count].reward_score < 0)
		{
			return -3;
		}

		++m_fish_combination_cfg_count;
		data_element = data_element->NextSiblingElement();
	}

	return 0;
}

int CrossFishingConfig::InitFishBaitCfg(TiXmlElement *RootElement)
{
	TiXmlElement *data_element = RootElement->FirstChildElement("data");
	if (NULL == data_element)
	{
		return -10000;
	}

	m_fish_bait_cfg_count = 0;

	while (NULL != data_element)
	{
		if (m_fish_bait_cfg_count >= FISHING_FISH_BAIT_CFG_MAX_COUNT)
		{
			return -1000;
		}

		int type = 0;
		if (!GetSubNodeValue(data_element, "type", type) || type != m_fish_bait_cfg_count)
		{
			return -1;
		}

		CrossFishingFishBaitConfig &cfg = m_fish_bait_cfg_list[type];
		cfg.type = type;

		if (!GetSubNodeValue(data_element, "fail_rate", cfg.fish_rate_list[0]) || cfg.fish_rate_list[0] < 0)
		{
			return -1;
		}

		if (!GetSubNodeValue(data_element, "item_id", cfg.item_id))
		{
			return -2;
		}

		if (!GetSubNodeValue(data_element, "gold_price", cfg.gold_price) || cfg.gold_price <= 0)
		{
			return -3;
		}

		const char rate_field_base[] = "fish_type_";
		char real_rate_field[sizeof(rate_field_base) + 1] = { 0 };
		for (int i = FISHING_FISH_TYPE_NOFISH + 1; i < FISHING_FISH_TYPE_MAX_COUNT; ++i)
		{
			SNPRINTF(real_rate_field, sizeof(real_rate_field), "%s%d", rate_field_base, i);
			if (!GetSubNodeValue(data_element, real_rate_field, cfg.fish_rate_list[i]) || cfg.fish_rate_list[i] < 0)
			{
				return -(2 + i);
			}

			if (cfg.fish_rate_list[i] > 0 && m_fish_cfg_list[i].score <= 0)
			{
				return -(10 + i);
			}
		}

		int total_rate = 0;
		for (int i = 0; i < FISHING_FISH_TYPE_MAX_COUNT; ++i)
		{
			total_rate += cfg.fish_rate_list[i];
		}
		if (total_rate != 100)
		{
			return -(2000 + type);
		}

		++m_fish_bait_cfg_count;
		data_element = data_element->NextSiblingElement();
	}

	return 0;
}

int CrossFishingConfig::InitEventCfg(TiXmlElement *RootElement)
{
	TiXmlElement *data_element = RootElement->FirstChildElement("data");
	if (NULL == data_element)
	{
		return -10000;
	}

	int event_cfg_count = 0;

	while (NULL != data_element)
	{
		if (event_cfg_count >= CrossFishingEventConfig::EVENT_TYPE_COUNT)
		{
			return -1000;
		}

		int type = 0;
		if (!GetSubNodeValue(data_element, "type", type) || type != event_cfg_count)
		{
			return -1;
		}

		CrossFishingEventConfig &cfg = m_event_cfg_list[type];
		cfg.type = type;

		if (!GetSubNodeValue(data_element, "trigger_weight", cfg.trigger_weight) || cfg.trigger_weight < 0)
		{
			return -2;
		}

		if (!GetSubNodeValue(data_element, "low_trigger_weight", cfg.low_trigger_weight) || cfg.low_trigger_weight < 0)
		{
			return -3;
		}

		++event_cfg_count;
		data_element = data_element->NextSiblingElement();
	}

	if (event_cfg_count != CrossFishingEventConfig::EVENT_TYPE_COUNT)
	{
		return -2000;
	}

	return 0;
}

int CrossFishingConfig::InitTreasureCfg(TiXmlElement *RootElement)
{
	TiXmlElement *data_element = RootElement->FirstChildElement("data");
	if (NULL == data_element)
	{
		return -10000;
	}

	m_treasure_cfg_count = 0;
	int total_weight = 0;

	while (NULL != data_element)
	{
		if (m_treasure_cfg_count >= FISHING_TREASURE_CFG_MAX_COUNT)
		{
			return -1000;
		}

		int seq = 0;
		if (!GetSubNodeValue(data_element, "seq", seq) || seq != m_treasure_cfg_count)
		{
			return -1;
		}

		CrossFishingTreasureConfig &cfg = m_treasure_cfg_list[seq];
		cfg.seq = seq;

		{
			TiXmlElement *item_elemet = data_element->FirstChildElement("reward_item");
			if (NULL == item_elemet)
			{
				return -100;
			}

			if (!cfg.reward_item.ReadConfigNoCheck(item_elemet))
			{
				return -101;
			}
		}

		if (!GetSubNodeValue(data_element, "weight", cfg.weight) || cfg.weight < 0)
		{
			return -2;
		}
		total_weight += cfg.weight;

		++m_treasure_cfg_count;
		data_element = data_element->NextSiblingElement();
	}

	if (total_weight <= 0)
	{
		return -2000;
	}

	return 0;
}

int CrossFishingConfig::InitStealCountBuyCfg(TiXmlElement *RootElement)
{
	TiXmlElement *dataElement = RootElement->FirstChildElement("data");

	m_steal_buy_count = 0;
	while (NULL != dataElement)
	{
		if (m_steal_buy_count >= FISHING_STEAL_COUNT_BUY_MAX)
		{
			return -100;
		}

		int buy_count = 0;
		if (!GetSubNodeValue(dataElement, "buy_count", buy_count) || buy_count < 0 || buy_count != m_steal_buy_count)
		{
			return -1;
		}

		CrossStealCountBuyCfg &buy_cfg = m_steal_buy_cfg_list[buy_count];
		buy_cfg.buy_count = buy_count;

		if (!GetSubNodeValue(dataElement, "need_gold", buy_cfg.need_gold) || buy_cfg.need_gold <= 0)
		{
			return -2;
		}

		++m_steal_buy_count;
		dataElement = dataElement->NextSiblingElement();
	}

	return 0;
}

int CrossFishingConfig::InitScoreRewardCfg(TiXmlElement* RootElement)
{
	TiXmlElement* data_element = RootElement->FirstChildElement("data");
	if (nullptr == data_element)
	{
		return -1000;
	}

	m_fishing_score_reward_cfg_count = 0;
	int last_stage = -1;
	int last_need_score = 0;

	while (data_element != nullptr)
	{
		if (m_fishing_score_reward_cfg_count >= CROSS_FISHING_SCORE_REWARD_CFG_MAX_NUM)
		{
			return -100;
		}

		CrossFishingScoreRewardCfg& cfg = m_fishing_score_reward_cfg_list[m_fishing_score_reward_cfg_count];

		if (!GetSubNodeValue(data_element, "stage", cfg.stage) || cfg.stage != last_stage + 1)
		{
			return -1;
		}
		last_stage = cfg.stage;

		if (!GetSubNodeValue(data_element, "need_score", cfg.need_score) || cfg.need_score <= last_need_score)
		{
			return -2;
		}
		last_need_score = cfg.need_score;

		if (!GetSubNodeValue(data_element, "reward_score", cfg.reward_score) || cfg.reward_score < 0)
		{
			return -3;
		}

		if (!GetSubNodeValue(data_element, "exp_reward", cfg.exp_reward) || cfg.exp_reward < 0)
		{
			return -4;
		}

		TiXmlElement* reward_item_list_element = data_element->FirstChildElement("reward_item_list");
		if (nullptr == reward_item_list_element)
		{
			return -5;
		}

 		TiXmlElement* item_element = reward_item_list_element->FirstChildElement("reward_item");
 		if (nullptr == item_element)
 		{
 			return -6;
 		}
 
		while (item_element != nullptr)
		{
			if (cfg.item_count >= MAX_ATTACHMENT_ITEM_NUM)
			{
				return -7;
			}

			if (!cfg.item_list[cfg.item_count].ReadConfigNoCheck(item_element))
			{
				return -8;
			}

			++cfg.item_count;
			item_element = item_element->NextSiblingElement();
		}
		if (cfg.item_count >= MAX_ATTACHMENT_ITEM_NUM)
		{
			return -9;
		}

		if (!GetSubNodeValue(data_element, "exp_factor_stage", cfg.exp_factor_stage) || cfg.exp_factor_stage < 0)
		{
			return -10;
		}

		++m_fishing_score_reward_cfg_count;
		data_element = data_element->NextSiblingElement();
	}

	return 0;
}

int CrossFishingConfig::InitActivityOpenTimeCfg(TiXmlElement * RootElement)
{
	if (NULL == RootElement)
	{
		return -10000;
	}

	TiXmlElement *dataElement = RootElement->FirstChildElement("data");
	if (NULL == dataElement)
	{
		return -20000;
	}

	int pre_activity_week_day = 0;
	int pre_activity_end_time = 0;
	m_activity_open_time_cfg_list_count = 0;

	while (NULL != dataElement)
	{
		if (m_activity_open_time_cfg_list_count >= CROSS_PASTURE_OPEN_TIME_MAX_COUNT)
		{
			return -1;
		}

		CrossFishingActivityOpenTimeInfo &open_cfg = m_activity_open_time_cfg_list[m_activity_open_time_cfg_list_count];

		if (!GetSubNodeValue(dataElement, "activity_week_day", open_cfg.activity_week_day) || open_cfg.activity_week_day < SUNDAY || open_cfg.activity_week_day > SATURDAY)
		{
			return -2;
		}

		if (!GetSubNodeValue(dataElement, "activity_ready_time", open_cfg.activity_ready_time) || open_cfg.activity_ready_time < 0 || this->DayTimeFormatInvalid(open_cfg.activity_ready_time))
		{
			return -3;
		}

		if (!GetSubNodeValue(dataElement, "activity_start_time", open_cfg.activity_start_time) || open_cfg.activity_start_time < 0 || this->DayTimeFormatInvalid(open_cfg.activity_start_time))
		{
			return -4;
		}

		if (!GetSubNodeValue(dataElement, "activity_end_time", open_cfg.activity_end_time) || open_cfg.activity_end_time < 0 || this->DayTimeFormatInvalid(open_cfg.activity_end_time))
		{
			return -5;
		}

		if (open_cfg.activity_week_day < pre_activity_week_day)
		{
			return -6;
		}
		else if (open_cfg.activity_week_day == pre_activity_week_day)
		{
			if (open_cfg.activity_ready_time <= pre_activity_end_time)
			{
				return -7;
			}
		}

		pre_activity_end_time = open_cfg.activity_end_time;
		pre_activity_week_day = open_cfg.activity_week_day;

		int ready_time_sec = int(open_cfg.activity_ready_time / 100) * 3600 + (open_cfg.activity_ready_time % 100) * 60;
		int start_time_sec = int(open_cfg.activity_start_time / 100) * 3600 + (open_cfg.activity_start_time % 100) * 60;
		int end_time_sec = int(open_cfg.activity_end_time / 100) * 3600 + (open_cfg.activity_end_time % 100) * 60;

		open_cfg.ready_dur_s = start_time_sec - ready_time_sec;
		open_cfg.open_dur_s = end_time_sec - start_time_sec;

		if (open_cfg.ready_dur_s < 0 || open_cfg.open_dur_s <= 0)
		{
			return -9;
		}

		++m_activity_open_time_cfg_list_count;
		dataElement = dataElement->NextSiblingElement();
	}

	return 0;
}

int CrossFishingConfig::InitBroadcastCfg(TiXmlElement *RootElement)
{
	if (NULL == RootElement)
	{
		return -10000;
	}

	TiXmlElement *data_element = RootElement->FirstChildElement("data");
	if (NULL == data_element)
	{
		return -20000;
	}

	while (NULL != data_element)
	{
		unsigned int interval = 0;
		if (!GetSubNodeValue(data_element, "interval", interval) || interval < 0)
		{
			return -1;
		}

		m_broadcast_interval_vec.push_back(interval);

		data_element = data_element->NextSiblingElement();
	}

	return 0;
}

bool CrossFishingConfig::DayTimeFormatInvalid(int day_time)
{
	int hour = day_time / 100;
	int minute = day_time % 100;

	return (hour < 0 || hour >= 24 || minute < 0 || minute >= 60);
}

CrossFishingActivityOpenTimeInfo *CrossFishingConfig::GetActivityOpenTimeInfo(int index)
{
	if (index >= 0 && index < m_activity_open_time_cfg_list_count && index < CROSS_PASTURE_OPEN_TIME_MAX_COUNT)
	{
		return &m_activity_open_time_cfg_list[index];
	}

	return NULL;
}

CrossFishingActivityOpenTimeInfo * CrossFishingConfig::GetActivityOpenTimeInfoByWeek(int week)
{
	for (int i = 0; i < m_activity_open_time_cfg_list_count; ++i)
	{
		auto cfg = this->GetActivityOpenTimeInfo(i);
		if (cfg && cfg->activity_week_day == week)
		{
			return cfg;
		}
	}
	return nullptr;
}

int CrossFishingConfig::GetOpenActivityConfigList(CrossFishingActivityOpenTimeInfo cfg_list[CROSS_PASTURE_OPEN_TIME_MAX_COUNT])
{
	memcpy(cfg_list, m_activity_open_time_cfg_list, sizeof(m_activity_open_time_cfg_list));
	return m_activity_open_time_cfg_list_count;
}

const CrossFishingFishConfig * CrossFishingConfig::GetFishCfg(int type)
{
	if (type < 0 || type > m_fish_cfg_count || type >= FISHING_FISH_TYPE_MAX_COUNT)
	{
		return NULL;
	}

	return &m_fish_cfg_list[type];
}

const CrossFishingFishBaitConfig * CrossFishingConfig::GetFishBaitCfg(int type)
{
	if (type < 0 || type >= m_fish_bait_cfg_count || type >= FISHING_FISH_BAIT_CFG_MAX_COUNT)
	{
		return NULL;
	}

	return &m_fish_bait_cfg_list[type];
}

const CrossFishingEventConfig * CrossFishingConfig::GetEventCfg(int type)
{
	if (type < 0 || type >= CrossFishingEventConfig::EVENT_TYPE_COUNT)
	{
		return NULL;
	}

	return &m_event_cfg_list[type];
}

const CrossFishingFishLeastCountConfig * CrossFishingConfig::GetLeastCountCfg(int index)
{
	if (index < 0 || index >= FISHING_FISH_LEAST_COUNT_CFG_MAX_COUNT || index >= m_fish_combination_cfg_count)
	{
		return NULL;
	}

	return &m_fish_least_count_cfg_list[index];
}

int CrossFishingConfig::GetRandomEventType(bool is_in_team, bool is_perfect)
{
	int weight_list[CrossFishingEventConfig::EVENT_TYPE_COUNT];
	memset(weight_list, 0, sizeof(weight_list));

	int total_weight = 0;
	for (int i = 0; i < CrossFishingEventConfig::EVENT_TYPE_COUNT; ++i)
	{
		if (i == CrossFishingEventConfig::EVENT_TYPE_BIGFISH && !is_in_team)
		{
			weight_list[i] = 0;
			continue;
		}

		if (is_perfect)
		{
			weight_list[i] = m_event_cfg_list[i].trigger_weight;
			total_weight += m_event_cfg_list[i].trigger_weight;
		}
		else
		{
			weight_list[i] = m_event_cfg_list[i].low_trigger_weight;
			total_weight += m_event_cfg_list[i].low_trigger_weight;
		}
	}
	if (total_weight <= 0) return -1;

	int rand_type = -1;
	int rand_val = RandomNum(total_weight);

	for (int i = 0; i < CrossFishingEventConfig::EVENT_TYPE_COUNT; ++i)
	{
		if (rand_val < weight_list[i])
		{
			rand_type = i;
			break;
		}

		rand_val -= weight_list[i];
	}

	return rand_type;
}

int CrossFishingConfig::GetRandomLeastCountCfgIndex()
{
	return RandomNum(FISHING_FISH_LEAST_COUNT_CFG_MAX_COUNT);
}

int CrossFishingConfig::GetRandomFishType(int bait_type)
{
	const CrossFishingFishBaitConfig *bait_cfg = this->GetFishBaitCfg(bait_type);
	if (NULL == bait_cfg) return FISHING_FISH_TYPE_NOFISH;

	int rand_val = RandomNum(100);

	for (int i = 0; i < FISHING_FISH_TYPE_MAX_COUNT; ++i)
	{
		if (rand_val < bait_cfg->fish_rate_list[i])
		{
			return i;
		}

		rand_val -= bait_cfg->fish_rate_list[i];
	}

	return FISHING_FISH_TYPE_NOFISH;
}

const CrossFishingTreasureConfig * CrossFishingConfig::GetRandomTreasureReward()
{
	int total_weight = 0;
	for (int i = 0; i < m_treasure_cfg_count && i < FISHING_TREASURE_CFG_MAX_COUNT; ++i)
	{
		total_weight += m_treasure_cfg_list[i].weight;
	}
	if (total_weight <= 0)
	{
		return NULL;
	}

	int rand_val = RandomNum(total_weight);

	for (int i = 0; i < m_treasure_cfg_count && i < FISHING_TREASURE_CFG_MAX_COUNT; ++i)
	{
		if (rand_val < m_treasure_cfg_list[i].weight)
		{
			return &m_treasure_cfg_list[i];
		}

		rand_val -= m_treasure_cfg_list[i].weight;
	}

	return NULL;
}

int CrossFishingConfig::GetStealBuyCountGold(int now_buycount)
{
	if (now_buycount < 0 || now_buycount >= m_steal_buy_count || now_buycount >= FISHING_STEAL_COUNT_BUY_MAX)
	{
		return 0;
	}

	return m_steal_buy_cfg_list[now_buycount].need_gold;
}

const CrossFishingScoreRewardCfg* CrossFishingConfig::GetFishingScoreReward(const int stage) const
{
	if (stage < 0 || stage >= m_fishing_score_reward_cfg_count || stage >= CROSS_FISHING_SCORE_REWARD_CFG_MAX_NUM) return nullptr;

	return &m_fishing_score_reward_cfg_list[stage];
}

unsigned int CrossFishingConfig::GetBroadCastInterval(int index)
{
	if (index < 0 || index >= static_cast<int>(m_broadcast_interval_vec.size()))
	{
		return 0;
	}

	return m_broadcast_interval_vec[index];
}
