#include "suitbase.hpp"

#include <memory.h>
#include <stdlib.h>

#include "servercommon/servercommon.h"
#include "servercommon/configcommon.h"
#include "obj/character/attribute.hpp"

#include "item/itembase.h"
#include "item/itempool.h"
#include "equipment/equipment.h"

EquipOtherConfig::EquipOtherConfig()
{
}

EquipOtherConfig::~EquipOtherConfig()
{

}

bool EquipOtherConfig::Init(const std::string &configname, std::string *err)
{
	char errinfo[1024] = {0};
	int iRet = 0;

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, configname, "EquipOtherConfig", *err);

	PugiXmlNode RootElement = document.document_element();

	if (RootElement.empty())
	{
		*err = configname + ": xml root node error.";
		return false;
	}

	{
		// Ì××°ÅäÖÃ
		PugiXmlNode suit_element = RootElement.child("suit_list");
		if (suit_element.empty())
		{
			*err = configname + ": no [suit_list].";
			return false;
		}

		iRet = InitSuitCfg(suit_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitSuitCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}


	return true;
}

const EquipSuitCfg * EquipOtherConfig::GetSuit(UInt16 suit_id) const
{
	std::map<UInt16, EquipSuitCfg>::const_iterator iter = m_suit_map.find(suit_id);
	if (iter != m_suit_map.end())
	{
		return &iter->second;
	}

	return NULL;
}

int EquipOtherConfig::InitSuitCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	while (!dataElement.empty())
	{
		int suit_id = 0;
		if (!PugiGetSubNodeValue(dataElement, "id", suit_id))
		{
			return -1;
		}

		EquipSuitCfg cfg;
		cfg.m_suit_id = suit_id;

		if (!PugiGetSubNodeValue(dataElement, "is_zhen", cfg.is_zhen)) return -2;

		if (!PugiGetSubNodeValue(dataElement, "suit_level", cfg.level)) return -3;

		if (m_suit_map.find(suit_id) != m_suit_map.end())
		{
			return -100;
		}

		int suit_seq = suit_id % 1000;

		if (suit_seq >= EquipSuitCfg::MAX_SUIT_SEQ)
		{
			return -101;
		}

		std::string tmp_attr_type;
		short tmp_type = 0;
		int tmp_value = 0;

		char tmp_tag[32];
		int tmp_num = 0;

		for (int i = 1; i <= EquipSuitCfg::SUIT_ATTR_MAX_NUM; ++i)
		{
			memset(tmp_tag, 0, sizeof(tmp_tag));
			sprintf(tmp_tag, "num%d", i);
			if (!PugiGetSubNodeValue(dataElement, tmp_tag, tmp_num) || tmp_num < 0 || tmp_num > EquipSuitCfg::SUIT_ATTR_MAX_NUM)
			{
				return -10;
			}

			memset(tmp_tag, 0, sizeof(tmp_tag));
			sprintf(tmp_tag, "attr_type%d", i);
			if (!PugiGetSubNodeValue(dataElement, tmp_tag, tmp_attr_type))
			{
				return -15;
			}
			if (!CharIntAttrs::GetAttrTypeFromAttrString(tmp_attr_type, &tmp_type)) 
			{
				return -16;
			}

			memset(tmp_tag, 0, sizeof(tmp_tag));
			sprintf(tmp_tag, "attr_value%d", i);
			if (!PugiGetSubNodeValue(dataElement, tmp_tag, tmp_value) || tmp_value < 0)
			{
				return -20;
			}

			if (tmp_num >= 1 && tmp_num <= EquipSuitCfg::SUIT_ATTR_MAX_NUM)
			{
				cfg.m_attr_type_list[tmp_num] = tmp_type;
				cfg.m_attr_value_list[tmp_num] = tmp_value;
			}
		}

		m_suit_map[cfg.m_suit_id] = cfg;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}
