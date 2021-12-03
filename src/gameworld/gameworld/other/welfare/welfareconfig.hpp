#ifndef __WELFARE_CONFIG_HPP__
#define __WELFARE_CONFIG_HPP__

#include <string>

#include "servercommon/welfaredef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/activitydef.hpp"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include "item/itemextern.hpp"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct OnlineRewardConfig
{
	OnlineRewardConfig() {}

	static const int MAX_ONLINE_REWARD_COUNT = 10;
	static const int MAX_ONLINE_REWARD_ITEM_COUNT = 3;
	static const int MAX_ONLINE_MINUTES = 24 * 60;

	struct RewardConfig
	{
		RewardConfig() : minutes(0) {}

		int minutes;
		ItemConfigData reward_item[MAX_ONLINE_REWARD_ITEM_COUNT];
	};

	RewardConfig online_reward[MAX_ONLINE_REWARD_COUNT];
};

struct OfflineExpFetchConfig
{
	static const int MAX_TYPE_COUT = 3;

	OfflineExpFetchConfig() {}

	struct ExpConfig
	{
		int type;
		int factor;
		int vip_level;
		int diamond;
	};
	
	ExpConfig expconfig[MAX_TYPE_COUT];
};

struct DailyFindConfigItem
{
	DailyFindConfigItem() : daycount_id(-1), count_perday(0), gold_cost_per_count(0), coin_cost_per_count(0), gold_regular_cost(0)
	{
		memset(exp_percent_list, 0, sizeof(exp_percent_list));
		memset(bind_coin_percent_list, 0, sizeof(bind_coin_percent_list));
		memset(gongxian_percent_list, 0, sizeof(gongxian_percent_list));
		memset(honor_percent_list, 0, sizeof(honor_percent_list));
		memset(item_percent_list, 0, sizeof(item_percent_list));
	}

	bool Invalid() const { return -1 == daycount_id; }

	int daycount_id;
	int count_perday;
	int gold_cost_per_count;
	int coin_cost_per_count;
	int gold_regular_cost;

	int exp_percent_list[DAILYFIND_GET_MAX];						// 经验找回比例
	int bind_coin_percent_list[DAILYFIND_GET_MAX];					// 铜钱找回比例
	int gongxian_percent_list[DAILYFIND_GET_MAX];					// 帮贡找回比例
	int honor_percent_list[DAILYFIND_GET_MAX];						// 荣誉找回比例
	int item_percent_list[DAILYFIND_GET_MAX];						// 物品找回比例

	float find_exp_factor;
	int item_count;
	ItemConfigData reward_list[MAX_ATTACHMENT_ITEM_NUM];
};

struct WelfareOtherConfig
{
	WelfareOtherConfig() :find_sign_in_cost(0), offline_exp_level_limit(0), offline_exp_factor(0), exp_itemid(0),paohuan_level_limit(0)
	{

	}

	int find_sign_in_cost;
	int offline_exp_level_limit;
	float offline_exp_factor;
	ItemID exp_itemid;
	int paohuan_level_limit;
};

struct WelfareSignInConfig
{
	static const int DAY_PER_MONTH = 31;
	static const int MONTH_CONFIG_COUNT = 12;

	struct DayConfig
	{
		DayConfig() {}

		ItemConfigData reward_item;
		int vip;
		int multiple;
		int diamond;	// 补签消耗钻石
	};

	struct MonthConfig
	{
		MonthConfig() {}

		DayConfig day_cfglist[DAY_PER_MONTH + 1];
	};

	WelfareSignInConfig() {}

	MonthConfig month_cfglist[MONTH_CONFIG_COUNT];
};

struct WelfareAccumulationSignConfig
{
	static const int ACCUMULATION_SIGNIN_COUNT = 6;
	static const int MAX_CONTINUITY_DAYS = 31;
	static const int MAX_MONTH_COUNT = 12;

	struct AccumulationConfig
	{
		AccumulationConfig() :month(0), seq(0), accumulation_days(0) {}

		int month;
		int seq;
		int accumulation_days;
		ItemConfigData accumulation_reward;
	};
	
	WelfareAccumulationSignConfig() {}

	AccumulationConfig accumulation_reward_list[MAX_MONTH_COUNT][ACCUMULATION_SIGNIN_COUNT];
};

