#ifndef __MSG_UPGRADE_H__
#define __MSG_UPGRADE_H__

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/struct/upgradeparam.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	enum UPGRADE_OPERA_TYPE
	{
		UPGRADE_OPERA_TYPE_INFO = 0,							// ��Ϣ	
		UPGRADE_OPERA_TYPE_USE_IMAGE,							// ʹ������ p1:is_temporary_image p2:image_id
		UPGRADE_OPERA_TYPE_FIGHT_OUT,							// �Ƿ��ս p1:is_fight_out 
		UPGRADE_OPERA_TYPE_UPGRADE,								// ���� p1:repeat_times p2:auto_buy
		UPGRADE_OPERA_TYPE_IMAGE_UPGRADE,						// ������� p1:image_id
		UPGRADE_OPERA_TYPE_SKILL_UPGRADE,						// ���ܽ��� p1:skill_idx p2:is_auto_buy
		UPGRADE_OPERA_TYPE_EQUIP_UPGRADE,						// װ������ p1:equip_idx

		UPGRADE_OPERA_TYPE_MAX,
	};

	class CSUpgradeOperaReq										// 8735 �������
	{
	public:
		CSUpgradeOperaReq();
		MessageHeader header;

		short upgrade_type;
		short opera_type;
		short param_1;
		short param_2;
		short param_3;
		short param_4;
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class SCUpgradeInfo				// 8733 ����ϵͳ��Ϣ
	{
	public:
		SCUpgradeInfo();
		MessageHeader header;

		short upgrade_type;															// ����ϵͳ����
		short fightout_flag;														// fightout_flag  -------- & 1 != 0 ���ս״̬
		short level;																// �ȼ�
		short grade;																// ��
		short star_level;															// �Ǽ�
		short used_imageid;															// ʹ�õ�����
		int grade_bless_val;														// ����ף��ֵ
		unsigned int clear_upgrade_time;											// ���ף��ֵ��ʱ��
		short temporary_imageid;													// ��ǰʹ����ʱ����
		short temporary_imageid_has_select;											// ��ѡ������ʱ����
		unsigned int temporary_imageid_invalid_time;								// ��ʱ������Чʱ��
		int equip_skill_level;														// װ�����ܵȼ�
		unsigned int last_upgrade_time;                                             // ��һ�ν��׳ɹ�ʱ��

		UInt16 shuxingdan_list[SHUXINGDAN_MAX_TYPE];								// ���Ե�����
		short equip_level_list[UPGRADE_EQUIP_COUNT];								// װ����Ϣ
		short skill_level_list[UPGRADE_SKILL_COUNT];								// ���ܵȼ�
		UInt8 active_img_flag[UPGRADE_MAX_IMAGE_BYTE];								// ���󼤻���
		UInt8 img_grade_list[UPGRADE_IMAGE_MAX_COUNT];								// ��������б�

	};

	class SCUpgradeAppeChange			// 8734 ������۸ı�
	{
	public:
		SCUpgradeAppeChange();
		MessageHeader header;

		short upgrade_type;													// ����ϵͳ����
		ObjID objid;														// ����obj_id
		unsigned short upgrade_appeid;										// �����Ϣid
		short reserve_sh;
	};
}

#pragma pack(pop)

#endif // 

