#ifndef __CHALLENGE_FIELD_CONFIG_HPP__
#define __CHALLENGE_FIELD_CONFIG_HPP__

#include <string>
#include "servercommon/serverdef.h"
#include "servercommon/challengefielddef.hpp"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include <vector>
#include <map>

struct ChallengeFieldOtherConfig
{
	ChallengeFieldOtherConfig() : scene_id(0), sponsor_pos(0, 0), opponent_pos(0, 0),free_join_times_before(0),free_join_times(0), buy_join_times_cost(0),
		win_add_jifen(0), lose_add_jifen(0), win_add_exp(0), lose_add_exp(0), win_add_guanghui(0), lose_add_guanghui(0),
		buy_buff_gold(0), buy_buff_times_max(0), buff_add_maxhp_per(0), buff_add_gongji_per(0), rank_reward_count(0), rank_reward_wing_id(0), win_exp_factor(0), lose_exp_factor(0),
		open_dur_day(0), change_free_times_create_day(0)
	{

	}

	int scene_id;
	Posi sponsor_pos;
	Posi opponent_pos;
	int free_join_times_before;
	int free_join_times;
	int buy_join_times_cost;
	int win_add_jifen;
	int lose_add_jifen;
	int win_add_exp;
	int lose_add_exp;
	int win_add_guanghui;
	int lose_add_guanghui;
	int buy_buff_gold;
	int buy_buff_times_max;
	int buff_add_maxhp_per;
	int buff_add_gongji_per;
	int rank_reward_count;
	int rank_reward_wing_id;
	double win_exp_factor;
	double lose_exp_factor;
	int open_dur_day;		// 竞技场开放持续天数，从开服第一天开始
	int change_free_times_create_day; // 创角第几天变更免费次数
};

struct ChallengeFieldJifenRewardConfig
{
	const static int DETAIL_REWARD_CFG_MAX_COUNT = 32;

	struct DetailRewardConfig
	{
		DetailRewardConfig() : role_level(0), reward_guanghui(0) {}

		int role_level;
		int reward_guanghui;
		ItemConfigData reward_item;
	};

	struct ConfigItem
	{
		ConfigItem() : seq(0), need_jifen(0), detail_reward_cfg_count(0) 
		{
		}

		const DetailRewardConfig * GetDetailReward(int role_level) const;

		int seq;
		int need_jifen;

		int detail_reward_cfg_count;
		DetailRewardConfig detail_reward_cfglist[DETAIL_REWARD_CFG_MAX_COUNT];
	};

	ChallengeFieldJifenRewardConfig() : cfg_count(0) {}

	int cfg_count;
	ConfigItem cfg_list[CHALLENGE_FIELD_JIFEN_REWARD_MAX_COUNT];
};

// 排名奖励
struct ChallengeFieldRankRewardConfig
{
	const static int CONFIG_ITEM_MAX_COUNT = 32;

	struct ConfigItem
	{
		ConfigItem() : min_rank_pos(0), max_rank_pos(0), reward_guanghui(0), title_id(0), wing_id(0), reward_count(0) {}

		int min_rank_pos;
		int max_rank_pos;
		int reward_guanghui;
		int title_id;
		int wing_id;
		int reward_count;
		ItemConfigData reward_item_list[MAX_ATTACHMENT_ITEM_NUM];
	};

	ChallengeFieldRankRewardConfig() : cfg_count(0) {}

	int cfg_count;
	ConfigItem cfg_list[CONFIG_ITEM_MAX_COUNT];
};

struct ChallengeFieldLuckRewardConfig
{
	struct ConfigItem
	{
		ConfigItem() : seq(0), reward_guanghui(0) {}

		int seq;
		int reward_guanghui;
	};

	ChallengeFieldLuckRewardConfig() : cfg_count(0) {}

	int cfg_count;
	ConfigItem cfg_list[CHALLENGE_FIELD_LUCK_RANK_POS_MAX_COUNT];
};

struct ChallengeFieldRankRewardTime
{
	ChallengeFieldRankRewardTime() : hour(0), minute(0), percent(0) {}

	int hour;
	int minute;
	int percent;
};

struct ChallengeFieldHistoryRankRewardConfig
{
	const static int CONFIG_ITEM_MAX_COUNT = 64;

	struct ConfigItem
	{
		ConfigItem() : best_rank_pos(0), reward_gold_bind(0) {}

		int best_rank_pos;
		int reward_gold_bind;
	};

	ChallengeFieldHistoryRankRewardConfig(): min_rank_pos(0), max_rank_pos(0), cfg_count(0) {}
	
	int min_rank_pos;
	int max_rank_pos;

	int cfg_count;
	ConfigItem cfg_list[CONFIG_ITEM_MAX_COUNT];
};

struct ChallengeFieldRobotConfig
{
	const static int CONFIG_ITEM_MAX_COUNT = 1000;

	ChallengeFieldRobotConfig() : is_sex_common(false), robot_sex(0) {}

	GameName robot_name;
	bool is_sex_common;
	int robot_sex;
};

