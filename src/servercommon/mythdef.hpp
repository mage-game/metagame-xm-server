#ifndef __MYTH_DEF_HPP__
#define __MYTH_DEF_HPP__

#include "servercommon.h"

static const int MAX_MYTH_CHAPTER_ID = 20;
static const int MAX_MYTH_SOUL_SLOT = 3;
static const int MAX_MYTH_SOUL_TYPE = 6;
static const int MAX_MYTH_DIGESTION_LEVEL = 300;
static const int MAX_MYTH_CHPATER_LEVEL = 150;
static const int MAX_MYTH_QUALITY_SOUL = 5;
static const int MAX_MYTH_RESONANCE_LEVEL = 20;
static const int MAX_MYTH_RESONANCE_ATTR_NUM = 4;
static const int MAX_MYTH_BAOJI_MULTIPLE = 3;
static const int MAX_MYTH_SOUL_QUALITY = 5;
static const int MAX_MYTH_STAR_SOUL = 3;
static const int MAX_MYTH_RAND_ATTR_NORMAL_COUNT = 9;
static const int MAX_MYTH_RAND_ATTR_RARE_COUNT = 9;

static const int MAX_MYTH_KNAPSACK_GIRD_COUNT = 100;							// 背包数
static const int MAX_MYTH_SOUL_RAND_ATTR_COUNT = 3;								// 随机属性数

static const int MAX_MYTH_KNAPSACK_GIRD_STACK_COUNT = 99;						// 堆叠上限

enum MYTH_ATTR_TYPE																// 装备属性属性种类
{
	MYTH_ATTR_TYPE_INVALID_MIN = 0,

	MYTH_ATTR_TYPE_PER_HP,														// 生命万分比
	MYTH_ATTR_TYPE_PER_GONGJI,													// 攻击万分比
	MYTH_ATTR_TYPE_PER_FANGYU,													// 物防万分比
	MYTH_ATTR_TYPE_PER_SHANBI,													// 闪避万分比
	MYTH_ATTR_TYPE_PER_MINGZHONG,												// 命中万分比
	MYTH_ATTR_TYPE_PER_BAOJI,													// 暴击万分比
	MYTH_ATTR_TYPE_PER_JIANREN,													// 抗暴万分比
	MYTH_ATTR_TYPE_PER_GUSHANG,													// 固伤万分比
	MYTH_ATTR_TYPE_PER_JIANSHANG,												// 减伤万分比
	MYTH_ATTR_TYPE_MAX_HP,														// 生命固定值
	MYTH_ATTR_TYPE_GONGJI,														// 攻击固定值
	MYTH_ATTR_TYPE_FANGYU,														// 物防固定值
	MYTH_ATTR_TYPE_SHANBI,														// 闪避固定值
	MYTH_ATTR_TYPE_MINGZHONG,													// 命中固定值
	MYTH_ATTR_TYPE_BAOJI,														// 暴击固定值
	MYTH_ATTR_TYPE_JIANREN,														// 抗暴固定值
	MYTH_ATTR_TYPE_GUSHANG,														// 固伤固定值											
	MYTH_ATTR_TYPE_JIANSHANG,													// 减伤固定值

	MYTH_ATTR_TYPE_INVALID_MAX,
};

enum MYTH_RESONANCE_ATTR_TYPE
{
	MYTH_RESONANCE_ATTR_TYPE_MAXHP = 1,
	MYTH_RESONANCE_ATTR_TYPE_GONGJI,
	MYTH_RESONANCE_ATTR_TYPE_FANGYU,
	MYTH_RESONANCE_ATTR_TYPE_MINGZHONG,
	MYTH_RESONANCE_ATTR_TYPE_SHANBI,
	MYTH_RESONANCE_ATTR_TYPE_BAOJI,
	MYTH_RESONANCE_ATTR_TYPE_JIANREN,
	MYTH_RESONANCE_ATTR_TYPE_CONSTANT_ZENGSHANG,
	MYTH_RESONANCE_ATTR_TYPE_CONSTANT_MIANSHANG,
	MYTH_RESONANCE_ATTR_TYPE_PER_GONGJI,										// 攻击万分比
	MYTH_RESONANCE_ATTR_TYPE_PER_FANGYU,										// 物防万分比
	MYTH_RESONANCE_ATTR_TYPE_PER_HP,											// 生命万分比
	MYTH_RESONANCE_ATTR_TYPE_PER_BAOJI,											// 暴击万分比
	MYTH_RESONANCE_ATTR_TYPE_PER_GUSHANG,										// 固伤万分比
	MYTH_RESONANCE_ATTR_TYPE_PER_JIANSHANG,										// 减伤万分比

