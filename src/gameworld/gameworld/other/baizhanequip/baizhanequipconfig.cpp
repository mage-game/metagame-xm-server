#include <memory.h>
#include <stdlib.h>

#include "baizhanequipconfig.hpp"

#include "servercommon/servercommon.h"
#include "servercommon/configcommon.h"
#include "servercommon/baizhanequipdef.hpp"
#include "item/itempool.h"
#include "obj/character/attribute.hpp"
#include "gameworld/checkresourcecenter.hpp"


#include <string>
#include<algorithm>

BaizhanEquipConfig::BaizhanEquipConfig()
{

}

BaizhanEquipConfig::~BaizhanEquipConfig()
{

}

bool BaizhanEquipConfig::Init(const std::string &configname, std::string *err)
{

	PUGI_XML_CONFIG_PRE_LOAD;

	// 其他
	PUGI_XML_LOAD_CONFIG("other", InitOtherConfig);

	// 装备配置
	PUGI_XML_LOAD_CONFIG("baizhan_equip_info", InitBaizhanEquipCfg);

	// 套装属性
	PUGI_XML_LOAD_CONFIG("suit_attr", InitSuitAttrCfg);

	//升级配置
	PUGI_XML_LOAD_CONFIG("equip_level_up", InitBaizhanEquipLevelCfg);

	return true;
}

// 其它配置
int BaizhanEquipConfig::InitOtherConfig(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}


	if (!PugiGetSubNodeValue(dataElement, "is_open", m_other_cfg.is_open) || m_other_cfg.is_open < 0)
	{
		return -1;
	}

	return 0;
}


int BaizhanEquipConfig::InitBaizhanEquipCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		BaizhanEquipCfg cfg;

		if (!PugiGetSubNodeValue(data_element, "equip_part", cfg.equip_part) || cfg.equip_part < 0 || cfg.equip_part >= E_INDEX_BAIZHAN::E_INDEX_BAIZHAN_MAX)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "equip_order", cfg.equip_order) || cfg.equip_order <= 0 || cfg.equip_order > ItemPool::MAX_EQUIPMENT_ORDER)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "role_need_min_level", cfg.role_need_min_level) || cfg.role_need_min_level <= 0 || cfg.role_need_min_level > MAX_ROLE_LEVEL)
		{
			return -3;
		}

		long long key = ConvertParamToLongLong(cfg.equip_part, cfg.equip_order);
		EquipPutOnLimitConfigMapIt it = m_equip_puton_limit_cfg_map.find(key);
		if (it != m_equip_puton_limit_cfg_map.end())
		{
			return -100;
		}

		m_equip_puton_limit_cfg_map[key] = cfg;

		data_element = data_element.next_sibling();
	}

	return 0;
}


const BaizhanEquipCfg * BaizhanEquipConfig::GetBaizhanEquipCfg(int equip_part, int equip_order)
{
	if (equip_part < 0 || equip_part >= Equipment::E_INDEX_MAX || equip_order <= 0 || equip_order > ItemPool::MAX_EQUIPMENT_ORDER)
	{
		return NULL;
	}

	long long key = ConvertParamToLongLong(equip_part, equip_order);
	EquipPutOnLimitConfigMapIt it = m_equip_puton_limit_cfg_map.find(key);
	if (it != m_equip_puton_limit_cfg_map.end())
	{
		return &it->second;
	}

	return NULL;
}

//套装相关-----------------------------------------------------------------------------

