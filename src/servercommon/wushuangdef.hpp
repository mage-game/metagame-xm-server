#ifndef __WUSHUANG_DEF_HPP__
#define __WUSHUANG_DEF_HPP__

#include "servercommon.h"

#pragma pack(push, 4)

const static int WUSHUANG_DAZAO_MAX_LEVEL = 10;				// 无双打造最大等级
const static int WUSHUANG_QUALITY_MAX_LEVEL = 10;			// 无双品质最大等级
const static int WUSHUANG_JINGLIAN_ATTR_COUNT = 3;			// 武装精炼属性数量
const static int WUSHUANG_FUMO_SLOT_COUNT = 3;				// 无双附魔槽数量
const static int WUSHUANG_FUHUN_SLOT_COUNT = 5;				// 无双附魂槽数量
const static int WUSHUANG_HUNSHOU_BAG_GRID_MAX_COUNT = 20;	// 魂兽背包格子最大数
const static int WUSHUANG_LIEHUN_POOL_MAX_COUNT = 18;		// 猎魂池
const static int HUNSHOU_TYPE_MAX_COUNT = 7;				// 魂兽类型数
const static int HUNSHOU_MAX_LEVEL = 20;					// 魂兽等级
const static int HUNSHOU_MAX_ID = 128;						// 魂兽最大ID
const static int HUNSHOU_EXP_ID = 30000;					// 经验魂兽ID
const static int HUNSHOU_PER_COLOR_MAX_COUNT = 64;			// 每种颜色的魂兽最大数量

enum WUSHUANG_EQUIP_TYPE
{
	WUSHUANG_EQUIP_TYPE_MIN = 0,
	WUSHUANG_EQUIP_TYPE_ZHANKUI = WUSHUANG_EQUIP_TYPE_MIN,	// 战盔
	WUSHUANG_EQUIP_TYPE_HUMUJING,							// 护目镜
	WUSHUANG_EQUIP_TYPE_ZHANPAO,							// 战袍
	WUSHUANG_EQUIP_TYPE_PIFENG,								// 披风
	WUSHUANG_EQUIP_TYPE_FABAO,								// 法宝
	WUSHUANG_EQUIP_TYPE_JIEZHI,								// 戒指
	WUSHUANG_EQUIP_TYPE_ZHANXUE,							// 战靴
	WUSHUANG_EQUIP_TYPE_HUSHOU,								// 护手

	WUSHUANG_EQUIP_TYPE_MAX_COUNT,							// 无双装备数量
};

enum HUNSHOU_COLOR
{
	HUNSHOU_COLOR_INVALID = 0,
	HUNSHOU_COLOR_GREEN,
	HUNSHOU_COLOR_BLUE,
	HUNSHOU_COLOR_PURPLE,
	HUNSHOU_COLOR_ORANGE,

	HUNSHOU_COLOR_MAX,
};

enum HUNSHOU_TYPE
{
	HUNSHOU_TYPE_GONGJI = 0,
	HUNSHOU_TYPE_FANGYU,
	HUNSHOU_TYPE_QIXUE,
	HUNSHOU_TYPE_MINGZHONG,
	HUNSHOU_TYPE_BAOJI,
	HUNSHOU_TYPE_SHANBI,
	HUNSHOU_TYPE_JIANREN,

	HUNSHOU_TYPE_MAX
};

enum CHOUHUN_COLOR
{
	CHOUHUN_COLOR_WHITE = 0,
	CHOUHUN_COLOR_GREEN,
	CHOUHUN_COLOR_BLUE,
	CHOUHUN_COLORPURPLE,
	CHOUHUN_COLOR_ROANGE,

	CHOUHUN_COLOR_MAX,
};

enum WUSHUANG_PLACE
{
	WUSHUANG_PLACE_INVALID = 0,
	WUSHUANG_PLACE_LIEHUN_POOL,
	WUSHUANG_PLACE_BAG,
	WUSHUANG_PLACE_EQUIP,

	WUSHUANG_PLACE_MAX,
};

struct WushuangHunShou
{
	WushuangHunShou() { this->Reset(); }

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

struct WushuangEquip
{
	WushuangEquip() { this->Reset(); }

	void Reset()
	{
		dazao_level = 0;
		quality_level = 0;
		dazao_exp = 0;
		memset(jinglian_attrs, 0, sizeof(jinglian_attrs));
		memset(fumo_items, 0, sizeof(fumo_items));
		reserve_sh = 0;

		for (int i = 0; i < WUSHUANG_FUHUN_SLOT_COUNT; ++ i)
		{
			hunshou_list[i].Reset();
		}
	}

	UNSTD_STATIC_CHECK(3 == WUSHUANG_FUMO_SLOT_COUNT);

	short dazao_level;
	short quality_level;
	int dazao_exp;
	int jinglian_attrs[WUSHUANG_JINGLIAN_ATTR_COUNT];
	ItemID fumo_items[WUSHUANG_FUMO_SLOT_COUNT];
	short reserve_sh;
	WushuangHunShou hunshou_list[WUSHUANG_FUHUN_SLOT_COUNT];
};

struct WushuangParam
{
	WushuangParam() { this->Reset(); }

	void Reset()
	{
		wushuang_fb_pass_level = 0;
		wushuang_fb_has_fetch_day_reward = 0;
		reserve_ch = 0;
		reserve_ll = 0;

		for (int i = 0; i < WUSHUANG_EQUIP_TYPE_MAX_COUNT; ++ i)
		{
			equip_list[i].Reset();
		}
	}

	void OnDayChange()
	{
		wushuang_fb_has_fetch_day_reward = 0;
	}

	short wushuang_fb_pass_level;
	char wushuang_fb_has_fetch_day_reward; 
	char reserve_ch;

	long long reserve_ll;

	WushuangEquip equip_list[WUSHUANG_EQUIP_TYPE_MAX_COUNT];
};

struct WushuangHunshouBagParam
{
	WushuangHunshouBagParam() { this->Reset(); }

	void Reset()
	{
		hunshou_exp = 0;
		chouhun_color = 0;
		reserve_ch = 0;
		reserve_sh = 0;
		reserve_ll = 0;

		memset(chouhun_pool, 0, sizeof(chouhun_pool));

		for (int i = 0; i < WUSHUANG_HUNSHOU_BAG_GRID_MAX_COUNT; ++ i)
		{
			grid_list[i].Reset();
		}
	}

	long long hunshou_exp;
	char chouhun_color;
	char reserve_ch;
	short reserve_sh;
	long long reserve_ll;

	UNSTD_STATIC_CHECK(18 == WUSHUANG_LIEHUN_POOL_MAX_COUNT);
	short chouhun_pool[WUSHUANG_LIEHUN_POOL_MAX_COUNT];

	WushuangHunShou grid_list[WUSHUANG_HUNSHOU_BAG_GRID_MAX_COUNT];
};

typedef char WushuangParamHex[sizeof(WushuangParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(WushuangParamHex) < 2048);

typedef char WushuangHunshouBagParamHex[sizeof(WushuangHunshouBagParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(WushuangHunshouBagParamHex) < 1024);

#pragma pack(pop)

#endif  // __WUSHUANG_DEF_HPP__

