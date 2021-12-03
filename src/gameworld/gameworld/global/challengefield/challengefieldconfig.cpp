#include "challengefieldconfig.hpp"

#include "servercommon/configcommon.h"
#include "checkresourcecenter.hpp"
#include "engineadapter.h"
#include "servercommon/campdef.hpp"
#include <algorithm>
#include "servercommon/namefilter.hpp"

ChallengeFieldConfig::ChallengeFieldConfig() : m_rank_reward_time_cfgcount(0), m_best_rank_pos_cfg_count(0)
{

}

ChallengeFieldConfig::~ChallengeFieldConfig()
{

}

bool ChallengeFieldConfig::Init(std::string path, std::string *err)
{
	int iRet = 0;
	char err_info[1024] = {0};

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "ChallengeFieldConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = path + " xml root node error";
		return false;
	}

	{
		// 杂项配置
		PugiXmlNode root_element = RootElement.child("other");
		if (root_element.empty())
		{
			*err = path + " xml not other node ";
			return false;
		}

		iRet = this->InitOtherCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitOtherCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		// 积分奖励
		PugiXmlNode root_element = RootElement.child("jifen_reward");
		if (root_element.empty())
		{
			*err = path + " xml not jifen_reward node ";
			return false;
		}

		iRet = this->InitJifenRewardCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitJifenRewardCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		// 积分具体奖励
		PugiXmlNode root_element = RootElement.child("jifen_reward_detail");
		if (root_element.empty())
		{
			*err = path + " xml not jifen_reward_detail node ";
			return false;
		}

		iRet = this->InitJifenRewardDetailCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitJifenRewardDetailCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}
	
	{
		// 幸运奖励
		PugiXmlNode root_element = RootElement.child("luck_reward");
		if (root_element.empty())
		{
			*err = path + " xml not luck_reward node ";
			return false;
		}

		iRet = this->InitLuckyRewardCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitLuckyRewardCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		// 排名奖励
		PugiXmlNode root_element = RootElement.child("rank_reward");
		if (root_element.empty())
		{
			*err = path + " xml not rank_reward node ";
			return false;
		}

		iRet = this->InitRankRewardCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitRankRewardCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		// 排名奖励时间配置
		PugiXmlNode root_element = RootElement.child("rank_reward_time_cfg");
		if (root_element.empty())
		{
			*err = path + " xml not rank_reward_time_cfg node ";
			return false;
		}

		iRet = this->InitRankRewardTimeCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitRankRewardTimeCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

 	{
 		// 历史排名奖励时间配置
 		PugiXmlNode root_element = RootElement.child("history_rank_reward");
 		if (root_element.empty())
 		{
 			*err = path + " xml not history_rank_reward node ";
 			return false;
 		}
 
 		iRet = this->InitHistoryRewardConfig(root_element);
 		if (0 != iRet)
 		{
 			sprintf(err_info, "%s InitHistoryRewardConfig fail %d ", path.c_str(), iRet);
 			*err = err_info;
 			return false;
 		}
 	}

	{
		// 机器人配置
		PugiXmlNode root_element = RootElement.child("robot_config");
		if (root_element.empty())
		{
			*err = path + " xml not robot_config node ";
			return false;
		}

		iRet = this->InitRobotConfig(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitRobotConfig fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	// 跨服排名奖励
	{
		PugiXmlNode root_element = RootElement.child("cross_rank_reward");
		if (root_element.empty())
		{
			*err = path + " xml not cross_rank_reward node ";
			return false;
		}

		iRet = this->InitCrossRankRewardCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitCrossRankRewardCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		PugiXmlNode root_element = RootElement.child("cross_rank_other_reward");
		if (root_element.empty())
		{
			*err = path + " xml not cross_rank_other_reward node ";
			return false;
		}

		iRet = this->InitCrossRankOtherRewardCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitCrossRankOtherRewardCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	return true;
}

const ChallengeFieldJifenRewardConfig::ConfigItem * ChallengeFieldConfig::GetJifenCfg(int seq)
{
	if (seq >= 0 && seq < CHALLENGE_FIELD_JIFEN_REWARD_MAX_COUNT && seq < m_jifen_reward_cfg.cfg_count)
	{
		return &m_jifen_reward_cfg.cfg_list[seq];
	}

	return NULL;
}

const ChallengeFieldLuckRewardConfig::ConfigItem * ChallengeFieldConfig::GetLuckyRewardCfg(int seq)
{
	if (seq >= 0 && seq < CHALLENGE_FIELD_LUCK_RANK_POS_MAX_COUNT && seq < m_lucky_reward_cfg.cfg_count)
	{
		return &m_lucky_reward_cfg.cfg_list[seq];
	}

	return NULL;
}

const ChallengeFieldRankRewardConfig::ConfigItem * ChallengeFieldConfig::GetRankRewardCfg(int open_game_day, int seq)
{
	auto it = m_rank_reward_cfg.find(open_game_day);
	if (it == m_rank_reward_cfg.end())
	{
		return NULL;
	}

	if (seq >= 0 && seq < ChallengeFieldRankRewardConfig::CONFIG_ITEM_MAX_COUNT && seq < it->second.cfg_count)
	{
		return &(it->second.cfg_list[seq]);
	}

	return NULL;
}

const ChallengeFieldRankRewardConfig::ConfigItem * ChallengeFieldConfig::GetRankRewardCfgByRank(int open_game_day, int rank)
{
	auto it = m_rank_reward_cfg.find(open_game_day);
	if (it == m_rank_reward_cfg.end())
	{
		return NULL;
	}

	for (int seq = 0; seq < it->second.cfg_count && seq < ChallengeFieldRankRewardConfig::CONFIG_ITEM_MAX_COUNT; seq ++)
	{
		if (rank >= it->second.cfg_list[seq].min_rank_pos && rank <= it->second.cfg_list[seq].max_rank_pos)
		{
			return &(it->second.cfg_list[seq]);
		}
	}

	return NULL;
}

int ChallengeFieldConfig::GetRankRewardCfgCount(int open_game_day)
{
	auto it = m_rank_reward_cfg.find(open_game_day);
	if (it != m_rank_reward_cfg.end())
	{
		return it->second.cfg_count;
	}

	return 0;
}

unsigned int ChallengeFieldConfig::CalNextRankRewadTime(int *reward_percent)
{
	const static int ONE_DAY_INTERVAL = 24 * 3600;

	time_t now_time = EngineAdapter::Instance().Time();
	int min_interval = ONE_DAY_INTERVAL;
	int percent = 0;

	{
		for (int i = 0; i < m_rank_reward_time_cfgcount; ++ i)
		{
			int interval = EngineAdapter::Instance().NextDayInterval(m_rank_reward_time_cfglist[i].hour, m_rank_reward_time_cfglist[i].minute, 0);
			if (interval <= min_interval)
			{
				min_interval = interval;
				percent = m_rank_reward_time_cfglist[i].percent;
			}
		}
	}

	unsigned int next_time = static_cast<unsigned int>(now_time + min_interval);
	if (NULL != reward_percent) *reward_percent = percent;

	return next_time;
}

const ChallengeFieldRankRewardTime * ChallengeFieldConfig::GetFirstRankRewardTime()
{
	return &m_rank_reward_time_cfglist[0];
}

const ChallengeFieldRankRewardTime * ChallengeFieldConfig::GetLastEndRankRewardTime()
{
	int index = m_rank_reward_time_cfgcount - 1;
	if (index < 0 || index >= RANK_REWARD_TIME_MAX_COUNT)
	{
		return nullptr;
	}

	return &m_rank_reward_time_cfglist[index];
}

ChallengeFieldHistoryRankRewardConfig::ConfigItem * ChallengeFieldConfig::GetHistoryRankRewardCfg(int rank_pos)
{
	if (rank_pos > m_history_reward_cfg.max_rank_pos)
	{
		rank_pos = m_history_reward_cfg.max_rank_pos;
	}

	if (rank_pos < m_history_reward_cfg.min_rank_pos)
	{
		rank_pos = m_history_reward_cfg.min_rank_pos;
	}

	for (int i = 0; i < m_history_reward_cfg.cfg_count; ++ i)
	{
		if (rank_pos <= m_history_reward_cfg.cfg_list[i].best_rank_pos)
		{
			return &m_history_reward_cfg.cfg_list[i];
		}
	}

	return NULL;
}

const std::vector<ChallengeFieldRobotConfig> & ChallengeFieldConfig::GetChallengeFieldRobotName()
{
	return m_robot_name_vector;
}

int ChallengeFieldConfig::InitOtherCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -10000;
	}

	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -20000;
	}

	if (!PugiGetSubNodeValue(data_element, "scene_id", m_other_cfg.scene_id) || m_other_cfg.scene_id <= 0)
	{
		return -1;
	}
	CheckResourceCenter::Instance().GetSceneCheck()->Add(m_other_cfg.scene_id);

	if (!PugiGetSubNodeValue(data_element, "sponsor_pos_x", m_other_cfg.sponsor_pos.x) || m_other_cfg.sponsor_pos.x <= 0)
	{
		return -2;
	}

	if (!PugiGetSubNodeValue(data_element, "sponsor_pos_y", m_other_cfg.sponsor_pos.y) || m_other_cfg.sponsor_pos.y <= 0)
	{
		return -3;
	}

	if (!PugiGetSubNodeValue(data_element, "opponent_pos_x", m_other_cfg.opponent_pos.x) || m_other_cfg.opponent_pos.x <= 0)
	{
		return -4;
	}

	if (!PugiGetSubNodeValue(data_element, "opponent_pos_y", m_other_cfg.opponent_pos.y) || m_other_cfg.opponent_pos.y <= 0)
	{
		return -5;
	}

	if (!PugiGetSubNodeValue(data_element, "free_join_times_before", m_other_cfg.free_join_times_before) || m_other_cfg.free_join_times_before < 0)
	{
		return -6;
	}

	if (!PugiGetSubNodeValue(data_element, "free_join_times", m_other_cfg.free_join_times) || m_other_cfg.free_join_times < 0)
	{
		return -7;
	}

	if (!PugiGetSubNodeValue(data_element, "buy_join_times_cost", m_other_cfg.buy_join_times_cost) || m_other_cfg.buy_join_times_cost <= 0)
	{
		return -8;
	}

// 	if (!PugiGetSubNodeValue(data_element, "win_add_jifen", m_other_cfg.win_add_jifen) || m_other_cfg.win_add_jifen < 0)
// 	{
// 		return -8;
// 	}
// 
// 	if (!PugiGetSubNodeValue(data_element, "lose_add_jifen", m_other_cfg.lose_add_jifen) || m_other_cfg.lose_add_jifen < 0)
// 	{
// 		return -9;
// 	}

	if (!PugiGetSubNodeValue(data_element, "win_add_exp", m_other_cfg.win_add_exp) || m_other_cfg.win_add_exp <= 0)
	{
		return -101;
	}

	if (!PugiGetSubNodeValue(data_element, "lose_add_exp", m_other_cfg.lose_add_exp) || m_other_cfg.lose_add_exp <= 0)
	{
		return -102;
	}

	if (!PugiGetSubNodeValue(data_element, "win_add_guanghui", m_other_cfg.win_add_guanghui) || m_other_cfg.win_add_guanghui <= 0)
	{
		return -10;
	}

	if (!PugiGetSubNodeValue(data_element, "lose_add_guanghui", m_other_cfg.lose_add_guanghui) || m_other_cfg.lose_add_guanghui <= 0)
	{
		return -11;
	}

	if (!PugiGetSubNodeValue(data_element, "buy_buff_gold", m_other_cfg.buy_buff_gold) || m_other_cfg.buy_buff_gold <= 0)
	{
		return -12;
	}

	if (!PugiGetSubNodeValue(data_element, "buy_buff_times_max", m_other_cfg.buy_buff_times_max) || m_other_cfg.buy_buff_times_max <= 0)
	{
		return -13;
	}

	if (!PugiGetSubNodeValue(data_element, "buff_add_maxhp_per", m_other_cfg.buff_add_maxhp_per) || m_other_cfg.buff_add_maxhp_per <= 0)
	{
		return -14;
	}

	if (!PugiGetSubNodeValue(data_element, "buff_add_gongji_per", m_other_cfg.buff_add_gongji_per) || m_other_cfg.buff_add_gongji_per <= 0)
	{
		return -15;
	}

	if (!PugiGetSubNodeValue(data_element, "rank_reward_count", m_other_cfg.rank_reward_count) || m_other_cfg.rank_reward_count <= 0)
	{
		return -16;
	}

	if (!PugiGetSubNodeValue(data_element, "rank_reward_wing_id", m_other_cfg.rank_reward_wing_id) || m_other_cfg.rank_reward_wing_id < 0)
	{
		return -17;
	}

	if (!PugiGetSubNodeValue(data_element, "exp_factor_win", m_other_cfg.win_exp_factor) || m_other_cfg.win_exp_factor < 0)
	{
		return -18;
	}

	if (!PugiGetSubNodeValue(data_element, "exp_factor_lose", m_other_cfg.lose_exp_factor) || m_other_cfg.lose_exp_factor < 0)
	{
		return -19;
	}

	if (!PugiGetSubNodeValue(data_element, "open_dur_day", m_other_cfg.open_dur_day) || m_other_cfg.open_dur_day <= 0)
	{
		return -20;
	}

	if (!PugiGetSubNodeValue(data_element, "change_free_times_create_day", m_other_cfg.change_free_times_create_day) || m_other_cfg.change_free_times_create_day < 0)
	{
		return -21;
	}

	if (!PugiGetSubNodeValue(data_element, "cross_scene_id", m_cross_scene_id) || m_other_cfg.scene_id <= 0)
	{
		return -22;
	}
	CheckResourceCenter::Instance().GetSceneCheck()->Add(m_cross_scene_id);

	if (!PugiGetSubNodeValue(data_element, "cross_reward_weekday", m_cross_reward_weekday) || m_cross_reward_weekday < 0)
	{
		return -23;
	}

	return 0;
}

