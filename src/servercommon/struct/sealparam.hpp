#ifndef __SEAL_PARAM_HPP__
#define __SEAL_PARAM_HPP__

#include "servercommon/servercommon.h"

static const int SEAL_BACKPACK_MAX_GRIDS = 200;	// 虚拟背包格子数
static const int SEAL_SLOT_MAX_NUM = 10;		// 圣印槽最大数
static const int SEAL_ORDER_MAX_NUM = 9;		// 圣印最大阶数
static const int SEAL_SOUL_MIN_TYPE = 1;		// 最小圣魂种类
static const int SEAL_SOUL_MAX_TYPE = 3;		// 最大圣魂种类
static const int SEAL_ATTR_MAX_NUM = 18;		// 圣印属性最大数
static const int SEAL_SUIT_MIN_TYPE = 1;		// 圣印套装最小类型
static const int SEAL_SUIT_MAX_TYPE = 18;		// 圣印套装最大类型

enum SEAL_COLOR_KIND
{
	RUNE_COLOR_KIND_GREEN = 0,					// 绿色
	RUNE_COLOR_KIND_BLUE,						// 蓝色
	RUNE_COLOR_KIND_PURPLE,						// 紫色
	RUNE_COLOR_KIND_ORANGE,						// 橙色
	RUNE_COLOR_KIND_RED,						// 红色
	RUNE_COLOR_KIND_PINK,						// 粉色

	RUNE_COLOR_KIND_MAX,
};

static const int SEAL_COLOR_MAX_TYPE = RUNE_COLOR_KIND_MAX;		// 圣印最大颜色

enum SEAL_SUIT_SAME_ORDER_TYPE // 套装相同阶数量划分
{
	SEAL_SUIT_SAME_ORDER_TYPE_ONE = 1,
	SEAL_SUIT_SAME_ORDER_TYPE_TWO = 2,
	SEAL_SUIT_SAME_ORDER_TYPE_THREE = 3,
	SEAL_SUIT_SAME_ORDER_TYPE_FOUR = 4,
	SEAL_SUIT_SAME_ORDER_TYPE_SIX = 6,
	SEAL_SUIT_SAME_ORDER_TYPE_EIGHT = 8,
	SEAL_SUIT_SAME_ORDER_TYPE_TEN = 10,
};

enum SEAL_TYPE
{
	SEAL_TYPE_JINGHUA = 0,														//精华
	SEAL_TYPE_WUQI = 1,															//武器
	SEAL_TYPE_XIEZI = 2,														//鞋子
	SEAL_TYPE_HUSHOU = 3,														//护手
	SEAL_TYPE_KUZI = 4,															//裤子
	SEAL_TYPE_YIFU = 5,															//衣服
	SEAL_TYPE_TOUKUI = 6,														//头盔
	SEAL_TYPE_JIEZHI = 7,														//戒指
	SEAL_TYPE_XIANGLIAN = 8,													//项链
	SEAL_TYPE_HUFU = 9,															//护符
	SEAL_TYPE_HEXIN = 10,														//核心

	SEAL_TYPE_MAX,																//最大符文类型
};

#pragma pack(push, 4)

struct SealItem
{
	void Reset()
	{
		slot_index = -1;
		order = -1;
		color = -1;
		level = -1;
		item_id = 0;
		is_bind = 0;
		reserve_ch = 0;
	}

	short slot_index;		// 圣印孔索引
	short order;			// 圣印阶数
	short color;			// 圣印颜色
	short level;			// 圣印等级
	unsigned short item_id;	// 圣印id
	unsigned char is_bind;
	unsigned char reserve_ch;

	bool IsEmpty() const { return slot_index < 0 || order < 0 || color < 0 || level < 0 || item_id <= 0; }
};

struct SealParam
{
	SealParam() { this->Reset(); }

	void Reset()
	{
		for (int i = 0; i < SEAL_SLOT_MAX_NUM; ++i)
		{
			slot_seal_list[i].Reset();
		}

// 		for (int i = 0; i < SEAL_BACKPACK_MAX_GRIDS; ++i)
// 		{
// 			backpack_seal_list[i].Reset();
// 		}

		hun_score = 0;
		slot_open_flag = 0;
		memset(soul_list, 0, sizeof(soul_list));
	}

	bool IsActivateSeal(int slot_index)
	{
		return 0 != (slot_open_flag & (1 << slot_index));
	}

	void ActivateSeal(int slot_index)
	{
		slot_open_flag |= (1 << slot_index);
	}

	UNSTD_STATIC_CHECK(10 == SEAL_SLOT_MAX_NUM);
	UNSTD_STATIC_CHECK(200 == SEAL_BACKPACK_MAX_GRIDS);
	UNSTD_STATIC_CHECK(3 == SEAL_SOUL_MAX_TYPE);

	int hun_score;															// 灵魂数
	int soul_list[SEAL_SOUL_MAX_TYPE];										// 圣魂列表

	int slot_open_flag;														// 圣印孔开启标志
	SealItem slot_seal_list[SEAL_SLOT_MAX_NUM];								// 孔内圣印列表
	//SealItem backpack_seal_list[SEAL_BACKPACK_MAX_GRIDS];					// 背包圣印列表
};

#pragma pack(pop)

typedef char SealParamHex[sizeof(SealParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(SealParamHex) < 512);

#endif