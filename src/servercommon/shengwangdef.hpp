#ifndef __SHENGWANG_DEF_HPP__
#define __SHENGWANG_DEF_HPP__

#include "servercommon.h"

#pragma pack(push, 4)

struct ShengWangParam
{
	static const int SHENGWANG_SHUXINGDAN_MAX_TYPE = 3;

	ShengWangParam()
	{
		this->Reset();
	}

	void Reset()
	{
		xianjie_level = 0;

		xiandan_level = 0;
		process_times = 0;
		reserve_sh = 0;
		day_times = 0;

		memset(shuxingdan_list, 0, sizeof(shuxingdan_list));
	}

	short xianjie_level;

	short xiandan_level;
	short process_times;
	short reserve_sh;
	int day_times;

	int shuxingdan_list[SHENGWANG_SHUXINGDAN_MAX_TYPE];
};

typedef char ShengWangParamHex[sizeof(ShengWangParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(ShengWangParamHex) < 64);

#pragma pack(pop)
#endif