int ChallengeFieldConfig::InitJifenRewardCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -10000;
	}

	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -20000;
	}

	m_jifen_reward_cfg.cfg_count = 0;

	int last_need_jifen = 0;

	while (!data_element.empty())
	{
		int seq = 0;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq != m_jifen_reward_cfg.cfg_count || seq >= CHALLENGE_FIELD_JIFEN_REWARD_MAX_COUNT)
		{
			return -1;
		}

		ChallengeFieldJifenRewardConfig::ConfigItem &cfg_item = m_jifen_reward_cfg.cfg_list[m_jifen_reward_cfg.cfg_count];
		cfg_item.seq = seq;

		if (!PugiGetSubNodeValue(data_element, "need_jifen", cfg_item.need_jifen) || cfg_item.need_jifen < last_need_jifen)
		{
			return -2;
		}
		last_need_jifen = cfg_item.need_jifen;

		++ m_jifen_reward_cfg.cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}


int ChallengeFieldConfig::InitJifenRewardDetailCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -10000;
	}

	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -20000;
	}

	int last_role_level[CHALLENGE_FIELD_JIFEN_REWARD_MAX_COUNT];
	memset(last_role_level, 0, sizeof(last_role_level));

	while (!data_element.empty())
	{
		int seq = 0;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq < 0 || seq >= m_jifen_reward_cfg.cfg_count)
		{
			return -1;
		}

		ChallengeFieldJifenRewardConfig::ConfigItem &cfg_item = m_jifen_reward_cfg.cfg_list[seq];

		if (cfg_item.detail_reward_cfg_count >= ChallengeFieldJifenRewardConfig::DETAIL_REWARD_CFG_MAX_COUNT)
		{
			return -2;
		}

		ChallengeFieldJifenRewardConfig::DetailRewardConfig &reward_detail = cfg_item.detail_reward_cfglist[cfg_item.detail_reward_cfg_count];

		if (!PugiGetSubNodeValue(data_element, "role_level", reward_detail.role_level) || reward_detail.role_level <= last_role_level[seq])
		{
			return -3;
		}
		last_role_level[seq] = reward_detail.role_level;

		if (!PugiGetSubNodeValue(data_element, "reward_guanghui", reward_detail.reward_guanghui) || reward_detail.reward_guanghui < 0)
		{
			return -4;
		}

		PugiXmlNode reward_item_element = data_element.child("reward_item");
		if (reward_item_element.empty())
		{
			return -6;
		}

		bool has_item = false;

		ItemID item_id = 0;
		if (PugiGetSubNodeValue(reward_item_element, "item_id", item_id))
		{
			if (!reward_detail.reward_item.ReadConfig(reward_item_element))
			{
				reward_detail.reward_item.item_id = 0;
				return -7;
			}

			has_item = true;
		}

		if (!has_item && 0 == reward_detail.reward_guanghui)
		{
			return -8;
		}

		++ cfg_item.detail_reward_cfg_count;
		data_element = data_element.next_sibling();
	}

	for (int i = 0; i < m_jifen_reward_cfg.cfg_count; ++ i)
	{
		const ChallengeFieldJifenRewardConfig::ConfigItem &cfg_item = m_jifen_reward_cfg.cfg_list[i];

		if (cfg_item.detail_reward_cfg_count <= 0 || 1 != cfg_item.detail_reward_cfglist[0].role_level)
		{
			return -3000;
		}
	}

	return 0;
}

