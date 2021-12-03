#ifndef __GUILD_QUESTION_CONFIG_HPP__
#define __GUILD_QUESTION_CONFIG_HPP__

#include <map>

#include "servercommon/configbase.h"
#include "servercommon/activitydef.hpp"
#include "servercommon/struct/itemlistparam.h"

static const int GUILD_QUESTION_GATHER_POS_MAX = 10;

struct GuildQuestionOtherConfig
{
	GuildQuestionOtherConfig() : scene_id(0), enter_pos(0, 0), begin_time(0), end_notice_time(0), reward_interval_s(0), question_total_num(0), next_question_interval(0), answer_right_interval_s(0),
		guild_score(0), gather_id(0), gather_time_s(0), gather_count_limit(0), right_guild_factor(0), exp_factor_gather(0), exp_factor_wait(0) {}

	int scene_id;
	Posi enter_pos;
	int begin_time;
	int end_notice_time;
	int reward_interval_s;
	int question_total_num;
	int next_question_interval;
	int answer_right_interval_s;
	int guild_score;

	int gather_id;
	int gather_time_s;
	int gather_count_limit;

	double right_guild_factor;
	double exp_factor_gather;
	double exp_factor_wait;

	ItemConfigData gather_reward_item;
};

struct GuildQuestionQuestionConfig
{
	GuildQuestionQuestionConfig()
	{
		memset(question_str, 0, sizeof(question_str));
		memset(answer, 0, sizeof(answer));
	}

	GuildQuestionStr question_str;
	GuildQuestionStr answer;
};

struct GuildQuestionRewardConfig
{
	GuildQuestionRewardConfig() : role_level_min(0), role_level_max(0), exp(0), gongxian(0), answer_right_myself_addexp(0), answer_right_myself_addgongxian(0),
		answer_right_give_member_addexp(0), answer_right_give_member_addgongxian(0) {}

	int role_level_min;
	int role_level_max;
	long long exp;
	int gongxian;
	long long answer_right_myself_addexp;
	int answer_right_myself_addgongxian;
	long long answer_right_give_member_addexp;
	int answer_right_give_member_addgongxian;
};

struct GuildQuestionRankRewardConfig
{

	static const int MAX_ITEM_NUM = 6;

	GuildQuestionRankRewardConfig() : rank(0), reward_bind_gold(0), gongzi(0), has_reward(false), reward_item_count(0) {}

	int rank;
	int reward_bind_gold;
	int gongzi;

	bool has_reward;
	int reward_item_count;
	ItemConfigData reward_item_list[MAX_ITEM_NUM];
};

class GuildQuestionConfig : public ConfigBase
{
public:
	GuildQuestionConfig();
	~GuildQuestionConfig();

	bool Init(const std::string &path, std::string *err);

	const GuildQuestionOtherConfig & GetOtherCfg() { return m_other_cfg; }
	int RandQuestionID();
	void GetQuestionStr(int question_id, GuildQuestionStr question_str);
	void GetAnswerStr(int question_id, GuildQuestionStr answer_str);
	int CalcScore(int answer_time, bool is_correct);
	const GuildQuestionRewardConfig * GetRewardCfg(int role_level);
	const GuildQuestionRankRewardConfig * GetRankRewardCfg(int rank);

	int GetGatherPosCount() { return m_pos_count; }
	bool GetGatherPos(int seq, Posi *get_pos);

private:

	int InitOtherCfg(TiXmlElement *RootElement);
	int InitQuestionCfg(TiXmlElement *RootElement);
	int InitRewardCfg(TiXmlElement *RootElement);
	int InitRankRewardCfg(TiXmlElement *RootElement);
	int InitGatherPosCfg(TiXmlElement *RootElement);

	GuildQuestionOtherConfig m_other_cfg;

	std::map<int, GuildQuestionQuestionConfig> m_question_map; // 题目ID对应的信息

	std::vector<GuildQuestionRewardConfig> m_reward_list;

	int m_rank_cfg_count;
	GuildQuestionRankRewardConfig m_rank_reward_cfg_list[GUILD_QUESTION_SCORE_RANK_MAX + 1];

	int m_pos_count = 0;
	Posi m_pos_list[GUILD_QUESTION_GATHER_POS_MAX];						// 采集物坐标点
};

#endif // __GUILD_QUESTION_CONFIG_HPP__

