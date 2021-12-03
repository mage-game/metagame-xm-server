#ifndef __CROSS_XIULUO_TOWER_COMMON_CONFIG_HPP__
#define __CROSS_XIULUO_TOWER_COMMON_CONFIG_HPP__

#include "servercommon/crossdef.hpp"
#include "servercommon/activitydef.hpp"

#include "servercommon/struct/itemlistparam.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

struct CrossXiuluoTowerTimeCfg
{
	CrossXiuluoTowerTimeCfg() : is_open(false), open_day_second(0), standby_dur_s(0), open_dur_s(0) 
	{
		memset(open_day_list, 0, sizeof(open_day_list));
	}

	bool is_open;
	bool open_day_list[CROSS_ACTIVITY_XIULUO_TOWER_MAX_WEEK_DAYS];
	int open_day_second;
	int standby_dur_s;
	int open_dur_s;
};

struct CrossXiuluoTowerLayerCfg
{
	CrossXiuluoTowerLayerCfg() : scene_id(0), up_layer_need_kills(0), realive_point_count(0) 
	{
		memset(realive_point_list, 0, sizeof(realive_point_list));
	}

	int scene_id;
	int up_layer_need_kills;

	int realive_point_count;
	Posi realive_point_list[CROSS_ACTIVITY_XIULUO_TOWER_REALIVE_POINT_MAX];
};

struct CrossXiuluoTowerLayerRewardCfg
{
	CrossXiuluoTowerLayerRewardCfg() : reward_layer(0), reward_cross_honor(0) {}

	int reward_layer;
	int reward_cross_honor;
	ItemConfigData reward_item;
};

struct CrossXiuluoTowerRankRewardCfg
{
	struct LevelRewardCfg
	{
		LevelRewardCfg() : min_level(0), max_level(0), reward_honor(0) {}

		int min_level;
		int max_level;
		int reward_honor;
		ItemConfigData reward_item;
	};

	int min_pos;
	int max_pos;
	int level_reward_count;
	LevelRewardCfg level_reward_list[CROSS_ACTIVITY_XIULUO_TOWER_RANK_REWARD_LEVEL_CFG_MAX];
};

struct CrossXiuluoTowerDurDeadBuff
{
	CrossXiuluoTowerDurDeadBuff() : dur_dead_count(0), add_max_hp_per(0), add_gongji_per(0) {}

	int dur_dead_count;
	int add_max_hp_per;
	int add_gongji_per;
};

struct CrossXiuluoTowerDurKillBuff
{
	CrossXiuluoTowerDurKillBuff() : dur_kill_count(0), add_max_hp_per(0), add_gongji_per(0) {}

	int dur_kill_count;
	int add_max_hp_per;
	int add_gongji_per;
};

struct CrossXiuluoTowerBossCfg
{
	CrossXiuluoTowerBossCfg() : monster_id(0), monster_num(0), pos(0, 0) {}

	int monster_id;
	int monster_num;
	Posi pos;
};

struct CrossXiuluoTowerFirstRewardCfg
{
	CrossXiuluoTowerFirstRewardCfg() : honour(0) {}

	int honour;
	ItemConfigData item;
};

struct CrossXiuluoTowerScoreRewardCfg
{
	CrossXiuluoTowerScoreRewardCfg() : score(0), reward_honor(0), item_count(0) {}

	int score;
	int reward_honor;
	int item_count;
	ItemConfigData item_list[MAX_ATTACHMENT_ITEM_NUM];
};

struct CrossXiuluoTowerGatherRewardCfg
{
	CrossXiuluoTowerGatherRewardCfg() : gather_id(0), is_golden(false) {}

	int gather_id;
	bool is_golden;
	ItemConfigData reward_item;
};

struct CrossXiuluoTowerRewardCfg
{
	CrossXiuluoTowerRewardCfg() {}

	ItemConfigData buff_item;
	CrossXiuluoTowerFirstRewardCfg first_reward[CROSS_ACTIVITY_XIULUO_TOWER_MAX_LAYER];
	CrossXiuluoTowerBossCfg boss_cfg;
	CrossXiuluoTowerScoreRewardCfg score_reward[CROSS_ACTIVITY_XIULUO_TOWER_MAX_SCORE_REWARD];
	std::map<int, CrossXiuluoTowerGatherRewardCfg> gather_reward_map;
};

