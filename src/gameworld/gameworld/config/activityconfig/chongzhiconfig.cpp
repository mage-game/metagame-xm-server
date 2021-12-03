#include "chongzhiconfig.hpp"
#include "gamecommon.h"
#include "servercommon/servercommon.h"
#include "servercommon/configcommon.h"

#include "world.h"
#include "config/sharedconfig/sharedconfig.h"

#include <string>

ChongzhiConfig::ChongzhiConfig() : m_reward_count(0), m_daily_chongzhi_max_level_seq(0), m_daily_chongzhi_min_chongzhi(0), m_first_chongzhi_max_count(0),
	m_daily_chongzhi_times_max_day(0), m_chongzhi_day_cfg_list_count(0)
{
}

ChongzhiConfig::~ChongzhiConfig()
{

}

bool ChongzhiConfig::Init(const std::string &configname, std::string *err)
{
	char errinfo[1024] = {0};
	int iRet = 0;

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, configname, "ChongzhiConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = configname + ": xml root node error.";
		return false;
	}

	{
		// 充值奖励
		PugiXmlNode rwd_element = RootElement.child("reward");
		if (rwd_element.empty())
		{
			*err = configname + ": no [reward].";
			return false;
		}

		iRet = this->InitChongzhiCfg(rwd_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitChongzhiCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 其他配置
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
		// 特殊首冲奖励
		PugiXmlNode root_element = RootElement.child("special_chongzhi_reward");
		if (root_element.empty())
		{
			*err = configname + ": no [special_chongzhi_reward].";
			return false;
		}

		iRet = this->InitSpecialFirstRechargeCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitSpecialFirstRechargeCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 每日首冲奖励
		PugiXmlNode root_element = RootElement.child("daily_fisrt_chongzhi_reward");
		if (root_element.empty())
		{
			*err = configname + ": no [daily_fisrt_chongzhi_reward].";
			return false;
		}

		iRet = this->InitDailyFirstRechargeRewardCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitDailyFirstRechargeRewardCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 每日累冲阶段
		PugiXmlNode root_element = RootElement.child("daily_total_chongzhi_stage");
		if (root_element.empty())
		{
			*err = configname + ": no [daily_total_chongzhi_stage].";
			return false;
		}

		iRet = this->InitDailyTotalStageCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitDailyTotalStageCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 每日累冲奖励
		PugiXmlNode root_element = RootElement.child("daily_total_chongzhi_reward");
		if (root_element.empty())
		{
			*err = configname + ": no [daily_total_chongzhi_reward].";
			return false;
		}

		iRet = this->InitDailyTotalRechargeRewardCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitDailyTotalRechargeRewardCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 我要变元宝充值活动配置
		PugiXmlNode root_element = RootElement.child("total_chongzhi_want_money");
		if (root_element.empty())
		{
			*err = configname + ": no [total_chongzhi_want_money].";
			return false;
		}

		iRet = this->InitChargeWantMoneyCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitChargeWantMoneyCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 每日累冲
		PugiXmlNode root_element = RootElement.child("weekday_total_chongzhi");
		if (root_element.empty())
		{
			*err = configname + ": no [weekday_total_chongzhi].";
			return false;
		}

		iRet = this->InitDiffWeekdayChongzhiCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitDiffWeekdayChongzhiCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 首次充值奖励
		PugiXmlNode root_element = RootElement.child("first_chongzhi_reward");
		if (root_element.empty())
		{
			*err = configname + ": no [first_chongzhi_reward].";
			return false;
		}

		iRet = this->InitFirstChongzhiRewradCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitFirstChongzhiRewradCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 每日充值奖励
		PugiXmlNode root_element = RootElement.child("daily_chongzhi_reward");
		if (root_element.empty())
		{
			*err = configname + ": no [daily_chongzhi_reward].";
			return false;
		}

		iRet = this->InitDailyChongzhiRewradCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitDailyChongzhiRewradCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 每日充值次数奖励
		PugiXmlNode root_element = RootElement.child("daily_chongzhi_times_reward");
		if (root_element.empty())
		{
			*err = configname + ": no [daily_chongzhi_times_reward].";
			return false;
		}

		iRet = this->InitDailyChongzhiTimesRewradCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitDailyChongzhiTimesRewradCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 每日充值奖励2
		PugiXmlNode root_element = RootElement.child("daily_chongzhi_reward2");
		if (root_element.empty())
		{
			*err = configname + ": no [daily_chongzhi_reward2].";
			return false;
		}

		iRet = this->InitDailyChongzhiRewrad2Cfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitDailyChongzhiRewrad2Cfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 天天返利
		PugiXmlNode root_element = RootElement.child("chongzhi_day_reward");
		if (root_element.empty())
		{
			*err = configname + ": no [chongzhi_day_reward].";
			return false;
		}

		iRet = this->InitDayChongzhiRewardCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitDayChongzhiRewardCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

int ChongzhiConfig::InitChongzhiCfg(PugiXmlNode RootElement)
{
	m_reward_count = 0;

	int last_chongzhi = 0;

	PugiXmlNode dataElement = RootElement.child("data");
	while (!dataElement.empty())
	{
		if (m_reward_count >= CHONGZHI_REWARD_CONFIG_MAX_NUM)
		{
			break;
		}

		ChongzhiReward cfg; 

		if (!PugiGetSubNodeValue(dataElement, "seq", cfg.seq) || cfg.seq != m_reward_count)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "chongzhi", cfg.chongzhi) || cfg.chongzhi <= last_chongzhi)
		{
			return -2;
		}
		last_chongzhi = cfg.chongzhi;

		if (!PugiGetSubNodeValue(dataElement, "extra_coin", cfg.extra_bind_coin) || cfg.extra_bind_coin < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "extra_bind_gold", cfg.extra_bind_gold) || cfg.extra_bind_gold < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "extra_gold", cfg.extra_gold) || cfg.extra_gold < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "reward_type", cfg.reward_type) || cfg.reward_type < CHONGZHI_EXTRA_REWARD_TYPE_INVALID || cfg.reward_type >= CHONGZHI_EXTRA_REWARD_TYPE_MAX)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "openserver_extra_gold_bind", cfg.openserver_extra_gold_bind) || cfg.openserver_extra_gold_bind < 0)
		{
			return -7;
		}
		
		if ((CHONGZHI_EXTRA_REWARD_TYPE_GOLD_BIND == cfg.reward_type && 0 == cfg.extra_bind_gold && 0 == cfg.openserver_extra_gold_bind) ||
			(CHONGZHI_EXTRA_REWARD_TYPE_COIN_BIND == cfg.reward_type && 0 == cfg.extra_bind_coin) || 
			(CHONGZHI_EXTRA_REWARD_TYPE_GOLD == cfg.reward_type && 0 == cfg.extra_gold))
		{
			return -8;
		}
		
		m_reward_list[m_reward_count] = cfg;
		++ m_reward_count;
		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int ChongzhiConfig::InitOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	if (!PugiGetSubNodeValue(data_element, "first_recharge_extra_bind_gold_times", m_other_cfg.first_recharge_extra_bind_gold_times) || m_other_cfg.first_recharge_extra_bind_gold_times < 0)
	{
		return -2;
	}

	if (!PugiGetSubNodeValue(data_element, "daily_first_recharge_extra_reward_need_times", m_other_cfg.daily_first_recharge_extra_reward_need_times) || m_other_cfg.daily_first_recharge_extra_reward_need_times <= 0)
	{
		return -3;
	}

	PugiXmlNode first_recharge_extra_reward_item_element = data_element.child("daily_first_recharge_extra_reward_item");
	if (first_recharge_extra_reward_item_element.empty())
	{
		return -4;
	}

	if (!m_other_cfg.daily_first_recharge_extra_reward_item.ReadConfig(first_recharge_extra_reward_item_element))
	{
		return -5;
	}

	if (!PugiGetSubNodeValue(data_element, "zai_chongzhi_need_chongzhi", m_other_cfg.zai_chongzhi_need_chongzhi) || m_other_cfg.zai_chongzhi_need_chongzhi <= 0)
	{
		return -6;
	}

	PugiXmlNode zai_chongzhi_reward_item_element = data_element.child("zai_chongzhi_reward_item");
	if (zai_chongzhi_reward_item_element.empty())
	{
		return -7;
	}

	if (!m_other_cfg.zai_chongzhi_reward_item.ReadConfig(zai_chongzhi_reward_item_element))
	{
		return -8;
	}

	if (!PugiGetSubNodeValue(data_element, "third_chongzhi_need_chongzhi", m_other_cfg.third_chongzhi_need_chongzhi) || m_other_cfg.third_chongzhi_need_chongzhi <= 0)
	{
		return -9;
	}

	PugiXmlNode third_chongzhi_reward_item_element = data_element.child("third_chongzhi_reward_item");
	if (third_chongzhi_reward_item_element.empty())
	{
		return -10;
	}

	if (!m_other_cfg.third_chongzhi_reward_item.ReadConfig(third_chongzhi_reward_item_element))
	{
		return -11;
	}

	if (!PugiGetSubNodeValue(data_element, "chongzhi_seven_day_need_gold", m_other_cfg.chongzhi_seven_day_need_gold) || m_other_cfg.chongzhi_seven_day_need_gold <= 0)
	{
		return -12;
	}

	if (!PugiGetSubNodeValue(data_element, "chongzhi_seven_day_reward_day", m_other_cfg.chongzhi_seven_day_reward_day) || m_other_cfg.chongzhi_seven_day_reward_day <= 0)
	{
		return -13;
	}

	if (!PugiGetSubNodeValue(data_element, "chongzhi_seven_day_reward_bind_gold", m_other_cfg.chongzhi_seven_day_reward_bind_gold) || m_other_cfg.chongzhi_seven_day_reward_bind_gold <= 0)
	{
		return -14;
	}

	if (!PugiGetSubNodeValue(data_element, "chongzhi_extra_reward_limit_day", m_other_cfg.chongzhi_extra_reward_limit_day) || m_other_cfg.chongzhi_extra_reward_limit_day < 0)
	{
		return -15;
	}

	return 0;
}

