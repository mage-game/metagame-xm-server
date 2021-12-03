#ifndef __FOLLOWBOSS_DEF_HPP__
#define __FOLLOWBOSS_DEF_HPP__

#include "servercommon.h"

#pragma pack(push) 
#pragma pack(4)

struct FollowBossParam
{
	static const int MAX_FOLLOW_BOSS_COUNT = 100;		// 最大关注boss数

	struct BossBaseInfo
	{
		short boss_id;
		char boss_type;
		char reserve_ch;
		int scene_id;
	};

	FollowBossParam() 
	{
		this->Reset();
	}

	void Reset()
	{
		for (int i = 0; i < MAX_FOLLOW_BOSS_COUNT; ++i)
		{
			follow_boss_list[i].boss_id = 0;
			follow_boss_list[i].boss_type = 0;
			follow_boss_list[i].reserve_ch = 0;
			follow_boss_list[i].scene_id = 0;
		}
	}

	
	BossBaseInfo follow_boss_list[MAX_FOLLOW_BOSS_COUNT];	// 关注Boss列表
};

#pragma pack(pop)

typedef char FollowBossParamHex[sizeof(FollowBossParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(FollowBossParamHex) < 2048);

#endif  // __FOLLOWBOSS_DEF_HPP__