#include <memory.h>
#include <stdlib.h>

#include "reallyequipconfig.hpp"
#include "servercommon/servercommon.h"
#include "servercommon/configcommon.h"
#include "item/itempool.h"
#include "obj/character/attribute.hpp"
#include "gameworld/checkresourcecenter.hpp"
#include "skill/skillpool.hpp"

#include <string>

ReallyequipComposeConfig::ReallyequipComposeConfig()
{

}

ReallyequipComposeConfig::~ReallyequipComposeConfig()
{

}

bool ReallyequipComposeConfig::Init(const std::string &configname, std::string *err)
{
// 	char errinfo[1024] = { 0 };
// 	int iRet = 0;
// 
// 	PugiXmlDocument document;
// 	PUGI_XML_LOAD_AND_CHECK(document, configname, "ReallyequipComposeConfig", *err);
// 
// 	PugiXmlNode RootElement = document.document_element();
// 
// 	if (RootElement.empty())
// 	{
// 		*err = configname + ": xml root node error.";
// 		return false;
// 	}
// 
// 	{
// 		PugiXmlNode root_element = RootElement.child("composeList");
// 		if (root_element.empty())
// 		{
// 			*err = configname + ": no [title].";
// 			return false;
// 		}
// 
// 		iRet = this->InitComposeListConfig(root_element);
// 		if (iRet)
// 		{
// 			sprintf(errinfo, "%s: InitComposeListConfig failed %d", configname.c_str(), iRet);
// 			*err = errinfo;
// 			return false;
// 		}
// 	}
// 
// 	{
// 		PugiXmlNode root_element = RootElement.child("levelupList");
// 		if (root_element.empty())
// 		{
// 			*err = configname + ": no [title].";
// 			return false;
// 		}
// 
// 		iRet = this->InitLevelUpConfig(root_element);
// 		if (iRet)
// 		{
// 			sprintf(errinfo, "%s: InitLevelUpConfig failed %d", configname.c_str(), iRet);
// 			*err = errinfo;
// 			return false;
// 		}
// 	}
// 
// 	{
// 		// 其它配置
// 		PugiXmlNode root_element = RootElement.child("Star_score");
// 		if (root_element.empty())
// 		{
// 			*err = configname + ": no [title].";
// 			return false;
// 		}
// 
// 		iRet = this->InitOtherConfig(root_element);
// 		if (iRet)
// 		{
// 			sprintf(errinfo, "%s: InitOtherConfig failed %d", configname.c_str(), iRet);
// 			*err = errinfo;
// 			return false;
// 		}
// 	}

	return true;
}

int ReallyequipComposeConfig::InitComposeListConfig(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		unsigned int itemid = 0;
		if (!PugiGetSubNodeValue(dataElement, "itemid", itemid) || NULL == ITEMPOOL->GetItem(itemid))
		{
			return -1;
		}

		ComposeListConfig &composeListConfig = m_comlist_map[itemid];
		composeListConfig.itemid = itemid;

		if (!PugiGetSubNodeValue(dataElement, "coin", composeListConfig.coin) || composeListConfig.coin == 0)
		{
			return -2;
		}
		if (!PugiGetSubNodeValue(dataElement, "bind", composeListConfig.bind))
		{
			return -3;
		}
		if (!PugiGetSubNodeValue(dataElement, "shuxing", composeListConfig.shuxing))
		{
			return -3;
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

const ComposeListConfig * ReallyequipComposeConfig::GetCompostListCfg(unsigned int itemid)
{
	std::map<int, ComposeListConfig>::const_iterator iter = m_comlist_map.find(itemid);
	if (m_comlist_map.end() != iter)
	{
		const ComposeListConfig &cfg_list = iter->second;

		return &cfg_list;
	}

	return NULL;
}

int ReallyequipComposeConfig::InitLevelUpConfig(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		unsigned int itemid = 0;
		if (!PugiGetSubNodeValue(dataElement, "itemid", itemid) || NULL == ITEMPOOL->GetItem(itemid))
		{
			return -1;
		}

		LevelupListConfig &levelupListConfig = m_levelup_map[itemid];
		levelupListConfig.itemid = itemid;

		if (!PugiGetSubNodeValue(dataElement, "dirid", levelupListConfig.dirid) || NULL == ITEMPOOL->GetItem(levelupListConfig.dirid))
		{
			return -2;
		}
		if (!PugiGetSubNodeValue(dataElement, "stuff_id_1", levelupListConfig.stuff_id_1))
		{
			return -3;
		}
		if (!PugiGetSubNodeValue(dataElement, "stuff_count_1", levelupListConfig.stuff_count_1))
		{
			return -4;
		}
		if (!PugiGetSubNodeValue(dataElement, "stuff_id_2", levelupListConfig.stuff_id_2))
		{
			return -5;
		}
		if (!PugiGetSubNodeValue(dataElement, "stuff_count_2", levelupListConfig.stuff_count_2))
		{
			return -6;
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

const LevelupListConfig * ReallyequipComposeConfig::GetLevelupListCfg(unsigned int itemid)
{
	std::map<int, LevelupListConfig>::const_iterator iter = m_levelup_map.find(itemid);
	if (m_levelup_map.end() != iter)
	{
		const LevelupListConfig &cfg_list = iter->second;

		return &cfg_list;
	}

	return NULL;
}

// 其它配置
int ReallyequipComposeConfig::InitOtherConfig(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		FeiXianOtherConfig tmp_cfg;

		if (!PugiGetSubNodeValue(dataElement,"Totle_min",tmp_cfg.total_score_min) || tmp_cfg.total_score_min < 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "Totle_max", tmp_cfg.total_score_max) || tmp_cfg.total_score_max < 0 || tmp_cfg.total_score_max <= tmp_cfg.total_score_min)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "attr_Star", tmp_cfg.star_num) || tmp_cfg.star_num < 0)
		{
			return -3;
		}

		m_other_vec.push_back(tmp_cfg);

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

const FeiXianOtherConfig* ReallyequipComposeConfig::GetOtherCfg(unsigned short score)
{
	std::vector<FeiXianOtherConfig>::iterator iter = m_other_vec.begin();

	for (;iter != m_other_vec.end();iter++)
	{
		if (score >= iter->total_score_min && score <= iter->total_score_max)
		{
			break;
		}
	}

	if (iter != m_other_vec.end())
	{
		const FeiXianOtherConfig& other_cfg = *iter;
		return &other_cfg;
	}

	return NULL;
}