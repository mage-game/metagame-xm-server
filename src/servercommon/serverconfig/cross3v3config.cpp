#include "cross3v3config.hpp"
#include "servercommon/configcommon.h"

Cross3v3Config::Cross3v3Config(): m_activity_open_time_cfg_list_count(0), m_max_priority(0)
{
	//memset(m_stronghold_rate_list, 0, sizeof(m_stronghold_rate_list));
}

Cross3v3Config::~Cross3v3Config()
{

}

Cross3v3Config & Cross3v3Config::Instance()
{
	static Cross3v3Config cmucc;
	return cmucc;
}

bool Cross3v3Config::Init(const std::string &configname, std::string *err)
{
	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, configname, "Cross3v3Config", *err);

	PugiXmlNode root_element = document.document_element();
	if (root_element.empty())
	{
		*err = configname + ": xml root node error.";
		return false;
	}

	int iRet= 0 ;
	char errinfo[1024] = { 0 };
	// 其他配置
	PUGI_XML_LOAD_CONFIG("other", InitOtherCfg);
	// 活动开启时间
	PUGI_XML_LOAD_CONFIG("activity_time_cfg", InitActivityOpenTimeCfg);
	// 据点坐标
	PUGI_XML_LOAD_CONFIG("stronghold_rate", InitStrongHoldCfg);
	// 每日参与场次奖励
	PUGI_XML_LOAD_CONFIG("join_times_reward", InitJoinRewardCfg);
	// 功勋奖励
	PUGI_XML_LOAD_CONFIG("score_reward", InitGongxunRewardCfg);
	// 段位积分
	PUGI_XML_LOAD_CONFIG("grade_score", InitDivisionScoreCfg);
	// 排行奖励
	PUGI_XML_LOAD_CONFIG("rank_reward", InitRankRewardCfg);
	// 匹配优先级
	PUGI_XML_LOAD_CONFIG("match_priority", InitPriorityConfig);
	// 额外扣除积分
	PUGI_XML_LOAD_CONFIG("extra_deduct_score", InitDivistionGapExtraScoreCfg);
	// 连胜积分
	PUGI_XML_LOAD_CONFIG("extra_increase_score", InitDurWinCfg);
	// 赛季令牌
	PUGI_XML_LOAD_CONFIG("season_card", InitSeasonRewardCfg);
	// N赛季后奖励
	PUGI_XML_LOAD_CONFIG("after_season_reward", InitAfterSeasonRewardCfg);

	return true;
}

int Cross3v3Config::GetOpenActivityConfig(Cross3v3ActivityOpenTimeInfo cfg_list[CROSS_PASTURE_OPEN_TIME_MAX_COUNT])
{
	memcpy(cfg_list, m_activity_open_time_cfg_list, sizeof(m_activity_open_time_cfg_list));
	return m_activity_open_time_cfg_list_count;
}

Cross3v3ActivityOpenTimeInfo *Cross3v3Config::GetCrossActivityOpenTimeInfo(int index)
{
	if (index >= 0 && index < m_activity_open_time_cfg_list_count && index < CROSS_PASTURE_OPEN_TIME_MAX_COUNT)
	{
		return &m_activity_open_time_cfg_list[index];
	}

	return NULL;
}


const Cross3V3GradeScoreConfig * Cross3v3Config::GetDivisionConfigWithScore(int score)
{
	for (std::vector<Cross3V3GradeScoreConfig>::reverse_iterator cfg_it = m_grade_score_cfg_vec.rbegin(); cfg_it != m_grade_score_cfg_vec.rend(); ++cfg_it)
	{
		if (score >= cfg_it->score)
		{
			return &(*cfg_it);
		}
	}

	return NULL; // 配置里最低配0，如果循环里找不到，说明他的积分是负数了，一般不会
}

Posi Cross3v3Config::GetStrongholdPos(int & radius)
{
	radius = m_other_cfg.stronghold_radius;
	return m_other_cfg.stronghold_pos;
}

int Cross3v3Config::GetStrongholdRate(int num)
{
	if (num < 1 || num > CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT)
	{
		return 0;
	}

	return m_stronghold_rate_list[num - 1].rate;
}

