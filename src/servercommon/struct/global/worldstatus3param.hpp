// 此模块用于存储跨服gs全局数据  合服不清此表

#ifndef __WORLD_STATUS3_PARAM_HPP__
#define __WORLD_STATUS3_PARAM_HPP__

#include "common/tlvprotocol.h"
#include "servercommon/serverdef.h"
#include "servercommon/basedef.h"

#include "servercommon/crossdef.hpp"
#include "servercommon/rankdef.hpp"
#include "servercommon/rolelotterycountdef.hpp"

struct WorldStatus3CommomParam
{
	WorldStatus3CommomParam()
	{
		this->Reset();
	}

	void Reset()
	{
		memset(cross_add_cap_rank_uid_list, 0, sizeof(cross_add_cap_rank_uid_list));
		memset(cross_add_charm_rank_uid_list, 0, sizeof(cross_add_charm_rank_uid_list));
		memset(cross_qingyuan_cap_rank_male_uid_list, 0, sizeof(cross_qingyuan_cap_rank_male_uid_list));
		memset(cross_qingyuan_cap_rank_female_uid_list, 0, sizeof(cross_qingyuan_cap_rank_female_uid_list));
		memset(cross_guild_id_list, 0, sizeof(cross_guild_id_list));
		memset(cross_qingyuan_cap_rank_male_uuid_list, 0, sizeof(cross_qingyuan_cap_rank_male_uuid_list));
		memset(cross_qingyuan_cap_rank_female_uuid_list, 0, sizeof(cross_qingyuan_cap_rank_female_uuid_list));
	}

	int cross_add_cap_rank_uid_list[CROSS_RANK_REWARD_COUNT];				// 跨服增加战力榜
	int cross_add_charm_rank_uid_list[CROSS_RANK_REWARD_COUNT];			    // 跨服增加魅力榜
	int cross_qingyuan_cap_rank_male_uid_list[CROSS_RANK_REWARD_COUNT];	    // 跨服情缘榜男-旧数据
	int cross_qingyuan_cap_rank_female_uid_list[CROSS_RANK_REWARD_COUNT];	// 跨服情缘榜女-旧数据
	int cross_guild_id_list[CROSS_RANK_REWARD_COUNT];						// 跨服公会击杀榜
	long long cross_qingyuan_cap_rank_male_uuid_list[CROSS_RANK_REWARD_COUNT];	    // 跨服情缘榜男
	long long cross_qingyuan_cap_rank_female_uuid_list[CROSS_RANK_REWARD_COUNT];	// 跨服情缘榜女
};

struct WorldLotteryCountParam
{
	WorldLotteryCountParam() { this->Reset(); }
	void Reset()
	{
		memset(lottery_count, 0, sizeof(lottery_count));
	}
	UserLotteryCount lottery_count[LOTTERY_REPLACE_TYPE_MAX];
};

class WorldStatus3Param
{
public:
	bool Serialize(TLVSerializer &outstream) const;
	bool Unserialize(TLVUnserializer &instream);

	WorldStatus3CommomParam common_data;
	TopRankParam toprank_param;
	WorldLotteryCountParam lottery_count_param;
	RandActivityParam rand_activity_param;
	RandActivityLuckyCloudBuyParam lucky_cloud_buy_param;
};

typedef char WorldStatus3CommomParamHex[sizeof(WorldStatus3CommomParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(WorldStatus3CommomParamHex) < 1024);

typedef char TopRankParamHex[sizeof(TopRankParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(TopRankParamHex) < 3072);

typedef char WorldLotteryCountParamHex[sizeof(WorldLotteryCountParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(WorldLotteryCountParamHex) < 256);

typedef char RandActivityParamHex[sizeof(RandActivityParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(RandActivityParamHex) <58880);

typedef char RandActivityLuckyCloudBuyParamHex[sizeof(RandActivityLuckyCloudBuyParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(RandActivityLuckyCloudBuyParamHex) < 2048);

#endif // __WORLD_STATUS3_PARAM_HPP__

