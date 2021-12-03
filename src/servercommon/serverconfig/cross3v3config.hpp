#ifndef __CROSS_3v3_CONFIG_H__
#define __CROSS_3v3_CONFIG_H__

#include "servercommon/crossdef.hpp"
#include "servercommon/servercommon.h"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

const static int CROSS_3V3_REWARD_ITEM_MAX_COUNT = 5;						// 物品奖励列表允许配的最大个数
const static int CROSS_ACTIVITY_3V3_JOIN_TIME_CFG_MAX_COUNT = 3;			// 参与次数配置最大数
static const int CROSS_ACTIVITY_3V3_ORDER_COUNT = 5;                        // 每个段位阶数
static const int CROSS_ACTIVITY_3V3_GRADE_TYPE_MAX = 35;					// 3V3最高段位
static const int CROSS_ACTIVITY_3V3_RANK_CFG_MAX_COUNT = 8;					// 排行奖励配置最大数
static const int CROSS_ACTIVITY_3V3_GONGXUN_CFG_MAX_COUNT = 20;				// 功勋奖励配置最大数
static const int CROSS_ACTIVITY_3V3_ETRA_GRADE_TYPE_MAX = 37;			    // 3V3最高段 + 排名
static const int CROSS_ACTIVITY_3V3_SEASON_MAX = 32;			    // 3v3最多只可保存32个赛季的令牌信息

enum CROSS_3V3_CARD_TYPE
{
	CROSS_3V3_CARD_TYPE_GRADE = 0,
	CROSS_3V3_CARD_TYPE_RANK = 1,
};

struct Cross3v3OtherConfig
{
	Cross3v3OtherConfig():is_open(0),join_limit_min_level(0),join_limit_min_capability(0),join_limit_daycount(0),
		match_scene_id(0),finish_match_score(0),kill_add_score(0),occupy_add_score(0),stronghold_add_score(0),
		exp_dan_type(0),exp_dan_count(0),win_exp_per(0),equal_exp_per(0),fail_exp_per(0),win_shengwang(0),win_gongxun(0),
		fail_gongxun(0),division_need_match_count(0),rank_score_limit(0),stronghold_radius(0),ready_time_s(0),fighting_time_s(0),
		season_days(0), open_server_day(0)
	{
		stronghold_pos=Posi(0,0);
		memset(relive_pos_list, 0, sizeof(relive_pos_list));
	}
	bool is_open;															// 活动是否开启
	int join_limit_min_level;												// 最小等级
	int join_limit_min_capability;											// 最低战力
	int join_limit_daycount;												// 限制每天参数次数
	int match_scene_id;														// 比赛场景
	Posi relive_pos_list[CROSS_MULTIUSER_CHALLENGE_SIDE_MAX];				// 复活坐标
	int finish_match_score;													// 提前结束比赛分值
	
	int kill_add_score;														// 击杀加分
	int occupy_add_score;													// 占领加分(暂时废弃)
	int stronghold_add_score;												// 据点加分(暂时废弃)

	int exp_dan_type;														// 经验丹类型
	int exp_dan_count;														// 经验丹数目
	int win_exp_per;														// 胜利获取经验百分比
	int equal_exp_per;														// 平局获取经验百分比
	int fail_exp_per;														// 失败获取经验百分比

	int win_shengwang;														// 胜利奖励声望
	int win_gongxun;;														// 胜利获得功勋
	int equal_shengwang;													// 平局奖励声望
	int equal_gongxun;														// 平局获得功勋
	int fail_shengwang;														// 失败奖励声望
	int fail_gongxun;														// 失败获得功勋
	
	int division_need_match_count;											// 获得段位至少需要匹配场数
	int rank_score_limit;													// 排行榜上榜积分限制

	Posi stronghold_pos;													// 据点坐标
	int  stronghold_radius;													// 据点半径
	int	 ready_time_s;														// 准备时间
	int  fighting_time_s;													// 战斗时间
	int  season_days;														// 赛季天数
	int  open_server_day;													// 开服第几天开启
};

struct Cross3v3ActivityOpenTimeInfo
{
	Cross3v3ActivityOpenTimeInfo() : activity_week_day(0), activity_ready_time(0), activity_start_time(0), activity_end_time(0), ready_dur_s(0), open_dur_s(0) {}

	int activity_week_day;													// 活动所在周几
	int activity_ready_time;												// 活动等待时间点
	int activity_start_time;												// 活动开始时间点
	int activity_end_time;													// 活动结束时间点

	int ready_dur_s;														// 活动等待时间
	int open_dur_s;															// 开启持续时间
};

struct Cross3v3ActivityTimeInterval
{
	Cross3v3ActivityTimeInterval() : open_day_second(0), dur_s(0) {}

	int open_day_second;		// 活动开放时间 距0点的秒数
	int dur_s;					// 持续时间 秒数
};

