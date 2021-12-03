#ifndef __FIGHTING_CONFIG_HPP__
#define __FIGHTING_CONFIG_HPP__

#include "servercommon/serverdef.h"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/crossdef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/fightingdef.hpp"
#include "servercommon/pugixml/pugixml_adapter.hpp"

struct FightingOtherConfig
{
	FightingOtherConfig() : dm_scene_id(0), dm_day_times(0), dm_buy_time_need_gold(0), dm_cost_time_m(0), dm_rob_times(0), dm_been_rob_times(0), dm_rob_reward_rate(0), sl_scene_id(0), sl_day_times(0), sl_buy_time_need_gold(0),
	                        sl_cost_time_m(0), sl_rob_times(0), sl_been_rob_times(0), sl_rob_reward_rate(0), sl_jinghua_reward_open_level(0), 
							cf_scene_id(0), cf_default_join_times(0), cf_buy_time_need_gold(0), cf_restore_join_times_need_time_m(0), cf_auto_reflush_interval_s(0), cf_reflush_need_bind_gold(0),
	                        cf_win_add_jifen(0), cf_win_add_mojing(0), cf_win_add_exp(0), cf_stop_level(0){}

	// 挖矿相关（Dig Mine）
	int dm_scene_id;									// 对战场景ID
	Posi dm_sponsor_pos;								// 挑战者坐标
	Posi dm_opponent_pos;								// 对手坐标
	int dm_day_times;									// 每天最大挖矿次数
	int dm_buy_time_need_gold;
	int dm_cost_time_m;
	int dm_rob_times;
	int dm_been_rob_times;
	int dm_rob_reward_rate;

	// 航海相关（Sailing）
	int sl_scene_id;									// 航海 - 对战场景ID
	Posi sl_sponsor_pos;
	Posi sl_opponent_pos;
	int sl_day_times;
	int sl_buy_time_need_gold;
	int sl_cost_time_m;
	int sl_rob_times;
	int sl_been_rob_times;
	int sl_rob_reward_rate;
	int sl_jinghua_reward_open_level;

	// 挑衅相关（Challenge Field）
	int cf_scene_id;
	Posi cf_sponsor_pos;
	Posi cf_opponent_pos;
	int cf_default_join_times;
	int cf_buy_time_need_gold;
	int cf_restore_join_times_need_time_m;
	int cf_auto_reflush_interval_s;
	int cf_reflush_need_bind_gold;						// 刷新对手需要消耗绑元
	int cf_win_add_jifen;
	int cf_win_add_mojing;
	long long cf_win_add_exp;
	int cf_stop_level;
	ItemConfigData cf_wint_item[MAX_ATTACHMENT_ITEM_NUM];
};

struct FightingMiningRewardCfg
{
	static const int MAX_QUALITY_TYPE = 10;						// 最大品质类型

	FightingMiningRewardCfg() : quality(0), consume_gold(0), upgrade_rate(0), reward_exp(0), reward_item_count(0), rob_get_item_count(0), robot_reflush_weight(0) {}

	int quality;
	int consume_gold;
	int upgrade_rate;
	long long reward_exp;

	int reward_item_count;
	ItemConfigData reward_item_list[MAX_ATTACHMENT_ITEM_NUM];

	int rob_get_item_count;										// 抢夺1次获取的物品数量
	int robot_reflush_weight;									// 机器人刷新到当前品质的权重
};

struct FightingSailingRewardCfg
{
	static const int MAX_QUALITY_TYPE = 10;						// 最大品质类型

	FightingSailingRewardCfg() : quality(0), consume_gold(0), upgrade_rate(0), reward_exp(0), shenge_jinghua(0), reward_item_count(0), rob_get_item_count(0), robot_reflush_weight(0) {}

	int quality;
	int consume_gold;
	int upgrade_rate;
	long long reward_exp;
	int shenge_jinghua;

	int reward_item_count;
	ItemConfigData reward_item_list[MAX_ATTACHMENT_ITEM_NUM];

