#ifndef __SHENSHOU_DEF_HPP__
#define __SHENSHOU_DEF_HPP__

#include "servercommon.h"

static const int SHENSHOU_MAX_ID = 32;												// �������ID
static const int SHENSHOU_MAX_EQUIP_SLOT_INDEX = 4;									// ����װ�����λindex
static const int SHENSHOU_MAX_EQUIP_ATTR_COUNT = 3;									// ����װ�����������Ը���
static const int SHENSHOU_MAX_BACKPACK_COUNT = 200;									// ���ޱ�����������
static const int SHENSHOU_MAX_DRAW_TIMES = 14;										// �������齱����
static const int SHENSHOU_MAX_RERFESH_ITEM_COUNT = 14;								// ����ˢ��һ����Ʒ����

enum SHENSHOU_ATTR_TYPE																// װ��������������
{
	SHENSHOU_ATTR_TYPE_INVALID_MIN = 0,

	SHENSHOU_ATTR_TYPE_PER_HP,														// ����������ֱ�
	SHENSHOU_ATTR_TYPE_PER_GONGJI,													// ���޹�����ֱ�
	SHENSHOU_ATTR_TYPE_PER_FANGYU,													// ���������ֱ�
	SHENSHOU_ATTR_TYPE_PER_SHANBI,													// ����������ֱ�
	SHENSHOU_ATTR_TYPE_PER_MINGZHONG,												// ����������ֱ�
	SHENSHOU_ATTR_TYPE_PER_BAOJI,													// ���ޱ�����ֱ�
	SHENSHOU_ATTR_TYPE_PER_JIANREN,													// ���޿�����ֱ�
	SHENSHOU_ATTR_TYPE_PER_GUSHANG,													// ���޹�����ֱ�
	SHENSHOU_ATTR_TYPE_PER_JIANSHANG,												// ���޼�����ֱ�
	SHENSHOU_ATTR_TYPE_MAX_HP,														// �����̶�ֵ
	SHENSHOU_ATTR_TYPE_GONGJI,														// �����̶�ֵ
	SHENSHOU_ATTR_TYPE_FANGYU,														// ����̶�ֵ
	SHENSHOU_ATTR_TYPE_SHANBI,														// ���̶ܹ�ֵ
	SHENSHOU_ATTR_TYPE_MINGZHONG,													// ���й̶�ֵ
	SHENSHOU_ATTR_TYPE_BAOJI,														// �����̶�ֵ
	SHENSHOU_ATTR_TYPE_JIANREN,														// �����̶�ֵ
	SHENSHOU_ATTR_TYPE_GUSHANG,														// ���˹̶�ֵ											
	SHENSHOU_ATTR_TYPE_JIANSHANG,													// ���˹̶�ֵ

	SHENSHOU_ATTR_TYPE_INVALID_MAX,
};

enum SHENSHOU_SKILL_TYPE
{
	SHENSHOU_SKILL_TYPE_MIN_INVALID = 0,

	SHENSHOU_SKILL_TYPE_1,															// ���׷���
	SHENSHOU_SKILL_TYPE_2,															// ������Ϣ
	SHENSHOU_SKILL_TYPE_3,															// �б�����
	SHENSHOU_SKILL_TYPE_4,															// �������
	SHENSHOU_SKILL_TYPE_5,															// ��������
	SHENSHOU_SKILL_TYPE_6,															// ��������
	SHENSHOU_SKILL_TYPE_7,															// ������
	SHENSHOU_SKILL_TYPE_8,															// ����ӻ�
	SHENSHOU_SKILL_TYPE_9,															// �����ٷ�
	SHENSHOU_SKILL_TYPE_10,															// �粽����
	SHENSHOU_SKILL_TYPE_11,															// ������ȡ
	SHENSHOU_SKILL_TYPE_12,															// ����֮ŭ
	SHENSHOU_SKILL_TYPE_13,															// ������
	SHENSHOU_SKILL_TYPE_14,															// ������ɽ
	SHENSHOU_SKILL_TYPE_15,															// �޾�ŭ��
	SHENSHOU_SKILL_TYPE_16,															// ���ֶ���
	SHENSHOU_SKILL_TYPE_17,															// ��������
	SHENSHOU_SKILL_TYPE_18,															// �ܵз�â
	SHENSHOU_SKILL_TYPE_19,															// ս�޲�ʤ
	SHENSHOU_SKILL_TYPE_20,															// �Ʋ��ɵ�
	SHENSHOU_SKILL_TYPE_21,															// ����ǧ��

