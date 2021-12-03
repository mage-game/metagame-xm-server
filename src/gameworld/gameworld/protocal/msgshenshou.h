#ifndef __MSG_SHENSHOU_H__
#define __MSG_SHENSHOU_H__

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/shenshoudef.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	enum SHENSHOU_REQ_TYPE
	{
		SHENSHOU_REQ_TYPE_ALL_INFO = 0,											// ����������Ϣ,����˷���2562,2563,2564
		SHENSHOU_REQ_TYPE_PUT_ON,												// װ���� param1 ����ID, param2 ��������index��param3 װ���۸���index
		SHENSHOU_REQ_TYPE_TAKE_OFF,												// ж�£� param1 ����ID, param2 װ����index
		SHENSHOU_REQ_TYPE_ZHUZHAN,												// ��ս�� param1 ����ID��
		SHENSHOU_REQ_TYPE_ADD_ZHUZHAN,											// ��չ������սλ
		SHENSHOU_REQ_TYPE_COMPOSE,												// �ϳɣ� param_1 ��Ʒid ��param_2 ��������index1 ��param_3 ��������index2��param_4 ��������index3
		
		SHENSHOU_REQ_TYPE_HUANLING_INFO,										// ��������Ϣ,����������2565
		SHENSHOU_REQ_TYPE_HUANLING_REFRESH,										// ����ˢ��
		SHENSHOU_REQ_TYPE_HUANLING_DRAW,										// ����齱
	};

	class CSShenshouOperaReq													// ���޲�������  2560
	{
	public:
		CSShenshouOperaReq();
		MessageHeader header;

		short opera_type;														// ��� SHENSHOU_REQ_TYPE
		short param_1;
		short param_2;
		short param_3;
		int param_4;
	};

	class CSSHenshouReqStrength													// ǿ��װ��	2561
	{
	public:
		CSSHenshouReqStrength();
		MessageHeader header;

		short shenshou_id;														// ����id
		short equip_index;														// Ҫǿ����װ���±�

		short is_use_double_shuliandu;											// �Ƿ񼤻�˫��������
		short destroy_num;														// ������Ʒ����
		short destroy_backpack_index_list[SHENSHOU_MAX_BACKPACK_COUNT];			// ���ĵ���Ʒ�±��б�
	};

	class SCShenshouBackpackInfo												// ������Ϣ  2562
	{
	public:
		struct GridItem
		{
			short reserve_sh;
			short index;
			ShenshouBackpackItem iteminfo;
		};

		SCShenshouBackpackInfo();
		MessageHeader header;

		char is_full_backpack;													// �Ƿ�ȫ����Ϣ�·�
		char reserve;

		short grid_num;															// ��������
		GridItem grid_list[SHENSHOU_MAX_BACKPACK_COUNT];						// ������Ϣ�б�
	};

	class SCShenshouListInfo													// ������Ϣ  2563
	{
	public:
		struct ShenshouListItem
		{
			short reserve_sh;
			short shou_id;
			ShenshouInfo shou_info;
		};

		SCShenshouListInfo();
		MessageHeader header;

		char is_all_shenshou;													// �Ƿ�ȫ����Ϣ�·�
		char reserve;

		short shenshou_num;														// ��װ����Ϣ�������б�
		ShenshouListItem shenshou_list[SHENSHOU_MAX_ID + 1];
	};

	class SCShenshouBaseInfo													// ���޻�����Ϣ  2564 
	{
	public:
		SCShenshouBaseInfo();
		MessageHeader header;

		short extra_zhuzhan_count;												// �������ս����
		short reserve_sh;
	};

	class SCShenshouHuanlingListInfo											// ���޻���ˢ���б� 2565
	{
	public:
		SCShenshouHuanlingListInfo();
		MessageHeader header;

		int score;																// �齱����
		int cur_draw_times;														// ��ǰ�齱����
		HuanlingListInfo huanling_list[SHENSHOU_MAX_RERFESH_ITEM_COUNT];
		UNSTD_STATIC_CHECK(0 == SHENSHOU_MAX_RERFESH_ITEM_COUNT % 2);
	};

	class SCShenshouHuanlingDrawInfo											// ���޻���齱��� 2566
	{
	public:
		SCShenshouHuanlingDrawInfo();
		MessageHeader header;

		int score;																// �齱����
		short seq;																// �齱���
		short cur_draw_times;													// ��ǰ�齱����
	};
}

#pragma pack(pop)

#endif

