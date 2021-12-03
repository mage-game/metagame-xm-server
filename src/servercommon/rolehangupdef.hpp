#ifndef __ROLEHANGUP_DEF_HPP__
#define __ROLEHANGUP_DEF_HPP__

#include "servercommon/servercommon.h"
#include "servercommon/serverdef.h"

#pragma pack(push, 4)

struct RoleHangupParam
{
	RoleHangupParam()
	{
		memset(hangup_set, 0, sizeof(hangup_set));
		memset(custom_set, 0, sizeof(custom_set));
	}

	void ResetHangupData()
	{
		memset(hangup_set, 0, sizeof(hangup_set));
	}

	void ResetCustomData()
	{
		memset(custom_set, 0, sizeof(custom_set));
	}

	HangupSetData hangup_set;
	CustomSetData custom_set;
};

typedef char RoleHangupSetDataHex[sizeof(HangupSetData) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(RoleHangupSetDataHex) < 256);

typedef char RoleCustomSetDataHex[sizeof(CustomSetData) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(RoleCustomSetDataHex) < 128);

#pragma pack(pop)

#endif // __ROLEHANGUP_DEF_HPP__