int Cross3v3Config::GetStrongholdRoleRate(int num)
{
	if (num < 1 || num > CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT)
	{
		return 0;
	}

	return m_stronghold_rate_list[num - 1].rate_role;
}

int Cross3v3Config::GetMaxSeason() const
{
	return m_max_season;
}

const Cross3V3GongxunRewardConfig *Cross3v3Config::GetGongxunRewardCfgBySeq(int seq) const
{
	if (seq < 0 || seq > m_gongxun_reward_cfg_max_seq || seq >= CROSS_ACTIVITY_3V3_GONGXUN_CFG_MAX_COUNT)
	{
		return NULL;
	}

	return &m_gongxun_reward_cfg_list[seq];
}

Posi Cross3v3Config::GetRelivePos(int side) const
{
	if (side >= 0 && side < CROSS_MULTIUSER_CHALLENGE_SIDE_MAX)
	{
		return gamecommon::GetDisRandPos(m_other_cfg.relive_pos_list[side], 3);
	}
	return Posi(0, 0);
}

const Cross3v3ScoreRankReward * Cross3v3Config::GetChallengeScoreRankReward(int rank_pos, int score)
{
	if (rank_pos < 0 || score <= 0) return NULL;

	for (int i = 0; i < CROSS_MULTIUSER_CHALLENGE_SCORE_RANK_REWARD_MAX_COUNT; i++)
	{
		if (m_rank_reward_cfg_list[i].Invalid()) continue;

		if (m_rank_reward_cfg_list[i].rank_pos_min <= rank_pos && 
			rank_pos <= m_rank_reward_cfg_list[i].rank_pos_max && 
			score >= m_rank_reward_cfg_list[i].score_limit)
		{
			
			return &m_rank_reward_cfg_list[i];
		}
	}

	return NULL;
}

int Cross3v3Config::GetDivisionDiffExtraScore(int delta_division)
{
	for (std::vector<Cross3V3ExtraScoreConfig>::const_reverse_iterator it = m_extra_score_cfg_vec.rbegin(); it != m_extra_score_cfg_vec.rend(); ++it)
	{
		if (delta_division >= it->gap) return -it->extra_score; // 自己比对方段位高，要扣分
		if (delta_division <= -it->gap) return it->extra_score; // 反之可以加分
	}

	return 0;	// 在配置配的最小值以内，不扣分
}

int Cross3v3Config::GetDurWinExtraScore(int dur_win_count)
{
	for (std::vector<Cross3V3DurWinScoreConfig>::const_reverse_iterator it = m_dur_win_score_cfg_vec.rbegin(); it != m_dur_win_score_cfg_vec.rend(); ++it)
	{
		if (dur_win_count >= it->dur_win_times)
		{
			return it->extra_score;
		}
	}

	return 0;
}

const Cross3v3SeasonRewardConfig *Cross3v3Config::GetSeasonCfgBySeasonGrade(int season, int grade, int pos /* = 0 */) const
{
	if (season <= 0 || season > m_max_season)
	{
		return NULL;
	}

	if (grade >= CROSS_ACTIVITY_3V3_GRADE_TYPE_MAX && pos > 0)
	{
		// 赛季排行令牌. 排行第一名的为1.取小于等于
		for (std::vector<Cross3v3SeasonRewardConfig>::const_reverse_iterator it = m_season_reward_rank_cfg_vec[season - 1].rbegin(); it != m_season_reward_rank_cfg_vec[season - 1].rend(); ++it)
		{
			if (pos <= it->rank)
			{
				return &(*it);
			}
		}

		return NULL;  // 有排名但是没有达成排名名次奖励条件
	}

	// 赛季段位令牌
	for (std::vector<Cross3v3SeasonRewardConfig>::const_iterator it = m_season_reward_grade_cfg_vec[season - 1].begin(); it != m_season_reward_grade_cfg_vec[season - 1].end(); ++it)
	{
		if (it->grade == grade)
		{
			return &(*it);
		}
	}

	return NULL;
}

