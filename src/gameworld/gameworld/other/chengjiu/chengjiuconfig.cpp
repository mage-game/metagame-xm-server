#include "item/itempool.h"
#include "servercommon/configcommon.h"
#include "checkresourcecenter.hpp"
#include "obj/character/attribute.hpp"
#include "servercommon/servercommon.h"
#include "chengjiuconfig.hpp"
#include "gamedef.h"
#include "skill/skillpool.hpp"
#include "servercommon/chengjiudef.hpp"

ChengJiuConfig::ChengJiuConfig() : m_reward_max_num(0), m_max_fuwen_level(0)
{

}

ChengJiuConfig::~ChengJiuConfig()
{

}

bool ChengJiuConfig::Init(const std::string &configname, std::string *err)
{
	char errinfo[1024] = {0};
	int iRet = 0;

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, configname, "ChengJiuConfig", *err);
	
	PugiXmlNode RootElement = document.document_element();

	if (RootElement.empty())
	{
		*err = configname + ": xml root node error.";
		return false;
	}

	{
		PugiXmlNode root_element = RootElement.child("title");
		if (root_element.empty())
		{
			*err = configname + ": no [title].";
			return false;
		}

		iRet = this->InitTitleConfig(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitTitleConfig failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode root_element = RootElement.child("reward");
		if (root_element.empty())
		{
			*err = configname + ": no [reward].";
			return false;
		}

		iRet = this->InitRewardConfig(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitRewardConfig failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

// 	{
// 		PugiXmlNode root_element = RootElement.child("fuwen");
// 		if (root_element.empty())
// 		{
// 			*err = configname + ": no [fuwen].";
// 			return false;
// 		}
// 
// 		iRet = this->InitFuwenConfig(root_element);
// 		if (iRet)
// 		{
// 			sprintf(errinfo,"%s: InitFuwenConfig failed %d", configname.c_str(), iRet);
// 			*err = errinfo;
// 			return false;
// 		}
// 	}

// 	{
// 		PugiXmlNode root_element = RootElement.child("fuwen_gold");
// 		if (root_element.empty())
// 		{
// 			*err = configname + ": no [fuwen_gold].";
// 			return false;
// 		}
// 
// 		iRet = this->InitFuwenGoldConfig(root_element);
// 		if (iRet)
// 		{
// 			sprintf(errinfo,"%s: InitFuwenGoldConfig failed %d", configname.c_str(), iRet);
// 			*err = errinfo;
// 			return false;
// 		}
// 	}

	return true;
}

int ChengJiuConfig::InitTitleConfig(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	short last_level = 0;

	while(!dataElement.empty())
	{
		short level = 0;
		if (!PugiGetSubNodeValue(dataElement, "level", level) || level - last_level != 1)
		{
			return -1;
		}

		TitleLevelCfg &title_cfg = m_title_map[level];
		title_cfg.level = level;
		last_level = level;

		if (!PugiGetSubNodeValue(dataElement, "chengjiu", title_cfg.chengjiu) || title_cfg.chengjiu <= 0)
		{
			return -2;
		}
		
		if (!PugiGetSubNodeValue(dataElement, "gongji", title_cfg.gongji) || title_cfg.gongji < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "fangyu", title_cfg.fangyu) || title_cfg.fangyu < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "maxhp", title_cfg.maxhp) || title_cfg.maxhp < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "shanbi", title_cfg.shanbi) || title_cfg.shanbi < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "is_broadcast", title_cfg.is_broadcast) || title_cfg.is_broadcast < 0)
		{
			return -7;
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int ChengJiuConfig::InitRewardConfig(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	m_reward_max_num = 0;

	while(!dataElement.empty())
	{
		short reward_id = 0;
		if (!PugiGetSubNodeValue(dataElement, "id", reward_id) || reward_id <= 0)
		{
			return -1;
		}

		if (reward_id >= CHENGJIU_REWARD_MAX_NUM)
		{
			return -100;
		}
		
		ChengJiuRewardCfg &reward_cfg = m_reward_map[reward_id];
		reward_cfg.reward_id = reward_id;

		if (!PugiGetSubNodeValue(dataElement, "condition", reward_cfg.condition) || reward_cfg.condition <= CJ_CONDITION_INVALID || reward_cfg.condition >= CJ_CONDITION_MAX)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "param1", reward_cfg.param1) || reward_cfg.param1 < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "chengjiu", reward_cfg.chengjiu) || reward_cfg.chengjiu <= 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "bind_gold", reward_cfg.bind_gold) || reward_cfg.bind_gold <= 0)
		{
			return -5;
		}
		
		++ m_reward_max_num;
		if (m_reward_max_num >= CHENGJIU_REWARD_MAX_NUM)
		{
			return -200;
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int ChengJiuConfig::InitFuwenConfig(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	short last_level = 0;
	short last_chengjiu = 0;

	while(!dataElement.empty())
	{
		short level = 0;
		if (!PugiGetSubNodeValue(dataElement, "level", level) || level <= 0 || level - last_level != 1)
		{
			return -1;
		}

		FuwenLevelCfg &fuwen_level_cfg = m_fuwen_map[level];
		fuwen_level_cfg.level = level;
		last_level = level;
		m_max_fuwen_level = level;
		
		if (!PugiGetSubNodeValue(dataElement, "title_limit", fuwen_level_cfg.title_limit) || fuwen_level_cfg.title_limit <= 0 || NULL == this->GetTitleLevelCfg(fuwen_level_cfg.title_limit))
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "once_cost_chengjiu", fuwen_level_cfg.once_cost_chengjiu) || fuwen_level_cfg.once_cost_chengjiu <= 0 || fuwen_level_cfg.once_cost_chengjiu < last_chengjiu)
		{
			return -3;
		}
		last_chengjiu = fuwen_level_cfg.once_cost_chengjiu;

		if (!PugiGetSubNodeValue(dataElement, "max_times", fuwen_level_cfg.max_times) || fuwen_level_cfg.max_times <= 0)
		{
			return -4;
		}


		if (!PugiGetSubNodeValue(dataElement, "once_add_gongji", fuwen_level_cfg.once_add_gongji) || fuwen_level_cfg.once_add_gongji < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "once_add_fangyu", fuwen_level_cfg.once_add_fangyu) || fuwen_level_cfg.once_add_fangyu < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "once_add_maxhp", fuwen_level_cfg.once_add_maxhp) || fuwen_level_cfg.once_add_maxhp < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElement, "once_add_shanbi", fuwen_level_cfg.once_add_shanbi) || fuwen_level_cfg.once_add_shanbi < 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(dataElement, "add_baoji_on_act", fuwen_level_cfg.add_baoji_on_act) || fuwen_level_cfg.add_baoji_on_act < 0)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(dataElement, "add_kangbao_on_act", fuwen_level_cfg.add_kangbao_on_act) || fuwen_level_cfg.add_kangbao_on_act < 0)
		{
			return -10;
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int ChengJiuConfig::InitFuwenGoldConfig(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	int last_max_times = 0;
	int last_gold = 0;

	while(!dataElement.empty())
	{
		int times_min = 0;
		int times_max = 0;

		if (!PugiGetSubNodeValue(dataElement, "times_min", times_min) ||  (last_max_times != 0 && times_min - last_max_times != 1))
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "times_max", times_max) ||  (times_max < times_min))
		{
			return -1;
		}
		last_max_times = times_max;

		FuwenGoldConfig &gold_cfg = m_fuwen_gold_map[times_min];
		gold_cfg.times_min = times_min;
		gold_cfg.times_max = times_max;

		if (!PugiGetSubNodeValue(dataElement, "gold", gold_cfg.cost_gold) || gold_cfg.cost_gold < last_gold)
		{
			return -1;
		}

		last_gold = gold_cfg.cost_gold;


		dataElement = dataElement.next_sibling();
	}
	return 0;
}

