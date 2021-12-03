#ifndef __DRESSING_ROOM_DEF_HPP__
#define __DRESSING_ROOM_DEF_HPP__

#include "servercommon.h"

#pragma pack(push, 4)

struct DressingRoomParam
{
	static const int MAX_SUIT_COUNT = 64;

	DressingRoomParam()
	{
		this->Reset();
	}

	void Reset()
	{
		reserve_ch_0 = 0;
		reserve_ch_1 = 0;
		reserve_sh = 0;
		memset(suit_list, 0, sizeof(suit_list));
	}
	char reserve_ch_0;
	char reserve_ch_1;
	short reserve_sh;
	int suit_list[MAX_SUIT_COUNT];
};

typedef char DressingRoomParamHex[sizeof(DressingRoomParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(DressingRoomParamHex) < 1024);

#pragma pack(pop)
#endif // __DRESSING_ROOM_DEF_HPP__

