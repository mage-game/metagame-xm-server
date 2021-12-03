#ifndef __CROSS_FISHING_CONFIG_HPP__
#define __CROSS_FISHING_CONFIG_HPP__

#include "servercommon/crossdef.hpp"
#include "servercommon/servercommon.h"
#include "servercommon/activitydef.hpp"

#include "servercommon/struct/itemlistparam.h"
#include "servercommon/serverdef.h"

static const int CROSS_FISHING_SCORE_REWARD_CFG_MAX_NUM = 10;							// 钓鱼积分奖励配置最大数

struct CrossFishingOtherConfig
{
	CrossFishingOtherConfig() : is_open(0), open_level(0), sceneid(0), event_min_cd_s(0), event_max_cd_s(0), pull_count_down_s(0), reduce_cd_percent_ingroup_2(0), reduce_cd_percent_ingroup_3(0),
		get_bigfish_rate_1(0), get_bigfish_rate_2(0), get_bigfish_rate_3(0), give_bait_count(0), max_robbed_count(0), net_get_fish_max_count(0), spear_get_fish_max_count(0),
		oil_reduce_event_cd_s(0), special_status_oil_duration_s(0), auto_fishing_need_gold(0), steal_count(0), be_stealed_count(0), steal_succ_rate(0), player_fishing_range(0),
		fishing_gather_id(0), fishing_gather_times(0), rank_title_id(0), rank_title_item_id(0){}

	bool is_open;
	int open_level;
	int sceneid;
	Posi enter_pos;
	int event_min_cd_s;
	int event_max_cd_s;
	int pull_count_down_s;
	int reduce_cd_percent_ingroup_2;
	int reduce_cd_percent_ingroup_3;
	int get_bigfish_rate_1;
	int get_bigfish_rate_2;
	int get_bigfish_rate_3;
	int give_bait_count;
	int max_robbed_count;
	int net_get_fish_max_count;
	int spear_get_fish_max_count;
	int oil_reduce_event_cd_s;
	int special_status_oil_duration_s;
	int auto_fishing_need_gold;
	int steal_count;
	int be_stealed_count;
	int steal_succ_rate;
	int player_fishing_range;
	int fishing_gather_id;
	int fishing_gather_times;
	int rank_title_id;
	int rank_title_item_id;
};

struct CrossFishingActivityOpenTimeInfo
{
	CrossFishingActivityOpenTimeInfo() : activity_week_day(0), activity_ready_time(0), activity_start_time(0), activity_end_time(0), ready_dur_s(0), open_dur_s(0) {}

	int activity_week_day;													// 活动所在周几
	int activity_ready_time;												// 活动等待时间点
	int activity_start_time;												// 活动开始时间点
	int activity_end_time;													// 活动结束时间点

	int ready_dur_s;														// 活动等待时间
	int open_dur_s;															// 开启持续时间
};


struct CrossFishingFishConfig
{
	CrossFishingFishConfig() : type(0), score(0), be_stealed_rate(0), is_rare(0),steal_score(0) {}

	int type;
	int score;
	int be_stealed_rate;
	int is_rare;
	int steal_score;
};

// 获取积分最少需要钓鱼数目的配置
struct CrossFishingFishLeastCountConfig
{
	CrossFishingFishLeastCountConfig() : reward_score(0) { memset(least_count_list, 0, sizeof(least_count_list)); }

	int least_count_list[FISHING_FISH_TYPE_MAX_COUNT];
	ItemConfigData reward_item;
	int reward_score;
};

struct CrossFishingFishBaitConfig
{
	CrossFishingFishBaitConfig() : type(0), item_id(0), gold_price(0) { memset(fish_rate_list, 0, sizeof(fish_rate_list)); }

	int type;
	ItemID item_id;
	int gold_price;
	int fish_rate_list[FISHING_FISH_TYPE_MAX_COUNT];
};

struct CrossFishingEventConfig
{
	enum EVENT_TYPE
	{
		EVENT_TYPE_GET_FISH = 0,						// 鱼类上钩
		EVENT_TYPE_TREASURE,							// 破旧宝箱
		EVENT_TYPE_YUWANG,								// 渔网
		EVENT_TYPE_YUCHA,								// 渔叉
		EVENT_TYPE_OIL,									// 香油
		EVENT_TYPE_ROBBER,								// 盗贼
		EVENT_TYPE_BIGFISH,								// 传说中的大鱼

		EVENT_TYPE_COUNT,

		EVENT_TYPE_NOTICE,								// 鱼被盯上提示（特殊事件类型，无需配置，放最后）
	};

	CrossFishingEventConfig() : type(0), trigger_weight(0), low_trigger_weight(0){}

	int type;
	int trigger_weight;
	int low_trigger_weight;
};

struct CrossFishingTreasureConfig
{
	CrossFishingTreasureConfig() : seq(0), weight(0) {}

