#ifndef __COMMON_ACTIVITY_PARAM_HPP__
#define __COMMON_ACTIVITY_PARAM_HPP__

#include "servercommon/servercommon.h"

#pragma pack(push) 
#pragma pack(4)

struct CommonActivityParam
{
	CommonActivityParam() { this->Reset(); }
	
	void Reset()
	{	
		last_join_activity_timestamp = 0;
	}

	unsigned int last_join_activity_timestamp;
};

#pragma pack(pop)

typedef char CommonActivityParamHex[sizeof(CommonActivityParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(CommonActivityParamHex) < 512);

#endif // __COMMON_ACTIVITY_PARAM_HPP__

