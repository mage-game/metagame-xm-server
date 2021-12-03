#ifndef __XUNZHANG_PARAM_HPP__
#define __XUNZHANG_PARAM_HPP__

#include "servercommon/servercommon.h"

#pragma pack(push) 
#pragma pack(4)

static const int MAX_XUNZHANG_COUNT = 6;
static const int MAX_XUNZHANG_LEVEL = 400;

enum XUNZHANG_ATTRIBUTE
{
	XUNZHANG_ADD_TYPE_MIN = 0,
	XUNZHANG_ADD_KILL_MONSTER_EXP_REWARD_PER = XUNZHANG_ADD_TYPE_MIN,		// 杀怪经验：+X%
	XUNZHANG_ADD_HURT_TO_BOSS_PER,											// 玩家对boos造成伤害：+X%
	XUNZHANG_ADD_HURT_TO_MONSTER_PER,										// 玩家对怪物造成伤害：+X%
	XUNZHANG_ADD_TYPE_MAX,
};

struct XunZhangParam
{
	XunZhangParam() { this->Reset(); }

	void Reset()
	{
		memset(level_list, 0, sizeof(level_list));
	}

	short level_list[MAX_XUNZHANG_COUNT];
};

#pragma pack(pop)

typedef char XunZhangParamHex[sizeof(XunZhangParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(XunZhangParamHex) < 128);

#endif // __XUNZHANG_PARAM_HPP__