int ChallengeFieldConfig::InitLuckyRewardCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -10000;
	}

	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -20000;
	}

	m_lucky_reward_cfg.cfg_count = 0;

	while (!data_element.empty())
	{
		int seq = 0;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq != m_lucky_reward_cfg.cfg_count || seq >= CHALLENGE_FIELD_LUCK_RANK_POS_MAX_COUNT)
		{
			return -1;
		}

		ChallengeFieldLuckRewardConfig::ConfigItem &cfg_item = m_lucky_reward_cfg.cfg_list[m_lucky_reward_cfg.cfg_count];
		cfg_item.seq = seq;

		if (!PugiGetSubNodeValue(data_element, "reward_guanghui", cfg_item.reward_guanghui) || cfg_item.reward_guanghui <= 0)
		{
			return -2;
		}

		++ m_lucky_reward_cfg.cfg_count;
		data_element = data_element.next_sibling();
	}

	if (m_lucky_reward_cfg.cfg_count != CHALLENGE_FIELD_LUCK_RANK_POS_MAX_COUNT)
	{
		return -30000;
	}

	return 0;
}

int ChallengeFieldConfig::InitRankRewardCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -10000;
	}

	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -20000;
	}

	int open_game_day = 0;   // 开服天数，最大只能对应竞技场结束天数
	int last_open_game_day = 0;

	m_rank_reward_cfg.clear();
	
	int last_rank_pos = -1;

	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "open_game_day", open_game_day) || open_game_day <= 0 || open_game_day > m_other_cfg.open_dur_day)
		{
			return -999;
		}

		if (last_open_game_day != open_game_day)
		{
			last_open_game_day = open_game_day;
			last_rank_pos = -1;
		}

		ChallengeFieldRankRewardConfig &rank_cfg = m_rank_reward_cfg[open_game_day];

		if (rank_cfg.cfg_count >= ChallengeFieldRankRewardConfig::CONFIG_ITEM_MAX_COUNT)
		{
			return -1;
		}

		ChallengeFieldRankRewardConfig::ConfigItem &cfg_item = rank_cfg.cfg_list[rank_cfg.cfg_count];

		if (!PugiGetSubNodeValue(data_element, "min_rank_pos", cfg_item.min_rank_pos) || cfg_item.min_rank_pos != last_rank_pos + 1)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "max_rank_pos", cfg_item.max_rank_pos) || cfg_item.max_rank_pos < cfg_item.min_rank_pos)
		{
			return -3;
		}
		last_rank_pos = cfg_item.max_rank_pos;

		if (!PugiGetSubNodeValue(data_element, "reward_guanghui", cfg_item.reward_guanghui) || cfg_item.reward_guanghui < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "title_id", cfg_item.title_id) || cfg_item.title_id < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "wing_id", cfg_item.wing_id) || cfg_item.wing_id < 0)
		{
			return -6;
		}

		int count = ItemConfigData::ReadConfigList(data_element, "reward_item", cfg_item.reward_item_list, MAX_ATTACHMENT_ITEM_NUM);
		if (count < 0)
		{
			return -100 + count;
		}

		cfg_item.reward_count = count;

		++rank_cfg.cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int ChallengeFieldConfig::InitRankRewardTimeCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -10000;
	}

	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -20000;
	}

	m_rank_reward_time_cfgcount = 0;

	while (!data_element.empty())
	{
		if (m_rank_reward_time_cfgcount >= RANK_REWARD_TIME_MAX_COUNT)
		{
			return -1;
		}

		ChallengeFieldRankRewardTime &cfg_item = m_rank_reward_time_cfglist[m_rank_reward_time_cfgcount];

		if (!PugiGetSubNodeValue(data_element, "honor", cfg_item.hour) || cfg_item.hour < 0 || cfg_item.hour >= 24)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "min", cfg_item.minute) || cfg_item.minute < 0 || cfg_item.minute >= 60)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "percent", cfg_item.percent) || cfg_item.percent <= 0)
		{
			return -4;
		}

		++ m_rank_reward_time_cfgcount;
		data_element = data_element.next_sibling();
	}

	if (m_rank_reward_time_cfgcount > 1)
	{
		for (int i = 1; i < m_rank_reward_time_cfgcount; ++ i)
		{
			bool is_invalid = false; // 越靠后的配置，时间应该越大
			{
				if (m_rank_reward_time_cfglist[i - 1].hour != m_rank_reward_time_cfglist[i].hour)
				{
					if (m_rank_reward_time_cfglist[i -1].hour > m_rank_reward_time_cfglist[i].hour)
					{
						is_invalid = true;
					}
				}
				else
				{
					if (m_rank_reward_time_cfglist[i - 1].minute >= m_rank_reward_time_cfglist[i].minute)
					{
						is_invalid = true;
					}
				}
			}

			if (is_invalid)
			{
				return -20000;
			}
		}
	}

	return 0;
}

