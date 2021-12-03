#ifndef __SHEN_YIN_DEF_HPP__
#define __SHEN_YIN_DEF_HPP__

#include "servercommon.h"
#include "virtualbaggridparamdef.hpp"

static const int SHEN_YIN_TABEL_MIDDLE_ROW = 4;									// 中间的那一行的行数
static const int SHEN_YIN_TABEL_ROW_COUNT = 9;									// 行 y 最大行数
static const int SHEN_YIN_TABEL_MIDDLE_GRIDS = 9;								// 列 x 最大列数
static const int SHEN_YIN_PASTURE_SPIRIT_MAX_IMPRINT_EXTRA_ATTR_COUNT = 5;		// 印记附加属性最大条数
static const int SHEN_YIN_PASTURE_SPIRIT_MAX_IMPRINT_SLOT = 10;					// 最大印位数
static const int SHEN_YIN_PASTURE_SPIRIT_MAX_GRID_COUNT = 100;					// 背包格子数
static const int SHEN_YIN_PASTURE_SPIRIT_MAX_SHOP_ITEM_COUNT = 14;				// 商店格子数
static const int SHEN_YIN_LIEHUN_POOL_MAX_COUNT = 20;							// 猎魂池容量最大数
//static const int SHEN_YIN_CHOU_HUN_MAX_NUM = 7;								// 现在用不上了，ShenYinLieMingChouhunConfig的rate_list改成用枚举的max值
static const int SHEN_YIN_MAX_NUM = 100;										// 抽魂物品最大数
static const int SHEN_YIN_XILIAN_BAODI_MAX_COUNT = 9;
static const int SHEN_YIN_MAX_ATTR_NUM = 7;

enum SHEN_YIN_BEAD_TYPE
{
	SHEN_YIN_BEAD_TYPE_INVALID = 0,		//无效
	SHEN_YIN_BEAD_TYPE_FIRE,			//火灵果
	SHEN_YIN_BEAD_TYPE_METALS,			//金灵果
	SHEN_YIN_BEAD_TYPE_WATER,			//水灵果
	SHEN_YIN_BEAD_TYPE_WOOD,			//木灵果
	SHEN_YIN_BEAD_TYPE_THUNDER,			//雷灵果

	SHEN_YIN_BEAD_TYPE_MAX,				//灵果类型最大
	SHEN_YIN_BEAD_TYPE_SAME = SHEN_YIN_BEAD_TYPE_MAX,  //同种灵果即可
	SHEN_YIN_BEAD_TYPE_ALL,				// 任意灵果即可
};

enum SHEN_YIN_PASTURE_SPIRIT_MAX_IMPRINT_SLOT_TYPE
{
	YIN,
	EARTH,
	WIND,
	WATER,
	MOUNTAINS,
	YANG,
	SKY,
	LIGHTNING,
	FIRE,
	POOL,

	COUNT,
};
UNSTD_STATIC_CHECK(SHEN_YIN_PASTURE_SPIRIT_MAX_IMPRINT_SLOT == COUNT);

enum SHEN_YIN_LIEMING_CHOUHUN_COLOR
{
	SHEN_YIN_LIEMING_CHOUHUN_COLOR_INVALID = -1,

	SHEN_YIN_LIEMING_CHOUHUN_COLOR_GREEN,
	SHEN_YIN_LIEMING_CHOUHUN_COLOR_BLUE,
	SHEN_YIN_LIEMING_CHOUHUN_COLOR_PURPLE,
	SHEN_YIN_LIEMING_CHOUHUN_COLOR_ORANGE,
	SHEN_YIN_LIEMING_CHOUHUN_COLOR_RED,
	SHEN_YIN_LIEMING_CHOUHUN_COLOR_PINK,
	SHEN_YIN_LIEMING_CHOUHUN_COLOR_QIANGHUA,
	SHEN_YIN_LIEMING_CHOUHUN_COLOR_XILIAN_FIRST,
	SHEN_YIN_LIEMING_CHOUHUN_COLOR_XILIAN_SECOND,

	SHEN_YIN_LIEMING_CHOUHUN_COLOR_MAX,
};

#pragma pack(push, 4)

struct ShenYinTableGridParam
{
	ShenYinTableGridParam() { this->Reset(); }
	void Reset()
	{
		type = 0;
		src_y = -1;
		src_x = -1;
		seq = -1;
	}