const Cross3v3SeasonRewardConfig *Cross3v3Config::GetSeasonCfgBySeq(int seq) const
{
	for (Cross3v3SeasonVector::const_iterator it = m_season_reward_grade_cfg_vec.begin(); it != m_season_reward_grade_cfg_vec.end(); ++it)
	{
		for (std::vector<Cross3v3SeasonRewardConfig>::const_iterator it_2 = it->begin(); it_2 != it->end(); ++it_2)
		{
			if (it_2->seq == seq)
			{
				return &(*it_2);
			}
		}
	}
	for (Cross3v3SeasonVector::const_iterator it = m_season_reward_rank_cfg_vec.begin(); it != m_season_reward_rank_cfg_vec.end(); ++it)
	{
		for (std::vector<Cross3v3SeasonRewardConfig>::const_iterator it_2 = it->begin(); it_2 != it->end(); ++it_2)
		{
			if (it_2->seq == seq)
			{
				return &(*it_2);
			}
		}
	}
	
	return NULL;
}

const Cross3v3SeasonRewardConfig *Cross3v3Config::GetAfterSeasonCfgBySeasonGrade(int season, int grade, int pos /* = 0 */) const
{
	if (season <= m_max_season)
	{
		return NULL;
	}

	if (grade >= CROSS_ACTIVITY_3V3_GRADE_TYPE_MAX && pos > 0)
	{
		// 赛季后排行奖励. 排行第一名的为1.取小于等于
		for (auto it = m_after_season_reward_rank_cfg_vec.rbegin(); it != m_after_season_reward_rank_cfg_vec.rend(); ++it)
		{
			if (pos <= it->rank)
			{
				return &(*it);
			}
		}

		return NULL;  // 有排名但是没有达成排名名次奖励条件
	}

	// 赛季后段位奖励
	for (auto it = m_after_season_reward_grade_cfg_vec.begin(); it != m_after_season_reward_grade_cfg_vec.end(); ++it)
	{
		if (it->grade == grade)
		{
			return &(*it);
		}
	}

	return NULL;
}

