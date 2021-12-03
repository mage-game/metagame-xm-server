#ifndef __MSG_MOUNT_H__
#define __MSG_MOUNT_H__

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/struct/mountparam.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	class SCMountInfo				// 4700 ������Ϣ
	{
	public:
		SCMountInfo();
		MessageHeader header;

		short mount_flag;															// MOUNTFLAG  -------- & 1 != 0 �������ϣ�������
		short reserved;
		short mount_level;															// �ȼ�
		short grade;																// ��
		short star_level;															// �Ǽ�
		short used_imageid;															// ʹ�õ�����
		unsigned short shuxingdan_list[SHUXINGDAN_MAX_TYPE];						// ���Ե�����
		int grade_bless_val;														// ����ף��ֵ
		unsigned char active_image_flag[MountParam::MAX_ACTIVE_IMAGE_BYTE];			// ����������б�  & (1 << imageid) != 0���򼤻�imageid
		unsigned char active_special_image_flag[MountParam::MAX_ACTIVE_SPECIAL_IMAGE_BYTE];	// ��������������б�
		unsigned int clear_upgrade_time;											// ���ף��ֵ��ʱ��
		short temporary_imageid;													// ��ǰʹ����ʱ����
		short temporary_imageid_has_select;											// ��ѡ������ʱ����
		unsigned int temporary_imageid_invalid_time;								// ��ʱ������Чʱ��
		int equip_skill_level;														// װ�����ܵȼ�

		short equip_level_list[MountParam::EQUIP_COUNT];							// װ����Ϣ
		short skill_level_list[MountParam::SKILL_COUNT];							// ���ܵȼ�
		char special_img_grade_list[MountParam::MAX_SPECIAL_IMAGE_COUNT];			// �����������
		unsigned int last_upgrade_time;                                             //��һ�ν��׳ɹ�ʱ��

	};

	class SCMountAppeChange			// 4701 ������۸ı�
	{
	public:
		SCMountAppeChange();
		MessageHeader header;

		ObjID objid;
		unsigned short mount_appeid;
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class CSGoonMount				// 4750 ������/������
	{
	public:
		CSGoonMount();
		MessageHeader header;

		int goon_mount;				// 1�ϣ�0��
	};

	class CSUpgradeMount			// 4751 �����������
	{
	public:
		CSUpgradeMount();
		MessageHeader header;

		short repeat_times;
		short auto_buy;
	};

	class CSUseMountImage			// 4752 ����ʹ������
	{
	public:
		CSUseMountImage();
		MessageHeader header;

		short is_temporary_image;
		short image_id;
	};

	class CSMountSpecialImgUpgrade	// 4753 ���������������
	{
	public:
		CSMountSpecialImgUpgrade();
		MessageHeader header;

		short special_image_id;
		short reserve_sh;
	};

	class CSMountUplevelEquip		// 4754 ��������װ������
	{
	public:
		CSMountUplevelEquip();	
		MessageHeader header;

		short equip_idx;			// װ������
		short reserve;				
	};

	class CSMountSkillUplevelReq
	{
	public:
		CSMountSkillUplevelReq();	// 4755 ���＼����������
		MessageHeader header;

		short skill_idx;
		short auto_buy;
	};

	class CSMountGetInfo			// 4756 ����������Ϣ
	{
	public:
		CSMountGetInfo();	
		MessageHeader header;
	};

	class CSMountUpStarLevel		// 4758 �������Ǽ�����
	{	
	public:
		CSMountUpStarLevel();
		MessageHeader header;

		short stuff_index;
		short is_auto_buy;

		int loop_times;
	};
}

#pragma pack(pop)

#endif // 

