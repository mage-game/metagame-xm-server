#include "crossmultiuserchallengeconfig.hpp"
#include "servercommon/configcommon.h"

CrossMultiUserChallengeConfig::CrossMultiUserChallengeConfig() 
	: m_join_min_level(0), m_join_min_capability(0), m_join_day_count_limit(0), m_match_scene_id(0), 
	m_finish_match_score(0), m_kill_add_score(0), m_occupy_add_score(0), m_stronghold_add_score(0), m_stronghold_gather_id(0), 
	m_win_add_honor(0), m_lose_add_honor(0), m_draw_add_honor(0), m_mvp_add_honor(0), m_day_reward_honor(0), m_reward_daycount(0), m_daycount_reward_shengwang(0)
{
	memset(m_stronghold_pos_list, 0, sizeof(m_stronghold_pos_list));
	memset(m_relive_pos_list, 0, sizeof(m_relive_pos_list));
}

CrossMultiUserChallengeConfig::~CrossMultiUserChallengeConfig()
{

}

CrossMultiUserChallengeConfig & CrossMultiUserChallengeConfig::Instance()
{
	static CrossMultiUserChallengeConfig cmucc;
	return cmucc;
}

bool CrossMultiUserChallengeConfig::Init(const std::string &path, std::string *err)
{
	char errinfo[1024] = {0};

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "CrossMultiUserChallengeConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = path + ": xml root node error.";
		return false;
	}

	{
		PugiXmlNode ActivityTimeCfgElement = RootElement.child("activity_time_cfg");
		if (ActivityTimeCfgElement.empty())
		{
			*err = path + ": xml node error in [config->activity_time_cfg]";
			return false;
		}

		if (!PugiGetSubNodeValue(ActivityTimeCfgElement, "is_open", m_activity_time_cfg.is_open))
		{
			*err = path + ": xml node error in [config->activity_time_cfg->is_open]";
			return false;
		}

		PugiXmlNode OpenDayListElement = ActivityTimeCfgElement.child("open_day_list");
		if (OpenDayListElement.empty())
		{
			*err = path + ": xml node error in [config->activity_time_cfg->open_day_list]";
			return false;
		}

		PugiXmlNode OpenDayElement = OpenDayListElement.child("open_day");
		if (OpenDayElement.empty())
		{
			*err = path + ": xml node error in [config->activity_time_cfg->open_day_list->open_day]";
			return false;
		}

		while (!OpenDayElement.empty())
		{
			int open_day = 0;
			if (!PugiGetNodeValue(OpenDayElement, open_day) || open_day <= 0 || open_day > CROSS_ACTIVITY_1V1_MAX_WEEK_DAYS)
			{
				*err = path + ": xml node error in [config->activity_time_cfg->open_day_list->open_day]";
				return false;
			}

			open_day = open_day % CROSS_ACTIVITY_1V1_MAX_WEEK_DAYS;
			if (open_day < 0 || open_day >= CROSS_ACTIVITY_1V1_MAX_WEEK_DAYS)
			{
				*err = path + ": xml node error in [config->activity_time_cfg->open_day_list->open_day]";
				return false;
			}

			m_activity_time_cfg.open_day_list[open_day] = true;

			OpenDayElement = OpenDayElement.next_sibling();
		}

		PugiXmlNode IntervalElement = ActivityTimeCfgElement.child("time_interval");
		if (IntervalElement.empty())
		{
			*err = path + ": xml node error in [config->activity_time_cfg->time_interval]";
			return false;
		}

		int hour = 0, minute = 0, second = 0, standby_dur_s = 0, open_dur_s = 0;
		if (!PugiGetSubNodeValue(IntervalElement, "hour", hour) || hour < 0 || hour >= 24)
		{
			*err = path + ": xml node error in [config->activity_time_cfg->time_interval->hour]";
			return false;
		}
		if (!PugiGetSubNodeValue(IntervalElement, "minute", minute) || minute < 0 || minute >= 60)
		{
			*err = path + ": xml node error in [config->activity_time_cfg->time_interval->minute]";
			return false;
		}
		if (!PugiGetSubNodeValue(IntervalElement, "second", second) || second < 0 || second >= 60)
		{
			*err = path + ": xml node error in [config->activity_time_cfg->time_interval->second]";
			return false;
		}
		if (!PugiGetSubNodeValue(IntervalElement, "standby_dur_s", standby_dur_s) || standby_dur_s <= 0)
		{
			*err = path + ": xml node error in [config->activity_time_cfg->time_interval->standby_dur_s]";
			return false;
		}
		if (!PugiGetSubNodeValue(IntervalElement, "open_dur_s", open_dur_s) || open_dur_s <= 0)
		{
			*err = path + ": xml node error in [config->activity_time_cfg->time_interval->open_dur_s]";
			return false;
		}

		m_activity_time_cfg.open_day_second = hour * 3600 + minute * 60 + second;
		m_activity_time_cfg.standby_dur_s = standby_dur_s;
		m_activity_time_cfg.open_dur_s = open_dur_s;
	}

	{
		PugiXmlNode JoinLimitElement = RootElement.child("join_limit");
		if (JoinLimitElement.empty())
		{
			*err = path + ": xml node error in [config->join_limit]";
			return false;
		}

		if (!PugiGetSubNodeValue(JoinLimitElement, "min_level", m_join_min_level) || m_join_min_level <= 0)
		{
			*err = path + ": xml node error in [config->join_limit->min_level]";
			return false;
		}
		if (!PugiGetSubNodeValue(JoinLimitElement, "min_capability", m_join_min_capability) || m_join_min_capability <= 0)
		{
			*err = path + ": xml node error in [config->join_limit->min_capability]";
			return false;
		}
		if (!PugiGetSubNodeValue(JoinLimitElement, "daycount_limit", m_join_day_count_limit) || m_join_day_count_limit <= 0)
		{
			*err = path + ": xml node error in [config->join_limit->daycount_limit]";
			return false;
		}
	}

	{
		if (!PugiGetSubNodeValue(RootElement, "match_scene_id", m_match_scene_id) || m_match_scene_id <= 0)
		{
			*err = path + ": xml node error in [config->match_scene_id]";
			return false;
		}

		PugiXmlNode PosListElement = RootElement.child("relive_pos_list");
		if (PosListElement.empty())
		{
			*err = path + ": xml node error in [config->relive_pos_list]";
			return false;
		}

		for (int i = 0; i < CROSS_MULTIUSER_CHALLENGE_SIDE_MAX; ++ i)
		{
			char temp_name[64] = {0};
			SNPRINTF(temp_name, sizeof(temp_name), "relive_pos_%d", i);

			PugiXmlNode PosElement = PosListElement.child(temp_name);
			if (PosElement.empty())
			{
				*err = path + ": xml node error in [config->relive_pos_list->" + temp_name + "]";
				return false;
			}

			if (!PugiGetSubNodeValue(PosElement, "x", m_relive_pos_list[i].x) || m_relive_pos_list[i].x <= 0)
			{
				*err = path + ": xml node error in [config->relive_pos_list->" + temp_name + "->x]";
				return false;
			}
			if (!PugiGetSubNodeValue(PosElement, "y", m_relive_pos_list[i].y) || m_relive_pos_list[i].y <= 0)
			{
				*err = path + ": xml node error in [config->relive_pos_list->" + temp_name + "->y]";
				return false;
			}
		}
	}

	if (!PugiGetSubNodeValue(RootElement, "finish_match_score", m_finish_match_score) || m_finish_match_score <= 0)
	{
		sprintf(errinfo, "%s :xml node error in [config->finish_match_score]", path.c_str());
		*err = errinfo;
		return false;
	}

	if (!PugiGetSubNodeValue(RootElement, "kill_add_score", m_kill_add_score) || m_kill_add_score <= 0)
	{
		sprintf(errinfo, "%s :xml node error in [config->kill_add_score]", path.c_str());
		*err = errinfo;
		return false;
	}

	if (!PugiGetSubNodeValue(RootElement, "occupy_add_score", m_occupy_add_score) || m_occupy_add_score <= 0)
	{
		sprintf(errinfo, "%s :xml node error in [config->occupy_add_score]", path.c_str());
		*err = errinfo;
		return false;
	}

	if (!PugiGetSubNodeValue(RootElement, "stronghold_add_score", m_stronghold_add_score) || m_stronghold_add_score <= 0)
	{
		sprintf(errinfo, "%s :xml node error in [config->stronghold_add_score]", path.c_str());
		*err = errinfo;
		return false;
	}

	if (!PugiGetSubNodeValue(RootElement, "stronghold_gather_id", m_stronghold_gather_id) || m_stronghold_gather_id <= 0)
	{
		sprintf(errinfo, "%s :xml node error in [config->stronghold_gather_id]", path.c_str());
		*err = errinfo;
		return false;
	}

	if (!PugiGetSubNodeValue(RootElement, "win_add_honor", m_win_add_honor) || m_win_add_honor <= 0)
	{
		sprintf(errinfo, "%s :xml node error in [config->win_add_honor]", path.c_str());
		*err = errinfo;
		return false;
	}

	if (!PugiGetSubNodeValue(RootElement, "lose_add_honor", m_lose_add_honor) || m_lose_add_honor <= 0)
	{
		sprintf(errinfo, "%s :xml node error in [config->lose_add_honor]", path.c_str());
		*err = errinfo;
		return false;
	}

	if (!PugiGetSubNodeValue(RootElement, "draw_add_honor", m_draw_add_honor) || m_draw_add_honor <= 0)
	{
		sprintf(errinfo, "%s :xml node error in [config->draw_add_honor]", path.c_str());
		*err = errinfo;
		return false;
	}

	if (!PugiGetSubNodeValue(RootElement, "mvp_add_honor", m_mvp_add_honor) || m_mvp_add_honor <= 0)
	{
		sprintf(errinfo, "%s :xml node error in [config->mvp_add_honor]", path.c_str());
		*err = errinfo;
		return false;
	}

	if (!PugiGetSubNodeValue(RootElement, "day_reward_honor", m_day_reward_honor) || m_day_reward_honor <= 0)
	{
		sprintf(errinfo, "%s :xml node error in [config->day_reward_honor]", path.c_str());
		*err = errinfo;
		return false;
	}

	{
		PugiXmlNode StrongholdPosListElement = RootElement.child("stronghold_pos_list");
		if (StrongholdPosListElement.empty())
		{
			sprintf(errinfo, "%s :xml node error in [config->stronghold_pos_list]", path.c_str());
			*err = errinfo;
			return false;
		}

		PugiXmlNode StrongholdPosElement = StrongholdPosListElement.child("stronghold_pos");
		if (StrongholdPosElement.empty())
		{
			sprintf(errinfo, "%s :xml node error in [config->stronghold_pos_list->stronghold_pos]", path.c_str());
			*err = errinfo;
			return false;
		}

		int count = 0;
		while (!StrongholdPosElement.empty())
		{
			if (count >= CROSS_MULTIUSER_CHALLENGE_STRONGHOLD_NUM)
			{
				sprintf(errinfo, "%s :xml node error in [config->stronghold_pos_list->stronghold_pos] more than limit", path.c_str());
				*err = errinfo;
				return false;
			}

			if (!PugiGetSubNodeValue(StrongholdPosElement, "pos_x", m_stronghold_pos_list[count].x) || m_stronghold_pos_list[count].x <= 0)
			{
				sprintf(errinfo, "%s :xml node error in [config->stronghold_pos_list->stronghold_pos->pos_x]", path.c_str());
				*err = errinfo;
				return false;
			}

			if (!PugiGetSubNodeValue(StrongholdPosElement, "pos_y", m_stronghold_pos_list[count].y) || m_stronghold_pos_list[count].y <= 0)
			{
				sprintf(errinfo, "%s :xml node error in [config->stronghold_pos_list->stronghold_pos->pos_y]", path.c_str());
				*err = errinfo;
				return false;
			}

			++ count;
			StrongholdPosElement = StrongholdPosElement.next_sibling();
		}

		if (count != CROSS_MULTIUSER_CHALLENGE_STRONGHOLD_NUM)
		{
			sprintf(errinfo, "%s :xml node error in [config->stronghold_pos_list->stronghold_pos] count not enough", path.c_str());
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode ChallengeRankRewardElement = RootElement.child("challenge_score_rank_reward");
		if (ChallengeRankRewardElement.empty())
		{
			*err = path + ": xml node error in [config->challenge_score_rank_reward]";
			return false;
		}

		int count = 0;
		PugiXmlNode RewardDataElement = ChallengeRankRewardElement.child("reward_data");
		while (!RewardDataElement.empty())
		{
			if (count >= CROSS_MULTIUSER_CHALLENGE_SCORE_RANK_REWARD_MAX_COUNT)
			{
				*err = path + ": xml node error in [config->challenge_score_rank_reward->reward_data] more than limit";
				return false;
			}

			if (!PugiGetSubNodeValue(RewardDataElement, "rank_pos_min", m_challenge_score_rank_reward_list[count].rank_pos_min) || m_challenge_score_rank_reward_list[count].rank_pos_min < 0)
			{
				*err = path + ": xml node error in [config->challenge_score_rank_reward->reward_data->rank_pos_min]";	
				return false;
			}

			if ((count == 0 && 0 != m_challenge_score_rank_reward_list[count].rank_pos_min) || (count > 0 && m_challenge_score_rank_reward_list[count].rank_pos_min != m_challenge_score_rank_reward_list[count - 1].rank_pos_max + 1))
			{
				*err = path + ": xml node error in [config->challenge_score_rank_reward->reward_data->rank_pos_min]";
				return false;
			}

			if (!PugiGetSubNodeValue(RewardDataElement, "rank_pos_max", m_challenge_score_rank_reward_list[count].rank_pos_max) || m_challenge_score_rank_reward_list[count].rank_pos_max < m_challenge_score_rank_reward_list[count].rank_pos_min)
			{
				*err = path + ": xml node error in [config->challenge_score_rank_reward->reward_data->rank_pos_max]";
				return false;
			}

			if (!PugiGetSubNodeValue(RewardDataElement, "honor", m_challenge_score_rank_reward_list[count].reward_honor) || m_challenge_score_rank_reward_list[count].reward_honor <= 0)
			{
				*err = path + ": xml node error in [config->challenge_score_rank_reward->reward_data->honor]";
				return false;
			}

			++ count;
			RewardDataElement = RewardDataElement.next_sibling("reward_data");
		}
	}

	{
		PugiXmlNode ChallengeDaycountRewardElement = RootElement.child("challenge_day_count_reward");
		if (ChallengeDaycountRewardElement.empty())
		{
			*err = path + ": xml node error in [config->challenge_day_count_reward]";
			return false;
		}

		if (!PugiGetSubNodeValue(ChallengeDaycountRewardElement, "reward_daycount", m_reward_daycount) || m_reward_daycount <= 0)
		{
			*err = path + ": xml node error in [config->challenge_day_count_reward->reward_daycount]";
			return false;
		}

		if (!PugiGetSubNodeValue(ChallengeDaycountRewardElement, "shengwang", m_daycount_reward_shengwang) || m_daycount_reward_shengwang <= 0)
		{
			*err = path + ": xml node error in [config->challenge_day_count_reward->shengwang]";
			return false;
		}

		if (!PugiGetSubNodeValue(ChallengeDaycountRewardElement, "item_id", m_daycount_reward_item.item_id) || m_daycount_reward_item.item_id <= 0)
		{
			*err = path + ": xml node error in [config->challenge_day_count_reward->item_id]";
			return false;
		}

		if (!PugiGetSubNodeValue(ChallengeDaycountRewardElement, "item_num", m_daycount_reward_item.num) || m_daycount_reward_item.num <= 0 || m_daycount_reward_item.num > 99)
		{
			*err = path + ": xml node error in [config->challenge_day_count_reward->item_num]";
			return false;
		}

		if (!PugiGetSubNodeValue(ChallengeDaycountRewardElement, "is_bind", m_daycount_reward_item.is_bind))
		{
			*err = path + ": xml node error in [config->challenge_day_count_reward->is_bind]";
			return false;
		}
	}

	return true;
}

