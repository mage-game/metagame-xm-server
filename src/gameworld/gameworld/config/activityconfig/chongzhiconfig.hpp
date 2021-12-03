#ifndef _CHONGZHICONFIG_HPP_
#define _CHONGZHICONFIG_HPP_

#include "gamedef.h"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include <vector>
#include "servercommon/activitydef.hpp"

static const int CHONGZHI_REWARD_CONFIG_MAX_NUM = 32;

static const int DIFF_WD_CHONGZHI_MAX_DAY_Of_WEEK = 7;						// 每日累充 -- 星期
static const int DIFF_WD_CHONGZHI_MAX_STAGE = 5;							// 每日累充 -- 阶段奖励	

enum CHONGZHI_EXTRA_REWARD_TYPE
{
	CHONGZHI_EXTRA_REWARD_TYPE_INVALID = 0,
	CHONGZHI_EXTRA_REWARD_TYPE_GOLD_BIND = 1,
	CHONGZHI_EXTRA_REWARD_TYPE_MONTH_CARD = 2,
	CHONGZHI_EXTRA_REWARD_TYPE_COIN_BIND = 3,
	CHONGZHI_EXTRA_REWARD_TYPE_GOLD = 4,

	CHONGZHI_EXTRA_REWARD_TYPE_MAX,
};


struct ChongzhiReward
{
	ChongzhiReward() : seq(0), chongzhi(0), reward_type(CHONGZHI_EXTRA_REWARD_TYPE_INVALID), extra_bind_gold(0), extra_bind_coin(0), extra_gold(0), openserver_extra_gold_bind(0){}

	int seq;
	int chongzhi;
	int reward_type;
	int extra_bind_gold;
	int extra_bind_coin;
	int extra_gold;
	int openserver_extra_gold_bind;
};

struct ChongzhiOtherConfig
{
	ChongzhiOtherConfig() : 
		first_recharge_extra_bind_gold_times(0), 
		daily_first_recharge_extra_reward_need_times(0),
		zai_chongzhi_need_chongzhi(0), third_chongzhi_need_chongzhi(0),
		chongzhi_seven_day_need_gold(0), chongzhi_seven_day_reward_day(0), chongzhi_seven_day_reward_bind_gold(0),
		chongzhi_extra_reward_limit_day(0)
	{

	}

	int first_recharge_extra_bind_gold_times;
	int daily_first_recharge_extra_reward_need_times;

	int zai_chongzhi_need_chongzhi;					// 再充值，需要的充值额度
	ItemConfigData zai_chongzhi_reward_item;		// 再充值，奖励物品ID
	ItemConfigData daily_first_recharge_extra_reward_item;

	int third_chongzhi_need_chongzhi;
	ItemConfigData third_chongzhi_reward_item;

	int chongzhi_seven_day_need_gold;
	int chongzhi_seven_day_reward_day;
	int chongzhi_seven_day_reward_bind_gold;
	int chongzhi_extra_reward_limit_day;
};

struct ChongzhiSpecialFirstRechargeConfig 
{
	const static int CONFIG_ITEM_MAX_COUNT = 8;

	struct ConfigItem 
	{
		ConfigItem() : seq(0) {}

		int seq;
		ItemConfigData reward_item;
	};

	ChongzhiSpecialFirstRechargeConfig() : cfg_count(0) {}

	int cfg_count;
	ConfigItem cfg_list[CONFIG_ITEM_MAX_COUNT];
};

struct ChongzhiDailyFirstRewardConfig
{
	const static int CONFIG_ITEM_MAX_COUNT = 7;

	UNSTD_STATIC_CHECK(CONFIG_ITEM_MAX_COUNT == 7); // 一个星期有七天


	struct ConfigItem
	{
		ConfigItem() : wday(0)
		{
		}
		
		int wday;
		ItemConfigData first_recharge_reward_item;
	};

	ChongzhiDailyFirstRewardConfig() {}

	ConfigItem cfg_list[CONFIG_ITEM_MAX_COUNT];
};

