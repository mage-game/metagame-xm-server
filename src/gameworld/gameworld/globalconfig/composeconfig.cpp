#include <memory.h>
#include <stdlib.h>

#include "composeconfig.h"
#include "servercommon/servercommon.h"
#include "globalconfig/equipconfig.h"
#include "servercommon/configcommon.h"
#include "item/itempool.h"
#include "obj/character/attribute.hpp"
#include "gameworld/checkresourcecenter.hpp"

#include <string>

ComposeConfig::ComposeConfig()
{
}

ComposeConfig::~ComposeConfig()
{
}

bool ComposeConfig::Init(const std::string &configname, std::string *err)
{
	char errinfo[1024] = {0};
	int iRet = 0;

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, configname, "ComposeConfig", *err);

	PugiXmlNode RootElement = document.document_element();

	if (RootElement.empty())
	{
		*err = configname + ": xml root node error.";
		return false;
	}

	PugiXmlNode compose_element = RootElement.child("compose_list");
	if (compose_element.empty())
	{
		*err = configname + ": no [compose_list].";
		return false;
	}

	iRet = InitProductCfg(compose_element);
	if (iRet)
	{
		sprintf(errinfo,"%s: InitProductCfg failed %d", configname.c_str(), iRet);
		*err = errinfo;
		return false;
	}

	return true;
}

const ProductConfig * ComposeConfig::GetProductCfg(int produect_seq) const
{
	std::map<int, ProductConfig>::const_iterator iter = m_product_map.find(produect_seq);

	if (m_product_map.end() == iter) return NULL;

	return &iter->second;
}

int ComposeConfig::InitProductCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	while (!dataElement.empty())
	{
		int producd_seq = 0;
		if (!PugiGetSubNodeValue(dataElement, "producd_seq", producd_seq))
		{
			return -1;
		}

		ItemID product_id = 0;
		
		if (!PugiGetSubNodeValue(dataElement, "product_id", product_id) || (NULL == ITEMPOOL->GetItem(product_id)))
		{
			return -101;
		}

		ProductConfig cfg;
		cfg.m_product_seq = producd_seq;
		cfg.m_product_id = product_id;

		if (!PugiGetSubNodeValue(dataElement, "coin", cfg.m_coin) || cfg.m_coin < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "nv_wa_shi", cfg.m_nv_wa_shi) || cfg.m_nv_wa_shi < 0)
		{
			return -40;
		}

		if (!PugiGetSubNodeValue(dataElement, "stuff_id_1", cfg.m_stuff_id_list[0]) || (NULL == ITEMPOOL->GetItem(cfg.m_stuff_id_list[0])))
		{
			return -102;
		}

		if (!PugiGetSubNodeValue(dataElement, "stuff_count_1", cfg.m_stuff_count_list[0]) || 
			(ItemBase::INVALID_ITEM_ID != cfg.m_stuff_id_list[0] && cfg.m_stuff_count_list[0] <= 0))
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "stuff_id_2", cfg.m_stuff_id_list[1]) || 
			(ItemBase::INVALID_ITEM_ID != cfg.m_stuff_id_list[1] && NULL == ITEMPOOL->GetItem(cfg.m_stuff_id_list[1])))
		{
			return -103;
		}

		if (!PugiGetSubNodeValue(dataElement, "stuff_count_2", cfg.m_stuff_count_list[1]) || 
			(ItemBase::INVALID_ITEM_ID != cfg.m_stuff_id_list[1] && cfg.m_stuff_count_list[1] <= 0))
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(dataElement, "stuff_id_3", cfg.m_stuff_id_list[2]) || 
			(ItemBase::INVALID_ITEM_ID != cfg.m_stuff_id_list[2] && NULL == ITEMPOOL->GetItem(cfg.m_stuff_id_list[2])))
		{
			return -104;
		}

		if (!PugiGetSubNodeValue(dataElement, "stuff_count_3", cfg.m_stuff_count_list[2]) || 
			(ItemBase::INVALID_ITEM_ID != cfg.m_stuff_id_list[2] && cfg.m_stuff_count_list[2] <= 0))
		{
			return -10;
		}

		if (!PugiGetSubNodeValue(dataElement, "stuff_id_4", cfg.m_stuff_id_list[3]) || 
			(ItemBase::INVALID_ITEM_ID != cfg.m_stuff_id_list[3] && NULL == ITEMPOOL->GetItem(cfg.m_stuff_id_list[3])))
		{
			return -105;
		}

		if (!PugiGetSubNodeValue(dataElement, "stuff_count_4", cfg.m_stuff_count_list[3]) || 
			(ItemBase::INVALID_ITEM_ID != cfg.m_stuff_id_list[3] && cfg.m_stuff_count_list[3] <= 0))
		{
			return -12;
		}

		if (!PugiGetSubNodeValue(dataElement, "is_broadcast", cfg.is_broadcast) || cfg.is_broadcast < 0)
		{
			return -13;
		}

		std::map<int, ProductConfig>::const_iterator iter = m_product_map.find(cfg.m_product_seq);

		if (m_product_map.end() != iter) 
		{
			return -100;
		}

		m_product_map[cfg.m_product_seq] = cfg;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}
