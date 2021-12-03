#include "impguardconfig.hpp"
#include "item/itempool.h"

ImpGuardConfig::ImpGuardConfig()
{
}

ImpGuardConfig::~ImpGuardConfig()
{
}

bool ImpGuardConfig::Init(const std::string & configname, std::string * err)
{
	char errinfo[1024] = { 0 };
	int iRet = 0;

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, configname, "ImpGuardConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = configname + ": xml root node error.";
		return false;
	}
	// ÆäËûÅäÖÃ
	{
		PugiXmlNode impguard_element = RootElement.child("other");
		if (impguard_element.empty())
		{
			*err = configname + ": no [other].";
			return false;
		}

		iRet = this->InitOtherCfg(impguard_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: ImpGuardConfig::InitOtherCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}
	// 
	{
		PugiXmlNode impguard_element = RootElement.child("imp");
		if (impguard_element.empty())
		{
			*err = configname + ": no [imp].";
			return false;
		}

		iRet = this->InitImpCfg(impguard_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: ImpGuardConfig::InitImpCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}


	return true;
}

const ImpConfig * ImpGuardConfig::GetImpCfgByImpType(int imp_type) const
{
	if (imp_type <= 0 || imp_type > IMP_GUARD_MAX_IMP_TYPE)
	{
		return NULL;
	}

	return &m_imp_cfg_list[imp_type];
}

const ImpConfig * ImpGuardConfig::GetImpCfgByItemId(ItemID item_id) const
{
	for (int i = 1; i <= IMP_GUARD_MAX_IMP_TYPE; i++)
	{
		if (m_imp_cfg_list[i].item_id == item_id)
		{
			return &m_imp_cfg_list[i];
		}
	}

	return NULL;
}

int ImpGuardConfig::InitOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	if (!PugiGetSubNodeValue(dataElement, "free_imp_type", m_other_cfg.free_imp_type) || m_other_cfg.free_imp_type <= 0 || m_other_cfg.free_imp_type > IMP_GUARD_MAX_IMP_TYPE)
	{
		return -1;
	}

	if (!PugiGetSubNodeValue(dataElement, "free_imp_item_id", m_other_cfg.free_imp_item_id) || NULL == ITEMPOOL->GetItem(m_other_cfg.free_imp_item_id))
	{
		return -2;
	}

	if (!PugiGetSubNodeValue(dataElement, "range", m_other_cfg.range) || m_other_cfg.range <= 0)
	{
		return -3;
	}

	return 0;
}

int ImpGuardConfig::InitImpCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	int imp_type = 0;
	int last_imp_type = 0;
	while (!dataElement.empty())
	{
		if (!PugiGetSubNodeValue(dataElement, "imp_type", imp_type) || imp_type <= 0 || imp_type > IMP_GUARD_MAX_IMP_TYPE || imp_type != last_imp_type + 1)
		{
			return -1;
		}

		last_imp_type = imp_type;
		m_imp_cfg_list[imp_type].imp_type = imp_type;

		if (!PugiGetSubNodeValue(dataElement, "item_id", m_imp_cfg_list[imp_type].item_id) || NULL == ITEMPOOL->GetItem(m_imp_cfg_list[imp_type].item_id))
		{
			return -2;
		}
		
		if (!PugiGetSubNodeValue(dataElement, "add_per_exp", m_imp_cfg_list[imp_type].add_per_exp) || m_imp_cfg_list[imp_type].add_per_exp < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "per_mianshang", m_imp_cfg_list[imp_type].per_mianshang) || m_imp_cfg_list[imp_type].per_mianshang < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "fangyu", m_imp_cfg_list[imp_type].fangyu) || m_imp_cfg_list[imp_type].fangyu < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "max_hp", m_imp_cfg_list[imp_type].max_hp) || m_imp_cfg_list[imp_type].max_hp < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "fujia_shanghai", m_imp_cfg_list[imp_type].fujia_shanghai) || m_imp_cfg_list[imp_type].fujia_shanghai < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElement, "per_kangbao", m_imp_cfg_list[imp_type].per_kangbao) || m_imp_cfg_list[imp_type].per_kangbao < 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(dataElement, "pvp_reduce_hurt_per", m_imp_cfg_list[imp_type].pvp_reduce_hurt_per) || m_imp_cfg_list[imp_type].pvp_reduce_hurt_per < 0)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(dataElement, "gold_price", m_imp_cfg_list[imp_type].gold_price) || m_imp_cfg_list[imp_type].gold_price < 0)
		{
			return -10;
		}

		if (!PugiGetSubNodeValue(dataElement, "bind_gold_price", m_imp_cfg_list[imp_type].bind_gold_price) || m_imp_cfg_list[imp_type].bind_gold_price < 0)
		{
			return -11;
		}

		if (!PugiGetSubNodeValue(dataElement, "appe_image_id", m_imp_cfg_list[imp_type].appe_image_id) || m_imp_cfg_list[imp_type].appe_image_id < 0)
		{
			return -12;
		}

		if (!PugiGetSubNodeValue(dataElement, "level_limit", m_imp_cfg_list[imp_type].level_limit) || m_imp_cfg_list[imp_type].level_limit < 0)
		{
			return -13;
		}

		if (!PugiGetSubNodeValue(dataElement, "use_day", m_imp_cfg_list[imp_type].use_day) || m_imp_cfg_list[imp_type].use_day < 0)
		{
			return -14;
		}

		dataElement = dataElement.next_sibling();
	}

 	return 0;
}