struct ChongzhiDailyTotalStageConfig
{
	const static int CONFIG_ITEM_MAX_COUNT = 8;
	const static int REWARD_ITEM_MAX_COUNT = 8;

	struct ConfigItem
	{
		ConfigItem() : stage(0), start_day(0), end_day(0), total_recharge_reward_item_count(0) 
		{
			memset(total_recharge_reward_need_chongzhi, 0, sizeof(total_recharge_reward_need_chongzhi));
		}

		int stage;
		int start_day;
		int end_day;

		int total_recharge_reward_item_count;
		ItemConfigData total_recharge_reward_item_list[REWARD_ITEM_MAX_COUNT];
		int total_recharge_reward_need_chongzhi[REWARD_ITEM_MAX_COUNT];
	};

	ChongzhiDailyTotalStageConfig() : cfg_count(0)
	{

	}

	int cfg_count;
	ConfigItem cfg_list[CONFIG_ITEM_MAX_COUNT];
};

// 我要变元宝累积充值活动配置
struct TotalChongzhiWantMoneyConfig
{
	const static int CONFIG_ITEM_MAX_COUNT = 16;
	UNSTD_STATIC_CHECK(CONFIG_ITEM_MAX_COUNT < 128);

	struct ConfigItem 
	{
		ConfigItem() : state(0), need_chongzhi(0), random_min(0), random_max(0) {}

		int state;
		long long need_chongzhi;
		int random_min;
		int random_max;
	};

	TotalChongzhiWantMoneyConfig() : m_max_state(0){}

	int m_max_state;
	ConfigItem cfg_list[CONFIG_ITEM_MAX_COUNT];
};

struct DiffWeekdayChongzhiConfig
{
	DiffWeekdayChongzhiConfig () : stage_cfg_count(0) {}

	struct RewardItemCfg
	{
		RewardItemCfg() : need_chongzhi(0) {}

		int need_chongzhi;
		ItemConfigData reward_item;
	};

	int stage_cfg_count;

	RewardItemCfg stage_item_list[DIFF_WD_CHONGZHI_MAX_STAGE];
};

static const int MAX_FIRST_CHONGZHI_REWARD_COUNT = 10;
static const int MAX_DAILY_CHONGZHI_REWARD_COUNT = 10;
static const int MAX_DAILY_CHONGZHI_LEVEL_SEQ = 20;
static const int MAX_DAILY_CHONGZHI_SELECT_COUNT = 3;
struct DailyChongzhiRewradConfig
{
	DailyChongzhiRewradConfig() : level_seq(0), min_level(0), max_level(0), seq(0), need_total_chongzhi(0) {}

	int level_seq;
	int min_level;
	int max_level;
	int seq;
	int need_total_chongzhi;
	ItemConfigData reward_item;
	ItemConfigData select_reward_item_list[MAX_DAILY_CHONGZHI_SELECT_COUNT];
};

struct FirstChongzhiRewradConfig
{
	FirstChongzhiRewradConfig() : seq(0), need_total_chongzhi(0), active_attr_time_limit(0) {}

	int seq;
	int need_total_chongzhi;
	ItemConfigData first_reward_item;
	time_t active_attr_time_limit;
	std::vector<int> attribute_type;
	std::vector<Attribute> attribute_value;
};

static const int MAX_DAILY_CHONGZHI_TIMES_REWARD_COUNT = 10;
struct DailyChongzhiTimesRewradConfig
{
	DailyChongzhiTimesRewradConfig() : seq(0), complete_days(0),opengameday(0) {}

	int seq;
	int complete_days;
	int opengameday;
	ItemConfigData reward_item;
};

struct OpenGameDayCfg
{
	OpenGameDayCfg() : opengameday(0) {}

	int opengameday;
	DailyChongzhiTimesRewradConfig m_daily_chongzhi_times_reward_cfg_list[MAX_DAILY_CHONGZHI_TIMES_REWARD_COUNT];
};

