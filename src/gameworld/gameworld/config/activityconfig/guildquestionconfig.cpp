#include "guildquestionconfig.hpp"

#include "servercommon/configcommon.h"

GuildQuestionConfig::GuildQuestionConfig() : m_rank_cfg_count(0)
{

}

GuildQuestionConfig::~GuildQuestionConfig()
{

}

bool GuildQuestionConfig::Init(const std::string &path, std::string *err)
{
	char errinfo[1024] = {0};
	int iRet = 0;

	TiXmlDocument document;

	if (path == "" || !document.LoadFile(path))
	{
		sprintf(errinfo,"%s: Load GuildQuestionConfig Error,\n %s.", path.c_str(), document.ErrorDesc());
		*err = errinfo;
		return false;
	}

	TiXmlElement *RootElement = document.RootElement();
	if (NULL == RootElement)
	{
		*err = path + ": xml root node error.";
		return false;
	}

	{
		// 其他配置信息
		TiXmlElement *root_element = RootElement->FirstChildElement("other");
		if (NULL == root_element)
		{
			*err = path + ": no [other].";
			return false;
		}

		iRet = this->InitOtherCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitOtherCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 题目列表配置信息
		TiXmlElement *root_element = RootElement->FirstChildElement("question_list");
		if (NULL == root_element)
		{
			*err = path + ": no [question_list].";
			return false;
		}

		iRet = this->InitQuestionCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitQuestionCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 奖励配置信息
		TiXmlElement *root_element = RootElement->FirstChildElement("reward");
		if (NULL == root_element)
		{
			*err = path + ": no [reward].";
			return false;
		}

		iRet = this->InitRewardCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitRewardCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 排行奖励配置信息
		TiXmlElement *root_element = RootElement->FirstChildElement("rank_reward");
		if (NULL == root_element)
		{
			*err = path + ": no [rank_reward].";
			return false;
		}

		iRet = this->InitRankRewardCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitRankRewardCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 采集物坐标配置信息
		TiXmlElement *root_element = RootElement->FirstChildElement("gather_pos_cfg");
		if (NULL == root_element)
		{
			*err = path + ": no [gather_pos_cfg].";
			return false;
		}

		iRet = this->InitGatherPosCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitGatherPosCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

int GuildQuestionConfig::InitOtherCfg(TiXmlElement *RootElement)
{
	TiXmlElement *data_element = RootElement->FirstChildElement("data");
	if (NULL == data_element)
	{
		return -10000;
	}

	if (!GetSubNodeValue(data_element, "scene_id", m_other_cfg.scene_id) || m_other_cfg.scene_id <= 0)
	{
		return -1;
	}

	if (!GetSubNodeValue(data_element, "enter_pos_x", m_other_cfg.enter_pos.x) || m_other_cfg.enter_pos.x <= 0)
	{
		return -2;
	}

	if (!GetSubNodeValue(data_element, "enter_pos_y", m_other_cfg.enter_pos.y) || m_other_cfg.enter_pos.y <= 0)
	{
		return -3;
	}

	if (!GetSubNodeValue(data_element, "begin_time", m_other_cfg.begin_time) || m_other_cfg.begin_time <= 0)
	{
		return -4;
	}

	if (!GetSubNodeValue(data_element, "end_notice_time", m_other_cfg.end_notice_time) || m_other_cfg.end_notice_time <= 0)
	{
		return -5;
	}

	if (!GetSubNodeValue(data_element, "reward_interval_s", m_other_cfg.reward_interval_s) || m_other_cfg.reward_interval_s <= 0)
	{
		return -6;
	}

	if (!GetSubNodeValue(data_element, "question_total_num", m_other_cfg.question_total_num) || m_other_cfg.question_total_num <= 0)
	{
		return -7;
	}

	if (!GetSubNodeValue(data_element, "next_question_interval", m_other_cfg.next_question_interval) || m_other_cfg.next_question_interval <= 0)
	{
		return -8;
	}

	if (!GetSubNodeValue(data_element, "answer_right_interval_s", m_other_cfg.answer_right_interval_s) || m_other_cfg.answer_right_interval_s <= 0)
	{
		return -9;
	}

	if (!GetSubNodeValue(data_element, "guild_score", m_other_cfg.guild_score) || m_other_cfg.guild_score <= 0)
	{
		return -10;
	}

	if (!GetSubNodeValue(data_element, "gather_id", m_other_cfg.gather_id) || m_other_cfg.gather_id <= 0)
	{
		return -11;
	}

	if (!GetSubNodeValue(data_element, "gather_time_s", m_other_cfg.gather_time_s) || m_other_cfg.gather_time_s <= 0)
	{
		return -12;
	}

	if (!GetSubNodeValue(data_element, "gather_count_limit", m_other_cfg.gather_count_limit) || m_other_cfg.gather_count_limit <= 0)
	{
		return -13;
	}

	TiXmlElement *reward_item_element = data_element->FirstChildElement("reward_item");
	if (NULL == reward_item_element)
	{
		return -14;
	}

	int item_id = 0;
	if (!GetSubNodeValue(reward_item_element, "item_id", item_id) || item_id < 0)    // 允许配置item_id 0，代表无奖励
	{
		return -19;
	}

	m_other_cfg.gather_reward_item.item_id = item_id;

	if (item_id > 0 && !m_other_cfg.gather_reward_item.ReadConfig(reward_item_element))
	{
		return -15;
	}

	if (!GetSubNodeValue(data_element, "right_guild_factor", m_other_cfg.right_guild_factor) || m_other_cfg.right_guild_factor < 0)
	{
		return -16;
	}

	if (!GetSubNodeValue(data_element, "exp_factor_gather", m_other_cfg.exp_factor_gather) || m_other_cfg.exp_factor_gather < 0)
	{
		return -17;
	}

	if (!GetSubNodeValue(data_element, "exp_factor_wait", m_other_cfg.exp_factor_wait) || m_other_cfg.exp_factor_wait < 0)
	{
		return -18;
	}

	return 0;
}

int GuildQuestionConfig::InitQuestionCfg(TiXmlElement *RootElement)
{
	TiXmlElement *questionElement = RootElement->FirstChildElement("data");
	if (NULL == questionElement)
	{
		return -10000;
	}

	while (NULL != questionElement)
	{
		int question_id = 0;
		if (!GetSubNodeValue(questionElement, "question_id", question_id) || question_id <= 0)
		{
			return -1;
		}
		if (m_question_map.find(question_id) != m_question_map.end())
		{
			return -2;
		}

		GuildQuestionQuestionConfig question_cfg;

		{
			std::string question_str;
			if (!GetSubNodeValue(questionElement, "content", question_str))
			{
				return -3;
			}
			if (question_str.length() >= sizeof(GuildQuestionStr))
			{
				return -4;
			}
			memcpy(question_cfg.question_str, question_str.c_str(), (int)question_str.length());
			question_cfg.question_str[question_str.length()] = '\0';
		}

		{
			std::string answer_str;
			if (!GetSubNodeValue(questionElement, "answer", answer_str))
			{
				return -5;
			}
			if (answer_str.length() >= sizeof(GuildQuestionStr))
			{
				return -6;
			}
			memcpy(question_cfg.answer, answer_str.c_str(), (int)answer_str.length());
			question_cfg.answer[answer_str.length()] = '\0';
		}

		m_question_map[question_id] = question_cfg;

		questionElement = questionElement->NextSiblingElement();
	}

	return 0;
}

int GuildQuestionConfig::InitRewardCfg(TiXmlElement *RootElement)
{
	TiXmlElement *data_element = RootElement->FirstChildElement("data");
	if (NULL == data_element)
	{
		return -10000;
	}

	while (NULL != data_element)
	{
		GuildQuestionRewardConfig cfg;

		if (!GetSubNodeValue(data_element, "role_level_min", cfg.role_level_min) || cfg.role_level_min <= 0 || cfg.role_level_min > MAX_ROLE_LEVEL)
		{
			return -1;
		}

		if (!GetSubNodeValue(data_element, "role_level_max", cfg.role_level_max) || cfg.role_level_max <= 0 || cfg.role_level_max > MAX_ROLE_LEVEL)
		{
			return -2;
		}

		if (cfg.role_level_max < cfg.role_level_min)
		{
			return -3;
		}

		if (!GetSubNodeValue(data_element, "exp", cfg.exp) || cfg.exp < 0)
		{
			return -4;
		}

		if (!GetSubNodeValue(data_element, "gongxian", cfg.gongxian) || cfg.gongxian < 0)
		{
			return -5;
		}

		if (!GetSubNodeValue(data_element, "answer_right_myself_addexp", cfg.answer_right_myself_addexp) || cfg.answer_right_myself_addexp < 0)
		{
			return -6;
		}

		if (!GetSubNodeValue(data_element, "answer_right_myself_addgongxian", cfg.answer_right_myself_addgongxian) || cfg.answer_right_myself_addgongxian < 0)
		{
			return -7;
		}

		if (!GetSubNodeValue(data_element, "answer_right_give_member_addexp", cfg.answer_right_give_member_addexp) || cfg.answer_right_give_member_addexp < 0)
		{
			return -8;
		}

		if (!GetSubNodeValue(data_element, "answer_right_give_member_addgongxian", cfg.answer_right_give_member_addgongxian) || cfg.answer_right_give_member_addgongxian < 0)
		{
			return -9;
		}

		m_reward_list.push_back(cfg);

		data_element = data_element->NextSiblingElement();
	}

	return 0;
}

int GuildQuestionConfig::InitRankRewardCfg(TiXmlElement *RootElement)
{
	TiXmlElement *data_element = RootElement->FirstChildElement("data");
	if (NULL == data_element)
	{
		return -10000;
	}

	int next_rank = 1;
	while (NULL != data_element)
	{
		int rank = 0;
		if (!GetSubNodeValue(data_element, "rank", rank) || rank <= 0 || rank > GUILD_QUESTION_SCORE_RANK_MAX || rank != next_rank)
		{
			return -1;
		}

		GuildQuestionRankRewardConfig &cfg = m_rank_reward_cfg_list[rank];
		cfg.rank = rank;

		if (!GetSubNodeValue(data_element, "reward_bind_gold", cfg.reward_bind_gold) || cfg.reward_bind_gold <= 0)
		{
			return -2;
		}

		int has_reward = 0;
		if (!GetSubNodeValue(data_element, "has_reward", has_reward) || (has_reward != 1 && has_reward != 0))
		{
			return -3;
		}

		if (!GetSubNodeValue(data_element, "gongzi", cfg.gongzi) || cfg.gongzi < 0)
		{
			return -4;
		}

		cfg.has_reward = has_reward > 0;
		if (cfg.has_reward)
		{
			cfg.reward_item_count = ItemConfigData::ReadConfigList(data_element, std::string("reward_item"),
				cfg.reward_item_list, GuildQuestionRankRewardConfig::MAX_ITEM_NUM);
			if (cfg.reward_item_count < 0)
			{
				return -100 + cfg.reward_item_count;
			}
		}

		++ m_rank_cfg_count;
		++ next_rank;
		data_element = data_element->NextSiblingElement();
	}

	return 0;
}

int GuildQuestionConfig::InitGatherPosCfg(TiXmlElement *RootElement)
{
	TiXmlElement *data_element = RootElement->FirstChildElement("data");
	if (NULL == data_element)
	{
		return -10000;
	}

	m_pos_count = 0;
	while (NULL != data_element)
	{

		if (m_pos_count >= GUILD_QUESTION_GATHER_POS_MAX)
		{
			return -100;
		}

		if (!GetSubNodeValue(data_element, "pos_x", m_pos_list[m_pos_count].x) || m_pos_list[m_pos_count].x <= 0)
		{
			return -1;
		}

		if (!GetSubNodeValue(data_element, "pos_y", m_pos_list[m_pos_count].y) || m_pos_list[m_pos_count].y <= 0)
		{
			return -2;
		}

		++m_pos_count;

		data_element = data_element->NextSiblingElement();
	}

	return 0;
}

int GuildQuestionConfig::RandQuestionID()
{
	if (m_question_map.empty()) return 0;

	std::map<int, GuildQuestionQuestionConfig>::iterator it = m_question_map.begin();
	int rand_index = RandomNum(static_cast<int>(m_question_map.size()));
	std::advance(it, rand_index);

	return it->first;
}

void GuildQuestionConfig::GetQuestionStr(int question_id, GuildQuestionStr question_str)
{
	question_str[0] = 0;

	std::map<int, GuildQuestionQuestionConfig>::iterator it = m_question_map.find(question_id);
	if (it != m_question_map.end())
	{
		F_STRNCPY(question_str, it->second.question_str, sizeof(GuildQuestionStr));
	}
}

void GuildQuestionConfig::GetAnswerStr(int question_id, GuildQuestionStr answer_str)
{
	answer_str[0] = 0;

	std::map<int, GuildQuestionQuestionConfig>::iterator it = m_question_map.find(question_id);
	if (it != m_question_map.end())
	{
		F_STRNCPY(answer_str, it->second.answer, sizeof(GuildQuestionStr));
	}
}

int GuildQuestionConfig::CalcScore(int answer_time, bool is_correct)
{
	static const int MAX_SCORE_LEFT_TIME = 10;
	static const int MIN_SCORE = 2;

	if (!is_correct)
	{
		return MIN_SCORE;
	}

	int left_time = QUESTION_ANSWER_TIME_S - answer_time;
	if (left_time > MAX_SCORE_LEFT_TIME) left_time = MAX_SCORE_LEFT_TIME;

	int score = 10 * left_time / MAX_SCORE_LEFT_TIME;
	if (score < MIN_SCORE) score = MIN_SCORE;

	return score;
}

const GuildQuestionRewardConfig * GuildQuestionConfig::GetRewardCfg(int role_level)
{
	for (int i = 0; i < (int)m_reward_list.size(); ++i)
	{
		if (role_level >= m_reward_list[i].role_level_min && role_level <= m_reward_list[i].role_level_max)
		{
			return &m_reward_list[i];
		}
	}

	return NULL;
}

const GuildQuestionRankRewardConfig * GuildQuestionConfig::GetRankRewardCfg(int rank)
{
	if (rank <= 0 || rank > m_rank_cfg_count || rank > GUILD_QUESTION_SCORE_RANK_MAX)
	{
		return NULL;
	}

	return &m_rank_reward_cfg_list[rank];
}

bool GuildQuestionConfig::GetGatherPos(int seq, Posi *get_pos)
{
	if (seq < 0 || seq >= m_pos_count || seq >= GUILD_QUESTION_GATHER_POS_MAX || NULL == get_pos) return false;

	*get_pos = m_pos_list[seq];

	return true;
}
