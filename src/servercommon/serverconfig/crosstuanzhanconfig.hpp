#ifndef __CROSS_TUANZHAN_CONFIG_HPP__
#define __CROSS_TUANZHAN_CONFIG_HPP__

#include "servercommon/crossdef.hpp"
#include "servercommon/activitydef.hpp"

#include "servercommon/struct/itemlistparam.h"
#include "servercommon/serverdef.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

struct CrossTuanzhanOtherCfg
{
	CrossTuanzhanOtherCfg() : is_open(false), scene_id(0), scene_standby_dur_s(0), kill_score(0), assist_score(0), scene_add_score_time(0), 
		static_score(0), pillar_time(0), pillar_player_score(0), pillar_teammate_score(0), weak_buff_need_been_kill(0), weak_buff_hp_percent(0), 
		winner_cross_honor(0), winner_rank_num(0), join_honor_factor_percent(0), join_honor_max(0), join_honor_min(0), rand_user_side_time(0){}

	bool is_open;															// 是否开放本功能-总开关

	int scene_id;															// 团战场景ID

	int scene_standby_dur_s;												// 场景准备时间（秒）
	
	int kill_score;															// 击杀玩家获得积分
	int assist_score;														// 助攻获得积分

	int scene_add_score_time;												// 战场获得积分间隔（秒）
	int static_score;														// 战场定时获得积分
	int pillar_time;														// 柱子获得积分间隔（秒）
	int pillar_player_score;												// 柱子定时加个人积分
	int pillar_teammate_score;												// 柱子定时加队友积分

	int weak_buff_need_been_kill;											// 获得弱势buff所需连续被杀数
	int weak_buff_hp_percent;												// 弱势气血加成百分比

	int winner_cross_honor;													// 胜利奖励荣誉
	int winner_rank_num;													// 胜利阵营前几名
	
	int winner_rank_reward_item_list_count;
	ItemConfigData winner_rank_reward_item_list[CROSS_TUANZHAN_WINNER_REWARD_ITEM_MAX_COUNT];// 胜利阵营前几名奖励物品

	int join_honor_factor_percent;											// 参与荣誉系数百分比
	int join_honor_max;														// 参与荣誉上限
	int join_honor_min;														// 参与荣誉下限
	int rand_user_side_time;												// 随机阵营的间隔时间
};

struct CrossTuanzhanPillaCfg
{
	CrossTuanzhanPillaCfg() : pilla_count(0) {}

	struct CrossTuanzhanPillaConfigItem
	{
		CrossTuanzhanPillaConfigItem() : index(0), pilla_id(0), pilla_side_0(0), pilla_side_1(0) {}

		int index;
		int pilla_id;															// 柱子怪物id
		int pilla_side_0;														// 柱子怪物id 0方
		int pilla_side_1;														// 柱子怪物id 1方
		Posi pilla_pos;															// 柱子出生点
	};

	int pilla_count;														// 柱子数量
	CrossTuanzhanPillaConfigItem pilla_list[CROSS_TUANZHAN_PILLA_MAX_COUNT];
};

struct ActivityOpenTimeInfo
{
	ActivityOpenTimeInfo() : activity_week_day(0), activity_ready_time(0), activity_start_time(0), activity_end_time(0), ready_dur_s(0), open_dur_s(0) {}

	int activity_week_day;													// 活动所在周几
	int activity_ready_time;												// 活动等待时间点
	int activity_start_time;												// 活动开始时间点
	int activity_end_time;													// 活动结束时间点

	int ready_dur_s;														// 活动等待时间
	int open_dur_s;															// 开启持续时间
};


struct CrossTuanzhanScoreRewardCfg
{
	CrossTuanzhanScoreRewardCfg() : seq(0), need_score(0), reward_currency(0), cross_honor(0), reward_item_count(0) {}

	int seq;
	int need_score;
	int reward_currency;
	int cross_honor;
	int reward_item_count;
	ItemConfigData reward_item_list[MAX_ATTACHMENT_ITEM_NUM];
};

