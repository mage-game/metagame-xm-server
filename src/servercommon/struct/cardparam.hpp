#ifndef __CARD_PARAM_HPP__
#define __CARD_PARAM_HPP__

#include "servercommon/servercommon.h"

enum CARD_COLOR
{
	CARD_COLOR_INVALID = 0,
	CARD_COLOR_GREEN,
	CARD_COLOR_BLUE,
	CARD_COLOR_YELLOW,
	CARD_COLOR_ORANGE,
	CARD_COLOR_RED,

	CARD_COLOR_MAX,
};

const static int CARD_MAX_COUNT = 16; 
const static int SLOT_PER_CARD = 4;

const static int MAX_CARD_LEVEL = 10;

const static int CARD_UPLEVEL_STUFF_MAX_COUNT = 50;

#pragma pack(push) 
#pragma pack(4)

struct CardItem
{
	CardItem() { this->Reset(); }

	void Reset()
	{
		exp = 0;
		level = 0;
		reserve = 0;
		memset(slot_color, 0, sizeof(slot_color));
	}

	int MinColor()
	{
		char min_color = CARD_COLOR_MAX - 1;

		for (int i = 0; i < SLOT_PER_CARD; ++ i)
		{
			if (slot_color[i] < min_color)
			{
				min_color = slot_color[i];
			}
		}

		return min_color;
	}

	int MaxColor()
	{
		char max_color = CARD_COLOR_INVALID;

		for (int i = 0; i < SLOT_PER_CARD; ++ i)
		{
			if (slot_color[i] > max_color)
			{
				max_color = slot_color[i];
			}
		}

		return max_color;
	}

	UNSTD_STATIC_CHECK(4 == SLOT_PER_CARD);

	char slot_color[SLOT_PER_CARD];
	int exp;
	short level;
	short reserve;
};

struct CardParam
{
	CardParam() { this->Reset(); }

	void Reset()
	{
		reserve_ll = 0;
		reserve_ll2 = 0;

		for (int i = 0; i < CARD_MAX_COUNT; ++ i)
		{
			item_list[i].Reset();
		}
	}

	long long reserve_ll;
	long long reserve_ll2;
	CardItem item_list[CARD_MAX_COUNT];
};

#pragma pack(pop)

typedef char CardParamHex[sizeof(CardParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(CardParamHex) < 1024)


#endif // __CARD_PARAM_HPP__