int ChongzhiConfig::InitSpecialFirstRechargeCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		if (m_special_first_recharge_cfg.cfg_count >= ChongzhiSpecialFirstRechargeConfig::CONFIG_ITEM_MAX_COUNT)
		{
			return -1;
		}

		ChongzhiSpecialFirstRechargeConfig::ConfigItem &cfg_item = m_special_first_recharge_cfg.cfg_list[m_special_first_recharge_cfg.cfg_count];

		if (!PugiGetSubNodeValue(data_element, "seq", cfg_item.seq) || cfg_item.seq != m_special_first_recharge_cfg.cfg_count)
		{
			return -2;
		}

		PugiXmlNode item_element = data_element.child("reward_item");
		if (item_element.empty())
		{
			return -3;
		}

		if (!cfg_item.reward_item.ReadConfig(item_element))
		{
			return -4;
		}

		++ m_special_first_recharge_cfg.cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int ChongzhiConfig::InitDailyFirstRechargeRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int cfg_count = 0;

	while (!data_element.empty())
	{
		if (cfg_count >= ChongzhiDailyFirstRewardConfig::CONFIG_ITEM_MAX_COUNT)
		{
			return -1;
		}

		ChongzhiDailyFirstRewardConfig::ConfigItem &cfg_item = m_daily_reward_cfg.cfg_list[cfg_count];

		if (!PugiGetSubNodeValue(data_element, "day", cfg_item.wday) || cfg_item.wday != cfg_count)
		{
			return -2;
		}

		PugiXmlNode item_element = data_element.child("first_recharge");
		if (item_element.empty())
		{
			return -3;
		}

		if (!cfg_item.first_recharge_reward_item.ReadConfig(item_element))
		{
			return -4;
		}

		++ cfg_count;
		data_element = data_element.next_sibling();
	}

	if (cfg_count != ChongzhiDailyFirstRewardConfig::CONFIG_ITEM_MAX_COUNT)
	{
		return -20000;
	}

	return 0;
}

