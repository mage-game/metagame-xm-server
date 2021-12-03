#include "questionconfig.hpp"

#include "servercommon/configcommon.h"
#include "checkresourcecenter.hpp"
#include"item/itempool.h"

QuestionConfig::QuestionConfig()
{
}

QuestionConfig::~QuestionConfig()
{

}

bool QuestionConfig::Init(const std::string &path, std::string *err)
{
	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "QuestionConfig", *err);
	
	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = path + ": xml root node error.";
		return false;
	}

	{
		PugiXmlNode questionListElement = RootElement.child("questionlist");
		if (questionListElement.empty())
		{
			*err = path + ": xml node error in [questionlist]";
			return false;
		}		

		PugiXmlNode questionElement = questionListElement.child("data");
		if (questionElement.empty())
		{
			*err = path + ": xml node error in [questionlist->question]";
			return false;
		}

		while (!questionElement.empty())
		{
			int question_type = QUESTION_TYPE_INVALID;
			if (!PugiGetSubNodeValue(questionElement, "question_type", question_type) || question_type <= QUESTION_TYPE_INVALID || question_type >= QUESTION_TYPE_MAX)
			{
				*err = path + ": xml node error in [question_list->question->question_type]";
				return false;
			}

			int question_id = 0;
			if (!PugiGetSubNodeValue(questionElement, "question_id", question_id) || question_id <= 0)
			{
				*err = path + ": xml node error in [question_list->question->question_id]";
				return false;
			}

			if (QUESTION_TYPE_HOTSPRING == question_type)
			{
				if (m_hotspring_question_map.find(question_id) != m_hotspring_question_map.end())
				{
					*err = path + ": xml node error in [hotspring_question_list->question->id repeat]";
					return false;
				}
			}
			else if(QUESTION_TYPE_WORLD == question_type)
			{
				if (m_world_question_map.find(question_id) != m_world_question_map.end())
				{
					*err = path + ": xml node error in [world_question_list->question->id repeat]";
					return false;
				}
			}
			else if(QUESTION_TYPE_GUILD == question_type)
			{
				if (m_guild_question_map.find(question_id) != m_guild_question_map.end())
				{
					*err = path + ": xml node error in [guild_question_list->question->id repeat]";
					return false;
				}
			}
			else
			{
				*err = path + ": xml node error in question_type, question_type error";
				return false;
			}

			QuestionCfg question_cfg;

			{
				std::string question_str;
				if (!PugiGetSubNodeValue(questionElement, "content", question_str))
				{
					*err = path + ": xml node error in [question_list->question->content]";
					return false;
				}
				if (question_str.length() >= sizeof(QuestionStr))
				{
					*err = path + ": xml node error in [question_list->question->timu too long]";
					return false;
				}
				memcpy(question_cfg.question_str, question_str.c_str(), (int)question_str.length());
				question_cfg.question_str[question_str.length()] = '\0';
			}

			{
				std::string answer_desc_str;
				if (!PugiGetSubNodeValue(questionElement, "answer_desc_0", answer_desc_str))
				{
					*err = path + ": xml node error in [question_list->question->answer_desc_0]";
					return false;
				}
				if (answer_desc_str.length() >= sizeof(QuestionStr))
				{
					*err = path + ": xml node error in [question_list->question->answer_desc_0 too long]";
					return false;
				}
				memcpy(question_cfg.answer_desc_0, answer_desc_str.c_str(), (int)answer_desc_str.length());
				question_cfg.answer_desc_0[answer_desc_str.length()] = '\0';
			}

			{
				std::string answer_desc_str;
				if (!PugiGetSubNodeValue(questionElement, "answer_desc_1", answer_desc_str))
				{
					*err = path + ": xml node error in [question_list->question->answer_desc_1]";
					return false;
				}
				if (answer_desc_str.length() >= sizeof(QuestionStr))
				{
					*err = path + ": xml node error in [question_list->question->answer_desc_1 too long]";
					return false;
				}
				memcpy(question_cfg.answer_desc_1, answer_desc_str.c_str(), (int)answer_desc_str.length());
				question_cfg.answer_desc_1[answer_desc_str.length()] = '\0';
			}

			{
				std::string answer_desc_str;
				if (!PugiGetSubNodeValue(questionElement, "answer_desc_2", answer_desc_str))
				{
					*err = path + ": xml node error in [question_list->question->answer_desc_2]";
					return false;
				}
				if (answer_desc_str.length() >= sizeof(QuestionStr))
				{
					*err = path + ": xml node error in [question_list->question->answer_desc_2 too long]";
					return false;
				}
				memcpy(question_cfg.answer_desc_2, answer_desc_str.c_str(), (int)answer_desc_str.length());
				question_cfg.answer_desc_2[answer_desc_str.length()] = '\0';
			}

			{
				std::string answer_desc_str;
				if (!PugiGetSubNodeValue(questionElement, "answer_desc_3", answer_desc_str))
				{
					*err = path + ": xml node error in [question_list->question->answer_desc_3]";
					return false;
				}
				if (answer_desc_str.length() >= sizeof(QuestionStr))
				{
					*err = path + ": xml node error in [question_list->question->answer_desc_3 too long]";
					return false;
				}
				memcpy(question_cfg.answer_desc_3, answer_desc_str.c_str(), (int)answer_desc_str.length());
				question_cfg.answer_desc_3[answer_desc_str.length()] = '\0';
			}

			if (!PugiGetSubNodeValue(questionElement, "answer", question_cfg.answer) || question_cfg.answer < 0)
			{
				*err = path + ": xml node error in [question_list->question->id]";
				return false;
			}

			if (QUESTION_TYPE_HOTSPRING == question_type)
			{
				m_hotspring_question_map[question_id] = question_cfg;
			}
			else if (QUESTION_TYPE_WORLD == question_type)
			{
				m_world_question_map[question_id] = question_cfg;
			}
			else if (QUESTION_TYPE_GUILD == question_type)
			{
				m_guild_question_map[question_id] = question_cfg;
			}

			questionElement = questionElement.next_sibling();
		}
	}

	{
		PugiXmlNode rewardListElement = RootElement.child("reward");
		if (rewardListElement.empty())
		{
			*err = path + ": xml node error in [reward_list]";
			return false;
		}		

		PugiXmlNode rewardElement = rewardListElement.child("data");
		if (rewardElement.empty())
		{
			*err = path + ": xml node error in [reward_list->reward]";
			return false;
		}

		int last_level = 0;
		while (!rewardElement.empty())
		{
			int level = 0;
			if (!PugiGetSubNodeValue(rewardElement, "rolelevel", level) || level != last_level + 1|| last_level > MAX_ROLE_LEVEL)
			{
				*err = path + ": xml node error in [reward_list->reward->rolelevel]";
				return false;
			}

			if (!PugiGetSubNodeValue(rewardElement, "exp", m_reward_list[level].exp) || m_reward_list[level].exp < 0)
			{
				*err = path + ": xml node error in [reward_list->reward->exp]";
				return false;
			}

			if (!PugiGetSubNodeValue(rewardElement, "yuanli", m_reward_list[level].yuanli) || m_reward_list[level].yuanli < 0)
			{
				*err = path + ": xml node error in [reward_list->reward->yuanli]";
				return false;
			}

			if (!PugiGetSubNodeValue(rewardElement, "xianhun", m_reward_list[level].xianhun) || m_reward_list[level].xianhun < 0)
			{
				*err = path + ": xml node error in [reward_list->reward->xianhun]";
				return false;
			}

			if (!PugiGetSubNodeValue(rewardElement, "gold_answer_per_question", m_reward_list[level].batch_answser_cost_per_question) 
				|| m_reward_list[level].batch_answser_cost_per_question <= 0)
			{
				*err = path + ": xml node error in [reward_list->reward->gold_answer_per_question]";
				return false;
			}

			if (!PugiGetSubNodeValue(rewardElement, "wrong_answer_percentage", m_reward_list[level].wrong_answer_percentage) 
				|| m_reward_list[level].wrong_answer_percentage < 0 || m_reward_list[level].wrong_answer_percentage > 100)
			{
				*err = path + ": xml node error in [reward_list->reward->wrong_answer_percentage]";
				return false;
			}

			{
				PugiXmlNode element = rewardElement.child("reward_item_list");
				if (element.empty())
				{
					*err = path + ": xml node error in [reward_list->reward->reward_item]";
					return false;
				}

				int i = 0;
				PugiXmlNode item_element = element.child("reward_item");
				while (!item_element.empty())
				{
					if (i >= MAX_ATTACHMENT_ITEM_NUM)
					{
						*err = path + ": xml node error in [reward_list->reward->reward_item]";
						return false;
					}

					if (!m_reward_list[level].reward_item[i].ReadConfig(item_element))
					{
						if (m_reward_list[level].reward_item[i].item_id == 0) break;

						*err = path + ": xml node error in [reward_list->reward->reward_item]";
						return false;
					}

					i++;
					item_element = item_element.next_sibling();
				}
			}

			last_level = level;
			rewardElement = rewardElement.next_sibling();
		}
	}

	{
		PugiXmlNode otherListElement = RootElement.child("other");
		if (otherListElement.empty())
		{
			*err = path + ": xml node error in [other_list]";
			return false;
		}		

		PugiXmlNode otherElement = otherListElement.child("data");
		if (otherElement.empty())
		{
			*err = path + ": xml node error in [other_list->other]";
			return false;
		}
		
		if (!PugiGetSubNodeValue(otherElement, "scene_id", m_other_cfg.scene_id) || m_other_cfg.scene_id <= 0)
		{
			*err = path + ": xml node error in [other_list->other->scene_id]";
			return false;
		}

		CheckResourceCenter::Instance().GetSceneCheck()->Add(m_other_cfg.scene_id);
		
		if (!PugiGetSubNodeValue(otherElement, "realive_pos_x", m_other_cfg.m_realive_area.x) || m_other_cfg.m_realive_area.x < 0)
		{
			*err = path + ": xml node error in [other_list->other->realive_pos_x]";
			return false;
		}

		if (!PugiGetSubNodeValue(otherElement, "realive_pos_y", m_other_cfg.m_realive_area.y) || m_other_cfg.m_realive_area.y < 0)
		{
			*err = path + ": xml node error in [other_list->other->realive_pos_y]";
			return false;
		}

		if (!PugiGetSubNodeValue(otherElement, "mianda_card", m_other_cfg.mianda_card_id) || NULL == ITEMPOOL->GetItem(m_other_cfg.mianda_card_id))
		{
			*err = path + ": xml node error in [other_list->other->mianda_card_id]";
			return false;
		}
		
		if (!PugiGetSubNodeValue(otherElement, "bianshen_card", m_other_cfg.bianshen_card_id) || NULL == ITEMPOOL->GetItem(m_other_cfg.bianshen_card_id))
		{
			*err = path + ": xml node error in [other_list->other->bianshen_card_id]";
			return false;
		}

		if (!PugiGetSubNodeValue(otherElement, "pig_slow_down_per", m_other_cfg.pig_slow_down_per) || m_other_cfg.pig_slow_down_per < 0)
		{
			*err = path + ": xml node error in [other_list->other->pig_slow_down_per]";
			return false;
		}

		if (!PugiGetSubNodeValue(otherElement, "rabbit_speed_up_per", m_other_cfg.rabbit_speed_up_per) || m_other_cfg.rabbit_speed_up_per < 0)
		{
			*err = path + ": xml node error in [other_list->other->rabbit_slow_dwon_per]";
			return false;
		}

		if (!PugiGetSubNodeValue(otherElement, "bianshen_pro", m_other_cfg.bianshen_pro) || m_other_cfg.bianshen_pro < 0)
		{
			*err = path + ": xml node error in [other_list->other->bianshen_pro]";
			return false;
		}

		if (!PugiGetSubNodeValue(otherElement, "rabbit_pro", m_other_cfg.rabbit_pro) || m_other_cfg.rabbit_pro < 0)
		{
			*err = path + ": xml node error in [other_list->other->rabbit_pro]";
			return false;
		}

		if (!PugiGetSubNodeValue(otherElement, "mianda_pro", m_other_cfg.mianda_pro) || m_other_cfg.mianda_pro < 0)
		{
			*err = path + ": xml node error in [other_list->other->mianda_pro]";
			return false;
		}

		if (!PugiGetSubNodeValue(otherElement, "activity_prepare_time", m_other_cfg.activity_prepare_time) || m_other_cfg.activity_prepare_time <= 0)
		{
			*err = path + ": xml node error in [other_list->other->activity_prepare_time]";
			return false;
		}

		if (!PugiGetSubNodeValue(otherElement, "answer_prepare_time", m_other_cfg.answer_prepare_time) || m_other_cfg.answer_prepare_time <= 0)
		{
			*err = path + ": xml node error in [other_list->other->answer_prepare_time]";
			return false;
		}

		if (!PugiGetSubNodeValue(otherElement, "answer_continue_time", m_other_cfg.answer_continue_time) || m_other_cfg.answer_continue_time <= 0)
		{
			*err = path + ": xml node error in [other_list->other->answer_continue_time]";
			return false;
		}

		if (!PugiGetSubNodeValue(otherElement, "bianshen_continue_time", m_other_cfg.bianshen_continue_time) || m_other_cfg.bianshen_continue_time <= 0)
		{
			*err = path + ": xml node error in [other_list->other->bianshen_continue_time]";
			return false;
		}

		if (!PugiGetSubNodeValue(otherElement, "total_question_count", m_other_cfg.total_question_count) || m_other_cfg.total_question_count <= 0)
		{
			*err = path + ": xml node error in [other_list->other->total_question_count]";
			return false;
		}

		{
			PugiXmlNode element = otherElement.child("common_reward_list");
			if (element.empty())
			{
				*err = path + ": xml node error in [other_list->other->common_reward_list]";
				return false;
			}

			int i = 0;
			PugiXmlNode item_element = element.child("common_reward");
			while (!item_element.empty())
			{
				if (i >= MAX_ATTACHMENT_ITEM_NUM)
				{
					*err = path + ": xml node error in [other_list->other->common_reward]";
					return false;
				}

				if (!m_other_cfg.common_item[i].ReadConfig(item_element))
				{
					if (m_other_cfg.common_item[i].item_id == 0) break;

					*err = path + ": xml node error in [other_list->other->common_reward]";
					return false;
				}

				i++;
				item_element = item_element.next_sibling();
			}
		}

		{
			PugiXmlNode element = otherElement.child("guild_rank_other_reward_list");
			if (element.empty())
			{
				*err = path + ": xml node error in [other_list->other->guild_rank_other_reward_list]";
				return false;
			}

			int i = 0;
			PugiXmlNode item_element = element.child("guild_rank_other_reward");
			while (!item_element.empty())
			{
				if (i >= MAX_ATTACHMENT_ITEM_NUM)
				{
					*err = path + ": xml node error in [other_list->other->guild_rank_other_reward]";
					return false;
				}

				if (!m_other_cfg.guild_rank_other_reward[i].ReadConfig(item_element))
				{
					if (m_other_cfg.guild_rank_other_reward[i].item_id == 0) break;

					*err = path + ": xml node error in [other_list->other->common_reward]";
					return false;
				}

				i++;
				item_element = item_element.next_sibling();
			}

			if (!PugiGetSubNodeValue(otherElement, "right_world_factor", m_other_cfg.right_world_factor) || m_other_cfg.right_world_factor < 0)
			{
				*err = path + ": xml node error in [other_list->other->right_world_factor]";
				return false;
			}

			if (!PugiGetSubNodeValue(otherElement, "wrong_world_factor", m_other_cfg.wrong_world_factor) || m_other_cfg.wrong_world_factor < 0)
			{
				*err = path + ": xml node error in [other_list->other->wrong_world_factor]";
				return false;
			}

			if (!PugiGetSubNodeValue(otherElement, "right_hotspring_factor", m_other_cfg.right_hotspring_factor) || m_other_cfg.right_hotspring_factor < 0)
			{
				*err = path + ": xml node error in [other_list->other->right_hotspring_factor]";
				return false;
			}

			if (!PugiGetSubNodeValue(otherElement, "wrong_hotspring_factor", m_other_cfg.wrong_hotspring_factor) || m_other_cfg.wrong_hotspring_factor < 0)
			{
				*err = path + ": xml node error in [other_list->other->wrong_hotspring_factor]";
				return false;
			}
		}
	}

	{
		PugiXmlNode rankListElement = RootElement.child("rank_reward");
		if (rankListElement.empty())
		{
			*err = path + ": xml node error in [rank_reward]";
			return false;
		}		

		PugiXmlNode rankElement = rankListElement.child("data");
		if (rankElement.empty())
		{
			*err = path + ": xml node error in [rank_reward_list->rank_reward]";
			return false;
		}

		int rank_last = 0;
		while (!rankElement.empty())
		{
			int rank = 0;
			if (!PugiGetSubNodeValue(rankElement, "rank", rank) || rank < 0 || rank < rank_last)
			{
				*err = path + ": xml node error in [rank_reward_list->scene_id]";
				return false;
			}
			
			HSQuestionRankRewardCfg rank_reward_cfg;
			rank_reward_cfg.rank = rank;
			rank_last = rank;

			{
				PugiXmlNode element = rankElement.child("rank_reward_list");
				if (element.empty())
				{
					*err = path + ": xml node error in [rank_reward_list->rank_reward_list]";
					return false;
				}

				int i = 0;
				PugiXmlNode item_element = element.child("rank_reward");
				while (!item_element.empty())
				{
					if (i >= MAX_ATTACHMENT_ITEM_NUM)
					{
						*err = path + ": xml node error in [rank_reward_list->rank_reward]";
						return false;
					}

					if (!rank_reward_cfg.rank_reward_item[i].ReadConfig(item_element))
					{
						if (rank_reward_cfg.rank_reward_item[i].item_id == 0) break;
				
						*err = path + ": xml node error in [rank_reward_list->rank_reward]";
						return false;
					}

					i++;
					item_element = item_element.next_sibling();
				}
			}

			m_question_rank_reward[rank] = rank_reward_cfg;

			rankElement = rankElement.next_sibling();
		}
	}

	{
		PugiXmlNode WGQuestionElement = RootElement.child("wg_question");
		if (WGQuestionElement.empty())
		{
			*err = path + ": xml node error in [wg_question]";
			return false;
		}

		PugiXmlNode WGElement = WGQuestionElement.child("data");
		if (WGElement.empty())
		{
			*err = path + ": xml node error in [wg_question]";
			return false;
		}

		while (!WGElement.empty())
		{
			int question_type = QUESTION_TYPE_INVALID;
			if (!PugiGetSubNodeValue(WGElement, "question_type", question_type) || question_type <= QUESTION_TYPE_INVALID || question_type >= QUESTION_TYPE_MAX)
			{
				*err = path + ": xml node error in [wg_question->question_type]";
				return false;
			}

			WGQuestionCfg &cfg = m_wg_question_cfg[question_type];
			cfg.question_type = question_type;

			if (!PugiGetSubNodeValue(WGElement, "open_level", cfg.open_level) || cfg.open_level <= 0 || cfg.open_level > MAX_ROLE_LEVEL)
			{
				*err = path + ": xml node error in [wg_question->open_level]";
				return false;
			}

			if (!PugiGetSubNodeValue(WGElement, "begin_time", cfg.begin_time) || cfg.begin_time < 0)
			{
				*err = path + ": xml node error in [wg_question->begin_time]";
				return false;
			}

			if (!PugiGetSubNodeValue(WGElement, "end_time", cfg.end_time) || cfg.end_time < 0)
			{
				*err = path + ": xml node error in [wg_question->end_time]";
				return false;
			}

			if (!PugiGetSubNodeValue(WGElement, "question_interval", cfg.question_interval) || cfg.question_interval < 0)
			{
				*err = path + ": xml node error in [wg_question->question_interval]";
				return false;
			}

			if (!PugiGetSubNodeValue(WGElement, "answer_continue_time", cfg.answer_continue_time) || cfg.answer_continue_time < 0)
			{
				*err = path + ": xml node error in [wg_question->answer_continue_time]";
				return false;
			}

			if (!PugiGetSubNodeValue(WGElement, "right_exp_reward", cfg.right_exp_reward) || cfg.right_exp_reward < 0)
			{
				*err = path + ": xml node error in [wg_question->right_exp_reward]";
				return false;
			}

			if (!PugiGetSubNodeValue(WGElement, "wrong_exp_reward", cfg.wrong_exp_reward) || cfg.wrong_exp_reward < 0)
			{
				*err = path + ": xml node error in [wg_question->wrong_exp_reward]";
				return false;
			}

			if (!PugiGetSubNodeValue(WGElement, "fast_reward_num", cfg.fast_reward_num) || cfg.fast_reward_num < 0)
			{
				*err = path + ": xml node error in [wg_question->fast_reward_num]";
				return false;
			}

			{
				PugiXmlNode element = WGElement.child("fast_reward_list");
				if (element.empty())
				{
					*err = path + ": xml node error in [wg_question->fast_reward_list]";
					return false;
				}

				int i = 0;
				PugiXmlNode item_element = element.child("fast_reward");
				while (!item_element.empty())
				{
					if (i >= MAX_ATTACHMENT_ITEM_NUM)
					{
						*err = path + ": xml node error in [wg_question->fast_reward]";
						return false;
					}

					if (!cfg.fast_reward[i].ReadConfig(item_element))
					{
						if (cfg.fast_reward[i].item_id == 0) break;

						*err = path + ": xml node error in [wg_question->fast_reward]";
						return false;
					}

					i++;
					item_element = item_element.next_sibling();
				}
			}

			WGElement = WGElement.next_sibling();
		}
	}

		{
			PugiXmlNode QGuildRankElement = RootElement.child("g_rank_reward");
			if (QGuildRankElement.empty())
			{
				*err = path + ": xml node error in [g_rank_reward]";
				return false;
			}

			PugiXmlNode GuildRankElement = QGuildRankElement.child("data");
			if (GuildRankElement.empty())
			{
				*err = path + ": xml node error in [g_rank_reward]";
				return false;
			}

			while (!GuildRankElement.empty())
			{
				int rank = 0;
				if (!PugiGetSubNodeValue(GuildRankElement, "rank", rank) || rank <= 0 || m_question_guild_rank_reward.find(rank) != m_question_guild_rank_reward.end())
				{
					*err = path + ": xml node error in [g_rank_reward->rank]";
					return false;
				}

				QuestionGuildRankRewardCfg &cfg = m_question_guild_rank_reward[rank];
				cfg.rank = rank;

				{
					PugiXmlNode element = GuildRankElement.child("rank_reward_list");
					if (element.empty())
					{
						*err = path + ": xml node error in [g_rank_reward->rank_reward_list]";
						return false;
					}

					int i = 0;
					PugiXmlNode item_element = element.child("rank_reward");
					while (!item_element.empty())
					{
						if (i >= MAX_ATTACHMENT_ITEM_NUM)
						{
							*err = path + ": xml node error in [g_rank_reward->rank_reward]";
							return false;
						}

						if (!cfg.rank_reward[i].ReadConfig(item_element))
						{
							if (cfg.rank_reward[i].item_id == 0) break;

							*err = path + ": xml node error in [g_rank_reward->rank_reward]";
							return false;
						}

						i++;
						item_element = item_element.next_sibling();
					}
				}

				GuildRankElement = GuildRankElement.next_sibling();
			}
	}
	
	return true;
}

