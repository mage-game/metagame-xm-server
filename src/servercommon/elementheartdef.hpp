#ifndef __CHARM_GHOST_DEF_HPP__
#define __CHARM_GHOST_DEF_HPP__

#include "servercommon.h"
#include "servercommon/struct/itemlistparam.h"
#include "shenzhuangdef.hpp"

#pragma pack(push, 4)

static const int ELEMENT_HEART_MAX_COUNT = 5;				// 最大个数数据库
static const int ELEMENT_HEART_MAX_GRID_COUNT = 100;		// 最大虚拟背包格子数
static const int ELEMENT_HEART_MAX_CHOU_TIMES_ONE = 1;		// 抽奖1抽
static const int ELEMENT_HEART_MAX_CHOU_TIMES_TEN = 10;		// 抽奖10抽
static const int ELEMENT_HEART_MAX_CHOU_TIMES_MAX = ELEMENT_HEART_MAX_CHOU_TIMES_TEN * 2; // 最大奖品列表
UNSTD_STATIC_CHECK(ELEMENT_HEART_MAX_CHOU_TIMES_MAX == ELEMENT_HEART_MAX_CHOU_TIMES_TEN * 2)

static const int ELEMENT_HEART_EQUIP_MAX_COUNT = 6;			// 装备格子数
static const int ELEMENT_HEART_EQUIP_MAX_LEVEL = 100;		// 装备最大等级
static const int ELEMENT_SHOP_ITEM_COUNT = 10;				// 商店一次刷新出来的物品数量

static const int ELEMENT_HEART_MAX_XILIAN_SLOT = 8;			// 洗练槽数量
static const int ELEMENT_HEART_XILIAN_MAX_STAR_COUNT = 10;	// 元素洗练属性最大星数

enum ELEMENT_HEART_ITEM_TYPE
{
	ELEMENT_HEART_ITEM_TYPE_INVALID = 0,
	ELEMENT_HEART_ITEM_TYPE_FOOD,							// 食物
	ELEMENT_HEART_ITEM_TYPE_GHOST,							// 符鬼
	ELEMENT_HEART_ITEM_TYPE_CHARM,							// 符咒升级材料
	ELEMENT_HEART_ITEM_TYPE_EQUIPMENT,						// 装备

	ELEMENT_HEART_ITEM_TYPE_MAX
};

enum ELEMENT_HEART_WUXING_TYPE
{
	ELEMENT_HEART_WUXING_TYPE_JIN,							// 金
	ELEMENT_HEART_WUXING_TYPE_MU,							// 木
	ELEMENT_HEART_WUXING_TYPE_SHUI,							// 水
	ELEMENT_HEART_WUXING_TYPE_HUO,							// 火
	ELEMENT_HEART_WUXING_TYPE_TU,							// 土

	ELEMENT_HEART_WUXING_TYPE_MAX							// 五行最大
};
UNSTD_STATIC_CHECK(5 == ELEMENT_HEART_WUXING_TYPE_MAX)

enum ELEMENT_HEART_QUALITY_TYPE
{
	ELEMENT_HEART_QUALITY_TYPE_INVALID = 0,
	ELEMENT_HEART_QUALITY_TYPE_WHITE,
	ELEMENT_HEART_QUALITY_TYPE_GREEN,
	ELEMENT_HEART_QUALITY_TYPE_BLUE,
	ELEMENT_HEART_QUALITY_TYPE_PINK,
	ELEMENT_HEART_QUALITY_TYPE_PURPLE,
	ELEMENT_HEART_QUALITY_TYPE_GOLD,

	ELEMENT_HEART_QUALITY_TYPE_MAX
};

struct ElementEquipItem
{
	ElementEquipItem()
	{
		this->Reset();
	}

	void Reset()
	{
		real_level = 0;
		slot_flag = 0;
		upgrade_progress = 0;
		reserve_sh = 0;
	}

	short real_level;
	short slot_flag;						// 当前已激活的物品标记
	short upgrade_progress;					// 升级进度
	short reserve_sh;
};

struct ElementSingle
{
	ElementSingle() { this->Reset(); }

	void Reset()
	{
		grade = 0;
		wuxing_type = 0;
		id = 0;
		tartget_wuxing_type = 0;
		wuxing_bless = 0;
		bless = 0;
		next_product_timestamp = 0;
		memset(wuxing_food_feed_times_list, 0, sizeof(wuxing_food_feed_times_list));

		equip_param.Reset();
	}

