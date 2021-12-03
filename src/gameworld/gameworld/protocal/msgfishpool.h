#ifndef MSGFISHPOOL_H
#define MSGFISHPOOL_H

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/basedef.h"
#include "gamedef.h"
#include "servercommon/fishpooldef.hpp"
#include "servercommon/guilddef.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	struct FishPoolCommonData
	{
		int owner_uid;
		GameName owner_name;
		int role_level;

		int bullet_buy_times;			// 购买子弹次数
		int bullet_buy_num;				// 购买子弹的数量
		int bullet_consume_num;			// 消耗子弹的数量
		
		int today_fang_fish_times;		// 养鱼次数
		int today_buy_fang_fish_tims;	// 购买养鱼次数
	};

	class SCFishPoolAllRaiseInfo								// 鱼塘鱼儿信息 2801
	{
	public:
		SCFishPoolAllRaiseInfo();
		MessageHeader header;

		int owner_uid;
		int fish_quality;				// 鱼的品质
		int fish_num;					// 鱼数量
		int fang_fish_time;				// 放鱼的时间
	};

	class SCFishPoolCommonInfo									// 普通信息 2802
	{
	public:
		SCFishPoolCommonInfo();
		MessageHeader header;

		FishPoolCommonData common_data;
	};

	class CSFishPoolBuyBulletReq								// 购买子弹请求 2851
	{
	public:
		CSFishPoolBuyBulletReq();
		MessageHeader header;
	};

	class SCUpFishQualityRet									// 提高鱼的品质结果 2862
	{
	public:
		SCUpFishQualityRet();
		MessageHeader header;

		int quality;
	};

	class CSFishPoolRaiseReq									// 请求放养鱼儿 2850
	{
	public:
		CSFishPoolRaiseReq();
		MessageHeader header;
	};

	enum FISH_POOL_QUERY_TYPE
	{
		FISH_POOL_QUERY_TYPE_ALL_INFO,							// 所有信息
		FISH_POOL_QUERY_TYPE_RAISE_INFO,						// 放养信息
		FISH_POOL_QUERY_TYPE_WORLD_GENERAL_INFO,				// 世界玩家简要信息
		FISH_POOL_QUERY_TYPE_STEAL_GENERAL_INFO,				// 偷鱼者简要信息
		FISH_POOL_UP_FISH_QUALITY,								// 请求升级鱼品质
		FISH_POOL_BUY_FANG_FISH_TIMES,							// 购买放鱼次数

		FISH_POOL_QUERY_MAX_TYPE,
	};

	class CSFishPoolQueryReq									// 查询信息请求  2852
	{
	public:
		CSFishPoolQueryReq();
		MessageHeader header;

		short query_type;
		short reserve_sh;
		int param;
	};

	class CSFishPoolStealFish									// 偷鱼请求 2853
	{
	public:
		CSFishPoolStealFish();
		MessageHeader header;

		int target_uid;
		char is_fake_pool;
		char quality;
		char type;												// 普通鱼：0  守卫鱼：1   没打到鱼：2
		char reserve;
	};

	class SCFishPoolChange										// 鱼数量变化 2863
	{
	public:
		SCFishPoolChange();
		MessageHeader header;

		int uid;
		int fish_num;
		int fish_quality;
		int is_steal_succ;										// 0偷鱼失败，1偷鱼成功，2更新信息
	};

	class CSFishPoolHarvest										// 收获请求 2854
	{
	public:
		CSFishPoolHarvest();
		MessageHeader header;

	};

	struct FishPoolGeneralInfoItem
	{
		FishPoolGeneralInfoItem() { this->Reset(); }
		void Reset()
		{
			owner_uid = 0;
			memset(owner_name, 0, sizeof(GameName));
			fish_quality = 0;
			fish_num = 0;
			fang_fish_time = 0;
			is_fake_pool = 0;
			is_fuchou = 0;
			be_steal_quality = 0;
			steal_fish_time = 0;
		}

		int owner_uid;
		GameName owner_name;
		int fish_quality;				// 鱼的品质
		int fish_num;					// 鱼数量
		int fang_fish_time;				// 放鱼的时间
		int is_fake_pool;				// 是否是假鱼池
		int is_fuchou;					// 是否已经复仇
		int be_steal_quality;			// 被偷的鱼的品质
		unsigned int steal_fish_time;	// 偷鱼时间
	};

	class SCFishPoolWorldGeneralInfo							// 随机100个玩家的鱼池信息 2804
	{
	public:
		SCFishPoolWorldGeneralInfo();
		MessageHeader header;

		int info_count;
		FishPoolGeneralInfoItem info_list[FISH_POOL_RANDOM_GET_ROLE_MAX_COUNT];
	};

	class SCFishPoolStealGeneralInfo							// 偷鱼者信息 2805
	{
	public:
		SCFishPoolStealGeneralInfo();
		MessageHeader header;

		int info_count;
		FishPoolGeneralInfoItem info_list[FISH_POOL_BE_TEAL_FISH_UID_MAX];
	};

	class SCFishPoolShouFishRewardInfo							// 收鱼奖励信息  2810
	{
	public:
		SCFishPoolShouFishRewardInfo();
		MessageHeader header;

		int item_id;
		int item_num;
		long long exp;
		int rune_score;
		int quality;
		char is_skip;											// 是否一键跳过0不是1是
		char reserve_ch;
		short reserve_sh;
	};
}

#pragma pack(pop)

#endif


