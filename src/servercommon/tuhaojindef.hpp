#ifndef __TUHAOJIN_DEF_H__
#define __TUHAOJIN_DEF_H__

#include "servercommon.h"

#pragma pack(push, 4)

struct TuHaoJinParam
{
	TuHaoJinParam() { this->Reset(); }

	void Reset()
	{
		tuhaojin_level = 0;
		reserve = 0;
		cur_tuhaojin_color = 0;
		max_tuhaojin_color = 0;
		reserve_2 = 0;
	}

	short tuhaojin_level;
	short reserve;
	char cur_tuhaojin_color;
	char max_tuhaojin_color;
	short reserve_2;
};

typedef char TuHaoJinParamHex[sizeof(TuHaoJinParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(TuHaoJinParamHex) < 64);

#pragma pack(pop)

#endif

