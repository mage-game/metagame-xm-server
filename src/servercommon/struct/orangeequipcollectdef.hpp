// 橙装收集相关定义
#ifndef __ORANGE_EQUIP_COLLECT_DEF_HPP__
#define __ORANGE_EQUIP_COLLECT_DEF_HPP__

#include "servercommon/servercommon.h"
#include "servercommon/struct/itemlistparam.h"

static constexpr int ORANGE_EQUIP_COLLECT_SEQ_MAX_COUNT = 18;
static constexpr int ORANGE_EQUIP_COLLECT_PRO_MAX = 5;

inline bool IsValidOrangeEquipCollectSeq(int seq)
{
	return seq >= 0 && seq < ORANGE_EQUIP_COLLECT_SEQ_MAX_COUNT;
}
inline bool IsValidOrangeEquipCollectPro(int pro)
{
	return pro > PROF_TYPE_INVALID && pro < PROF_TYPE_PROF_NO_LIMIT;
}

static constexpr int ORANGE_EQUIP_COLLECT_SLOT_COUNT = 12;			// 橙装收集槽数
inline bool IsValidOrangeEquipCollectSlot(int slot)
{
	return slot >= 0 && slot < ORANGE_EQUIP_COLLECT_SLOT_COUNT;
}

static constexpr int ORANGE_EQUIP_COLLECT_ACT_REWARD_MAX_COUNT = 2;
inline bool IsValidOrangeEquipActRewardSeq(int seq)
{
	return seq >= 0 && seq < ORANGE_EQUIP_COLLECT_ACT_REWARD_MAX_COUNT;
}

#pragma pack(push, 4)

struct OrangeEquipCollectInfo
{
	int item_count = 0;		// 个数
	int stars = 0;			// 星级（高品质仙品个数）
};

struct OrangeEquipCollectParam
{
	OrangeEquipCollectParam() { this->Reset(); }

	void Reset()
	{
		seq_active_flag = 0;
		collect_succ_msg_send_flag = 0;
		opengame_act_reward_fetch_flag = 0;

		for (int i = 0; i < ARRAY_ITEM_COUNT(equip_slot); ++i)
		{
			equip_slot[i].Reset();
		}

		active_reward_fetch_flag = 0;
	}

	UInt32 seq_active_flag;								// 套装激活标记（已激活才可穿戴）
	UInt32 collect_succ_msg_send_flag;					// 集齐传闻发送标记
	UInt32 opengame_act_reward_fetch_flag;				// 开服七天活动奖励领取标记
	ItemDataWrapper equip_slot[ORANGE_EQUIP_COLLECT_SEQ_MAX_COUNT * ORANGE_EQUIP_COLLECT_SLOT_COUNT];	// 红装收集槽

	UInt32 active_reward_fetch_flag;							// 激活奖励标记
};

typedef char OrangeEquipCollectParamHex[sizeof(OrangeEquipCollectParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(OrangeEquipCollectParamHex) < 52000);

#pragma pack(pop)

#endif // __ORANGE_EQUIP_COLLECT_DEF_HPP__