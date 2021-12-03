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
	XUNZHANG_ADD_KILL_MONSTER_EXP_REWARD_PER = XUNZHANG_ADD_TYPE_MIN,		// ɱ�־��飺+X%
	XUNZHANG_ADD_HURT_TO_BOSS_PER,											// ��Ҷ�boos����˺���+X%
	XUNZHANG_ADD_HURT_TO_MONSTER_PER,										// ��ҶԹ�������˺���+X%
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
