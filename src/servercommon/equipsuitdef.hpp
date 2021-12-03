#ifndef __EQUIPSUIT_DEF_HPP__
#define __EQUIPSUIT_DEF_HPP__

#include "servercommon.h"

#pragma pack(push, 4)

static const int MAX_SUIT_EQUIP_PART = 11;
static const int MAX_EQUIP_SUIT_LEVEL = 2;

struct EquipSuitParam
{
	EquipSuitParam() { this->Reset(); }

	void Reset()
	{
		memset(equip_suit_level_list, 0, sizeof(equip_suit_level_list));
	}

	short equip_suit_level_list[MAX_SUIT_EQUIP_PART];
};

typedef char EquipSuitParamHex[sizeof(EquipSuitParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(EquipSuitParamHex) < 64);

#pragma pack(pop)

#endif  // __STRENGTHEN_DEF_HPP__

