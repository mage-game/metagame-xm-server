// 名将系统相关定义

#ifndef __GREATE_SOLDIER_DEF_HPP__
#define __GREATE_SOLDIER_DEF_HPP__

#include "servercommon.h"

static const int GREATE_SOLDIER_MAX_COUNT = 32;						// 名将最大数量
static const int GREATE_SOLDIER_PASSIVE_SKILL_MAX_COUNT = 30;		// 最多的被动技能数量
static const int GREATE_SOLDIER_WASH_RATE_MAX_COUNT = 4;			// 最多的洗练段数
static const int GREATE_SOLDIER_SLOT_MAX_COUNT = 5;					// 最多的将位槽数
static const int GREATE_SOLDIER_SLOT_LEVEL_MAX_COUNT = 320;			// 最多等级数
static const int GREATE_SOLDIER_COMBINE_MAX_COUNT = 50;				// 最多组合数
static const int GREATE_SOLDIER_EQUIPMENT_SLOT_MAX_COUNT = 4;		// 名将装备槽个数
static const int GREATE_SOLDIER_MAX_EQUIP_STRENGTH_LEVEL = 200;		// 名将装备最大强化等级
static const int GREATE_SOLDIER_SPECIAL_SKILL_MAX_LEVEL = 50;		// 名将特殊技能最大等级

// 特殊技能类型
enum GREATE_SOLDIER_SPECIAL_SKILL_TYPE
{
	GREATE_SOLDIER_SPECIAL_SKILL_TYPE_INVALID = 0,

	GREATE_SOLDIER_SPECIAL_SKILL_TYPE_1 = 1,				// 全队每秒回血5%
	GREATE_SOLDIER_SPECIAL_SKILL_TYPE_2 = 2,				// 免疫控制技能
	GREATE_SOLDIER_SPECIAL_SKILL_TYPE_3 = 3,				// 激活/结束时，晕眩周围对象2秒，
	GREATE_SOLDIER_SPECIAL_SKILL_TYPE_4 = 4,				// 变身时全队回复生命50%
	GREATE_SOLDIER_SPECIAL_SKILL_TYPE_5 = 5,				// 变身时对周围玩家造成攻击*3的伤害（攻击范围35）
	GREATE_SOLDIER_SPECIAL_SKILL_TYPE_6 = 6,				// 变身时自身受到伤害降低50%
	
	GREATE_SOLDIER_SPECIAL_SKILL_TYPE_7 = 7,				// 变身时额外暴击率+20%
	GREATE_SOLDIER_SPECIAL_SKILL_TYPE_8 = 8,				// 变身时额外闪避率+10%
	GREATE_SOLDIER_SPECIAL_SKILL_TYPE_9 = 9,				// 变身时额外伤害加成+10%
	GREATE_SOLDIER_SPECIAL_SKILL_TYPE_10 = 10,				// 变身时额外伤害减免+10%

	GREATE_SOLDIER_SPECIAL_SKILL_TYPE_11 = 11,				// 变身期间，每2秒对圆形范围(10)内x个目标造成80%攻击的伤害(策划需要走战斗公式，所以做成了假的主动技能，技能ID 603)
	GREATE_SOLDIER_SPECIAL_SKILL_TYPE_12 = 12,				// 变身期间，反弹20%受到的伤害
	GREATE_SOLDIER_SPECIAL_SKILL_TYPE_13 = 13,				// 变身期间，自己和队友(距离15内)每2秒恢复4%最大生命值
	GREATE_SOLDIER_SPECIAL_SKILL_TYPE_14 = 14,				// 变身时，对圆形范围内x个目标造成200%攻击的伤害并使其眩晕，持续2秒(策划需要走战斗公式，所以做成了假的主动技能，技能ID 604)
	GREATE_SOLDIER_SPECIAL_SKILL_TYPE_15 = 15,				// 变身的瞬间，对圆形范围（12）内6个目标拉到自己身边，造成200%的攻击力的伤害并使其移动速度降低50%，持续3秒.(策划需要走战斗公式，所以做成了假的主动技能，技能ID 605)
	GREATE_SOLDIER_SPECIAL_SKILL_TYPE_16 = 16,				// 变身结束的一瞬间，对周围8个目标造成120%攻击的伤害并使其沉默1秒(策划需要走战斗公式，所以做成了假的主动技能，技能ID 606)

