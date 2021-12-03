#ifndef __SEAL_PARAM_HPP__
#define __SEAL_PARAM_HPP__

#include "servercommon/servercommon.h"

static const int SEAL_BACKPACK_MAX_GRIDS = 200;	// ���ⱳ��������
static const int SEAL_SLOT_MAX_NUM = 10;		// ʥӡ�������
static const int SEAL_ORDER_MAX_NUM = 9;		// ʥӡ������
static const int SEAL_SOUL_MIN_TYPE = 1;		// ��Сʥ������
static const int SEAL_SOUL_MAX_TYPE = 3;		// ���ʥ������
static const int SEAL_ATTR_MAX_NUM = 18;		// ʥӡ���������
static const int SEAL_SUIT_MIN_TYPE = 1;		// ʥӡ��װ��С����
static const int SEAL_SUIT_MAX_TYPE = 18;		// ʥӡ��װ�������

enum SEAL_COLOR_KIND
{
	RUNE_COLOR_KIND_GREEN = 0,					// ��ɫ
	RUNE_COLOR_KIND_BLUE,						// ��ɫ
	RUNE_COLOR_KIND_PURPLE,						// ��ɫ
	RUNE_COLOR_KIND_ORANGE,						// ��ɫ
	RUNE_COLOR_KIND_RED,						// ��ɫ
	RUNE_COLOR_KIND_PINK,						// ��ɫ

	RUNE_COLOR_KIND_MAX,
};

static const int SEAL_COLOR_MAX_TYPE = RUNE_COLOR_KIND_MAX;		// ʥӡ�����ɫ

enum SEAL_SUIT_SAME_ORDER_TYPE // ��װ��ͬ����������
{
	SEAL_SUIT_SAME_ORDER_TYPE_ONE = 1,
	SEAL_SUIT_SAME_ORDER_TYPE_TWO = 2,
	SEAL_SUIT_SAME_ORDER_TYPE_THREE = 3,
	SEAL_SUIT_SAME_ORDER_TYPE_FOUR = 4,
	SEAL_SUIT_SAME_ORDER_TYPE_SIX = 6,
	SEAL_SUIT_SAME_ORDER_TYPE_EIGHT = 8,
	SEAL_SUIT_SAME_ORDER_TYPE_TEN = 10,
};

enum SEAL_TYPE
{
	SEAL_TYPE_JINGHUA = 0,														//����
	SEAL_TYPE_WUQI = 1,															//����
	SEAL_TYPE_XIEZI = 2,														//Ь��
	SEAL_TYPE_HUSHOU = 3,														//����
	SEAL_TYPE_KUZI = 4,															//����
	SEAL_TYPE_YIFU = 5,															//�·�
	SEAL_TYPE_TOUKUI = 6,														//ͷ��
	SEAL_TYPE_JIEZHI = 7,														//��ָ
	SEAL_TYPE_XIANGLIAN = 8,													//����
	SEAL_TYPE_HUFU = 9,															//����
	SEAL_TYPE_HEXIN = 10,														//����

	SEAL_TYPE_MAX,																//����������
};

#pragma pack(push, 4)

struct SealItem
{
	void Reset()
	{
		slot_index = -1;
		order = -1;
		color = -1;
		level = -1;
		item_id = 0;
		is_bind = 0;
		reserve_ch = 0;
	}

	short slot_index;		// ʥӡ������
	short order;			// ʥӡ����
	short color;			// ʥӡ��ɫ
	short level;			// ʥӡ�ȼ�
	unsigned short item_id;	// ʥӡid
	unsigned char is_bind;
	unsigned char reserve_ch;

	bool IsEmpty() const { return slot_index < 0 || order < 0 || color < 0 || level < 0 || item_id <= 0; }
};

struct SealParam
{
	SealParam() { this->Reset(); }

	void Reset()
	{
		for (int i = 0; i < SEAL_SLOT_MAX_NUM; ++i)
		{
			slot_seal_list[i].Reset();
		}

// 		for (int i = 0; i < SEAL_BACKPACK_MAX_GRIDS; ++i)
// 		{
// 			backpack_seal_list[i].Reset();
// 		}

		hun_score = 0;
		slot_open_flag = 0;
		memset(soul_list, 0, sizeof(soul_list));
	}

	bool IsActivateSeal(int slot_index)
	{
		return 0 != (slot_open_flag & (1 << slot_index));
	}

	void ActivateSeal(int slot_index)
	{
		slot_open_flag |= (1 << slot_index);
	}

	UNSTD_STATIC_CHECK(10 == SEAL_SLOT_MAX_NUM);
	UNSTD_STATIC_CHECK(200 == SEAL_BACKPACK_MAX_GRIDS);
	UNSTD_STATIC_CHECK(3 == SEAL_SOUL_MAX_TYPE);

	int hun_score;															// �����
	int soul_list[SEAL_SOUL_MAX_TYPE];										// ʥ���б�

	int slot_open_flag;														// ʥӡ�׿�����־
	SealItem slot_seal_list[SEAL_SLOT_MAX_NUM];								// ����ʥӡ�б�
	//SealItem backpack_seal_list[SEAL_BACKPACK_MAX_GRIDS];					// ����ʥӡ�б�
};

#pragma pack(pop)

typedef char SealParamHex[sizeof(SealParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(SealParamHex) < 512);

#endif