int BaizhanEquipConfig::InitSuitAttrCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		SuitAttrCfg cfg;

		if (!PugiGetSubNodeValue(data_element, "equip_order", cfg.equip_order) || cfg.equip_order <= 0 || cfg.equip_order >= ItemPool::MAX_EQUIPMENT_ORDER)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "same_order_num", cfg.same_order_num) || cfg.same_order_num <= 0)
		{
			return -2;
		}

		int iRet = cfg.attr_cfg.ReadConfig(data_element);
		if (iRet < 0)
		{
			return iRet;
		}

		if (!PugiGetSubNodeValue(data_element, "per_pvp_zengshenang", cfg.per_pvp_zengshenang) || cfg.per_pvp_zengshenang < 0)
		{
			return -110;
		}

		if (!PugiGetSubNodeValue(data_element, "per_pvp_mianshang", cfg.per_pvp_mianshang) || cfg.per_pvp_mianshang < 0)
		{
			return -111;
		}

		if (!PugiGetSubNodeValue(data_element, "huixinyiji", cfg.huixinyiji) || cfg.huixinyiji < 0)
		{
			return -112;
		}

		if (!PugiGetSubNodeValue(data_element, "per_jingzhun", cfg.per_jingzhun) || cfg.per_jingzhun < 0)
		{
			return -113;
		}

		if (!PugiGetSubNodeValue(data_element, "per_zhufuyiji", cfg.per_zhufuyiji) || cfg.per_zhufuyiji < 0)
		{
			return -114;
		}

		if (!PugiGetSubNodeValue(data_element, "skill_zengshang_per", cfg.skill_zengshang_per) || cfg.skill_zengshang_per < 0)
		{
			return -115;
		}

		if (!PugiGetSubNodeValue(data_element, "skill_jianshang_per", cfg.skill_jianshang_per) || cfg.skill_jianshang_per < 0)
		{
			return -116;
		}

		if (!PugiGetSubNodeValue(data_element, "per_pofang", cfg.per_pofang) || cfg.per_pofang < 0)
		{
			return -117;
		}

		if (!PugiGetSubNodeValue(data_element, "per_maxhp", cfg.per_maxhp) || cfg.per_maxhp < 0)
		{
			return -118;
		}

		if (!PugiGetSubNodeValue(data_element, "per_gongji", cfg.per_gongji) || cfg.per_gongji < 0)
		{
			return -114;
		}

		m_suit_attr_vec.push_back(cfg);

		data_element = data_element.next_sibling();
	}

	return 0;
}

int BaizhanEquipConfig::InitBaizhanEquipLevelCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{

		BaizhanEquipLevelCfg cfg;

		if (!PugiGetSubNodeValue(dataElement, "old_equip_item_id", cfg.old_equip_item_id) || nullptr == ITEMPOOL->GetItem(cfg.old_equip_item_id))
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "new_equip_item_id", cfg.new_equip_item_id) || nullptr == ITEMPOOL->GetItem(cfg.new_equip_item_id))
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "need_num", cfg.need_num) || cfg.need_num <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "is_broadcast", cfg.is_broadcast) || cfg.is_broadcast < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "part", cfg.part) || cfg.part < 0 || cfg.part >= E_INDEX_BAIZHAN_MAX)
		{
			return -5;
		}

		m_baizhan_equip_level_vec.push_back(cfg);

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

const SuitAttrCfg * BaizhanEquipConfig::GetSuitAttrCfg(int equip_order, int same_order_num)
{
	for (int i = (int)m_suit_attr_vec.size() - 1; i >= 0; -- i)
	{
		if (equip_order == m_suit_attr_vec[i].equip_order &&
			same_order_num >= m_suit_attr_vec[i].same_order_num
			)
		{
			return &m_suit_attr_vec[i];
		}
	}

	return NULL;
}

const BaizhanEquipLevelCfg * BaizhanEquipConfig::GetBaizhanEquipFirstCfg()
{
	if (m_baizhan_equip_level_vec.empty())
	{
		return nullptr;
	}

	return &m_baizhan_equip_level_vec[0];
}

const BaizhanEquipLevelCfg * BaizhanEquipConfig::GetBaizhanEquipLevelCfg(int old_equip_id, int part)
{
	auto it = std::find_if(m_baizhan_equip_level_vec.begin(), m_baizhan_equip_level_vec.end(), [old_equip_id, part](const BaizhanEquipLevelCfg &cmp_item) {
		if (old_equip_id == cmp_item.old_equip_item_id && part == cmp_item.part)
		{
			return true;
		}
		else
		{
			return false;
		}
	});

	if (it != m_baizhan_equip_level_vec.end())
	{
		return &(*it);
	}

	return NULL;
}

const BaizhanEquipLevelCfg * BaizhanEquipConfig::GetBaizhanEquipLevelReverseCfg(int new_equip_id, int part)
{
	auto it = std::find_if(m_baizhan_equip_level_vec.begin(), m_baizhan_equip_level_vec.end(), [new_equip_id, part](const BaizhanEquipLevelCfg &cmp_item) {
		if (new_equip_id == cmp_item.new_equip_item_id && part == cmp_item.part)
		{
			return true;
		}
		else
		{
			return false;
		}
	});

	if (it != m_baizhan_equip_level_vec.end())
	{
		return &(*it);
	}

	return NULL;
}

