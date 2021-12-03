#ifndef __CROSS_1V1_COMMON_CONFIG_HPP__
#define __CROSS_1V1_COMMON_CONFIG_HPP__

#include "servercommon/crossdef.hpp"
#include "servercommon/servercommon.h"
#include "servercommon/activitydef.hpp"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include "servercommon/struct/itemlistparam.h"
#include <map>
#include <vector>

static const int CROSS_1V1_DEFAULT_STANDBY_TIME_S = 300;
static const int CROSS_1V1_DEFAULT_OPEN_TIME_S = 1800;
const static int CROSS_1V1_REWARD_ITEM_MAX_COUNT = 4;

const static int CROSS_1V1_MAX_SEASON = 32;         // 跨服1v1最多只可保存32个赛季的戒指信息

struct Cross1V1ActivityTimeInterval
{
	Cross1V1ActivityTimeInterval() : open_day_second(0), dur_s(0) {}

	int open_day_second;		// 活动开放时间 距0点的秒数
	int dur_s;					// 持续时间 秒数
};

struct Cross1V1OtherConfig
{
	Cross1V1OtherConfig() :is_open(false), free_times(0), max_buy_times(0), buy_time_cost(0), match_time_s(0), fight_wait_s(0), fight_time_s(0), pos_1(0, 0), pos_2(0, 0), scene_id(0),
		winner_score(0), loser_score(0), winner_exp_per(0), loser_exp_per(0), cross_info_clear_time(0), role_shadow_add_attr_per(0), role_shadow_alay_ms(0), limit_join_level(0), fail_addorreduce_grade(0),
		open_server_day(0)
	{
	}

	bool is_open;
	int free_times;
	int max_buy_times;
	int buy_time_cost;
	int match_time_s;
	int fight_wait_s;
	int fight_time_s;
	Posi pos_1;
	Posi pos_2;
	int scene_id;
	int winner_score;
	int loser_score;
	int winner_exp_per;
	int loser_exp_per;
	int cross_info_clear_time;
	int role_shadow_add_attr_per;
	int role_shadow_alay_ms;
	ItemConfigData winner_item;
	ItemConfigData loser_item;
	ItemConfigData join_item;
	int atleast_join_times;
	int rank_reward_min_grade;
	int limit_join_level;
	int fail_addorreduce_grade;
	int open_server_day;													// 开服第几天开启
};

struct Cross1V1OpenTimeConfig
{
	Cross1V1OpenTimeConfig() : activity_week_day(0), activity_ready_time(0), activity_start_time(0), activity_end_time(0), ready_dur_s(0), open_dur_s(0) {}

	int activity_week_day;													// 活动所在周几
	int activity_ready_time;												// 活动等待时间点
	int activity_start_time;												// 活动开始时间点
	int activity_end_time;													// 活动结束时间点

	int ready_dur_s;														// 活动等待时间
	int open_dur_s;															// 开启持续时间
};

struct Cross1V1JoinTimesRewardConfig
{
	Cross1V1JoinTimesRewardConfig() : seq(0), jion_times(0), reward_item_l_count(0), reward_item_h_count(0)
	{
	}

	int seq;
	int jion_times;
	int reward_item_l_count;
	int reward_item_h_count;
	ItemConfigData reward_item_l_list[MAX_ATTACHMENT_ITEM_NUM];
	ItemConfigData reward_item_h_list[MAX_ATTACHMENT_ITEM_NUM];
};

struct Cross1V1ScoreRewardConfig
{
	Cross1V1ScoreRewardConfig() : seq(0), need_score(0), reward_count(0) {}

	int seq;
	int need_score;
	int reward_count;
	ItemConfigData reward_item_list[CROSS_1V1_REWARD_ITEM_MAX_COUNT];
};

struct Cross1V1RankRewardConfig
{
	Cross1V1RankRewardConfig() : min_rank_pos(0), max_rank_pos(0), score_limit(0), reward_count(0), coin(0), bind_gold(0), rank_ring_grade(0)
	{
	}

	int min_rank_pos;
	int max_rank_pos;
	int score_limit;
	int reward_count;
	int coin;
	int bind_gold;
	int rank_ring_grade;
	ItemConfigData reward_item_list[CROSS_1V1_REWARD_ITEM_MAX_COUNT];
};

struct Cross1V1RandNameConfig
{
	Cross1V1RandNameConfig() : sex(-1)
	{
		memset(name, 0, sizeof(name));
	}

	int sex;
	GameName name;
};

struct Cross1V1GradeScoreConfig
{
	Cross1V1GradeScoreConfig() : grade(0), score(0), victory_score(0), fail_score(0), reward_count(0), bind_gold(0), coin(0) {}
	int grade;
	int score;
	int victory_score;
	int fail_score;
	int reward_count;
	int bind_gold;
	int coin;
	ItemConfigData reward_item_list[CROSS_1V1_REWARD_ITEM_MAX_COUNT];
};

struct Cross1V1PriorityConfig
{
	Cross1V1PriorityConfig() : priority(0), match_time_s(0), interval(0) {}

	int priority;
	int match_time_s;
	int interval;
};

struct Cross1V1ExtraScoreConfig
{
	Cross1V1ExtraScoreConfig() : gap(0), extra_score(0) {}

	int gap;
	int extra_score;
};

struct Cross1V1SeasonRewardConfig
{
	Cross1V1SeasonRewardConfig() : seq(0), season(0), grade(0), max_hp(0), gong_ji(0), po_jia(0),
		fang_yu(0), fa_fang_yu(0), add_shang(0), reduce_shang(0) {}