int Cross3v3Config::InitOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	int is_open = 0;
	if (!PugiGetSubNodeValue(dataElement, "is_open", is_open))
	{
		return -1;
	}
	m_other_cfg.is_open = (is_open != 0);

	if (!PugiGetSubNodeValue(dataElement, "join_limit_min_level", m_other_cfg.join_limit_min_level) || m_other_cfg.join_limit_min_level <= 0)
	{
		return -2;
	}

	if (!PugiGetSubNodeValue(dataElement, "join_limit_min_capability", m_other_cfg.join_limit_min_capability) || m_other_cfg.join_limit_min_capability <= 0)
	{
		return -3;
	}

	if (!PugiGetSubNodeValue(dataElement, "join_limit_daycount", m_other_cfg.join_limit_daycount) || m_other_cfg.join_limit_daycount <= 0)
	{
		return -4;
	}

	if (!PugiGetSubNodeValue(dataElement, "match_scene_id", m_other_cfg.match_scene_id) || m_other_cfg.match_scene_id <= 0)
	{
		return -5;
	}


	for (int i = 0; i < CROSS_MULTIUSER_CHALLENGE_SIDE_MAX; ++ i)
	{
		char temp_name[64] = {0};
		
		SNPRINTF(temp_name, sizeof(temp_name), "relive_pos_%d_x", i);
		if (!PugiGetSubNodeValue(dataElement, temp_name, m_other_cfg.relive_pos_list[i].x) || m_other_cfg.relive_pos_list[i].x <= 0)
		{
			return -11;
		}

		SNPRINTF(temp_name, sizeof(temp_name), "relive_pos_%d_y", i);
		if (!PugiGetSubNodeValue(dataElement, temp_name, m_other_cfg.relive_pos_list[i].y) || m_other_cfg.relive_pos_list[i].y <= 0)
		{
			return -12;
		}
	}

	if (!PugiGetSubNodeValue(dataElement, "finish_match_score", m_other_cfg.finish_match_score) || m_other_cfg.finish_match_score <= 0)
	{
		return -6;
	}

	if (!PugiGetSubNodeValue(dataElement, "kill_add_score", m_other_cfg.kill_add_score) || m_other_cfg.kill_add_score <= 0)
	{
		return -7;
	}

	if (!PugiGetSubNodeValue(dataElement, "occupy_add_score", m_other_cfg.occupy_add_score) || m_other_cfg.occupy_add_score <= 0)
	{
		return -8;
	}

	if (!PugiGetSubNodeValue(dataElement, "stronghold_add_score", m_other_cfg.stronghold_add_score) || m_other_cfg.stronghold_add_score <= 0)
	{
		return -9;
	}

	if (!PugiGetSubNodeValue(dataElement, "exp_dan_type", m_other_cfg.exp_dan_type) || m_other_cfg.exp_dan_type < 0 || m_other_cfg.exp_dan_type > 2)
	{
		return -21;
	}

	if (!PugiGetSubNodeValue(dataElement, "exp_dan_count", m_other_cfg.exp_dan_count) || m_other_cfg.exp_dan_count <= 0)
	{
		return -22;
	}

	if (!PugiGetSubNodeValue(dataElement, "win_exp_per", m_other_cfg.win_exp_per) || m_other_cfg.win_exp_per <= 0)
	{
		return -23;
	}

	if (!PugiGetSubNodeValue(dataElement, "equal_exp_per", m_other_cfg.equal_exp_per) || m_other_cfg.equal_exp_per <= 0)
	{
		return -24;
	}

	if (!PugiGetSubNodeValue(dataElement, "fail_exp_per", m_other_cfg.fail_exp_per) || m_other_cfg.fail_exp_per <= 0)
	{
		return -25;
	}

	if (!PugiGetSubNodeValue(dataElement, "win_shengwang", m_other_cfg.win_shengwang) || m_other_cfg.win_shengwang < 0)
	{
		return -26;
	}

	if (!PugiGetSubNodeValue(dataElement, "win_gongxun", m_other_cfg.win_gongxun) || m_other_cfg.win_gongxun < 0)
	{
		return -27;
	}

	if (!PugiGetSubNodeValue(dataElement, "equal_shengwang", m_other_cfg.equal_shengwang) || m_other_cfg.equal_shengwang < 0)
	{
		return -29;
	}

	if (!PugiGetSubNodeValue(dataElement, "equal_gongxun", m_other_cfg.equal_gongxun) || m_other_cfg.equal_gongxun < 0)
	{
		return -30;
	}

	if (!PugiGetSubNodeValue(dataElement, "fail_shengwang", m_other_cfg.fail_shengwang) || m_other_cfg.fail_shengwang < 0)
	{
		return -32;
	}

	if (!PugiGetSubNodeValue(dataElement, "fail_gongxun", m_other_cfg.fail_gongxun) || m_other_cfg.fail_gongxun < 0)
	{
		return -33;
	}

	if (!PugiGetSubNodeValue(dataElement, "division_need_match_count", m_other_cfg.division_need_match_count) || m_other_cfg.division_need_match_count <= 0)
	{
		return -35;
	}

	if (!PugiGetSubNodeValue(dataElement, "rank_score_limit", m_other_cfg.rank_score_limit) || m_other_cfg.rank_score_limit <= 0)
	{
		return -36;
	}

	if (!PugiGetSubNodeValue(dataElement, "stronghold_x", m_other_cfg.stronghold_pos.x) || m_other_cfg.stronghold_pos.x <= 0)
	{
		return -37;
	}

	if (!PugiGetSubNodeValue(dataElement, "stronghold_y", m_other_cfg.stronghold_pos.y) || m_other_cfg.stronghold_pos.y <= 0)
	{
		return -38;
	}

	if (!PugiGetSubNodeValue(dataElement, "stronghold_radius", m_other_cfg.stronghold_radius) || m_other_cfg.stronghold_radius <= 0)
	{
		return -39;
	}
	if (!PugiGetSubNodeValue(dataElement, "ready_time_s", m_other_cfg.ready_time_s) || m_other_cfg.ready_time_s <= 0)
	{
		return -40;
	}
	if (!PugiGetSubNodeValue(dataElement, "fight_time_s", m_other_cfg.fighting_time_s) || m_other_cfg.fighting_time_s <= 0)
	{
		return -41;
	}
	if (!PugiGetSubNodeValue(dataElement, "season_days", m_other_cfg.season_days) || m_other_cfg.season_days <= 0)
	{
		return -42;
	}
	if (!PugiGetSubNodeValue(dataElement, "open_server_day", m_other_cfg.open_server_day) || m_other_cfg.open_server_day <= 0)
	{
		return -43;
	}

	return 0;
}

