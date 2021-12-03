#include "cross1v1commonconfig.hpp"

#include "servercommon/configcommon.h"

#include "servercommon/serverconfig/configstruct.h"

Cross1V1CommonConfig::Cross1V1CommonConfig() : m_max_priority(0)
{
}

Cross1V1CommonConfig::~Cross1V1CommonConfig()
{

}

Cross1V1CommonConfig & Cross1V1CommonConfig::Instance()
{
	static Cross1V1CommonConfig cvcc;
	return cvcc;
}

bool Cross1V1CommonConfig::Init(const std::string &configname, std::string *err)
{
	PUGI_XML_CONFIG_PRE_LOAD;

	// 其他配置
	PUGI_XML_LOAD_CONFIG("other", InitOtherCfg);
	// 开启时间
	PUGI_XML_LOAD_CONFIG("activity_open_time", InitActivityOpenTimeCfg);
	// 每日参与场次奖励
	PUGI_XML_LOAD_CONFIG("join_times_reward", InitJoinTimesRewardCfg);
	// 积分达标奖励
	PUGI_XML_LOAD_CONFIG("score_reward", InitScoreRewardCfg);
	// 排行奖励
	PUGI_XML_LOAD_CONFIG("rank_reward", InitRankRewardCfg);
	// 奖励经验
	PUGI_XML_LOAD_CONFIG("reward_exp", InitRewardExpCfg);
	// 随机名字
	PUGI_XML_LOAD_CONFIG("rand_name", InitRandNameCfg);
	// 段位积分
	PUGI_XML_LOAD_CONFIG("grade_score", InitGradeScoreCfg);
	// 匹配优先级
	PUGI_XML_LOAD_CONFIG("match_priority", InitPriorityConfig);
	// 额外扣分
	PUGI_XML_LOAD_CONFIG("extra_deduct_score", InitExtraScoreConfig);
	//连胜积分
	PUGI_XML_LOAD_CONFIG("extra_increase_score", InitExtraIncreaseScoreConfig);
	// 赛季戒指
	PUGI_XML_LOAD_CONFIG("season_ring", InitSeasonRewardConfig);
	// N赛季后奖励
	PUGI_XML_LOAD_CONFIG("after_season_reward", InitAfterSeasonRewardConfig);
	
	return true;
}

int Cross1V1CommonConfig::GetOpenActivityConfigList(Cross1V1OpenTimeConfig cfg_list[CROSS_ACTIVITY_OPEN_TIME_CFG_MAX_COUNT]) const
{
	memcpy(cfg_list, m_activity_open_time_cfg_list, sizeof(m_activity_open_time_cfg_list));
	return m_activity_open_time_cfg_list_count;
}

int Cross1V1CommonConfig::GetActivityStandbyTime(int index) const
{
	if (index >= 0 && index < m_activity_open_time_cfg_list_count && index < CROSS_ACTIVITY_OPEN_TIME_CFG_MAX_COUNT)
	{
		return m_activity_open_time_cfg_list[index].ready_dur_s;
	}

	return CROSS_1V1_DEFAULT_STANDBY_TIME_S;
}

int Cross1V1CommonConfig::GetActivityOpenTime(int index) const
{
	if (index >= 0 && index < m_activity_open_time_cfg_list_count && index < CROSS_ACTIVITY_OPEN_TIME_CFG_MAX_COUNT)
	{
		return m_activity_open_time_cfg_list[index].open_dur_s;
	}

	return CROSS_1V1_DEFAULT_OPEN_TIME_S;
}

const Cross1V1JoinTimesRewardConfig * Cross1V1CommonConfig::GetJoinTimeRewardCfgBySeq(int seq) const
{
	if (seq < 0 || seq >= CROSS_ACTIVITY_1V1_JOIN_TIME_CFG_MAX_COUNT)
	{
		return NULL;
	}

	return &m_join_times_reward_cfg_list[seq];
}

const Cross1V1ScoreRewardConfig * Cross1V1CommonConfig::GetScoreRewardCfgBySeq(int seq) const
{
	if (seq < 0 || seq >= CROSS_ACTIVITY_1V1_SCORE_CFG_MAX_COUNT)
	{
		return NULL;
	}

	return &m_score_reward_cfg_list[seq];
}

