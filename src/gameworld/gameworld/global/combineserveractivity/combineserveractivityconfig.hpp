#ifndef __COMBINE_SERVER_ACTIVITY_CONFIG_HPP__
#define __COMBINE_SERVER_ACTIVITY_CONFIG_HPP__

#include "servercommon/configcommon.h"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/maildef.hpp"
#include "servercommon/activitydef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

struct CSATimeConfig
{
	CSATimeConfig() : sub_type(0), is_open(false), begin_day(0), end_day(0), end_time(0) {}

	int sub_type;
	bool is_open;
	int begin_day;
	int end_day;
	int end_time;
};

struct CSARankRewardConfig
{
	CSARankRewardConfig() : sub_type(0), rank_limit(0) {}

	bool IsValid() { return sub_type > CSA_SUB_TYPE_INVALID && sub_type < CSA_SUB_TYPE_MAX; }

	int sub_type;
	int rank_limit;
	ItemConfigData rank_reward_items[COMBINE_SERVER_ACTIVITY_RANK_REWARD_ROLE_NUM];
};

struct CSAOtherConfig
{
	CSAOtherConfig() : gcz_winner_item_count(0), gcz_camp_item_count(0), xmz_winner_item_count(0), xmz_guild_item_count(0), 
		kill_boss_reward_cost(0), touzi_jihua_buy_cost(0), touzi_jihua_buy_reward_gold(0),
		exp_refine_grades(0), exp_refine_level_limit(0), exp_refine_max_level(0){}

	int gcz_winner_item_count;
	ItemConfigData gcz_winner_item_list[MAX_ATTACHMENT_ITEM_NUM];
	int gcz_camp_item_count;
	ItemConfigData gcz_camp_item_list[MAX_ATTACHMENT_ITEM_NUM];
	int xmz_winner_item_count;
	ItemConfigData xmz_winner_item_list[MAX_ATTACHMENT_ITEM_NUM];
	int xmz_guild_item_count;
	ItemConfigData xmz_guild_item_list[MAX_ATTACHMENT_ITEM_NUM];

	ItemConfigData kill_boss_reward_item;
	ItemConfigData boss_rank_master_reward_item;
	ItemConfigData boss_rank_member_reward_item;
	int kill_boss_reward_cost;

	int touzi_jihua_buy_cost;
	int touzi_jihua_buy_reward_gold;
	int exp_refine_grades;
	int exp_refine_level_limit;
	int exp_refine_max_level;
};

struct CSAQianggouItemConfig
{
	CSAQianggouItemConfig() : limit_num(0), cost(0) {}

	int limit_num;
	int cost;
	ItemConfigData stuff_item;
};

struct CSARollConfigItem
{
	CSARollConfigItem() : seq(0), weight(0), is_broadcast(false) {}

	int seq;
	int weight;
	bool is_broadcast;
	ItemConfigData reward_item;
};

struct CSARollConfig
{
	CSARollConfig() : roll_cost(0), total_weight(0), cfg_count(0) {}

	int roll_cost;
	int total_weight;
	int cfg_count;
	CSARollConfigItem cfg_list[COMBINE_SERVER_ROLL_CFG_MAX_COUNT];
};

struct CSASingleChargeCfg
{
	struct ConfigItem
	{
		ConfigItem() : seq(0), charge_value(0) {}

		int seq;														// 索引
		int charge_value;												// 单笔充值额度
		ItemConfigData reward_item;										// 奖励物品
	};

	CSASingleChargeCfg() : cfg_count(0) {}

	int cfg_count;
	ConfigItem cfg_list[COMBINE_SERVER_SINGLE_CHARGE_CONFIG_MAX_COUNT];
};


struct CSALoginGiftCfg
{
	struct ConfigItem
	{
		ConfigItem() : seq(0), need_login_days(0) {}

		int seq;
		int need_login_days;
		ItemConfigData common_reward_item;
		ItemConfigData vip_reward_item;
	};

	CSALoginGiftCfg() : need_accumulate_days(0), cfg_count(0) {}

	int need_accumulate_days;
	ItemConfigData accumulate_reward;

	int cfg_count;
	ConfigItem cfg_list[COMBINE_SERVER_LOGIN_GIFT_REWARD_CONFIG_MAX_COUNT];
};

struct CSAServerPanicBuyCfg
{
	struct ConfigItem
	{
		ConfigItem() : seq(0), gold_price(0), server_limit_buy_count(0), personal_limit_buy_count(0) {}

		int seq;
		int gold_price;														
		int server_limit_buy_count;
		int personal_limit_buy_count;
		ItemConfigData reward_item;
	};

	CSAServerPanicBuyCfg() : cfg_count(0) {}

	int cfg_count;
	ConfigItem cfg_list[COMBINE_SERVER_SERVER_PANIC_BUY_ITEM_MAX_COUNT];
};


struct CSAPersonalPanicBuyCfg
{
	struct ConfigItem
	{
		ConfigItem() : seq(0), gold_price(0), limit_buy_count(0) {}

		int seq;
		int gold_price;
		int limit_buy_count;
		ItemConfigData reward_item;
	};

	CSAPersonalPanicBuyCfg() : cfg_count(0) {}

	int cfg_count;
	ConfigItem cfg_list[COMBINE_SERVER_PERSONAL_PANIC_BUY_ITEM_MAX_COUNT];
};

struct CSATouzijihuaCfg
{
	CSATouzijihuaCfg() : login_day(0), reward_gold(0)
	{}