int ChallengeFieldConfig::InitHistoryRewardConfig(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -10000;
	}

	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -20000;
	}

	int last_rank_pos = CHALLENGE_FIELD_MAX_USER + 1;
	int last_index = -1;

	while ( !data_element.empty())
	{
		ChallengeFieldBestRankPosCfg cfg;
		int index;
		if (!PugiGetSubNodeValue(data_element, "index", index) || (last_index + 1) != index || index >= CHALLENGE_FIELD_BEST_RANK_CFG_MAX_COUNT)
		{
			return -1;
		}
		last_index = index;

		if (!PugiGetSubNodeValue(data_element, "best_rank_pos", cfg.rank_pos) || cfg.rank_pos >= last_rank_pos || cfg.rank_pos < 0)
		{
			return -2;
		}
		last_rank_pos = cfg.rank_pos;

		if (!PugiGetSubNodeValue(data_element, "maxhp", cfg.maxhp) || cfg.maxhp < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "gongji", cfg.gongji) || cfg.gongji < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "fangyu", cfg.fangyu) || cfg.fangyu < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "reward_guanghui", cfg.reward_guanghui) || cfg.reward_guanghui < 0)
		{
			return -6;
		}

		int item_count = ItemConfigData::ReadConfigList(data_element, "reward_item", cfg.reward_item, MAX_ATTACHMENT_ITEM_NUM);
		if (item_count < 0)
		{
			return -100 + item_count;
		}

		++m_best_rank_pos_cfg_count;
		m_best_rank_pos_cfg_list[index] = cfg;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int ChallengeFieldConfig::InitRobotConfig(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -10000;
	}

	m_robot_name_vector.clear();

	std::vector<std::string> prefix_name_vector;		// 通用前缀
	std::vector<std::string> common_suffix_vector;		// 通用后缀
	std::vector<std::string> male_suffix_vector;		// 男性专用后缀
	std::vector<std::string> female_suffix_vector;		// 女性专用后缀

	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -20000;
	}

	while (!data_element.empty())
	{
		// 加载通用前缀
		std::string commom_prefix_name;
		if (PugiGetSubNodeValue(data_element, "prefix", commom_prefix_name))
		{
			prefix_name_vector.push_back(commom_prefix_name);
		}

		// 加载通用后缀
		std::string common_suffix_name;
		if (PugiGetSubNodeValue(data_element, "suffix", common_suffix_name))
		{
			common_suffix_vector.push_back(common_suffix_name);
		}

		// 加载男性专用后缀
		std::string male_suffix_name;
		if (PugiGetSubNodeValue(data_element, "malesuffix", male_suffix_name))
		{
			male_suffix_vector.push_back(male_suffix_name);
		}

		// 加载女性专用后缀
		std::string female_suffix_name;
		if (PugiGetSubNodeValue(data_element, "femalesuffix", female_suffix_name))
		{
			female_suffix_vector.push_back(female_suffix_name);
		}

		data_element = data_element.next_sibling();
	}

	for (std::vector<std::string>::const_iterator prefix_it = prefix_name_vector.begin(); prefix_it != prefix_name_vector.end(); ++ prefix_it)
	{
		for (std::vector<std::string>::const_iterator it = common_suffix_vector.begin(); it != common_suffix_vector.end(); ++ it)
		{
			ChallengeFieldRobotConfig robot_cfg;
			SNPRINTF(robot_cfg.robot_name, sizeof(robot_cfg.robot_name), "%s%s", prefix_it->c_str(), it->c_str());
			if (NameFilter::Instance().HasSensitiveWord(sizeof(robot_cfg.robot_name), robot_cfg.robot_name))
			{
				return -5;
			}

			robot_cfg.is_sex_common = true;
			robot_cfg.robot_sex = SEX_MAX;
			m_robot_name_vector.push_back(robot_cfg);
		}

		for (std::vector<std::string>::const_iterator it = male_suffix_vector.begin(); it != male_suffix_vector.end(); ++ it)
		{
			ChallengeFieldRobotConfig robot_cfg;
			SNPRINTF(robot_cfg.robot_name, sizeof(robot_cfg.robot_name), "%s%s", prefix_it->c_str(), it->c_str());
			if (NameFilter::Instance().HasSensitiveWord(sizeof(robot_cfg.robot_name), robot_cfg.robot_name))
			{
				return -6;
			}

			robot_cfg.is_sex_common = false;
			robot_cfg.robot_sex = MALE;
			m_robot_name_vector.push_back(robot_cfg);
		}

		for (std::vector<std::string>::const_iterator it = female_suffix_vector.begin(); it != female_suffix_vector.end(); ++ it)
		{
			ChallengeFieldRobotConfig robot_cfg;
			SNPRINTF(robot_cfg.robot_name, sizeof(robot_cfg.robot_name), "%s%s", prefix_it->c_str(), it->c_str());
			if (NameFilter::Instance().HasSensitiveWord(sizeof(robot_cfg.robot_name), robot_cfg.robot_name))
			{
				return -7;
			}

			robot_cfg.is_sex_common = false;
			robot_cfg.robot_sex = FEMALE;
			m_robot_name_vector.push_back(robot_cfg);
		}
	}

	random_shuffle(m_robot_name_vector.begin(), m_robot_name_vector.end());

	if (static_cast<int>(m_robot_name_vector.size()) < ChallengeFieldRobotConfig::CONFIG_ITEM_MAX_COUNT)
	{
		return -8;
	}

	return 0;
}