const Cross1V1RankRewardConfig * Cross1V1CommonConfig::GetScoreRankRewardByRankPos(int rank_pos, int score) const
{
	for (int i = 0; i < CROSS_ACTIVITY_1V1_RANK_CFG_MAX_COUNT; i++)
	{
		if (rank_pos <= m_rank_reward_cfg_list[i].max_rank_pos && score >= m_rank_reward_cfg_list[i].score_limit && m_rank_reward_cfg_list[i].score_limit > 0)
		{
			return &m_rank_reward_cfg_list[i];
		}
	}

	return NULL;
}

const Cross1V1RandNameConfig * Cross1V1CommonConfig::GetRandNameConfig() const
{
	if ((int)m_rand_name_map.size() <= 0)
	{
		return NULL;
	}

	int rand_num = RandomNum((int)m_rand_name_map.size());
	if (rand_num < 0 || rand_num >= (int)m_rand_name_map.size())
	{
		return NULL;
	}

	std::map<int, Cross1V1RandNameConfig>::const_iterator it = m_rand_name_map.find(rand_num);
	if (it != m_rand_name_map.end())
	{
		return &it->second;
	}

	return NULL;
}

long long Cross1V1CommonConfig::GetRewardExpByLevel(int level)
{
	if (int(m_reward_exp_map.size()) <= 0) return 0;
	std::map<int, long long>::reverse_iterator rit;
	for (rit = m_reward_exp_map.rbegin(); rit != m_reward_exp_map.rend(); rit++)
	{
		if (level >= rit->first)
		{
			return rit->second;
		}
	}
	return 0;
}