	int seq;
	ItemConfigData reward_item;
	int weight;
};

struct CrossStealCountBuyCfg
{
	CrossStealCountBuyCfg() : buy_count(0), need_gold(0) {}

	int buy_count;
	int need_gold;
};

struct CrossFishingScoreRewardCfg
{
	CrossFishingScoreRewardCfg() : stage(0), need_score(0), reward_score(0), exp_reward(0), item_count(0), exp_factor_stage(0){}

	int stage;
	int need_score;
	int reward_score;
	int exp_reward;
	int item_count;
	double exp_factor_stage;
	ItemConfigData item_list[MAX_ATTACHMENT_ITEM_NUM];
};

class CrossFishingConfig
{
public:
	CrossFishingConfig();
	~CrossFishingConfig();

	static CrossFishingConfig & Instance();
	bool Init(const std::string &path, std::string *err);

	const CrossFishingOtherConfig & GetOtherCfg() { return m_other_cfg; }
	CrossFishingActivityOpenTimeInfo *GetActivityOpenTimeInfo(int index);
	CrossFishingActivityOpenTimeInfo *GetActivityOpenTimeInfoByWeek(int week);
	int GetOpenActivityConfigList(CrossFishingActivityOpenTimeInfo cfg_list[CROSS_PASTURE_OPEN_TIME_MAX_COUNT]);
	const CrossFishingFishConfig * GetFishCfg(int type);
	const CrossFishingFishBaitConfig * GetFishBaitCfg(int type);
	const CrossFishingEventConfig * GetEventCfg(int type);
	const CrossFishingFishLeastCountConfig * GetLeastCountCfg(int index);
	int GetRandomEventType(bool is_in_team, bool is_perfect);
	int GetRandomLeastCountCfgIndex();
	int GetRandomFishType(int bait_type);
	const CrossFishingTreasureConfig * GetRandomTreasureReward();
	int GetStealBuyCountGold(int now_buycount);
	const int GetMaxBuyStealTimes() { return m_steal_buy_count; }
	const int GetFishingScoreCfgCount() { return m_fishing_score_reward_cfg_count; }
	const CrossFishingScoreRewardCfg* GetFishingScoreReward(const int stage) const;
	unsigned int GetBroadCastInterval(int index);
	int GetBroadCastTimes() { return static_cast<unsigned int>(m_broadcast_interval_vec.size()); }

private:

	int InitOtherCfg(TiXmlElement *RootElement);
	int InitFishCfg(TiXmlElement *RootElement);
	int InitCombinationCfg(TiXmlElement* RootElement);
	int InitFishBaitCfg(TiXmlElement *RootElement);
	int InitEventCfg(TiXmlElement *RootElement);
	int InitTreasureCfg(TiXmlElement *RootElement);
	int InitStealCountBuyCfg(TiXmlElement *RootElement);
	int InitScoreRewardCfg(TiXmlElement* RootElement);
	int InitActivityOpenTimeCfg(TiXmlElement *RootElement);
	int InitBroadcastCfg(TiXmlElement *RootElement);

	bool DayTimeFormatInvalid(int day_time);

	CrossFishingOtherConfig m_other_cfg;

	int m_activity_open_time_cfg_list_count;								// 活动开启时间配置数量
	CrossFishingActivityOpenTimeInfo m_activity_open_time_cfg_list[CROSS_ACTIVITY_OPEN_TIME_CFG_MAX_COUNT];	// 活动开启时间

	int m_fish_cfg_count;
	CrossFishingFishConfig m_fish_cfg_list[FISHING_FISH_TYPE_MAX_COUNT];

	int m_fish_combination_cfg_count;
	CrossFishingFishLeastCountConfig m_fish_least_count_cfg_list[FISHING_FISH_LEAST_COUNT_CFG_MAX_COUNT + 1];

	int m_fish_bait_cfg_count;
	CrossFishingFishBaitConfig m_fish_bait_cfg_list[FISHING_FISH_BAIT_CFG_MAX_COUNT];

	CrossFishingEventConfig m_event_cfg_list[CrossFishingEventConfig::EVENT_TYPE_COUNT];

	int m_treasure_cfg_count;
	CrossFishingTreasureConfig m_treasure_cfg_list[FISHING_TREASURE_CFG_MAX_COUNT];

	int m_steal_buy_count;
	CrossStealCountBuyCfg m_steal_buy_cfg_list[FISHING_STEAL_COUNT_BUY_MAX];

	int m_fishing_score_reward_cfg_count;
	CrossFishingScoreRewardCfg m_fishing_score_reward_cfg_list[CROSS_FISHING_SCORE_REWARD_CFG_MAX_NUM];

	std::vector<unsigned int> m_broadcast_interval_vec;
};

#endif // __CROSS_FISHING_CONFIG_HPP__