int QuestionConfig::GetQuestionEnterInfo(Posi *pos)
{
	if (NULL == pos) return 0;
	*pos = m_other_cfg.m_realive_area;
	return m_other_cfg.scene_id;
}

int QuestionConfig::RandQuestionID(int *answer, int question_type)
{
	if (question_type <= QUESTION_TYPE_INVALID || question_type >= QUESTION_TYPE_MAX) return 0;

	switch (question_type)
	{
	case QUESTION_TYPE_HOTSPRING:
	{
		if (m_hotspring_question_map.empty()) return 0;

		std::map<int, QuestionCfg>::iterator it = m_hotspring_question_map.begin();
		int rand_index = RandomNum(static_cast<int>(m_hotspring_question_map.size()));
		std::advance(it, rand_index);

		if (NULL != answer) *answer = it->second.answer;

		return it->first;
	}
	break;

	case QUESTION_TYPE_WORLD:
	{
		if (m_world_question_map.empty()) return 0;

		std::map<int, QuestionCfg>::iterator it = m_world_question_map.begin();
		int rand_index = RandomNum(static_cast<int>(m_world_question_map.size()));
		std::advance(it, rand_index);

		if (NULL != answer) *answer = it->second.answer;

		return it->first;
	}
	break;

	case QUESTION_TYPE_GUILD:
	{
		if (m_guild_question_map.empty()) return 0;

		std::map<int, QuestionCfg>::iterator it = m_guild_question_map.begin();
		int rand_index = RandomNum(static_cast<int>(m_guild_question_map.size()));
		std::advance(it, rand_index);

		if (NULL != answer) *answer = it->second.answer;

		return it->first;
	}
	break;

	default:
		break;
	}

	return 0;
}

