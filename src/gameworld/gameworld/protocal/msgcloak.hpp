#ifndef __MSG_CLOAK_H__
#define __MSG_CLOAK_H__

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/cloakdef.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	class SCCloakInfo				// 6525������Ϣ
	{
	public:
		SCCloakInfo();
		MessageHeader header;

		int cloak_level;															// �ȼ�
		int cur_exp;																// ��ǰ����
		int used_imageid;															// ʹ�õ�����

		int active_image_flag;														// ����������б�  & (1 << imageid) != 0���򼤻�imageid
		unsigned long long active_special_image_flag;								// ��������������б�
		int equip_skill_level;														// װ�����ܵȼ�

		unsigned short shuxingdan_list[CloakParam::SHUXINGDAN_MAX_TYPE];			// ���Ե�����
		short equip_level_list[CloakParam::EQUIP_COUNT];							// װ����Ϣ
		short skill_level_list[CloakParam::SKILL_COUNT];							// ���ܵȼ�
		char special_img_grade_list[CloakParam::MAX_SPECIAL_IMAGE_ID + 1];			// �����������
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	enum CLOAK_OPERATE_TYPE
	{
		CLOAK_OPERATE_TYPE_INFO_REQ = 0,			// ������Ϣ
		CLOAK_OPERATE_TYPE_UP_LEVEL,				// �������� param_1=>stuff_index param_2=>is_auto_buy param_3=>loop_times
		CLOAK_OPERATE_TYPE_USE_IMAGE,				// ����ʹ������ param_1=>image_id

		CLOAK_OPERATE_TYPE_UP_SPECIAL_IMAGE,		// ����������������� param_1=>special_image_id
		CLOAK_OPERATE_TYPE_UP_LEVEL_EQUIP,			// ��������װ�� param_1=>equip_idx
		CLOAK_OPERATE_TYPE_UP_LEVEL_SKILL,			// ������������ param_1=>skill_idx param_2=>auto_buy
		
	};

	class CSCloakOperate			//  6526 �������
	{
	public:
		CSCloakOperate();
		MessageHeader header;

		short operate_type;
		short param_1;
		short param_2;
		short param_3;
	};
}

#pragma pack(pop)

#endif // 