int ChallengeFieldConfig::InitCrossRankRewardCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -10000;
	}

	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -20000;
	}

	int last_rank_pos = 0;

	while (!data_element.empty())
	{
		CrossRankRewardCfg rank_cfg;

		if (!PugiGetSubNodeValue(data_element, "min_rank_pos", rank_cfg.min_rank_pos) || rank_cfg.min_rank_pos != last_rank_pos + 1)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "max_rank_pos", rank_cfg.max_rank_pos) || rank_cfg.max_rank_pos < rank_cfg.min_rank_pos)
		{
			return -3;
		}
		last_rank_pos = rank_cfg.max_rank_pos;

		if (!PugiGetSubNodeValue(data_element, "img_id", rank_cfg.img_id) || rank_cfg.img_id < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "title_id", rank_cfg.title_id) || rank_cfg.title_id < 0)
		{
			return -5;
		}

		m_cross_rank_reward_vec.push_back(rank_cfg);

		data_element = data_element.next_sibling();
	}

	return 0;
}

int ChallengeFieldConfig::InitCrossRankOtherRewardCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -10000;
	}

	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -20000;
	}

	int last_rank_pos = 0;

	while (!data_element.empty())
	{
		CrossRankOtherRewardCfg rank_cfg;

		if (!PugiGetSubNodeValue(data_element, "min_rank_pos", rank_cfg.min_rank_pos) || rank_cfg.min_rank_pos != last_rank_pos + 1)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "max_rank_pos", rank_cfg.max_rank_pos) || rank_cfg.max_rank_pos < rank_cfg.min_rank_pos)
		{
			return -3;
		}
		last_rank_pos = rank_cfg.max_rank_pos;

		if (!PugiGetSubNodeValue(data_element, "reward_hunjing", rank_cfg.reward_hunjing) || rank_cfg.reward_hunjing < 0)
		{
			return -4;
		}

		m_cross_rank_other_reward_vec.push_back(rank_cfg);

		data_element = data_element.next_sibling();
	}

	return 0;
}