void QuestionConfig::GetQuestionStr(int question_id, QuestionStr question_str, int question_type)
{
	question_str[0] = 0;

	switch (question_type)
	{
	case QUESTION_TYPE_HOTSPRING:
	{
		std::map<int, QuestionCfg>::iterator it = m_hotspring_question_map.find(question_id);
		if (it != m_hotspring_question_map.end())
		{
			F_STRNCPY(question_str, it->second.question_str, sizeof(QuestionStr));
		}
	}
	break;

	case QUESTION_TYPE_WORLD:
	{
		std::map<int, QuestionCfg>::iterator it = m_world_question_map.find(question_id);
		if (it != m_world_question_map.end())
		{
			F_STRNCPY(question_str, it->second.question_str, sizeof(QuestionStr));
		}
	}
	break;

	case QUESTION_TYPE_GUILD:
	{
		std::map<int, QuestionCfg>::iterator it = m_guild_question_map.find(question_id);
		if (it != m_guild_question_map.end())
		{
			F_STRNCPY(question_str, it->second.question_str, sizeof(QuestionStr));
		}
	}
	break;

	default:
		break;
	}
}

void QuestionConfig::GetAnswer0DescStr(int question_id, QuestionStr answer_desc_str, int question_type)
{
	answer_desc_str[0] = 0;

	switch (question_type)
	{
	case QUESTION_TYPE_HOTSPRING:
	{
		std::map<int, QuestionCfg>::iterator it = m_hotspring_question_map.find(question_id);
		if (it != m_hotspring_question_map.end())
		{
			F_STRNCPY(answer_desc_str, it->second.answer_desc_0, sizeof(QuestionStr));
		}
	}
	break;

	case QUESTION_TYPE_WORLD:
	{
		std::map<int, QuestionCfg>::iterator it = m_world_question_map.find(question_id);
		if (it != m_world_question_map.end())
		{
			F_STRNCPY(answer_desc_str, it->second.answer_desc_0, sizeof(QuestionStr));
		}
	}
	break;

	case QUESTION_TYPE_GUILD:
	{
		std::map<int, QuestionCfg>::iterator it = m_guild_question_map.find(question_id);
		if (it != m_guild_question_map.end())
		{
			F_STRNCPY(answer_desc_str, it->second.answer_desc_0, sizeof(QuestionStr));
		}
	}
	break;

	default:
		break;
	}
}

