#include "lovetreeconfig.hpp"
#include "item/itempool.h"
#include <servercommon/configcommon.h>

LoveTreeCfg::LoveTreeCfg()
{
}

LoveTreeCfg::~LoveTreeCfg()
{
}

bool LoveTreeCfg::Init(std::string path, std::string *err)
{
	int iRet = 0;
	char err_info[1024] = { 0 };

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "LoveTreeCfg", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = path + "xml root node error";
		return false;
	}

	{
		// ÆäËûÅäÖÃ
		PugiXmlNode root_element = RootElement.child("other_cfg");
		if (root_element.empty())
		{
			*err = path + ":no [other]";
			return false;
		}

		iRet = this->InitOtherCfg(root_element);
		if (iRet)
		{
			sprintf(err_info, "%s: LoveTreeConfig:InitOtherCfg failed %d", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		// ÐÇ¼¶ÅäÖÃ
		PugiXmlNode root_element = RootElement.child("love_tree_level_cfg");
		if (root_element.empty())
		{
			*err = path + ":no [love_tree_level_cfg]";
			return false;
		}

		iRet = this->InitUpStarLevelCfg(root_element);
		if (iRet)
		{
			sprintf(err_info, "%s: LoveTreeConfig:InitUpStarLevelCfg failed %d", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	return true;
}

int LoveTreeCfg::InitOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	if (!PugiGetSubNodeValue(dataElement, "self_free_water_time", m_other_cfg.self_free_water_time) || m_other_cfg.self_free_water_time < 0)
	{
		return -1;
	}

	if (!PugiGetSubNodeValue(dataElement, "assist_free_water_time", m_other_cfg.assist_free_water_time) || m_other_cfg.assist_free_water_time < 0)
	{
		return -2;
	}

	if (!PugiGetSubNodeValue(dataElement, "rename_cost", m_other_cfg.rename_cost) || m_other_cfg.rename_cost < 0)
	{
		return -3;
	}

	if (!PugiGetSubNodeValue(dataElement, "marry_add_per", m_other_cfg.marry_add_per) || m_other_cfg.marry_add_per < 0)
	{
		return -4;
	}

	return 0;
}

int LoveTreeCfg::InitUpStarLevelCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	int last_tree_star = -1;
	while(!dataElement.empty())
	{
		short tree_star = 0;

		if (!PugiGetSubNodeValue(dataElement, "tree_star", tree_star) || tree_star < 0 || last_tree_star + 1 != tree_star || m_love_tree_star_level_map.find(tree_star) != m_love_tree_star_level_map.end())
		{
			return -1;
		}

		LoveTreeLevelConfig love_tree_cfg;
		love_tree_cfg.tree_star = tree_star;
		last_tree_star = tree_star;

		if (!PugiGetSubNodeValue(dataElement, "need_exp", love_tree_cfg.need_exp) || love_tree_cfg.need_exp <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "baoji_add_exp_rate", love_tree_cfg.baoji_add_exp_rate) || love_tree_cfg.baoji_add_exp_rate < 0)
		{
			return -3;
		} 

		if (!PugiGetSubNodeValue(dataElement, "extra_add_exp_per", love_tree_cfg.extra_add_exp_per) || love_tree_cfg.extra_add_exp_per < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "maxhp", love_tree_cfg.maxhp) || love_tree_cfg.maxhp < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "gongji", love_tree_cfg.gongji) || love_tree_cfg.gongji < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "fangyu", love_tree_cfg.fangyu) || love_tree_cfg.fangyu < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElement, "shanbi", love_tree_cfg.shanbi) || love_tree_cfg.shanbi < 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(dataElement, "mingzhong", love_tree_cfg.mingzhong) || love_tree_cfg.mingzhong < 0)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(dataElement, "baoji", love_tree_cfg.baoji) || love_tree_cfg.baoji < 0)
		{
			return -10;
		}

		if (!PugiGetSubNodeValue(dataElement, "jianren", love_tree_cfg.jianren) || love_tree_cfg.jianren < 0)
		{
			return -11;
		}

		if (!PugiGetSubNodeValue(dataElement, "add_exp", love_tree_cfg.add_exp) || love_tree_cfg.add_exp < 0)
		{
			return -12;
		}

		PugiXmlNode Itemelement = dataElement.child("male_up_star_item");
		ItemID itemid = 0;
		if (NULL != Itemelement && PugiGetSubNodeValue(Itemelement, "item_id", itemid) && NULL != ITEMPOOL->GetItem(itemid))
		{
			if (!love_tree_cfg.male_up_star_item.ReadConfig(Itemelement))
			{
				return -13;
			}
		}

		Itemelement = dataElement.child("female_up_star_item");
		itemid = 0;
		if (NULL != Itemelement && PugiGetSubNodeValue(Itemelement, "item_id", itemid) && NULL != ITEMPOOL->GetItem(itemid))
		{
			if (!love_tree_cfg.female_up_star_item.ReadConfig(Itemelement))
			{
				return -14;
			}
		}

		m_love_tree_star_level_map[tree_star] = love_tree_cfg;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

const LoveTreeLevelConfig * LoveTreeCfg::GetLoveTreeLevelCfg(int star_level)
{
	if (star_level < 0)
	{
		return NULL;
	}

	std::map<int, LoveTreeLevelConfig>::iterator iter = m_love_tree_star_level_map.find(star_level);
	if (m_love_tree_star_level_map.end() != iter)
	{
		return &iter->second;
	}

	return NULL;
}