struct Cross3v3ScoreRankReward
{
	Cross3v3ScoreRankReward() : rank_pos_min(-1), rank_pos_max(-1), bind_gold(0), mojing(0), reward_count(0), score_limit(0) {}

	bool Invalid() const { return (rank_pos_min < 0 || rank_pos_max < 0); }

	int rank_pos_min;
	int rank_pos_max;

	int bind_gold;
	int mojing;

	int reward_count;
	ItemConfigData reward_item_list[CROSS_3V3_REWARD_ITEM_MAX_COUNT];

	int score_limit;
};

struct Cross3V3JoinTimesRewardConfig
{
	Cross3V3JoinTimesRewardConfig() : seq(0), jion_times(0), reward_count(0)
	{
	}

	int seq;
	int jion_times;
	int reward_count;
	ItemConfigData reward_item_list[CROSS_3V3_REWARD_ITEM_MAX_COUNT];
};

struct Cross3V3GongxunRewardConfig
{
	Cross3V3GongxunRewardConfig() : seq(0), need_gongxun(0), reward_count(0) {}

	int seq;
	int need_gongxun;
	int reward_count;
	ItemConfigData reward_item_list[CROSS_3V3_REWARD_ITEM_MAX_COUNT];
};

struct Cross3V3GradeScoreConfig
{
	Cross3V3GradeScoreConfig() : grade(0), score(0), win_score(0), equal_score(0), fail_score(0), bind_gold(0), mojing(0), reward_count(0) {}

	int grade;
	int score;
	
	int win_score;															// 胜利获得积分
	int equal_score;														// 平局获得积分
	int fail_score;															// 失败获得积分

	// 以下是每月发放的奖励
	int bind_gold;
	int mojing;
	int reward_count;
	ItemConfigData reward_item_list[CROSS_3V3_REWARD_ITEM_MAX_COUNT];
};

struct Cross3V3PriorityConfig
{
	Cross3V3PriorityConfig() : priority(0), match_time_s(0), interval(0) {}

	int priority;
	int match_time_s;								// 当前已匹配的时间
	int interval;									// 匹配时允许最大段位间隔
};

struct Cross3V3ExtraScoreConfig
{
	Cross3V3ExtraScoreConfig() : gap(0), extra_score(0) {}

	int gap;
	int extra_score;
};

struct Cross3V3DurWinScoreConfig
{
	Cross3V3DurWinScoreConfig() : dur_win_times(0), extra_score(0) {}

	int dur_win_times;
	int extra_score;
};

struct Cross3v3SeasonRewardConfig
{
	Cross3v3SeasonRewardConfig() : seq(0), season(0), grade(0), rank(0), 
		max_hp(0), gong_ji(0), po_jia(0), fang_yu(0), fa_fang_yu(0), add_hurt(0), reduce_hurt(0),
		card_type(0) 
	{
	}

	int seq;
	int season;

	// 以下两个只能有一个生效，证明这份配置是段位令牌，还是排行奖励令牌
	int grade;
	int rank;

	ItemConfigData reward_item;
	Attribute max_hp;
	Attribute gong_ji;
	Attribute po_jia;
	Attribute fang_yu;
	Attribute fa_fang_yu;
	Attribute add_hurt;
	Attribute reduce_hurt;

	int card_type;
};

struct StrongholdRateConfig
{
	StrongholdRateConfig() : rate(0), rate_role(0) {}

	int rate;		// 进度条占领速率
	int rate_role;	// 角色积分增加速率(每秒)
};

// ==========================================================
// 新的跨服3v3配置类，原来是：CrossMultiUserChallengeConfig
// 由于原配置读取类没有删除，所以新的3v3配置直接改成这个
// ==========================================================
class Cross3v3Config
{
public:
	static Cross3v3Config & Instance();

	bool Init(const std::string &path, std::string *err);

	const Cross3v3OtherConfig & GetOtherCfg() { return m_other_cfg; }
	int GetOpenActivityConfig(Cross3v3ActivityOpenTimeInfo cfg_list[CROSS_ACTIVITY_OPEN_TIME_CFG_MAX_COUNT]);
	Cross3v3ActivityOpenTimeInfo *GetCrossActivityOpenTimeInfo(int index);// 获取活动的开启配置

	const Cross3V3GradeScoreConfig * GetDivisionConfigWithScore(int score);

	int GetOpenDay() const { return 1; } // 写死还是读配置？先不删除，预防策划后面要还原，原来配置是30
	int GetJoinMinLevel() const { return m_other_cfg.join_limit_min_level; }
	int GetJoinMinCapability() const { return m_other_cfg.join_limit_min_capability; }
	int GetJoinDayCountLimit() const { return m_other_cfg.join_limit_daycount; }

	int GetMatchSceneID() const { return m_other_cfg.match_scene_id; }
	Posi GetRelivePos(int side) const;

	int GetFinishMatchScore() { return m_other_cfg.finish_match_score; }