int ChongzhiConfig::InitDailyTotalRechargeRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		int stage = 0;
		if (!PugiGetSubNodeValue(data_element, "stage", stage) || stage <= 0 || stage > m_daily_total_stage_cfg.cfg_count)
		{
			return -1;
		}

		ChongzhiDailyTotalStageConfig::ConfigItem &cfg_item = m_daily_total_stage_cfg.cfg_list[stage - 1];

		int seq = 0;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq < 0 || seq >= ChongzhiDailyTotalStageConfig::REWARD_ITEM_MAX_COUNT ||
			seq != cfg_item.total_recharge_reward_item_count)
		{
			return -2;
		}

		int need_total_chongzhi = 0;
		if (!PugiGetSubNodeValue(data_element, "need_total_chongzhi", need_total_chongzhi) || need_total_chongzhi <= 0)
		{
			return -3;
		}
		cfg_item.total_recharge_reward_need_chongzhi[seq] = need_total_chongzhi;

		PugiXmlNode reward_item_element = data_element.child("total_recharge");
		if (reward_item_element.empty())
		{
			return -4;
		}

		if (!cfg_item.total_recharge_reward_item_list[seq].ReadConfig(reward_item_element))
		{
			return -5;
		}

		++ cfg_item.total_recharge_reward_item_count;
		data_element = data_element.next_sibling();
	}

	for (int i = 0; i < m_daily_total_stage_cfg.cfg_count; ++ i)
	{
		if (m_daily_total_stage_cfg.cfg_list[i].total_recharge_reward_item_count <= 0)
		{
			return -20000;
		}
	}

	return 0;
}

