#include "rolebigsmallgoalconfig.hpp"

RoleBigSmallGoalConfig::RoleBigSmallGoalConfig()
{
	
}

RoleBigSmallGoalConfig::~RoleBigSmallGoalConfig()
{

}

bool RoleBigSmallGoalConfig::Init(const std::string &path, std::string *err)
{
	int iRet = 0;
	char errinfo[1024] = {0};

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "RoleBigSmallGoalConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = path + "xml root node error.";
		return false;
	}

	{
		PugiXmlNode root_element = RootElement.child("item_cfg");
		if (root_element.empty())
		{
			*err = path + "xml no item_cfg node.";
			return false;
		}

		iRet = this->InitGoalItemConfig(root_element);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s : InitGoalItemConfig fail %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		
		PugiXmlNode root_element = RootElement.child("attr_cfg");
		if (root_element.empty())
		{
			*err = path + "xml no attr_cfg node.";
			return false;
		}

		iRet = this->InitGoalAttrConfig(root_element);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s : InitGoalAttrConfig fail %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

int RoleBigSmallGoalConfig::InitGoalItemConfig(PugiXmlNode RootElement)
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

	while (!data_element.empty())
	{
		RoleBSGoalItemConfig item_cfg;
		if (!PugiGetSubNodeValue(data_element, "reward_type", item_cfg.reward_type) || item_cfg.reward_type < 0 || item_cfg.reward_type >= ROLE_BIG_SMALL_GOAL_REWARD_TYPE_MAX)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "system_type", item_cfg.system_type) || item_cfg.system_type < ROLE_BIG_SMALL_GOAL_SYSTEM_RUNE || item_cfg.system_type >= ROLE_BIG_SMALL_GOAL_SYSTEM_MAX)
		{
			return -2;
		}

		int free_time_h = 0;
		if (!PugiGetSubNodeValue(data_element, "free_time_since_open", free_time_h) || free_time_h <= 0)
		{
			return -3;
		}

		item_cfg.free_time_since_open_s = free_time_h * SECOND_PER_HOUR;

		if (!PugiGetSubNodeValue(data_element, "condition_type", item_cfg.condition_type) || item_cfg.condition_type <= ROLE_BIG_SMALL_GOAL_CONDITION_TYPE_INVALID || item_cfg.condition_type >= ROLE_BIG_SMALL_GOAL_CONDITION_TYPE_MAX)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "param_1", item_cfg.param_1) || item_cfg.param_1 < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "param_2", item_cfg.param_2) || item_cfg.param_2 < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "cost", item_cfg.cost_gold) || item_cfg.cost_gold <= 0)
		{
			return -7;
		}

		{
			PugiXmlNode reward_item_list = data_element.child("reward_item_list");
			if (reward_item_list.empty())
			{
				return -8;
			}

			PugiXmlNode reward_item = reward_item_list.child("reward_item");
			if (reward_item.empty())
			{
				return -9;
			}

			while (!reward_item.empty())
			{
				if (item_cfg.reward_count >= MAX_ATTACHMENT_ITEM_NUM)
				{
					return -10;
				}


				if (!item_cfg.reward_item_list[item_cfg.reward_count].ReadConfig(reward_item))
				{
					return -11;
				}

				++item_cfg.reward_count;
				reward_item = reward_item.next_sibling();
			}
		}

		if (!PugiGetSubNodeValue(data_element, "is_broadcast", item_cfg.is_broadcast))
		{
			return -12;
		}

		if (!PugiGetSubNodeValue(data_element, "openserver_day", item_cfg.openserver_day) || item_cfg.openserver_day < 0)
		{
			return -13;
		}

		if (!PugiGetSubNodeValue(data_element, "open_level", item_cfg.open_level) || item_cfg.open_level < 0)
		{
			return -14;
		}


		m_role_bs_goal_reward_vec.push_back(item_cfg);
		data_element = data_element.next_sibling();
	}

	return 0;
}

RoleBSGoalItemConfig *RoleBigSmallGoalConfig::GetBSGoalItemConfig(int system_type, int goal_type)
{
	if (system_type < ROLE_BIG_SMALL_GOAL_SYSTEM_RUNE || system_type >= ROLE_BIG_SMALL_GOAL_SYSTEM_MAX ||
		goal_type < 0 || goal_type >= ROLE_BIG_SMALL_GOAL_REWARD_TYPE_MAX)
	{
		return nullptr;
	}

	RoleBSGoalItemVecIt it = m_role_bs_goal_reward_vec.begin();
	for (; it != m_role_bs_goal_reward_vec.end(); ++it)
	{
		if (it->system_type == system_type && it->reward_type == goal_type)
		{
			return &(*it);
		}
	}

	return nullptr;
}

int RoleBigSmallGoalConfig::InitGoalAttrConfig(PugiXmlNode RootElement)
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

	int system_type = 0;
	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "system_type", system_type) || system_type < ROLE_BIG_SMALL_GOAL_SYSTEM_RUNE || system_type >= ROLE_BIG_SMALL_GOAL_SYSTEM_MAX)
		{
			return -1;
		}

		RoleBSGoalAttrConfig *cfg = &m_role_bs_goal_attr_list[system_type];

		cfg->system_type = system_type;

		if (!PugiGetSubNodeValue(data_element, "active_attr_param", cfg->active_attr_param) || cfg->active_attr_param < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "add_per", cfg->add_attr_value) || cfg->add_attr_value < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "max_hp", cfg->max_hp) || cfg->max_hp < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "gongji", cfg->gongji) || cfg->gongji < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "fangyu", cfg->fangyu) || cfg->fangyu < 0)
		{
			return -6;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int RoleBigSmallGoalConfig::GetBSGoalAttrAddValue(int system_type)
{
	if (system_type < ROLE_BIG_SMALL_GOAL_SYSTEM_RUNE || system_type >= ROLE_BIG_SMALL_GOAL_SYSTEM_MAX)
	{
		return 0;
	}

	return m_role_bs_goal_attr_list[system_type].add_attr_value;
}

const RoleBSGoalAttrConfig *RoleBigSmallGoalConfig::GetBSGoalAttrCfg(int system_type)
{
	if (system_type < ROLE_BIG_SMALL_GOAL_SYSTEM_RUNE || system_type >= ROLE_BIG_SMALL_GOAL_SYSTEM_MAX)
	{
		return nullptr;
	}

	return &m_role_bs_goal_attr_list[system_type];
}