//static const int DAILY_CHONGZHI_REWRAD2_REQ_MAX = 10;
//static const int DAILY_CHONGZHI_REWRAD2_COUNT_MAX = 60;
static const int DAILY_CHONGZHI_REWRAD2_ITEM_COUNT_MAX = 6;
struct DailyChongzhiRewrad2Config
{
	DailyChongzhiRewrad2Config() : seq(-1), open_day(0), max_level(0), need_chongzhi(0), reward_item_count(0) {}

	int seq;
	int open_day;
	int max_level;
	int need_chongzhi;
	int reward_item_count;
	ItemConfigData reward_item_list[DAILY_CHONGZHI_REWRAD2_ITEM_COUNT_MAX];
};
typedef std::map<int, std::map<int, DailyChongzhiRewrad2Config> > DailyReward2ConfigMapInMap;

static const int CHONGZHI_DAY_CFG_MAX_COUNT = DAY_CHONGZHI_REWARD_FLAG_LIST_LEN * 32;
static const int CHONGZHI_DAY_REWARD_ITEM_MAX_COUNT = 3;
struct ChongzhiDayReward
{
	int need_chongzhi_day = 0;
	int need_gold = 0;
	int reward_bind_gold = 0;
	int reward_gold = 0;

	int item_count = 0;
	ItemConfigData reward_item_list[CHONGZHI_DAY_REWARD_ITEM_MAX_COUNT];

	int rare_item_count = 0;
	ItemConfigData rare_reward_item_list[CHONGZHI_DAY_REWARD_ITEM_MAX_COUNT];
};

class ChongzhiConfig : public ConfigBase
{
public:
	ChongzhiConfig();
	~ChongzhiConfig();

	bool Init(const std::string &configname, std::string *err);

	int GetExtraRewardCfgCount() { return m_reward_count; }
	const ChongzhiReward * GetExtraRewardByNum(int chongzhi);
	const ChongzhiReward * GetExtraRewardBySeq(int seq);
	const ChongzhiDailyFirstRewardConfig::ConfigItem * GetDailyFirstReward(int wday_idx);
	const ChongzhiSpecialFirstRechargeConfig::ConfigItem * GetSpecialFirstRechargeReward(int seq);
	const ChongzhiOtherConfig & GetOtherCfg() { return m_other_cfg; }
	int GetSpecialFirstRechargeRewardCount() { return m_special_first_recharge_cfg.cfg_count; }
	const ChongzhiDailyTotalStageConfig::ConfigItem * GetDailyTotalStageByDay(int open_day);
	const ChongzhiDailyTotalStageConfig::ConfigItem * GetDailyTotalStageByStage(int stage);
	int GetDailyChongzhiMaxStage() { return m_daily_total_stage_cfg.cfg_count; }
	bool IsVaildDailyTotalChongzhiStage(int stage);
	bool IsValidDailyTotalChongzhiDay(int open_day);
	int GetDailyTotalChongzhiMinStartDay();
	const TotalChongzhiWantMoneyConfig::ConfigItem * GetWantMoneyCfg(int state);
	const DiffWeekdayChongzhiConfig::RewardItemCfg * GetDiffWeekdayChongzhiCfg(int weekday, int stage);
	const DiffWeekdayChongzhiConfig * GetDiffWeekdayChongzhiCfgList(int weekday);

	int GetDailyChongzhiFirstMaxSeq() { return 0; }
	int GetFirstChongzhiFirstMaxCount() { return m_first_chongzhi_max_count; }
	int GetDailyChongzhiMinChongzhi() { return m_daily_chongzhi_min_chongzhi; }
	const DailyChongzhiRewradConfig * GetDailyChongzhiRewradConfig(int level, int seq);
	const FirstChongzhiRewradConfig * GetFirstChongzhiRewradConfig(int seq);
	int GetDailyChongzhiTimesMaxDay() { return m_daily_chongzhi_times_max_day; }
	const DailyChongzhiTimesRewradConfig * GetDailyChongzhiTimesRewradConfig(int seq);
	const DailyChongzhiRewrad2Config * GetDailyChongzhiRewrad2Config(int level, int seq);
	const DailyChongzhiRewrad2Config * GetDailyChongzhiRewrad2ConfigByLastChongzhiTime(int level, int seq, int last_chongzhi_time);
	const DailyReward2ConfigMapInMap & GetDailyChongzhiRewrad2ConfigMap();