const static int CHALLENGE_FIELD_BEST_RANK_CFG_MAX_COUNT = 30;
struct ChallengeFieldBestRankPosCfg
{
	ChallengeFieldBestRankPosCfg() : rank_pos(0), maxhp(0), gongji(0), fangyu(0), reward_guanghui(0)
	{}

	int rank_pos;
	Attribute maxhp;
	Attribute gongji;
	Attribute fangyu;
	int reward_guanghui;

	ItemConfigData reward_item[MAX_ATTACHMENT_ITEM_NUM];
};

// 跨服竞技场-------------------------------------------------
// 跨服排名奖励
struct CrossRankRewardCfg
{
	int min_rank_pos = 0;
	int max_rank_pos = 0;
	int img_id = 0;
	int title_id = 0;
};

struct CrossRankOtherRewardCfg
{
	int min_rank_pos = 0;
	int max_rank_pos = 0;
	int reward_hunjing = 0;
};

class ChallengeFieldConfig : public ConfigBase
{
public:
	ChallengeFieldConfig();
	~ChallengeFieldConfig();

	bool Init(std::string path, std::string *err);

	const ChallengeFieldOtherConfig & GetOtherCfg() { return m_other_cfg; }
	const ChallengeFieldJifenRewardConfig::ConfigItem * GetJifenCfg(int seq);
	const ChallengeFieldLuckRewardConfig::ConfigItem * GetLuckyRewardCfg(int seq);
	const ChallengeFieldRankRewardConfig::ConfigItem * GetRankRewardCfg(int open_game_day, int seq);
	const ChallengeFieldRankRewardConfig::ConfigItem * GetRankRewardCfgByRank(int open_game_day, int rank);
	int GetRankRewardCfgCount(int open_game_day);

	unsigned int CalNextRankRewadTime(int *reward_percent);
	const ChallengeFieldRankRewardTime * GetFirstRankRewardTime();
	const ChallengeFieldRankRewardTime * GetLastEndRankRewardTime();   // 获取最后一个奖励结算时间

	ChallengeFieldHistoryRankRewardConfig::ConfigItem * GetHistoryRankRewardCfg(int rank_pos);

	const std::vector<ChallengeFieldRobotConfig> & GetChallengeFieldRobotName();

	const ChallengeFieldBestRankPosCfg * GetBestRankPosCfgByLeveL(int level);

	int GetRankTopRewardWingID() { return m_other_cfg.rank_reward_wing_id; }

	int GetRoleFreeJoinTimes(const int level);
	int GetRoleFreeJoinTimesByCreateTime(const time_t create_role_time);

	// 跨服竞技场-------------------------------------------------
	int GetCrossSceneId() { return m_cross_scene_id; }
	bool IsCrossRewardDay(int day_index);
	int GetCrossRewardDay() { return m_cross_reward_weekday; }
	const CrossRankRewardCfg * GetCrossRankReward(int rank);
	const CrossRankOtherRewardCfg * GetCrossRankOtherReward(int rank);

private:
	int InitOtherCfg(PugiXmlNode RootElement);
	int InitJifenRewardCfg(PugiXmlNode RootElement);
	int InitJifenRewardDetailCfg(PugiXmlNode RootElement);
	int InitLuckyRewardCfg(PugiXmlNode RootElement);
	int InitRankRewardCfg(PugiXmlNode RootElement);
	int InitRankRewardTimeCfg(PugiXmlNode RootElement);
	int InitHistoryRewardConfig(PugiXmlNode RootElement);
	int InitRobotConfig(PugiXmlNode RootElement);
	int InitCrossRankRewardCfg(PugiXmlNode RootElement);
	int InitCrossRankOtherRewardCfg(PugiXmlNode RootElement);

	ChallengeFieldOtherConfig m_other_cfg;
	ChallengeFieldJifenRewardConfig m_jifen_reward_cfg;
	ChallengeFieldLuckRewardConfig m_lucky_reward_cfg;
	std::map<int, ChallengeFieldRankRewardConfig> m_rank_reward_cfg;  // 排名奖励 <open_game_day, rank_cfg>

	//////////////////////////////////////////////////////////////////////////////////////////////////
	const static int RANK_REWARD_TIME_MAX_COUNT = 32;

	int m_rank_reward_time_cfgcount;  // 每日奖励结算时间个数
	ChallengeFieldRankRewardTime m_rank_reward_time_cfglist[RANK_REWARD_TIME_MAX_COUNT];

	ChallengeFieldHistoryRankRewardConfig m_history_reward_cfg;

	std::vector<ChallengeFieldRobotConfig> m_robot_name_vector;

	int m_best_rank_pos_cfg_count;
	ChallengeFieldBestRankPosCfg m_best_rank_pos_cfg_list[CHALLENGE_FIELD_BEST_RANK_CFG_MAX_COUNT];

	// 跨服竞技场-------------------------------------------------
	int m_cross_scene_id = 0;
	int m_cross_reward_weekday = 0;
	std::vector<CrossRankRewardCfg> m_cross_rank_reward_vec;
	std::vector<CrossRankOtherRewardCfg> m_cross_rank_other_reward_vec;

};

#endif // __CHALLENGE_FIELD_CONFIG_HPP__


