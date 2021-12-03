#ifndef __SHENSHOU_DEF_HPP__
#define __SHENSHOU_DEF_HPP__

#include "servercommon.h"

static const int SHENSHOU_MAX_ID = 32;												// 神兽最大ID
static const int SHENSHOU_MAX_EQUIP_SLOT_INDEX = 4;									// 神兽装备最大部位index
static const int SHENSHOU_MAX_EQUIP_ATTR_COUNT = 3;									// 神兽装备最大随机属性个数
static const int SHENSHOU_MAX_BACKPACK_COUNT = 200;									// 神兽背包容量个数
static const int SHENSHOU_MAX_DRAW_TIMES = 14;										// 唤灵最大抽奖次数
static const int SHENSHOU_MAX_RERFESH_ITEM_COUNT = 14;								// 唤灵刷新一次物品数量

enum SHENSHOU_ATTR_TYPE																// 装备属性属性种类
{
	SHENSHOU_ATTR_TYPE_INVALID_MIN = 0,

	SHENSHOU_ATTR_TYPE_PER_HP,														// 神兽生命万分比
	SHENSHOU_ATTR_TYPE_PER_GONGJI,													// 神兽攻击万分比
	SHENSHOU_ATTR_TYPE_PER_FANGYU,													// 神兽物防万分比
	SHENSHOU_ATTR_TYPE_PER_SHANBI,													// 神兽闪避万分比
	SHENSHOU_ATTR_TYPE_PER_MINGZHONG,												// 神兽命中万分比
	SHENSHOU_ATTR_TYPE_PER_BAOJI,													// 神兽暴击万分比
	SHENSHOU_ATTR_TYPE_PER_JIANREN,													// 神兽抗暴万分比
	SHENSHOU_ATTR_TYPE_PER_GUSHANG,													// 神兽固伤万分比
	SHENSHOU_ATTR_TYPE_PER_JIANSHANG,												// 神兽减伤万分比
	SHENSHOU_ATTR_TYPE_MAX_HP,														// 生命固定值
	SHENSHOU_ATTR_TYPE_GONGJI,														// 攻击固定值
	SHENSHOU_ATTR_TYPE_FANGYU,														// 物防固定值
	SHENSHOU_ATTR_TYPE_SHANBI,														// 闪避固定值
	SHENSHOU_ATTR_TYPE_MINGZHONG,													// 命中固定值
	SHENSHOU_ATTR_TYPE_BAOJI,														// 暴击固定值
	SHENSHOU_ATTR_TYPE_JIANREN,														// 抗暴固定值
	SHENSHOU_ATTR_TYPE_GUSHANG,														// 固伤固定值											
	SHENSHOU_ATTR_TYPE_JIANSHANG,													// 减伤固定值

	SHENSHOU_ATTR_TYPE_INVALID_MAX,
};

enum SHENSHOU_SKILL_TYPE
{
	SHENSHOU_SKILL_TYPE_MIN_INVALID = 0,

	SHENSHOU_SKILL_TYPE_1,															// 铁甲防御
	SHENSHOU_SKILL_TYPE_2,															// 生命不息
	SHENSHOU_SKILL_TYPE_3,															// 残暴攻击
	SHENSHOU_SKILL_TYPE_4,															// 金刚领域
	SHENSHOU_SKILL_TYPE_5,															// 生命领域
	SHENSHOU_SKILL_TYPE_6,															// 攻击领域
	SHENSHOU_SKILL_TYPE_7,															// 灵魂觉醒
	SHENSHOU_SKILL_TYPE_8,															// 祖灵庇护
	SHENSHOU_SKILL_TYPE_9,															// 御甲临风
	SHENSHOU_SKILL_TYPE_10,															// 寸步难行
	SHENSHOU_SKILL_TYPE_11,															// 生命汲取
	SHENSHOU_SKILL_TYPE_12,															// 雷霆之怒
	SHENSHOU_SKILL_TYPE_13,															// 兽神附体
	SHENSHOU_SKILL_TYPE_14,															// 不动如山
	SHENSHOU_SKILL_TYPE_15,															// 无尽怒火
	SHENSHOU_SKILL_TYPE_16,															// 争分夺秒
	SHENSHOU_SKILL_TYPE_17,															// 力不从心
	SHENSHOU_SKILL_TYPE_18,															// 避敌锋芒
	SHENSHOU_SKILL_TYPE_19,															// 战无不胜
	SHENSHOU_SKILL_TYPE_20,															// 势不可挡
	SHENSHOU_SKILL_TYPE_21,															// 力破千军

