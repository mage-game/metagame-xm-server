#include "jilianconfig.hpp"
#include "servercommon/configcommon.h"
#include "checkresourcecenter.hpp"
#include "jilian.hpp"

JilianConfig::JilianConfig() : m_reward_cfg_count(0)
{

}

JilianConfig::~JilianConfig()
{

}

bool JilianConfig::Init(const std::string &path, std::string *err)
{
	int iRet = 0;
	char err_info[1024] = {0};
 
	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "JilianConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = path + " xml root node error";
		return false;
	}

	{
		// ÔÓÏîÅäÖÃ
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

int JilianConfig::GetRewardExp(int role_level, int jilian_type)
{
	if (jilian_type <= JILIAN_TYPE_INVALID || jilian_type >= JILIAN_TYPE_MAX)
	{
		return 0;
	}

	for (int i = m_reward_cfg_count - 1; i >= 0; -- i)
	{
		if (role_level >= m_reward_cfg_list[i].need_level)
		{
			if (JILIAN_TYPE_SUPER == jilian_type)
			{
				return m_reward_cfg_list[i].super_exp;
			}
			else
			{
				return m_reward_cfg_list[i].common_exp;
			}
		}
	}

	return 0;
}

int JilianConfig::InitOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (!dataElement.empty())
	{
		if (!PugiGetSubNodeValue(dataElement, "can_join_times", m_other_cfg.can_join_times) || m_other_cfg.can_join_times <= 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "lianlu_scene_id", m_other_cfg.lianlu_scene_id) || m_other_cfg.lianlu_scene_id <= 0)
		{
			return -2;
		}
		CheckResourceCenter::Instance().GetSceneCheck()->Add(m_other_cfg.lianlu_scene_id);

		if (!PugiGetSubNodeValue(dataElement, "lianlu_pos_x", m_other_cfg.lianlu_pos.x) || m_other_cfg.lianlu_pos.x <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "lianlu_pos_y", m_other_cfg.lianlu_pos.y) || m_other_cfg.lianlu_pos.y <= 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "lianlu_range", m_other_cfg.lianlu_range) || m_other_cfg.lianlu_range <= 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "jilian_times", m_other_cfg.jilian_times) || m_other_cfg.jilian_times <= 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "common_jilian_cost", m_other_cfg.common_jilian_cost) || m_other_cfg.common_jilian_cost <= 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElement, "super_jilian_cost", m_other_cfg.super_jilian_cost) || m_other_cfg.super_jilian_cost <= 0)
		{
			return -8;
		}

		PugiXmlNode reward_item_element = dataElement.child("reward_item");
		if (reward_item_element.empty())
		{
			return -9;
		}

		if (!m_other_cfg.reward_item.ReadConfig(reward_item_element))
		{
			return -10;
		}

		if (!PugiGetSubNodeValue(dataElement, "qte_rate", m_other_cfg.qte_rate) || m_other_cfg.qte_rate <= 0)
		{
			return -11;
		}

		if (!PugiGetSubNodeValue(dataElement, "qte_duration_s", m_other_cfg.qte_duration_s) || m_other_cfg.qte_duration_s <= 0)
		{
			return -12;
		}

		if (!PugiGetSubNodeValue(dataElement, "cost_time_s", m_other_cfg.cost_time_s) || m_other_cfg.cost_time_s <= 0)
		{
			return -13;
		}

		if (m_other_cfg.qte_duration_s >= m_other_cfg.cost_time_s)
		{
			return -14;
		}

		if (!PugiGetSubNodeValue(dataElement, "qte_succ_max_times", m_other_cfg.qte_succ_max_times) || m_other_cfg.qte_succ_max_times <= 0)
		{
			return -15;
		}
	}

	return 0;
}

int JilianConfig::InitRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -100000;
	}

	int last_level = 0;

	while (!dataElement.empty())
	{
		if (m_reward_cfg_count >= MAX_ROLE_LEVEL)
		{
			return -1;
		}

		RewardConfig &reward_cfg = m_reward_cfg_list[m_reward_cfg_count];

		if (!PugiGetSubNodeValue(dataElement, "level", reward_cfg.need_level) || reward_cfg.need_level <= last_level)
		{
			return -2;
		}
		last_level = reward_cfg.need_level;

		if (!PugiGetSubNodeValue(dataElement, "common_exp", reward_cfg.common_exp) ||reward_cfg.common_exp <= 0) 
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "super_exp", reward_cfg.super_exp) ||reward_cfg.super_exp <= 0) 
		{
			return -4;
		}

		++ m_reward_cfg_count;
		dataElement = dataElement.next_sibling();
	}

	if (1 != m_reward_cfg_list[0].need_level)
	{
		return -1000;
	}

	return 0;
}

