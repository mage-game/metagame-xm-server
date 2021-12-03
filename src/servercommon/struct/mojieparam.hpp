#ifndef __MOJIEPARAM_HPP__
#define __MOJIEPARAM_HPP__

#include "servercommon/servercommon.h"

#pragma pack(push) 
#pragma pack(4)

struct MojieParam
{
	static const int MOJIE_MAX_TYPE = 4;						// 魔戒最大类型
	static const int MOJIE_MAX_LEVEL = 100;						// 魔戒最大等级


	struct MojieInfo
	{
		MojieInfo():mojie_skill_type(0),mojie_level(0), mojie_skill_id(0), mojie_skill_level(0){}

		unsigned short mojie_skill_type;
		unsigned short mojie_level;
		unsigned short mojie_skill_id;
		unsigned short mojie_skill_level;
		unsigned int last_colddown_time;  //魔戒被动技能冷却时间
	};

	MojieParam()
	{
		this->Reset();
	}

	void Reset()
	{
		for (int i = 0; i < MOJIE_MAX_TYPE; i++)
		{
			mojie_list[i].mojie_skill_type = i;
			mojie_list[i].mojie_level = 0;
			mojie_list[i].mojie_skill_id = 0;
			mojie_list[i].mojie_skill_level = 0;
			mojie_list[i].last_colddown_time = 0;
		}

		is_first_active_mojie = 0;
		reserve_1 = 0;
		reserve_2 = 0;
	}

	MojieInfo mojie_list[MOJIE_MAX_TYPE];

	char is_first_active_mojie;
	char reserve_1;
	short reserve_2;
};

#pragma pack(pop)

typedef char MojieParamHex[sizeof(MojieParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(MojieParamHex) < 128);

#endif