	SHENSHOU_SKILL_TYPE_22,															// ȫ��������
	SHENSHOU_SKILL_TYPE_23,															// ȫ����������
	SHENSHOU_SKILL_TYPE_24,															// ������������
	SHENSHOU_SKILL_TYPE_25,															// ���ܼ�������
	SHENSHOU_SKILL_TYPE_26,															// �˺��ӳ�����
	SHENSHOU_SKILL_TYPE_27,															// �˺���������

	SHENSHOU_SKILL_TYPE_28,															// �񵲼���
	SHENSHOU_SKILL_TYPE_29,															// ���м���
	SHENSHOU_SKILL_TYPE_30,															// �˺��ӳ�

	SHENSHOU_SKILL_TYPE_COUNT,
};

#pragma pack(push, 4)

struct ShenshouBackpackItem
{
	struct AttrItem
	{
		short attr_type;
		char is_start;
		char reserve_sh;
		int attr_value;
	};

	ShenshouBackpackItem() { this->Reset(); }

	void Reset()
	{
		item_id = 0;
		strength_level = 0;
		shuliandu = 0;
		memset(attr_list, 0, sizeof(attr_list));
	}

	short item_id;
	short strength_level;						// ǿ���ȼ�
	int shuliandu;								// ������

	AttrItem attr_list[SHENSHOU_MAX_EQUIP_ATTR_COUNT];
};

struct ShenshouInfo
{
	ShenshouInfo() { this->Reset(); }

	void Reset()
	{
		has_zhuzhan = 0;
		reserve_ch = 0;
		reserve_sh = 0;

		for (int index = 0; index <= SHENSHOU_MAX_EQUIP_SLOT_INDEX; ++ index)
		{
			equip_list[index].Reset();
		}
	}

	char has_zhuzhan;			// �Ƿ���ս��
	char reserve_ch;
	short reserve_sh;
	ShenshouBackpackItem equip_list[SHENSHOU_MAX_EQUIP_SLOT_INDEX + 1];
};

struct HuanlingListInfo
{
	HuanlingListInfo() : huanling_seq(0), draw(0), pool_index(0)
	{}

	short huanling_seq;
	char draw;
	char pool_index;
};

struct ShenshouParam
{
	ShenshouParam() { this->Reset(); }

	void Reset()
	{
		cur_huanling_draw_times = 0;
		extra_zhuzhan_count = 0;
		huanling_score = 0;
		refresh_huanling_times = 0;	
		first_huanling = 0;
		reserve_ch = 0;
		reserve_sh = 0;
		for (int i = 0; i <= SHENSHOU_MAX_ID; ++ i)
		{
			shenshou_list[i].Reset();
		}
// 		for (int i = 0; i < SHENSHOU_MAX_BACKPACK_COUNT; ++i)
// 		{
// 			backpack_item_list[i].Reset();
// 		}

		memset(huanling_list, 0, sizeof(huanling_list));
	}

	short extra_zhuzhan_count;				// ������սλ����Ҫ����
	short cur_huanling_draw_times;			// ��ǰ����鵽�˵ڼ���

	int huanling_score;						// �������
	int refresh_huanling_times;				// ����ˢ�´���

	char first_huanling;					// �Ƿ�Ϊ��һ�λ���
	char reserve_ch;
	short reserve_sh;

	ShenshouInfo shenshou_list[SHENSHOU_MAX_ID + 1];
	HuanlingListInfo huanling_list[SHENSHOU_MAX_RERFESH_ITEM_COUNT];
	//ShenshouBackpackItem backpack_item_list[SHENSHOU_MAX_BACKPACK_COUNT];
};

typedef char ShenshouParamHex[sizeof(ShenshouParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(ShenshouParamHex) < 15360);

#pragma pack(pop)
#endif