enum ACTIVITY_FIND_OPEN
{
	ACTIVITY_FIND_OPEN_INVALID = 0, //不开启
	ACTIVITY_FIND_OPEN_GOLD = 1,	//元宝
	ACTIVITY_FIND_OPEN_FREE = 2		//元宝加免费
};

struct  WelfareActivityFindConfig 
{
	const static int REWARD_ITEM_MAX_COUNT = 4;

	struct ConfigItem
	{
		ConfigItem() : find_type(-1), activity_type(0), free_find_percent(0),is_open(0),
			level_min(0),level_max(0),open_day_min(0),open_day_max(0)
		{

		}

		int find_type;
		int activity_type;
		int free_find_percent;
		int is_open;
		int level_min;
		int level_max;
		int open_day_min;
		int open_day_max;
	};

	WelfareActivityFindConfig() {}

	ConfigItem cfg_list[ACTIVITY_FIND_TYPE_MAX];
};

struct WelfareActivityFindRewardConfig
{
	const static int CONFIG_ITEM_MAX_COUNT = 64;
	const static int REWARD_ITEM_MAX_COUNT = 4;

	struct ConfigItem
	{
		ConfigItem() : limit_level(0), cost(0), exp(0), honor(0), bind_coin(0), guild_gongxian(0), cross_honor(0), mo_jing(0),item_count(0),find_exp_factor(0.0){}

		int limit_level;
		int cost;
		int exp;
		int honor;
		int bind_coin;
		int guild_gongxian;
		int cross_honor;
		int mo_jing;
		int item_count;
		ItemConfigData item_list[REWARD_ITEM_MAX_COUNT];
		double find_exp_factor;
	};

	WelfareActivityFindRewardConfig() : cfg_count(0) {}

	int cfg_count;
	ConfigItem cfg_list[CONFIG_ITEM_MAX_COUNT];
};

struct WelfareUPlevelConfig
{
	const static int CFG_ITEM_MAX_COUNT = 16;
	const static int REWARD_ITEM_MAX_COUNT = 4;
	UNSTD_STATIC_CHECK(CFG_ITEM_MAX_COUNT <= 32);

	struct ConfigItem 
	{
		ConfigItem() : seq(0), level(0), item_count(0)
		{
		}

		int seq;
		int level;

		int item_count;
		ItemConfigData item_list[REWARD_ITEM_MAX_COUNT];
	};

	WelfareUPlevelConfig() : cfg_count(0)
	{
	}

	int cfg_count;
	ConfigItem cfg_list[CFG_ITEM_MAX_COUNT];
};

struct WelfareHappyTreeConfig
{
	const static int MAX_HAPPY_TREE_LEVEL = 100;
	const static int MAX_FETCH_ITEM_COUT = 5;

	struct RewardCfg
	{
		RewardCfg() :fecth_type(0), growth_val(0) {}

		int fecth_type;
		int growth_val;
		ItemConfigData reward_item;
	};

	WelfareHappyTreeConfig() : item_count(0) {}

	int item_count;
	RewardCfg reward_list[MAX_FETCH_ITEM_COUT + 1];
};

struct WelfareExchangeConfig
{
	const static int MAX_CAN_EXCHANGEITEM_REWARD_ITEM_NUM = 30;

	struct ExchangeCfg
	{
		ExchangeCfg() : exchange_growth_val(0) {}

		int exchange_growth_val;
		ItemConfigData reward_item;
	};

	WelfareExchangeConfig() {}

	 ExchangeCfg exchange_cfg[MAX_CAN_EXCHANGEITEM_REWARD_ITEM_NUM];
};

struct WelfareGrowthValueConfig
{
	struct GrowthCfg
	{
		GrowthCfg() : max_growth_value(0) {}

		int max_growth_value;
	};

	WelfareGrowthValueConfig() {}

	GrowthCfg growthcfg_list[MAX_HAPPY_TREE_LEVEL + 1][GET_HAPPYTREE_VALUE_TYPE_MAX + 1];
};

struct WelfareGetGrowthConfig
{
	struct configitem
	{
		configitem() : get_type_id(0), add_value(0) { }

		int get_type_id;
		int add_value;
	};

	configitem item_list[GET_HAPPYTREE_VALUE_TYPE_MAX];
};

