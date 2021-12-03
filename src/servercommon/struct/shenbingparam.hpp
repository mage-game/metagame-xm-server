#ifndef __SHENBING_PARAM_HPP__
#define __SHENBING_PARAM_HPP__

#include "servercommon/servercommon.h"

#pragma pack(push) 
#pragma pack(4)

struct ShenBingParam
{
	static const int SHUXINGDAN_MAX_TYPE = 2;				// 属性丹类型最大数

	ShenBingParam()
	{
		this->Reset();
	}

	void Reset()
	{
		level = 0;
		use_image = 0;
		memset(shuxingdan_list, 0, sizeof(shuxingdan_list));
		exp = 0;

		last_skill1_time = 0;
		last_skill2_time = 0;

		capability = 0;
	}

	short level;
	short use_image;
	unsigned short shuxingdan_list[SHUXINGDAN_MAX_TYPE];	// 属性丹数量列表
	int exp;

	unsigned long long last_skill1_time;
	unsigned long long last_skill2_time;

	int capability;
};

#pragma pack(pop)

typedef char ShenBingParamHex[sizeof(ShenBingParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(ShenBingParamHex) < 128);

#endif // __SHENBING_PARAM_HPP__