struct CrossXiuluoTowerOtherCfg
{
	CrossXiuluoTowerOtherCfg() : gather_buff_gold(0), gather_buff_duration(0), title_drop_id(0), title_drop_rate(0), role_title_drop_daily_num_limit(0), server_title_drop_total_num_limit(0),
		mount_drop_id(0), mount_drop_rate(0), role_mount_drop_daily_num_limit(0), server_mount_drop_total_num_limit(0), gold_box_item_id(0), gold_box_item_rate(0),
		role_gold_box_item_daily_num_limit(0), server_gold_box_total_num_limit(0) {}

	int gather_buff_gold;				// 采集buff所需元宝
	int gather_buff_duration;			// 采集buff持续时长

	int title_drop_id;						// 称号掉落id
	int title_drop_rate;					// 称号掉落概率
	int role_title_drop_daily_num_limit;	// 玩家称号掉落每日获取上限
	int server_title_drop_total_num_limit;	// 全服称号掉落总数上限

	int mount_drop_id;						// 坐骑掉落id
	int mount_drop_rate;					// 坐骑掉落概率
	int role_mount_drop_daily_num_limit;	// 玩家坐骑掉落每日获取上限
	int server_mount_drop_total_num_limit;	// 全服坐骑掉落总数上限

	int gold_box_item_id;					// 金箱子物品id
	int gold_box_item_rate;					// 金箱子物品概率
	int role_gold_box_item_daily_num_limit;	// 玩家金箱子每日获得上限
	int server_gold_box_total_num_limit;	// 全服金箱子总数上限
};

// 掉落限制配置
struct CrossXiuluoTowerDropLimitConfig
{
	CrossXiuluoTowerDropLimitConfig() : drop_num_limit(0), weight(0) {}

	int drop_num_limit;
	int weight;
};

struct CrossXiuluoTowerGatherCfg
{
	CrossXiuluoTowerGatherCfg():gather_count(0){}

	struct GatherItem
	{
		GatherItem():gather_index(0),gather_id(0),gather_time(0),pos_x(0),pos_y(0),disappear_s(0){}
		int gather_index;
		int gather_id;
		int gather_time;
		int pos_x;
		int pos_y;
		int disappear_s;
	};
	int gather_count;
	GatherItem gather_list[CROSS_XIULUO_TOWER_GATER_ITEM_NUM_MAX];
};

class CrossXiuluoTowerCommonConfig
{
public:
	CrossXiuluoTowerCommonConfig();
	~CrossXiuluoTowerCommonConfig();

	static CrossXiuluoTowerCommonConfig & Instance();

	bool Init(const std::string &path, std::string *err);

	CrossXiuluoTowerTimeCfg * GetActivityTimeCfg() { return &m_activity_time_cfg; }
	int GetActivityStandbyTime() { return m_activity_time_cfg.standby_dur_s; }
	int GetActivityOpenTime() { return m_activity_time_cfg.open_dur_s; }

	int GetLayerSceneId(int layer, Posi *enter_pos = NULL);
	int GetSceneIdStandy(Posi *enter_pos);
	int GetUpLayerNeedKills(int layer);
	Posi GetRealivePos(int layer);
	ItemID GetKillBossRewardItemId() { return m_kill_boss_reward_item_id; }
	int GetKillRoleRewardHonor() { return m_kill_role_reward_honor; }
	int GetKillRoleScore() { return m_kill_role_score; }
	int GetKillMonsterScore() { return m_kill_monster_score; }
	int GetPassLayerScore() { return m_pass_layer_score; }
	int GetBossRefreshTime() { return m_boss_refresh_time; }
	int GetBossRefreshNoticeTime() { return m_boss_refresh_notice_time; }
	CrossXiuluoTowerRankRewardCfg::LevelRewardCfg * GetRankRewardItem(int rank_pos, int level);
	bool GetLayerReward(int layer, ItemConfigData *reward_item, int *reward_cross_honor);

	bool GetDurDeadBuff(int dur_dead_count, int *add_max_hp_per, int *add_gongji_per);
	bool GetDurKillBuff(int dur_kill_count, int *add_max_hp_per, int *add_gongji_per);

