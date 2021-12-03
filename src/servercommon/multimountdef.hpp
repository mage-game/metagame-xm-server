#ifndef __MULTI_MOUNT_DEF_H__
#define __MULTI_MOUNT_DEF_H__

#include "servercommon.h"

static const int MULTIMOUNT_MAX_GRADE = 20;									//������
static const int MULTIMOUNT_MAX_ID = 5;										//���ID
static const int MULTIMOUNT_SPECIAL_IMG_MAX_ID_LIMIT = 31;                  //�û�ID���ֵ
static const int MULTIMOUNT_EQUIP_TYPE_NUM = 8;								//װ������

#pragma pack(push, 4)

struct MultiMountParamItem
{
	MultiMountParamItem() { this->Reset(); }

	void Reset()
	{
		is_mount_active = 0;
		is_img_active = 0;
		reserve_sh = 0;

		grade_bless = 0;
		grade = 0;

		memset(reserve_ll, 0, sizeof(reserve_ll));
	}

	char is_mount_active;													// �����Ƿ��Ѽ���
	char is_img_active;														// ���������Ƿ��Ѽ���
	short reserve_sh;

	short grade_bless;														// ����ף��ֵ
	short grade;															// ����

	long long reserve_ll[3];
};

struct MultiMountParam
{
	MultiMountParam() { this->Reset(); }

	void Reset()
	{
		cur_select_mount_id = 0;
		cur_is_riding = 0;
		reserve_ch = 0;

		for (int mount_id = 0; mount_id < MULTIMOUNT_MAX_ID; ++ mount_id)
		{
			mount_list[mount_id].Reset();
		}

		mount_list[0].is_mount_active = 1; // ��һֻ����Ĭ�ϼ���

		special_img_active_flag = 0;
		cur_use_img_id = 0;
		reserve_sh = 0;

		memset(equip_level_list, 0, sizeof(equip_level_list));

		memset(reserve_ll_list, 0, sizeof(reserve_ll_list));
	}

	short cur_select_mount_id;												// ��ǰѡ�е�����
	char cur_is_riding;														// ��ǰ�Ƿ����������
	char reserve_ch;

	long long reserve_ll_list[5]; 

	// ����id��1��ʼ���������鳤��ֱ�����������id�������±� = ����id - 1
	MultiMountParamItem mount_list[MULTIMOUNT_MAX_ID];

	UNSTD_STATIC_CHECK(31 == MULTIMOUNT_SPECIAL_IMG_MAX_ID_LIMIT);
	int special_img_active_flag;		// �������Ｄ��λ���, 0δ���1����
	short cur_use_img_id;				// ��ǰʹ�õ���������id,0��ʹ���κ���������
	short reserve_sh;

	UNSTD_STATIC_CHECK(8 == MULTIMOUNT_EQUIP_TYPE_NUM);
	short equip_level_list[MULTIMOUNT_EQUIP_TYPE_NUM];						// ����װ���ȼ��б�
};

typedef char MultiMountParamHex[sizeof(MultiMountParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(MultiMountParamHex) < 1024);

#pragma pack(pop)

#endif

