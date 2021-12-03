#ifndef __MSG_QILINBI_H__
#define __MSG_QILINBI_H__

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/struct/qilinbiparam.hpp"
#include "gameworld/gameworld/config/shuxingdanconfig.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	class SCQiLinBiInfo																// 8750 �������Ϣ
	{
	public:
		SCQiLinBiInfo();
		MessageHeader header;

		short qilinbi_level;														// �ȼ�
		short grade;																// ��
		short star_level;															// �Ǽ�
		short used_imageid;															// ʹ�õ�����
		int grade_bless_val;														// ����ף��ֵ
		unsigned char active_image_flag[QiLinBiParam::MAX_ACTIVE_IMAGE_BYTE];		// ����������б�
		unsigned char active_special_image_flag[QiLinBiParam::MAX_ACTIVE_SPECIAL_IMAGE_BYTE];	// ��������������б�
		unsigned int clear_upgrade_time;											// ���ף��ֵ��ʱ��
		short temporary_imageid;													// ��ǰʹ����ʱ����
		short temporary_imageid_has_select;											// ��ѡ������ʱ����
		unsigned int temporary_imageid_invalid_time;								// ��ʱ������Чʱ��

		unsigned short shuxingdan_list[SHUXINGDAN_MAX_TYPE];						// ���Ե�����
		short skill_level_list[QiLinBiParam::SKILL_COUNT];							// ���ܵȼ�
		char special_img_grade_list[QiLinBiParam::MAX_SPECIAL_IMAGE_COUNT];			// �����������
		short equip_level_list[QiLinBiParam::EQUIP_COUNT];							// װ����Ϣ

	};

	class SCQiLinBiAppeChange														// 8751 �������۸ı�
	{
	public:
		SCQiLinBiAppeChange();
		MessageHeader header;

		ObjID objid;
		unsigned short qilinbi_appeid;
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	enum QILINBI_OPERA_TYPE
	{
		QILINBI_OPERA_TYPE_INFO = 0,								// ��Ϣ	
		QILINBI_OPERA_TYPE_UPGRADE,									// ���� p1:repeat_times p2:auto_buy
		QILINBI_OPERA_TYPE_SPECIAL_IMAGE_UPGRADE,					// ����������� p1:special_image_id
		QILINBI_OPERA_TYPE_USE_IMAGE,								// ʹ������ p1:image_id
		QILINBI_OPERA_TYPE_SKILL_UPGRADE,							// ���ܽ��� p1:skill_idx p2:is_auto_buy
		QILINBI_OPERA_TYPE_EQUIP_UPGRADE,							// װ������ p1:equip_idx
	};

	class CSQiLinBiOperaReq															// 8752 �������
	{
	public:
		CSQiLinBiOperaReq();
		MessageHeader header;

		short opera_type;
		short param_1;
		short param_2;
		short param_3;
	};

}

#pragma pack(pop)

#endif // 