int ChongzhiConfig::InitDailyTotalStageCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int last_day = 0;

	while (!data_element.empty())
	{
		if (m_daily_total_stage_cfg.cfg_count >= ChongzhiDailyTotalStageConfig::CONFIG_ITEM_MAX_COUNT)
		{
			return -1;
		}

		ChongzhiDailyTotalStageConfig::ConfigItem &cfg_item = m_daily_total_stage_cfg.cfg_list[m_daily_total_stage_cfg.cfg_count];

		if (!PugiGetSubNodeValue(data_element, "stage", cfg_item.stage) || cfg_item.stage != m_daily_total_stage_cfg.cfg_count + 1)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "start_day", cfg_item.start_day) || cfg_item.start_day <= 0)
		{
			return -3;
		}
		if (last_day != 0 && cfg_item.start_day != last_day + 1)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "end_day", cfg_item.end_day) || cfg_item.end_day < cfg_item.start_day)
		{
			return -5;
		}
		last_day = cfg_item.end_day;

		++ m_daily_total_stage_cfg.cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int ChongzhiConfig::InitChargeWantMoneyCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int last_state = -1;
	long long last_need_chongzhi = 0;

	while (!data_element.empty())
	{
		TotalChongzhiWantMoneyConfig::ConfigItem cfg_item;

		if (!PugiGetSubNodeValue(data_element, "state", cfg_item.state) || cfg_item.state != last_state + 1)
		{
			return -1;
		}
		last_state = cfg_item.state;

		if (!PugiGetSubNodeValue(data_element, "charged", cfg_item.need_chongzhi) || cfg_item.need_chongzhi <= last_need_chongzhi)
		{
			return -2;
		}
		last_need_chongzhi = cfg_item.need_chongzhi;

		if (!PugiGetSubNodeValue(data_element, "random_min", cfg_item.random_min) || cfg_item.random_min <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "random_max", cfg_item.random_max) || cfg_item.random_max < cfg_item.random_min)
		{
			return -4;
		}

		m_total_chongzhi_want_money_cfg.cfg_list[cfg_item.state] = cfg_item;
		m_total_chongzhi_want_money_cfg.m_max_state = cfg_item.state;

		data_element = data_element.next_sibling();
	}

	return 0;
}

int ChongzhiConfig::InitDiffWeekdayChongzhiCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int weekday = 0;
	int last_weekday = 0;

	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "day_of_week", weekday) || weekday >= DIFF_WD_CHONGZHI_MAX_DAY_Of_WEEK)
		{
			return -1;
		}

		if (weekday != last_weekday && 1 != weekday - last_weekday)
		{
			return -2;
		}
		last_weekday = weekday;

		DiffWeekdayChongzhiConfig &cfg = m_daily_diffwd_chongzhi_cfg[weekday];

		if (cfg.stage_cfg_count >= DIFF_WD_CHONGZHI_MAX_STAGE)
		{
			return -3;
		}

		DiffWeekdayChongzhiConfig::RewardItemCfg &cfg_item = cfg.stage_item_list[cfg.stage_cfg_count];

		if (!PugiGetSubNodeValue(data_element, "need_total_chongzhi", cfg_item.need_chongzhi) || cfg_item.need_chongzhi <= 0)
		{
			return -4;
		}

		PugiXmlNode reward_item_element = data_element.child("reward_item");
		if (reward_item_element.empty())
		{
			return -4;
		}

		if (!cfg_item.reward_item.ReadConfig(reward_item_element))
		{
			return -5;
		}

		++ cfg.stage_cfg_count;

		data_element = data_element.next_sibling();
	}

	return 0;
}

int ChongzhiConfig::InitFirstChongzhiRewradCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_first_chongzhi_max_count = 0;
	int last_seq = -1;
	int last_chongzhi = 0;
	const int max_lolumn = 100;
	int ret = 0;
	while (!data_element.empty())
	{
		int seq = -1;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq < 0 || seq >= MAX_FIRST_CHONGZHI_REWARD_COUNT || seq != last_seq + 1)
		{
			return -1;
		}

		FirstChongzhiRewradConfig &cfg = m_first_chongzhi_reward_cfg[seq];
		cfg.seq = seq;
		last_seq = seq;

		if (!PugiGetSubNodeValue(data_element, "need_danbi_chongzhi", cfg.need_total_chongzhi) || cfg.need_total_chongzhi <= 0 || cfg.need_total_chongzhi < last_chongzhi)
		{
			return -2;
		}

		last_chongzhi = cfg.need_total_chongzhi;

		{
			PugiXmlNode item_element = data_element.child("first_reward_item");
			if (item_element.empty())
			{
				return -6;
			}

			if (!cfg.first_reward_item.ReadConfig(item_element) && cfg.first_reward_item.item_id != 0)
			{
				return -7;
			}
		}

		if (!PugiGetSubNodeValue(data_element, "active_attr_time_limit", cfg.active_attr_time_limit) || cfg.active_attr_time_limit < 0)
		{
			return -5;
		}

		ret = this->ReadColumnList(data_element, "attribute_type_", cfg.attribute_type, max_lolumn);
		if (ret < 0)
		{
			return -8 + ret;
		}

		for (size_t i = 0; i < cfg.attribute_type.size(); ++i)
		{
			if (!CharIntAttrs::IsRoleBaseAttr(cfg.attribute_type[i]))
			{
				return -100 - i;
			}
		}

		ret = this->ReadColumnList(data_element, "attribute_value_", cfg.attribute_value, max_lolumn);
		if (ret < 0)
		{
			return -10 + ret;
		}

		for (size_t i = 0; i < cfg.attribute_value.size(); ++i)
		{
			if (cfg.attribute_value[i] < 0)
			{
				return -200 - i;
			}
		}
		if (cfg.attribute_type.size() != cfg.attribute_value.size())
		{
			return -300;
		}

		++m_first_chongzhi_max_count;

		data_element = data_element.next_sibling();
	}

	return 0;
}

