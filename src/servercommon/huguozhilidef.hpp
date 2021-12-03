// 护国之力定义
#ifndef __HUGUOZHILI_DEF_HPP__
#define __HUGUOZHILI_DEF_HPP__

#include "servercommon.h"

#pragma pack(push, 4)

struct HuguozhiliParam
{
	HuguozhiliParam() { this->Reset(); }

	void Reset()
	{
		today_die_times = 0;
		today_active_times = 0;
		active_huguozhili_timestamp = 0;
	}

	int today_die_times;				// 今日死亡次数
	int today_active_times;				// 今日激活次数
	unsigned int active_huguozhili_timestamp;	// 激活护国之力的时间
};

#pragma pack(pop)

typedef char HuguozhiliParamHex[sizeof(HuguozhiliParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(HuguozhiliParamHex) < 256)

#endif // __HUGUOZHILI_DEF_HPP__