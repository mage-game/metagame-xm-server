#ifndef __SPOUSE_HOME_CONFIG_H__
#define __SPOUSE_HOME_CONFIG_H__

#include "servercommon/configbase.h"
#include "servercommon/configcommon.h"
#include "servercommon/spousehomedef.hpp"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include <vector>
#include <map>

static const int ROOM_SPECIAL_ROOM_THEME_MAX_CFG_COUNT = 5;            // 房间主题最大数

struct SpouseHomeOtherCfg
{
	SpouseHomeOtherCfg() : room_limit_count(0), spouse_capability_shared_per(0), max_equip_item_count(0), together_have_x_room(0), together_have_x_room_cap_add(0), 
						   discount_rand_activiy(0)
	{
	}

	int room_limit_count;
	int spouse_capability_shared_per;
	int max_equip_item_count;
	int together_have_x_room;
	int together_have_x_room_cap_add;
	int discount_rand_activiy;
};

struct RoomThemeCfg
{
	RoomThemeCfg() : index(0), need_gold(0), price_type(0){}

	int index;
	int need_gold;
	int price_type;
};

class SpouseHomeConfig : public ConfigBase
{
public:
	SpouseHomeConfig();
	~SpouseHomeConfig();

	bool Init(const std::string &configname, std::string *err);

	const SpouseHomeOtherCfg & GetOtherCfg() const { return m_other_cfg; }
	const AttrConfig * GetFurnitureBaseAttr(int item_id);
	const RoomThemeCfg * GetSpecialThemeCfg(int index) const;
	const AttrConfig * GetRoomAttrCfg(int room_theme_type);
	const int GetFurnitureShopCfg(ItemID item_id);
	const int GetCombinItemId(ItemID item_id, short furniture_type);

protected:
	int InitOtherCfg(PugiXmlNode RootElement);
	int InitFurnitureBaseAttrCfg(PugiXmlNode RootElement);
	int InitRoomThemeCfg(PugiXmlNode RootElement);
	int InitRoomAttrCfg(PugiXmlNode RootElement);
	int InitFurnitureShopCfg(PugiXmlNode RootElement);

private:
	SpouseHomeOtherCfg m_other_cfg;

	std::map<int, AttrConfig> m_furniture_base_attr_cfg_map;																// 基础属性

	int m_room_theme_cfg_count;
	RoomThemeCfg m_theme_cfg_list[ROOM_SPECIAL_ROOM_THEME_MAX_CFG_COUNT];													// 特殊主题

	int m_room_attr_cfg_count;
	AttrConfig m_room_attr_cfg[ROOM_SPECIAL_ROOM_THEME_MAX_CFG_COUNT];														// 房间属性

	std::map<int, int> m_furniture_shop_cfg_map;																			// 家具商店
};

#endif