	char grade;														// 阶级
	char wuxing_type;												// 五行类型
	char id;														// id
	char tartget_wuxing_type;										// 即将转换的五行类型
	int wuxing_bless;												// 五行祝福值
	int bless;														// 祝福值
	unsigned int next_product_timestamp;							// 下次产出时间
	int wuxing_food_feed_times_list[ELEMENT_HEART_WUXING_TYPE_MAX];	// 记录每个食物喂养次数

	ElementEquipItem equip_param;									// 装备
};

struct ElementTextureParam
{
	ElementTextureParam() { this->Reset(); }

	bool IsInvalid() { return ELEMENT_HEART_WUXING_TYPE_JIN > wuxing_type; }
	void Reset()
	{
		wuxing_type = 0;
		grade = 0;
		reserve_sh = 0;
		exp = 0;
	}

	char wuxing_type;
	char grade;
	short reserve_sh;
	int exp;
};

struct ElementHeartGridParam
{
	ElementHeartGridParam() { this->Reset(); }

	bool IsEmpty() { return 0 == v_item_id; }
	void Reset()
	{
		v_item_id = 0;
		num = 0;
		is_bind = 0;
		reserve_ch = 0;
		reserve_sh = 0;
	}

	short v_item_id;
	short num;
	char is_bind;
	char reserve_ch;
	short reserve_sh;
};

struct ElementHeartShopItem
{
	ElementHeartShopItem() { this->Reset(); }

	void Reset()
	{
		shop_seq = -1;
		need_gold_buy = 0;
		has_buy = 0;
	}

	short shop_seq;						// 商店配置seq
	char need_gold_buy;					// 是否需要元宝购买
	char has_buy;						// 是否已经购买过
};

struct ElementHeartXiLianItem
{
	ElementHeartXiLianItem()
	{
		this->Reset();
	}

	void Reset()
	{
		xilian_val = 0;
		element_attr_type = 0;
		reserve_ch = 0;
		reserve_sh = 0;
	}

	int xilian_val;
	char element_attr_type;
	char reserve_ch;
	short reserve_sh;
};

struct ElementHeartXiLianParam
{
	ElementHeartXiLianParam()
	{
		this->Reset();
	}

	void Reset()
	{
		open_slot_flag = 0;

		for (int i = 0; i < ELEMENT_HEART_MAX_XILIAN_SLOT; i++)
		{
			slot_list[i].Reset();
		}
	}

	bool IsSlotOpen(int slot_id) const { return 0 != (open_slot_flag & (1 << slot_id)); }

	int open_slot_flag;
	ElementHeartXiLianItem slot_list[ELEMENT_HEART_MAX_XILIAN_SLOT];
};

struct ElementHeartParam
{
	ElementHeartParam() { this->Reset(); }

	void Reset()
	{
		one_chou_free_times = 0;
		reserve_ch = 0;
		day_shop_flush_times = 0;
		next_fresh_shop_timestamp = 0;

		for (int i = 0; i < ELEMENT_HEART_MAX_COUNT; ++i)
		{
			element_heart_list[i].Reset();
		}

		for (int i = 0; i < SHENZHUANG_MAX_PART; ++i)
		{
			element_texture_list[i].Reset();
		}

		for (int i = 0; i < ELEMENT_SHOP_ITEM_COUNT; ++i)
		{
			shop_item_list[i].Reset();
		}

		for (int i = 0; i < ELEMENT_HEART_MAX_COUNT; ++i)
		{
			element_xilian_list[i].Reset();
		}
	}

	char one_chou_free_times;
	char reserve_ch;
	short day_shop_flush_times;
	unsigned int next_fresh_shop_timestamp;

	ElementSingle element_heart_list[ELEMENT_HEART_MAX_COUNT];
	ElementTextureParam element_texture_list[SHENZHUANG_MAX_PART];
	ElementHeartShopItem shop_item_list[ELEMENT_SHOP_ITEM_COUNT];
	ElementHeartXiLianParam element_xilian_list[ELEMENT_HEART_MAX_COUNT];
};

typedef char ElementHeartParamHex[sizeof(ElementHeartParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(ElementHeartParamHex) < 2048);

#pragma pack(pop)

#endif
