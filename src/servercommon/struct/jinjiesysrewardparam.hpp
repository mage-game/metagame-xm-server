#ifndef __JINJIESYS_PARAM_HPP__
#define __JINJIESYS_PARAM_HPP__

#include "servercommon/servercommon.h"

static const int JINJIE_REWARD_TYPE_MAX_COUNT = 2;
static const int JINJIE_TYPE_MAX_COUNT = 32;

#pragma pack(push) 
#pragma pack(4)

struct JinjieSysRewardParam
{
	JinjieSysRewardParam()
	{
		this->Reset();
	}

	void Reset()
	{
		had_init = 0;
		reserve_ch = 0;
		reserve_sh = 0;
		memset(complete_timestamp_list, 0, sizeof(complete_timestamp_list));
	}

	char had_init;
	char reserve_ch;
	short reserve_sh;
	unsigned int complete_timestamp_list[JINJIE_REWARD_TYPE_MAX_COUNT][JINJIE_TYPE_MAX_COUNT];
};

#pragma pack(pop)

typedef char JinjieSysRewardParamHex[sizeof(JinjieSysRewardParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(JinjieSysRewardParamHex) < 1024);

#endif  // __JINJIESYS_PARAM_HPP__