	int seq;
	int season;
	int grade;
	ItemConfigData reward_item;
	Attribute max_hp;
	Attribute gong_ji;
	Attribute po_jia;
	Attribute fang_yu;
	Attribute fa_fang_yu;
	Attribute add_shang;
	Attribute reduce_shang;
};

struct Cross1V1ExtraIncreaseScoreConfig
{
	Cross1V1ExtraIncreaseScoreConfig() : dur_win_times(0), extra_score(0) {}

	int dur_win_times;
	int extra_score;
};

class Cross1V1CommonConfig
{
public:
	Cross1V1CommonConfig();
	~Cross1V1CommonConfig();

	static Cross1V1CommonConfig & Instance();

	bool Init(const std::string &configname, std::string *err);

	const Cross1V1OtherConfig &GetOtherCfg() { return m_other_cfg; }
	int GetOpenActivityConfigList(Cross1V1OpenTimeConfig cfg_list[CROSS_ACTIVITY_OPEN_TIME_CFG_MAX_COUNT]) const;
	int GetActivityStandbyTime(int index) const;
	int GetActivityOpenTime(int index) const;
	const Cross1V1JoinTimesRewardConfig *GetJoinTimeRewardCfgBySeq(int seq) const;
	const Cross1V1ScoreRewardConfig *GetScoreRewardCfgBySeq(int seq) const;
	const Cross1V1RankRewardConfig *GetScoreRankRewardByRankPos(int rank_pos, int score) const;
	const Cross1V1RandNameConfig *GetRandNameConfig() const;

	const Cross1V1GradeScoreConfig *GetGradeScoreCfgByGrade(int grade) const;
	const Cross1V1GradeScoreConfig *GetGradeScoreCfgByScore(int score) const;
	const Cross1V1PriorityConfig *GetPriorityCfgByMatchTimes(int times) const;
	const Cross1V1SeasonRewardConfig *GetSeasonCfgBySeasonGrade(int season, int grade) const;
	const Cross1V1SeasonRewardConfig *GetSeasonCfgBySeq(int seq)const;
	const Cross1V1SeasonRewardConfig *GetAfterSeasonCfgBySeasonGrade(int season, int grade) const;
	int GetMaxPriority()const;                      
	int GetMaxSeason()const;                           // 最大赛季
	int GetExtraScoreByGap(int grade_self, int grade_other)const;
	int GetGradeByScore(int score)const;
	int GetExtraScoreByWinTimes(int times)const;

	int GetFightWaitTime() const { return m_other_cfg.fight_wait_s; }
	int GetInitScore() const { return 0; }
	Posi GetPos1() const { return m_other_cfg.pos_1; }
	Posi GetPos2() const { return m_other_cfg.pos_2; }
	int GetSceneID() const { return m_other_cfg.scene_id; }

	unsigned int GetCrossInfoClearTime() { return m_other_cfg.cross_info_clear_time; }
	long long GetRewardExpByLevel(int level);

	bool GetIsAddFailScoreByGrade(int grade);          //段位分界之下失败加积分，之上减积分
private:
	int InitOtherCfg(PugiXmlNode RootElement);
	int InitActivityOpenTimeCfg(PugiXmlNode RootElement);
	int InitJoinTimesRewardCfg(PugiXmlNode RootElement);
	int InitScoreRewardCfg(PugiXmlNode RootElement);
	int InitRankRewardCfg(PugiXmlNode RootElement);
	int InitRewardExpCfg(PugiXmlNode RootElement);
	int InitRandNameCfg(PugiXmlNode RootElement);

	int InitGradeScoreCfg(PugiXmlNode RootElement);
	int InitPriorityConfig(PugiXmlNode RootElement);
	int InitExtraScoreConfig(PugiXmlNode RootElement);
	int InitExtraIncreaseScoreConfig(PugiXmlNode RootElement);
	int InitSeasonRewardConfig(PugiXmlNode RootElement);
	int InitAfterSeasonRewardConfig(PugiXmlNode RootElement);

	bool DayTimeFormatInvalid(int day_time);

	Cross1V1OtherConfig m_other_cfg;

	int m_activity_open_time_cfg_list_count;														// 活动开启时间配置数量
	Cross1V1OpenTimeConfig m_activity_open_time_cfg_list[CROSS_ACTIVITY_OPEN_TIME_CFG_MAX_COUNT];	// 活动开启时间

	Cross1V1JoinTimesRewardConfig m_join_times_reward_cfg_list[CROSS_ACTIVITY_1V1_JOIN_TIME_CFG_MAX_COUNT];
	Cross1V1ScoreRewardConfig m_score_reward_cfg_list[CROSS_ACTIVITY_1V1_SCORE_CFG_MAX_COUNT];
	Cross1V1RankRewardConfig m_rank_reward_cfg_list[CROSS_ACTIVITY_1V1_RANK_CFG_MAX_COUNT];

	int m_max_priority;
	int m_max_season;
	std::map<int, long long> m_reward_exp_map;
	std::map<int, Cross1V1RandNameConfig> m_rand_name_map;
	std::vector<Cross1V1GradeScoreConfig> m_grade_score_cfg_vec;
	std::vector<Cross1V1PriorityConfig> m_priority_cfg_vec;
	std::vector<Cross1V1ExtraScoreConfig> m_extra_score_cfg_vec;
	std::vector<Cross1V1ExtraIncreaseScoreConfig> m_extra_increase_score_cfg_vec;
	typedef std::vector<std::vector<Cross1V1SeasonRewardConfig> > Cross1v1SeasonVector;
	Cross1v1SeasonVector m_season_reward_cfg_vec;

	std::vector<Cross1V1SeasonRewardConfig> m_after_season_reward_cfg_vec;
};

#endif // __CROSS_1V1_COMMON_CONFIG_HPP__

