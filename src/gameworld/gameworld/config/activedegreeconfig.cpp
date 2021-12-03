#include "activedegreeconfig.hpp"

#include "globalconfig/globalconfig.h"

#include "servercommon/configcommon.h"
#include "checkresourcecenter.hpp"
#include "item/itempool.h"

ActiveDegreeConfig::ActiveDegreeConfig():m_max_online_time(0)
{

}

ActiveDegreeConfig::~ActiveDegreeConfig()
{

}

bool ActiveDegreeConfig::Init(const std::string &configname, std::string *err)
{
	int iRet = 0;
	char err_info[1024] = {0};

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, configname, "ActiveDegreeConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = configname + ": xml node error in root";
		return false;
	}

	{
		PugiXmlNode root_element = RootElement.child("degree");
		if (root_element.empty())
		{
			*err = configname + ": xml node error in root->degree";
			return false;
		}

		iRet = this->InitActiveConfig(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s:InitActiveConfig fail %d\n", configname.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		PugiXmlNode root_element = RootElement.child("reward");
		if (root_element.empty())
		{
			*err = configname + ": xml node error in root->reward";
			return false;
		}

		iRet = this->InitRewardConfig(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s:InitRewardConfig fail %d\n", configname.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}


	{
		PugiXmlNode root_element = RootElement.child("ratio");
		if (root_element.empty())
		{
			*err = configname + ": xml node error in root->ratio";
			return false;
		}

		iRet = this->InitExpLevelConfig(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s:InitExpLevelConfig fail %d\n", configname.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		// 按天配置的活跃度阈值奖励
		PugiXmlNode root_element = RootElement.child("reward_on_day");
		if (root_element.empty())
		{
			*err = configname + ": xml node error in root->reward_on_day";
			return false;
		}

		iRet = this->InitRewardOnDayConfig(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s:InitRewardOnDayConfig fail %d\n", configname.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	return true;
}

int ActiveDegreeConfig::InitActiveConfig(PugiXmlNode RootElement)
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
		int type = -1;
		if (!PugiGetSubNodeValue(data_element, "type", type) || type >= ACTIVEDEGREE_TYPE_NUM)
		{
			return -1;
		}

		ActiveDegreeInfo &cfg_item = m_active_degree_list[type];
		cfg_item.type = type;

		if (!PugiGetSubNodeValue(data_element, "max_times", cfg_item.max_times) || cfg_item.max_times <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "add_degree", cfg_item.add_degree) || cfg_item.add_degree < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "add_exp", cfg_item.add_exp) || cfg_item.add_exp < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "exp_factor_type", cfg_item.exp_factor_type) || cfg_item.exp_factor_type < 0)
		{
			return -5;
		}

		if (ACTIVEDEGREE_TYPE_ONLIME_TIME == type)
		{
			m_max_online_time = cfg_item.max_times;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int ActiveDegreeConfig::InitRewardConfig(PugiXmlNode RootElement)
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
		int reward_index = 0;
		if (!PugiGetSubNodeValue(data_element, "reward_index", reward_index) || reward_index < 0)
		{
			return -1;
		}

		ActiveDegreeReward *item_cfg = &m_reward_list[reward_index];
		item_cfg->reward_index = reward_index;

		if (!PugiGetSubNodeValue(data_element, "degree_limit", item_cfg->degree_limit) || item_cfg->degree_limit <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "is_notice", item_cfg->is_notice) || item_cfg->is_notice < 0)
		{
			return -3;
		}

		PugiXmlNode Itemelement = data_element.child("item");
		if (!item_cfg->item.ReadConfig(Itemelement))
		{
			return -4;
		}

		if (reward_index >= ACTIVEDEGREE_REWARD_ITEM_MAX_NUM)
		{
			return -200;
		}

		data_element = data_element.next_sibling();
	}
	
	return 0;
}