	GREATE_SOLDIER_SPECIAL_SKILL_TYPE_MAX,					// 名将特殊技能最大枚举类型
};

UNSTD_STATIC_CHECK(GREATE_SOLDIER_SPECIAL_SKILL_TYPE_MAX <= int(sizeof(UInt32) * CHAR_BIT))

// 潜能类型
enum GREATE_SOLDIER_POTENTIAL_TYPE
{
	GREATE_SOLDIER_POTENTIAL_TYPE_INVALID = 0,

	GREATE_SOLDIER_POTENTIAL_TYPE_GONGJI = 1,
	GREATE_SOLDIER_POTENTIAL_TYPE_FANGYU = 2,
	GREATE_SOLDIER_POTENTIAL_TYPE_HP = 3,

	GREATE_SOLDIER_POTENTIAL_TYPE_MAX
};

// 洗练属性类型
enum GRAETE_SOLDIER_WASH_ATTR_TYPE
{
	GRAETE_SOLDIER_WASH_ATTR_TYPE_BEGIN = 0,
	GRAETE_SOLDIER_WASH_ATTR_TYPE_GONGJI = GRAETE_SOLDIER_WASH_ATTR_TYPE_BEGIN,
	GRAETE_SOLDIER_WASH_ATTR_TYPE_FANGYU,
	GRAETE_SOLDIER_WASH_ATTR_TYPE_MAXHP,

	GRAETE_SOLDIER_WASH_ATTR_TYPE_MAX,
};

// 抽奖类型
enum GREATE_SOLDIER_DRAW_TYPE
{
	GREATE_SOLDIER_DRAW_TYPE_INVALID = 0,

	GREATE_SOLDIER_DRAW_TYPE_1_DRAW = 1,
	GREATE_SOLDIER_DRAW_TYPE_10_DRAW = 2,
	GREATE_SOLDIER_DRAW_TYPE_50_DRAW = 3,

	GREATE_SOLDIER_DRAW_TYPE_SPECIAL_10_DRAW = 10,
	GREATE_SOLDIER_DRAW_TYPE_SPECIAL_50_DRAW = 11,

	GREATE_SOLDIER_DRAW_TYPE_MAX
};

#pragma pack(push, 4)

// 其它信息
struct GreateSoldierOtherParam
{
	GreateSoldierOtherParam() { this->Reset(); }
	void Reset()
	{
		cur_used_seq = -1;
		is_on_bianshen_trail = 0;
		has_dailyfirst_draw_ten = 0;
		use_huanhua_id = 0;
	

		bianshen_end_timestamp = 0;
		bianshen_cd = 0;

		huanhua_flag = 0;
		reserve_int = 0;
	}

	char cur_used_seq;					// 当前使用的名将seq
	char is_on_bianshen_trail;			// 是否处于体验变身状态
	char has_dailyfirst_draw_ten;		// 名将抽奖每日是否已经十连抽
	char use_huanhua_id;				// 使用的幻化ID

	UInt32 bianshen_end_timestamp;		// 变身结束时间
	int bianshen_cd;					// 变身剩余cd (ms)

	unsigned int huanhua_flag;			// 幻化标记
	unsigned int reserve_int;
};

// 名将装备信息
struct GreateSoldierEquipmentInfo
{
	GreateSoldierEquipmentInfo() { this->Reset(); }

	void Reset()
	{
		item_id = 0;
		strength_level = 0;
		shuliandu = 0;
	}

	unsigned short item_id;						// 装备物品ID
	short strength_level;						// 强化等级/装备等级
	int shuliandu;								// 熟练度
};
// 名将信息
struct GreateSoldierItemInfo
{
	bool HasActive() { return level > 0; }