int Cross3v3Config::InitActivityOpenTimeCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	int pre_activity_week_day = 0;
	int pre_activity_end_time = 0;

	while (!dataElement.empty())
	{
		if (m_activity_open_time_cfg_list_count >= CROSS_PASTURE_OPEN_TIME_MAX_COUNT)
		{
			return -1;
		}

		Cross3v3ActivityOpenTimeInfo &open_cfg = m_activity_open_time_cfg_list[m_activity_open_time_cfg_list_count];

		if (!PugiGetSubNodeValue(dataElement, "activity_week_day", open_cfg.activity_week_day) || open_cfg.activity_week_day < SUNDAY || open_cfg.activity_week_day > SATURDAY)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "activity_ready_time", open_cfg.activity_ready_time) || open_cfg.activity_ready_time < 0 || this->DayTimeFormatInvalid(open_cfg.activity_ready_time))
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "activity_start_time", open_cfg.activity_start_time) || open_cfg.activity_start_time < 0 || this->DayTimeFormatInvalid(open_cfg.activity_start_time))
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "activity_end_time", open_cfg.activity_end_time) || open_cfg.activity_end_time < 0 || this->DayTimeFormatInvalid(open_cfg.activity_end_time))
		{
			return -5;
		}

		if (open_cfg.activity_week_day < pre_activity_week_day)
		{
			return -6;
		}
		else if (open_cfg.activity_week_day == pre_activity_week_day)
		{
			if (open_cfg.activity_ready_time <= pre_activity_end_time)
			{
				return -7;
			}
		}

		pre_activity_end_time = open_cfg.activity_end_time;
		pre_activity_week_day = open_cfg.activity_week_day;

		int ready_time_sec = int(open_cfg.activity_ready_time / 100) * 3600 + (open_cfg.activity_ready_time % 100) * 60;
		int start_time_sec = int(open_cfg.activity_start_time / 100) * 3600 + (open_cfg.activity_start_time % 100) * 60;
		int end_time_sec = int(open_cfg.activity_end_time / 100) * 3600 + (open_cfg.activity_end_time % 100) * 60;

		open_cfg.ready_dur_s = start_time_sec - ready_time_sec;
		open_cfg.open_dur_s = end_time_sec - start_time_sec;

		if (open_cfg.ready_dur_s < 0 || open_cfg.open_dur_s <= 0)
		{
			return -9;
		}

		++m_activity_open_time_cfg_list_count;
		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int Cross3v3Config::InitStrongHoldCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	int count = 0;
	while (!dataElement.empty())
	{
		if (count >= CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "rate", m_stronghold_rate_list[count].rate) || m_stronghold_rate_list[count].rate <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "rate_role", m_stronghold_rate_list[count].rate_role) || m_stronghold_rate_list[count].rate_role <= 0)
		{
			return -3;
		}

		++count;
		dataElement = dataElement.next_sibling();
	}

	if (count != CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT)
	{
		return -4;
	}

	return 0;
}
int Cross3v3Config::InitJoinRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int seq = 0;
	int last_seq = -1;
	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq < 0 || seq >= CROSS_ACTIVITY_3V3_JOIN_TIME_CFG_MAX_COUNT || seq != last_seq + 1)
		{
			return -1;
		}

		last_seq = seq;
		Cross3V3JoinTimesRewardConfig &item_cfg = m_join_times_reward_cfg_list[seq];
		item_cfg.seq = seq;

		if (!PugiGetSubNodeValue(data_element, "jion_times", item_cfg.jion_times) || item_cfg.jion_times <= 0)
		{
			return -2;
		}

		PugiXmlNode item_element_list = data_element.child("reward_item_list");
		if (item_element_list.empty())
		{
			return -3;
		}

		PugiXmlNode item_element = item_element_list.child("reward_item");
		if (item_element.empty())
		{
			return -4;
		}

		while (!item_element.empty())
		{
			if (item_cfg.reward_count >= CROSS_3V3_REWARD_ITEM_MAX_COUNT)
			{
				return -5;
			}

			if (!item_cfg.reward_item_list[item_cfg.reward_count].ReadConfigNoCheck(item_element))
			{
				return -6;
			}

			item_cfg.reward_count++;
			item_element = item_element.next_sibling();
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int Cross3v3Config::InitGongxunRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_gongxun_reward_cfg_max_seq = -1;
	while (!data_element.empty())
	{
		int seq = 0;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq < 0 || seq >= CROSS_ACTIVITY_1V1_SCORE_CFG_MAX_COUNT || seq != m_gongxun_reward_cfg_max_seq + 1)
		{
			return -1;
		}

		m_gongxun_reward_cfg_max_seq = seq;
		Cross3V3GongxunRewardConfig &item_cfg = m_gongxun_reward_cfg_list[seq];
		item_cfg.seq = seq;

		if (!PugiGetSubNodeValue(data_element, "need_score", item_cfg.need_gongxun) || item_cfg.need_gongxun <= 0)
		{
			return -2;
		}

		PugiXmlNode item_element_list = data_element.child("reward_item_list");
		if (item_element_list.empty())
		{
			return -3;
		}

		PugiXmlNode item_element = item_element_list.child("reward_item");
		if (item_element.empty())
		{
			return -4;
		}

		while (!item_element.empty())
		{
			if (item_cfg.reward_count >= CROSS_3V3_REWARD_ITEM_MAX_COUNT)
			{
				return -5;
			}

			if (!item_cfg.reward_item_list[item_cfg.reward_count].ReadConfigNoCheck(item_element))
			{
				return -6;
			}

			item_cfg.reward_count++;
			item_element = item_element.next_sibling();
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int Cross3v3Config::InitDivisionScoreCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int last_grade = -1;
	while (!data_element.empty())
	{
		Cross3V3GradeScoreConfig temp_cfg;
		if (!PugiGetSubNodeValue(data_element, "grade", temp_cfg.grade) || temp_cfg.grade != last_grade + 1)
		{
			return -1;
		}
		last_grade = temp_cfg.grade;

		if (!PugiGetSubNodeValue(data_element, "score", temp_cfg.score) || temp_cfg.score < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "win_score", temp_cfg.win_score) || temp_cfg.win_score <= 0)
		{
			return -10;
		}

		if (!PugiGetSubNodeValue(data_element, "equal_score", temp_cfg.equal_score) || temp_cfg.equal_score < 0)
		{
			return -11;
		}

		if (!PugiGetSubNodeValue(data_element, "fail_score", temp_cfg.fail_score) || temp_cfg.fail_score < 0)
		{
			return -12;
		}

		if (!PugiGetSubNodeValue(data_element, "bind_gold", temp_cfg.bind_gold) || temp_cfg.bind_gold < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "mojing", temp_cfg.mojing) || temp_cfg.mojing < 0)
		{
			return -5;
		}

		int need_read_reward = 0;

		if (!PugiGetSubNodeValue(data_element, "need_read_reward", need_read_reward))
		{
			return -10;
		}

		if (need_read_reward == 1)
		{
			PugiXmlNode item_element_list = data_element.child("reward_item_list");
			if (item_element_list.empty())
			{
				return -6;
			}

			PugiXmlNode item_element = item_element_list.child("reward_item");
			if (item_element.empty())
			{
				item_element = item_element.next_sibling();
				continue; // 允许配不合格的物品，直接跳过，有些配置不需要奖励物品，直接配到令牌那边
//				return -9;
			}

			while (!item_element.empty())
			{
				if (temp_cfg.reward_count >= CROSS_3V3_REWARD_ITEM_MAX_COUNT)
				{
					return -8;
				}

				if (!temp_cfg.reward_item_list[temp_cfg.reward_count].ReadConfigNoCheck(item_element))
				{
					return -9;
				}

				temp_cfg.reward_count++;
				item_element = item_element.next_sibling();
			}

		}

		
		m_grade_score_cfg_vec.push_back(temp_cfg);
		data_element = data_element.next_sibling();
	}

	return 0;
}