struct CrossTuanzhanScoreRewardLevelCfg
{
	CrossTuanzhanScoreRewardLevelCfg() : level(0), score_reward_cfg_list_count(0) {}

	int level;
	int score_reward_cfg_list_count;
	CrossTuanzhanScoreRewardCfg score_reward_cfg_list[CROSS_TUANZHAN_SCORE_REWARD_CFG_MAX];
};

struct CrossTuanzhanRankRewardCfg
{
	CrossTuanzhanRankRewardCfg() : rank(0), reward_currency(0), cross_honor(0), reward_item_count(0) {}

	int rank;
	int reward_currency;
	int cross_honor;
	int reward_item_count;
	ItemConfigData reward_item_list[MAX_ATTACHMENT_ITEM_NUM];
};

struct CrossTuanzhanRankRewardLevelCfg
{
	CrossTuanzhanRankRewardLevelCfg() : level(0), rank_reward_cfg_list_count(0) {}

	int level;
	int rank_reward_cfg_list_count;
	CrossTuanzhanRankRewardCfg rank_reward_cfg_list[CROSS_TUANZHAN_RANK_REWARD_CFG_MAX];
};

class CrossTuanzhanConfig
{
public:
	CrossTuanzhanConfig();
	~CrossTuanzhanConfig();

	static CrossTuanzhanConfig & Instance();
	bool Init(const std::string &path, std::string *err);

	const CrossTuanzhanOtherCfg & GetOtherCfg() { return m_other_cfg; }
	const CrossTuanzhanPillaCfg & GetPillaCfg() { return m_pilla_cfg; }
	ActivityOpenTimeInfo *GetActivityOpenTimeInfo(int index);				// 获取活动的开启配置
	Posi GetRandomRelivePos();												// 获取一个随机的复活点

	int GetOpenActivityConfig(ActivityOpenTimeInfo cfg_list[CROSS_TUANZHAN_OPEN_TIME_MAX_COUNT]);
	const CrossTuanzhanScoreRewardCfg *GetCrossTuanzhanScoreRewardCfg(int role_level, int score);
	const CrossTuanzhanScoreRewardCfg *GetCrossTuanzhanScoreRewardCfgBySeq(int role_level, int seq);
	const CrossTuanzhanRankRewardCfg *GetCrossTuanzhanRankRewardCfg(int role_level, int score);

private:
	int InitOtherCfg(PugiXmlNode RootElement);
	int InitActivityOpenTimeCfg(PugiXmlNode RootElement);
	int InitPillaCfg(PugiXmlNode RootElement);
	int InitReliveCfg(PugiXmlNode RootElement);
	int InitScoreRewardCfg(PugiXmlNode RootElement);
	int InitRankRewardCfg(PugiXmlNode RootElement);

	bool DayTimeFormatInvalid(int day_time);

	CrossTuanzhanOtherCfg m_other_cfg;
	CrossTuanzhanPillaCfg m_pilla_cfg;										// 柱子信息

	int m_activity_open_time_cfg_list_count;								// 活动开启时间配置数量
	ActivityOpenTimeInfo m_activity_open_time_cfg_list[CROSS_TUANZHAN_OPEN_TIME_MAX_COUNT];	// 活动开启时间

	int m_player_relive_pos_list_count;										// 复活点数量
	Posi m_player_relive_pos_list[CROSS_TUANZHAN_RELIVE_POS_MAX_COUNT];

	int m_score_reward_level_cfg_count;
	CrossTuanzhanScoreRewardLevelCfg m_score_level_reward_cfg_list[CROSS_TUANZHAN_SCORE_REWARD_LEVEL_CFG_MAX];

	int m_rank_reward_level_cfg_count;
	CrossTuanzhanRankRewardLevelCfg m_rank_reward_level_cfg_list[CROSS_TUANZHAN_SCORE_REWARD_LEVEL_CFG_MAX];
};

#endif // __CROSS_1V1_COMMON_CONFIG_HPP__

