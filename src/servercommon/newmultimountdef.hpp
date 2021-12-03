#ifndef __COUPLE_MOUNT_DEF_H__
#define __COUPLE_MOUNT_DEF_H__

#include "servercommon.h"

static const int COUPLEMOUNT_MAX_GRADE = 120;								//������
static const int COUPLEMOUNT_MAX_ID = 128;									//���ID
static const int COUPLEMOUNT_SPECIAL_IMG_MAX_ID_LIMIT = 31;                 //�û�ID���ֵ

#pragma pack(push, 4)

struct MultiMountParamInfo
{
	MultiMountParamInfo() { this->Reset(); }

	void Reset()
	{
		is_mount_active = 0;
		is_img_active = 0;
		reserve_sh = 0;

		grade_bless = 0;
		grade = 0;
	}

	char is_mount_active;													// �����Ƿ��Ѽ���
	char is_img_active;														// ���������Ƿ��Ѽ���
	short reserve_sh;

	short grade_bless;														// ����ף��ֵ
	short grade;															// ����
};

struct CoupleMountParam
{
	CoupleMountParam() { this->Reset(); }

	void Reset()
	{
		cur_select_mount_id = 0;
		cur_is_riding = 0;
		reserve_ch = 0;

		for (int mount_id = 0; mount_id < COUPLEMOUNT_MAX_ID; ++ mount_id)
		{
			mount_list[mount_id].Reset();
		}
		//�ĳ�Ĭ�ϲ������Ҫʹ�õ��߼���
		//mount_list[0].is_mount_active = 1; // ��һֻ����Ĭ�ϼ���

		special_img_active_flag = 0;
		cur_use_img_id = 0;
		reserve_sh = 0;

		memset(mount_special_image_level_list, 0, sizeof(mount_special_image_level_list));
		reserve_ch_1 = 0;
	}

	short cur_select_mount_id;												// ��ǰѡ�е�����
	char cur_is_riding;														// ��ǰ�Ƿ����������
	char reserve_ch;

	UNSTD_STATIC_CHECK(31 == COUPLEMOUNT_SPECIAL_IMG_MAX_ID_LIMIT);
	unsigned int special_img_active_flag;		// �������Ｄ��λ���, 0δ���1����
	short cur_use_img_id;				// ��ǰʹ�õ���������id,0��ʹ���κ���������
	short reserve_sh;

	char mount_special_image_level_list[COUPLEMOUNT_SPECIAL_IMG_MAX_ID_LIMIT];
	char reserve_ch_1;

	// ����id��1��ʼ���������鳤��ֱ�����������id�������±� = ����id - 1
	MultiMountParamInfo mount_list[COUPLEMOUNT_MAX_ID];
};

typedef char CoupleMountParamHex[sizeof(CoupleMountParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(CoupleMountParamHex) < 2560);

#pragma pack(pop)

#endif

