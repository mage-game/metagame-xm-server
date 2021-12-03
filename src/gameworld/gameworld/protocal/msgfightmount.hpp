#ifndef __MSG_FIGHT_MOUNT_H__
#define __MSG_FIGHT_MOUNT_H__

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/struct/fightmountparam.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	class SCFightMountInfo				//  ս��������Ϣ  2550
	{
	public:
		SCFightMountInfo();
		MessageHeader header;

		short mount_flag;															// MOUNTFLAG  -------- & 1 != 0 �������ϣ�������
		short reserved;
		short mount_level;															// �ȼ�
		short grade;																// ��
		short star_level;															// �Ǽ�
		short used_imageid;															// ʹ�õ�����
		short shuxingdan_count;														// ���Ե����� 
		short chengzhangdan_count;													// �ɳ�������
		int grade_bless_val;														// ����ף��ֵ
		unsigned char active_image_flag[FightMountParam::MAX_ACTIVE_IMAGE_BYTE];	// ����������б�  & (1 << imageid) != 0���򼤻�imageid
		unsigned char active_special_image_flag[FightMountParam::MAX_ACTIVE_SPECIAL_IMAGE_BYTE];	// ��������������б�
		unsigned int clear_upgrade_time;											// ���ף��ֵ��ʱ��
		int equip_skill_level;														// װ�����ܵȼ�

		short equip_level_list[FightMountParam::EQUIP_COUNT];	// װ����Ϣ
		short skill_level_list[FightMountParam::SKILL_COUNT];						// ���ܵȼ�
		char special_img_grade_list[FightMountParam::MAX_SPECIAL_IMAGE_COUNT];		// �����������
	};

	class SCFightMountAppeChange			//  ������۸ı�
	{
	public:
		SCFightMountAppeChange();
		MessageHeader header;

		ObjID objid;
		unsigned short mount_appeid;
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class CSGoonFightMount				//  ������/������
	{
	public:
		CSGoonFightMount();
		MessageHeader header;

		int goon_mount;				// 1�ϣ�0��
	};

	class CSUpgradeFightMount			//  �����������
	{
	public:
		CSUpgradeFightMount();
		MessageHeader header;

		short repeat_times;
		short auto_buy;
	};

	class CSUseFightMountImage			//  ����ʹ������
	{
	public:
		CSUseFightMountImage();
		MessageHeader header;

		short reserve_sh;
		short image_id;
	};

	class CSFightMountSpecialImgUpgrade	//  ���������������
	{
	public:
		CSFightMountSpecialImgUpgrade();
		MessageHeader header;

		short special_image_id;
		short reserve_sh;
	};

	class CSFightMountUplevelEquip		//  ��������װ������
	{
	public:
		CSFightMountUplevelEquip();	
		MessageHeader header;

		char equip_idx;
		short reserve;
	};

	class CSFightMountSkillUplevelReq
	{
	public:
		CSFightMountSkillUplevelReq();	//  ���＼����������
		MessageHeader header;

		short skill_idx;
		short auto_buy;
	};

	class CSFightMountGetInfo			//  ����������Ϣ
	{
	public:
		CSFightMountGetInfo();	
		MessageHeader header;
	};

	class CSFightMountUpStarLevel		// ս���������Ǽ�����
	{
	public:
		CSFightMountUpStarLevel();
		MessageHeader header;

		short stuff_index;
		short is_auto_buy;

		int loop_times;
	};
}

#pragma pack(pop)

#endif // 

