#ifndef __HOME_SYSTEM_DEF_HPP__
#define __HOME_SYSTEM_DEF_HPP__

#include "servercommon.h"
#include "virtualbaggridparamdef.hpp"

static const int SPOUSE_HOME_FURNITURE_MAX_ITEM_SLOT = 20;						// 最大可装道具数
static const int SPOUSE_HOME_FURNITURE_MAX_SHOP_ITEM_COUNT = 100;				// 商店格子数
static const int SPOUSE_HOME_MAX_ATTR_NUM = 7;
static const int SPOUSE_HOME_MAX_ROOM_NUM = 10;									// 最大房间数量

#pragma pack(push, 4)

struct SpouseHomeRoomParam
{
	SpouseHomeRoomParam() { this->Reset(); }

	void Reset()
	{
		is_open = 0;
		special_theme = -1;
		reserve_sh = 0;

		memset(furniture_slot_list, 0, sizeof(furniture_slot_list));
	}

	char is_open;
	char special_theme;
	short reserve_sh;
	ItemID furniture_slot_list[SPOUSE_HOME_FURNITURE_MAX_ITEM_SLOT];
};

struct SpouseHomeParam
{
	SpouseHomeParam() { this->Reset(); }

	void Reset()
	{
		lover_id = 0;

		for (int i = 0; i < SPOUSE_HOME_MAX_ROOM_NUM; ++i)
		{
			room_list[i].Reset();
		}

		open_room_count = 0;
		is_cap_upgrade = 0;
		has_check_shared_attr_flag = 0;
	}

	int lover_id;
	short open_room_count;
	char is_cap_upgrade;				// 同时拥有x套类型房间增加共享战力
	char has_check_shared_attr_flag;	// 是否检查过共享属性标记
	SpouseHomeRoomParam room_list[SPOUSE_HOME_MAX_ROOM_NUM];
};

typedef char SpouseHomeParamHex[sizeof(SpouseHomeParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(SpouseHomeParamHex) < 1024);

#pragma pack(pop)
#endif