int Cross1V1CommonConfig::InitOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int is_open = 0;
	if (!PugiGetSubNodeValue(data_element, "is_open", is_open))
	{
		return -1;
	}
	m_other_cfg.is_open = (is_open != 0);

	if (!PugiGetSubNodeValue(data_element, "free_times", m_other_cfg.free_times) || m_other_cfg.free_times < 0)
	{
		return -2;
	}

	if (!PugiGetSubNodeValue(data_element, "max_buy_times", m_other_cfg.max_buy_times) || m_other_cfg.max_buy_times < 0)
	{
		return -3;
	}

	if (!PugiGetSubNodeValue(data_element, "buy_time_cost", m_other_cfg.buy_time_cost) || m_other_cfg.buy_time_cost <= 0)
	{
		return -4;
	}

	if (!PugiGetSubNodeValue(data_element, "match_time_s", m_other_cfg.match_time_s) || m_other_cfg.match_time_s < 0)
	{
		return -5;
	}

	if (!PugiGetSubNodeValue(data_element, "fight_wait_s", m_other_cfg.fight_wait_s) || m_other_cfg.fight_wait_s < 0)
	{
		return -6;
	}

	if (!PugiGetSubNodeValue(data_element, "fight_time_s", m_other_cfg.fight_time_s) || m_other_cfg.fight_time_s < 0)
	{
		return -7;
	}

	if (!PugiGetSubNodeValue(data_element, "pos_1_x", m_other_cfg.pos_1.x) || m_other_cfg.pos_1.x < 0)
	{
		return -8;
	}

	if (!PugiGetSubNodeValue(data_element, "pos_1_y", m_other_cfg.pos_1.y) || m_other_cfg.pos_1.y < 0)
	{
		return -9;
	}

	if (!PugiGetSubNodeValue(data_element, "pos_2_x", m_other_cfg.pos_2.x) || m_other_cfg.pos_2.x < 0)
	{
		return -10;
	}

	if (!PugiGetSubNodeValue(data_element, "pos_2_y", m_other_cfg.pos_2.y) || m_other_cfg.pos_2.y < 0)
	{
		return -11;
	}

	if (!PugiGetSubNodeValue(data_element, "scene_id", m_other_cfg.scene_id) || m_other_cfg.scene_id <= 0)
	{
		return -12;
	}

	if (!PugiGetSubNodeValue(data_element, "winner_score", m_other_cfg.winner_score) || m_other_cfg.winner_score < 0)
	{
		return -13;
	}

	if (!PugiGetSubNodeValue(data_element, "loser_score", m_other_cfg.loser_score) || m_other_cfg.loser_score < 0)
	{
		return -14;
	}

	if (!PugiGetSubNodeValue(data_element, "winner_exp_per", m_other_cfg.winner_exp_per) || m_other_cfg.winner_exp_per < 0)
	{
		return -15;
	}

	if (!PugiGetSubNodeValue(data_element, "loser_exp_per", m_other_cfg.loser_exp_per) || m_other_cfg.loser_exp_per < 0)
	{
		return -16;
	}

	if (!PugiGetSubNodeValue(data_element, "cross_info_clear_time", m_other_cfg.cross_info_clear_time) || m_other_cfg.cross_info_clear_time < 0)
	{
		return -17;
	}

	if (!PugiGetSubNodeValue(data_element, "role_shadow_add_attr_per", m_other_cfg.role_shadow_add_attr_per) || m_other_cfg.role_shadow_add_attr_per < 0)
	{
		return -18;
	}

	if (!PugiGetSubNodeValue(data_element, "role_shadow_alay_ms", m_other_cfg.role_shadow_alay_ms) || m_other_cfg.role_shadow_alay_ms < 0)
	{
		return -19;
	}

	PugiXmlNode item_element_winner = data_element.child("winner_item");
	if (item_element_winner.empty())
	{
		return -20;
	}

	if (!m_other_cfg.winner_item.ReadConfigNoCheck(item_element_winner))
	{
		return -21;
	}

	PugiXmlNode item_element_loser = data_element.child("loser_item");
	if (item_element_loser.empty())
	{
		return -22;
	}

	if (!m_other_cfg.loser_item.ReadConfigNoCheck(item_element_loser))
	{
		return -23;
	}

	PugiXmlNode item_element_join = data_element.child("join_item");
	if (item_element_join.empty())
	{
		return -24;
	}

	if (!m_other_cfg.join_item.ReadConfigNoCheck(item_element_join))
	{
		return -25;
	}

	if (!PugiGetSubNodeValue(data_element, "atleast_join_times", m_other_cfg.atleast_join_times) || m_other_cfg.atleast_join_times < 0)
	{
		return -26;
	}

	if (!PugiGetSubNodeValue(data_element, "rank_reward_min_grade", m_other_cfg.rank_reward_min_grade) || m_other_cfg.rank_reward_min_grade < 0)
	{
		return -27;
	}

	if (!PugiGetSubNodeValue(data_element, "limit_join_level", m_other_cfg.limit_join_level) || m_other_cfg.limit_join_level < 0)
	{
		return -28;
	}
	
	if (!PugiGetSubNodeValue(data_element, "fail_addorreduce_grade", m_other_cfg.fail_addorreduce_grade) || m_other_cfg.fail_addorreduce_grade < 0 || m_other_cfg.fail_addorreduce_grade > CROSS_ACTIVITY_1V1_GRADE_TYPE_MAX)
	{
		return -29;
	}

	if (!PugiGetSubNodeValue(data_element, "open_server_day", m_other_cfg.open_server_day) || m_other_cfg.open_server_day <= 0)
	{
		return -30;
	}

	return 0;
}

int Cross1V1CommonConfig::InitActivityOpenTimeCfg(PugiXmlNode RootElement)
{
	
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int pre_activity_week_day = 0;
	int pre_activity_end_time = 0;
	m_activity_open_time_cfg_list_count = 0;

	while (!data_element.empty())
	{
		if (m_activity_open_time_cfg_list_count >= CROSS_PASTURE_OPEN_TIME_MAX_COUNT)
		{
			return -1;
		}

		Cross1V1OpenTimeConfig &open_cfg = m_activity_open_time_cfg_list[m_activity_open_time_cfg_list_count];

		if (!PugiGetSubNodeValue(data_element, "activity_week_day", open_cfg.activity_week_day) || open_cfg.activity_week_day < SUNDAY || open_cfg.activity_week_day > SATURDAY)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "activity_ready_time", open_cfg.activity_ready_time) || open_cfg.activity_ready_time < 0 || this->DayTimeFormatInvalid(open_cfg.activity_ready_time))
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "activity_start_time", open_cfg.activity_start_time) || open_cfg.activity_start_time < 0 || this->DayTimeFormatInvalid(open_cfg.activity_start_time))
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "activity_end_time", open_cfg.activity_end_time) || open_cfg.activity_end_time < 0 || this->DayTimeFormatInvalid(open_cfg.activity_end_time))
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
		data_element = data_element.next_sibling();
	}

	return 0;
}

