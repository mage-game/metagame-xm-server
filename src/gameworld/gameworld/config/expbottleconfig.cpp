#include "expbottleconfig.hpp"

#include "gamecommon.h"
#include "servercommon/servercommon.h"
#include "servercommon/configcommon.h"
#include "checkresourcecenter.hpp"
#include "obj/character/monster.h"
#include "monster/monsterpool.h"
#include "world.h"
#include "scene/scene.h"
#include "scene/map.h"
#include <string>

#include "item/itempool.h"

ExpBottleConfig::ExpBottleConfig()
{

}

ExpBottleConfig::~ExpBottleConfig()
{

}

bool ExpBottleConfig::Init(const std::string &configname, std::string *err)
{
	char errinfo[1024] = {0};
	int iRet = 0;

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, configname, "ExpBottleConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = configname + ": xml root node error.";
		return false;
	}

	{
		PugiXmlNode ll_element = RootElement.child("bottle_lv_list");
		if (ll_element.empty())
		{
			*err = configname + ": no [other].";
			return false;
		}

		iRet = this->InitLvCfg(ll_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitLvCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode ll_element = RootElement.child("bottle_item_list");
		if (ll_element.empty())
		{
			*err = configname + ": no [other].";
			return false;
		}

		iRet = this->InitItemCfg(ll_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitItemCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

const ExpBottleLvCfg * ExpBottleConfig::GetExpBottleLvCfg(int role_lv)
{
	for (int i = 0; i < EXP_BOTTLE_LV_CFG_MAX; i++)
	{
		if (role_lv >= m_bottle_lv_list[i].min_lv && role_lv <= m_bottle_lv_list[i].max_lv)
		{
			return &m_bottle_lv_list[i];
		}
	}

	return NULL;
}

const ExpBottleItemCfg * ExpBottleConfig::GetExpBottleItemCfg(int seq)
{
	for (int i = 0; i < m_bottle_item_count && i < EXP_BOTTLE_ITEM_CFG_MAX; i++)
	{
		if (seq == m_bottle_item_list[i].seq)
		{
			return &m_bottle_item_list[i];
		}
	}

	return NULL;
}

int ExpBottleConfig::InitLvCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	while (!dataElement.empty())
	{
		int lv_idx = 0;
		if (!PugiGetSubNodeValue(dataElement, "lv_idx", lv_idx) || lv_idx < 0 || lv_idx >= EXP_BOTTLE_LV_CFG_MAX)
		{
			return -16;
		}

		ExpBottleLvCfg *logic_cfg = &m_bottle_lv_list[lv_idx];
		if (0 != logic_cfg->min_lv)
		{
			return -22;
		}

		if (!PugiGetSubNodeValue(dataElement, "min_lv", logic_cfg->min_lv) || logic_cfg->min_lv <= 0)
		{
			return -171;
		}
		if (!PugiGetSubNodeValue(dataElement, "max_lv", logic_cfg->max_lv) || logic_cfg->max_lv <= 0)
		{
			return -181;
		}
		if (!PugiGetSubNodeValue(dataElement, "use_maxcount", logic_cfg->use_maxcount) || logic_cfg->use_maxcount <= 0)
		{
			return -191;
		}
		if (!PugiGetSubNodeValue(dataElement, "transform_maxcount", logic_cfg->transform_maxcount) || logic_cfg->transform_maxcount <= 0)
		{
			return -192;
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int ExpBottleConfig::InitItemCfg(PugiXmlNode RootElement)
{
	m_bottle_item_count = 0;

	PugiXmlNode dataElement = RootElement.child("data");
	while (!dataElement.empty())
	{
		if (m_bottle_item_count >= EXP_BOTTLE_ITEM_CFG_MAX)
		{
			return -888;
		}

		ExpBottleItemCfg *logic_cfg = &m_bottle_item_list[m_bottle_item_count];
		if (0 != logic_cfg->seq)
		{
			return -151;
		}

		if (!PugiGetSubNodeValue(dataElement, "seq", logic_cfg->seq) || logic_cfg->seq <= 0)
		{
			return -161;
		}
		if (!PugiGetSubNodeValue(dataElement, "need_lv", logic_cfg->need_lv) || logic_cfg->need_lv <= 0)
		{
			return -171;
		}
		if (!PugiGetSubNodeValue(dataElement, "cost_exp", logic_cfg->cost_exp) || logic_cfg->cost_exp <= 0)
		{
			return -181;
		}
		if (!PugiGetSubNodeValue(dataElement, "bottle_itemid", logic_cfg->bottle_itemid) || logic_cfg->bottle_itemid <= 0)
		{
			return -191;
		}
		if (NULL == ITEMPOOL->GetItem(logic_cfg->bottle_itemid))
		{
			return -192;
		}

		++ m_bottle_item_count;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}


