#ifndef __SHENBING_DEF_HPP__
#define __SHENBING_DEF_HPP__

#include "servercommon.h"

enum SHENBING_ID
{
	SHENBING_ID_INVALID = -1,

	SHENBING_ID_WUQI = 0,
	SHENBING_ID_CHIBANG,

	SHENBING_ID_MAX,
};

enum SHENBING_STATUS
{
	SHENBING_STATUS_NOT_ATTACH = 0,
	SHENBING_STATUS_ATTACH = 1,
	SHENBING_STATUS_FROZEN = 2,
	SHENBING_STATUS_FINISH = 3,
};

#pragma pack(push, 4)

struct ShenbingParam
{
	struct ShenbingInfo
	{
		unsigned int expire_time;
		unsigned int cd_end_time;
		short reserve1;
		short reserve2;
		int reserve3;
	};

	ShenbingParam() { this->Reset(); }

	void Reset()
	{
		attach_status = 0;
		attach_left_ms = 0;
		memset(shenbing_list, 0, sizeof(shenbing_list));
	}

	char attach_status;												// ∏ΩÃÂ◊¥Ã¨
	char reserve1;
	short reserve2;
	int attach_left_ms;												// ∏ΩÃÂ £”‡ ±º‰ ∫¡√Î
	ShenbingInfo shenbing_list[SHENBING_ID_MAX];					
};

typedef char ShenbingParamHex[sizeof(ShenbingParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(ShenbingParamHex) < 128);

#pragma pack(pop)

#endif  // __XIANJIE_DEF_HPP__