int ChongzhiConfig::InitDailyChongzhiRewradCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_daily_chongzhi_min_chongzhi = 999999;
	while (!data_element.empty())
	{
		int level_seq = -1;
		if (!PugiGetSubNodeValue(data_element, "level_seq", level_seq) || level_seq < 0 || level_seq >= MAX_DAILY_CHONGZHI_LEVEL_SEQ)
		{
			return -1;
		}

		int seq = -1;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq < 0 || seq >= MAX_DAILY_CHONGZHI_REWARD_COUNT)
		{
			return -2;
		}

		DailyChongzhiRewradConfig &cfg = m_daily_chongzhi_reward_cfg_list[level_seq][seq];
		cfg.seq = seq;

		if (!PugiGetSubNodeValue(data_element, "min_level", cfg.min_level) || cfg.min_level < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "max_level", cfg.max_level) || cfg.max_level < 0 || cfg.min_level > cfg.max_level)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "need_total_chongzhi", cfg.need_total_chongzhi) || cfg.need_total_chongzhi <= 0)
		{
			return -5;
		}

		if (m_daily_chongzhi_min_chongzhi > cfg.need_total_chongzhi)
		{
			m_daily_chongzhi_min_chongzhi = cfg.need_total_chongzhi;
		}

		{
			PugiXmlNode item_element = data_element.child("reward_item");
			if (item_element.empty())
			{
				return -8;
			}

			if (!cfg.reward_item.ReadConfig(item_element) && 0 != cfg.reward_item.item_id)
			{
				return -9;
			}
		}


		// 每日充值三选一奖励，策划需求不要了，再要的时候把代码开回来就可以了
// 		{
// 			PugiXmlNode item_list_element = data_element.child("select_reward_item_list");
// 			if (item_list_element.empty())
// 			{
// 				return -1001;
// 			}
// 
// 			PugiXmlNode item_element = item_list_element.child("select_reward_item");
// 			if (item_element.empty())
// 			{
// 				return -1002;
// 			}
// 
// 			int item_count = 0;
// 			while (!item_element.empty())
// 			{
// 				if (item_count >= MAX_DAILY_CHONGZHI_SELECT_COUNT)
// 				{
// 					return -1003;
// 				}
// 
// 				if (!cfg.select_reward_item_list[item_count].ReadConfig(item_element))
// 				{
// 					return -1004;
// 				}
// 
// 				item_count++;
// 				item_element = item_element.next_sibling();
// 			}
// 		}

		m_daily_chongzhi_max_level_seq++;

		data_element = data_element.next_sibling();
	}

	return 0;
}