	int GetKillAddScore() { return m_other_cfg.kill_add_score; }
	//int GetOccupyAddScore() { return m_other_cfg.occupy_add_score; }
	//int GetStrongholdAddScore() { return m_other_cfg.stronghold_add_score; }
	Posi GetStrongholdPos(int & radius);
	int GetStrongholdRate(int num);		// 获取占领速率
	int GetStrongholdRoleRate(int num); // 获取加分速率
	int GetReadyTime(){ return m_other_cfg.ready_time_s; }
	int GetFightingTime() { return m_other_cfg.fighting_time_s;}
	int GetMaxSeason()const;                           // 最大赛季


	const Cross3V3GongxunRewardConfig *GetGongxunRewardCfgBySeq(int seq) const;
	const Cross3V3JoinTimesRewardConfig * GetJoinTimesRewardCfg() { return m_join_times_reward_cfg_list; }

	const Cross3v3ScoreRankReward *GetChallengeScoreRankReward(int rank_pos, int score);

	int GetDivisionDiffExtraScore(int delta_division);	// 获取比赛结束后，段位差额外加/扣的积分（传入的是自己跟对方段位差值）
	int GetDurWinExtraScore(int dur_win_count);

	const std::vector<Cross3V3PriorityConfig> &GetPriorityConfig() const { return m_priority_cfg_vec; }
	
	// 获取赛季令牌配置， 3v3的有效排行名次pos从1开始
	const Cross3v3SeasonRewardConfig *GetSeasonCfgBySeasonGrade(int season, int grade, int pos = 0) const;
	const Cross3v3SeasonRewardConfig *GetSeasonCfgBySeq(int seq) const;
	// 获取N赛季后礼包配置， 3v3的有效排行名次pos从1开始
	const Cross3v3SeasonRewardConfig *GetAfterSeasonCfgBySeasonGrade(int season, int grade, int pos = 0) const;
	
private:
	Cross3v3Config();
	~Cross3v3Config();

	int InitOtherCfg(PugiXmlNode RootElement);
	int InitActivityOpenTimeCfg(PugiXmlNode RootElement);
	int InitStrongHoldCfg(PugiXmlNode RootElement);
	int InitJoinRewardCfg(PugiXmlNode RootElement);
	int InitGongxunRewardCfg(PugiXmlNode RootElement);
	int InitDivisionScoreCfg(PugiXmlNode RootElement);
	int InitRankRewardCfg(PugiXmlNode RootElement);
	int InitPriorityConfig(PugiXmlNode RootElement);
	int InitDivistionGapExtraScoreCfg(PugiXmlNode RootElement);
	int InitDurWinCfg(PugiXmlNode RootElement);
	int InitSeasonRewardCfg(PugiXmlNode RootElement);
	int InitAfterSeasonRewardCfg(PugiXmlNode RootElement);

	bool DayTimeFormatInvalid(int day_time);

	Cross3v3OtherConfig m_other_cfg;

	int m_activity_open_time_cfg_list_count;								// 活动开启时间配置数量
	Cross3v3ActivityOpenTimeInfo m_activity_open_time_cfg_list[CROSS_PASTURE_OPEN_TIME_MAX_COUNT];	// 活动开启时间

	StrongholdRateConfig m_stronghold_rate_list[CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT];	// 据点速率

	int m_gongxun_reward_cfg_max_seq;
	Cross3V3GongxunRewardConfig m_gongxun_reward_cfg_list[CROSS_ACTIVITY_3V3_GONGXUN_CFG_MAX_COUNT];

	Cross3v3ScoreRankReward m_rank_reward_cfg_list[CROSS_MULTIUSER_CHALLENGE_SCORE_RANK_REWARD_MAX_COUNT];	// 积分排行奖励列表

	Cross3V3JoinTimesRewardConfig m_join_times_reward_cfg_list[CROSS_ACTIVITY_3V3_JOIN_TIME_CFG_MAX_COUNT];

	std::vector<Cross3V3GradeScoreConfig> m_grade_score_cfg_vec;

	int m_max_priority;
	std::vector<Cross3V3PriorityConfig> m_priority_cfg_vec;

	std::vector<Cross3V3ExtraScoreConfig> m_extra_score_cfg_vec;
	std::vector<Cross3V3DurWinScoreConfig> m_dur_win_score_cfg_vec;

	int m_max_season;
	typedef std::vector<std::vector<Cross3v3SeasonRewardConfig> > Cross3v3SeasonVector;
	Cross3v3SeasonVector m_season_reward_grade_cfg_vec;
	Cross3v3SeasonVector m_season_reward_rank_cfg_vec;

	std::vector<Cross3v3SeasonRewardConfig> m_after_season_reward_grade_cfg_vec;
	std::vector<Cross3v3SeasonRewardConfig> m_after_season_reward_rank_cfg_vec;
};

//typedef Cross3v3Config CrossMultiUserChallengeConfig;

#endif	// __CROSS_3v3_CONFIG_H__

