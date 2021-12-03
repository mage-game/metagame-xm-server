#include "crossgoalconfig.hpp"
#include "config/sharedconfig/sharedconfig.h"
#include "engineadapter.h"

CrossGoalConfig::CrossGoalConfig()
{
	m_cross_goal_item_count = 0;
	m_guild_goal_item_count = 0;
}

CrossGoalConfig::~CrossGoalConfig()
{

}


bool CrossGoalConfig::Init(std::string path, std::string *err)
{
	int iRet = 0;
	char err_info[1024] = {0};

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "CrossGoalConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = path + " xml root node error";
		return false;
	}

	{
		// 战场目标技能配置
		PugiXmlNode other_element = RootElement.child("other");
		if (other_element.empty())
		{
			*err = path + " xml other node ";
			return false;
		}

		iRet = this->InitOtherCfg(other_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitOtherCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		PugiXmlNode item_element = RootElement.child("cross_goal_item");
		if (item_element.empty())
		{
			*err = path + "xml not cross_goal_item node";
			return false;
		}

		iRet = this->InitGoalItemCfg(item_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitGoalItemCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		PugiXmlNode item_element = RootElement.child("guild_goal_item");
		if (item_element.empty())
		{
			*err = path + "xml not guild_goal_item node";
			return false;
		}

		iRet = this->InitGuildItemCfg(item_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitGuildItemCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	return true;
}

const CrossGoalItem * CrossGoalConfig::GetCrossGoalItemByIndex(int index) const
{
	if (index < 0 ||index >= m_cross_goal_item_count || index >= CROSS_GOAL_ITEM_CFG_MAX) return NULL;

	if (m_cross_goal_item_list[index].index == index)
	{
		return &m_cross_goal_item_list[index];
	}

	return NULL;
}

const CrossGoalItem * CrossGoalConfig::GetGuildGoalItemByIndex(int index) const
{
	if (index < 0 || index >= m_guild_goal_item_count || index >= CROSS_GOAL_ITEM_CFG_MAX) return NULL;

	if (m_guild_goal_item_list[index].index == index)
	{
		return &m_guild_goal_item_list[index];
	}

	return NULL;
}

bool CrossGoalConfig::HaveGuildGoalFinish(int cond_type, int param, int *flag)
{
	bool have_act  = false;
	for (int i = 0; i < m_guild_goal_item_count && i < CROSS_GOAL_ITEM_CFG_MAX; ++i)
	{
		if (0 != (*flag & (1 << i)))
		{
			continue;
		}
		CrossGoalItem & item = m_guild_goal_item_list[i];
		if (item.cond_type == cond_type && param >= item.cond_param[0])
		{
			*flag |= ( 1 << i);
			have_act = true;
		}
	}
	return have_act;
}

bool CrossGoalConfig::CheckGuildFinalGoal(int *flag)
{
	bool finish = true;
	int index = -1;
	for (int i = 0; i < m_guild_goal_item_count && i < CROSS_GOAL_ITEM_CFG_MAX; ++i)
	{
		CrossGoalItem & item = m_guild_goal_item_list[i];
		if (item.cond_type == CROSS_GOAL_COND_FINISH_ALL_BEFORE)
		{
			index = i;
			for (unsigned int j = 0; j < item.cond_param.size(); ++j)
			{
				if (0 == (*flag & (1 << item.cond_param[j])))
				{
					finish = false;
					break;
				}
			}
		}
	}
	if (finish)
	{
		*flag |= 1 << index;
	}

	return finish;
}


int CrossGoalConfig::InitOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1000;
	}
	if (!PugiGetSubNodeValue(data_element, "open_day_beg", m_other_cfg.open_day_beg) || m_other_cfg.open_day_beg <= 0)
	{
		return -1;
	}

	if (!PugiGetSubNodeValue(data_element, "open_day_end", m_other_cfg.open_day_end) || m_other_cfg.open_day_end <= 0)
	{
		return -2;
	}

	if (!PugiGetSubNodeValue(data_element, "guild_notify_sec", m_other_cfg.guild_notify_sec) || m_other_cfg.guild_notify_sec < 0)
	{
		return -3;
	}

	if (m_other_cfg.open_day_beg > m_other_cfg.open_day_end)
	{
		return -3;
	}

	return 0;
}
int CrossGoalConfig::InitGoalItemCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1000;
	}

	m_cross_goal_item_count = 0;

	int last_index = -1;
	while (!data_element.empty())
	{
		int index = 0;
		if (!PugiGetSubNodeValue(data_element, "index", index) || index < 0 || index >= CROSS_GOAL_ITEM_CFG_MAX)
		{
			return -1;
		}
		if (index != last_index + 1)
		{
			return -100;
		}
		last_index = index;

		CrossGoalItem & goal_item = m_cross_goal_item_list[index];
		goal_item.index = index;

		if (!PugiGetSubNodeValue(data_element, "cond_type", goal_item.cond_type) || goal_item.cond_type <= CROSS_GOAL_COND_INVALID_TYPE ||
			goal_item.cond_type >= CROSS_GOAL_COND_MAX_TYPE)
		{
			return -2;
		}

		int vec_ret = this->ReadList(data_element, "cond_param", goal_item.cond_param, ",");
		if (vec_ret < 0)
		{
			return -200 + vec_ret;
		}

		if (goal_item.cond_param.size() <= 0)
		{
			return -200;
		}

		int count = ItemConfigData::ReadConfigList(data_element, "reward_item", goal_item.goal_reward, MAX_ATTACHMENT_ITEM_NUM);
		if (count < 0)
		{
			return -300 + count;
		}

		m_cross_goal_item_count ++;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int CrossGoalConfig::InitGuildItemCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1000;
	}

	m_guild_goal_item_count = 0;

	int last_index = -1;
	while (!data_element.empty())
	{
		int index = 0;
		if (!PugiGetSubNodeValue(data_element, "index", index) || index < 0 || index >= CROSS_GOAL_ITEM_CFG_MAX)
		{
			return -1;
		}
		if (index != last_index + 1)
		{
			return -100;
		}
		last_index = index;

		CrossGoalItem & goal_item = m_guild_goal_item_list[index];
		goal_item.index = index;

		if (!PugiGetSubNodeValue(data_element, "cond_type", goal_item.cond_type) || goal_item.cond_type <= CROSS_GOAL_COND_INVALID_TYPE ||
			goal_item.cond_type >= CROSS_GOAL_COND_MAX_TYPE)
		{
			return -2;
		}

		int vec_ret = this->ReadList(data_element, "cond_param", goal_item.cond_param, ",");
		if (vec_ret < 0)
		{
			return -200 + vec_ret;
		}

		if (goal_item.cond_param.size() <= 0)
		{
			return -200;
		}

		int count = ItemConfigData::ReadConfigList(data_element, "reward_item", goal_item.goal_reward, MAX_ATTACHMENT_ITEM_NUM);
		if (count < 0)
		{
			return -300 + count;
		}

		m_guild_goal_item_count++;
		data_element = data_element.next_sibling();
	}

	return 0;
}
