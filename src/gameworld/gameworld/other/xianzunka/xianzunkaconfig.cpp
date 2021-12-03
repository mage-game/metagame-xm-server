#include "xianzunkaconfig.hpp"
#include "servercommon/configcommon.h"
#include "item/itempool.h"

XianZunKaConfig::XianZunKaConfig()
{

}

XianZunKaConfig::~XianZunKaConfig()
{

}

bool XianZunKaConfig::Init(const std::string &path, std::string *err)
{
	char error_info[1024] = { 0 };
	int iRet = 0;

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "XianZunKaConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = path + ": xml root node error.";
		return false;
	}

	//ÏÉ×ð¿¨»ù´¡ÅäÖÃ
	{
		PugiXmlNode xianzunka_base_element = RootElement.child("other_cfg");
		if (xianzunka_base_element.empty())
		{
			*err = path + "xml no [other_cfg] node.\n";
			return false;
		}

		iRet = this->InitOtherCfg(xianzunka_base_element);
		if (iRet)
		{
			sprintf(error_info, "%s InitOtherCfg failed [%d]", path.c_str(), iRet);
			*err = error_info;
			return false;
		}
	}

	//ÏÉ×ð¿¨»ù´¡ÅäÖÃ
	{
		PugiXmlNode xianzunka_base_element = RootElement.child("xianzunka_base_cfg");
		if (xianzunka_base_element.empty())
		{
			*err = path + "xml no [xianzunka_base_cfg] node.\n";
			return false;
		}

		iRet = this->InitXianZunKaCfg(xianzunka_base_element);
		if (iRet)
		{
			sprintf(error_info, "%s InitXianZunKaCfg failed [%d]", path.c_str(), iRet);
			*err = error_info;
			return false;
		}
	}

	//ÏÉ×ð¿¨¸½¼ÓÅäÖÃ
	{
		PugiXmlNode xianzunka_addition_element = RootElement.child("xianzunka_addition_cfg");
		if (xianzunka_addition_element.empty())
		{
			*err = path + "xml no [xianzunka_addition_cfg] node.\n";
			return false;
		}

		iRet = this->InitXianZunKaAdditionCfg(xianzunka_addition_element);
		if (iRet)
		{
			sprintf(error_info, "%s InitXianZunKaAdditionCfg failed [%d]", path.c_str(), iRet);
			*err = error_info;
			return false;
		}
	}

	return true;
}

const XianZunKaOtherCfg & XianZunKaConfig::GetOtherCfg()
{
	return m_other_cfg;
}

const XianZunKaBaseConfig * XianZunKaConfig::GetXianZunKaBaseCfg(int card_type)
{
	if (card_type < XIANZUNKA_TYPE_SILVER || card_type >= XIANZUNKA_TYPE_MAX)
	{
		return nullptr;
	}

	if (m_xianzunka_cfg_list[card_type].card_type != card_type)
	{
		return nullptr;
	}

	return &m_xianzunka_cfg_list[card_type];
}

const XianZunKaAdditionConfig * XianZunKaConfig::GetXianZunKaAdditionCfg(int card_type)
{
	if (card_type < XIANZUNKA_TYPE_SILVER || card_type >= XIANZUNKA_TYPE_MAX)
	{
		return nullptr;
	}

	if (m_xianzunka_addition_cfg_list[card_type].card_type != card_type)
	{
		return nullptr;
	}

	return &m_xianzunka_addition_cfg_list[card_type];
}

int XianZunKaConfig::InitOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1000;
	}

	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "active_attribute_type", m_other_cfg.active_attribute_type) || !CharIntAttrs::IsRoleBaseAttr(m_other_cfg.active_attribute_type))
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "active_attribute_value", m_other_cfg.active_attribute_value) || m_other_cfg.active_attribute_value < 0)
		{
			return -2;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int XianZunKaConfig::InitXianZunKaCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1000;
	}

	int card_type = 0;

	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "card_type", card_type) || card_type < XIANZUNKA_TYPE_SILVER || card_type >= XIANZUNKA_TYPE_MAX)
		{
			return -1;
		}

		XianZunKaBaseConfig &xianzunka_cfg = m_xianzunka_cfg_list[card_type];
		xianzunka_cfg.card_type = card_type;

		if (!PugiGetSubNodeValue(data_element, "valid_day", xianzunka_cfg.valid_day) || xianzunka_cfg.valid_day <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "need_gold", xianzunka_cfg.need_gold) || xianzunka_cfg.need_gold <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "daily_reward_bind_gold", xianzunka_cfg.daily_reward_bind_gold) || xianzunka_cfg.daily_reward_bind_gold < 0)
		{
			return -4;
		}

		PugiXmlNode first_active_reward_item_element = data_element.child("first_active_reward");
		if (first_active_reward_item_element.empty())
		{
			return -5;
		}

		if (!xianzunka_cfg.first_reward_item.ReadConfig(first_active_reward_item_element))
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "active_item_id", xianzunka_cfg.active_item_id) || nullptr == ITEMPOOL->GetItem(xianzunka_cfg.active_item_id))
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(data_element, "gift_day", xianzunka_cfg.gift_day) || xianzunka_cfg.gift_day < 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(data_element, "free_time_limit", xianzunka_cfg.free_time_limit) || xianzunka_cfg.free_time_limit < 0)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(data_element, "active_chong_zhi", xianzunka_cfg.active_chong_zhi) || xianzunka_cfg.active_chong_zhi < 0)
		{ 
			return -10;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int XianZunKaConfig::InitXianZunKaAdditionCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1000;
	}

	int card_type = 0;

	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "card_type", card_type) || card_type < XIANZUNKA_TYPE_SILVER || card_type >= XIANZUNKA_TYPE_MAX)
		{
			return -1;
		}

		XianZunKaAdditionConfig &addition_cfg = m_xianzunka_addition_cfg_list[card_type];
		addition_cfg.card_type = card_type;

		if (!PugiGetSubNodeValue(data_element, "add_exp_per", addition_cfg.add_exp_per) || addition_cfg.add_exp_per < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "add_equip_strength_succ_rate", addition_cfg.add_equip_strength_succ_rate) || addition_cfg.add_equip_strength_succ_rate < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "add_mount_grade_attrs_per", addition_cfg.add_mount_grade_attrs_per) || addition_cfg.add_mount_grade_attrs_per < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "add_wing_grade_attrs_per", addition_cfg.add_wing_grade_attrs_per) || addition_cfg.add_wing_grade_attrs_per < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "add_shenbing_grade_attrs_per", addition_cfg.add_shenbing_grade_attrs_per) || addition_cfg.add_shenbing_grade_attrs_per < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "add_shizhuang_grade_attrs_per", addition_cfg.add_shizhuang_grade_attrs_per) || addition_cfg.add_shizhuang_grade_attrs_per < 0)
		{
			return -7;
		}
		
		data_element = data_element.next_sibling();
	}

	return 0;
}