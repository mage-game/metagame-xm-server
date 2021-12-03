#ifndef __MSG_TOUSHI_H__
#define __MSG_TOUSHI_H__

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/struct/toushiparam.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	class SCTouShiInfo																// 8740 ͷ����Ϣ
	{
	public:
		SCTouShiInfo();
		MessageHeader header;

		short toushi_level;															// �ȼ�
		short grade;																// ��
		short star_level;															// �Ǽ�
		short used_imageid;															// ʹ�õ�����
		int grade_bless_val;														// ����ף��ֵ
		unsigned char active_image_flag[TouShiParam::MAX_ACTIVE_IMAGE_BYTE];		// ����������б�
		unsigned char active_special_image_flag[TouShiParam::MAX_ACTIVE_SPECIAL_IMAGE_BYTE];	// ��������������б�
		unsigned int clear_upgrade_time;											// ���ף��ֵ��ʱ��
		short temporary_imageid;													// ��ǰʹ����ʱ����
		short temporary_imageid_has_select;											// ��ѡ������ʱ����
		unsigned int temporary_imageid_invalid_time;								// ��ʱ������Чʱ��

		unsigned short shuxingdan_list[SHUXINGDAN_MAX_TYPE];						// ���Ե�����
		short skill_level_list[TouShiParam::SKILL_COUNT];							// ���ܵȼ�
		char special_img_grade_list[TouShiParam::MAX_SPECIAL_IMAGE_COUNT];			// �����������

	};

	class SCTouShiAppeChange														// 8741 ͷ����۸ı�
	{
	public:
		SCTouShiAppeChange();
		MessageHeader header;

		ObjID objid;
		unsigned short toushi_appeid;
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	enum TOUSHI_OPERA_TYPE
	{
		TOUSHI_OPERA_TYPE_INFO = 0,								// ��Ϣ	
		TOUSHI_OPERA_TYPE_UPGRADE,								// ͷ�ν��� p1:repeat_times p2:auto_buy
		TOUSHI_OPERA_TYPE_SPECIAL_IMAGE_UPGRADE,				// ����������� p1:special_image_id
		TOUSHI_OPERA_TYPE_USE_IMAGE,							// ʹ������ p1:image_id
		TOUSHI_OPERA_TYPE_SKILL_UPGRADE,						// ���ܽ��� p1:skill_idx p2:is_auto_buy
	};

	class CSTouShiOperaReq															// 8742 �������
	{
	public:
		CSTouShiOperaReq();
		MessageHeader header;

		short opera_type;
		short param_1;
		short param_2;
		short param_3;
	};
}

#pragma pack(pop)

#endif // 