void QuestionConfig::GetAnswer1DescStr(int question_id, QuestionStr answer_desc_str, int question_type)
{
	answer_desc_str[0] = 0;

	switch (question_type)
	{
	case QUESTION_TYPE_HOTSPRING:
	{
		std::map<int, QuestionCfg>::iterator it = m_hotspring_question_map.find(question_id);
		if (it != m_hotspring_question_map.end())
		{
			F_STRNCPY(answer_desc_str, it->second.answer_desc_1, sizeof(QuestionStr));
		}
	}
	break;

	case QUESTION_TYPE_WORLD:
	{
		std::map<int, QuestionCfg>::iterator it = m_world_question_map.find(question_id);
		if (it != m_world_question_map.end())
		{
			F_STRNCPY(answer_desc_str, it->second.answer_desc_1, sizeof(QuestionStr));
		}
	}
	break;

	case QUESTION_TYPE_GUILD:
	{
		std::map<int, QuestionCfg>::iterator it = m_guild_question_map.find(question_id);
		if (it != m_guild_question_map.end())
		{
			F_STRNCPY(answer_desc_str, it->second.answer_desc_1, sizeof(QuestionStr));
		}
	}
	break;

	default:
		break;
	}
}

void QuestionConfig::GetAnswer2DescStr(int question_id, QuestionStr answer_desc_str, int question_type)
{
	answer_desc_str[0] = 0;

	switch (question_type)
	{
	case QUESTION_TYPE_HOTSPRING:
	{
		std::map<int, QuestionCfg>::iterator it = m_hotspring_question_map.find(question_id);
		if (it != m_hotspring_question_map.end())
		{
			F_STRNCPY(answer_desc_str, it->second.answer_desc_2, sizeof(QuestionStr));
		}
	}
	break;

	case QUESTION_TYPE_WORLD:
	{
		std::map<int, QuestionCfg>::iterator it = m_world_question_map.find(question_id);
		if (it != m_world_question_map.end())
		{
			F_STRNCPY(answer_desc_str, it->second.answer_desc_2, sizeof(QuestionStr));
		}
	}
	break;

	case QUESTION_TYPE_GUILD:
	{
		std::map<int, QuestionCfg>::iterator it = m_guild_question_map.find(question_id);
		if (it != m_guild_question_map.end())
		{
			F_STRNCPY(answer_desc_str, it->second.answer_desc_2, sizeof(QuestionStr));
		}
	}
	break;

	default:
		break;
	}
}

