#ifndef __MSG_SHENYI_H__
#define __MSG_SHENYI_H__

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/shenyidef.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	class SCShenyiInfo				// 6500 ������Ϣ
	{
	public:
		SCShenyiInfo();
		MessageHeader header;

		short star_level;															// �Ǽ�
		short shenyi_level;															// �ȼ�
		short grade;																// ��
		short used_imageid;															// ʹ�õ�����
		int grade_bless_val;														// ����ף��ֵ
		short shuxingdan_count;														// ���Ե����� 
		short chengzhangdan_count;													// �ɳ�������
		unsigned char active_image_flag[ShenyiParam::MAX_ACTIVE_IMAGE_BYTE];					// ����������б�  & (1 << imageid) != 0���򼤻�imageid
		unsigned char active_special_image_flag[ShenyiParam::MAX_ACTIVE_SPECIAL_IMAGE_BYTE];	// ��������������б�
		unsigned int clear_upgrade_time;											// ���ף��ֵ��ʱ��
		int equip_skill_level;														// װ�����ܵȼ�

		short equip_level_list[ShenyiParam::EQUIP_COUNT];							// װ����Ϣ
		short skill_level_list[ShenyiParam::SKILL_COUNT];							// ���ܵȼ�
		char special_img_grade_list[ShenyiParam::MAX_SPECIAL_IMAGE_COUNT];			// �����������
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class CSUpgradeShenyi				// 6502 �����������
	{
	public:
		CSUpgradeShenyi();
		MessageHeader header;

		short repeat_times;
		short auto_buy;
	};

	class CSUseShenyiImage			// 6503 ����ʹ������
	{
	public:
		CSUseShenyiImage();
		MessageHeader header;

		short reserve_sh;
		short image_id;
	};

	class CSShenyiGetInfo				// 6504 ����������Ϣ
	{
	public:
		CSShenyiGetInfo();	
		MessageHeader header;
	};

	class CSShenyiUplevelEquip		// 6505 ��������װ������
	{
	public:
		CSShenyiUplevelEquip();	
		MessageHeader header;

		short equip_idx;
		short reserve;
	};

	class CSShenyiSkillUplevelReq
	{
	public:
		CSShenyiSkillUplevelReq();	// 6506 ��������������
		MessageHeader header;

		short skill_idx;
		short auto_buy;
	};

	class CSShenyiSpecialImgUpgrade	// 6507 ���������������
	{
	public:
		CSShenyiSpecialImgUpgrade();
		MessageHeader header;

		short special_image_id;
		short reserve_sh;
	};

	class CSShenyiUpStarLevel	// 6508�������Ǽ�����
	{
	public:
		CSShenyiUpStarLevel();
		MessageHeader header;

		short stuff_index;
		short is_auto_buy;

		int loop_times;
	};

	class CSUnUseShenyiImage			// 6509 ����ȡ��ʹ������
	{
	public:
		CSUnUseShenyiImage();
		MessageHeader header;

		short image_id;
		short reserve_sh;
	};
}

#pragma pack(pop)

#endif // 

