#include "equipsuitconfig.hpp"

#include "servercommon/servercommon.h"
#include "servercommon/configcommon.h"
#include "item/itempool.h"

EquipSuitConfig::EquipSuitConfig()
{
}

EquipSuitConfig::~EquipSuitConfig()
{
	
}

bool EquipSuitConfig::Init(const std::string &configname, std::string *err)
{
	char errinfo[1024] = {0};
	int iRet = 0;

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, configname, "EquipSuitConfig", *err);

	PugiXmlNode RootElement = document.document_element();

	if (RootElement.empty())
	{
		*err = configname + ": xml root node error.";
		return false;
	}

	{
		// 装备升级套装配置
		PugiXmlNode suit_element = RootElement.child("suit_uplevel");
		if (suit_element.empty())
		{
			*err = configname + ": no [suit_uplevel].";
			return false;
		}

		iRet = InitEquipSuitUpLevelCfg(suit_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitEquipSuitUpLevelCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 史诗装备属性
		PugiXmlNode suit_element = RootElement.child("suit_attr_ss");
		if (suit_element.empty())
		{
			*err = configname + ": no [suit_attr_ss].";
			return false;
		}

		iRet = InitEquipShiShiSuitCfg(suit_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitEquipShiShiSuitCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 传奇装备属性
		PugiXmlNode suit_element = RootElement.child("suit_attr_cq");
		if (suit_element.empty())
		{
			*err = configname + ": no [suit_attr_cq].";
			return false;
		}

		iRet = InitEquipChuanQiSuitCfg(suit_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitEquipChuanQiSuitCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

int EquipSuitConfig::InitEquipSuitUpLevelCfg( PugiXmlNode RootElement )
{
	PugiXmlNode dataElement = RootElement.child("data");
	while (!dataElement.empty())
	{
		int equip_id = 0;
		if (!PugiGetSubNodeValue(dataElement, "equip_id", equip_id) || NULL == ITEMPOOL->GetItem(equip_id))
		{
			return -1;
		}
		
		SuitEquipUplevelCfg cfg;
		cfg.equip_id = equip_id;

		if (!PugiGetSubNodeValue(dataElement, "need_stuff_id_ss", cfg.need_stuff_id_ss) || NULL == ITEMPOOL->GetItem(cfg.need_stuff_id_ss))
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "need_stuff_count_ss", cfg.need_stuff_count_ss) || cfg.need_stuff_count_ss <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "need_stuff_id_cq1", cfg.need_stuff_id_cq1) || NULL == ITEMPOOL->GetItem(cfg.need_stuff_id_cq1))
		{
			return -4;
		}
	
		if (!PugiGetSubNodeValue(dataElement, "need_stuff_count_cq1", cfg.need_stuff_count_cq1) || cfg.need_stuff_count_cq1 <= 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "need_stuff_id_cq2", cfg.need_stuff_id_cq2) || NULL == ITEMPOOL->GetItem(cfg.need_stuff_id_cq2))
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "need_stuff_count_cq2", cfg.need_stuff_count_cq2) || cfg.need_stuff_count_cq2 <= 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElement, "suit_id", cfg.suit_id) || cfg.suit_id <= 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(dataElement, "total_equip_count", cfg.total_equip_cout) || cfg.total_equip_cout <= 0)
		{
			return -9;
		}

		std::map<ItemID, SuitEquipUplevelCfg>::const_iterator iter = m_suit_uplevel_map.find(equip_id);

		if (m_suit_uplevel_map.end() != iter) 
		{
			return -100;
		}

		m_suit_uplevel_map[equip_id] = cfg;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int EquipSuitConfig::InitEquipShiShiSuitCfg( PugiXmlNode RootElement )
{
	int last_suit_id = 0;
	int last_equip_count = 0;
	PugiXmlNode dataElement = RootElement.child("data");
	while (!dataElement.empty())
	{
		int suit_id = 0;
		if (!PugiGetSubNodeValue(dataElement, "suit_id", suit_id)|| suit_id <= 0)
		{
			return -20;
		}

		int equip_count = 0;
		if (!PugiGetSubNodeValue(dataElement, "equip_count", equip_count)|| equip_count <= 0)
		{
			return -1;
		}

		if (last_suit_id == suit_id && equip_count != last_equip_count + 1)
		{
			return -1000;
		}
		last_suit_id = suit_id;
		last_equip_count = equip_count;

		EquipSuitAttrCfg cfg;
		cfg.suit_id = suit_id;
		cfg.equip_count = equip_count;

		if (!PugiGetSubNodeValue(dataElement, "maxhp", cfg.maxhp) || cfg.maxhp < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "gongji", cfg.gongji) || cfg.gongji < 0)
		{
			return -3;
		}
		
		if (!PugiGetSubNodeValue(dataElement, "fangyu", cfg.fangyu) || cfg.fangyu < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "mingzhong", cfg.mingzhong) || cfg.mingzhong < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "shanbi", cfg.shanbi) || cfg.shanbi < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "baoji", cfg.baoji) || cfg.baoji < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElement, "jianren", cfg.jianren) || cfg.jianren < 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(dataElement, "maxhp_attr", cfg.maxhp_attr) || cfg.maxhp_attr < 0)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(dataElement, "gongji_attr", cfg.gongji_attr) || cfg.gongji_attr < 0)
		{
			return -10;
		}

		if (!PugiGetSubNodeValue(dataElement, "fangyu_attr", cfg.fangyu_attr) || cfg.fangyu_attr < 0)
		{
			return -11;
		}

		if (!PugiGetSubNodeValue(dataElement, "mingzhong_attr", cfg.mingzhong_attr) || cfg.mingzhong_attr < 0)
		{
			return -12;
		}

		if (!PugiGetSubNodeValue(dataElement, "shanbi_attr", cfg.shanbi_attr) || cfg.shanbi_attr < 0)
		{
			return -13;
		}

		if (!PugiGetSubNodeValue(dataElement, "baoji_attr", cfg.baoji_attr) || cfg.baoji_attr < 0)
		{
			return -14;
		}

		if (!PugiGetSubNodeValue(dataElement, "jianren_attr", cfg.jianren_attr) || cfg.jianren_attr < 0)
		{
			return -15;
		}

		if (!PugiGetSubNodeValue(dataElement, "fujiashanghai", cfg.fujiashanghai) || cfg.fujiashanghai < 0)
		{
			return -16;
		}

		if (!PugiGetSubNodeValue(dataElement, "dikangshanghai", cfg.dikangshanghai) || cfg.dikangshanghai < 0)
		{
			return -17;
		}

		if (!PugiGetSubNodeValue(dataElement, "is_broacast", cfg.is_broacast) || cfg.is_broacast < 0)
		{
			return -18;
		}
		
		long long key = ConvertParamToLongLong(suit_id, equip_count);
		std::map<long long, EquipSuitAttrCfg>::const_iterator iter = m_suit_attr_ss_map.find(key);

		if (m_suit_attr_ss_map.end() != iter) 
		{
			return -100;
		}

		m_suit_attr_ss_map[key] = cfg;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int EquipSuitConfig::InitEquipChuanQiSuitCfg( PugiXmlNode RootElement )
{
	PugiXmlNode dataElement = RootElement.child("data");
	while (!dataElement.empty())
	{
		int suit_id = 0;
		if (!PugiGetSubNodeValue(dataElement, "suit_id", suit_id)|| suit_id <= 0)
		{
			return -1;
		}

		int equip_count = 0;
		if (!PugiGetSubNodeValue(dataElement, "equip_count", equip_count)|| equip_count <= 0)
		{
			return -1;
		}

		EquipSuitAttrCfg cfg;
		cfg.suit_id = suit_id;
		cfg.equip_count = equip_count;

		if (!PugiGetSubNodeValue(dataElement, "maxhp", cfg.maxhp) || cfg.maxhp < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "gongji", cfg.gongji) || cfg.gongji < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "fangyu", cfg.fangyu) || cfg.fangyu < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "mingzhong", cfg.mingzhong) || cfg.mingzhong < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "shanbi", cfg.shanbi) || cfg.shanbi < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "baoji", cfg.baoji) || cfg.baoji < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElement, "jianren", cfg.jianren) || cfg.jianren < 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(dataElement, "maxhp_attr", cfg.maxhp_attr) || cfg.maxhp_attr < 0)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(dataElement, "gongji_attr", cfg.gongji_attr) || cfg.gongji_attr < 0)
		{
			return -10;
		}

		if (!PugiGetSubNodeValue(dataElement, "fangyu_attr", cfg.fangyu_attr) || cfg.fangyu_attr < 0)
		{
			return -11;
		}

		if (!PugiGetSubNodeValue(dataElement, "mingzhong_attr", cfg.mingzhong_attr) || cfg.mingzhong_attr < 0)
		{
			return -12;
		}

		if (!PugiGetSubNodeValue(dataElement, "shanbi_attr", cfg.shanbi_attr) || cfg.shanbi_attr < 0)
		{
			return -13;
		}

		if (!PugiGetSubNodeValue(dataElement, "baoji_attr", cfg.baoji_attr) || cfg.baoji_attr < 0)
		{
			return -14;
		}

		if (!PugiGetSubNodeValue(dataElement, "jianren_attr", cfg.jianren_attr) || cfg.jianren_attr < 0)
		{
			return -15;
		}

		if (!PugiGetSubNodeValue(dataElement, "fujiashanghai", cfg.fujiashanghai) || cfg.fujiashanghai < 0)
		{
			return -16;
		}

		if (!PugiGetSubNodeValue(dataElement, "dikangshanghai", cfg.dikangshanghai) || cfg.dikangshanghai < 0)
		{
			return -17;
		}

		long long key = ConvertParamToLongLong(suit_id, equip_count);
		std::map<long long, EquipSuitAttrCfg>::const_iterator iter = m_suit_attr_cq_map.find(key);

		if (m_suit_attr_cq_map.end() != iter) 
		{
			return -100;
		}

		m_suit_attr_cq_map[key] = cfg;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