int ChongzhiConfig::InitDailyChongzhiTimesRewradCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_daily_chongzhi_times_max_day = 0;
	while (!data_element.empty())
	{
		int opengame_day = 0;
		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day < 0)
		{
			return -100;
		}
		
		int seq = 0;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq < 0 || seq >= MAX_DAILY_CHONGZHI_TIMES_REWARD_COUNT)
		{
			return -1;
		}

		//DailyChongzhiTimesRewradConfig &cfg = m_daily_chongzhi_times_reward_cfg_list[opengame_day][seq];
		DailyChongzhiTimesRewradConfig &cfg = m_daily_chongzhi_reward_cfg_map[opengame_day].m_daily_chongzhi_times_reward_cfg_list[seq];
		cfg.seq = seq;
		cfg.opengameday = opengame_day;
		
		if (!PugiGetSubNodeValue(data_element, "complete_days", cfg.complete_days) || cfg.complete_days <= 0)
		{
			return -2;
		}

		if (cfg.complete_days > m_daily_chongzhi_times_max_day)
		{
			m_daily_chongzhi_times_max_day = cfg.complete_days;
		}

		{
			PugiXmlNode item_element = data_element.child("reward_item");
			if (item_element.empty() || !cfg.reward_item.ReadConfig(item_element))
			{
				return -3;
			}
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int ChongzhiConfig::InitDailyChongzhiRewrad2Cfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int last_seq = -1;
	int last_open_day = 0;
	int last_max_level = 0;
	int count_index = 0;

	m_daily_chongzhi_reward2_cfg_map.clear();
	while (!data_element.empty())
	{
		int seq = -1;
		if (!PugiGetSubNodeValue(data_element, "seq", seq)/* || seq < 0 || seq >= DAILY_CHONGZHI_REWRAD2_REQ_MAX*/)
		{
			return -2;
		}

		if (last_seq != seq)
		{
			last_seq = seq;
			last_open_day = 0;
			last_max_level = 0;
			count_index = 0;
		}

		/*if (count_index < 0 || count_index >= DAILY_CHONGZHI_REWRAD2_COUNT_MAX)
		{
			return -3;
		}*/
		if (m_daily_chongzhi_reward2_cfg_map.find(seq) == m_daily_chongzhi_reward2_cfg_map.end())
		{
			DailyChongzhiRewrad2Config init_cfg;
			std::map<int, DailyChongzhiRewrad2Config> init_cfg_map;
			init_cfg_map[count_index] = init_cfg;
			m_daily_chongzhi_reward2_cfg_map[seq] = init_cfg_map;
		}
		std::map<int, DailyChongzhiRewrad2Config> &cfg_map = m_daily_chongzhi_reward2_cfg_map[seq];
		if (cfg_map.find(count_index) == cfg_map.end())
		{
			DailyChongzhiRewrad2Config init_cfg;
			cfg_map[count_index] = init_cfg;
		}
		DailyChongzhiRewrad2Config &cfg = cfg_map[count_index];
		
		//DailyChongzhiRewrad2Config &cfg = m_daily_chongzhi_reward2_cfg_list[seq][count_index];
		if (cfg.seq >= 0)
		{
			return -4;
		}
		cfg.seq = seq;

		if (!PugiGetSubNodeValue(data_element, "open_day", cfg.open_day) || cfg.open_day < last_open_day)
		{
			return -5;
		}

		if (cfg.open_day != last_open_day)
		{
			last_open_day = cfg.open_day;
			last_max_level = 0;
		}

		if (!PugiGetSubNodeValue(data_element, "max_level", cfg.max_level) || cfg.max_level <= last_max_level)
		{
			return -6;
		}
		last_max_level = cfg.max_level;

		if (!PugiGetSubNodeValue(data_element, "need_chongzhi", cfg.need_chongzhi) || cfg.need_chongzhi <= 0)
		{
			return -7;
		}

		{
			PugiXmlNode item_list_element = data_element.child("reward_item_list");
			if (item_list_element.empty())
			{
				return -1001;
			}

			PugiXmlNode item_element = item_list_element.child("reward_item");
			if (item_element.empty())
			{
				return -1002;
			}

			cfg.reward_item_count = 0;
			while (!item_element.empty())
			{
				if (cfg.reward_item_count < 0 || cfg.reward_item_count >= DAILY_CHONGZHI_REWRAD2_ITEM_COUNT_MAX)
				{
					return -1003;
				}

				if (!cfg.reward_item_list[cfg.reward_item_count].ReadConfig(item_element))
				{
					return -1004;
				}

				cfg.reward_item_count++;
				item_element = item_element.next_sibling();
			}
		}

		count_index++;
		data_element = data_element.next_sibling();
	}

	return 0;
}

const TotalChongzhiWantMoneyConfig::ConfigItem * ChongzhiConfig::GetWantMoneyCfg(int state)
{
	if (state < 0 || state > m_total_chongzhi_want_money_cfg.m_max_state)
	{
		return NULL;
	}

	return &m_total_chongzhi_want_money_cfg.cfg_list[state];
}

const ChongzhiReward * ChongzhiConfig::GetExtraRewardByNum(int chongzhi)
{
	for (int i = 0; i < m_reward_count; ++ i)
	{
		if (chongzhi == m_reward_list[i].chongzhi)
		{
			return &m_reward_list[i];
		}
	}

	return NULL;
}

const ChongzhiReward * ChongzhiConfig::GetExtraRewardBySeq(int seq)
{
	if (seq >= 0 && seq < m_reward_count)
	{
		return &m_reward_list[seq];
	}

	return NULL;
}

const ChongzhiDailyFirstRewardConfig::ConfigItem * ChongzhiConfig::GetDailyFirstReward(int wday_idx)
{
	if (wday_idx >= 0 && wday_idx < ChongzhiDailyFirstRewardConfig::CONFIG_ITEM_MAX_COUNT)
	{
		return &m_daily_reward_cfg.cfg_list[wday_idx];
	}

	return NULL;
}

const ChongzhiSpecialFirstRechargeConfig::ConfigItem * ChongzhiConfig::GetSpecialFirstRechargeReward(int seq)
{
	if (seq >= 0 && seq < m_special_first_recharge_cfg.cfg_count)
	{
		return &m_special_first_recharge_cfg.cfg_list[seq];
	}

	return NULL;
}

const ChongzhiDailyTotalStageConfig::ConfigItem * ChongzhiConfig::GetDailyTotalStageByDay(int open_day)
{
	for (int i = 0; i < m_daily_total_stage_cfg.cfg_count; ++ i)
	{
		if (open_day >= m_daily_total_stage_cfg.cfg_list[i].start_day && open_day <= m_daily_total_stage_cfg.cfg_list[i].end_day)
		{
			return &m_daily_total_stage_cfg.cfg_list[i];
		}
	}

	return NULL;
}

const ChongzhiDailyTotalStageConfig::ConfigItem * ChongzhiConfig::GetDailyTotalStageByStage(int stage)
{
	if (stage > 0 && stage <= m_daily_total_stage_cfg.cfg_count)
	{
		return &m_daily_total_stage_cfg.cfg_list[stage - 1];
	}

	return NULL;
}

