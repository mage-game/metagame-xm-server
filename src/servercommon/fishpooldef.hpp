#ifndef __FISH_POOL_DEF_HPP__
#define __FISH_POOL_DEF_HPP__

#include "servercommon/servercommon.h"

const static int FISH_POOL_ONCE_LOAD_MAX_COUNT = 512;						// 一次从数据库Load的池塘数
const static int FISH_POOL_CHECK_SAVE_INTERVAL = 30;						// 检查保存的时间间隔

const static int FISH_QUALITY_COUNT = 4;									// 鱼的品质数
const static int FISH_POOL_BE_TEAL_FISH_UID_MAX = 100;						// 记录偷自己鱼的玩家的最大数
const static int FISH_POOL_RANDOM_GET_ROLE_MAX_COUNT = 100;

#pragma pack(push, 4)

struct TealFishParam
{
	TealFishParam() { this->Reset(); }
	void Reset()
	{
		uid = 0;
		is_fuchou = 0;
		be_steal_quality = 0;
		steal_fish_time = 0;
	}

	int uid;
	int is_fuchou;
	int be_steal_quality;
	unsigned int steal_fish_time;
};

struct FishPoolCommonDataParam
{
	FishPoolCommonDataParam() { this->Reset(); }

	void Reset()
	{
		memset(role_name, 0, sizeof(role_name));

		role_level = 0;
		bullet_buy_times = 0;
		bullet_buy_num = 0;
		bullet_consume_num = 0;
		fish_quality = 0;
		fish_num = 0;
		fang_fish_time = 0;
		today_fang_fish_times = 0;
		today_buy_fang_fish_tims = 0;
		
		for (int i = 0; i < FISH_POOL_BE_TEAL_FISH_UID_MAX; i++)
		{
			teal_fish_list[i].Reset();
		}
	}

	GameName role_name;
	int role_level;
	short bullet_buy_times;
	int bullet_buy_num;
	int bullet_consume_num;
	int fish_quality;
	int fish_num;
	int fang_fish_time;
	int today_fang_fish_times;
	int today_buy_fang_fish_tims;
	TealFishParam teal_fish_list[FISH_POOL_BE_TEAL_FISH_UID_MAX];
}; 

typedef char FishPoolCommonDataParamHex[sizeof(FishPoolCommonDataParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(FishPoolCommonDataParamHex) < 4096);

#pragma pack(pop)

#endif

