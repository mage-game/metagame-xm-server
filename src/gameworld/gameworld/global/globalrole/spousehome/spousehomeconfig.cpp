#include "spousehomeconfig.hpp"
#include "item/itempool.h"

SpouseHomeConfig::SpouseHomeConfig() : m_room_theme_cfg_count(0), m_room_attr_cfg_count(0)
{
}

SpouseHomeConfig::~SpouseHomeConfig()
{
}

bool SpouseHomeConfig::Init(const std::string &configname, std::string *err)
{
	char errinfo[1024] = { 0 };
	int iRet = 0;

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, configname, "SpouseHomeConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = configname + ": xml root node error.";
		return false;
	}

	// 其他配置
	{
		PugiXmlNode bosscard_element = RootElement.child("other");
		if (bosscard_element.empty())
		{
			*err = configname + ": no [other].";
			return false;
		}

		iRet = this->InitOtherCfg(bosscard_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: SpouseHomeConfig::InitOtherCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	// 家具
	{
		PugiXmlNode bosscard_element = RootElement.child("furniture_base_attr");
		if (bosscard_element.empty())
		{
			*err = configname + ": no [furniture_base_attr].";
			return false;
		}

		iRet = this->InitFurnitureBaseAttrCfg(bosscard_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: SpouseHomeConfig::InitFurnitureBaseAttrCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	// 房间主题
	{
		PugiXmlNode bosscard_element = RootElement.child("buy_special_theme");
		if (bosscard_element.empty())
		{
			*err = configname + ": no [buy_special_theme].";
			return false;
		}

		iRet = this->InitRoomThemeCfg(bosscard_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: SpouseHomeConfig::InitRoomThemeCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	// 房间属性
	{
		PugiXmlNode bosscard_element = RootElement.child("room_attr");
		if (bosscard_element.empty())
		{
			*err = configname + ": no [room_attr].";
			return false;
		}

		iRet = this->InitRoomAttrCfg(bosscard_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: SpouseHomeConfig::InitRoomAttrCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	// 家具商城
	{
		PugiXmlNode bosscard_element = RootElement.child("furniture_shop");
		if (bosscard_element.empty())
		{
			*err = configname + ": no [furniture_shop].";
			return false;
		}

		iRet = this->InitFurnitureShopCfg(bosscard_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: SpouseHomeConfig::InitFurnitureShopCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

const AttrConfig * SpouseHomeConfig::GetFurnitureBaseAttr(int item_id)
{
	std::map<int, AttrConfig>::iterator map_iter =  m_furniture_base_attr_cfg_map.find(item_id);
	if (map_iter != m_furniture_base_attr_cfg_map.end())
	{
		return &map_iter->second;
	}

	return nullptr;
}

const RoomThemeCfg * SpouseHomeConfig::GetSpecialThemeCfg(int index) const
{
	if (index < 0 || index >= m_room_theme_cfg_count || index >= ROOM_SPECIAL_ROOM_THEME_MAX_CFG_COUNT)
	{
		return nullptr;
	}

	return &m_theme_cfg_list[index];
}

const AttrConfig * SpouseHomeConfig::GetRoomAttrCfg(int room_theme_type)
{
	if (room_theme_type < 0 || room_theme_type >= ROOM_SPECIAL_ROOM_THEME_MAX_CFG_COUNT || room_theme_type >= m_room_attr_cfg_count)
	{
		return nullptr;
	}

	return &m_room_attr_cfg[room_theme_type];
}

const int SpouseHomeConfig::GetFurnitureShopCfg(ItemID item_id)
{
	auto tmp_map_it = m_furniture_shop_cfg_map.find(item_id);
	if (tmp_map_it != m_furniture_shop_cfg_map.end())
	{
		return tmp_map_it->second;
	}

	return -1;
}

const int SpouseHomeConfig::GetCombinItemId(ItemID item_id, short furniture_type)
{
	int combin_item_id = 0;
	combin_item_id |= item_id;
	combin_item_id = combin_item_id << 16;
	combin_item_id |= furniture_type;

	return combin_item_id;
}

int SpouseHomeConfig::InitOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	if (!PugiGetSubNodeValue(data_element, "room_limit_count", m_other_cfg.room_limit_count) || m_other_cfg.room_limit_count <= 0)
	{
		return -1;
	}

	if (!PugiGetSubNodeValue(data_element, "spouse_capability_shared_per", m_other_cfg.spouse_capability_shared_per) || m_other_cfg.spouse_capability_shared_per <= 0)
	{
		return -2;
	}

	if (!PugiGetSubNodeValue(data_element, "max_equip_item_count", m_other_cfg.max_equip_item_count) || m_other_cfg.max_equip_item_count <= 0)
	{
		return -3;
	}

	if (!PugiGetSubNodeValue(data_element, "together_have_x_room", m_other_cfg.together_have_x_room) || m_other_cfg.together_have_x_room <= 0)
	{
		return -4;
	}
	
	if (!PugiGetSubNodeValue(data_element, "together_have_x_room_cap_add", m_other_cfg.together_have_x_room_cap_add) || m_other_cfg.together_have_x_room_cap_add < 0)
	{
		return -5;
	}
	if (!PugiGetSubNodeValue(data_element, "discount_rand_activiy", m_other_cfg.discount_rand_activiy) || m_other_cfg.discount_rand_activiy < 0)
	{
		return -6;
	}
	return 0;
}

int SpouseHomeConfig::InitFurnitureBaseAttrCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		ItemID item_id = 0;
		short furniture_type = -1;

		if (!PugiGetSubNodeValue(data_element, "item_id", item_id) || nullptr == ITEMPOOL->GetItem(item_id))
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "imprint_slot", furniture_type) || furniture_type < 0)
		{
			return -2;
		}

		int combin_item_id = this->GetCombinItemId(item_id, furniture_type);

		AttrConfig attr_cfg;
		int iRet = attr_cfg.ReadConfig(data_element);
		if (iRet < 0)
		{
			return iRet;
		}

		if (!m_furniture_base_attr_cfg_map.emplace(std::make_pair(combin_item_id, attr_cfg)).second)
		{
			return -4;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int SpouseHomeConfig::InitRoomThemeCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_room_theme_cfg_count = 0;
	int last_index = -1;

	while (!data_element.empty())
	{
		if (m_room_theme_cfg_count >= ROOM_SPECIAL_ROOM_THEME_MAX_CFG_COUNT)
		{
			return -10001;
		}

		RoomThemeCfg &room_theme_cfg = m_theme_cfg_list[m_room_theme_cfg_count];

		if (!PugiGetSubNodeValue(data_element, "theme_type", room_theme_cfg.index) || room_theme_cfg.index < 0 || last_index + 1 != room_theme_cfg.index)
		{
			return -10002;
		}

		last_index = room_theme_cfg.index;

		if (!PugiGetSubNodeValue(data_element, "buy_need_gold", room_theme_cfg.need_gold) || room_theme_cfg.need_gold <= 0)
		{
			return -10003;
		}

		if (!PugiGetSubNodeValue(data_element, "price_type", room_theme_cfg.price_type) || (room_theme_cfg.price_type != 0 && room_theme_cfg.price_type != 1))
		{
			return -10004;
		}

		++m_room_theme_cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int SpouseHomeConfig::InitRoomAttrCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_room_attr_cfg_count = 0;
	int theme_type = -1;
	int last_theme_type = -1;

	while (!data_element.empty())
	{
		if (m_room_attr_cfg_count >= ROOM_SPECIAL_ROOM_THEME_MAX_CFG_COUNT)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "theme_type", theme_type) || theme_type < 0 || last_theme_type + 1 != theme_type)
		{
			return -2;
		}

		last_theme_type = theme_type;

		int ret = m_room_attr_cfg[m_room_attr_cfg_count].ReadConfig(data_element);
		if (0 != ret)
		{
			return ret;
		}

		++m_room_attr_cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int SpouseHomeConfig::InitFurnitureShopCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		int furniture_item_id = 0;
		int need_gold = 0;

		if (!PugiGetSubNodeValue(data_element, "item_id", furniture_item_id) || !ITEMPOOL->GetItem(furniture_item_id))
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "need_gold", need_gold) || need_gold <= 0)
		{
			return -2;
		}

		if (!m_furniture_shop_cfg_map.emplace(std::make_pair(furniture_item_id, need_gold)).second)
		{
			return -3;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}