int Cross1V1CommonConfig::InitJoinTimesRewardCfg(PugiXmlNode RootElement)
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
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq < 0 || seq >= CROSS_ACTIVITY_1V1_JOIN_TIME_CFG_MAX_COUNT || seq != last_seq + 1)
		{
			return -1;
		}

		last_seq = seq;
		Cross1V1JoinTimesRewardConfig &item_cfg = m_join_times_reward_cfg_list[seq];
		item_cfg.seq = seq;

		if (!PugiGetSubNodeValue(data_element, "jion_times", item_cfg.jion_times) || item_cfg.jion_times <= 0)
		{
			return -2;
		}

		int item_count = ItemConfigData::ReadConfigListNoCheck(data_element, "reward_item_l", item_cfg.reward_item_l_list, MAX_ATTACHMENT_ITEM_NUM);
		if (item_count < 0)
		{
			return -1000 + item_count;
		}
		item_cfg.reward_item_l_count = item_count;

		item_count = ItemConfigData::ReadConfigListNoCheck(data_element, "reward_item_h", item_cfg.reward_item_h_list, MAX_ATTACHMENT_ITEM_NUM);
		if (item_count < 0)
		{
			return -2000 + item_count;
		}
		item_cfg.reward_item_h_count = item_count;

		data_element = data_element.next_sibling();
	}

	return 0;
}

int Cross1V1CommonConfig::InitScoreRewardCfg(PugiXmlNode RootElement)
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
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq < 0 || seq >= CROSS_ACTIVITY_1V1_SCORE_CFG_MAX_COUNT || seq != last_seq + 1)
		{
			return -1;
		}

		last_seq = seq;
		Cross1V1ScoreRewardConfig &item_cfg = m_score_reward_cfg_list[seq];
		item_cfg.seq = seq;

		if (!PugiGetSubNodeValue(data_element, "need_score", item_cfg.need_score) || item_cfg.need_score <= 0)
		{
			return -2;
		}

		{
			item_cfg.reward_count = ItemConfigData::ReadConfigListNoCheck(data_element, std::string("reward_item"),
				item_cfg.reward_item_list, CROSS_1V1_REWARD_ITEM_MAX_COUNT);
			if (item_cfg.reward_count < 0)
			{
				return -200 + item_cfg.reward_count;
			}
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int Cross1V1CommonConfig::InitRankRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int cfg_num = 0;
	int last_max_rank_pos = -1;
	while (!data_element.empty())
	{
		if (cfg_num < 0 || cfg_num >= CROSS_ACTIVITY_1V1_RANK_CFG_MAX_COUNT)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "min_rank_pos", m_rank_reward_cfg_list[cfg_num].min_rank_pos) || m_rank_reward_cfg_list[cfg_num].min_rank_pos < 0 || m_rank_reward_cfg_list[cfg_num].min_rank_pos != last_max_rank_pos + 1)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "max_rank_pos", m_rank_reward_cfg_list[cfg_num].max_rank_pos) || m_rank_reward_cfg_list[cfg_num].max_rank_pos < m_rank_reward_cfg_list[cfg_num].min_rank_pos)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "score_limit", m_rank_reward_cfg_list[cfg_num].score_limit) || m_rank_reward_cfg_list[cfg_num].score_limit <= 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "rank_ring_grade", m_rank_reward_cfg_list[cfg_num].rank_ring_grade) || m_rank_reward_cfg_list[cfg_num].rank_ring_grade < 0 || m_rank_reward_cfg_list[cfg_num].rank_ring_grade > CROSS_ACTIVITY_1V1_RING_TYPE_MAX)
		{
			return -5;
		}
		
		last_max_rank_pos = m_rank_reward_cfg_list[cfg_num].max_rank_pos;

		{
			m_rank_reward_cfg_list[cfg_num].reward_count = ItemConfigData::ReadConfigListNoCheck(data_element, std::string("reward_item"),
				m_rank_reward_cfg_list[cfg_num].reward_item_list, CROSS_1V1_REWARD_ITEM_MAX_COUNT);
			if (m_rank_reward_cfg_list[cfg_num].reward_count < 0)
			{
				return -200 + m_rank_reward_cfg_list[cfg_num].reward_count;
			}
		}

		if (!PugiGetSubNodeValue(data_element, "coin", m_rank_reward_cfg_list[cfg_num].coin) || m_rank_reward_cfg_list[cfg_num].coin < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(data_element, "bind_gold", m_rank_reward_cfg_list[cfg_num].bind_gold) || m_rank_reward_cfg_list[cfg_num].bind_gold < 0)
		{
			return -8;
		}

		cfg_num++;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int Cross1V1CommonConfig::InitRewardExpCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_reward_exp_map.clear();

	int role_level = 0;
	int last_role_level = 0;
	long long reward_exp = 0;
	long long last_reward_exp = 0;
	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "role_level", role_level) || role_level <= last_role_level)
		{
			return -1;
		}

		last_role_level = role_level;

		if (!PugiGetSubNodeValue(data_element, "reward_exp", reward_exp) || reward_exp < last_reward_exp)
		{
			return -2;
		}

		last_reward_exp = reward_exp;
		if (m_reward_exp_map.find(role_level) != m_reward_exp_map.end())
		{
			return -3;
		}

		m_reward_exp_map[role_level] = reward_exp;

		data_element = data_element.next_sibling();
	}

	return 0;
}

