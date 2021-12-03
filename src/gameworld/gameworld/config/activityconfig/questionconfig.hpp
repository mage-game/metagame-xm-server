#ifndef __QUESTION_CONFIG_HPP__
#define __QUESTION_CONFIG_HPP__

#include <map>

#include "servercommon/activitydef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

struct HSQuestionRewardCfg
{
	HSQuestionRewardCfg() : exp(0), xianhun(0), yuanli(0), batch_answser_cost_per_question(0),wrong_answer_percentage(0)
	{

	}

	int exp;
	int xianhun;
	int yuanli;
	ItemConfigData reward_item[MAX_ATTACHMENT_ITEM_NUM];
	int batch_answser_cost_per_question;
	int wrong_answer_percentage;
};

struct QuestionOtherCfg
{
	QuestionOtherCfg():scene_id(0),m_realive_area(0,0),bianshen_card_id(0),pig_slow_down_per(0),rabbit_speed_up_per(0),rabbit_pro(0),mianda_card_id(0),bianshen_pro(0),mianda_pro(0),
	activity_prepare_time(0),answer_prepare_time(0),answer_continue_time(0),bianshen_continue_time(0),total_question_count(0), right_world_factor(0), wrong_world_factor(0), right_hotspring_factor(0),
	wrong_hotspring_factor(0)
	{}

	int scene_id;
	Posi m_realive_area;
	int bianshen_card_id;
	int pig_slow_down_per;
	int rabbit_speed_up_per;
	int rabbit_pro;
	int mianda_card_id;
	int bianshen_pro;
	int mianda_pro;
	unsigned int activity_prepare_time;
	unsigned int answer_prepare_time;
	unsigned int answer_continue_time;
	unsigned int bianshen_continue_time;
	int total_question_count;
	double right_world_factor;
	double wrong_world_factor;
	double right_hotspring_factor;
	double wrong_hotspring_factor;
	ItemConfigData common_item[MAX_ATTACHMENT_ITEM_NUM];
	ItemConfigData guild_rank_other_reward[MAX_ATTACHMENT_ITEM_NUM];
};

struct HSQuestionRankRewardCfg
{
	HSQuestionRankRewardCfg():rank(0){}

	int rank;
	ItemConfigData rank_reward_item[MAX_ATTACHMENT_ITEM_NUM];
};

enum QUESTION_TYP
{
	QUESTION_TYPE_INVALID = 0,

	QUESTION_TYPE_HOTSPRING,		// 温泉答题
	QUESTION_TYPE_WORLD,			// 世界答题
	QUESTION_TYPE_GUILD,			// 公会答题

	QUESTION_TYPE_MAX,
};

struct WGQuestionCfg
{
	WGQuestionCfg()
	{}

	short question_type;
	short open_level;
	int begin_time;
	int end_time;
	int question_interval;
	int answer_continue_time;
	long long right_exp_reward;
	long long wrong_exp_reward;
	int fast_reward_num;

	ItemConfigData fast_reward[MAX_ATTACHMENT_ITEM_NUM];
};

struct QuestionGuildRankRewardCfg
{
	QuestionGuildRankRewardCfg() : rank(0){}

	int rank;
	ItemConfigData rank_reward[MAX_ATTACHMENT_ITEM_NUM];
};

class QuestionConfig : public ConfigBase
{
public:
	QuestionConfig();
	~QuestionConfig();

	bool Init(const std::string &path, std::string *err);

	int GetQuestionEnterInfo(Posi *pos);

	int RandQuestionID(int *answer, int question_type);
	void GetQuestionStr(int question_id, QuestionStr question_str, int question_type);
	void GetAnswer0DescStr(int question_id, QuestionStr answer_desc_str, int question_type);
	void GetAnswer1DescStr(int question_id, QuestionStr answer_desc_str, int question_typ);
	void GetAnswer2DescStr(int question_id, QuestionStr answer_desc_str, int question_type);
	void GetAnswer3DescStr(int question_id, QuestionStr answer_desc_str, int question_typ);
	int GetAnswer(int question_id, int question_type);
	int HotSpringCalcScore(int answer_time);
	const HSQuestionRewardCfg * GetHotSpringRewardCfg(int role_level);
	const QuestionOtherCfg * GetOtherCfg() {return &m_other_cfg;}
	const HSQuestionRankRewardCfg * GetHotSpringRankRewardCfg(int rank);

	const WGQuestionCfg * GetWGQuestionCfg(int question_type);

	QuestionGuildRankRewardCfg * GetQuestionGuildRankReward(int rank);

private:
	struct QuestionCfg
	{
		QuestionCfg() : answer(0) 
		{
			memset(question_str, 0, sizeof(question_str));
			memset(answer_desc_0, 0, sizeof(answer_desc_0));
			memset(answer_desc_1, 0, sizeof(answer_desc_1));
		}

		QuestionStr question_str;
		int answer;
		QuestionStr answer_desc_0;
		QuestionStr answer_desc_1;
		QuestionStr answer_desc_2;
		QuestionStr answer_desc_3;
	};

	std::map<int, QuestionCfg> m_hotspring_question_map;
	std::map<int, QuestionCfg> m_world_question_map;
	std::map<int, QuestionCfg> m_guild_question_map;
	HSQuestionRewardCfg m_reward_list[MAX_ROLE_LEVEL + 1];
	QuestionOtherCfg m_other_cfg;
	std::map<int, HSQuestionRankRewardCfg> m_question_rank_reward;

	WGQuestionCfg m_wg_question_cfg[QUESTION_TYPE_MAX];
	std::map<int, QuestionGuildRankRewardCfg> m_question_guild_rank_reward;
};

#endif // __QUESTION_CONFIG_HPP__

