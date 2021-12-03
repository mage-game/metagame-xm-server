#ifndef __MSG_HALO_H__
#define __MSG_HALO_H__

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/halodef.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	class SCHaloInfo				// 6400 �⻷��Ϣ
	{
	public:
		SCHaloInfo();
		MessageHeader header;

		short halo_level;															// �ȼ�
		short grade;																// ��
		short star_level;															// �Ǽ�
		short used_imageid;															// ʹ�õ�����
		short shuxingdan_count;														// ���Ե����� 
		short chengzhangdan_count;													// �ɳ�������
		int grade_bless_val;														// ����ף��ֵ
		unsigned char active_image_flag[HaloParam::MAX_ACTIVE_IMAGE_BYTE];			// ����������б�  & (1 << imageid) != 0���򼤻�imageid
		unsigned char active_special_image_flag[HaloParam::MAX_ACTIVE_SPECIAL_IMAGE_BYTE];			// ��������������б�
		unsigned int clear_upgrade_time;											// ���ף��ֵ��ʱ��
		unsigned int reward_limit_time;												// ���ؽ��׵���ʱ��
		int equip_skill_level;														// װ�����ܵȼ�

		short equip_level_list[HaloParam::EQUIP_COUNT];								// װ����Ϣ
		short skill_level_list[HaloParam::SKILL_COUNT];								// ���ܵȼ�
		char special_img_grade_list[HaloParam::MAX_SPECIAL_IMAGE_COUNT];			// �����������
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class CSUpgradeHalo				// 6402 ����⻷����
	{
	public:
		CSUpgradeHalo();
		MessageHeader header;

		short repeat_times;
		short auto_buy;
	};

	class CSUseHaloImage			// 6403 ����ʹ������
	{
	public:
		CSUseHaloImage();
		MessageHeader header;

		short reserve_sh;
		short image_id;
	};

	class CSHaloGetInfo				// 6404 ����⻷��Ϣ
	{
	public:
		CSHaloGetInfo();	
		MessageHeader header;
	};

	class CSHaloUplevelEquip		// 6405 �⻷����װ������
	{
	public:
		CSHaloUplevelEquip();	
		MessageHeader header;

		short equip_idx;			// װ������
		short reserve;				
	};

	class CSHaloSkillUplevelReq
	{
	public:
		CSHaloSkillUplevelReq();	// 6406 �⻷������������
		MessageHeader header;

		short skill_idx;
		short auto_buy;
	};

	class CSHaloSpecialImgUpgrade	// 6407 �⻷�����������
	{
	public:
		CSHaloSpecialImgUpgrade();
		MessageHeader header;

		short special_image_id;
		short reserve_sh;
	};

	class CSHaloUpStarLevel		// 6408 �⻷���Ǽ�����
	{
	public:
		CSHaloUpStarLevel();
		MessageHeader header;

		short stuff_index;
		short is_auto_buy;

		int loop_times;
	};
}

#pragma pack(pop)

#endif // 

