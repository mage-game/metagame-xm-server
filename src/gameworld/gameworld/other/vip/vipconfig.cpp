#include "vipconfig.hpp"
#include "servercommon/configcommon.h"

#include "item/itempool.h"
#include "item/itembase.h"
#include "checkresourcecenter.hpp"

VipConfig::VipConfig() : m_qifu_cost_cfg_count(0)
{
	memset(m_auth_list, 0, sizeof(m_auth_list));
	memset(m_max_exp_list, 0, sizeof(m_max_exp_list));
}

VipConfig::~VipConfig()
{

}

bool VipConfig::Init(const std::string &configname, std::string *err)
{
	char errinfo[1024]= {0};
	int iRet = 0;

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, configname, "VipConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		sprintf(errinfo, "Load vip config [%s] Error.cannot find root node.", configname.c_str());
		*err = errinfo;
		return false;
	}

	{
		// 等级配置
		PugiXmlNode lvl_element = RootElement.child("level");
		if (lvl_element.empty())
		{
			*err = configname + ": no [level].";
			return false;
		}

		iRet = InitLevelCfg(lvl_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitLevelCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 升级配置
		PugiXmlNode root_element = RootElement.child("uplevel");
		if (root_element.empty())
		{
			*err = configname + ": no [uplevel].";
			return false;
		}

		iRet = InitUplevelCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitUplevelCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 升级配置
		PugiXmlNode root_element = RootElement.child("level_reward");
		if (root_element.empty())
		{
			*err = configname + ": no [level_reward].";
			return false;
		}

		iRet = InitLevelReward(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitLevelReward failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 祈福的摇奖励花费配置
		PugiXmlNode root_element = RootElement.child("qifu_cost");
		if (root_element.empty())
		{
			*err = configname + ": no [qifu_cost].";
			return false;
		}

		iRet = InitQiFuCostCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitQiFuCostCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 祈福的结果配置
		PugiXmlNode root_element = RootElement.child("qifu_reward");
		if (root_element.empty())
		{
			*err = configname + ": no [qifu_reward].";
			return false;
		}

		iRet = InitQiFuRewardCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitQiFuRewardCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 其他
		PugiXmlNode root_element = RootElement.child("other");
		if (root_element.empty())
		{
			*err = configname + ": no [other].";
			return false;
		}

		iRet = InitOtherCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitOtherCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 祈福的领取奖励的配置
		PugiXmlNode root_element = RootElement.child("qifu_times_reward");
		if (root_element.empty())
		{
			*err = configname + ": no [qifu_times_reward].";
			return false;
		}

		iRet = InitQiFuTimesRewardCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitQiFuTimesRewardCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// vipbuff配置
		PugiXmlNode root_element = RootElement.child("vipbuff");
		if (root_element.empty())
		{
			*err = configname + ": no [vipbuff].";
			return false;
		}

		iRet = InitVipBuffcfg(root_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitVipBuffcfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 在线获得vip经验配置
		PugiXmlNode root_element = RootElement.child("vip_gain_exp");
		if (root_element.empty())
		{
			*err = configname + ": no [vip_gain_exp].";
			return false;
		}

		iRet = InitOnlineVipExpRewardCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitOnlineVipExpRewardCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

int VipConfig::InitLevelCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	while (!dataElement.empty())
	{
		int auth_type = 0;
		if (!PugiGetSubNodeValue(dataElement, "auth_type", auth_type) || auth_type < 0 || auth_type >= VAT_MAX)
		{
			return -1;
		}

		char tag_name[32] = {0};

		for (int i = 0; i <= MAX_VIP_LEVEL; ++i)
		{
			SNPRINTF(tag_name, sizeof(tag_name), "param_%d", i);

			int param = 0;
			if (!PugiGetSubNodeValue(dataElement, tag_name, param))
			{
				return -600;
			}

			m_auth_list[i][auth_type] = param;
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int VipConfig::InitUplevelCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");

	int last_vip_level = -1;

	while (!dataElement.empty())
	{
		int vip_level = 0;
		if (!PugiGetSubNodeValue(dataElement, "level", vip_level) || vip_level < 0 || vip_level > MAX_VIP_LEVEL || vip_level != last_vip_level + 1)
		{
			return -1;
		}
		last_vip_level = vip_level;

		if (!PugiGetSubNodeValue(dataElement, "need_gold", m_max_exp_list[vip_level]) || m_max_exp_list[vip_level] <= 0)
		{
			return -2;
		}

		dataElement = dataElement.next_sibling();
	}

	if (last_vip_level != MAX_VIP_LEVEL)
	{
		return -1000000;
	}

	return 0;
}

int VipConfig::InitLevelReward(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -1000000;
	}

	int last_seq = -1;

	while (!dataElement.empty())
	{
		int seq = 0;
		if (!PugiGetSubNodeValue(dataElement, "seq", seq) || seq >= VipLevelReward::ITEM_CONFIG_MAX_COUNT || seq != last_seq + 1)
		{
			return -1;
		}
		last_seq = seq;

		VipLevelReward::ConfigItem &cfg_item = m_level_reward.cfg_list[seq];
		cfg_item.seq =seq;

		if (!PugiGetSubNodeValue(dataElement, "level", cfg_item.need_level) || cfg_item.need_level <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "week_reward_num", cfg_item.week_reward_num) || cfg_item.week_reward_num <= 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "cost", cfg_item.cost) || cfg_item.cost <= 0)
		{
			return -5;
		}

		PugiXmlNode reward_item_element = dataElement.child("reward_item");
		if (reward_item_element.empty())
		{
			return -3;
		}
		if (!cfg_item.reward_item.ReadConfig(reward_item_element))
		{
			return -4;
		}

		dataElement = dataElement.next_sibling();
	}

	m_level_reward.cfg_count = last_seq + 1;

	return 0;
}

int VipConfig::InitQiFuCostCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -1000000;
	}

	m_qifu_cost_cfg_count = 0;

	int last_max_times = 0;

	while (!dataElement.empty())
	{
		if (m_qifu_cost_cfg_count >= QIFU_COST_CFG_MAX_COUNT)
		{
			return -1000000;
		}

		QiFuCostConfig &qf_cost = m_qifu_cost_cfglist[m_qifu_cost_cfg_count];

		if (!PugiGetSubNodeValue(dataElement, "min_buy_times", qf_cost.min_buy_times) ||  qf_cost.min_buy_times != last_max_times + 1)
		{
			return -1;
		}

		if(!PugiGetSubNodeValue(dataElement, "max_buy_times", qf_cost.max_buy_times) || qf_cost.max_buy_times < qf_cost.min_buy_times)
		{
			return -2;
		}

		last_max_times = qf_cost.max_buy_times;

		if (!PugiGetSubNodeValue(dataElement, "buy_coin_cost", qf_cost.buycoin_cost) || qf_cost.buycoin_cost <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "buy_yuanli_cost", qf_cost.buyyuanli_cost) || qf_cost.buyyuanli_cost <= 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "buy_xianhun_cost", qf_cost.buyxianhun_cost) || qf_cost.buyxianhun_cost <= 0)
		{
			return -5;
		}

		++ m_qifu_cost_cfg_count;
		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int VipConfig::InitQiFuTimesRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -100000;
	}

	int last_seq_list[QIFU_TYPE_MAX];

	for (int i = 0; i < QIFU_TYPE_MAX; ++ i)
	{
		last_seq_list[i] = -1;
	}

	while (!dataElement.empty())
	{
		int type = 0;
		if (!PugiGetSubNodeValue(dataElement, "type", type) || type < 0 || type > QIFU_TYPE_MAX)
		{
			return -1;
		}

		QiFuTimesRewardConfig &reward_cfg = m_qifu_times_reward_cfg[type];
		reward_cfg.type = type;

		if (reward_cfg.cfg_count >= QiFuTimesRewardConfig::REWARD_CFG_ITEM_MAX_COUNT)
		{
			return -2000000;
		}

		int seq = -1;
		if (!PugiGetSubNodeValue(dataElement, "seq", seq) || seq < 0 || seq >= QiFuTimesRewardConfig::REWARD_CFG_ITEM_MAX_COUNT || 
			seq != last_seq_list[type] + 1 || seq != reward_cfg.cfg_count)
		{
			return -2;
		}
		last_seq_list[type] = seq;

		QiFuTimesRewardConfig::QiFuRewardItemCfg &cfg_item = reward_cfg.cfg_list[reward_cfg.cfg_count];
		cfg_item.seq = seq;

		if (!PugiGetSubNodeValue(dataElement, "need_count", cfg_item.need_count) || cfg_item.need_count <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "reward_coin_factor", cfg_item.reward_coin_factor) || cfg_item.reward_coin_factor < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "reward_xianhun_factor", cfg_item.reward_xianhun_factor) || cfg_item.reward_xianhun_factor < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "reward_yuanli_factor", cfg_item.reward_yuanli_factor) || cfg_item.reward_yuanli_factor < 0)
		{
			return -6;
		}

		if (cfg_item.reward_coin_factor <= 0 && cfg_item.reward_xianhun_factor <= 0 && cfg_item.reward_yuanli_factor <= 0)
		{
			return -7;
		}

		++ reward_cfg.cfg_count;
		dataElement = dataElement.next_sibling();
	}	

	return 0;
}

