#include "crossrankweekrewardconfig.hpp"
#include "servercommon/configcommon.h"

CrossRankWeekRewardConfig::CrossRankWeekRewardConfig()
{
}

CrossRankWeekRewardConfig::~CrossRankWeekRewardConfig() 
{

}

bool CrossRankWeekRewardConfig::Init(const std::string &path, std::string *err)
{
	int iRet = 0;
	char err_info[1024] = {0};

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "CrossRankWeekRewardConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = path + " xml root node error";
		return false;
	}

	{
		// ½±ÀøÅäÖÃ
		PugiXmlNode root_element = RootElement.child("reward");
		if (root_element.empty())
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

	return true;
}

int CrossRankWeekRewardConfig::InitRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -100000;
	}


	int last_rank[CROSS_PERSON_RANK_TYPE_MAX];
	for (int i = 0; i < CROSS_PERSON_RANK_TYPE_MAX; ++ i)
	{
		last_rank[i] = -1;
	}

	while (!dataElement.empty())
	{
		CrossRankWeekReward reward_cfg;

		if (!PugiGetSubNodeValue(dataElement, "rank_type", reward_cfg.rank_type) || reward_cfg.rank_type < 0 || reward_cfg.rank_type >= CROSS_PERSON_RANK_TYPE_MAX)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "rank_idx", reward_cfg.rank) || reward_cfg.rank < 0 || reward_cfg.rank >= CROSS_RANK_WEEK_REWARD_MAX_ROLE_COUNT)
		{
			return -2;
		}
		if (reward_cfg.rank != last_rank[reward_cfg.rank_type] + 1)
		{
			return -3;
		}
		last_rank[reward_cfg.rank_type] = reward_cfg.rank;

		PugiXmlNode reward_item_element = dataElement.child("reward_item");
		if (reward_item_element.empty())
		{
			return -4;
		}

		if (!reward_cfg.reward_item.ReadConfig(reward_item_element))
		{
			return -5;
		}

		m_reward_list[reward_cfg.rank_type][reward_cfg.rank] = reward_cfg;
		dataElement = dataElement.next_sibling();
	}

	return 0;
}

const CrossRankWeekReward * CrossRankWeekRewardConfig::GetReward(int rank_type, int rank)
{
	if (rank_type >= 0 && rank_type < CROSS_PERSON_RANK_TYPE_MAX && rank >= 0 && rank < CROSS_RANK_WEEK_REWARD_MAX_ROLE_COUNT)
	{
		if (m_reward_list[rank_type][rank].IsValid())
		{
			return &m_reward_list[rank_type][rank];
		}
	}

	return NULL;
}


