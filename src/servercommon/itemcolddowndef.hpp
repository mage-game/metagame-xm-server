#ifndef __ITEMCOLDDOWN_DEF_HPP__
#define __ITEMCOLDDOWN_DEF_HPP__

#include "servercommon/servercommon.h"

static const int MAX_COLDDOWN_NUM = 15;		// 最大物品CD数量

#pragma pack(push, 4)

struct ItemColddownInfo
{
	ItemColddownInfo() : colddown_id(0), reserve(0), end_time(0) {}

	UInt16 colddown_id;
	short reserve;
	UInt32 end_time;
};

struct ItemColddownParam
{
	ItemColddownParam() : count(0) {}

	void Reset()
	{
		count = 0;
		memset(colddown_info_list, 0, sizeof(colddown_info_list));
	}

	int count;
	ItemColddownInfo colddown_info_list[MAX_COLDDOWN_NUM];
};

typedef char ItemColddownParamHex[sizeof(ItemColddownParam) * 2 + 1];

UNSTD_STATIC_CHECK(sizeof(ItemColddownParamHex) < 256);

#pragma pack(pop)

#endif

