#ifndef __STONE_DEF_HPP__
#define __STONE_DEF_HPP__

#include "servercommon.h"

#pragma pack(push, 4)

static const int MAX_STONE_COUNT = 6;
static const int MAX_STONE_EQUIP_PART = 11;
static const int STONE_UPLEVEL_COUNT = 4;
static const int MAX_STONE_LEVEL = 11;
static const int SYSTEM_MSG_TIME_INTERVAL = 30;		// 发宝石全身传闻的时间间隔

enum STONE_TYPE 
{
	STONE_TYPE_0 = 0,		// 攻击
	STONE_TYPE_1,			// 生命
	STONE_TYPE_2,			// 防御
	STONE_TYPE_3,			// 力量
	STONE_TYPE_4,			// 守护
	STONE_TYPE_5,			// 荣耀

	STONE_TYPE_MAX,
};

struct StoneItem
{
	StoneItem() : stone_id(0), reserve(0), reserve2(0) {}

	int stone_id;
	short reserve;
	short reserve2;

	void Reset()
	{
		stone_id = 0;
		reserve = 0;
		reserve2 = 0;
	}

	bool Invalid() const
	{
		return stone_id <= 0;
	}
};

struct StonesOnPart
{
	StoneItem stone_list[MAX_STONE_COUNT];
};

struct StoneParam
{
	StoneParam() { this->Reset(); }

	void Reset()
	{
		reserve_1 = 0;
		reserve_2 = 0;
		
		memset(stones_list, 0, sizeof(stones_list));
	}

	short reserve_1;	
	short reserve_2;											
	StonesOnPart stones_list[MAX_STONE_EQUIP_PART];
};

typedef char StoneParamHex[sizeof(StoneParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(StoneParamHex) < 2048);

#pragma pack(pop)

#endif  // __XIANJIE_DEF_HPP__

