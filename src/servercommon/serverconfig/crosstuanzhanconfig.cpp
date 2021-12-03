#include "crosstuanzhanconfig.hpp"
#include "servercommon/configcommon.h"

CrossTuanzhanConfig::CrossTuanzhanConfig()
	: m_activity_open_time_cfg_list_count(0), m_player_relive_pos_list_count(0), m_score_reward_level_cfg_count(0), m_rank_reward_level_cfg_count(0)
{

}

CrossTuanzhanConfig::~CrossTuanzhanConfig()
{

}

CrossTuanzhanConfig & CrossTuanzhanConfig::Instance()
{
	static CrossTuanzhanConfig _instance;
	return _instance;
}

bool CrossTuanzhanConfig::Init(const std::string &path, std::string *err)
{
	int iRet = 0;
	char errinfo[1024] = {0};

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "CrossTuanzhanConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = path + ": xml root node error.";
		return false;
	}

	{
		// ÆäËûÅäÖÃ
		PugiXmlNode OtherElement = RootElement.child("other");
		if (OtherElement.empty())
		{
			*err = path + ": xml node error in [config->other]";
			return false;
		}

		iRet = this->InitOtherCfg(OtherElement);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitOtherCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// »î¶¯¿ªÆôÊ±¼äÅäÖÃ
		PugiXmlNode OpenTimeElement = RootElement.child("activity_open_time");
		if (OpenTimeElement.empty())
		{
			*err = path + ": xml node error in [config->activity_open_time]";
			return false;
		}

		iRet = this->InitActivityOpenTimeCfg(OpenTimeElement);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitActivityOpenTimeCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// Öù×ÓÅäÖÃ
		PugiXmlNode PillaElement = RootElement.child("pilla_info");
		if (PillaElement.empty())
		{
			*err = path + ": xml node error in [config->pilla_info]";
			return false;
		}

		iRet = this->InitPillaCfg(PillaElement);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitPillaCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// ¸´»îµãÅäÖÃ
		PugiXmlNode ReliveElement = RootElement.child("create_pos");
		if (ReliveElement.empty())
		{
			*err = path + ": xml node error in [config->create_pos]";
			return false;
		}

		iRet = this->InitReliveCfg(ReliveElement);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitReliveCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}
	
	{
		// »ý·Ö½±ÀøÅäÖÃ
		PugiXmlNode ScoreRewardElement = RootElement.child("score_reward");
		if (ScoreRewardElement.empty())
		{
			*err = path + ": xml node error in [config->score_reward]";
			return false;
		}

		iRet = this->InitScoreRewardCfg(ScoreRewardElement);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitScoreRewardCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// ÅÅÐÐ°ñ½±ÀøÅäÖÃ
		PugiXmlNode RankRewardElement = RootElement.child("rank_reward");
		if (RankRewardElement.empty())
		{
			*err = path + ": xml node error in [config->rank_reward]";
			return false;
		}

		iRet = this->InitRankRewardCfg(RankRewardElement);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitRankRewardCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}
	
	return true;
}

