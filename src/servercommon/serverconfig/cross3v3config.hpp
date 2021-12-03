#ifndef __CROSS_3v3_CONFIG_H__
#define __CROSS_3v3_CONFIG_H__

#include "servercommon/crossdef.hpp"
#include "servercommon/servercommon.h"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

const static int CROSS_3V3_REWARD_ITEM_MAX_COUNT = 5;						// ��Ʒ�����б��������������
const static int CROSS_ACTIVITY_3V3_JOIN_TIME_CFG_MAX_COUNT = 3;			// ����������������
static const int CROSS_ACTIVITY_3V3_ORDER_COUNT = 5;                        // ÿ����λ����
static const int CROSS_ACTIVITY_3V3_GRADE_TYPE_MAX = 35;					// 3V3��߶�λ
static const int CROSS_ACTIVITY_3V3_RANK_CFG_MAX_COUNT = 8;					// ���н������������
static const int CROSS_ACTIVITY_3V3_GONGXUN_CFG_MAX_COUNT = 20;				// ��ѫ�������������
static const int CROSS_ACTIVITY_3V3_ETRA_GRADE_TYPE_MAX = 37;			    // 3V3��߶� + ����
static const int CROSS_ACTIVITY_3V3_SEASON_MAX = 32;			    // 3v3���ֻ�ɱ���32��������������Ϣ

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
	bool is_open;															// ��Ƿ���
	int join_limit_min_level;												// ��С�ȼ�
	int join_limit_min_capability;											// ���ս��
	int join_limit_daycount;												// ����ÿ���������
	int match_scene_id;														// ��������
	Posi relive_pos_list[CROSS_MULTIUSER_CHALLENGE_SIDE_MAX];				// ��������
	int finish_match_score;													// ��ǰ����������ֵ
	
	int kill_add_score;														// ��ɱ�ӷ�
	int occupy_add_score;													// ռ��ӷ�(��ʱ����)
	int stronghold_add_score;												// �ݵ�ӷ�(��ʱ����)

	int exp_dan_type;														// ���鵤����
	int exp_dan_count;														// ���鵤��Ŀ
	int win_exp_per;														// ʤ����ȡ����ٷֱ�
	int equal_exp_per;														// ƽ�ֻ�ȡ����ٷֱ�
	int fail_exp_per;														// ʧ�ܻ�ȡ����ٷֱ�

	int win_shengwang;														// ʤ����������
	int win_gongxun;;														// ʤ����ù�ѫ
	int equal_shengwang;													// ƽ�ֽ�������
	int equal_gongxun;														// ƽ�ֻ�ù�ѫ
	int fail_shengwang;														// ʧ�ܽ�������
	int fail_gongxun;														// ʧ�ܻ�ù�ѫ
	
	int division_need_match_count;											// ��ö�λ������Ҫƥ�䳡��
	int rank_score_limit;													// ���а��ϰ��������

	Posi stronghold_pos;													// �ݵ�����
	int  stronghold_radius;													// �ݵ�뾶
	int	 ready_time_s;														// ׼��ʱ��
	int  fighting_time_s;													// ս��ʱ��
	int  season_days;														// ��������
	int  open_server_day;													// �����ڼ��쿪��
};

struct Cross3v3ActivityOpenTimeInfo
{
	Cross3v3ActivityOpenTimeInfo() : activity_week_day(0), activity_ready_time(0), activity_start_time(0), activity_end_time(0), ready_dur_s(0), open_dur_s(0) {}

	int activity_week_day;													// ������ܼ�
	int activity_ready_time;												// ��ȴ�ʱ���
	int activity_start_time;												// ���ʼʱ���
	int activity_end_time;													// �����ʱ���

	int ready_dur_s;														// ��ȴ�ʱ��
	int open_dur_s;															// ��������ʱ��
};

struct Cross3v3ActivityTimeInterval
{
	Cross3v3ActivityTimeInterval() : open_day_second(0), dur_s(0) {}

	int open_day_second;		// �����ʱ�� ��0�������
	int dur_s;					// ����ʱ�� ����
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
	
	int win_score;															// ʤ����û���
	int equal_score;														// ƽ�ֻ�û���
	int fail_score;															// ʧ�ܻ�û���

	// ������ÿ�·��ŵĽ���
	int bind_gold;
	int mojing;
	int reward_count;
	ItemConfigData reward_item_list[CROSS_3V3_REWARD_ITEM_MAX_COUNT];
};

struct Cross3V3PriorityConfig
{
	Cross3V3PriorityConfig() : priority(0), match_time_s(0), interval(0) {}

	int priority;
	int match_time_s;								// ��ǰ��ƥ���ʱ��
	int interval;									// ƥ��ʱ��������λ���
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

	// ��������ֻ����һ����Ч��֤����������Ƕ�λ���ƣ��������н�������
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

	int rate;		// ������ռ������
	int rate_role;	// ��ɫ������������(ÿ��)
};

// ==========================================================
// �µĿ��3v3�����࣬ԭ���ǣ�CrossMultiUserChallengeConfig
// ����ԭ���ö�ȡ��û��ɾ���������µ�3v3����ֱ�Ӹĳ����
// ==========================================================
class Cross3v3Config
{
public:
	static Cross3v3Config & Instance();

	bool Init(const std::string &path, std::string *err);

	const Cross3v3OtherConfig & GetOtherCfg() { return m_other_cfg; }
	int GetOpenActivityConfig(Cross3v3ActivityOpenTimeInfo cfg_list[CROSS_ACTIVITY_OPEN_TIME_CFG_MAX_COUNT]);
	Cross3v3ActivityOpenTimeInfo *GetCrossActivityOpenTimeInfo(int index);// ��ȡ��Ŀ�������

	const Cross3V3GradeScoreConfig * GetDivisionConfigWithScore(int score);

	int GetOpenDay() const { return 1; } // д�����Ƕ����ã��Ȳ�ɾ����Ԥ���߻�����Ҫ��ԭ��ԭ��������30
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
	int GetStrongholdRate(int num);		// ��ȡռ������
	int GetStrongholdRoleRate(int num); // ��ȡ�ӷ�����
	int GetReadyTime(){ return m_other_cfg.ready_time_s; }
	int GetFightingTime() { return m_other_cfg.fighting_time_s;}
	int GetMaxSeason()const;                           // �������


	const Cross3V3GongxunRewardConfig *GetGongxunRewardCfgBySeq(int seq) const;
	const Cross3V3JoinTimesRewardConfig * GetJoinTimesRewardCfg() { return m_join_times_reward_cfg_list; }

	const Cross3v3ScoreRankReward *GetChallengeScoreRankReward(int rank_pos, int score);

	int GetDivisionDiffExtraScore(int delta_division);	// ��ȡ���������󣬶�λ������/�۵Ļ��֣���������Լ����Է���λ��ֵ��
	int GetDurWinExtraScore(int dur_win_count);

	const std::vector<Cross3V3PriorityConfig> &GetPriorityConfig() const { return m_priority_cfg_vec; }
	
	// ��ȡ�����������ã� 3v3����Ч��������pos��1��ʼ
	const Cross3v3SeasonRewardConfig *GetSeasonCfgBySeasonGrade(int season, int grade, int pos = 0) const;
	const Cross3v3SeasonRewardConfig *GetSeasonCfgBySeq(int seq) const;
	// ��ȡN������������ã� 3v3����Ч��������pos��1��ʼ
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

	int m_activity_open_time_cfg_list_count;								// �����ʱ����������
	Cross3v3ActivityOpenTimeInfo m_activity_open_time_cfg_list[CROSS_PASTURE_OPEN_TIME_MAX_COUNT];	// �����ʱ��

	StrongholdRateConfig m_stronghold_rate_list[CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT];	// �ݵ�����

	int m_gongxun_reward_cfg_max_seq;
	Cross3V3GongxunRewardConfig m_gongxun_reward_cfg_list[CROSS_ACTIVITY_3V3_GONGXUN_CFG_MAX_COUNT];

	Cross3v3ScoreRankReward m_rank_reward_cfg_list[CROSS_MULTIUSER_CHALLENGE_SCORE_RANK_REWARD_MAX_COUNT];	// �������н����б�

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

