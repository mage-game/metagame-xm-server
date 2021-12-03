#ifndef __PERSONALIZE_WINDOW_DEF_H__
#define __PERSONALIZE_WINDOW_DEF_H__

#include "servercommon.h"
#include "servercommon/struct/shizhuangparam.hpp"

#pragma pack(push, 4)

static const int BUBBLE_WINDOW_MAX_TYPE = 50;			// 气泡框最大配置数量
static const int AVATAR_WINDOW_MAX_TYPE = 50;			// 头像框最大配置数量

struct PersonalizeWindowParam
{
	PersonalizeWindowParam() { this->Reset(); }

	void Reset()
	{
		cur_use_bubble_type = -1;
		reserve = 0;
		memset(bubble_level, 0, sizeof(bubble_level));
		memset(avatar_level, 0, sizeof(avatar_level));

		cur_use_avatar_type = -1;
		reserve_sh_1 = 0;
	}
	short cur_use_bubble_type;
	short reserve;
	short bubble_level[BUBBLE_WINDOW_MAX_TYPE];
	short avatar_level[AVATAR_WINDOW_MAX_TYPE];

	short cur_use_avatar_type;
	short reserve_sh_1;
};

typedef char PersonalizeWindowParamHex[sizeof(PersonalizeWindowParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(PersonalizeWindowParamHex) < 768);

#pragma pack(pop)

#endif

