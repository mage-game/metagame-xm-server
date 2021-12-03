#ifndef __VIPCONFIG_HPP__
#define __VIPCONFIG_HPP__

#include "servercommon/struct/itemlistparam.h"
#include "servercommon/serverdef.h"
#include "gamedef.h"
#include "servercommon/vipdef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

#include <map>
#include <string>

struct VipLevelReward
{
	const static int ITEM_CONFIG_MAX_COUNT = 24;
	UNSTD_STATIC_CHECK(ITEM_CONFIG_MAX_COUNT <= 32);

	struct ConfigItem
	{
		ConfigItem() : seq(0), need_level(0), week_reward_num(0), cost(0)
		{

		}

		int seq;
		int need_level;
		int week_reward_num;
		int cost;
		ItemConfigData reward_item;
	};

	VipLevelReward() : cfg_count(0)
	{

	}

	int cfg_count;
	ConfigItem cfg_list[ITEM_CONFIG_MAX_COUNT];
};

struct QiFuRewardConfig
{
	QiFuRewardConfig() : level(0), coin(0),yuanli(0), xianhun(0) {}

	int level;
	int coin;
	int yuanli;
	int xianhun;
};

static const int QIFU_COST_CFG_MAX_COUNT = 100;
struct QiFuCostConfig
{
	QiFuCostConfig() : min_buy_times(0), max_buy_times(0), buycoin_cost(0), buyyuanli_cost(0), buyxianhun_cost(0) 
	{

	}

	int min_buy_times;
	int max_buy_times;

	int buycoin_cost;
	int buyyuanli_cost;
	int buyxianhun_cost;
};

struct QiFuTimesRewardConfig
{
	static const int REWARD_CFG_ITEM_MAX_COUNT = 8;
	UNSTD_STATIC_CHECK(REWARD_CFG_ITEM_MAX_COUNT == 8);

	struct QiFuRewardItemCfg
	{
		QiFuRewardItemCfg() :  seq(0), need_count(0), reward_coin_factor(0), reward_yuanli_factor(0), reward_xianhun_factor(0)
		{

		} 

		int seq;
		int need_count;

		double reward_coin_factor;
		double reward_yuanli_factor;
		double reward_xianhun_factor;
	};

	QiFuTimesRewardConfig() : type(0), cfg_count(0) 
	{

	}

	int type;
	int cfg_count;
	QiFuRewardItemCfg cfg_list[REWARD_CFG_ITEM_MAX_COUNT];
};

struct VipBuffConfig
{
	VipBuffConfig() : level(0), maxhp(0), gongji(0), fangyu(0) {}

	int level;
	Attribute maxhp;
	Attribute gongji;
	Attribute fangyu;
};

struct VipOtherConfig
{
	static const int REWARD_EXP_CFG_MAX_COUNT = 8;

	VipOtherConfig() : qifu_coin_free_times(0), qifu_yuanli_free_times(0), qifu_xianhun_free_times(0), week_reward_id(0), time_limit_vip_time(0), exp_day_open(0){}

	int qifu_coin_free_times;
	int qifu_yuanli_free_times;
	int qifu_xianhun_free_times;
	int week_reward_id;
	unsigned int time_limit_vip_time;
	int exp_day_open;
};

struct OnlineVipExpRewradCfg
{
	OnlineVipExpRewradCfg() :seq(0), vip_exp(0){}
	int seq;
	int vip_exp;
};

class VipConfig : public ConfigBase
{
public:

	VipConfig();
	~VipConfig();

	bool Init(const std::string &configname, std::string *err);

	int GetAuthParam(int vip_level, int auth_type);
	int GetVipExpMaxByLevel(int vip_level);

	const VipLevelReward::ConfigItem * GetLevelReward(int seq);

	const VipBuffConfig *GetVipBuffCfg(int vip_level) const;

	const VipOtherConfig & GetOtherCfg() { return m_other_cfg; }

	const OnlineVipExpRewradCfg *GetOnlineRewardVipExp(int online_time) const;

private:
	int InitLevelCfg(PugiXmlNode RootElement);
	int InitUplevelCfg(PugiXmlNode RootElement);
	int InitLevelReward(PugiXmlNode RootElement);

	int InitQiFuCostCfg(PugiXmlNode RootElement);
	int InitQiFuTimesRewardCfg(PugiXmlNode RootElement);
	int InitQiFuRewardCfg(PugiXmlNode RootElement);
	int InitVipBuffcfg(PugiXmlNode RootElement);
	int InitWeekRewardCfg(PugiXmlNode RootElement);
	int InitOtherCfg(PugiXmlNode RootElement);
	int InitOnlineVipExpRewardCfg(PugiXmlNode RootElement);
	
	int m_auth_list[MAX_VIP_LEVEL + 1][VAT_MAX];
	int m_max_exp_list[MAX_VIP_LEVEL + 1];

	VipLevelReward m_level_reward;

	QiFuRewardConfig m_qifu_reward_cfg[MAX_ROLE_LEVEL + 1];
	QiFuTimesRewardConfig m_qifu_times_reward_cfg[QIFU_TYPE_MAX];

	int m_qifu_cost_cfg_count;
	QiFuCostConfig m_qifu_cost_cfglist[QIFU_COST_CFG_MAX_COUNT];

	VipBuffConfig m_vipbuff_cfg[MAX_VIP_LEVEL + 1];

	VipOtherConfig m_other_cfg;

	std::map<int, OnlineVipExpRewradCfg> m_online_vip_exp_reward_map;
};

#endif // __VIPCONFIG_HPP__