	int rob_get_item_count;										// 抢夺1次获取的物品数量
	int robot_reflush_weight;									// 机器人刷新到当前品质的权重
};

struct FightingRankRewardCfg
{
	static const int CFG_COUNT = 20;							// 配置最大数量

	FightingRankRewardCfg() : rank(0), reward_item_count(0) {}

	int rank;

	int reward_item_count;
	ItemConfigData reward_item_list[MAX_ATTACHMENT_ITEM_NUM];
};

struct FightingSkipCfg
{
	FightingSkipCfg() : limit_level(0), consume(0)
	{}

	int limit_level;
	int consume;
};

struct FightingSkipTypeCfg
{
	FightingSkipTypeCfg() : sailing_max_quality(0), mining_max_quality(0), challenge_skip_limit_level(0), challenge_skip_consume(0),
		mining_rob_limit_level(0), mining_rob_consume(0), sailing_rob_limit_level(0), sailing_rob_consume(0)
	{}

	enum FIGHT_SKIP_TPYE
	{
		FIGHT_SKIP_TYPE_SAIL = 0,
		FIGHT_SKIP_TYPE_MINE,
		FIGHT_SKIP_TYPE_CHALLENGE,
		FIGHT_SKIP_TYPE_MINE_ROB,
		FIGHT_SKIP_TYPE_SAIL_ROB,
	};

	int sailing_max_quality;
	FightingSkipCfg sailing_reward_cfg_list[FightingSailingRewardCfg::MAX_QUALITY_TYPE + 1];

	int mining_max_quality;
	FightingSkipCfg mining_reward_cfg_list[FightingMiningRewardCfg::MAX_QUALITY_TYPE + 1];

	int challenge_skip_limit_level;
	int challenge_skip_consume;

	int mining_rob_limit_level;
	int mining_rob_consume;

	int sailing_rob_limit_level;
	int sailing_rob_consume;
};

class FightingConfig : public ConfigBase
{
public:
	FightingConfig();
	~FightingConfig();

	bool Init(std::string path, std::string *err);

	const FightingOtherConfig & GetOtherCfg() const { return m_other_cfg; }
	const FightingMiningRewardCfg * GetMiningCfg(int mining_quality) const;
	const FightingSailingRewardCfg * GetSailingCfg(int quality) const;
	const FightingSkipTypeCfg & GetSkipCfg() const { return m_skip_type_cfg; }
	int GetMineMaxQuality() const { return m_mining_max_quality; }
	int GetSailingMaxQuality() const { return m_sailing_max_quality; }
	const FightingRankRewardCfg * GetChallengeRankReward(int rank) const;
	int MiningGetRandomRobotQuality() const;
	int SailingGetRandomRobotQuality() const;

private:
	int InitOtherCfg(PugiXmlNode RootElement);
	int InitMiningRewardCfg(PugiXmlNode RootElement);
	int InitSailingRewardCfg(PugiXmlNode RootElement);
	int InitChallengeRankRewardCfg(PugiXmlNode RootElement);
	int InitFightingSkipCfg(PugiXmlNode RootElement);

	FightingOtherConfig m_other_cfg;

	int m_mining_max_quality;
	int m_mining_robot_total_weight;	// 机器人刷新的总权重
	FightingMiningRewardCfg m_mining_reward_cfg_list[FightingMiningRewardCfg::MAX_QUALITY_TYPE + 1];

	int m_sailing_max_quality;
	int m_sailing_robot_total_weight;	// 机器人刷新的总权重
	FightingSailingRewardCfg m_sailing_reward_cfg_list[FightingSailingRewardCfg::MAX_QUALITY_TYPE + 1];

	int m_challenge_rank_reward_cfg_count;
	FightingRankRewardCfg m_challenge_rank_reward_cfg_list[FightingRankRewardCfg::CFG_COUNT];

	FightingSkipTypeCfg m_skip_type_cfg;
};

#endif // __CHALLENGE_FIELD_CONFIG_HPP__