int Cross1V1CommonConfig::InitRandNameCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}
	
	m_rand_name_map.clear();
	int seq = 0;
	int last_seq = -1;
	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq != last_seq + 1)
		{
			return -1;
		}

		last_seq = seq;
		Cross1V1RandNameConfig temp_cfg;
		if (!PugiGetSubNodeValue(data_element, "sex", temp_cfg.sex) || (temp_cfg.sex != 0 && temp_cfg.sex != 1))
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "name", temp_cfg.name))
		{
			return -3;
		}

		if (m_rand_name_map.find(seq) != m_rand_name_map.end())
		{
			return -4;
		}

		m_rand_name_map[seq] = temp_cfg;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int Cross1V1CommonConfig::InitGradeScoreCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int last_grade = -1;
	while (!data_element.empty())
	{
		Cross1V1GradeScoreConfig temp_cfg;
		if (!PugiGetSubNodeValue(data_element, "grade", temp_cfg.grade) || temp_cfg.grade != last_grade + 1)
		{
			return -1;
		}
		last_grade = temp_cfg.grade;

		if (!PugiGetSubNodeValue(data_element, "score", temp_cfg.score) || temp_cfg.score < 0 )
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "victory_score", temp_cfg.victory_score) || temp_cfg.victory_score < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "fail_score", temp_cfg.fail_score) || temp_cfg.fail_score < 0)
		{
			return -5;
		}

		int need_read_reward = 0;
		if (!PugiGetSubNodeValue(data_element, "need_read_reward", need_read_reward))
		{
			return -12;
		}

		if (need_read_reward == 1)
		{
			{
				temp_cfg.reward_count = ItemConfigData::ReadConfigListNoCheck(data_element, std::string("reward_item"),
					temp_cfg.reward_item_list, CROSS_1V1_REWARD_ITEM_MAX_COUNT);
				if (temp_cfg.reward_count < 0)
				{
					return -200 + temp_cfg.reward_count;
				}
			}
		}
		

		if (!PugiGetSubNodeValue(data_element, "bind_gold", temp_cfg.bind_gold) || temp_cfg.bind_gold < 0)
		{
			return -10;
		}

		if (!PugiGetSubNodeValue(data_element, "coin", temp_cfg.coin) || temp_cfg.coin < 0)
		{
			return -11;
		}

		m_grade_score_cfg_vec.push_back(temp_cfg);
		data_element = data_element.next_sibling();
	}

	return 0;
}