	SHENSHOU_SKILL_TYPE_22,															// 全身攻击增加
	SHENSHOU_SKILL_TYPE_23,															// 全身生命增加
	SHENSHOU_SKILL_TYPE_24,															// 技能增伤增加
	SHENSHOU_SKILL_TYPE_25,															// 技能减伤增加
	SHENSHOU_SKILL_TYPE_26,															// 伤害加成增加
	SHENSHOU_SKILL_TYPE_27,															// 伤害减免增加

	SHENSHOU_SKILL_TYPE_28,															// 格挡几率
	SHENSHOU_SKILL_TYPE_29,															// 命中几率
	SHENSHOU_SKILL_TYPE_30,															// 伤害加成

	SHENSHOU_SKILL_TYPE_COUNT,
};

#pragma pack(push, 4)

struct ShenshouBackpackItem
{
	struct AttrItem
	{
		short attr_type;
		char is_start;
		char reserve_sh;
		int attr_value;
	};

	ShenshouBackpackItem() { this->Reset(); }

	void Reset()
	{
		item_id = 0;
		strength_level = 0;
		shuliandu = 0;
		memset(attr_list, 0, sizeof(attr_list));
	}

	short item_id;
	short strength_level;						// 强化等级
	int shuliandu;								// 熟练度

	AttrItem attr_list[SHENSHOU_MAX_EQUIP_ATTR_COUNT];
};

struct ShenshouInfo
{
	ShenshouInfo() { this->Reset(); }

	void Reset()
	{
		has_zhuzhan = 0;
		reserve_ch = 0;
		reserve_sh = 0;

		for (int index = 0; index <= SHENSHOU_MAX_EQUIP_SLOT_INDEX; ++ index)
		{
			equip_list[index].Reset();
		}
	}

	char has_zhuzhan;			// 是否助战了
	char reserve_ch;
	short reserve_sh;
	ShenshouBackpackItem equip_list[SHENSHOU_MAX_EQUIP_SLOT_INDEX + 1];
};

struct HuanlingListInfo
{
	HuanlingListInfo() : huanling_seq(0), draw(0), pool_index(0)
	{}

	short huanling_seq;
	char draw;
	char pool_index;
};

struct ShenshouParam
{
	ShenshouParam() { this->Reset(); }

	void Reset()
	{
		cur_huanling_draw_times = 0;
		extra_zhuzhan_count = 0;
		huanling_score = 0;
		refresh_huanling_times = 0;	
		first_huanling = 0;
		reserve_ch = 0;
		reserve_sh = 0;
		for (int i = 0; i <= SHENSHOU_MAX_ID; ++ i)
		{
			shenshou_list[i].Reset();
		}
// 		for (int i = 0; i < SHENSHOU_MAX_BACKPACK_COUNT; ++i)
// 		{
// 			backpack_item_list[i].Reset();
// 		}

		memset(huanling_list, 0, sizeof(huanling_list));
	}

	short extra_zhuzhan_count;				// 额外助战位（需要购买）
	short cur_huanling_draw_times;			// 当前唤灵抽到了第几次

	int huanling_score;						// 唤灵积分
	int refresh_huanling_times;				// 唤灵刷新次数

	char first_huanling;					// 是否为第一次唤灵
	char reserve_ch;
	short reserve_sh;

	ShenshouInfo shenshou_list[SHENSHOU_MAX_ID + 1];
	HuanlingListInfo huanling_list[SHENSHOU_MAX_RERFESH_ITEM_COUNT];
	//ShenshouBackpackItem backpack_item_list[SHENSHOU_MAX_BACKPACK_COUNT];
};

typedef char ShenshouParamHex[sizeof(ShenshouParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(ShenshouParamHex) < 15360);

#pragma pack(pop)
#endif