int CrossTuanzhanConfig::InitOtherCfg(PugiXmlNode RootElement)
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
	
	if (!PugiGetSubNodeValue(dataElement, "scene_id", m_other_cfg.scene_id) || m_other_cfg.scene_id <= 0)
	{
		return -2;
	}

	if (!PugiGetSubNodeValue(dataElement, "scene_standby_dur_s", m_other_cfg.scene_standby_dur_s) || m_other_cfg.scene_standby_dur_s < 0)
	{
		return -10;
	}

	if (!PugiGetSubNodeValue(dataElement, "kill_score", m_other_cfg.kill_score) || m_other_cfg.kill_score < 0)
	{
		return -20;
	}

	if (!PugiGetSubNodeValue(dataElement, "assist_score", m_other_cfg.assist_score) || m_other_cfg.assist_score < 0)
	{
		return -21;
	}

	if (!PugiGetSubNodeValue(dataElement, "scene_add_score_time", m_other_cfg.scene_add_score_time) || m_other_cfg.scene_add_score_time <= 0)
	{
		return -22;
	}

	if (!PugiGetSubNodeValue(dataElement, "static_score", m_other_cfg.static_score) || m_other_cfg.static_score < 0)
	{
		return -23;
	}

	if (!PugiGetSubNodeValue(dataElement, "pillar_time", m_other_cfg.pillar_time) || m_other_cfg.pillar_time <= 0)
	{
		return -24;
	}

	if (!PugiGetSubNodeValue(dataElement, "pillar_player_score", m_other_cfg.pillar_player_score) || m_other_cfg.pillar_player_score < 0)
	{
		return -25;
	}

	if (!PugiGetSubNodeValue(dataElement, "pillar_teammate_score", m_other_cfg.pillar_teammate_score) || m_other_cfg.pillar_teammate_score < 0)
	{
		return -26;
	}

	if (!PugiGetSubNodeValue(dataElement, "weak_buff_need_been_kill", m_other_cfg.weak_buff_need_been_kill) || m_other_cfg.weak_buff_need_been_kill < 0)
	{
		return -30;
	}

	if (!PugiGetSubNodeValue(dataElement, "weak_buff_hp_percent", m_other_cfg.weak_buff_hp_percent) || m_other_cfg.weak_buff_hp_percent < 0)
	{
		return -31;
	}

	if (!PugiGetSubNodeValue(dataElement, "winner_cross_honor", m_other_cfg.winner_cross_honor) || m_other_cfg.winner_cross_honor < 0)
	{
		return -40;
	}

	if (!PugiGetSubNodeValue(dataElement, "winner_rank_num", m_other_cfg.winner_rank_num) || m_other_cfg.winner_rank_num < 0)
	{
		return -41;
	}


	PugiXmlNode reward_item_list_element = dataElement.child("reward_item_list");
	if (reward_item_list_element.empty())
	{
		return -42;
	}

	PugiXmlNode reward_item_element = reward_item_list_element.child("reward_item");
	if (reward_item_element.empty())
	{
		return -43;
	}

	m_other_cfg.winner_rank_reward_item_list_count = 0;
	while (!reward_item_element.empty())
	{
		if (m_other_cfg.winner_rank_reward_item_list_count >= CROSS_TUANZHAN_WINNER_REWARD_ITEM_MAX_COUNT)
		{
			return -44;
		}

		if (!m_other_cfg.winner_rank_reward_item_list[m_other_cfg.winner_rank_reward_item_list_count].ReadConfigNoCheck(reward_item_element))
		{
			return -45;
		}

		++ m_other_cfg.winner_rank_reward_item_list_count;
		reward_item_element = reward_item_element.next_sibling();
	}

	if (!PugiGetSubNodeValue(dataElement, "join_honor_factor_percent", m_other_cfg.join_honor_factor_percent) || m_other_cfg.join_honor_factor_percent < 0 || m_other_cfg.join_honor_factor_percent > 100)
	{
		return -46;
	}

	if (!PugiGetSubNodeValue(dataElement, "join_honor_max", m_other_cfg.join_honor_max) || m_other_cfg.join_honor_max < 0)
	{
		return -47;
	}

	if (!PugiGetSubNodeValue(dataElement, "join_honor_min", m_other_cfg.join_honor_min) || m_other_cfg.join_honor_min < 0 || m_other_cfg.join_honor_min > m_other_cfg.join_honor_max)
	{
		return -48;
	}
	
	if (!PugiGetSubNodeValue(dataElement, "rand_user_side_time", m_other_cfg.rand_user_side_time) || m_other_cfg.rand_user_side_time <= 0)
	{
		return -49;
	}

	return 0;
}

int CrossTuanzhanConfig::InitActivityOpenTimeCfg(PugiXmlNode RootElement)
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
		if (m_activity_open_time_cfg_list_count >= CROSS_TUANZHAN_OPEN_TIME_MAX_COUNT)
		{
			return -1;
		}

		ActivityOpenTimeInfo &open_cfg = m_activity_open_time_cfg_list[m_activity_open_time_cfg_list_count];

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
		
		++ m_activity_open_time_cfg_list_count;
		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int CrossTuanzhanConfig::InitPillaCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		if (m_pilla_cfg.pilla_count >= CROSS_TUANZHAN_PILLA_MAX_COUNT)
		{
			return -1;
		}

		CrossTuanzhanPillaCfg::CrossTuanzhanPillaConfigItem &pilla_cfg = m_pilla_cfg.pilla_list[m_pilla_cfg.pilla_count];

		if (!PugiGetSubNodeValue(dataElement, "index", pilla_cfg.index) || pilla_cfg.index < 0 || pilla_cfg.index >= CROSS_TUANZHAN_PILLA_MAX_COUNT)
		{
			return -10;
		}

		if (!PugiGetSubNodeValue(dataElement, "pilla_id", pilla_cfg.pilla_id) || pilla_cfg.pilla_id <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "pilla_side_0", pilla_cfg.pilla_side_0) || pilla_cfg.pilla_side_0 <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "pilla_side_1", pilla_cfg.pilla_side_1) || pilla_cfg.pilla_side_1 <= 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "x_pos", pilla_cfg.pilla_pos.x) || pilla_cfg.pilla_pos.x <= 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "y_pos", pilla_cfg.pilla_pos.y) || pilla_cfg.pilla_pos.y <= 0)
		{
			return -6;
		}

		++ m_pilla_cfg.pilla_count;
		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int CrossTuanzhanConfig::InitReliveCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		if (m_player_relive_pos_list_count >= CROSS_TUANZHAN_RELIVE_POS_MAX_COUNT)
		{
			return -1;
		}

		Posi &player_pos = m_player_relive_pos_list[m_player_relive_pos_list_count];

		if (!PugiGetSubNodeValue(dataElement, "x_pos", player_pos.x) || player_pos.x <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "y_pos", player_pos.y) || player_pos.y <= 0)
		{
			return -4;
		}

		++ m_player_relive_pos_list_count;
		dataElement = dataElement.next_sibling();
	}

	if (m_player_relive_pos_list_count <= 0)
	{
		return -5;
	}

	return 0;
}