	short level = 0;					// 名将等级
	short potential_level = 0;			// 潜能等级

	UInt32 unactive_timestamp = 0;		// 形象ID结束时间（0代表永久）

	GreateSoldierEquipmentInfo equipment_list[GREATE_SOLDIER_EQUIPMENT_SLOT_MAX_COUNT];
};

struct GreateSoldierItemParam
{
	GreateSoldierItemParam() { this->Reset(); }
	void Reset() { memset(item_list, 0, sizeof(item_list)); }

	GreateSoldierItemInfo item_list[GREATE_SOLDIER_MAX_COUNT];
};

// 将位信息
struct GreateSoldierSlotItemInfo
{
	bool IsFree() const { return item_seq < 0; }

	char item_seq = -1;				// 名将seq
	char reserve = 0;
	short level = 1;				// 等级
	int level_val = 0;				// 升级祝福值
};

struct GreateSoldierSlotParam
{
	GreateSoldierSlotParam() { this->Reset(); }
	void Reset()
	{
		for (auto &item : slot_item_list)
		{
			item.item_seq = -1;
			item.reserve = 0;
			item.level = 1;
			item.level_val = 0;
		}
	}

	GreateSoldierSlotItemInfo slot_item_list[GREATE_SOLDIER_SLOT_MAX_COUNT];	// 0是主将位
};

//-------------------------------------------------------------------------------------------

// 名将目标

struct GreateSoldierGoalData
{
	GreateSoldierGoalData() { this->Reset(); }
	void Reset()
	{
		memset(data_list, 0, sizeof(data_list));
	}

	struct DataItem
	{
		char is_finish;					// 目标是否已完成
		char is_reward_fetched;			// 奖励是否已领取
		char is_purchased;				// 是否已经购买过
		char reserve;
		UInt32 end_time;				// 目标结束时间
	};

	enum GOAL_TYPE
	{
		GOAL_TYPE_1 = 0,
		GOAL_TYPE_2 = 1,
		GOAL_TYPE_MAX,
	};
	static bool IsValidGoalType(int type)
	{
		return type >= 0 && type < GOAL_TYPE_MAX;
	}

	static const int GOAL_TYPE_COUNT = 4;
	UNSTD_STATIC_CHECK(GOAL_TYPE_MAX < GOAL_TYPE_COUNT);
	DataItem data_list[GOAL_TYPE_COUNT];
};

//-------------------------------------------------------------------------------------------

struct GreateSoldierParam
{
	GreateSoldierParam() { this->Reset(); }

	void Reset()
	{
		other_param.Reset();
		item_param.Reset();
		slot_param.Reset();

		memset(xinghun_level_list, 0, sizeof(xinghun_level_list));
		memset(xinghun_level_max_list, 0, sizeof(xinghun_level_max_list));
		xinghun_progress = 1;
		reward_fetch_flag = 0;
		draw_times = 0;

		goal_data.Reset();
		memset(seq_exchange_counts, 0, sizeof(seq_exchange_counts));
	}

	GreateSoldierOtherParam other_param;
	GreateSoldierItemParam item_param;
	GreateSoldierSlotParam slot_param;

	//-------------------------------------------------------------------------------------------名将根骨BEGIN
	short xinghun_level_list[GREATE_SOLDIER_MAX_COUNT];
	short xinghun_level_max_list[GREATE_SOLDIER_MAX_COUNT];
	int xinghun_progress;			// 星魂开锁进度
	unsigned int reward_fetch_flag;		// 抽奖保底奖励
	int draw_times;					//抽奖次数
	//-------------------------------------------------------------------------------------------名将根骨END

	GreateSoldierGoalData goal_data;
	short seq_exchange_counts[GREATE_SOLDIER_MAX_COUNT];											//兑换信息

};

typedef char GreateSoldierParamHex[sizeof(GreateSoldierParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(GreateSoldierParamHex) < 4096);

//-------------------------------------------------------------------------------------------

#pragma pack(pop)

#endif // __GREATE_SOLDIER_DEF_HPP__