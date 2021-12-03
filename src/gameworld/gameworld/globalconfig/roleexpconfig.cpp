#include "roleexpconfig.h"
#include "servercommon/configcommon.h"
#include "globalconfig/globalconfig.h"
#include "item/itempool.h"

RoleLevelConfig::RoleLevelConfig()
{
	memset(m_rolelevelcfg_list, 0, sizeof(m_rolelevelcfg_list));
}

RoleLevelConfig::~RoleLevelConfig()
{

}

bool RoleLevelConfig::Init(const std::string &configname, std::string *err)
{
	TiXmlDocument m_document;

	if (configname == "" || !m_document.LoadFile(configname))
	{
		*err = configname + " Load fail: " +m_document.ErrorDesc();
		return false;
	}

	TiXmlElement *RootElement = m_document.RootElement();
	if (!RootElement)
	{
		*err = configname + ": xml node error in root";
		return false;
	}

	TiXmlElement *ExpConfigElement = RootElement->FirstChildElement("exp_config");
	if (NULL == ExpConfigElement)
	{
		*err = configname + ": xml node error in [exp_config] not exist.";
		return false;
	}

	TiXmlElement *CurrElement = ExpConfigElement->FirstChildElement("data");
	if (NULL == CurrElement)
	{
		*err = configname + ": xml node error in [exp_config->data] not exist.";
		return false;
	}

	for (int i = 1; i <= MAX_ROLE_LEVEL; ++i)
	{
		if (NULL == CurrElement)
		{
			*err = configname + ": xml node error in [data]";
			return false;
		}

		int level = 0;
		if (!GetSubNodeValue(CurrElement, "level", level) || level != i)
		{
			*err = configname + ": xml node error in [level].";
			return false;
		}

		if (!GetSubNodeValue(CurrElement, "exp", m_rolelevelcfg_list[i].max_exp) || m_rolelevelcfg_list[i].max_exp <= 0)
		{
			*err = configname + ": xml node error in [exp].";
			return false;
		}

		if (!GetSubNodeValue(CurrElement, "maxhp", m_rolelevelcfg_list[i].maxhp))
		{
			*err = configname + ": xml node error in [maxhp].";
			return false;
		}

		if (!GetSubNodeValue(CurrElement, "maxmp", m_rolelevelcfg_list[i].maxmp))
		{
			*err = configname + ": xml node error in [maxmp].";
			return false;
		}

		if (!GetSubNodeValue(CurrElement, "gongji", m_rolelevelcfg_list[i].gongji))
		{
			*err = configname + ": xml node error in [gongji].";
			return false;
		}

		if (!GetSubNodeValue(CurrElement, "fangyu", m_rolelevelcfg_list[i].fangyu))
		{
			*err = configname + ": xml node error in [fangyu].";
			return false;
		}

		if (!GetSubNodeValue(CurrElement, "mingzhong", m_rolelevelcfg_list[i].mingzhong))
		{
			*err = configname + ": xml node error in [mingzhong].";
			return false;
		}

		if (!GetSubNodeValue(CurrElement, "shanbi", m_rolelevelcfg_list[i].shanbi))
		{
			*err = configname + ": xml node error in [shanbi].";
			return false;
		}

		CurrElement = CurrElement->NextSiblingElement("data");
	}

	return true;
}

long long RoleLevelConfig::GetRoleMaxExp(int level)
{
	if (level <= 0 || level > MAX_ROLE_LEVEL) return 999999999999;

	return m_rolelevelcfg_list[level].max_exp;
}

RoleLevelCfg * RoleLevelConfig::GetLevelCfg(int level)
{
	if (level <= 0 || level > MAX_ROLE_LEVEL) return 0;

	return &m_rolelevelcfg_list[level];
}


RoleZhuanShengConfig::RoleZhuanShengConfig()
{
	memset(m_zhuansheng_cfg_list, 0, sizeof(m_zhuansheng_cfg_list));
}

RoleZhuanShengConfig::~RoleZhuanShengConfig()
{

}

bool RoleZhuanShengConfig::Init(const std::string &configname, std::string *err)
{
	TiXmlDocument m_document;

	if (configname == "" || !m_document.LoadFile(configname))
	{
		*err = configname + " Load fail: " +m_document.ErrorDesc();
		return false;
	}

	TiXmlElement *RootElement = m_document.RootElement();
	if (!RootElement)
	{
		*err = configname + ": xml node error in root";
		return false;
	}

	TiXmlElement *ExpConfigElement = RootElement->FirstChildElement("zhuansheng");
	if (NULL == ExpConfigElement)
	{
		*err = configname + ": xml node error in [zhuansheng] not exist.";
		return false;
	}

	TiXmlElement *CurrElement = ExpConfigElement->FirstChildElement("data");
	if (NULL == CurrElement)
	{
		*err = configname + ": xml node error in [zhuansheng->data] not exist.";
		return false;
	}

	for (int i = 1; i <= MAX_ZHUAN_SHENG_LEVEL; ++i)
	{
		if (NULL == CurrElement)
		{
			*err = configname + ": xml node error in [data]";
			return false;
		}
		
		RoleZhuanShengCfg *cfg = &m_zhuansheng_cfg_list[i];
		if (!GetSubNodeValue(CurrElement, "zhuansheng_level", cfg->zhuansheng_level) || cfg->zhuansheng_level != i)
		{
			*err = configname + ": xml node error in [zhuansheng_level].";
			return false;
		}
		
		TiXmlElement *Itemelement = CurrElement->FirstChildElement("consume_item");
		ItemID itemid = 0;
		if (NULL != Itemelement && GetSubNodeValue(Itemelement, "item_id", itemid) && NULL != ITEMPOOL->GetItem(itemid))
		{
			if (!cfg->consume_item.ReadConfig(Itemelement))
			{
				*err = configname + ": xml node error in [consume_item].";
				return false;
			}
		}

		if (!GetSubNodeValue(CurrElement, "is_broadcast", cfg->is_broadcast))
		{
			return false;
		}

		CurrElement = CurrElement->NextSiblingElement("data");
	}
	return true;
}

RoleZhuanShengCfg * RoleZhuanShengConfig::GetZhuanShengCfg(int role_level)
{
	if(role_level <=0 || role_level > MAX_ROLE_LEVEL) return NULL;
	
	int zhuansheng_level = role_level / ZHUANSHENG_LEVEL_STEP;
	if(zhuansheng_level <= 0 || zhuansheng_level > MAX_ZHUAN_SHENG_LEVEL) return NULL;

	return &m_zhuansheng_cfg_list[zhuansheng_level];
}