int CrossTuanzhanConfig::InitScoreRewardCfg(PugiXmlNode  RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	int last_level = 0;
	int level_index = -1;
	int last_seq = -1;
	while (!dataElement.empty())
	{
		int level = 0;
		if (!PugiGetSubNodeValue(dataElement, "level", level) || level < 0 || level > MAX_ROLE_LEVEL)
		{
			return -1;
		}

		if (level != last_level)
		{
			++level_index;

			if (level_index >= CROSS_TUANZHAN_SCORE_REWARD_LEVEL_CFG_MAX)
			{
				return -2;
			}

			m_score_reward_level_cfg_count = level_index + 1;
		}

		int seq = 0;
		if (!PugiGetSubNodeValue(dataElement, "seq", seq) || seq < 0 || (level != last_level && seq != 0) || (level == last_level && seq != last_seq + 1))
		{
			return -1;
		}

		last_level = level;
		last_seq = seq;

		m_score_level_reward_cfg_list[level_index].level = level;

		CrossTuanzhanScoreRewardCfg &cfg = m_score_level_reward_cfg_list[level_index].score_reward_cfg_list[seq];
		cfg.seq = seq;

		if (!PugiGetSubNodeValue(dataElement, "need_score", cfg.need_score) || cfg.need_score <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "reward_currency", cfg.reward_currency) || cfg.reward_currency < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "cross_honor", cfg.cross_honor) || cfg.cross_honor < 0)
		{
			return -4;
		}

		PugiXmlNode reward_item_list_element = dataElement.child("reward_item_list");
		if (reward_item_list_element.empty())
		{
			return -11;
		}

		cfg.reward_item_count = 0;
		PugiXmlNode reward_item_element = reward_item_list_element.child("reward_item");
		while (!reward_item_element.empty())
		{
			if (cfg.reward_item_count >= MAX_ATTACHMENT_ITEM_NUM)
			{
				return -13;
			}

			if (!cfg.reward_item_list[cfg.reward_item_count].ReadConfigNoCheck(reward_item_element))
			{
				return -14;
			}

			++cfg.reward_item_count;
			reward_item_element = reward_item_element.next_sibling();
		}

		m_score_level_reward_cfg_list[level_index].score_reward_cfg_list_count = seq + 1;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int CrossTuanzhanConfig::InitRankRewardCfg(PugiXmlNode  RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	int last_level = 0;
	int level_index = -1;
	int cur_seq = -1;
	while (!dataElement.empty())
	{
		int level = 0;
		if (!PugiGetSubNodeValue(dataElement, "level", level) || level < 0 || level > MAX_ROLE_LEVEL)
		{
			return -1;
		}

		if (level != last_level)
		{
			++level_index;

			if (level_index >= CROSS_TUANZHAN_SCORE_REWARD_LEVEL_CFG_MAX)
			{
				return -2;
			}

			m_rank_reward_level_cfg_count = level_index + 1;

			cur_seq = 0;
		}

		last_level = level;

		if (cur_seq >= CROSS_TUANZHAN_RANK_REWARD_CFG_MAX)
		{
			return -100;
		}

		m_rank_reward_level_cfg_list[level_index].level = level;

		CrossTuanzhanRankRewardCfg &cfg = m_rank_reward_level_cfg_list[level_index].rank_reward_cfg_list[cur_seq++];

		if (!PugiGetSubNodeValue(dataElement, "rank", cfg.rank) || cfg.rank <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "reward_currency", cfg.reward_currency) || cfg.reward_currency < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "cross_honor", cfg.cross_honor) || cfg.cross_honor < 0)
		{
			return -5;
		}

		PugiXmlNode reward_item_list_element = dataElement.child("reward_item_list");
		if (reward_item_list_element.empty())
		{
			return -11;
		}

		PugiXmlNode reward_item_element = reward_item_list_element.child("reward_item");
		if (reward_item_element.empty())
		{
			return -12;
		}

		cfg.reward_item_count = 0;
		while (!reward_item_element.empty())
		{
			if (cfg.reward_item_count >= MAX_ATTACHMENT_ITEM_NUM)
			{
				return -13;
			}

			if (!cfg.reward_item_list[cfg.reward_item_count].ReadConfigNoCheck(reward_item_element))
			{
				return -14;
			}

			++cfg.reward_item_count;
			reward_item_element = reward_item_element.next_sibling();
		}

		m_rank_reward_level_cfg_list[level_index].rank_reward_cfg_list_count = cur_seq;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

