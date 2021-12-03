#ifndef __MSG_SHENZHOU_WEAPON_H__
#define __MSG_SHENZHOU_WEAPON_H__

#include "gamedef.h"
#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/shenzhouweapondef.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	enum SHENZHOU_REQ_TYPE
	{
		SHENZHOU_REQ_TYPE_ALL_INFO_REQ = 0,									// ����������Ϣ
		SHENZHOU_REQ_TYPE_BUY_GATHER_TIME,									// ����ɼ�����
		SHENZHOU_REQ_TYPE_EXCHANGE_IDENTIFY_EXP,							// �һ���������
		SHENZHOU_REQ_TYPE_INDENTIFY,										// ������Ʒ param1 ������Ʒ�±�, param2 ��������
		SHENZHOU_REQ_TYPE_UPLEVEL_WEAPON_SLOT ,								// �������������ȼ��� param1 �������ͣ�param2 ������λ
		SHENZHOU_REQ_TYPE_GATHER_INFO_REQ,									// ����ɼ���Ϣ
		SHENZHOU_REQ_TYPE_HELP_OTHER_BOX,									// Э�����˵ı���
		SHENZHOU_REQ_TYPE_OPEN_BOX,											// �򿪱���
		SHENZHOU_REQ_TYPE_BOX_INFO,											// ��������Ϣ
		SHENZHOU_REQ_TYPE_PUT_BOX,											// ���뱦��
		SHENZHOU_REQ_TYPE_UPLEVEL_ELEMENT ,									// ����Ԫ�صȼ��� param1 �������ͣ�param2 Ԫ������
		SHENZHOU_REQ_TYPE_UPLEVEL_LINGSHU,									// ��������ȼ��� param1 �������ͣ� param2 ��ӡ��
		SHENZHOU_REQ_TYPE_HUNYIN_INLAY,										// ��Ƕ��ӡ�� param1 �������ͣ� param2 ��ӡ�ۣ� param3��������
		SHENZHOU_REQ_TYPE_INVITE_HELP_OTHER_BOX,							// ����Э������
		SHENZHOU_REQ_TYPE_REMOVE_HELP_BOX,									// ���Э��
		SHENZHOU_REQ_TYPE_XILIAN_OPEN_SLOT,									// ����ϴ���ۣ�param1 �������ͣ� param2 ���Բ�
		SHENZHOU_REQ_TYPE_XILIAN_REQ,										// ����ϴ����param1 �������ͣ� param2������0-7λ��ʾ1-8λ����, param3ϴ����������, param4 �Ƿ��Զ�����, param5 �Ƿ����ϴ��
		SHENZHOU_REQ_TYPE_OPEN_HUNYIN_SLOT,									// ������ӡ�ۣ�param1 �������ͣ� param2 ��ӡ��
		SHENZHOU_REQ_TYPE_AUTO_UPLEVEL_LINGSHU,								// һ����������ȼ��� param1 ��������
	};

	class CSSHenzhouWeaponOperaReq											// ����������������  5684
	{
	public:
		CSSHenzhouWeaponOperaReq();
		MessageHeader header;

		short opera_type;
		short reserve_sh;
		int param_1;
		int param_2;
		int param_3;
		int param_4;
		int param_5;
	};

	class SCShenzhouWeapondAllInfo											// ��������������Ϣ 2407
	{
	public:
		SCShenzhouWeapondAllInfo();
		MessageHeader header;

		char today_gather_times;											// ���ղɼ�����
		char today_buy_gather_times;										// ���չ���ɼ��ܴ���
		short today_exchange_identify_exp_times;							// ���նһ������������
		short identify_level;												// �����ȼ�
		short identify_star_level;											// �����Ǽ�
		int identify_exp;													// ��������
		int hunqi_jinghua;													// ��������
		int lingshu_exp;													// ���ྭ��
		int day_free_xilian_times;											// ���������ϴ������

		int hunqi_count;													// ��������
		ShenzhouWeaponParam::WeaponData weapon_data[SHENZHOU_WEAPON_COUNT]; // �����ȼ�
		ShenzhouWeaponParam::XilianData xilian_data[SHENZHOU_WEAPON_COUNT]; // ϴ������
	};

	class SCShenzhouWeapondGatherInfo										// ���������ɼ���Ϣ 2408
	{
	public:
		SCShenzhouWeapondGatherInfo();
		MessageHeader header;

		char today_gather_times;											// ���ղɼ�����
		char today_buy_gather_times;										// ���չ���ɼ��ܴ���
		short scene_leave_num;												// ����ʣ������		
		int normal_item_num;												// ��ͨ��Ʒ����
		int rare_item_num;													// ϡ����Ʒ����
		int unique_item_num;												// ������Ʒ����
		unsigned int next_refresh_time;										// �´�ˢ��ʱ��
	};

	class SCShenzhouBoxInfo													// ��������������Ϣ 5692
	{
	public:
		SCShenzhouBoxInfo();
		MessageHeader header;

		int box_id;															// ����id
		int today_open_box_free_num;										// ������ѿ��������
		int last_free_ggl_time;												// �����ѿ������ʱ��
		int today_help_box_num;												// ÿ��Э�����˱���Ĵ���
		int box_help_uid_list[SHENZHOU_WEAPON_BOX_HELP_MAX_CONUT];			// Э����ɫ��uid
	};

	class CSSHenzhouWeaponOneKeyIdentifyReq									// 5693һ������
	{
	public:
		CSSHenzhouWeaponOneKeyIdentifyReq();
		MessageHeader header;
	};

	class CSShenzhouHunyinResolveReq										// 5694��ӡ�ֽ�
	{
	public:
		CSShenzhouHunyinResolveReq();
		MessageHeader header;

		static const int HUNYIN_RESOLVE_MAX_INDEX_NUM = 200;

		int index_count;
		short index_in_bag_list[HUNYIN_RESOLVE_MAX_INDEX_NUM];
	};
}

#pragma pack(pop)

#endif