	int login_day;
	int reward_gold;
};

struct CSAFoundationConfig
{
	CSAFoundationConfig() : seq(0), chongzhi_gold(0)
	{}

	struct CSAFoundationRewardCfg
	{
		CSAFoundationRewardCfg() : need_up_level(0), reward_gold(0)
		{}

		int need_up_level;
		int reward_gold;
	};

	int seq;
	int chongzhi_gold;

	std::vector<CSAFoundationRewardCfg> reward_cfg_vec;
};

// 经验炼制
struct CSAExpRefineRewardCfg
{
	CSAExpRefineRewardCfg() : level(0), cost(0){}

	int level;
	int cost;
};

// boss抢夺
struct CSAKillBossCfg
{
	int boss_id = 0;
	int delta_lv_limit = 0;
};


class CombineServerActivityConfig : public ConfigBase
{
public:
	CombineServerActivityConfig();
	~CombineServerActivityConfig();

	bool Init(const std::string &path, std::string *err);

	CSATimeConfig * GetCSATimeConfig(int activity_sub_type);

	static bool IsRankSubActivityType(int activity_sub_type);
	int GetRelatedRankType(int sub_type);
	int GetRelatedSubType(int rank_type);
	const CSARankRewardConfig * GetCSARankRewardCfgByRankType(int rank_type);
	const CSARankRewardConfig * GetCSARankRewardCfgBySubType(int sub_type);

	const CSAQianggouItemConfig * GetRankQianggouCfg(int item_idx);
	const CSARollConfig & GetRollCfg() { return m_roll_cfg; }

	const CSAOtherConfig & GetOtherCfg() { return m_other_cfg; }

	const CSASingleChargeCfg::ConfigItem * GetSingleChargeCfg(long long charge_value);

	const CSALoginGiftCfg & GetLoginGiftCfg() { return m_login_gift_cfg; }
	const CSALoginGiftCfg::ConfigItem * GetLoginGiftRewardCfg(int seq);

	const CSAServerPanicBuyCfg::ConfigItem * GetServerPanicBuyItemCfg(int item_idx);
	const CSAPersonalPanicBuyCfg::ConfigItem * GetPersonalPanicBuyItemCfg(int item_idx);

	int GetTouzijihuaCfgCount() { return static_cast<int>(m_touzijihua_reward_cfg_vec.size()); }
	const CSATouzijihuaCfg * GetTouzijihuaCfg(int phase);

	const CSAFoundationConfig * GetFoundationCfg(int chongzhi_num);
	int GetFoundationRewardGold(int reward_index, int reward_times, int delta_level);
	int GetFoundationCfgCount() { return static_cast<int>(m_foundation_cfg_vec.size()); }
	int GetFoundationRewardTimesCount(int reward_index);

	int GetExpRefineCost(int role_level, int to_level);

	const CSAKillBossCfg * GetKillBossCfg(int boss_id);

private:
	int InitSubActivityTimeCfg(PugiXmlNode RootElement);
	int InitRankRewardCfg(PugiXmlNode RootElement);
	int InitOtherCfg(PugiXmlNode RootElement);
	int InitRankQianggouCfg(PugiXmlNode RootElement);
	int InitRollCfg(PugiXmlNode RootElement);
	int InitSingleChargeCfg(PugiXmlNode RootElement);
	int InitLoginGiftRewardCfg(PugiXmlNode RootElement);
	int InitServerPanicBuyCfg(PugiXmlNode RootElement);
	int InitPersonalPanicBuyCfg(PugiXmlNode RootElement);
	int InitTouzijihuaCfg(PugiXmlNode RootElement);
	int InitFoundationCfg(PugiXmlNode RootElement);
	int InitExpRefineCfg(PugiXmlNode RootElement);
	int InitKillBossCfg(PugiXmlNode RootElement);

	CSATimeConfig m_time_config_list[CSA_SUB_TYPE_MAX];												// 活动时间配置
	CSARankRewardConfig m_rank_reward_list[CSA_SUB_TYPE_MAX];										// 排名奖励

	int m_sub_activity_rank_type_map[CSA_SUB_TYPE_MAX];
	int m_rank_type_sub_activity_map[CSA_RANK_TYPE_MAX];

	CSAQianggouItemConfig m_rank_qianggou_itemlist[COMBINE_SERVER_RANK_QIANGOU_ITEM_MAX_TYPE];		// 抢购配置

	CSARollConfig m_roll_cfg;																		// 摇奖配置
	CSAOtherConfig m_other_cfg;																		// 其他配置
	CSASingleChargeCfg m_single_charge_cfg;															// 单笔充值配置
	CSALoginGiftCfg m_login_gift_cfg;																// 登录奖励配置
	CSAServerPanicBuyCfg m_server_panic_buy_cfg;													// 全民疯抢
	CSAPersonalPanicBuyCfg m_personal_panic_buy_cfg;												// 个人疯抢

	std::vector<CSATouzijihuaCfg> m_touzijihua_reward_cfg_vec;										// 投资计划
	std::vector<CSAFoundationConfig> m_foundation_cfg_vec;											// 成长基金
	std::vector<CSAExpRefineRewardCfg> m_exp_refine_cfg_vec;                                        // 经验炼制

	std::map<int, CSAKillBossCfg> m_kill_boss_cfg;													// BOSS抢夺
};

#endif // __COMBINE_SERVER_ACTIVITY_CONFIG_HPP__