bool ChongzhiConfig::IsVaildDailyTotalChongzhiStage(int stage)
{
	if (stage > 0 && stage <= m_daily_total_stage_cfg.cfg_count)
	{
		return true;
	}

	return false;
}

bool ChongzhiConfig::IsValidDailyTotalChongzhiDay(int open_day)
{
	if (m_daily_total_stage_cfg.cfg_count <= 0)
	{
		return false;
	}

	if (open_day >= m_daily_total_stage_cfg.cfg_list[0].start_day && 
		open_day <= m_daily_total_stage_cfg.cfg_list[m_daily_total_stage_cfg.cfg_count - 1].end_day)
	{
		return true;
	}
	
	return false;
}

int ChongzhiConfig::GetDailyTotalChongzhiMinStartDay()
{
	return m_daily_total_stage_cfg.cfg_list[0].start_day;
}

const DiffWeekdayChongzhiConfig::RewardItemCfg * ChongzhiConfig::GetDiffWeekdayChongzhiCfg(int weekday, int stage)
{
	if (weekday < 0 || weekday >= DIFF_WD_CHONGZHI_MAX_DAY_Of_WEEK ||
		stage < 0 || stage >= DIFF_WD_CHONGZHI_MAX_STAGE)
	{
		return NULL;
	}

	DiffWeekdayChongzhiConfig &cfg = m_daily_diffwd_chongzhi_cfg[weekday];

	return &cfg.stage_item_list[stage];
}

const DiffWeekdayChongzhiConfig * ChongzhiConfig::GetDiffWeekdayChongzhiCfgList(int weekday)
{
	if (weekday < 0 || weekday >= DIFF_WD_CHONGZHI_MAX_DAY_Of_WEEK)
	{
		return NULL;
	}

	return &m_daily_diffwd_chongzhi_cfg[weekday];
}

const DailyChongzhiRewradConfig * ChongzhiConfig::GetDailyChongzhiRewradConfig(int level, int seq)
{
	if (level <= 0 || level > MAX_ROLE_LEVEL)
	{
		return NULL;
	}

	if (seq < 0 || seq >= MAX_DAILY_CHONGZHI_REWARD_COUNT)
	{
		return NULL;
	}

	for (int i = 0; i < m_daily_chongzhi_max_level_seq && i < MAX_DAILY_CHONGZHI_LEVEL_SEQ; i++)
	{
		DailyChongzhiRewradConfig &cfg = m_daily_chongzhi_reward_cfg_list[i][seq];
		if (cfg.min_level <= level && level <= cfg.max_level)
		{
			if (m_daily_chongzhi_reward_cfg_list[i][seq].seq == seq)
			{
				return &m_daily_chongzhi_reward_cfg_list[i][seq];
			}
		}
	}

	return NULL;
}

const FirstChongzhiRewradConfig * ChongzhiConfig::GetFirstChongzhiRewradConfig(int seq)
{
	if (seq < 0 || seq >= m_first_chongzhi_max_count || seq >= MAX_FIRST_CHONGZHI_REWARD_COUNT) return NULL;

	FirstChongzhiRewradConfig &cfg = m_first_chongzhi_reward_cfg[seq];
	if (cfg.seq != seq) return NULL;

	return &cfg;
}

const DailyChongzhiTimesRewradConfig * ChongzhiConfig::GetDailyChongzhiTimesRewradConfig(int seq)
{
	if (seq < 0 || seq >= MAX_DAILY_CHONGZHI_TIMES_REWARD_COUNT /*|| m_daily_chongzhi_times_reward_cfg_list[seq].seq != seq*/)
	{
		return NULL;
	}

	int day_index = GetDayIndex(SHAREDCONFIG->GetOpenServerConfig().GetServerRealStartTime(), EngineAdapter::Instance().Time()) + 1;

	for (auto & iter : m_daily_chongzhi_reward_cfg_map)
	{
		if (day_index <= iter.first)
		{
			if (iter.second.m_daily_chongzhi_times_reward_cfg_list[seq].seq == seq)
			{
				return &iter.second.m_daily_chongzhi_times_reward_cfg_list[seq];
			}
		}
	}

	return NULL;
	//return &m_daily_chongzhi_times_reward_cfg_list[seq];
}

const DailyChongzhiRewrad2Config * ChongzhiConfig::GetDailyChongzhiRewrad2Config(int level, int seq)
{
	if (/*seq < 0 || seq >= DAILY_CHONGZHI_REWRAD2_REQ_MAX ||*/ m_daily_chongzhi_reward2_cfg_map.find(seq) == m_daily_chongzhi_reward2_cfg_map.end())
	{
		return NULL;
	}

	int open_day_index = GetDayIndex(SHAREDCONFIG->GetOpenServerConfig().GetServerRealStartTime(), EngineAdapter::Instance().Time());
	//for (int i = 0; i < DAILY_CHONGZHI_REWRAD2_COUNT_MAX; i++)
	for (auto &cfg_map : m_daily_chongzhi_reward2_cfg_map[seq])
	{
		DailyChongzhiRewrad2Config &cfg = cfg_map.second;
		if (cfg.seq < 0)
		{
			return NULL;
		}

		if (open_day_index < cfg.open_day && level <= cfg.max_level)
		{
			return &cfg;
		}
	}

	return NULL;
}