int Cross3v3Config::InitRankRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int cfg_num = 0;
	int last_rank_pos_max = -1;
	while (!data_element.empty())
	{
		if (cfg_num < 0 || cfg_num >= CROSS_ACTIVITY_3V3_RANK_CFG_MAX_COUNT)
		{
			return -1;
		}

		m_rank_reward_cfg_list[cfg_num].rank_pos_min = last_rank_pos_max + 1;

		if (!PugiGetSubNodeValue(data_element, "rank", m_rank_reward_cfg_list[cfg_num].rank_pos_max) || m_rank_reward_cfg_list[cfg_num].rank_pos_max < m_rank_reward_cfg_list[cfg_num].rank_pos_min)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "bind_gold", m_rank_reward_cfg_list[cfg_num].bind_gold) || m_rank_reward_cfg_list[cfg_num].bind_gold < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "mojing", m_rank_reward_cfg_list[cfg_num].mojing) || m_rank_reward_cfg_list[cfg_num].mojing < 0)
		{
			return -5;
		}

		last_rank_pos_max = m_rank_reward_cfg_list[cfg_num].rank_pos_max;

		PugiXmlNode item_element_list = data_element.child("reward_item_list");
		if (item_element_list.empty())
		{
			return -6;
		}

		PugiXmlNode  item_element = item_element_list.child("reward_item");
		if (item_element.empty())
		{
			return -7;
		}

		while (!item_element.empty())
		{
			if (m_rank_reward_cfg_list[cfg_num].reward_count >= CROSS_3V3_REWARD_ITEM_MAX_COUNT)
			{
				return -8;
			}

			if (!m_rank_reward_cfg_list[cfg_num].reward_item_list[m_rank_reward_cfg_list[cfg_num].reward_count].ReadConfigNoCheck(item_element))
			{
				return -9;
			}

			m_rank_reward_cfg_list[cfg_num].reward_count++;
			item_element = item_element.next_sibling();
		}

		if (!PugiGetSubNodeValue(data_element, "score_limit", m_rank_reward_cfg_list[cfg_num].score_limit) || m_rank_reward_cfg_list[cfg_num].score_limit <= 0)
		{
			return -10;
		}

		cfg_num++;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int Cross3v3Config::InitPriorityConfig(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int last_priority = 0;

	while (!data_element.empty())
	{
		Cross3V3PriorityConfig temp_cfg;

		if (!PugiGetSubNodeValue(data_element, "priority", temp_cfg.priority) || temp_cfg.priority != last_priority + 1)
		{
			return -1;
		}
		last_priority = temp_cfg.priority;

		if (!PugiGetSubNodeValue(data_element, "match_time_s", temp_cfg.match_time_s) || temp_cfg.match_time_s < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "interval", temp_cfg.interval) || temp_cfg.interval < 0)
		{
			return -3;
		}

		m_priority_cfg_vec.push_back(temp_cfg);
		++m_max_priority;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int Cross3v3Config::InitDivistionGapExtraScoreCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		Cross3V3ExtraScoreConfig temp_cfg;

		if (!PugiGetSubNodeValue(data_element, "gap", temp_cfg.gap) || temp_cfg.gap < 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "extra_score", temp_cfg.extra_score) || temp_cfg.extra_score < 0)
		{
			return -2;
		}

		m_extra_score_cfg_vec.push_back(temp_cfg);

		data_element = data_element.next_sibling();
	}

	return 0;
}

