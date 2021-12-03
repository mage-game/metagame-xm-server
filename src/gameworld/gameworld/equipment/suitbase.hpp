#ifndef __SUITBASE_H__
#define __SUITBASE_H__

#include "servercommon/basedef.h"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include "obj/character/attribute.hpp"
#include "gameworld/item/itembase.h"
#include <string>

#include <map>

struct EquipSuitCfg
{
	const static UInt16 INVALID_SUIT_ID = 0;
	const static short SUIT_ATTR_MAX_NUM = 8;
	const static UInt16 MAX_SUIT_SEQ = 128;

	const static UInt16 FANGJU_SUIT_TYPE_FACTOR = 10000;
	const static UInt16 WUQI_SUIT_TYPE_FACTOR = 20000;

	EquipSuitCfg() : m_suit_id(INVALID_SUIT_ID), is_zhen(0), level(0)
	{
		memset(m_attr_type_list, 0, sizeof(m_attr_type_list));
		memset(m_attr_value_list, 0, sizeof(m_attr_value_list));
	}

	UInt16 m_suit_id;

	short is_zhen;
	short level;

	short m_attr_type_list[SUIT_ATTR_MAX_NUM + 1];
	int m_attr_value_list[SUIT_ATTR_MAX_NUM + 1];
};

class EquipOtherConfig : public ConfigBase
{
public:
	EquipOtherConfig();
	~EquipOtherConfig();

	bool Init(const std::string &configname, std::string *err);

	const EquipSuitCfg * GetSuit(UInt16 suit_id) const;

private:
	int InitSuitCfg(PugiXmlNode RootElement);
	
	std::map<UInt16, EquipSuitCfg> m_suit_map;					// Ì××°ÁÐ±í
};

#endif
