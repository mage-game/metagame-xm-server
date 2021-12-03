#include "crossrandactivityconfig.hpp"
#include "gamecommon.h"
#include "servercommon/servercommon.h"
#include "item/itempool.h"
#include "servercommon/configcommon.h"

#include <string>
#include "config/logicconfigmanager.hpp"
#include "config/sharedconfig/sharedconfig.h"
#include "scene/activityshadow/activityshadow.hpp"

CrossRandActivityConfig::CrossRandActivityConfig() : m_chongzhi_rank_max_rank(0)
{
}

CrossRandActivityConfig::~CrossRandActivityConfig()
{

}

bool CrossRandActivityConfig::Init(const std::string &configname, std::string *err)
{
	int iRet = 0;
	char errinfo[1024] = {0};
	PugiXmlDocument document;

	PUGI_XML_LOAD_AND_CHECK(document, configname, "BuildTowerFbConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = configname + ": xml root node error.";
		return false;
	}

	{
		// 其他配置
		PugiXmlNode root_element = RootElement.child("other");
		if (root_element.empty())
		{
			*err = configname + ": xml no other node";
			return false;
		}

		iRet = this->InitOtherCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitOtherCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 充值排行配置
		PugiXmlNode root_element = RootElement.child("chongzhi_rank");
		if (root_element.empty())
		{
			*err = configname + ": xml no chongzhi_rank node";
			return false;
		}

		iRet = this->InitChongzhiRankCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitChongzhiRankCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 充值排行配置
		PugiXmlNode root_element = RootElement.child("consume_rank");
		if (root_element.empty())
		{
			*err = configname + ": xml no consume_rank node";
			return false;
		}

		iRet = this->InitConsumeRankCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitConsumeRankCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

const CrossRandActivityChongzhiRankConfig * CrossRandActivityConfig::GetChongzhiRankCfg(int rank, long long total_chongzhi)
{
	if (rank <= 0 || rank > m_chongzhi_rank_max_rank || rank > CROSS_RA_CHONGZHI_RANK_MAX_RANK)
	{
		return nullptr;
	}

	CrossRandActivityChongzhiRankConfig * rank_cfg = nullptr;
	for (int tmp_rank = rank; tmp_rank <= m_chongzhi_rank_max_rank && tmp_rank <= CROSS_RA_CHONGZHI_RANK_MAX_RANK; ++ tmp_rank)
	{
		if (total_chongzhi >= m_chongzhi_rank_cfg_list[tmp_rank].need_total_chongzhi)
		{
			rank_cfg = &m_chongzhi_rank_cfg_list[tmp_rank];
			break;
		}
	}

	return rank_cfg;
}

const CrossRandActivityConsumeRankConfig * CrossRandActivityConfig::GetConsumeRankCfg(int rank, long long total_consume)
{
	if (rank <= 0 || rank > m_consume_rank_max_rank || rank > CROSS_RA_CONSUME_RANK_MAX_RANK)
	{
		return nullptr;
	}

	CrossRandActivityConsumeRankConfig * rank_cfg = nullptr;
	for (int tmp_rank = rank; tmp_rank <= m_consume_rank_max_rank && tmp_rank <= CROSS_RA_CONSUME_RANK_MAX_RANK; ++tmp_rank)
	{
		if (total_consume >= m_consume_rank_cfg_list[tmp_rank].need_total_consume)
		{
			rank_cfg = &m_consume_rank_cfg_list[tmp_rank];
			break;
		}
	}

	return rank_cfg;
}

int CrossRandActivityConfig::InitOtherCfg(PugiXmlNode  RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (nullptr == data_element)
	{
		return -10000;
	}

	if (!PugiGetSubNodeValue(data_element, "chongzhi_rank_join_reward_need", m_other_cfg.chongzhi_rank_join_reward_need) || m_other_cfg.chongzhi_rank_join_reward_need <= 0)
	{
		return -1;
	}

	if (!PugiGetSubNodeValue(data_element, "consume_rank_join_reward_need", m_other_cfg.consume_rank_join_reward_need) || m_other_cfg.consume_rank_join_reward_need <= 0)
	{
		return -2;
	}

	return 0;
}

int CrossRandActivityConfig::InitChongzhiRankCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_chongzhi_rank_max_rank = 0;

	while (!data_element.empty())
	{
		if (m_chongzhi_rank_max_rank >= CROSS_RA_CHONGZHI_RANK_MAX_RANK)
		{
			break;
		}

		int rank = 0;
		if (!PugiGetSubNodeValue(data_element, "rank", rank) || rank != m_chongzhi_rank_max_rank + 1)
		{
			return -1;
		}

		CrossRandActivityChongzhiRankConfig &cfg_item = m_chongzhi_rank_cfg_list[rank];
		cfg_item.rank = rank;
		m_chongzhi_rank_max_rank = rank;

		if (!PugiGetSubNodeValue(data_element, "need_total_chongzhi", cfg_item.need_total_chongzhi) || cfg_item.need_total_chongzhi < 0)
		{
			return -2;
		}

		{
			PugiXmlNode reward_item = data_element.child("join_reward_item");
			if (nullptr == reward_item)
			{
				return -3;
			}

			if (!cfg_item.rank_join_reward_item.ReadConfig(reward_item))
			{
				return -4;
			}
		}

		{
			PugiXmlNode reward_item = data_element.child("person_reward_item");
			if (nullptr != reward_item)
			{
				if (!cfg_item.personal_reward_item.ReadConfig(reward_item))
				{
					// 读取出错不要返回，直接置为无效数据就行
					cfg_item.personal_reward_item.item_id = 0;
					cfg_item.personal_reward_item.is_bind = true;
					cfg_item.personal_reward_item.num = 0;
				}
			}
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int CrossRandActivityConfig::InitConsumeRankCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_consume_rank_max_rank = 0;

	while (!data_element.empty())
	{
		if (m_consume_rank_max_rank >= CROSS_RA_CONSUME_RANK_MAX_RANK)
		{
			break;
		}

		int rank = 0;
		if (!PugiGetSubNodeValue(data_element, "rank", rank) || rank != m_consume_rank_max_rank + 1)
		{
			return -1;
		}

		CrossRandActivityConsumeRankConfig &cfg_item = m_consume_rank_cfg_list[rank];
		cfg_item.rank = rank;
		m_consume_rank_max_rank = rank;

		if (!PugiGetSubNodeValue(data_element, "need_total_consume", cfg_item.need_total_consume) || cfg_item.need_total_consume < 0)
		{
			return -2;
		}

		{
			PugiXmlNode reward_item = data_element.child("join_reward_item");
			if (reward_item.empty())
			{
				return -3;
			}

			if (!cfg_item.rank_join_reward_item.ReadConfig(reward_item))
			{
				return -4;
			}
		}

		{
			PugiXmlNode reward_item = data_element.child("person_reward_item");
			if (nullptr != reward_item)
			{
				if (!cfg_item.personal_reward_item.ReadConfig(reward_item))
				{
					// 读取出错不要返回，直接置为无效数据就行
					cfg_item.personal_reward_item.item_id = 0;
					cfg_item.personal_reward_item.is_bind = true;
					cfg_item.personal_reward_item.num = 0;
				}
			}
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}