int Cross3v3Config::InitDurWinCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		Cross3V3DurWinScoreConfig temp_cfg;

		if (!PugiGetSubNodeValue(data_element, "dur_win_times", temp_cfg.dur_win_times) || temp_cfg.dur_win_times < 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "extra_score", temp_cfg.extra_score) || temp_cfg.extra_score < 0)
		{
			return -2;
		}

		m_dur_win_score_cfg_vec.push_back(temp_cfg);

		data_element = data_element.next_sibling();
	}

	return 0;
}

int Cross3v3Config::InitSeasonRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int last_seq = 0;
	int last_season = 0;
	while (!data_element.empty())
	{
		Cross3v3SeasonRewardConfig temp_cfg;
		if (!PugiGetSubNodeValue(data_element, "season", temp_cfg.season) || temp_cfg.season <= 0 || temp_cfg.season > CROSS_ACTIVITY_3V3_SEASON_MAX)
		{
			return -1;
		}

		if (last_season != temp_cfg.season)
		{
			std::vector<Cross3v3SeasonRewardConfig> sub_vec;
			m_season_reward_grade_cfg_vec.push_back(sub_vec);
			m_season_reward_rank_cfg_vec.push_back(sub_vec);

			last_season = temp_cfg.season;
			++m_max_season;
		}

		if (!PugiGetSubNodeValue(data_element, "seq", temp_cfg.seq) || temp_cfg.seq != last_seq)
		{
			return -2;
		}

		if (PugiGetSubNodeValue(data_element, "grade", temp_cfg.grade))
		{
			if (temp_cfg.grade < 0 || temp_cfg.grade > CROSS_ACTIVITY_3V3_ETRA_GRADE_TYPE_MAX)
			{
				return -3;
			}
		}
		
		bool has_rank_cfg = false;
		if (PugiGetSubNodeValue(data_element, "rank", temp_cfg.rank))
		{
			if (temp_cfg.rank < 0)
			{
				return -13;
			}

			has_rank_cfg = true;
		}

		PugiXmlNode item_element = data_element.child("reward_item");
		if (item_element.empty())
		{
			return -4;
		}

		if (!temp_cfg.reward_item.ReadConfigNoCheck(item_element))
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "max_hp", temp_cfg.max_hp) || temp_cfg.max_hp < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "gong_ji", temp_cfg.gong_ji) || temp_cfg.gong_ji < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(data_element, "po_jia", temp_cfg.po_jia) || temp_cfg.po_jia < 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(data_element, "fang_yu", temp_cfg.fang_yu) || temp_cfg.fang_yu < 0)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(data_element, "fa_fang_yu", temp_cfg.fa_fang_yu) || temp_cfg.fa_fang_yu < 0)
		{
			return -10;
		}

		if (!PugiGetSubNodeValue(data_element, "add_hurt", temp_cfg.add_hurt) || temp_cfg.add_hurt < 0)
		{
			return -11;
		}

		if (!PugiGetSubNodeValue(data_element, "reduce_hurt", temp_cfg.reduce_hurt) || temp_cfg.reduce_hurt < 0)
		{
			return -12;
		}

		if (!PugiGetSubNodeValue(data_element, "card_type", temp_cfg.card_type) || temp_cfg.card_type < 0)
		{
			return -13;
		}

		m_season_reward_grade_cfg_vec[temp_cfg.season - 1].push_back(temp_cfg);

		if (has_rank_cfg)
		{
			m_season_reward_rank_cfg_vec[temp_cfg.season - 1].push_back(temp_cfg);
		}

		++last_seq;
		data_element = data_element.next_sibling();
	}
	return 0;
}

