#ifndef _ZHUANZHI_EQUIP_DEF_HPP_
#define _ZHUANZHI_EQUIP_DEF_HPP_

#include "servercommon.h"
#include "servercommon/struct/itemlistparam.h"

#pragma pack(push, ALIGN_VALUE)

static const int MAX_ZHUANZHI_FULING_ATTR_COUNT = 4;			// 附灵属性最大数量

static const int MAX_ZHUANZHI_STONE_SLOT = 6;					// 每个部位最大孔位
static const int MAX_ZHUANZHI_STONE_LEVEL = 11;					// 玉石最大等级
static const int MAX_REFINE_STUFF_COUNT = 3;					// 玉石精炼材料最大数量

static const int SUIT_ROLE_HAS_MAX_SUIT_NUM = 4;				// 角色可以有的最多的套装

static const int EQUIP_ZHIZUN_COMPOSE_CONSUME_NUM = 2;			//至尊装备合成消耗的装备数
static const int MAX_ZHUANZHI_EQUIP_AWAKENING_COUNT = 3;		//每件转职装备的觉醒数

static const int MAX_ZHUANZHI_EQUIP_COMPOSE_COUNT = 5;			//转职装备合成所需子装备最大数量

typedef unsigned short wake_type_t;
typedef unsigned short wake_level_t;
typedef unsigned int wake_weight_t;
typedef int wake_key_t;

//装备-------------------------------------------------------------------------------------------

enum E_INDEX_ZHUANZHI
{
	E_INDEX_WUQI = 0,											// 武器
	E_INDEX_YIFU,												// 铠甲
	E_INDEX_HUSHOU,												// 护手
	E_INDEX_YAODAI,												// 腰带
	E_INDEX_TOUKUI,												// 头盔
	E_INDEX_XIANGLIAN,											// 项链
	E_INDEX_SHOUZHUO,											// 手镯
	E_INDEX_JIEZHI,												// 戒指
	E_INDEX_XIEZI,												// 鞋子
	E_INDEX_YUPEI,												// 玉佩

	E_INDEX_MAX,
};

//计算储存觉醒锁需要的字节数，按4对齐
static const int WAKE_BYTE = BIT_COUNT_OF_BYTE * ALIGN_VALUE;
static const int MAX_ZHUANZHI_EQUIP_AWAKENING_LOCK_COUNT = MAX_ZHUANZHI_EQUIP_AWAKENING_COUNT * E_INDEX_MAX;
static const int ALIGN_ZHUANZHI_EQUIP_AWAKENING_LOCK_COUNT = (MAX_ZHUANZHI_EQUIP_AWAKENING_LOCK_COUNT % WAKE_BYTE == 0) ? MAX_ZHUANZHI_EQUIP_AWAKENING_LOCK_COUNT : MAX_ZHUANZHI_EQUIP_AWAKENING_LOCK_COUNT + (WAKE_BYTE - MAX_ZHUANZHI_EQUIP_AWAKENING_LOCK_COUNT % WAKE_BYTE);
static const int MAX_ZHUANZHI_EQUIP_AWAKENING_LOCK_BYTE = ALIGN_ZHUANZHI_EQUIP_AWAKENING_LOCK_COUNT / BIT_COUNT_OF_BYTE;

enum EQUIP_SPECIAL_EFFECT_TYPE
{
	EQUIP_SPECIAL_EFFECT_TYPE_INVALID = 0,
	EQUIP_SPECIAL_EFFECT_TYPE_1,						// 增加攻击力的p1万分比
	EQUIP_SPECIAL_EFFECT_TYPE_2,						// 减少受到伤害的p1万分比

	EQUIP_SPECIAL_EFFECT_TYPE_MAX,
};

struct ZhuanzhiEquipItem
{
	ZhuanzhiEquipItem() { this->Reset(); }

	void Reset()
	{
		star_level = 0;
		reserve_sh = 0;
		star_exp = 0;
		zhuanzhi_equip_data.Reset();
		memset(fuling_count_list, 0, sizeof(fuling_count_list));
	}

	UInt16 star_level;											// 星星等级
	Int16 reserve_sh;
	UInt32 star_exp;											// 星星经验
	ItemDataWrapper zhuanzhi_equip_data;						// 装备数据
	UInt16 fuling_count_list[MAX_ZHUANZHI_FULING_ATTR_COUNT];	// 附灵次数数据
};

//玉石-------------------------------------------------------------------------------------------

enum ZHUANZHI_STONE_TYPE
{
	ZHUANZHI_STONE_TYPE_RED = 0,								// 红玉石  
	ZHUANZHI_STONE_TYPE_BLUE,									// 蓝玉石

	ZHUANZHI_STONE_TYPE_MAX,
};

enum ZHUANZHI_STONE_SLOT_OPEN_TYPE
{
	ZHUANZHI_STONE_SLOT_OPEN_TYPE_ORDER = 0,							// 装备阶数 
	ZHUANZHI_STONE_SLOT_OPEN_TYPE_VIP,									// vip等级

	ZHUANZHI_STONE_SLOT_OPEN_TYPE_MAX,
};

struct ZhuanzhiStoneItem
{
	ZhuanzhiStoneItem() : stone_id(0), is_bind(0), reserve_ch(0), reserve1(0), reserve2(0) {}

	ItemID stone_id;
	UInt8 is_bind;
	UInt8 reserve_ch;
	Int16 reserve1;
	Int16 reserve2;

