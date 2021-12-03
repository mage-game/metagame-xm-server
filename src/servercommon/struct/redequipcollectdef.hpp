// ��װ�ռ���ض���
#ifndef __RED_EQUIP_COLLECT_DEF_HPP__
#define __RED_EQUIP_COLLECT_DEF_HPP__

#include "servercommon/servercommon.h"
#include "servercommon/struct/itemlistparam.h"

static constexpr int RED_EQUIP_COLLECT_SEQ_MAX_COUNT = 18;
static constexpr int RED_EQUIP_COLLECT_PRO_MAX = 5;

inline bool IsValidRedEquipCollectSeq(int seq)
{
	return seq >= 0 && seq < RED_EQUIP_COLLECT_SEQ_MAX_COUNT;
}
inline bool IsValidRedEquipCollectPro(int pro)
{
	return pro > PROF_TYPE_INVALID && pro < PROF_TYPE_PROF_NO_LIMIT;
}

static constexpr int RED_EQUIP_COLLECT_SLOT_COUNT = 12;			// ��װ�ռ�����
inline bool IsValidRedEquipCollectSlot(int slot)
{
	return slot >= 0 && slot < RED_EQUIP_COLLECT_SLOT_COUNT;
}

static constexpr int RED_EQUIP_COLLECT_ACT_REWARD_MAX_COUNT = 2;
inline bool IsValidRedEquipActRewardSeq(int seq)
{
	return seq >= 0 && seq < RED_EQUIP_COLLECT_ACT_REWARD_MAX_COUNT;
}

#pragma pack(push, 4)

struct RedEquipCollectInfo
{
	int item_count = 0;		// ����
	int stars = 0;			// �Ǽ�����Ʒ����Ʒ������
};

struct RedEquipCollectParam
{
	RedEquipCollectParam() { this->Reset(); }

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
		check_level = 0;
		reserve_ch = 0;
		reserve_sh = 0;
	}

	UInt32 seq_active_flag;								// ��װ�����ǣ��Ѽ���ſɴ�����
	UInt32 collect_succ_msg_send_flag;					// ���봫�ŷ��ͱ��
	UInt32 opengame_act_reward_fetch_flag;				// ��������������ȡ���
	ItemDataWrapper equip_slot[RED_EQUIP_COLLECT_SEQ_MAX_COUNT * RED_EQUIP_COLLECT_SLOT_COUNT];	// ��װ�ռ���

	UInt32 active_reward_fetch_flag;							// ��������
	char check_level;
	char reserve_ch;
	short reserve_sh;
};

#pragma pack(pop)

typedef char RedEquipCollectParamHex[sizeof(RedEquipCollectParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(RedEquipCollectParamHex) < 52000);

#endif // __RED_EQUIP_COLLECT_DEF_HPP__