const ChallengeFieldJifenRewardConfig::DetailRewardConfig * ChallengeFieldJifenRewardConfig::ConfigItem::GetDetailReward(int role_level) const
{
	for (int i = detail_reward_cfg_count - 1; i >= 0; -- i)
	{
		if (role_level >= detail_reward_cfglist[i].role_level)
		{
			return &detail_reward_cfglist[i];
		}
	}

	return NULL;
}

const ChallengeFieldBestRankPosCfg * ChallengeFieldConfig::GetBestRankPosCfgByLeveL(int level)
{
	if (level < 0 || level >= m_best_rank_pos_cfg_count || level >= CHALLENGE_FIELD_BEST_RANK_CFG_MAX_COUNT)
	{
		return nullptr;
	}

	return &m_best_rank_pos_cfg_list[level];
}

int ChallengeFieldConfig::GetRoleFreeJoinTimes(const int level)
{
	if (level > 200)
	{
		return m_other_cfg.free_join_times;
	}
	else
	{
		return m_other_cfg.free_join_times_before;
	}
}

int ChallengeFieldConfig::GetRoleFreeJoinTimesByCreateTime(const time_t create_role_time)
{
	// 创建角色第几天
	int create_day = GetDayIndex(create_role_time, EngineAdapter::Instance().Time()) + 1;

	// 超过限制天数变更免费参与次数
	if (create_day >= m_other_cfg.change_free_times_create_day)
	{
		return m_other_cfg.free_join_times;
	}
	else
	{
		return m_other_cfg.free_join_times_before;
	}
}

bool ChallengeFieldConfig::IsCrossRewardDay(int day_index)
{
	if (day_index == m_cross_reward_weekday)
	{
		return true;
	}

	return false;
}

const CrossRankRewardCfg * ChallengeFieldConfig::GetCrossRankReward(int rank)
{
	for (auto &it : m_cross_rank_reward_vec)
	{
		if (it.min_rank_pos <= rank && rank <= it.max_rank_pos)
		{
			return &it;
		}
	}

	return nullptr;
}

const CrossRankOtherRewardCfg * ChallengeFieldConfig::GetCrossRankOtherReward(int rank)
{
	for (auto &it : m_cross_rank_other_reward_vec)
	{
		if (it.min_rank_pos <= rank && rank <= it.max_rank_pos)
		{
			return &it;
		}
	}

	return nullptr;
}