	void ResetCobine()
	{
		src_y = -1;
		src_x = -1;
		seq = -1;
	}

	bool IsEmpty() const { return 0 == type; }	//是否为空

	//是否已使用
	bool IsUse() const
	{
		return src_x >= 0 && src_x < SHEN_YIN_TABEL_MIDDLE_GRIDS && src_y >= 0 && src_y < SHEN_YIN_TABEL_ROW_COUNT;
	}	

	char type;
	char src_y;
	char src_x;
	char seq;
};

struct ShenYinPastureSpiritGridParam : VirtualBagGridParam
{
	struct AttrItem
	{
		AttrItem()
		{
			this->Reset();
		}

		void Reset()
		{
			memset(value_list, -1, sizeof(value_list));
			memset(type_list, -1, sizeof(type_list));
		}

		int value_list[SHEN_YIN_PASTURE_SPIRIT_MAX_IMPRINT_EXTRA_ATTR_COUNT];
		int type_list[SHEN_YIN_PASTURE_SPIRIT_MAX_IMPRINT_EXTRA_ATTR_COUNT];
	};

	ShenYinPastureSpiritGridParam()
	{
		this->Reset();
	}

	void Reset()
	{
		VirtualBagGridParam::Reset();

		attr_param.Reset();
		new_attr_param.Reset();
		grade = 0;
		qianghua_count = 0;
	}

	AttrItem attr_param;
	AttrItem new_attr_param;
	int grade;
	int qianghua_count;
};

struct ShenYinParam
{
	ShenYinParam() { this->Reset(); }
	
	void Reset()
	{
		for (int x = 0; x < SHEN_YIN_TABEL_MIDDLE_GRIDS; ++x)
		{
			for (int y = 0; y < SHEN_YIN_TABEL_ROW_COUNT; ++y)
			{
				table_lsit[x][y].Reset();
			}
		}

		imprint_score = 0;
		for (int index = 0; index < SHEN_YIN_PASTURE_SPIRIT_MAX_IMPRINT_SLOT; ++index)
		{
			imprint_slot_list[index].Reset();
		}

// 		for (int index1 = 0; index1 < SHEN_YIN_PASTURE_SPIRIT_MAX_GRID_COUNT; ++index1)
// 		{
// 			bag_grid_list[index1].Reset();
// 		}

		memset(shop_buy_count_list, 0, sizeof(shop_buy_count_list));
		reserve_sh = 0;

		chouhun_color = 0;
		memset(reserve_ch, 0, sizeof(reserve_ch));
		memset(chouhun_pool, 0, sizeof(chouhun_pool));

		shen_yin_chouhun_score = 0;

		memset(shop_buy_count_flush_timestamp_list, 0, sizeof(shop_buy_count_flush_timestamp_list));
	}

	ShenYinTableGridParam table_lsit[SHEN_YIN_TABEL_MIDDLE_GRIDS][SHEN_YIN_TABEL_ROW_COUNT];
	int imprint_score;
	ShenYinPastureSpiritGridParam imprint_slot_list[SHEN_YIN_PASTURE_SPIRIT_MAX_IMPRINT_SLOT];
	char shop_buy_count_list[SHEN_YIN_PASTURE_SPIRIT_MAX_SHOP_ITEM_COUNT];
	short reserve_sh;
	UNSTD_STATIC_CHECK(SHEN_YIN_PASTURE_SPIRIT_MAX_SHOP_ITEM_COUNT == 14)
	char chouhun_color;
	char reserve_ch[3];
	short chouhun_pool[SHEN_YIN_LIEHUN_POOL_MAX_COUNT];
	UNSTD_STATIC_CHECK(20 == SHEN_YIN_LIEHUN_POOL_MAX_COUNT);
	int shen_yin_chouhun_score;
	unsigned int shop_buy_count_flush_timestamp_list[SHEN_YIN_PASTURE_SPIRIT_MAX_SHOP_ITEM_COUNT];
	//ShenYinPastureSpiritGridParam bag_grid_list[SHEN_YIN_PASTURE_SPIRIT_MAX_GRID_COUNT];
};

typedef char ShenYinParamHex[sizeof(ShenYinParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(ShenYinParamHex) < 5120);

#pragma pack(pop)
#endif
