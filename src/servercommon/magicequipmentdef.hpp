#ifndef __MAGIC_EQUIPMENT_DEF_H__
#define __MAGIC_EQUIPMENT_DEF_H__

#include "servercommon.h"

static const int MAGIC_EQUIP_MAX_COUNT = 5;										// ħ��װ���������
static const int MAGIC_EQUIP_STONE_SLOT_COUNT = 6;								// ħ������Ƕ�ı�ʯ�׸���

#pragma pack(push, 4)

struct MagicEquipmentItem
{
	MagicEquipmentItem() { this->Reset(); }
	
	void Reset()
	{
		quality_progress = 0;
		quality_level = 0;
		strengthen_bless = 0;
		strengthen_level = 0;
		for (int index = 0; index < MAGIC_EQUIP_STONE_SLOT_COUNT; ++ index)
		{
			stone_list[index] = -1;
		}
		reserve_sh = 0;
		memset(reserve_ll, 0, sizeof(reserve_ll));
	}

	bool IsActive()
	{
		return (quality_level > 0);
	}

	void Active()
	{
		quality_level = 1;
	}

	short quality_progress;														// ���ɽ���
	short quality_level;														// Ʒ�ʵȼ�

	int strengthen_bless;														// ǿ��ף��ֵ
	short strengthen_level;														// ǿ���ȼ�
	short stone_list[MAGIC_EQUIP_STONE_SLOT_COUNT];								// ��Ƕ��ħʯ�б�
	short reserve_sh;

	long long reserve_ll[8];
};

struct MagicEquipmentParam
{
	MagicEquipmentParam() { this->Reset(); }

	void Reset()
	{
		for (int index = 0; index < MAGIC_EQUIP_MAX_COUNT; ++ index)
		{
			equip_list[index].Reset();
		}
	}

	MagicEquipmentItem equip_list[MAGIC_EQUIP_MAX_COUNT];
};

typedef char MagicEquipmentParamHex[sizeof(MagicEquipmentParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(MagicEquipmentParamHex) < 1024);

#pragma pack(pop)

#endif

