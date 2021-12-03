#ifndef MSG_LIEMING_H
#define MSG_LIEMING_H

#include "gamedef.h"
#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/liemingdef.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	enum LIEMING_HUNSHOU_OPERA_TYPE
	{
		LIEMING_HUNSHOU_OPERA_TYPE_INVALID,
		LIEMING_HUNSHOU_OPERA_TYPE_CHOUHUN,               // ���  param1Ϊ��ɫ
		LIEMING_HUNSHOU_OPERA_TYPE_SUPER_CHOUHUN,
		LIEMING_HUNSHOU_OPERA_TYPE_BATCH_HUNSHOU,         // �Ի�
		LIEMING_HUNSHOU_OPERA_TYPE_SPECIAL_BATCH_HUNSHOU,
		LIEMING_HUNSHOU_OPERA_TYPE_PUT_BAG,               // �ӳ���ȡ������  param1Ϊ����index
		LIEMING_HUNSHOU_OPERA_TYPE_CONVERT_TO_EXP,        // �����жһ��ɾ���
		LIEMING_HUNSHOU_OPERA_TYPE_MERGE,                
		LIEMING_HUNSHOU_OPERA_TYPE_SINGLE_CONVERT_TO_EXP, // �����һ�����  param_2:1 Ϊ���  ����Ϊ ����   param1Ϊindex
		LIEMING_HUNSHOU_TAKEON,                           // װ��
		LIEMING_HUNSHOU_TAKEOFF,                          // ж��
		LIEMING_HUNSHOU_FUHUN_ADD_EXP,                    
		LIEMING_HUNSHOU_OPERA_TYPE_PUT_BAG_ONE_KEY,       // һ�� �ӳ���ȡ������
		LIEMING_HUNSHOU_OPERA_TYPE_AUTO_UPLEVEL,		  // һ�� ����

		LIEMING_HUNSHOU_OPERA_TYPE_MAX,
	};

 	enum LIEMING_BAG_NOTIFY_REASON
 	{
 		LIEMING_BAG_NOTIFY_REASON_INVALID = 0,
 		LIEMING_BAG_NOTIFY_REASON_BAG_MERGE,
 
 		LIEMING_BAG_NOTIFY_REASON_MAX,
 	};

	struct LieMingHunShou
	{
		short hunshou_id;
		short level;
		long long curr_exp;
	};

	struct InfoType
	{
		int	index;
		ItemDataWrapper	item_wrapper;
	};

	class CSLieMingHunshouOperaReq
	{
	public:
		CSLieMingHunshouOperaReq();
		MessageHeader header;

		short opera_type;
		short reserve_sh;
		int param_1;
		int param_2;
	};

 	//class SCLieMingSlotInfo
 	//{
 	//public:
		//SCLieMingSlotInfo();
 	//	MessageHeader header;
 
 	//	int notify_reason;
		//int slot_activity_flag;
 
 	//	LieMingHunShou slot_list[LIEMING_FUHUN_SLOT_COUNT];
 	//};

 	class SCLieMingSlotInfo
 	{
 	public:
		SCLieMingSlotInfo();
 		MessageHeader header;

 		int notify_reason;
		int slot_activity_flag;
		int count;

		long long total_exp;
		InfoType minghun_list[LIEMING_FUHUN_SLOT_COUNT];
 	};

	class SCLieMingBagInfo
	{
	public:
		SCLieMingBagInfo();
		MessageHeader header;

		int notify_reason;
		long long hunshou_exp;
		char liehun_color;
		char reserve_ch;
		short reserve_sh;
		int hunli;

		short liehun_pool[LIEMING_LIEHUN_POOL_MAX_COUNT];
		LieMingHunShou grid_list[LIEMING_HUNSHOU_BAG_GRID_MAX_COUNT];
	};

	class CSLieMingExchangeList		// 5658 ���꽻������
	{
	public:
		CSLieMingExchangeList();
		MessageHeader header;

		struct ExchangeInfo
		{
			short source_index;
			short dest_index;
		};

		int exchange_count;
		ExchangeInfo exchange_list[LIEMING_FUHUN_SLOT_COUNT];
	};
}

#pragma pack(pop)

#endif