	void Reset()
	{
		stone_id = 0;
		is_bind = 0;
		reserve_ch = 0;
		reserve1 = 0;
		reserve2 = 0;
	}

	bool Invalid()
	{
		return stone_id <= 0;
	}
};

struct ZhuanzhiStonesPartData
{
	ZhuanzhiStonesPartData() { this->Reset(); }

	void Reset()
	{
		slot_open_flag = 0;
		reserve_ch = 0;
		refine_level = 0;
		refine_val = 0;

		for (int i = 0; i < MAX_ZHUANZHI_STONE_SLOT; i ++)
		{
			slot_list[i].Reset();
		}
	}

	UInt8 slot_open_flag;									// 开孔标记
	UInt8 reserve_ch;
	UInt16 refine_level;									// 精炼等级
	UInt32 refine_val;										// 精炼值

	ZhuanzhiStoneItem slot_list[MAX_ZHUANZHI_STONE_SLOT];	// 每个部分宝石信息
};

//套装-------------------------------------------------------------------------------------------

enum ZHUANZHI_SUIT_TYPE
{
	ZHUANZHI_SUIT_TYPE_INVALID = -1,
	ZHUANZHI_SUIT_TYPE_ZHUXIAN = 0,	// 诛仙套装
	ZHUANZHI_SUIT_TYPE_ZHUSHEN,		// 诛神套装

	ZHUANZHI_SUIT_TYPE_MAX,
};

enum ZHUANZHI_SUIT_GROUP_TYPE
{
	ZHUANZHI_SUIT_GROUP_TYPE_FANG = 0,	// 防具组合
	ZHUANZHI_SUIT_GROUP_TYPE_GONG,		// 攻击组合
	ZHUANZHI_SUIT_GROUP_TYPE_ZHIZUN,	// 至尊组合

	ZHUANZHI_SUIT_GROUP_TYPE_MAX,
};

struct OneSuitInfo
{
	OneSuitInfo() { this->Reset(); }

	struct stuff_info {
		ItemID item_id;
		short count;
	};
	void Reset()
	{
		memset(order_list, 0, sizeof(order_list));
		memset(stuff_bind_list, 0, sizeof(stuff_bind_list));
		memset(stuff_unbind_list, 0, sizeof(stuff_unbind_list));
		reserve_sh = 0;
	}

	UInt8 order_list[E_INDEX_MAX];	// 装备部位的阶数
	UInt16 reserve_sh;
	stuff_info stuff_bind_list[E_INDEX_MAX][ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
	stuff_info stuff_unbind_list[E_INDEX_MAX][ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
};

//转职装备觉醒
struct ZhuanzhiEquipAwakening
{
	ZhuanzhiEquipAwakening()
	{
		//this->Reset();	//都是结构体,构造不需要Reset
	}	

	void Reset()
	{
		memset(awakening_in_equip, 0, sizeof(awakening_in_equip));
		memset(awakening_in_displacement, 0, sizeof(awakening_in_displacement));
	}
	struct Awakening
	{
		Awakening() { this->Reset(); }

		void Reset()
		{
			type = 0;
			level = 0;
		}
		wake_type_t type;
		wake_level_t level;
	};
	Awakening awakening_in_equip[MAX_ZHUANZHI_EQUIP_AWAKENING_COUNT];				//装备位
	Awakening awakening_in_displacement[MAX_ZHUANZHI_EQUIP_AWAKENING_COUNT];		//替换位
};

//至尊-------------------------------------------------------------------------------------------
enum ZHIZUNEQUIP_PART_TYPE
{
	ZHIZUNEQUIP_PART_TYPE_0,
	ZHIZUNEQUIP_PART_TYPE_1,

	ZHIZUNEQUIP_PART_TYPE_MAX,
};

struct ZhuanzhiEquipParam
{
	ZhuanzhiEquipParam() { this->Reset(); }

	void Reset()
	{

		capability = 0;
		stone_score = 0;

		for (int i = 0; i < E_INDEX_MAX; i++)
		{
			zhuanzhi_equip_list[i].Reset();
			stone_list[i].Reset();
			zhuanzhi_equip_awakening_list[i].Reset();
		}

		for (int i = 0; i < SUIT_ROLE_HAS_MAX_SUIT_NUM; i ++)
		{
			zhuanzhi_suit_order_list[i].Reset();
		}

		memset(zhuanzhi_equip_awakening_lock_flag, 0, sizeof(zhuanzhi_equip_awakening_lock_flag));
	}

	UInt32 capability;															// 战力值
	UInt32 stone_score;															// 玉石精粹

	ZhuanzhiEquipItem zhuanzhi_equip_list[E_INDEX_MAX];							// 装备数据
	ZhuanzhiStonesPartData stone_list[E_INDEX_MAX]; 							// 玉石数据
	OneSuitInfo zhuanzhi_suit_order_list[SUIT_ROLE_HAS_MAX_SUIT_NUM];			// 套装
	ZhuanzhiEquipAwakening zhuanzhi_equip_awakening_list[E_INDEX_MAX];			// 觉醒
	unsigned char zhuanzhi_equip_awakening_lock_flag[MAX_ZHUANZHI_EQUIP_AWAKENING_LOCK_BYTE];	//锁定标志位
};

typedef char ZhuanzhiEquipParamHex[sizeof(ZhuanzhiEquipParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(ZhuanzhiEquipParamHex) < 8196);

#pragma pack(pop)

#endif

