#include "crossrankrewardconfig.hpp"
#include "servercommon/configcommon.h"
#include "../rankdef.hpp"
#include "../struct/shizhuangparam.hpp"

CrossRankRewardConfig::CrossRankRewardConfig()
{
}

CrossRankRewardConfig::~CrossRankRewardConfig()
{

}

bool CrossRankRewardConfig::Init(const std::string &path, std::string *err)
{
	int iRet = 0;
	char err_info[1024] = {0};

	TiXmlDocument document;
	if ("" == path || !document.LoadFile(path))
	{
		sprintf(err_info, "%s load CrossRankWeekRewardConfig fail !\n %s", path.c_str(), document.ErrorDesc());
		*err = err_info;
		return false;
	}

	TiXmlElement *RootElement = document.RootElement();
	if (NULL == RootElement)
	{
		*err = path + " xml root node error";
		return false;
	}

	{
		// ½±ÀøÅäÖÃ
		TiXmlElement *root_element = RootElement->FirstChildElement("reward_cfg");
		if (NULL == root_element)
		{
			*err = path + " xml not reward node ";
			return false;
		}

		iRet = this->InitRewardCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitRewardCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		// ½áËãÈÕÆÚÅäÖÃ
		TiXmlElement *root_element = RootElement->FirstChildElement("reward_date");
		if (NULL == root_element)
		{
			*err = path + " xml not reward_date node ";
			return false;
		}

		iRet = this->InitDateCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitDateCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		// ¿ç·þ¹«»á°ñÅäÖÃ
		TiXmlElement *root_element = RootElement->FirstChildElement("cross_guild_kill_boss_rank");
		if (NULL == root_element)
		{
			*err = path + " xml not cross_guild_kill_boss_rank node ";
			return false;
		}

		iRet = this->InitCrossGuildKillRankCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitCrossGuildKillRankCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		// ¿ç·þ°ñÏÞÖÆÅäÖÃ
		TiXmlElement *root_element = RootElement->FirstChildElement("cross_rank_limit");
		if (NULL == root_element)
		{
			*err = path + " xml not cross_rank_limit node ";
			return false;
		}

		iRet = this->InitCrossRankLimitCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitCrossRankLimitCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	return true;
}

const CrossRankReward * CrossRankRewardConfig::GetReward(int rank_type, int rank)
{
	auto iter = m_reward_map.find(rank_type);
	if (iter != m_reward_map.end())
	{
		const auto &cfg_vec = iter->second;
		for (const auto &cfg : cfg_vec)
		{
			if (rank <= cfg.rank)
			{
				return &cfg;
			}
		}
	}

	return nullptr;
}

const CrossRankRewardDateCfg * CrossRankRewardConfig::GetDateCfg(int rank_type)
{
	auto iter = m_date_cfg_map.find(rank_type);
	if (iter != m_date_cfg_map.end())
	{
		return &(iter->second);
	}

	return nullptr;
}

int CrossRankRewardConfig::InitRewardCfg(TiXmlElement *RootElement)
{
	TiXmlElement *dataElement = RootElement->FirstChildElement("data");
	if (NULL == dataElement)
	{
		return -100000;
	}

	while (NULL != dataElement)
	{
		CrossRankReward reward_cfg;

		if (!GetSubNodeValue(dataElement, "rank_type", reward_cfg.rank_type) || reward_cfg.rank_type < 0)
		{
			return -1;
		}

//		int reward_type = reward_cfg.rank_type;

		if (!GetSubNodeValue(dataElement, "rank_pos", reward_cfg.rank) || reward_cfg.rank < 0)
		{
			return -2;
		}

		if (GetSubNodeValue(dataElement, "img_item_id", reward_cfg.img_item_id))
		{
			if (reward_cfg.img_item_id < 0)
			{
				return -3;
			}
		}

		auto &cfg_vec = m_reward_map[reward_cfg.rank_type];

		if (!cfg_vec.empty() && cfg_vec.back().rank >= reward_cfg.rank)
		{
			return -4;
		}

		if (!GetSubNodeValue(dataElement, "part_type", reward_cfg.part_type) || reward_cfg.part_type < 0)
		{
			return -5;
		}

		if (!GetSubNodeValue(dataElement, "img_id", reward_cfg.img_id) || reward_cfg.img_id < 0)
		{
			return -6;
		}

		if (!GetSubNodeValue(dataElement, "limit_value", reward_cfg.limit_value) || reward_cfg.limit_value < 0)
		{
			return -7;
		}

		cfg_vec.push_back(reward_cfg);

		dataElement = dataElement->NextSiblingElement();
	}

	return 0;
}

int CrossRankRewardConfig::InitDateCfg(TiXmlElement *RootElement)
{
	TiXmlElement *dataElement = RootElement->FirstChildElement("data");
	if (NULL == dataElement)
	{
		return -100000;
	}

	while (NULL != dataElement)
	{
		int rank_type = -1;
		if (!GetSubNodeValue(dataElement, "rank_type", rank_type) || rank_type < 0)
		{
			return -1;
		}
		auto &date_cfg = m_date_cfg_map[rank_type];
		date_cfg.rank_type = rank_type;

		int day_index = 0;
		if (!GetSubNodeValue(dataElement, "weekday", day_index) || day_index <= 0 || day_index > DAY_PER_WEEK)
		{
			return -2;
		}

		if (0 != day_index)
		{
			if (date_cfg.CanReward(day_index))
			{
				return -3;
			}

			date_cfg.SetFlag(day_index);
		}

		dataElement = dataElement->NextSiblingElement();
	}

	return 0;
}

int CrossRankRewardConfig::InitCrossGuildKillRankCfg(TiXmlElement *RootElement)
{
	TiXmlElement *dataElement = RootElement->FirstChildElement("data");
	if (NULL == dataElement)
	{
		return -100000;
	}

	while (NULL != dataElement)
	{
		CrossGuildKillBossRankReward reward_cfg;

		if (!GetSubNodeValue(dataElement, "rank_type", reward_cfg.rank_type) || reward_cfg.rank_type < 0)
		{
			return -1;
		}

		if (!GetSubNodeValue(dataElement, "rank_pos", reward_cfg.rank_pos) || reward_cfg.rank_pos <= 0)
		{
			return -2;
		}

		auto &cfg_vec = m_cross_guild_kill_rank_map[reward_cfg.rank_type];

		if (!cfg_vec.empty() && cfg_vec.back().rank_pos >= reward_cfg.rank_pos)
		{
			return -100;
		}

		if (!GetSubNodeValue(dataElement, "tuanzhang_title_id", reward_cfg.tuanzhang_title_id) || reward_cfg.tuanzhang_title_id <= 0)
		{
			return -3;
		}

		if (!GetSubNodeValue(dataElement, "member_title_id", reward_cfg.member_title_id) || reward_cfg.member_title_id <= 0)
		{
			return -4;
		}

		if (!GetSubNodeValue(dataElement, "limit_value", reward_cfg.limit_value) || reward_cfg.limit_value < 0)
		{
			return -5;
		}

		cfg_vec.push_back(reward_cfg);

		dataElement = dataElement->NextSiblingElement();
	}

	return 0;
}

int CrossRankRewardConfig::InitCrossRankLimitCfg(TiXmlElement *RootElement)
{
	TiXmlElement *dataElement = RootElement->FirstChildElement("data");
	if (NULL == dataElement)
	{
		return -100000;
	}

	while (NULL != dataElement)
	{
		CrossRankLimitCfg limit_cfg;

		if (!GetSubNodeValue(dataElement, "rank_type", limit_cfg.rank_type) || limit_cfg.rank_type < 0)
		{
			return -1;
		}

		if (m_cross_rank_limit_map.end() != m_cross_rank_limit_map.find(limit_cfg.rank_type))
		{
			return -100;
		}

		if (!GetSubNodeValue(dataElement, "limit_open_day", limit_cfg.limit_open_day) || limit_cfg.limit_open_day < 0)
		{
			return -2;
		}

		m_cross_rank_limit_map[limit_cfg.rank_type] = limit_cfg;

		dataElement = dataElement->NextSiblingElement();
	}

	return 0;
}

bool CrossRankRewardConfig::GetRewardImageTypeById(ItemID img_item_id, int &part_type, int &img_id)
{
	auto iter = m_reward_map.begin();
	for (; iter != m_reward_map.end(); ++iter)
	{
		const auto &cfg_vec = iter->second;
		for (const auto &cfg : cfg_vec)
		{
			if (cfg.img_item_id == img_item_id)
			{
				part_type = cfg.part_type;
				img_id = cfg.img_id;
				return true;
			}
		}
	}

	return false;
}

const CrossGuildKillBossRankReward * CrossRankRewardConfig::GetCrossGuildRankReward(int rank_type, int rank)
{
	if (rank_type < 0 || rank_type >= CROSS_PERSON_RANK_TYPE_MAX || rank <= 0)
	{
		return nullptr;
	}

	auto iter = m_cross_guild_kill_rank_map.find(rank_type);
	if (iter != m_cross_guild_kill_rank_map.end())
	{
		const auto &cfg_vec = iter->second;
		for (const auto &cfg : cfg_vec)
		{
			if (rank <= cfg.rank_pos)
			{
				return &cfg;
			}
		}
	}

	return nullptr;
}

int CrossRankRewardConfig::GetCrossRankLimitOpenDay(int rank_type)
{
	if (rank_type < 0 || rank_type >= CROSS_PERSON_RANK_TYPE_MAX)
	{
		return 0;
	}

	auto it = m_cross_rank_limit_map.find(rank_type);
	if (it != m_cross_rank_limit_map.end())
	{
		return it->second.limit_open_day;
	}

	return 0;
}