int VipConfig::InitQiFuRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	int last_level = 0;

	while (!dataElement.empty())
	{
		QiFuRewardConfig reward_cfg;

		if (!PugiGetSubNodeValue(dataElement, "level", reward_cfg.level) || reward_cfg.level < 0 || reward_cfg.level > MAX_ROLE_LEVEL || reward_cfg.level != last_level + 1)
		{
			return -1;
		}
		last_level = reward_cfg.level;

		if (!PugiGetSubNodeValue(dataElement, "coin", reward_cfg.coin) || reward_cfg.coin <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "yuanli", reward_cfg.yuanli) || reward_cfg.yuanli <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "xianhun", reward_cfg.xianhun) || reward_cfg.xianhun <= 0)
		{
			return -4;
		}

		m_qifu_reward_cfg[last_level] = reward_cfg;
		dataElement = dataElement.next_sibling();
	}
// 
// 	if (last_level != MAX_ROLE_LEVEL)
// 	{
// 		return -100000;
// 	}

	return 0;
}

int VipConfig::InitVipBuffcfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	int last_level = 0;
	while (!dataElement.empty())
	{
		VipBuffConfig vipbuff_cfg;
		if (!PugiGetSubNodeValue(dataElement, "level", vipbuff_cfg.level) || vipbuff_cfg.level < 0 || vipbuff_cfg.level > MAX_VIP_LEVEL || vipbuff_cfg.level != last_level + 1)
		{
			return -1;
		}
		last_level = vipbuff_cfg.level;

		if (!PugiGetSubNodeValue(dataElement, "maxhp", vipbuff_cfg.maxhp) || vipbuff_cfg.maxhp <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "gongji", vipbuff_cfg.gongji) || vipbuff_cfg.gongji <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "fangyu", vipbuff_cfg.fangyu) || vipbuff_cfg.fangyu <= 0)
		{
			return -4;
		}

		m_vipbuff_cfg[vipbuff_cfg.level] = vipbuff_cfg;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int VipConfig::InitOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	if (!PugiGetSubNodeValue(dataElement, "qifu_coin_free_times", m_other_cfg.qifu_coin_free_times) || m_other_cfg.qifu_coin_free_times < 0)
	{
		return -1;
	}

	if (!PugiGetSubNodeValue(dataElement, "qifu_yuanli_free_times", m_other_cfg.qifu_yuanli_free_times) || m_other_cfg.qifu_yuanli_free_times < 0)
	{
		return -2;
	}

	if (!PugiGetSubNodeValue(dataElement, "qifu_xianhun_free_times", m_other_cfg.qifu_xianhun_free_times) || m_other_cfg.qifu_xianhun_free_times < 0)
	{
		return -3;
	}

	if (!PugiGetSubNodeValue(dataElement, "week_reward_id", m_other_cfg.week_reward_id) || m_other_cfg.week_reward_id < 0)
	{
		return -4;
	}

	if (!PugiGetSubNodeValue(dataElement, "time_limit_vip_time", m_other_cfg.time_limit_vip_time) || m_other_cfg.time_limit_vip_time <= 0)
	{
		return -5;
	}

	if (!PugiGetSubNodeValue(dataElement, "exp_day_open", m_other_cfg.exp_day_open) || m_other_cfg.exp_day_open < 0)
	{
		return -6;
	}

	return 0;
}