struct WelfaceChongjihaoli
{
	int index;
	int level;
	int is_limit_num;
	int limit_num;
	int item_count;
	ItemConfigData reward_item_list[MAX_ATTACHMENT_ITEM_NUM];
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class WelfareConfig : public ConfigBase
{
public:
	WelfareConfig();
	~WelfareConfig();

	bool Init(const std::string &path, std::string *err);

	const WelfareOtherConfig & GetOtherCfg() { return m_other_cfg; }
	const OfflineExpFetchConfig::ExpConfig* GetOfflineExpConfig(int find_type) const;
	const DailyFindConfigItem * GetDailyFindItem(int type) const;
	const WelfareUPlevelConfig::ConfigItem * GetUplevelCfg(int seq);
	const WelfareSignInConfig::DayConfig * GetSignInCfg(int month, int day);
	const WelfareAccumulationSignConfig::AccumulationConfig * GetAccumulationSignInCfg(int month, int seq);
	const WelfareActivityFindConfig::ConfigItem * GetActivityFindConfig(int activity_find_type);
	bool GetFindType(int activity_type, int *find_type, int hold_times = 1);
	const WelfareActivityFindRewardConfig::ConfigItem * GetActivityFindReward(int activity_find_type, int level);
	const double GetActivityFindFactor(int activity_find_type, int level) const;
	const OnlineRewardConfig::RewardConfig * GetOnlineRewardCfg(int part);
	const WelfareHappyTreeConfig::RewardCfg * GetHappyTreeReward(int level, int type);
	const WelfareHappyTreeConfig * GetHappyTreeLevelCfg(int level);
	const WelfareExchangeConfig::ExchangeCfg * GetExchangeCfg(int part);
	const WelfareGrowthValueConfig::GrowthCfg * GetGrowthValueCfg(int tree_lelve, int type);
	const WelfareGetGrowthConfig::configitem * GetGrowthCfg(int type);
	const WelfaceChongjihaoli * GetChongjihaoliItemCfg(int index);
	int GetWelfaceChongjihaoliLevelCount(){ return static_cast<int>(m_welface_chongjihaoli_gift.size()); }

private:
	int InitOfflineExpFetchTypeCfg(PugiXmlNode RootElement);
	int InitDailyFindCfg(PugiXmlNode RootElement);
	int InitOtherCfg(PugiXmlNode RootElement);
	int InitSignInCfg(PugiXmlNode RootElement);
	int InitAccumulationSignInCfg(PugiXmlNode RootElement);
	int InitActivityFindCfg(PugiXmlNode RootElement);
	int InitActivityFindRewardCfg(PugiXmlNode RootElement);
	int InitUplevelCfg(PugiXmlNode RootElement);
	int InitOnlineRewardCfg(PugiXmlNode RootElement);
	int InitHappyTreeCfg(PugiXmlNode RootElement);
	int InitExchangeCfg(PugiXmlNode RootElement);
	int InitGrowthValueCfg(PugiXmlNode RootElement);
	int InitGetGrowthCfg(PugiXmlNode RootElement);
	int InitChongjihaoli(PugiXmlNode RootElement);

	OnlineRewardConfig m_online_reward_config;													// 在线奖励
	OfflineExpFetchConfig m_offline_fetch_config;												// 离线经验
	DailyFindConfigItem m_daily_find[DAILYFIND_TYPE_MAX];										// 日常找回
	WelfareOtherConfig m_other_cfg;																// 其他配置
	WelfareSignInConfig m_signin_cfg;															// 签到配置
	WelfareAccumulationSignConfig m_acc_signin_cfg;												// 累计签到奖励配置
	WelfareActivityFindConfig m_activity_find_cfg;												// 活动找回配置
	WelfareActivityFindRewardConfig m_activity_find_reward_cfglist[ACTIVITY_FIND_TYPE_MAX]; 	// 活动找回奖励
	WelfareUPlevelConfig m_uplevel_cfg;															// 升级奖励
	WelfareHappyTreeConfig m_happy_tree_cfg[WelfareHappyTreeConfig::MAX_HAPPY_TREE_LEVEL + 1];	// 欢乐果树
	WelfareExchangeConfig m_exchange_cfg;														// 兑换
	WelfareGrowthValueConfig m_growth_value_cfg;												// 每日长成值上限
	WelfareGetGrowthConfig m_get_growth_cfg;													// 长成值获得配置
	std::vector<WelfaceChongjihaoli> m_welface_chongjihaoli_gift;								// 冲级豪礼
};

#endif // __WELFARE_CONFIG_HPP__