const TitleLevelCfg *ChengJiuConfig::GetTitleLevelCfg(short level)
{
	std::map<short, TitleLevelCfg>::const_iterator iter = m_title_map.find(level);
	if (m_title_map.end() == iter)
	{
		return NULL;
	}

	return &iter->second;
}

const ChengJiuRewardCfg *ChengJiuConfig::GetRewardCfg(short reward_id)
{
	std::map<short, ChengJiuRewardCfg>::const_iterator iter = m_reward_map.find(reward_id);
	if (m_reward_map.end() == iter)
	{
		return NULL;
	}

	return &iter->second;
}

const FuwenLevelCfg *ChengJiuConfig::GetFuwenLevelCfg(short level)
{
	std::map<short, FuwenLevelCfg>::const_iterator iter = m_fuwen_map.find(level);
	if (m_fuwen_map.end() == iter)
	{
		return NULL;
	}

	return &iter->second;
}

void ChengJiuConfig::GetFuwenUpLevelCostGold(int times, int *cost_gold)
{
	if (times < 0 || NULL == cost_gold)
	{
		return;
	}

	std::map<int, FuwenGoldConfig>::const_iterator iter;
	for (iter = m_fuwen_gold_map.begin(); iter != m_fuwen_gold_map.end(); ++ iter)
	{
		if (times >= iter->second.times_min && times <= iter->second.times_max)
		{
			*cost_gold = iter->second.cost_gold;
			break;
		}
	}
}
