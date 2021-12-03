#ifndef __PASTURESPIRIT_DEF_HPP__
#define __PASTURESPIRIT_DEF_HPP__

#include "servercommon.h"

static const int PASTURE_SPIRIT_MAX_COUNT = 12;				// 牧场精灵最大数目
static const int PASTURE_SPIRIT_MAX_LEVEL = 100;			// 牧场精灵最高等级
static const int PASTURE_SPIRIT_MAX_QUALITY = 20;			// 牧场精灵最高品质

#pragma pack(push, 4)

struct PastureSpiritItem
{
	PastureSpiritItem() { this->Reset(); }

	void Reset()
	{
		level = 0;
		quality = 0;
		reserve_sh = 0;
		quality_val = 0;

		reserve_ll = 0;
	}

	char level;
	char quality;
	short reserve_sh;
	int quality_val;		// 品质进化值

	long long reserve_ll;
};

struct PastureSpiritParam
{
	PastureSpiritParam() { this->Reset(); }

	void Reset()
	{
		for (int index = 0; index < PASTURE_SPIRIT_MAX_COUNT; ++ index)
		{
			pasture_spirit_item_list[index].Reset();
		}

		last_free_chou_timestamp = 0;
	}

	PastureSpiritItem pasture_spirit_item_list[PASTURE_SPIRIT_MAX_COUNT];

	unsigned int last_free_chou_timestamp;
};

typedef char PastureSpiritParamHex[sizeof(PastureSpiritParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(PastureSpiritParamHex) < 512);

#pragma pack(pop)

#endif