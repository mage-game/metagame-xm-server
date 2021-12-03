#ifndef TALENTDEF_DEF_HPP
#define TALENTDEF_DEF_HPP

#include "servercommon.h"

#pragma pack(push, 4)

static const int JINJIE_TALENT_CHOUJIANG_GRID_MAX_NUM = 9;	//天赋抽奖最大格子数量
static const int JINJIE_TALENT_SKILL_GRID_MAX_NUM = 13;		//天赋技能最大格子数量
//static const int TALENT_TYPE_MAX = GREATE_SOLDIER_MAX_COUNT;	//开放天赋类型
static const int TALENT_ATTENTION_SKILL_MAX_SAVE_NUM = 32;		//关注的技能列表

enum JINJIE_TALENT_TYPE
{
	JINJIE_TALENT_MOUNT = 0,		//坐骑
	JINJIE_TALENT_WING,				//羽翼
	JINJIE_TALENT_HALO,				//光环
	JINJIE_TALENT_FIGHTMOUNT,		//魔骑
	//JINJIE_TALENT_SHENGGONG,		//神弓
	JINJIE_TALENT_SHIZHUANG,		//时装
	//JINJIE_TALENT_SHENYI,			//神翼
	JINJIE_TALENT_SHENBING,			//神兵
	JINJIE_TALENT_FOOTPRINT,		//足记
	JINJIE_TALENT_FABAO,			//法宝
	JINJIE_TALENT_TYPE_MAX,
};

enum JINJIE_TALENT_SKILL_TYPE
{
	JINJIE_TALENT_SKILL_TYPE_0 = 0,		//气血
	JINJIE_TALENT_SKILL_TYPE_1,			//攻击
	JINJIE_TALENT_SKILL_TYPE_2,			//防御
	JINJIE_TALENT_SKILL_TYPE_3,			//命中
	JINJIE_TALENT_SKILL_TYPE_4,			//闪避
	JINJIE_TALENT_SKILL_TYPE_5,			//暴击
	JINJIE_TALENT_SKILL_TYPE_6,			//抗暴
	JINJIE_TALENT_SKILL_TYPE_7,			//固定增伤
	JINJIE_TALENT_SKILL_TYPE_8,			//固定免伤
	JINJIE_TALENT_SKILL_TYPE_9,			//对应系统进阶属性百分比
	JINJIE_TALENT_SKILL_TYPE_10,			//本天赋页气血百分比+固定值
	JINJIE_TALENT_SKILL_TYPE_11,			//本天赋页攻击百分比+固定值
	JINJIE_TALENT_SKILL_TYPE_12,			//本天赋页防御百分比+固定值
	JINJIE_TALENT_SKILL_TYPE_13,			//本天赋页命中百分比+固定值
	JINJIE_TALENT_SKILL_TYPE_14,			//本天赋页闪避百分比+固定值
	JINJIE_TALENT_SKILL_TYPE_15,			//本天赋页暴击百分比+固定值
	JINJIE_TALENT_SKILL_TYPE_16,			//本天赋页抗暴百分比+固定值
	JINJIE_TALENT_SKILL_TYPE_17,			//本天赋页固定增伤百分比+固定值
	JINJIE_TALENT_SKILL_TYPE_18,			//本天赋页固定免伤百分比+固定值
	JINJIE_TALENT_SKILL_TYPE_19,			//坐骑终极技能
	JINJIE_TALENT_SKILL_TYPE_20,			//羽翼终极技能
	JINJIE_TALENT_SKILL_TYPE_21,			//光环终极技能
	JINJIE_TALENT_SKILL_TYPE_22,			//魔骑终极技能
	JINJIE_TALENT_SKILL_TYPE_23,			//神弓终极技能
	JINJIE_TALENT_SKILL_TYPE_24,			//神翼终极技能
	JINJIE_TALENT_SKILL_TYPE_25,			//足迹终极技能
	JINJIE_TALENT_SKILL_TYPE_26,			//法宝终极技能
	JINJIE_TALENT_SKILL_TYPE_COUNT,
};

/*
天赋技能格
*/
struct SCJinJieTalentGrid
{
public:
	SCJinJieTalentGrid()
	{
		Reset();
	}
	void Reset()
	{
		is_open = 0;
		skill_star = 0;
		skill_id = 0;
	}

	char is_open;				//格子是否开启
	char skill_star;			//技能星级
	short skill_id;				//技能ID
};

/*
单个天赋
*/
struct SCJinJieTalentSingle
{
public:
	SCJinJieTalentSingle()
	{
		Reset();
	}

	void Reset()
	{
		memset(grid_array, 0, sizeof(grid_array));
	}

	SCJinJieTalentGrid grid_array[JINJIE_TALENT_SKILL_GRID_MAX_NUM];
};

struct JinJieTalentParam
{
	JinJieTalentParam()
	{
		this->Reset();
	}

	void Reset()
	{
		flush_count = 0;
		memset(choujiang_grid_skill, 0, sizeof(choujiang_grid_skill));
		for (int i = 0; i < JINJIE_TALENT_TYPE_MAX; ++i)
		{
			skill_info_list[i].Reset();
		}
		free_chou_count = 0;
		reserve_sh = 0;

		memset(attention_skill_id, 0, sizeof(attention_skill_id));
	}

	void ResetChoujiang()
	{
		flush_count = 0;
		memset(choujiang_grid_skill, 0, sizeof(choujiang_grid_skill));
	}

	short flush_count;
	short choujiang_grid_skill[JINJIE_TALENT_CHOUJIANG_GRID_MAX_NUM];						//抽奖格技能
	SCJinJieTalentSingle skill_info_list[JINJIE_TALENT_TYPE_MAX];
	short free_chou_count;
	short reserve_sh;
	short attention_skill_id[TALENT_ATTENTION_SKILL_MAX_SAVE_NUM];
};

typedef char JinJieTalentParamHex[sizeof(JinJieTalentParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(JinJieTalentParamHex) < 1024);

#pragma pack(pop)
#endif