int Cross3v3Config::InitAfterSeasonRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int last_seq = 0;
	while (!data_element.empty())
	{
		Cross3v3SeasonRewardConfig temp_cfg;
		if (!PugiGetSubNodeValue(data_element, "season", temp_cfg.season) || temp_cfg.season <= 0 || temp_cfg.season > CROSS_ACTIVITY_3V3_SEASON_MAX)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "seq", temp_cfg.seq) || temp_cfg.seq != last_seq)
		{
			return -2;
		}

		if (PugiGetSubNodeValue(data_element, "grade", temp_cfg.grade))
		{
			if (temp_cfg.grade < 0 || temp_cfg.grade > CROSS_ACTIVITY_3V3_ETRA_GRADE_TYPE_MAX)
			{
				return -3;
			}
		}

		bool has_rank_cfg = false;
		if (PugiGetSubNodeValue(data_element, "rank", temp_cfg.rank))
		{
			if (temp_cfg.rank < 0)
			{
				return -4;
			}

			has_rank_cfg = true;
		}

		PugiXmlNode item_element = data_element.child("reward_item");
		if (item_element.empty())
		{
			return -5;
		}

		if (!temp_cfg.reward_item.ReadConfigNoCheck(item_element))
		{
			return -6;
		}

		m_after_season_reward_grade_cfg_vec.push_back(temp_cfg);

		if (has_rank_cfg)
		{
			m_after_season_reward_rank_cfg_vec.push_back(temp_cfg);
		}

		++last_seq;
		data_element = data_element.next_sibling();
	}

	return 0;
}

bool Cross3v3Config::DayTimeFormatInvalid(int day_time)
{
	int hour = day_time / 100;
	int minute = day_time % 100;

	return (hour < 0 || hour >= 24 || minute < 0 || minute >= 60);
}