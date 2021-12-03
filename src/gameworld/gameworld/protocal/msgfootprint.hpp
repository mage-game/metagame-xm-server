#ifndef __MSG_FOOTPRINT_H__
#define __MSG_FOOTPRINT_H__

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/footprintdef.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	class SCFootprintInfo				//  6520�㼣��Ϣ
	{
	public:
		SCFootprintInfo();
		MessageHeader header;

		short footprint_level;														// �ȼ�
		short grade;																// ��
		short star_level;															// �Ǽ�
		short used_imageid;															// ʹ�õ�����
		short shuxingdan_count;														// ���Ե����� 
		short chengzhangdan_count;													// �ɳ�������
		int grade_bless_val;														// ����ף��ֵ
		unsigned char active_image_flag[FootprintParam::MAX_ACTIVE_IMAGE_BYTE];		// ����������б�
		unsigned char active_special_image_flag[FootprintParam::MAX_ACTIVE_SPECIAL_IMAGE_BYTE];		// ��������������б�
		unsigned int clear_upgrade_time;											// ���ף��ֵ��ʱ��
		int equip_skill_level;														// װ�����ܵȼ�

		short equip_level_list[FootprintParam::EQUIP_COUNT];						// װ����Ϣ
		short skill_level_list[FootprintParam::SKILL_COUNT];						// ���ܵȼ�
		char special_img_grade_list[FootprintParam::MAX_SPECIAL_IMAGE_COUNT];		// �����������
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	enum FOOTPRINT_OPERATE_TYPE
	{
		FOOTPRINT_OPERATE_TYPE_INFO_REQ = 0,			// ������Ϣ
		FOOTPRINT_OPERATE_TYPE_UP_GRADE,				// ������� param_1=>repeat_times  param_2=>auto_buy
		FOOTPRINT_OPERATE_TYPE_USE_IMAGE,				// ����ʹ������ param_1=>image_id
		FOOTPRINT_OPERATE_TYPE_UP_LEVEL_EQUIP,			// ��������װ�� param_1=>equip_idx
		FOOTPRINT_OPERATE_TYPE_UP_STAR,					// �������� param_1=>stuff_index param_2=>is_auto_buy param_3=>loop_times
		FOOTPRINT_OPERATE_TYPE_UP_LEVEL_SKILL,			// ������������ param_1=>skill_idx param_2=>auto_buy
		FOOTPRINT_OPERATE_TYPE_UP_SPECIAL_IMAGE,		// ����������������� param_1=>special_image_id
	};

	class CSFootprintOperate			//  6521�㼣����
	{
	public:
		CSFootprintOperate();
		MessageHeader header;

		short operate_type;
		short param_1;
		short param_2;
		short param_3;
	};
}

#pragma pack(pop)

#endif // 

