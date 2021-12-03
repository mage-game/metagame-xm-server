#include "xunzhangconfig.hpp"
#include <memory.h>
#include <stdlib.h>

#include "servercommon/servercommon.h"
#include "servercommon/configcommon.h"
#include "item/itempool.h"

XunZhangConfig::XunZhangConfig() : m_suit_cfg_count(0)
{

}

XunZhangConfig::~XunZhangConfig()
{

}

bool XunZhangConfig::Init(std::string path, std::string *err)
{
	int iRet = 0;
	char err_info[1024] = {0};

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "XunZhangConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = path + " xml root node error";
		return false;
	}

	{
		// 等级属性
		PugiXmlNode element = RootElement.child("level_attr");
		if (element.empty())
		{
			*err = path + " xml not level_attr node ";
			return false;
		}

		iRet = this->InitLevelCfg(element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitLevelCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		// 套装属性
		PugiXmlNode element = RootElement.child("suit_attr");
		if (element.empty())
		{
			*err = path + " xml not suit_attr node ";
			return false;
		}

		iRet = this->InitSuitCfg(element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitSuitCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	return true;
}

int XunZhangConfig::InitLevelCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -100000;
	}

	while (!dataElement.empty())
	{
		int xunzhang_id = 0;
		if (!PugiGetSubNodeValue(dataElement, "xunzhang_id", xunzhang_id) || xunzhang_id < 0 || xunzhang_id >= MAX_XUNZHANG_COUNT)
		{
			return -1;
		}

		int level = 0;
		if (!PugiGetSubNodeValue(dataElement, "level", level) || level < 0 || level > MAX_XUNZHANG_LEVEL)
		{
			return -2;
		}

		XunZhangLevelCfg &cfg = m_level_cfg_list[xunzhang_id][level];
		if (0 != cfg.level)
		{
			return -3;
		}

		cfg.xunzhang_id = xunzhang_id;
		cfg.level = level;

		if (!PugiGetSubNodeValue(dataElement, "uplevel_stuff_id", cfg.uplevel_stuff_id) || NULL == ITEMPOOL->GetItem(cfg.uplevel_stuff_id)) 
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "uplevel_stuff_num", cfg.uplevel_stuff_num) || cfg.uplevel_stuff_num <= 0) 
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "maxhp", cfg.maxhp) || cfg.maxhp < 0) return -101;

		if (!PugiGetSubNodeValue(dataElement, "gongji", cfg.gongji) || cfg.gongji < 0) return -102;

		if (!PugiGetSubNodeValue(dataElement, "fangyu", cfg.fangyu) || cfg.fangyu < 0) return -103;

		if (!PugiGetSubNodeValue(dataElement, "mingzhong", cfg.mingzhong) || cfg.mingzhong < 0) return -104;

		if (!PugiGetSubNodeValue(dataElement, "shanbi", cfg.shanbi) || cfg.shanbi < 0) return -105;

		if (!PugiGetSubNodeValue(dataElement, "baoji", cfg.baoji) || cfg.baoji < 0) return -106;

		if (!PugiGetSubNodeValue(dataElement, "jianren", cfg.jianren) || cfg.jianren < 0) return -107;

		if (!PugiGetSubNodeValue(dataElement, "per_jingzhun", cfg.per_jingzhun) || cfg.per_jingzhun < 0) return -108;

		if (!PugiGetSubNodeValue(dataElement, "per_baoji", cfg.per_baoji) || cfg.per_baoji < 0) return -109;

		if (!PugiGetSubNodeValue(dataElement, "per_pofang", cfg.per_pofang) || cfg.per_pofang < 0) return -110;

		if (!PugiGetSubNodeValue(dataElement, "per_mianshang", cfg.per_mianshang) || cfg.per_mianshang < 0) return -111;

		if (!PugiGetSubNodeValue(dataElement, "per_monster_exp", cfg.per_monster_exp) || cfg.per_monster_exp < 0) return -112;

		if (!PugiGetSubNodeValue(dataElement, "per_boss_hurt", cfg.per_boss_hurt) || cfg.per_boss_hurt < 0) return -113;

		if (!PugiGetSubNodeValue(dataElement, "per_monster_hurt", cfg.per_monster_hurt) || cfg.per_monster_hurt < 0) return -114;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int XunZhangConfig::InitSuitCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -100000;
	}

	m_suit_cfg_count = 0;
	int last_total_level = 0;

	while (!dataElement.empty())
	{
		if (m_suit_cfg_count >= MAX_XUNZHANG_SUIT_CFG_COUNT)
		{
			return -100;
		}

		XunZhangSuitCfg &cfg = m_suit_cfg_list[m_suit_cfg_count];

		if (!PugiGetSubNodeValue(dataElement, "total_level", cfg.total_level) || cfg.total_level < last_total_level)
		{
			return -1;
		}

		last_total_level = cfg.total_level;

		if (!PugiGetSubNodeValue(dataElement, "mount_attr_add", cfg.mount_attr_add) || cfg.mount_attr_add < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "wing_attr_add", cfg.wing_attr_add) || cfg.wing_attr_add < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "halo_attr_add", cfg.halo_attr_add) || cfg.halo_attr_add < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElement, "magic_bow_attr_add", cfg.magic_bow_attr_add) || cfg.magic_bow_attr_add < 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(dataElement, "magic_wing_attr_add", cfg.magic_wing_attr_add) || cfg.magic_wing_attr_add < 0)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(dataElement, "footprint_attr_add", cfg.footprint_attr_add) || cfg.footprint_attr_add < 0)
		{
			return -10;
		}

		m_suit_cfg_count++;
		dataElement = dataElement.next_sibling();
	}

	return 0;
}

XunZhangLevelCfg * XunZhangConfig::GetXunZhangLevelCfg(int xunzhang_id, int level)
{
	if (xunzhang_id < 0 || xunzhang_id >= MAX_XUNZHANG_COUNT || level < 0 || level > MAX_XUNZHANG_LEVEL)
	{
		return NULL;
	}

	if (m_level_cfg_list[xunzhang_id][level].level != level)
	{
		return NULL;
	}

	return &m_level_cfg_list[xunzhang_id][level];
}

XunZhangSuitCfg * XunZhangConfig::GetXunZhangSuitCfg(int total_level)
{
	if (m_suit_cfg_count <= 0 || m_suit_cfg_count > MAX_XUNZHANG_SUIT_CFG_COUNT)
	{
		return NULL;
	}

	for (int i = m_suit_cfg_count - 1; i >= 0; i--)
	{
		if (total_level >= m_suit_cfg_list[i].total_level)
		{
			return &m_suit_cfg_list[i];
		}
	}

	return NULL;
}