const DailyChongzhiRewrad2Config * ChongzhiConfig::GetDailyChongzhiRewrad2ConfigByLastChongzhiTime(int level, int seq, int last_chongzhi_time)
{
	if (m_daily_chongzhi_reward2_cfg_map.find(seq) == m_daily_chongzhi_reward2_cfg_map.end())
	{
		return NULL;
	}

	for (auto &cfg_map : m_daily_chongzhi_reward2_cfg_map[seq])
	{
		DailyChongzhiRewrad2Config &cfg = cfg_map.second;
		if (cfg.seq < 0)
		{
			return NULL;
		}

		if (last_chongzhi_time <= cfg.open_day && level <= cfg.max_level)
		{
			return &cfg;
		}
	}

	return NULL;
}

const DailyReward2ConfigMapInMap & ChongzhiConfig::GetDailyChongzhiRewrad2ConfigMap()
{
	return m_daily_chongzhi_reward2_cfg_map;
}

int ChongzhiConfig::InitDayChongzhiRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int last_index = -1;
	int last_need_chongzhi_day = 0;
	m_chongzhi_day_cfg_list_count = 0;
	while (!data_element.empty())
	{
		int index = -1;
		if (!PugiGetSubNodeValue(data_element, "index", index) || index < 0 || index >= CHONGZHI_DAY_CFG_MAX_COUNT || index != last_index + 1)
		{
			return -1;
		}
		last_index = index;
		ChongzhiDayReward &cfg = m_chongzhi_day_reward_cfg_list[index];

		if (!PugiGetSubNodeValue(data_element, "need_chongzhi_day", cfg.need_chongzhi_day) || cfg.need_chongzhi_day <= 0 || cfg.need_chongzhi_day <= last_need_chongzhi_day)
		{
			return -2;
		}
		last_need_chongzhi_day = cfg.need_chongzhi_day;

		if (!PugiGetSubNodeValue(data_element, "need_gold", cfg.need_gold) || cfg.need_gold <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "reward_bind_gold", cfg.reward_bind_gold))
		{
			if (cfg.reward_bind_gold < 0)
			{
				return -4;
			}
		}

		if (!PugiGetSubNodeValue(data_element, "reward_gold", cfg.reward_gold))
		{
			if (cfg.reward_gold < 0)
			{
				return -5;
			}
		}

		{
			PugiXmlNode item_list_element = data_element.child("reward_item_list");
			if (item_list_element.empty())
			{
				return -1001;
			}

			PugiXmlNode item_element = item_list_element.child("reward_item");
			if (item_element.empty())
			{
				return -1002;
			}

			cfg.item_count = 0;
			while (!item_element.empty())
			{
				if (cfg.item_count < 0 || cfg.item_count >= CHONGZHI_DAY_REWARD_ITEM_MAX_COUNT)
				{
					return -1003;
				}

				if (!cfg.reward_item_list[cfg.item_count].ReadConfig(item_element))
				{
					return -1004;
				}

				cfg.item_count++;
				item_element = item_element.next_sibling();
			}
		}

		{
			PugiXmlNode item_list_element = data_element.child("rare_reward_item_list");
			if (!item_list_element.empty())
			{
				cfg.rare_item_count = 0;
				PugiXmlNode item_element = item_list_element.child("rare_reward_item");
				while (!item_element.empty())
				{
					if (cfg.rare_item_count < 0 || cfg.rare_item_count >= CHONGZHI_DAY_REWARD_ITEM_MAX_COUNT)
					{
						return -1003;
					}

					if (!cfg.rare_reward_item_list[cfg.rare_item_count].ReadConfig(item_element))
					{
						return -1004;
					}

					cfg.rare_item_count++;
					item_element = item_element.next_sibling();
				}
			}
		}

		++m_chongzhi_day_cfg_list_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

const ChongzhiDayReward * ChongzhiConfig::GetDayChongzhiRewardCfg(int seq)
{
	if (seq < 0 || seq >= m_chongzhi_day_cfg_list_count || seq >= CHONGZHI_DAY_CFG_MAX_COUNT)
	{
		return nullptr;
	}

	return &m_chongzhi_day_reward_cfg_list[seq];
}

const ChongzhiDayReward * ChongzhiConfig::GetDayChongzhiRewardCfgByChongzhiDay(int day_count)
{
	for (int i = 0; i < m_chongzhi_day_cfg_list_count && i < CHONGZHI_DAY_CFG_MAX_COUNT; ++i)
	{
		if (day_count < m_chongzhi_day_reward_cfg_list[i].need_chongzhi_day)
		{
			return &m_chongzhi_day_reward_cfg_list[i];
		}
	}

	return nullptr;
}