// 初始化等级经验系数数据
int ActiveDegreeConfig::InitExpLevelConfig(PugiXmlNode RootElement)
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

	int level_index = 0;
	int last_max_level = 0;
	
	while (!data_element.empty())
	{
		if (level_index >= ACTIVEDEGREE_REWARD_LEVEL_MAX_NUM)
		{
			return -1;
		}

		ActiveDegreeExpLevelRatio *item_cfg = &m_explevel_ratio_list[level_index];

		if (!PugiGetSubNodeValue(data_element, "min_level", item_cfg->min_level) || item_cfg->min_level <= 0 || item_cfg->min_level <= last_max_level)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "max_level", item_cfg->max_level) || item_cfg->max_level <= 0)
		{
			return -3;
		}

		last_max_level = item_cfg->max_level;

		if (!PugiGetSubNodeValue(data_element, "exp_ratio", item_cfg->exp_ratio) || item_cfg->exp_ratio <= 0)
		{
			return -4;
		}

		if (item_cfg->min_level > item_cfg->max_level)
		{
			return -5;
		}

		data_element = data_element.next_sibling();
		++level_index;
	}

	return 0;
}

int ActiveDegreeConfig::InitRewardOnDayConfig(PugiXmlNode RootElement)
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
		int day_idx = 0;
		if (!PugiGetSubNodeValue(data_element, "day", day_idx) || day_idx <= 0 || day_idx > ACTIVEDEGREE_REWARD_MAX_DAY)
		{
			return -1;
		}

		int reward_index = 0;
		if (!PugiGetSubNodeValue(data_element, "reward_index", reward_index) || reward_index < 0 || reward_index >= ACTIVEDEGREE_REWARD_ITEM_MAX_NUM)
		{
			return -2;
		}

		ActiveDegreeReward *item_cfg = &m_reward_on_day_list[day_idx][reward_index];
		item_cfg->reward_index = reward_index;

		if (!PugiGetSubNodeValue(data_element, "degree_limit", item_cfg->degree_limit) || item_cfg->degree_limit <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "is_notice", item_cfg->is_notice) || item_cfg->is_notice < 0)
		{
			return -4;
		}

		PugiXmlNode item_element = data_element.child("item");
		if (!item_cfg->item.ReadConfig(item_element))
		{
			return -5;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}


const ActiveDegreeInfo *ActiveDegreeConfig::GetActiveDegreeInfo(int type)
{
	if (type < 0 || type >= ACTIVEDEGREE_TYPE_NUM || m_active_degree_list[type].type != type)
	{
		return NULL;
	}

	return &m_active_degree_list[type];
}

const ActiveDegreeReward *ActiveDegreeConfig::GetActiveDegreeReward(int index)
{
	if (index < 0 || index >= ACTIVEDEGREE_REWARD_ITEM_MAX_NUM)
	{
		return NULL;
	}

	return &m_reward_list[index];
}

// 获取等级经验系数
const ActiveDegreeExpLevelRatio *ActiveDegreeConfig::getActiveDegreeExpLevelRatio(int level)
{
	if (level < 0 || level > MAX_ROLE_LEVEL) return NULL;
	for (int index = 0; index < ACTIVEDEGREE_REWARD_LEVEL_MAX_NUM; index++)
	{
		if (level >= m_explevel_ratio_list[index].min_level && level <= m_explevel_ratio_list[index].max_level)
		{
			return &m_explevel_ratio_list[index];
		}
	}
	return NULL;
}

const ActiveDegreeReward *ActiveDegreeConfig::GetActiveDegreeRewardOnDay(int open_day, int reward_index)
{
	if (reward_index < 0 || reward_index >= ACTIVEDEGREE_REWARD_ITEM_MAX_NUM || open_day <= 0)
	{
		return nullptr;
	}

	if (open_day >= ACTIVEDEGREE_REWARD_MAX_DAY)
	{
		open_day = ACTIVEDEGREE_REWARD_MAX_DAY;
	}

	return &m_reward_on_day_list[open_day][reward_index];
}
