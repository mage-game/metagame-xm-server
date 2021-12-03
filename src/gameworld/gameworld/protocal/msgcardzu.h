#ifndef MSG_MSG_CARDZU_H
#define MSG_MSG_CARDZU_H

#include "gamedef.h"
#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/cardzudef.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	static const int CARDZU_BATCH_CHOUCARD_TIMES = 10;						// Ԫ�������鿨����

	enum CARDZU_REQ_TYPE
	{
		CARDZU_REQ_TYPE_CHOU_CARD = 0,										// �鿨����
		CARDZU_REQ_TYPE_HUALING,											// ��������
		CARDZU_REQ_TYPE_LINGZHU,											// ��������
		CARDZU_REQ_TYPE_ACTIVE_ZUHE,										// ��������
		CARDZU_REQ_TYPE_UPGRADE_ZUHE,										// �����������
	};

	class CSCardzuOperaReq													// ������ϲ�������  5650
	{
	public:
		CSCardzuOperaReq();
		MessageHeader header;

		short opera_type;
		short reserve_sh;
		int param_1;
		int param_2;
		int param_3;
	};

	class SCCardzuAllInfo													// ����������Ϣ 5600
	{
	public:
		SCCardzuAllInfo();
		MessageHeader header;

		int lingli;															// ����
		char today_coin_buy_card_times[CARDZU_TYPE_MAX_COUNT];				// �������ͭ�ҵ������
		char today_gold_bind_buy_card_times[CARDZU_TYPE_MAX_COUNT];			// ������հ��굥�����
		short all_card_count_list[CARDZU_MAX_CARD_ID + 1];					// ���п��Ƶ������б�
		short all_zuhe_level_list[CARDZU_MAX_ZUHE_ID + 1];					// ������ϵĵȼ��б�
	};

	class SCCardzuChangeNotify												// ������Ϣ�������� 5601
	{
	public:
		SCCardzuChangeNotify();
		MessageHeader header;

		struct ChangeCardItem
		{
			short card_id;
			short count;
		};

		int lingli;
		char today_coin_buy_card_times[CARDZU_TYPE_MAX_COUNT];				// �������ͭ�ҵ������
		char today_gold_bind_buy_card_times[CARDZU_TYPE_MAX_COUNT];			// ������հ��굥�����
		
		short change_zuhe_id;
		short change_zuhe_level;

		int change_card_count;
		ChangeCardItem change_card_list[CARDZU_BATCH_CHOUCARD_TIMES];
	};

	class SCCardzuChouCardResult											// �鿨���ͳһ֪ͨ  5602
	{
	public:
		SCCardzuChouCardResult();
		MessageHeader header;

		short all_card_count_list[CARDZU_BATCH_CHOUCARD_TIMES];				// �鵽�Ŀ���ID�б�
	};
}

#pragma pack(pop)

#endif

