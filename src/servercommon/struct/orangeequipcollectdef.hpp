// ��װ�ռ���ض���
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

static constexpr int ORANGE_EQUIP_COLLECT_SLOT_COUNT = 12;			// ��װ�ռ�����
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
	int item_count = 0;		// ����
	int stars = 0;			// �Ǽ�����Ʒ����Ʒ������
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

	UInt32 seq_active_flag;								// ��װ�����ǣ��Ѽ���ſɴ�����
	UInt32 collect_succ_msg_send_flag;					// ���봫�ŷ��ͱ��
	UInt32 opengame_act_reward_fetch_flag;				// ��������������ȡ���
	ItemDataWrapper equip_slot[ORANGE_EQUIP_COLLECT_SEQ_MAX_COUNT * ORANGE_EQUIP_COLLECT_SLOT_COUNT];	// ��װ�ռ���

	UInt32 active_reward_fetch_flag;							// ��������
};

typedef char OrangeEquipCollectParamHex[sizeof(OrangeEquipCollectParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(OrangeEquipCollectParamHex) < 52000);

#pragma pack(pop)

#endif // __ORANGE_EQUIP_COLLECT_DEF_HPP__