	const ItemConfigData * GetBuffReward();
	const CrossXiuluoTowerBossCfg * GetCrossXiuluoTowerBossCfg();
	const CrossXiuluoTowerFirstRewardCfg * GetCrossXiuluoTowerFirstRewardCfg(int layer);
	const CrossXiuluoTowerScoreRewardCfg * GetCrossXiuluoTowerScoreRewardCfg(int index);
	const CrossXiuluoTowerGatherRewardCfg * GetCrossXiuluoTowerGatherRewardCfg(int gather_id);

	int GetDropWeightByLayerID(int layer_id);

	const CrossXiuluoTowerOtherCfg & GetOtherCfg() { return m_other_cfg; }
	const int GetCrossXiuluoServerTitleDropNumLimit();
	const int GetCrossXiuluoServerMountDropNumLimit();
	const int GetCrossXiuluoServerGoldBoxDropNumLimit();

	const int GetXiuluoTowerRankTitleId(int rank);
	const CrossXiuluoTowerGatherCfg & GetGatherCfg() { return m_xiuluo_tower_gather_cfg; }

private:
	
	int InitOtherCfg(PugiXmlNode RootElement);
	int InitServerTitleDropLimitCfg(PugiXmlNode RootElement);
	int InitServerMountDropLimitCfg(PugiXmlNode RootElement);
	int InitServerGoldBoxDropLimitCfg(PugiXmlNode RootElement);
	int InitXiuluoTowerRankTitle(PugiXmlNode RootElement);
	int InitXiuluoTowerGatherCfg(PugiXmlNode RootElement);

	CrossXiuluoTowerTimeCfg  m_activity_time_cfg;
	CrossXiuluoTowerLayerCfg m_layer_list[CROSS_ACTIVITY_XIULUO_TOWER_MAX_LAYER];

	int m_layer_reward_cfg_count;
	CrossXiuluoTowerLayerRewardCfg m_layer_reward_cfg_list[CROSS_ACTIVITY_XIULUO_TOWER_REWARD_CFG_MAX];

	int m_rank_reward_cfg_count;
	CrossXiuluoTowerRankRewardCfg m_rank_reward_cfg_list[CROSS_ACTIVITY_XIULUO_TOWER_RANK_REWARD_CFG_MAX];

	ItemID m_kill_boss_reward_item_id;
	int m_kill_role_reward_honor;
	int m_kill_role_score;
	int m_kill_monster_score;
	int m_pass_layer_score;
	int m_boss_refresh_time;
	int m_boss_refresh_notice_time;

	int m_dur_dead_buff_count;
	CrossXiuluoTowerDurDeadBuff m_dur_dead_buff_list[CROSS_ACTIVITY_XIULUO_TOWER_DUR_DEAD_BUFF_MAX];

	int m_dur_kill_buff_count;
	CrossXiuluoTowerDurKillBuff m_dur_kill_buff_list[CROSS_ACTIVITY_XIULUO_TOWER_DUR_KILL_BUFF_MAX];

	CrossXiuluoTowerRewardCfg m_reward_cfg;

	int m_drop_weight_list[CROSS_ACTIVITY_XIULUO_TOWER_MAX_LAYER];

	CrossXiuluoTowerOtherCfg m_other_cfg;

	int m_server_title_drop_total_weight;
	CrossXiuluoTowerDropLimitConfig m_server_title_drop_limit_cfg_list[CROSS_ACTIVITY_XIULUO_TOWER_DROP_LIMIT_CFG_COUNT_MAX];

	int m_server_mount_drop_total_weight;
	CrossXiuluoTowerDropLimitConfig m_server_mount_drop_limit_cfg_list[CROSS_ACTIVITY_XIULUO_TOWER_DROP_LIMIT_CFG_COUNT_MAX];

	int m_server_glodbox_drop_total_weight;
	CrossXiuluoTowerDropLimitConfig m_server_goldbox_drop_limit_cfg_list[CROSS_ACTIVITY_XIULUO_TOWER_DROP_LIMIT_CFG_COUNT_MAX];

	int m_xiuluotower_rank_title[CROSS_ACTIVITY_XIULUO_TOWER_RANK_TITLE_NUM];

	CrossXiuluoTowerGatherCfg  m_xiuluo_tower_gather_cfg;
};

#endif // __CROSS_XIULUO_TOWER_COMMON_CONFIG_HPP__

