#ifndef __LIEMING_DEF_HPP__
#define __LIEMING_DEF_HPP__

#include "servercommon.h"

#pragma pack(push, 4)

const static int LIEMING_FUHUN_SLOT_COUNT = 8;						// 附魂槽数量
const static int LIEMING_HUNSHOU_BAG_GRID_MAX_COUNT = 36;			// 魂兽背包格子最大数
const static int LIEMING_LIEHUN_POOL_MAX_COUNT = 18;				// 猎魂池
const static int LIEMING_HUNSHOU_TYPE_MAX_COUNT = 8;				// 魂兽类型数
const static int LIEMING_HUNSHOU_MAX_LEVEL = 300;					// 魂兽等级
const static int LIEMING_HUNSHOU_MAX_ID = 128;						// 魂兽最大ID
const static int LIEMING_HUNSHOU_EXP_ID = 30000;					// 经验魂兽ID
const static int LIEMING_HUNSHOU_PER_COLOR_MAX_COUNT = 64;			// 每种颜色的魂兽最大数量


enum LIEMING_HUNSHOU_COLOR
{
	LIEMING_HUNSHOU_COLOR_INVALID = 0,
	LIEMING_HUNSHOU_COLOR_GREEN,
	LIEMING_HUNSHOU_COLOR_BLUE,
	LIEMING_HUNSHOU_COLOR_PURPLE,
	LIEMING_HUNSHOU_COLOR_ORANGE,
	LIEMING_HUNSHOU_COLOR_RED,
	LIEMING_HUNSHOU_COLOR_PINK,
	LIEMING_HUNSHOU_COLOR_MAX,
};

enum LIEMING_HUNSHOU_TYPE
{
	LIEMING_HUNSHOU_TYPE_GONGJI = 0,
	LIEMING_HUNSHOU_TYPE_FANGYU,
	LIEMING_HUNSHOU_TYPE_QIXUE,
	LIEMING_HUNSHOU_TYPE_MINGZHONG,
	LIEMING_HUNSHOU_TYPE_SHANBI,
	LIEMING_HUNSHOU_TYPE_BAOJI,
	LIEMING_HUNSHOU_TYPE_JIANREN,
	LIEMING_HUNSHOU_TYPE_DIKANG_SHANGHAI,

	LIEMING_HUNSHOU_TYPE_MAX
};

enum LIEMING_CHOUHUN_COLOR
{
	LIEMING_CHOUHUN_COLOR_WHITE = 0,
	LIEMING_CHOUHUN_COLOR_GREEN,
	LIEMING_CHOUHUN_COLOR_BLUE,
	LIEMING_CHOUHUN_COLOR_PURPLE,
	LIEMING_CHOUHUN_COLOR_ROANGE,

	LIEMING_CHOUHUN_COLOR_MAX,
};

enum LIEMING_PLACE
{
	LIEMING_PLACE_INVALID = 0,
	LIEMING_PLACE_LIEHUN_POOL,
	LIEMING_PLACE_BAG,
	LIEMING_PLACE_SLOT,

	LIEMING_PLACE_MAX,
};

struct LieMingHunShou
{
	LieMingHunShou() { this->Reset(); }

	void Reset()
	{
		hunshou_id = 0;
		level = 0;
		curr_exp = 0;
	}

	bool IsValid() { return 0 != hunshou_id; }

	short hunshou_id;
	short level;
	long long curr_exp;
};

struct LieMingParam
{
	LieMingParam() { this->Reset(); }

	void Reset()
	{
		for (int i = 0; i < LIEMING_FUHUN_SLOT_COUNT; ++i)
		{
			slot_list[i].Reset();
		}

		solt_activity_flag = 0;
	}

	bool IsSoltActive(int attr_id)
	{
		return 0 != (solt_activity_flag & (1 << attr_id));
	}

	void ActiveSolt(int attr_id)
	{
		solt_activity_flag |= (1 << attr_id);
	}

	int solt_activity_flag;
	LieMingHunShou slot_list[LIEMING_FUHUN_SLOT_COUNT];
};

struct MingHun
{
	MingHun() { this->Reset(); }

	void Reset()
	{
		item_id = 0;
		is_bind = 0;
		reserve_sh = 0;
		level = 0;
		hunli = 0;
	}

	ItemID item_id;
	bool is_bind;
	short reserve_sh;
	short level;
	int hunli;
};

struct LieMingParam2
{
	LieMingParam2() { this->Reset(); }

	void Reset()
	{
		for (int i = 0; i < LIEMING_FUHUN_SLOT_COUNT; ++ i)
		{
			slot_list[i].Reset();
		}

		solt_activity_flag = 0;
		personal_minghun_exp = 0;
	}

	bool IsSoltActive(int attr_id) 
	{
		return 0 != (solt_activity_flag & (1 << attr_id)); 
	}

	void ActiveSolt(int attr_id) 
	{
		solt_activity_flag |= (1 << attr_id); 
	}

	int solt_activity_flag;
	int personal_minghun_exp;
	MingHun slot_list[LIEMING_FUHUN_SLOT_COUNT];
	
};

struct LieMingHunshouBagParam
{
	LieMingHunshouBagParam() { this->Reset(); }

	void Reset()
	{
		hunshou_exp = 0;
		chouhun_color = 0;
		reserve_ch = 0;
		reserve_sh = 0;
		reserve_ll = 0;
		reserve_int = 0;

		memset(chouhun_pool, 0, sizeof(chouhun_pool));

		for (int i = 0; i < LIEMING_HUNSHOU_BAG_GRID_MAX_COUNT; ++ i)
		{
			grid_list[i].Reset();
		}
	}

	long long hunshou_exp;
	char chouhun_color;
	char reserve_ch;
	short reserve_sh;
	long long reserve_ll;
	int reserve_int;

	UNSTD_STATIC_CHECK(18 == LIEMING_LIEHUN_POOL_MAX_COUNT);
	short chouhun_pool[LIEMING_LIEHUN_POOL_MAX_COUNT];

	LieMingHunShou grid_list[LIEMING_HUNSHOU_BAG_GRID_MAX_COUNT];
};

typedef char LieMingSlotParamHex[sizeof(LieMingParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(LieMingSlotParamHex) < 512);

typedef char LieMingHunshouBagParamHex[sizeof(LieMingHunshouBagParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(LieMingHunshouBagParamHex) < 1024);

typedef char LieMingParam2Hex[sizeof(LieMingParam2) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(LieMingParam2Hex) < 512);

#pragma pack(pop)

#endif

