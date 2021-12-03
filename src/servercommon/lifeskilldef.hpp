#ifndef __LIFE_SKILL_DEF_HPP__
#define __LIFE_SKILL_DEF_HPP__

#include "servercommon.h"

#pragma pack(push, 4)

enum LIFE_SKIL_TYPE 
{
	LIFE_SKIL_TYPE_FUMO = 0,													// 附魔
	LIFE_SKIL_TYPE_LIANJIN,														// 炼金 
	LIFE_SKIL_TYPE_DAZAO,														// 打造
	LIFE_SKIL_TYPE_ZHIYAO,														// 制药
	LIFE_SKIL_TYPE_MAX,
};

static const int LIFE_SKILL_SHULIANDU_LV_MAX = 10;								// 熟练度最高10级
static const int LIFE_SKILL_LV_MAX = 100;										// 技能等级100级

struct LifeSkill
{
	LifeSkill() : skill_lv(0), shuliandu_lv(0), shuliandu(0), reserve1(0) {}

	short skill_lv;
	short shuliandu_lv;
	int shuliandu;
	int reserve1;
};

struct LifeSkillParam
{
	LifeSkillParam() {}

	void Reset()
	{
		memset(life_skill_list, 0, sizeof(life_skill_list));
	}

	LifeSkill life_skill_list[LIFE_SKIL_TYPE_MAX];
};

typedef char LifeSkillParamHex[sizeof(LifeSkillParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(LifeSkillParamHex) < 128);

#pragma pack(pop)

#endif  // __LIFE_SKILL_DEF_HPP__

