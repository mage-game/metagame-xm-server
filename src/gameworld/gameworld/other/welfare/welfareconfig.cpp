#include "welfareconfig.hpp"

#include "servercommon/configcommon.h"

#include "item/itempool.h"
#include "item/itembase.h"
#include "task/taskpool.h"
#include "other/daycounter/daycounter.hpp"

#include "config/logicconfigmanager.hpp"
#include "config/fbconfig.hpp"

WelfareConfig::WelfareConfig() 
{
	
}

WelfareConfig::~WelfareConfig()
{

}

bool WelfareConfig::Init(const std::string &path, std::string *err)
{
	int iRet = 0;

	char errinfo[1024] = {0};

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "WelfareConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = path + ": xml root node error.";
		return false;
	}

	{
		// 在线奖励
		PugiXmlNode online_reward_element = RootElement.child("online_reward");
		if (online_reward_element.empty())
		{
			sprintf(errinfo, "%s: xml node error in [config->online_reward]", path.c_str());
			*err = errinfo;
			return false;
		}

		iRet = this->InitOnlineRewardCfg(online_reward_element);
		if (0 != iRet)
		{
			sprintf(errinfo,"%s: InitOnlineRewardCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 离线经验
		PugiXmlNode offline_exp_element = RootElement.child("offline_exp");
		if (offline_exp_element.empty())
		{
			sprintf(errinfo, "%s: xml node error in [config->offline_exp]", path.c_str());
			*err = errinfo;
			return false;
		}

		iRet = this->InitOfflineExpFetchTypeCfg(offline_exp_element);
		if (0 != iRet)
		{
			sprintf(errinfo,"%s: InitOfflineExpFetchTypeCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 日常找回
		PugiXmlNode daily_find_element = RootElement.child("daily_find_list");
		if (daily_find_element.empty())
		{
			sprintf(errinfo, "%s: xml node error in [config->daily_find_list]", path.c_str());
			*err = errinfo;
			return false;
		}

		iRet = this->InitDailyFindCfg(daily_find_element);
		if (0 != iRet)
		{
			sprintf(errinfo,"%s: InitDailyFindCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 其他配置
		PugiXmlNode root_element = RootElement.child("other");
		if (root_element.empty())
		{
			sprintf(errinfo, "%s: xml node error in [config->other]", path.c_str());
			*err = errinfo;
			return false;
		}

		iRet = this->InitOtherCfg(root_element);
		if (0 != iRet)
		{
			sprintf(errinfo,"%s: InitOtherCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 签到配置
		PugiXmlNode root_element = RootElement.child("sign_in");
		if (root_element.empty())
		{
			sprintf(errinfo, "%s: xml node error in [config->sign_in]", path.c_str());
			*err = errinfo;
			return false;
		}

		iRet = this->InitSignInCfg(root_element);
		if (0 != iRet)
		{
			sprintf(errinfo,"%s: InitSignInCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 累计签到配置
		PugiXmlNode root_element = RootElement.child("total_sign_in");
		if (root_element.empty())
		{
			sprintf(errinfo, "%s: xml node error in [config->total_sign_in]", path.c_str());
			*err = errinfo;
			return false;
		}

		iRet = this->InitAccumulationSignInCfg(root_element);
		if (0 != iRet)
		{
			sprintf(errinfo,"%s: InitAccumulationSignInCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 活动找回配置
		PugiXmlNode root_element = RootElement.child("activity_find");
		if (root_element.empty())
		{
			sprintf(errinfo, "%s: xml node error in [config->activity_find]", path.c_str());
			*err = errinfo;
			return false;
		}

		iRet = this->InitActivityFindCfg(root_element);
		if (0 != iRet)
		{
			sprintf(errinfo,"%s: InitActivityFindCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 活动找回奖励
		PugiXmlNode root_element = RootElement.child("activity_find_reward");
		if (root_element.empty())
		{
			sprintf(errinfo, "%s: xml node error in [config->activity_find_reward]", path.c_str());
			*err = errinfo;
			return false;
		}

		iRet = this->InitActivityFindRewardCfg(root_element);
		if (0 != iRet)
		{
			sprintf(errinfo,"%s: InitActivityFindRewardCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

// 	{
// 		// 升级奖励
// 		PugiXmlNode root_element = RootElement.child("uplevel_reward");
// 		if (root_element.empty())
// 		{
// 			sprintf(errinfo, "%s: xml node error in [config->uplevel_reward]", path.c_str());
// 			*err = errinfo;
// 			return false;
// 		}
// 
// 		iRet = this->InitUplevelCfg(root_element);
// 		if (0 != iRet)
// 		{
// 			sprintf(errinfo,"%s: InitUplevelCfg failed %d", path.c_str(), iRet);
// 			*err = errinfo;
// 			return false;
// 		}
// 	}

	{
		// 欢乐果树
		PugiXmlNode root_element = RootElement.child("happy_tree");
		if (root_element.empty())
		{
			sprintf(errinfo, "%s: xml node error in [config->happy_tree]", path.c_str());
			*err = errinfo;
			return false;
		}

		iRet = this->InitHappyTreeCfg(root_element);
		if (0 != iRet)
		{
			sprintf(errinfo,"%s: InitHappyTreeCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 兑换
		PugiXmlNode root_element = RootElement.child("exchange");
		if (root_element.empty())
		{
			sprintf(errinfo, "%s: xml node error in [config->exchange]", path.c_str());
			*err = errinfo;
			return false;
		}

		iRet = this->InitExchangeCfg(root_element);
		if (0 != iRet)
		{
			sprintf(errinfo,"%s: InitExchangeCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		//欢乐果树每日长成值上限
		PugiXmlNode root_element = RootElement.child("max_growth_val_per_day");
		if (root_element.empty())
		{
			sprintf(errinfo, "%s: xml node error in [config->max_growth_val_per_day]", path.c_str());
			*err = errinfo;
			return false;
		}

		iRet = this->InitGrowthValueCfg(root_element);
		if (0 != iRet)
		{
			sprintf(errinfo,"%s: InitGrowthValueCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		//欢乐果树每日长成值上限
		PugiXmlNode root_element = RootElement.child("growth_val_config");
		if (root_element.empty())
		{
			sprintf(errinfo, "%s: xml node error in [config->growth_val_config]", path.c_str());
			*err = errinfo;
			return false;
		}

		iRet = this->InitGetGrowthCfg(root_element);
		if (0 != iRet)
		{
			sprintf(errinfo,"%s: InitGetGrowthCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		//冲级豪礼
		PugiXmlNode root_element = RootElement.child("chong_level_gift");
		if (root_element.empty())
		{
			sprintf(errinfo, "%s: xml node error in [config->chong_level_gift]", path.c_str());
			*err = errinfo;
			return false;
		}

		iRet = this->InitChongjihaoli(root_element);
		if (0 != iRet)
		{
			sprintf(errinfo,"%s: InitChongLevelGift failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

const OnlineRewardConfig::RewardConfig * WelfareConfig::GetOnlineRewardCfg(int part)
{
	if (part < 0 || part >= OnlineRewardConfig::MAX_ONLINE_REWARD_COUNT)
	{
		return NULL;
	}

	return &m_online_reward_config.online_reward[part];

}

const WelfareHappyTreeConfig::RewardCfg * WelfareConfig::GetHappyTreeReward(int level, int type)
{
	if (level < 1 || level > WelfareHappyTreeConfig::MAX_HAPPY_TREE_LEVEL || type < 1 || type > WelfareHappyTreeConfig::MAX_FETCH_ITEM_COUT)
	{
		return NULL;
	}
	
	return &m_happy_tree_cfg[level].reward_list[type];
}

const WelfareHappyTreeConfig * WelfareConfig::GetHappyTreeLevelCfg(int level)
{
	if (level < 1 || level > WelfareHappyTreeConfig::MAX_HAPPY_TREE_LEVEL)
	{
		return NULL;
	}

	return &m_happy_tree_cfg[level];
}

const OfflineExpFetchConfig::ExpConfig * WelfareConfig::GetOfflineExpConfig(int type) const
{
	if (type >= 0 && type < OfflineExpFetchConfig::MAX_TYPE_COUT)
	{
		return &m_offline_fetch_config.expconfig[type];
	}
	
	return NULL;
}

const DailyFindConfigItem * WelfareConfig::GetDailyFindItem(int type) const
{
	if (type < 0 || type >= DAILYFIND_TYPE_MAX || m_daily_find[type].Invalid()) return NULL;

	return &m_daily_find[type];
}

int WelfareConfig::InitOnlineRewardCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -10000;
	}

	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -20000;
	}

	while (!data_element.empty())
	{
		int seq = 0;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq < 0 ||seq >= OnlineRewardConfig::MAX_ONLINE_REWARD_COUNT)
		{
			return -1;
		}

		OnlineRewardConfig::RewardConfig &online_reward = m_online_reward_config.online_reward[seq];

		if (!PugiGetSubNodeValue(data_element, "minutes",online_reward.minutes) ||online_reward.minutes < 0 || online_reward.minutes >= OnlineRewardConfig::MAX_ONLINE_MINUTES)
		{
			return -2;
		}

		{
			PugiXmlNode element = data_element.child("reward_item_list");
			if (element.empty())
			{
				return -100;
			}

			int i = 0;
			PugiXmlNode item_element = element.child("reward_item");
			while (!item_element.empty())
			{
				if (i >= OnlineRewardConfig::MAX_ONLINE_REWARD_ITEM_COUNT)
				{
					return -101 - i;
				}

				if (!online_reward.reward_item[i].ReadConfig(item_element))
				{
					return - 151 - i;
				}

				i++;
				item_element = item_element.next_sibling();
			}
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int WelfareConfig::InitHappyTreeCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -10002;
	}

	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -20002;
	}

	int last_level = 1;
	while (!data_element.empty())
	{
		int  level = 0;
		if (!PugiGetSubNodeValue(data_element, "level", level) || level <= 0 ||level > WelfareHappyTreeConfig::MAX_HAPPY_TREE_LEVEL)
		{
			return -1;
		}

		int fecth_type = 0;
		if (!PugiGetSubNodeValue(data_element, "fecth_type", fecth_type) || fecth_type <= 0 || fecth_type > WelfareHappyTreeConfig::MAX_FETCH_ITEM_COUT)
		{
			return -2;
		}

		WelfareHappyTreeConfig::RewardCfg &reward_cfg = m_happy_tree_cfg[level].reward_list[fecth_type];

		if (last_level == level)
		{
			if (m_happy_tree_cfg[level].item_count < 0 || m_happy_tree_cfg[level].item_count > WelfareHappyTreeConfig::MAX_FETCH_ITEM_COUT)
			{
				return -1111;
			}
			m_happy_tree_cfg[level].item_count += 1; 
		}
		
		if(last_level != level)
		{
			last_level = level;
			m_happy_tree_cfg[level].item_count = 1;
		}

		if (!PugiGetSubNodeValue(data_element, "growth_val",reward_cfg.growth_val) ||reward_cfg.growth_val < 0)
		{
			return -3;
		}

		PugiXmlNode reward_item_element = data_element.child("reward_item");
		if (reward_item_element.empty())
		{
			return -4;
		}

		if (!reward_cfg.reward_item.ReadConfig(reward_item_element))
		{
			return -5;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int WelfareConfig::InitOfflineExpFetchTypeCfg(PugiXmlNode RootElement)
{
	PugiXmlNode itemElement = RootElement.child("data");

	while (!itemElement.empty())
	{
		int type = 0;
		if (!PugiGetSubNodeValue(itemElement, "type", type) || type < 0 || type >= OfflineExpFetchConfig::MAX_TYPE_COUT)
		{
			return -1;
		}

		OfflineExpFetchConfig::ExpConfig &off_exp = m_offline_fetch_config.expconfig[type];
		off_exp.type = type;

		if (!PugiGetSubNodeValue(itemElement, "factor", off_exp.factor) ||off_exp.factor <= 0)
		{
			return -2;
		}
		
		if (!PugiGetSubNodeValue(itemElement, "vip_level", off_exp.vip_level) ||off_exp.vip_level < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(itemElement, "diamond", off_exp.diamond) ||off_exp.diamond < 0)
		{
			return -4;
		}

		itemElement = itemElement.next_sibling();
	}

	return 0;
}

int WelfareConfig::InitDailyFindCfg(PugiXmlNode RootElement)
{
	PugiXmlNode itemElement = RootElement.child("data");

	int last_find_type = -1;
	while (!itemElement.empty())
	{
		int type = 0;
		if (!PugiGetSubNodeValue(itemElement, "type", type) || type < 0 || type >= DAILYFIND_TYPE_MAX || type != last_find_type + 1)
		{
			return -1;
		}
		last_find_type = type;

		DailyFindConfigItem &cfg_item = m_daily_find[type];

		if (!PugiGetSubNodeValue(itemElement, "daycount_id", cfg_item.daycount_id) || cfg_item.daycount_id < 0 || cfg_item.daycount_id >= DayCounter::DAYCOUNT_ID_MAX)
		{
			return -3;
		}
	
// 		if (cfg_item.daycount_id <= DayCounter::DAYCOUNT_ID_FB_END && !LOGIC_CONFIG->GetFBConfig().IsDaycountIDExist(cfg_item.daycount_id))
// 		{
// 			return -4;
// 		}
		
		if (!PugiGetSubNodeValue(itemElement, "count_perday", cfg_item.count_perday) || m_daily_find[type].count_perday <= 0 || m_daily_find[type].count_perday > 100)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(itemElement, "gold_cost_per_count", m_daily_find[type].gold_cost_per_count) || m_daily_find[type].gold_cost_per_count < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(itemElement, "coin_cost_per_count", m_daily_find[type].coin_cost_per_count) || m_daily_find[type].coin_cost_per_count < 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(itemElement, "gold_regular_cost", m_daily_find[type].gold_regular_cost) || m_daily_find[type].gold_regular_cost < 0)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(itemElement, "free_exp_percent", m_daily_find[type].exp_percent_list[DAILYFIND_GET_FREE]) 
			|| m_daily_find[type].exp_percent_list[DAILYFIND_GET_FREE] < 0)
		{
			return -80;
		}

		if (!PugiGetSubNodeValue(itemElement, "gold_exp_percent", m_daily_find[type].exp_percent_list[DAILYFIND_GET_GOLD]) 
			|| m_daily_find[type].exp_percent_list[DAILYFIND_GET_GOLD] < 0)
		{
			return -81;
		}

		if (!PugiGetSubNodeValue(itemElement, "free_bind_coin_percent", m_daily_find[type].bind_coin_percent_list[DAILYFIND_GET_FREE]) 
			|| m_daily_find[type].bind_coin_percent_list[DAILYFIND_GET_FREE] < 0)
		{
			return -82;
		}

		if (!PugiGetSubNodeValue(itemElement, "gold_bind_coin_percent", m_daily_find[type].bind_coin_percent_list[DAILYFIND_GET_GOLD]) 
			|| m_daily_find[type].bind_coin_percent_list[DAILYFIND_GET_GOLD] < 0)
		{
			return -83;
		}

		if (!PugiGetSubNodeValue(itemElement, "free_honor_percent", m_daily_find[type].honor_percent_list[DAILYFIND_GET_FREE]) 
			|| m_daily_find[type].honor_percent_list[DAILYFIND_GET_FREE] < 0)
		{
			return -84;
		}

		if (!PugiGetSubNodeValue(itemElement, "gold_honor_percent", m_daily_find[type].honor_percent_list[DAILYFIND_GET_GOLD]) 
			|| m_daily_find[type].honor_percent_list[DAILYFIND_GET_GOLD] < 0)
		{
			return -85;
		}

		if (!PugiGetSubNodeValue(itemElement, "free_item_percent", m_daily_find[type].item_percent_list[DAILYFIND_GET_FREE]) 
			|| m_daily_find[type].item_percent_list[DAILYFIND_GET_FREE] < 0)
		{
			return -86;
		}

		if (!PugiGetSubNodeValue(itemElement, "gold_item_percent", m_daily_find[type].item_percent_list[DAILYFIND_GET_GOLD]) 
			|| m_daily_find[type].item_percent_list[DAILYFIND_GET_GOLD] < 0)
		{
			return -87;
		}

		if (!PugiGetSubNodeValue(itemElement, "free_gongxian_percent", m_daily_find[type].gongxian_percent_list[DAILYFIND_GET_FREE]) 
			|| m_daily_find[type].gongxian_percent_list[DAILYFIND_GET_FREE] < 0)
		{
			return -88;
		}

		if (!PugiGetSubNodeValue(itemElement, "gold_gongxian_percent", m_daily_find[type].gongxian_percent_list[DAILYFIND_GET_GOLD]) 
			|| m_daily_find[type].gongxian_percent_list[DAILYFIND_GET_GOLD] < 0)
		{
			return -89;
		}

		if (!PugiGetSubNodeValue(itemElement, "find_exp_factor", m_daily_find[type].find_exp_factor) || m_daily_find[type].find_exp_factor < 0)
		{
			return 10;
		}

		m_daily_find[type].item_count = ItemConfigData::ReadConfigList(itemElement, "reward_item", m_daily_find[type].reward_list, MAX_ATTACHMENT_ITEM_NUM);
		if (m_daily_find[type].item_count < 0)
		{
			return -100 + m_daily_find[type].item_count;
		}

		itemElement = itemElement.next_sibling();
	}

	return 0;
}

int WelfareConfig::InitOtherCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -10000;
	}

	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -20000;
	}

	if (!PugiGetSubNodeValue(data_element, "find_sign_in_cost", m_other_cfg.find_sign_in_cost) || m_other_cfg.find_sign_in_cost <= 0)
	{
		return -1;
	}

	if (!PugiGetSubNodeValue(data_element, "offline_exp_level_limit", m_other_cfg.offline_exp_level_limit) || m_other_cfg.offline_exp_level_limit < 0)
	{
		return -2;
	}

	if (!PugiGetSubNodeValue(data_element, "offline_exp_factor", m_other_cfg.offline_exp_factor) || m_other_cfg.offline_exp_factor < 0)
	{
		return -3;
	}

	if (!PugiGetSubNodeValue(data_element, "exp_itemid", m_other_cfg.exp_itemid) || m_other_cfg.exp_itemid <= 0 || (NULL == ITEMPOOL->GetItem(m_other_cfg.exp_itemid)))
	{
		return -4;
	}

	if (!PugiGetSubNodeValue(data_element, "paohuan_level_limit", m_other_cfg.paohuan_level_limit) || m_other_cfg.paohuan_level_limit < 0)
	{
		return -5;
	}

	return 0;
}

int WelfareConfig::InitSignInCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -10000;
	}

	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -20000;
	}

	while (!data_element.empty())
	{
		int month = 0;
		if (!PugiGetSubNodeValue(data_element, "month", month) || month < 0 || month >= WelfareSignInConfig::MONTH_CONFIG_COUNT)
		{
			return -1;
		}

		int day = 0;
		if (!PugiGetSubNodeValue(data_element, "day", day) || day <= 0 || day > WelfareSignInConfig::DAY_PER_MONTH)
		{
			return -2;
		}

		WelfareSignInConfig::DayConfig &day_cfg = m_signin_cfg.month_cfglist[month].day_cfglist[day];

		PugiXmlNode reward_item_element = data_element.child("reward_item");
		if (reward_item_element.empty())
		{
			return -3;
		}

		if (!day_cfg.reward_item.ReadConfig(reward_item_element))
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "vip", day_cfg.vip) || day_cfg.vip < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "multiple", day_cfg.multiple) || day_cfg.multiple < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "diamond", day_cfg.diamond) || day_cfg.diamond < 0)
		{
			return -7;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int WelfareConfig::InitAccumulationSignInCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -10001;
	}

	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -20001;
	}


	while (!data_element.empty())
	{
		int month = -1;
		if (!PugiGetSubNodeValue(data_element, "month", month) || month < 0 || month >= WelfareAccumulationSignConfig::MAX_MONTH_COUNT)
		{
			return -1;
		}

		int seq = -1;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq < 0 || seq >= WelfareAccumulationSignConfig::ACCUMULATION_SIGNIN_COUNT)
		{
			return -2;
		}

		WelfareAccumulationSignConfig::AccumulationConfig &acc_cfg = m_acc_signin_cfg.accumulation_reward_list[month][seq];
		acc_cfg.month = month;
		acc_cfg.seq = seq;

		int total_sign_in = 0;
		if (!PugiGetSubNodeValue(data_element, "total_sign_in", total_sign_in) || total_sign_in <= 0 || total_sign_in > WelfareAccumulationSignConfig::MAX_CONTINUITY_DAYS)
		{
			return -3;
		}

		acc_cfg.accumulation_days = total_sign_in;

		PugiXmlNode reward_item_element = data_element.child("reward_item");
		if (reward_item_element.empty())
		{
			return -3;
		}

		if (!acc_cfg.accumulation_reward.ReadConfig(reward_item_element))
		{
			return -4;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int WelfareConfig::InitActivityFindCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -10000;
	}

	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -20000;
	}

	int last_type = -1;

	while (!data_element.empty())
	{
		if (last_type >= ACTIVITY_FIND_TYPE_MAX)
		{
			return -40000;
		}

		int find_type = 0;
		if (!PugiGetSubNodeValue(data_element, "find_type", find_type) || find_type >= ACTIVITY_FIND_TYPE_MAX || find_type < last_type )
		{
			return -1;
		}
		last_type = find_type;

		WelfareActivityFindConfig::ConfigItem &cfg_item = m_activity_find_cfg.cfg_list[find_type];
		cfg_item.find_type = find_type;

		if (!PugiGetSubNodeValue(data_element, "activity_type", cfg_item.activity_type) || cfg_item.activity_type <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "free_find_percent", cfg_item.free_find_percent) || cfg_item.free_find_percent < 0 || cfg_item.free_find_percent > 100)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "is_open", cfg_item.is_open) || cfg_item.is_open < ACTIVITY_FIND_OPEN_INVALID || cfg_item.is_open > ACTIVITY_FIND_OPEN_FREE)
		{
			return -4;
		}
		
		if (!PugiGetSubNodeValue(data_element, "level_min", cfg_item.level_min) || cfg_item.level_min < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "level_max", cfg_item.level_max) || cfg_item.level_max < 0)
		{
			return -6;
		}

		if (cfg_item.level_min > cfg_item.level_max)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(data_element, "open_day_min", cfg_item.open_day_min) || cfg_item.open_day_min < 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(data_element, "open_day_max", cfg_item.open_day_max) || cfg_item.open_day_max < 0)
		{
			return -9;
		}

		if (cfg_item.open_day_min > cfg_item.open_day_max)
		{
			return -10;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int WelfareConfig::InitActivityFindRewardCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -10000;
	}

	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -20000;
	}

	int last_level_list[ACTIVITY_FIND_TYPE_MAX]; memset (last_level_list, 0, sizeof(last_level_list));

	while (!data_element.empty())
	{
		int find_type = 0;
		if (!PugiGetSubNodeValue(data_element, "find_type", find_type) || find_type < 0 || find_type >= ACTIVITY_FIND_TYPE_MAX)
		{
			return -1;
		}

		WelfareActivityFindRewardConfig &reward_config = m_activity_find_reward_cfglist[find_type];

		if (reward_config.cfg_count >= WelfareActivityFindRewardConfig::CONFIG_ITEM_MAX_COUNT)
		{
			return -30000;
		}

		WelfareActivityFindRewardConfig::ConfigItem &cfg_item = reward_config.cfg_list[reward_config.cfg_count];

		if (!PugiGetSubNodeValue(data_element, "level", cfg_item.limit_level) || cfg_item.limit_level <= last_level_list[find_type])
		{
			return -2;
		}
		last_level_list[find_type] = cfg_item.limit_level;

		if (!PugiGetSubNodeValue(data_element, "cost", cfg_item.cost) || cfg_item.cost <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "exp", cfg_item.exp) || cfg_item.exp < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "honor", cfg_item.honor) || cfg_item.honor < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "bind_coin", cfg_item.bind_coin) || cfg_item.bind_coin < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "guild_gongxian", cfg_item.guild_gongxian) || cfg_item.guild_gongxian < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(data_element, "cross_honor", cfg_item.cross_honor) || cfg_item.cross_honor < 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(data_element, "mo_jing", cfg_item.mo_jing) || cfg_item.mo_jing < 0)
		{
			return -9;
		}

		cfg_item.item_count = ItemConfigData::ReadConfigList(data_element, "reward_item", cfg_item.item_list, WelfareActivityFindRewardConfig::REWARD_ITEM_MAX_COUNT);
		if (cfg_item.item_count < 0)
		{
			return -100 + cfg_item.item_count;
		}
// 		PugiXmlNode item_list_element = data_element.child("reward_item_list");
// 		if (NULL != item_list_element)
// 		{
// 			PugiXmlNode item_element = item_list_element.child("reward_item");
// 			if (NULL != item_element)
// 			{
// 				int item_count = 0;
// 				while (!item_element.empty())
// 				{
// 					if (item_count >= WelfareActivityFindRewardConfig::REWARD_ITEM_MAX_COUNT)
// 					{
// 						return -9;
// 					}
// 
// 					if (!cfg_item.item_list[item_count].ReadConfig(item_element))
// 					{
// 						return -10;
// 					}
// 
// 					++ item_count;
// 					item_element = item_element.next_sibling();
// 				}
// 				cfg_item.item_count = item_count;
// 			}
// 		}
		if (!PugiGetSubNodeValue(data_element, "find_exp_factor", cfg_item.find_exp_factor) || cfg_item.find_exp_factor < 0)
		{
			return -10;
		}

		if (0 == cfg_item.exp && 0 == cfg_item.bind_coin && 0 == cfg_item.honor && 0 == cfg_item.guild_gongxian && 0 == cfg_item.cross_honor && 0 == cfg_item.item_count &&
			0== cfg_item.mo_jing && cfg_item.find_exp_factor == 0.0)
		{
			return -40000;
		}

		++ reward_config.cfg_count;
		data_element = data_element.next_sibling();
	}

	for (int i = 0; i < ACTIVITY_FIND_TYPE_MAX; ++ i)
	{
		if (m_activity_find_reward_cfglist[i].cfg_count <= 0)
		{
			continue;
		}

		if (1 != m_activity_find_reward_cfglist[i].cfg_list[0].limit_level)
		{
			return -50001;
		}
	}

	return 0;
}

int WelfareConfig::InitUplevelCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -10000;
	}

	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -20000;
	}

	int cfg_count = 0, last_level = 0, last_seq = -1;

	while (!data_element.empty())
	{
		if (cfg_count >= WelfareUPlevelConfig::CFG_ITEM_MAX_COUNT)
		{
			return -30000;
		}

		WelfareUPlevelConfig::ConfigItem &cfg_item = m_uplevel_cfg.cfg_list[cfg_count];

		if (!PugiGetSubNodeValue(data_element, "seq", cfg_item.seq) || cfg_item.seq != last_seq + 1)
		{
			return -5;
		}
		last_seq = cfg_item.seq;

		if (!PugiGetSubNodeValue(data_element, "level", cfg_item.level) || (0 != last_level && cfg_item.level <= last_level))
		{
			return -1;
		}
		last_level = cfg_item.level;

		PugiXmlNode reward_item_list_element = data_element.child("reward_item_list");
		if (reward_item_list_element.empty())
		{
			return -2;
		}

		PugiXmlNode reward_item_element = reward_item_list_element.child("reward_item");
		if (reward_item_element.empty())
		{
			return -3;
		}

		int item_count = 0;
		while (!reward_item_element.empty())
		{
			if (!cfg_item.item_list[item_count].ReadConfig(reward_item_element))
			{
				return -4;
			}

			++ item_count;
			reward_item_element = reward_item_element.next_sibling();
		}
		cfg_item.item_count = item_count;

		++ cfg_count;
		data_element = data_element.next_sibling();
	}
	m_uplevel_cfg.cfg_count = cfg_count;

	return 0;
}

const WelfareUPlevelConfig::ConfigItem * WelfareConfig::GetUplevelCfg(int seq)
{
	if (seq < 0 || seq >= WelfareUPlevelConfig::CFG_ITEM_MAX_COUNT || seq >= m_uplevel_cfg.cfg_count)
	{
		return NULL;
	}

	return &m_uplevel_cfg.cfg_list[seq];
}

const WelfareSignInConfig::DayConfig * WelfareConfig::GetSignInCfg(int month, int day)
{
	if (month < 0 || month >= WelfareSignInConfig::MONTH_CONFIG_COUNT)
	{
		return NULL;
	}

	if (day <= 0 || day > WelfareSignInConfig::DAY_PER_MONTH)
	{
		return NULL;
	}

	return &m_signin_cfg.month_cfglist[month].day_cfglist[day];
}

const WelfareAccumulationSignConfig::AccumulationConfig * WelfareConfig::GetAccumulationSignInCfg(int month, int seq)
{
	if (month < 0 || month >= WelfareAccumulationSignConfig::MAX_MONTH_COUNT)
	{
		return NULL;
	}

	if (seq < 0 || seq >= WelfareAccumulationSignConfig::ACCUMULATION_SIGNIN_COUNT)
	{
		return NULL;
	}

	WelfareAccumulationSignConfig::AccumulationConfig &cfg = m_acc_signin_cfg.accumulation_reward_list[month][seq];
	
	if (cfg.month != month || cfg.seq != seq)
	{
		return NULL;
	}

	return &m_acc_signin_cfg.accumulation_reward_list[month][seq];
}

const WelfareActivityFindConfig::ConfigItem * WelfareConfig::GetActivityFindConfig(int activity_find_type)
{
	if (activity_find_type < 0 || activity_find_type >= ACTIVITY_FIND_TYPE_MAX)
	{
		return NULL;
	}

	if (m_activity_find_cfg.cfg_list[activity_find_type].find_type < 0)
	{
		return NULL;
	}

	return &m_activity_find_cfg.cfg_list[activity_find_type];
}


bool WelfareConfig::GetFindType(int activity_type, int *find_type, int hold_times)
{
	if (NULL != find_type && hold_times > 0) 
	{
		int hit_times = 0;

		for (int i = 0; i < ACTIVITY_FIND_TYPE_MAX; i++)
		{
			if (m_activity_find_cfg.cfg_list[i].activity_type != activity_type)
			{
				continue;
			}

			++ hit_times;
			if (hit_times == hold_times)
			{
				*find_type = i;
				return true;
			}
		}
	}

	return false;
}

const WelfareActivityFindRewardConfig::ConfigItem * WelfareConfig::GetActivityFindReward(int activity_find_type, int level)
{
	if (activity_find_type >= 0 && activity_find_type < ACTIVITY_FIND_TYPE_MAX)
	{
		const WelfareActivityFindRewardConfig &reward_cfg = m_activity_find_reward_cfglist[activity_find_type];

		if (reward_cfg.cfg_count <= 0)
		{
			return NULL;
		}

		for (int i = reward_cfg.cfg_count - 1; i >= 0; -- i)
		{
			if (level >= reward_cfg.cfg_list[i].limit_level)
			{
				return &reward_cfg.cfg_list[i];
			}
		}
	}

	return NULL;
}

const double WelfareConfig::GetActivityFindFactor(int activity_find_type, int level) const
{
	if (activity_find_type >= 0 && activity_find_type < ACTIVITY_FIND_TYPE_MAX)
	{
		const WelfareActivityFindRewardConfig &reward_cfg = m_activity_find_reward_cfglist[activity_find_type];

		if (reward_cfg.cfg_count <= 0)
		{
			return 0.0;
		}

		for (int i = reward_cfg.cfg_count - 1; i >= 0; --i)
		{
			if (level >= reward_cfg.cfg_list[i].limit_level)
			{
				return reward_cfg.cfg_list[i].find_exp_factor;
			}
		}
	}

	return 0.0;
}

const WelfareExchangeConfig::ExchangeCfg * WelfareConfig::GetExchangeCfg(int part)
{
	if (part < 0 || part >= WelfareExchangeConfig::MAX_CAN_EXCHANGEITEM_REWARD_ITEM_NUM)
	{
		return NULL;
	}
	
	return &m_exchange_cfg.exchange_cfg[part];
}

const WelfareGrowthValueConfig::GrowthCfg * WelfareConfig::GetGrowthValueCfg(int tree_level, int type)
{
	if (tree_level <= 0 || tree_level > MAX_HAPPY_TREE_LEVEL || type < 0 || type >= GET_HAPPYTREE_VALUE_TYPE_MAX)
	{
		return NULL;
	}
	
	return &m_growth_value_cfg.growthcfg_list[tree_level][type + 1];
}

const WelfareGetGrowthConfig::configitem * WelfareConfig::GetGrowthCfg(int type)
{
	if (type < 0 || type > GET_HAPPYTREE_VALUE_TYPE_MAX)
	{
		return NULL;
	}

	return &m_get_growth_cfg.item_list[type];
}

int WelfareConfig::InitExchangeCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -10003;
	}

	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -20003;
	}

	while (!data_element.empty())
	{
		int seq = 0;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) ||seq < 0 ||seq >= WelfareExchangeConfig::MAX_CAN_EXCHANGEITEM_REWARD_ITEM_NUM)
		{
			return -1;
		}

		WelfareExchangeConfig::ExchangeCfg &exchange_cfg = m_exchange_cfg.exchange_cfg[seq];

		if (!PugiGetSubNodeValue(data_element, "exchange_growth_val",exchange_cfg.exchange_growth_val) || exchange_cfg.exchange_growth_val <= 0)
		{
			return -2;
		}

		PugiXmlNode reward_item_element = data_element.child("reward_item");
		if (reward_item_element.empty())
		{
			return -3;
		}

		if (!exchange_cfg.reward_item.ReadConfig(reward_item_element))
		{
			return -4;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int WelfareConfig::InitGrowthValueCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -10004;
	}

	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -20004;
	}

	while (!data_element.empty())
	{
		int tree_level = 0;
		if (!PugiGetSubNodeValue(data_element, "tree_level", tree_level) ||tree_level <= 0 ||tree_level > MAX_HAPPY_TREE_LEVEL)
		{
			return -1;
		}

		int fetch_type = 0;
		if (!PugiGetSubNodeValue(data_element, "fetch_type",fetch_type) || fetch_type <= 0 || fetch_type > GET_HAPPYTREE_VALUE_TYPE_MAX)
		{
			return -2;
		}
	
		WelfareGrowthValueConfig::GrowthCfg &growth_cfg = m_growth_value_cfg.growthcfg_list[tree_level][fetch_type];

		if (!PugiGetSubNodeValue(data_element, "max_value",growth_cfg.max_growth_value) || growth_cfg.max_growth_value <= 0)
		{
			return -3;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int WelfareConfig::InitGetGrowthCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -10004;
	}

	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -20004;
	}

	while (!data_element.empty())
	{
		int get_type_id = 0;
		if (!PugiGetSubNodeValue(data_element, "get_type_id", get_type_id) ||get_type_id < 0 ||get_type_id > GET_HAPPYTREE_VALUE_TYPE_MAX)
		{
			return -1;
		}

		 WelfareGetGrowthConfig::configitem  &growth_cfg = m_get_growth_cfg.item_list[get_type_id];

		if (!PugiGetSubNodeValue(data_element, "add_value",growth_cfg.add_value) || growth_cfg.add_value < 0)
		{
			return -2;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int WelfareConfig::InitChongjihaoli(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -1;
	}

	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -2;
	}
	
	int last_index = -1;
	while (!data_element.empty())
	{
		WelfaceChongjihaoli chong_level_gift;

		if (!PugiGetSubNodeValue(data_element, "index", chong_level_gift.index) || chong_level_gift.index < 0 || last_index + 1 != chong_level_gift.index)
		{
			return -3;
		}

		last_index = chong_level_gift.index;

		if (!PugiGetSubNodeValue(data_element, "level", chong_level_gift.level) || chong_level_gift.level <= 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "is_limit_num", chong_level_gift.is_limit_num) || chong_level_gift.is_limit_num < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "limit_num", chong_level_gift.limit_num) || chong_level_gift.limit_num < 0 || (1 == chong_level_gift.is_limit_num && 0 == chong_level_gift.limit_num))
		{
			return -6;
		}

		{
			PugiXmlNode element = data_element.child("reward_item_list");
			if (element.empty())
			{
				return -7;
			}

			int i = 0;
			PugiXmlNode item_element = element.child("reward_item");
			while (!item_element.empty())
			{
				if (i >= MAX_ATTACHMENT_ITEM_NUM)
				{
					return -8;
				}
				
				ItemID itemid = 0;
				if (NULL != item_element && PugiGetSubNodeValue(item_element, "item_id", itemid) && NULL != ITEMPOOL->GetItem(itemid) )
				{
					if (!chong_level_gift.reward_item_list[i].ReadConfig(item_element))
					{
						return -9;
					}

				}else
				{
					return -10;
				}

				chong_level_gift.item_count++;
				i++;
				item_element = item_element.next_sibling();
			}
		}

		m_welface_chongjihaoli_gift.push_back(chong_level_gift);

		data_element = data_element.next_sibling();
	}

	return 0;
}

const WelfaceChongjihaoli *WelfareConfig::GetChongjihaoliItemCfg(int level)
{
	int size = static_cast<int>(m_welface_chongjihaoli_gift.size());
	for (int i = 0; i < size; ++ i)
	{
		if(m_welface_chongjihaoli_gift[i].level == level)
		{
			return &m_welface_chongjihaoli_gift[i];
		}
	}

	return NULL;
}