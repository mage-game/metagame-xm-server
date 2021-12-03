#ifndef __MAGIC_CARD_DEF_H__
#define __MAGIC_CARD_DEF_H__

#include "servercommon.h"

static const int MAGIC_CARD_SLOT_TYPE_LIMIT_COUNT = 4;							// ħ��λ�������������
static const int MAGIC_CARD_MAX_LIMIT_COUNT = 27;								// ħ�����������
static const int MAGIC_CARD_CHOU_CARD_LIMIT_REWARD_COUNT = 16;					// ħ���鿨��Ʒ�������

static const int MAGIC_CARD_LIMIT_STRENGTH_LEVEL_MAX = 10;						// ħ�����ǿ���ȼ�

enum MAGIC_CARD_COLOR_TYPE
{
	MAGIC_CARD_COLOR_TYPE_BLUE = 0,											// ��ɫ
	MAGIC_CARD_COLOR_TYPE_PURPLE,											// ��ɫ
	MAGIC_CARD_COLOR_TYPE_ORANGE,											// ��ɫ
	MAGIC_CARD_COLOR_TYPE_RED,												// ��ɫ

	MAGIC_CARD_COLOR_TYPE_COLOR_COUNT,
};

#pragma pack(push, 4)

struct MagicCardSlotItem
{
	MagicCardSlotItem() : card_id(0), strength_level(0), exp(0) {}

	short card_id;
	short strength_level;
	int exp;
};

struct MagicCardParam
{
	MagicCardParam() { this->Reset(); }

	void Reset()
	{
		today_purple_free_chou_card_times = 0;
		all_card_has_exchange_flag = 0;

		memset(all_card_num_list, 0, sizeof(all_card_num_list));
		memset(card_slot_list, 0, sizeof(card_slot_list));
	}

	int today_purple_free_chou_card_times;									// �Ͽ��齱��������ѳ鿨����

	UNSTD_STATIC_CHECK(MAGIC_CARD_MAX_LIMIT_COUNT < 31);
	int all_card_has_exchange_flag;											// ���п����Ƿ񱻶һ����ı��

	UNSTD_STATIC_CHECK(MAGIC_CARD_MAX_LIMIT_COUNT == 27);
	unsigned short all_card_num_list[MAGIC_CARD_MAX_LIMIT_COUNT + 1];

	UNSTD_STATIC_CHECK(MAGIC_CARD_SLOT_TYPE_LIMIT_COUNT == 4);				// ����λ������ֻ��Ϊ4�����������޷���չ
	MagicCardSlotItem card_slot_list[MAGIC_CARD_COLOR_TYPE_COLOR_COUNT][MAGIC_CARD_SLOT_TYPE_LIMIT_COUNT]; // ������ɫ�����б�	
};

typedef char MagicCardParamHex[sizeof(MagicCardParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(MagicCardParamHex) < 512);

#pragma pack(pop)

#endif