int VipConfig::InitOnlineVipExpRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	int last_online_time = 0;

	while (!dataElement.empty())
	{
		int seq = 0;
		if (!PugiGetSubNodeValue(dataElement, "seq", seq) || seq < 0 || seq > VipOtherConfig::REWARD_EXP_CFG_MAX_COUNT)
		{
			return -1;
		}

		int online_time = 0;
		if (!PugiGetSubNodeValue(dataElement, "day_running_time", online_time) || online_time <= 0)
		{
			return -2;
		}

		if (online_time <= last_online_time)
		{
			return -99;
		}
		last_online_time = online_time;

		int reward_vip_exp = 0;
		if (!PugiGetSubNodeValue(dataElement, "vip_exp", reward_vip_exp) || reward_vip_exp < 0)
		{
			return -3;
		}

		if (m_online_vip_exp_reward_map.find(online_time) != m_online_vip_exp_reward_map.end())
		{
			return -100;
		}

		m_online_vip_exp_reward_map[online_time].seq = seq;
		m_online_vip_exp_reward_map[online_time].vip_exp = reward_vip_exp;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int VipConfig::GetAuthParam(int vip_level, int auth_type)
{
	if (vip_level < 0 || vip_level > MAX_VIP_LEVEL || auth_type < 0 || auth_type >= VAT_MAX)
	{
		return 0;
	}

	return m_auth_list[vip_level][auth_type];
}

int VipConfig::GetVipExpMaxByLevel(int vip_level)
{
	if (vip_level < 0 || vip_level > MAX_VIP_LEVEL)
	{
		return 999999999;
	}

	return m_max_exp_list[vip_level];
}

const VipLevelReward::ConfigItem * VipConfig::GetLevelReward(int seq)
{
	if (seq < 0 || seq >= VipLevelReward::ITEM_CONFIG_MAX_COUNT || seq >= m_level_reward.cfg_count)
	{
		return NULL;
	}

	return &m_level_reward.cfg_list[seq];
}

const VipBuffConfig *VipConfig::GetVipBuffCfg(int vip_level) const
{
	if (vip_level <= 0 || vip_level > MAX_VIP_LEVEL)
	{
		return NULL;
	}

	return &m_vipbuff_cfg[vip_level];
}

const OnlineVipExpRewradCfg *VipConfig::GetOnlineRewardVipExp(int online_time) const
{
	auto it = m_online_vip_exp_reward_map.find(online_time);
	if(it != m_online_vip_exp_reward_map.end())
	{
		return &it->second;
	}

	return NULL;
}