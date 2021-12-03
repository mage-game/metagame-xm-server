#ifndef __COMBINE_SERVER_EQUIP_DEF_HPP__
#define __COMBINE_SERVER_EQUIP_DEF_HPP__

#include "servercommon.h"

#pragma pack(push, 4)

static const int CSA_EQUIP_MAX_PART = 10;
static const int CSA_EQUIP_MAX_LEVEL = 100;

UNSTD_STATIC_CHECK(CSA_EQUIP_MAX_LEVEL % 2 == 0);

struct CSAEquipParam
{
	CSAEquipParam()
	{
		this->Reset();
	}

	void Reset()
	{
		memset(common_level_list, 0, sizeof(common_level_list));
		memset(great_level_list, 0, sizeof(great_level_list));
		active_special = 0;
		is_active_sys = 0;
		reserve_ch_2 = 0;
	}

	short common_level_list[CSA_EQUIP_MAX_PART];
	short great_level_list[CSA_EQUIP_MAX_PART];
	short active_special;
	char is_active_sys;							// 是否合服开启，用于客户端
	char reserve_ch_2;
};

typedef char CSAEquipParamHex[sizeof(CSAEquipParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(CSAEquipParamHex) < 128);

#pragma pack(pop)
#endif //__COMBINE_SERVER_EQUIP_DEF_HPP__