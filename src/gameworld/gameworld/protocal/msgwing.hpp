#ifndef __MSG_WING_H__
#define __MSG_WING_H__

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/wingdef.hpp"
#include "config/shuxingdanconfig.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	class SCWingInfo				// 4703 ������Ϣ
	{
	public:
		SCWingInfo();
		MessageHeader header;

		short wing_level;															// �ȼ�
		short grade;																// ��
		short star_level;															// �Ǽ�
		short used_imageid;															// ʹ�õ�����
		unsigned short shuxingdan_list[SHUXINGDAN_MAX_TYPE];						// ���Ե�����
		int grade_bless_val;														// ����ף��ֵ
		unsigned char active_image_flag[WingParam::MAX_ACTIVE_IMAGE_BYTE];					// ����������б�
		unsigned char active_special_image_flag[WingParam::MAX_ACTIVE_SPECIAL_IMAGE_BYTE];	// ��������������б�
		unsigned int clear_upgrade_time;											// ���ף��ֵ��ʱ��
		short temporary_imageid;													// ��ǰʹ����ʱ����
		short temporary_imageid_has_select;											// ��ѡ������ʱ����
		unsigned int temporary_imageid_invalid_time;								// ��ʱ������Чʱ��
		int equip_skill_level;														// װ�����ܵȼ�

		short equip_level_list[WingParam::EQUIP_COUNT];								// װ����Ϣ
		short skill_level_list[WingParam::SKILL_COUNT];								// ���ܵȼ�
		char special_img_grade_list[WingParam::MAX_SPECIAL_IMAGE_COUNT];			// �����������
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class CSUpgradeWing				// 4757 �����������
	{
	public:
		CSUpgradeWing();
		MessageHeader header;

		short repeat_times;
		short auto_buy;
	};

	class CSUseWingImage			// 4774 ����ʹ������
	{
	public:
		CSUseWingImage();
		MessageHeader header;

		short is_temporary_image;
		short image_id;
	};

	class CSWingSpecialImgUpgrade	// 4789 ���������������
	{
	public:
		CSWingSpecialImgUpgrade();
		MessageHeader header;

		short special_image_id;
		short reserve_sh;
	};

	class CSWingUplevelEquip		// 4786 ��������װ������
	{
	public:
		CSWingUplevelEquip();	
		MessageHeader header;

		short equip_idx;			// װ������
		short reserve;				
	};

	class CSWingSkillUplevelReq
	{
	public:
		CSWingSkillUplevelReq();	// 4787 ��������������
		MessageHeader header;

		short skill_idx;
		short auto_buy;
	};

	class CSWingGetInfo				// 4775 ����������Ϣ
	{
	public:
		CSWingGetInfo();	
		MessageHeader header;
	};	
	
	class CSWingUpStarLevel		    // 4776 �������Ǽ�����
	{
	public:
		CSWingUpStarLevel();
		MessageHeader header;

		short stuff_index;
		short is_auto_buy;

		int loop_times;
	};
}

#pragma pack(pop)

#endif // 