Posi CrossMultiUserChallengeConfig::GetStrongholdPos(int stronghold_index)
{
	if (stronghold_index < 0 || stronghold_index >= CROSS_MULTIUSER_CHALLENGE_STRONGHOLD_NUM) return Posi(0, 0);
	return m_stronghold_pos_list[stronghold_index];
}

Posi CrossMultiUserChallengeConfig::GetRelivePos(int side) const
{
	if (side >= 0 && side < CROSS_MULTIUSER_CHALLENGE_SIDE_MAX)
	{
		return gamecommon::GetDisRandPos(m_relive_pos_list[side], 3);
	}
	return Posi(0, 0);
}

bool CrossMultiUserChallengeConfig::GetDaycountReward(ItemConfigData *reward_item, int *reward_shengwang)
{
	if (NULL == reward_item || NULL == reward_shengwang) return false;

	*reward_item = m_daycount_reward_item;
	*reward_shengwang = m_daycount_reward_shengwang;

	return true;
}

bool CrossMultiUserChallengeConfig::GetChallengeScoreRankReward(int rank_pos, int *reward_honor)
{
	if (rank_pos < 0 || NULL == reward_honor) return false;

	for (int i = 0; i < CROSS_MULTIUSER_CHALLENGE_SCORE_RANK_REWARD_MAX_COUNT; i++)
	{
		if (m_challenge_score_rank_reward_list[i].Invalid()) continue;

		if (m_challenge_score_rank_reward_list[i].rank_pos_min <= rank_pos && rank_pos <= m_challenge_score_rank_reward_list[i].rank_pos_max)
		{
			*reward_honor = m_challenge_score_rank_reward_list[i].reward_honor;

			return true;
		}
	}

	return false;
}

