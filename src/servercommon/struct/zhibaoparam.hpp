#ifndef __ZHIBAO_PARAM_HPP__
#define __ZHIBAO_PARAM_HPP__

#include "servercommon/servercommon.h"

#pragma pack(push) 
#pragma pack(4)

static const int MAX_ZHIBAO_LEVEL = 300;
static const int MAX_ZHIBAO_SKILL_COUNT = 4;
static const int MAX_ZHIBAO_SKILL_LEVEL = 50;
static const int MAX_ZHIBAO_IMAGE_ID = 12;

static const int MAX_ZHIBAO_HUANHUA_TPYE_COUNT = 30;
static const int MAX_ZHIBAO_HUANHUA_GRADE_COUNT = 150;

struct ZhiBaoParam
{
	ZhiBaoParam() { this->Reset(); }

	bool IsHuanHuaActive(int huanhua_id) 
	{
		return 0 != (active_huanhua_flag & (1 << huanhua_id));
	}

	void ActiveHuanHua(int huanhua_id) 
	{
		active_huanhua_flag |= (1 << huanhua_id); 
	}

	void Reset()
	{
		level = 0;
		use_image = 0;
		exp = 0;
		active_huanhua_flag = 0;
		last_skill_time = 0;
		reseve = 0;

		memset(grade, 0, sizeof(grade));
	}

	short level;
	short use_image;
	int exp;
	unsigned long long last_skill_time;
	short reseve;
	short active_huanhua_flag;							// 激活的幻化列表
	short grade[MAX_ZHIBAO_HUANHUA_TPYE_COUNT];
};

#pragma pack(pop)

typedef char ZhiBaoParamHex[sizeof(ZhiBaoParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(ZhiBaoParamHex) < 256);

#endif // __ZHIBAO_PARAM_HPP__
