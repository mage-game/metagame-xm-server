
#ifndef __COMMON_DATA_GOLBAL_HPP__
#define __COMMON_DATA_GOLBAL_HPP__

#include "servercommon/struct/itemlistparam.h"

#pragma pack(push) 
#pragma pack(4)

//物品赠送----------------------------------
static const int FRIEND_ITEM_GIFT_RECORD_MAX_COUNT = 20;		    // 赠送物品记录条数
static const int GIVE_ITEM_MAX_COUNT = 7;							// 单次赠送物品数

struct ItemGiftInfo
{
	ItemGiftInfo()
	{
		this->Reset();
	}

	void Reset()
	{
		uid = 0;
		memset(role_name, 0, sizeof(role_name));
		item_id = 0;
		item_num = 0;
		timestamp = 0;
	}

	int uid;
	GameName role_name;
	ItemID item_id;
	unsigned short item_num;
	unsigned int timestamp;
};

struct ItemGiftRecord
{
	ItemGiftRecord()
	{
		this->Reset();
	}

	void Reset()
	{
		give_gift.Reset();
		recieve_gift.Reset();
	}

	ItemGiftInfo give_gift;
	ItemGiftInfo recieve_gift;
};

struct CommonDataGlobalParam
{

	CommonDataGlobalParam()
	{
		this->Reset();
	}

	void Reset()
	{
		sys_consume_gold_in_cross = 0;
		is_chengzhu = 0;
		reserve_ch = 0;
		combine_kill_boss_count = 0;

		memset(gift_record_list, 0, sizeof(gift_record_list));

		cross_challengefield_rank_last_mount_id = 0;
		cross_challengefield_rank_mount_id = 0;
		cross_challengefield_rank_title_id = 0;
		cross_challengefield_rank_last_title_id = 0;
		nex_clear_id_time = 0;
		cross_challengefield_rank = 0;

		reserve_sh = 0;
	}

	int sys_consume_gold_in_cross;					// 上一次在跨服因系统消耗的元宝
	char is_chengzhu;								// 是否是城主
	char reserve_ch;								// 
	short combine_kill_boss_count;					// 跨服击杀合服boss数

	ItemGiftRecord gift_record_list[FRIEND_ITEM_GIFT_RECORD_MAX_COUNT];						// 物品赠送/收到记录

	short cross_challengefield_rank_last_mount_id;		// 跨服竞技场排行奖励上一次坐骑
	short cross_challengefield_rank_mount_id;			// 跨服竞技场排行奖励坐骑
	short cross_challengefield_rank_title_id;			// 跨服竞技场排行奖励称号
	short cross_challengefield_rank_last_title_id;		// 跨服竞技场排行奖励上一次称号
	unsigned int nex_clear_id_time;						// 下次清空排行奖励时间
	short cross_challengefield_rank;					// 跨服竞技场排名
	short reserve_sh;						

};

#pragma pack(pop)

typedef char CommonDataGlobalParamHex[sizeof(CommonDataGlobalParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(CommonDataGlobalParamHex) < 4096);

#endif  // __COMMON_DATA_GOLBAL_HPP__