	MYTH_RESONANCE_ATTR_TYPE_SKILL_ZENGSHANG_PER = 21,
	MYTH_RESONANCE_ATTR_TYPE_SKILL_JIANSHANG_PER,
	MYTH_RESONANCE_ATTR_TYPE_PER_KANGBAO,
	MYTH_RESONANCE_ATTR_TYPE_MINGZHONG_PER,
	MYTH_RESONANCE_ATTR_TYPE_SHANBI_PER,
	MYTH_RESONANCE_ATTR_TYPE_GEDANG_PER,
};


#pragma pack(push, 4)

struct MythSoulGodItem
{
	bool Isvalid() const
	{
		return item_id > 0;
	}

	struct AttrItem
	{
		AttrItem()
		{
			this->Reset();
		}

		void Reset()
		{
			attr_type = 0;
			reserve_sh = 0;
			attr_value = 0;
		}

		short attr_type;
		short reserve_sh;
		int attr_value;
	};

	MythSoulGodItem() { this->Reset(); }

	void Reset()
	{
		item_id = 0;
		num = 0;
		quality = 0;

		for (int i = 0; i < MAX_MYTH_SOUL_RAND_ATTR_COUNT; i++)
		{
			attr_list[i].Reset();
		}
	}

	ItemID item_id;
	unsigned char num;
	unsigned char quality;
	AttrItem attr_list[MAX_MYTH_SOUL_RAND_ATTR_COUNT];
};

struct MythResonanceItem
{
	MythResonanceItem() { this->Reset();}

	bool IsActivatedResonance()
	{
		return (0 != resonance_level);
	}

	void ActivatedResonance()
	{
		resonance_level = 1;
		memset(cur_level_resonance, 0, sizeof(cur_level_resonance));
	}

	void ResonanceUpLevel()
	{
		++resonance_level;
		memset(cur_level_resonance, 0, sizeof(cur_level_resonance));
	}

	void Reset()
	{
		memset(cur_level_resonance, 0, sizeof(cur_level_resonance));
		resonance_level = 0;
	}

	char cur_level_resonance[MAX_MYTH_SOUL_SLOT];
	char dirty_ch;			// 不可用，没有初始化的脏数据

	short resonance_level;
	short dirty_sh;			// 不可用，没有初始化的脏数据
};

struct MythChpaterItem
{
	MythChpaterItem() { this->Reset(); }

	void Reset()
	{
		level = 0;
		digestion_level = 0;
		digestion_level_val = 0;
	
		resonance_item.Reset();

		for (int i = 0; i < MAX_MYTH_SOUL_SLOT; i++)
		{
			soul_god_list[i].Reset();
		}
	}

	bool IsActivatingMythChpater()
	{
		return 1 == level;
	}

	void UpLevelMythChpater()
	{
		++level;
	}

	short level;
	short digestion_level;
	int digestion_level_val;

	MythResonanceItem resonance_item;
	MythSoulGodItem soul_god_list[MAX_MYTH_SOUL_SLOT];
};

struct MythParam
{
	MythParam() { this->Reset(); }

	void Reset()
	{
		soul_essence = 0;

		for (int i = 0; i < MAX_MYTH_CHAPTER_ID; i ++)
		{
			chpater_list[i].Reset();
		}

		for (int i = 0; i < MAX_MYTH_KNAPSACK_GIRD_COUNT; i++)
		{
			knapasck_list[i].Reset();
		}
	}

	int soul_essence;														//神魂精华（分解获得）

	MythSoulGodItem knapasck_list[MAX_MYTH_KNAPSACK_GIRD_COUNT];			//背包列表
	MythChpaterItem chpater_list[MAX_MYTH_CHAPTER_ID];						//放在后面方便拓展		
};

typedef char MythParamHex[sizeof(MythParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(MythParamHex) < 10240);

#pragma pack(pop)

#endif //__MYTH_DEF_HPP__