void QuestionConfig::GetAnswer3DescStr(int question_id, QuestionStr answer_desc_str, int question_type)
{
	answer_desc_str[0] = 0;

	switch (question_type)
	{
	case QUESTION_TYPE_HOTSPRING:
	{
		std::map<int, QuestionCfg>::iterator it = m_hotspring_question_map.find(question_id);
		if (it != m_hotspring_question_map.end())
		{
			F_STRNCPY(answer_desc_str, it->second.answer_desc_3, sizeof(QuestionStr));
		}
	}
	break;

	case QUESTION_TYPE_WORLD:
	{
		std::map<int, QuestionCfg>::iterator it = m_world_question_map.find(question_id);
		if (it != m_world_question_map.end())
		{
			F_STRNCPY(answer_desc_str, it->second.answer_desc_3, sizeof(QuestionStr));
		}
	}
	break;

	case QUESTION_TYPE_GUILD:
	{
		std::map<int, QuestionCfg>::iterator it = m_guild_question_map.find(question_id);
		if (it != m_guild_question_map.end())
		{
			F_STRNCPY(answer_desc_str, it->second.answer_desc_3, sizeof(QuestionStr));
		}
	}
	break;

	default:
		break;
	}
}

int QuestionConfig::GetAnswer(int question_id, int question_type)
{
	switch (question_type)
	{
	case QUESTION_TYPE_HOTSPRING:
	{
		std::map<int, QuestionCfg>::iterator it = m_hotspring_question_map.find(question_id);
		if (it != m_hotspring_question_map.end())
		{
			return it->second.answer;
		}
	}
	break;

	case QUESTION_TYPE_WORLD:
	{
		std::map<int, QuestionCfg>::iterator it = m_world_question_map.find(question_id);
		if (it != m_world_question_map.end())
		{
			return it->second.answer;
		}
	}
	break;

	case QUESTION_TYPE_GUILD:
	{
		std::map<int, QuestionCfg>::iterator it = m_guild_question_map.find(question_id);
		if (it != m_guild_question_map.end())
		{
			return it->second.answer;
		}
	}
	break;

	default:
		break;
	}

	return 0;
}