bool CrossTuanzhanConfig::DayTimeFormatInvalid(int day_time)
{
	int hour = day_time / 100;
	int minute = day_time % 100;

	return (hour < 0 || hour >= 24 || minute < 0 || minute >= 60);
}

ActivityOpenTimeInfo *CrossTuanzhanConfig::GetActivityOpenTimeInfo(int index)
{
	if (index >= 0 && index < m_activity_open_time_cfg_list_count && index < CROSS_TUANZHAN_OPEN_TIME_MAX_COUNT)
	{
		return &m_activity_open_time_cfg_list[index];
	}

	return NULL;
}

Posi CrossTuanzhanConfig::GetRandomRelivePos()
{
	if (m_player_relive_pos_list_count <= 0)
	{
		return Posi(0, 0);
	}

	int rand_index = RandomNum(m_player_relive_pos_list_count);
	return m_player_relive_pos_list[rand_index];
}

int CrossTuanzhanConfig::GetOpenActivityConfig(ActivityOpenTimeInfo cfg_list[CROSS_TUANZHAN_OPEN_TIME_MAX_COUNT])
{
	memcpy(cfg_list, m_activity_open_time_cfg_list, sizeof(m_activity_open_time_cfg_list));
	return m_activity_open_time_cfg_list_count;
}

const CrossTuanzhanScoreRewardCfg * CrossTuanzhanConfig::GetCrossTuanzhanScoreRewardCfg(int role_level, int score)
{
	for (int index = m_score_reward_level_cfg_count - 1; index >= 0; --index)
	{
		if (role_level >= m_score_level_reward_cfg_list[index].level)
		{
			for (int i = m_score_level_reward_cfg_list[index].score_reward_cfg_list_count - 1; i >= 0; --i)
			{
				if (score >= m_score_level_reward_cfg_list[index].score_reward_cfg_list[i].need_score)
				{
					return &m_score_level_reward_cfg_list[index].score_reward_cfg_list[i];
				}
			}
		}
	}

	return NULL;
}

const CrossTuanzhanScoreRewardCfg * CrossTuanzhanConfig::GetCrossTuanzhanScoreRewardCfgBySeq(int role_level, int seq)
{
	if (seq < 0 || seq >= CROSS_TUANZHAN_SCORE_REWARD_CFG_MAX)
	{
		return NULL;
	}

	for (int index = m_score_reward_level_cfg_count - 1; index >= 0; --index)
	{
		if (role_level >= m_score_level_reward_cfg_list[index].level)
		{
			return &m_score_level_reward_cfg_list[index].score_reward_cfg_list[seq];
		}
	}

	return NULL;
}

const CrossTuanzhanRankRewardCfg * CrossTuanzhanConfig::GetCrossTuanzhanRankRewardCfg(int role_level, int rank)
{
	for (int index = m_rank_reward_level_cfg_count - 1; index >= 0; --index)
	{
		if (role_level >= m_rank_reward_level_cfg_list[index].level)
		{
			for (int i = m_rank_reward_level_cfg_list[index].rank_reward_cfg_list_count - 1; i >= 0; --i)
			{
				if (rank >= m_rank_reward_level_cfg_list[index].rank_reward_cfg_list[i].rank)
				{
					return &m_rank_reward_level_cfg_list[index].rank_reward_cfg_list[i];
				}
			}
		}
	}

	return NULL;
}
