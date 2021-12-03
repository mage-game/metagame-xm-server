#ifndef __MSG_MASK_H__
#define __MSG_MASK_H__

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/struct/maskparam.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	class SCMaskInfo																// 8760 �����Ϣ
	{
	public:
		SCMaskInfo();
		MessageHeader header;

		short mask_level;															// �ȼ�
		short grade;																// ��
		short star_level;															// �Ǽ�
		short used_imageid;															// ʹ�õ�����
		int grade_bless_val;														// ����ף��ֵ
		unsigned char active_image_flag[MaskParam::MAX_ACTIVE_IMAGE_BYTE];			// ����������б�
		unsigned char active_special_image_flag[MaskParam::MAX_ACTIVE_SPECIAL_IMAGE_BYTE];	// ��������������б�
		unsigned int clear_upgrade_time;											// ���ף��ֵ��ʱ��
		short temporary_imageid;													// ��ǰʹ����ʱ����
		short temporary_imageid_has_select;											// ��ѡ������ʱ����
		unsigned int temporary_imageid_invalid_time;								// ��ʱ������Чʱ��

		unsigned short shuxingdan_list[SHUXINGDAN_MAX_TYPE];						// ���Ե�����
		short skill_level_list[MaskParam::SKILL_COUNT];								// ���ܵȼ�
		char special_img_grade_list[MaskParam::MAX_SPECIAL_IMAGE_COUNT];			// �����������

	};

	class SCMaskAppeChange														// 8761 �����۸ı�
	{
	public:
		SCMaskAppeChange();
		MessageHeader header;

		ObjID objid;
		unsigned short mask_appeid;
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	enum MASK_OPERA_TYPE
	{
		MASK_OPERA_TYPE_INFO = 0,								// ��Ϣ	
		MASK_OPERA_TYPE_UPGRADE,								// ���� p1:repeat_times p2:auto_buy
		MASK_OPERA_TYPE_SPECIAL_IMAGE_UPGRADE,					// ����������� p1:special_image_id
		MASK_OPERA_TYPE_USE_IMAGE,								// ʹ������ p1:image_id
		MASK_OPERA_TYPE_SKILL_UPGRADE,							// ���ܽ��� p1:skill_idx p2:is_auto_buy
	};

	class CSMaskOperaReq															// 8762 �������
	{
	public:
		CSMaskOperaReq();
		MessageHeader header;

		short opera_type;
		short param_1;
		short param_2;
		short param_3;
	};

}

#pragma pack(pop)

#endif // 