int Cross1V1CommonConfig::InitPriorityConfig(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int last_priority = 0;
	while (!data_element.empty())
	{
		Cross1V1PriorityConfig temp_cfg;

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

int Cross1V1CommonConfig::InitExtraScoreConfig(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		Cross1V1ExtraScoreConfig temp_cfg;

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

int Cross1V1CommonConfig::InitExtraIncreaseScoreConfig(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		Cross1V1ExtraIncreaseScoreConfig temp_cfg;

		if (!PugiGetSubNodeValue(data_element, "dur_win_times", temp_cfg.dur_win_times) || temp_cfg.dur_win_times < 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "extra_score", temp_cfg.extra_score) || temp_cfg.extra_score < 0)
		{
			return -2;
		}

		m_extra_increase_score_cfg_vec.push_back(temp_cfg);

		data_element = data_element.next_sibling();
	}
	return 0;
}

int Cross1V1CommonConfig::InitSeasonRewardConfig(PugiXmlNode RootElement)
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
		Cross1V1SeasonRewardConfig temp_cfg;
		if (!PugiGetSubNodeValue(data_element, "season", temp_cfg.season) || temp_cfg.season <= 0 || temp_cfg.season > CROSS_1V1_MAX_SEASON)
		{
			return -1;
		}

		if (last_season != temp_cfg.season)
		{
			std::vector<Cross1V1SeasonRewardConfig> sub_vec;
			m_season_reward_cfg_vec.push_back(sub_vec);

			last_season = temp_cfg.season;
			++m_max_season;
		}

		if (!PugiGetSubNodeValue(data_element, "seq", temp_cfg.seq) || temp_cfg.seq != last_seq)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "grade", temp_cfg.grade) || temp_cfg.grade < 0 || temp_cfg.grade > CROSS_ACTIVITY_1V1_RING_TYPE_MAX)
		{
			return -3;
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

		if (!PugiGetSubNodeValue(data_element, "add_hurt", temp_cfg.add_shang) || temp_cfg.add_shang < 0)
		{
			return -11;
		}

		if (!PugiGetSubNodeValue(data_element, "reduce_hurt", temp_cfg.reduce_shang) || temp_cfg.reduce_shang < 0)
		{
			return -12;
		}

		m_season_reward_cfg_vec[temp_cfg.season - 1].push_back(temp_cfg);

		++last_seq;
		data_element = data_element.next_sibling();
	}
	return 0;
}

