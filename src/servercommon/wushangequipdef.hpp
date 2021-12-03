#ifndef __WUSHANG_EQUIP_DEF_H__
#define __WUSHANG_EQUIP_DEF_H__

#include "servercommon.h"

const static int WUSHANGEQUIP_MAX_ID_LIMIT = 40;				// 器件配置数量最大值
const static int WUSHANGEQUIP_MAX_LEVEL_LIMIT = 10;				// 装备最高等级
const static int WUSHANGEQUIP_MAX_TYPE_COUNT = 4;				// 装备类型最大限制
const static int WUSHANGEQUIP_MAX_STRONG_LIMIT = 20;			// 强化最大等级
const static int WUSHANGEQUIP_STAR_LEVEL = 10;					// 星级
const static int WUSHANGEQUIP_MAX_GLORY_ITEM_LIMIT = 20;		// 最大荣耀兑换物品数

#pragma pack(push, 4)

struct WushangEquipStarInfo
{
	WushangEquipStarInfo() : star_level(0), jinhua_val(0) {}

	short star_level;	// 星级
	short jinhua_val;	// 进化值
};

struct WuShangEquipParam
{
	WuShangEquipParam() { this->Reset(); }

	void Reset()
	{
		memset(equip_id_list, 0, sizeof(equip_id_list));
		memset(strong_level_list, 0, sizeof(strong_level_list));
		memset(star_list, 0, sizeof(star_list));
	}

	short equip_id_list[WUSHANGEQUIP_MAX_TYPE_COUNT];
	char strong_level_list[WUSHANGEQUIP_MAX_TYPE_COUNT];						// 强化等级
	WushangEquipStarInfo star_list[WUSHANGEQUIP_MAX_TYPE_COUNT];				// 星级
};

typedef char WushangEquipParamHex[sizeof(WuShangEquipParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(WushangEquipParamHex) < 128);

#pragma pack(pop)

#endif

