#ifndef __SHENZHUANG_DEF_HPP__
#define __SHENZHUANG_DEF_HPP__

#include "servercommon.h"

#pragma pack(push, 4)

static const int SHENZHUANG_MAX_PART = 10;
static const int SHENZHUANG_MAX_LEVEL = 100;

UNSTD_STATIC_CHECK(SHENZHUANG_MAX_PART % 2 == 0);

struct ShenZhuangParam
{
	ShenZhuangParam()
	{
		this->Reset();
	}

	void Reset()
	{
		day_drop_level_times = 0;
		day_gain_times = 0;
		memset(szlevel_list, 0, sizeof(szlevel_list));
	}

	short szlevel_list[SHENZHUANG_MAX_PART];
	int day_drop_level_times;
	int day_gain_times;
};

typedef char ShenZhuangParamHex[sizeof(ShenZhuangParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(ShenZhuangParamHex) < 256);

#pragma pack(pop)
#endif