int Cross1V1CommonConfig::InitAfterSeasonRewardConfig(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int last_seq = 0;
	while (!data_element.empty())
	{
		Cross1V1SeasonRewardConfig temp_cfg;
		if (!PugiGetSubNodeValue(data_element, "season", temp_cfg.season) || temp_cfg.season <= 0 || temp_cfg.season > CROSS_1V1_MAX_SEASON)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "seq", temp_cfg.seq) || temp_cfg.seq != last_seq)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "grade", temp_cfg.grade) || temp_cfg.grade < 0 || temp_cfg.grade > CROSS_ACTIVITY_1V1_RING_TYPE_MAX)
		{
			return -3;
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

		m_after_season_reward_cfg_vec.push_back(temp_cfg);

		++last_seq;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int Cross1V1CommonConfig::GetMaxPriority()const
{
	return m_max_priority;
}

int Cross1V1CommonConfig::GetMaxSeason()const
{
	return m_max_season;
}

const Cross1V1GradeScoreConfig *Cross1V1CommonConfig::GetGradeScoreCfgByGrade(int grade) const
{
	if (grade < 0 || grade >= CROSS_ACTIVITY_1V1_RING_TYPE_MAX || grade > static_cast<int>(m_grade_score_cfg_vec.size()))
	{
		return NULL;
	}
	return &m_grade_score_cfg_vec[grade];
}

const Cross1V1GradeScoreConfig *Cross1V1CommonConfig::GetGradeScoreCfgByScore(int score) const
{
	int grade = this->GetGradeByScore(score);
	const Cross1V1GradeScoreConfig *cfg = this->GetGradeScoreCfgByGrade(grade);
	return cfg;
}

const Cross1V1PriorityConfig *Cross1V1CommonConfig::GetPriorityCfgByMatchTimes(int times) const
{
	if (times < 0 || times >= static_cast<int>(m_priority_cfg_vec.size()))
	{
		int time_s = static_cast<int>(m_priority_cfg_vec.size()) - 1 > 0 ? static_cast<int>(m_priority_cfg_vec.size()) - 1 : 0;
		return &m_priority_cfg_vec[time_s];
	}
	return &m_priority_cfg_vec[times];
}

const Cross1V1SeasonRewardConfig *Cross1V1CommonConfig::GetSeasonCfgBySeasonGrade(int season, int grade) const
{
	if (season <= 0 || season > m_max_season || season >= CROSS_1V1_MAX_SEASON)
	{
		return NULL;
	}

	if (grade < 0 || grade > CROSS_ACTIVITY_1V1_RING_TYPE_MAX)
	{
		return NULL;
	}

	if (m_season_reward_cfg_vec.empty())
	{
		return nullptr;
	}

	if (season > static_cast<int>(m_season_reward_cfg_vec.size()))
	{
		season = m_season_reward_cfg_vec.size();
	}

	for (std::vector<Cross1V1SeasonRewardConfig>::const_iterator it = m_season_reward_cfg_vec[season - 1].begin(); it != m_season_reward_cfg_vec[season - 1].end(); ++it)
	{
		if (it->grade == grade)
		{
			return &(*it);
		}
	}
	
	return NULL;
}

const Cross1V1SeasonRewardConfig *Cross1V1CommonConfig::GetSeasonCfgBySeq(int seq)const
{
	for (Cross1v1SeasonVector::const_iterator it = m_season_reward_cfg_vec.begin(); it != m_season_reward_cfg_vec.end(); ++it)
	{
		for (std::vector<Cross1V1SeasonRewardConfig>::const_iterator it_2 = it->begin(); it_2 != it->end(); ++it_2)
		{
			if (it_2->seq == seq)
			{
				return &(*it_2);
			}
		}
	}
	return NULL;
}

const Cross1V1SeasonRewardConfig *Cross1V1CommonConfig::GetAfterSeasonCfgBySeasonGrade(int season, int grade) const
{
	if (season <= m_max_season)
	{
		return NULL;
	}

	if (grade < 0 || grade > CROSS_ACTIVITY_1V1_RING_TYPE_MAX)
	{
		return NULL;
	}

	for (auto it = m_after_season_reward_cfg_vec.begin(); it != m_after_season_reward_cfg_vec.end(); ++it)
	{
		if (it->grade == grade)
		{
			return &(*it);
		}
	}

	return NULL;
}

int Cross1V1CommonConfig::GetExtraScoreByGap(int grade_self, int grade_other)const
{
	if (grade_self <= grade_other)
	{
		return 0;
	}

	int gap = static_cast<int>(fabs(grade_self - grade_other));
	int score = 0;

	for (int i = 0; i < static_cast<int>(m_extra_score_cfg_vec.size()); ++i)
	{
		if (gap > m_extra_score_cfg_vec[i].gap)
		{
			score = m_extra_score_cfg_vec[i].extra_score;
		}
	}
	return -score;
}

int Cross1V1CommonConfig::GetGradeByScore(int score)const
{
	int size = static_cast<int>(m_grade_score_cfg_vec.size());
	if (size <= 0)
	{
		return 0;
	}

	for (int i = size - 1; i >= 0; --i)
	{
		if (score >= m_grade_score_cfg_vec[i].score)
		{
			return m_grade_score_cfg_vec[i].grade;
		}
	}

	return 0;
}

int Cross1V1CommonConfig::GetExtraScoreByWinTimes(int times)const
{
	for (int i = 0; i < static_cast<int>(m_extra_increase_score_cfg_vec.size()); ++i)
	{
		if (times >= m_extra_increase_score_cfg_vec[i].dur_win_times)
		{
			return m_extra_increase_score_cfg_vec[i].extra_score;
		}
	}
	return 0;
}

bool Cross1V1CommonConfig::DayTimeFormatInvalid(int day_time)
{
	int hour = day_time / 100;
	int minute = day_time % 100;

	return (hour < 0 || hour >= 24 || minute < 0 || minute >= 60);
}

bool Cross1V1CommonConfig::GetIsAddFailScoreByGrade(int grade)
{
	return grade <= m_other_cfg.fail_addorreduce_grade;          // 在此段位及之下失败不扣积分反而增加
}

