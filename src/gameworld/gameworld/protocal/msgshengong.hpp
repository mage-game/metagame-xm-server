#ifndef __MSG_SHENGONG_H__
#define __MSG_SHENGONG_H__

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/shengongdef.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	class SCShengongInfo				// 6450 ����Ϣ
	{
	public:
		SCShengongInfo();
		MessageHeader header;

		short star_level;															// �Ǽ�
		short shengong_level;														// �ȼ�
		short grade;																// ��
		short used_imageid;															// ʹ�õ�����
		int grade_bless_val;														// ����ף��ֵ
		short shuxingdan_count;														// ���Ե����� 
		short chengzhangdan_count;													// �ɳ�������
		unsigned char active_image_flag[ShengongParam::MAX_ACTIVE_IMAGE_BYTE];		// ����������б�
		unsigned char active_special_image_flag[ShengongParam::MAX_ACTIVE_SPECIAL_IMAGE_BYTE];	// ��������������б�
		unsigned int clear_upgrade_time;											// ���ף��ֵ��ʱ��
		int equip_skill_level;														// װ�����ܵȼ�

		short equip_level_list[ShengongParam::EQUIP_COUNT];							// װ����Ϣ
		short skill_level_list[ShengongParam::SKILL_COUNT];							// ���ܵȼ�
		char special_img_grade_list[ShengongParam::MAX_SPECIAL_IMAGE_COUNT];			// �����������
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class CSUpgradeShengong				// 6452 �����񹭽���
	{
	public:
		CSUpgradeShengong();
		MessageHeader header;

		short repeat_times;
		short auto_buy;
	};

	class CSUseShengongImage			// 6453 ����ʹ������
	{
	public:
		CSUseShengongImage();
		MessageHeader header;

		short reserve_sh;
		short image_id;
	};

	class CSShengongGetInfo				// 6454 ��������Ϣ
	{
	public:
		CSShengongGetInfo();	
		MessageHeader header;
	};


	class CSShengongUplevelEquip		// 6455 ������װ������
	{
	public:
		CSShengongUplevelEquip();	
		MessageHeader header;

		char equip_idx;
		short reserve;
	};

	class CSShengongSkillUplevelReq
	{
	public:
		CSShengongSkillUplevelReq();	// 6456 �񹭼�����������
		MessageHeader header;

		short skill_idx;
		short auto_buy;
	};

	class CSShengongSpecialImgUpgrade	// 6457 �������������
	{
	public:
		CSShengongSpecialImgUpgrade();
		MessageHeader header;

		short special_image_id;
		short reserve_sh;
	};


	class CSShengongUpStarLevel		// 6458�����Ǽ�����
	{
	public:
		CSShengongUpStarLevel();
		MessageHeader header;

		short stuff_index;
		short is_auto_buy;

		int loop_times;
	};

	class CSUnUseShengongImage			// 6459 ����ȡ��ʹ������
	{
	public:
		CSUnUseShengongImage();
		MessageHeader header;

		short image_id;
		short reserve_sh;
	};
}

#pragma pack(pop)

#endif // 