	int GetChongzhiDayRewardCfgCount() const { return m_chongzhi_day_cfg_list_count; }
	const ChongzhiDayReward * GetDayChongzhiRewardCfg(int seq);
	const ChongzhiDayReward * GetDayChongzhiRewardCfgByChongzhiDay(int day_count);
private:
	int InitChongzhiCfg(PugiXmlNode RootElement);
	int InitOtherCfg(PugiXmlNode RootElement);
	int InitSpecialFirstRechargeCfg(PugiXmlNode RootElement);
	int InitDailyFirstRechargeRewardCfg(PugiXmlNode RootElement);
	int InitDailyTotalRechargeRewardCfg(PugiXmlNode RootElement);
	int InitDailyTotalStageCfg(PugiXmlNode RootElement);
	int InitChargeWantMoneyCfg(PugiXmlNode RootElement);
	int InitDiffWeekdayChongzhiCfg(PugiXmlNode RootElement);
	int InitFirstChongzhiRewradCfg(PugiXmlNode RootElement);
	int InitDailyChongzhiRewradCfg(PugiXmlNode RootElement);
	int InitDailyChongzhiTimesRewradCfg(PugiXmlNode RootElement);
	int InitDailyChongzhiRewrad2Cfg(PugiXmlNode RootElement);
	int InitDayChongzhiRewardCfg(PugiXmlNode RootElement);

	int m_reward_count;
	ChongzhiReward m_reward_list[CHONGZHI_REWARD_CONFIG_MAX_NUM];
	ChongzhiOtherConfig m_other_cfg;
	ChongzhiSpecialFirstRechargeConfig m_special_first_recharge_cfg;
	ChongzhiDailyFirstRewardConfig m_daily_reward_cfg;
	ChongzhiDailyTotalStageConfig m_daily_total_stage_cfg;
	TotalChongzhiWantMoneyConfig m_total_chongzhi_want_money_cfg;
	DiffWeekdayChongzhiConfig m_daily_diffwd_chongzhi_cfg[DIFF_WD_CHONGZHI_MAX_DAY_Of_WEEK];

	int m_daily_chongzhi_max_level_seq;
	int m_daily_chongzhi_min_chongzhi;
	DailyChongzhiRewradConfig m_daily_chongzhi_reward_cfg_list[MAX_DAILY_CHONGZHI_LEVEL_SEQ][MAX_DAILY_CHONGZHI_REWARD_COUNT];

	int m_first_chongzhi_max_count;
	FirstChongzhiRewradConfig m_first_chongzhi_reward_cfg[MAX_FIRST_CHONGZHI_REWARD_COUNT];

	int m_daily_chongzhi_times_max_day;
	//DailyChongzhiTimesRewradConfig m_daily_chongzhi_times_reward_cfg_list[MAX_DAILY_CHONGZHI_TIMES_REWARD_COUNT];
	std::map<int, OpenGameDayCfg> m_daily_chongzhi_reward_cfg_map;

	//DailyChongzhiRewrad2Config m_daily_chongzhi_reward2_cfg_list[DAILY_CHONGZHI_REWRAD2_REQ_MAX][DAILY_CHONGZHI_REWRAD2_COUNT_MAX];	//改成了map,让策划自由配置
	DailyReward2ConfigMapInMap m_daily_chongzhi_reward2_cfg_map;

	int m_chongzhi_day_cfg_list_count;
	ChongzhiDayReward m_chongzhi_day_reward_cfg_list[CHONGZHI_DAY_CFG_MAX_COUNT];
};

#endif