int QuestionConfig::HotSpringCalcScore(int answer_time)
{
	const static int LIMIT_ANSWER_TIME = 15;
	const static int FULL_SCORRE_MAX_TIME = 5;
	int score = 0;
	
	if (answer_time >= LIMIT_ANSWER_TIME)
	{
		return score;
	}

	if (answer_time <= FULL_SCORRE_MAX_TIME)
	{
		score = 10;
	}
	else
	{
		score = LIMIT_ANSWER_TIME - answer_time;
	}

	return score;
}

const HSQuestionRewardCfg* QuestionConfig::GetHotSpringRewardCfg(int role_level)
{
	if (0 < role_level && role_level <= MAX_ROLE_LEVEL)
	{
		return &m_reward_list[role_level];
	}

	return NULL;
}

const HSQuestionRankRewardCfg * QuestionConfig::GetHotSpringRankRewardCfg(int rank)
{
	if (rank < 0) return NULL;

	const std::map<int, HSQuestionRankRewardCfg>::const_iterator iter = m_question_rank_reward.find(rank);
	if (m_question_rank_reward.end() == iter)
		return NULL;

	return &iter->second;
}

const WGQuestionCfg * QuestionConfig::GetWGQuestionCfg(int question_type)
{
	if (question_type <= QUESTION_TYPE_INVALID || question_type >= QUESTION_TYPE_MAX) return NULL;

	WGQuestionCfg &cfg = m_wg_question_cfg[question_type];
	if (cfg.question_type != question_type)
	{
		return NULL;
	}

	return &cfg;
}

QuestionGuildRankRewardCfg * QuestionConfig::GetQuestionGuildRankReward(int rank)
{
	std::map<int, QuestionGuildRankRewardCfg>::iterator iter = m_question_guild_rank_reward.find(rank);
	if (iter != m_question_guild_rank_reward.end() && iter->second.rank == rank)
	{
		return &iter->second;
	}

	return NULL;
}