const SuitEquipUplevelCfg * EquipSuitConfig::GetSuitUpLevelCfg( ItemID equip_id ) const
{
	std::map<UInt16, SuitEquipUplevelCfg>::const_iterator iter = m_suit_uplevel_map.find(equip_id);
	if (iter != m_suit_uplevel_map.end())
	{
		return &iter->second;
	}

	return NULL;
}

const EquipSuitAttrCfg * EquipSuitConfig::GetEquipShiShiSuitCfg( int suit_id, int equip_count ) const
{
	if (suit_id <= 0 || equip_count <=0)
	{
		return NULL;
	}

	long long key = ConvertParamToLongLong(suit_id, equip_count);
	std::map<long long, EquipSuitAttrCfg>::const_iterator iter = m_suit_attr_ss_map.find(key);
	if (iter != m_suit_attr_ss_map.end())
	{
		return &iter->second;
	}

	return NULL;
}

const EquipSuitAttrCfg * EquipSuitConfig::GetEquipChuanQiSuitCfg( int suit_id, int equip_count ) const
{
	if (suit_id <= 0 || equip_count <=0)
	{
		return NULL;
	}

	long long key = ConvertParamToLongLong(suit_id, equip_count);
	std::map<long long, EquipSuitAttrCfg>::const_iterator iter = m_suit_attr_cq_map.find(key);
	if (iter != m_suit_attr_cq_map.end())
	{
		return &iter->second;
	}

	return NULL;
}

