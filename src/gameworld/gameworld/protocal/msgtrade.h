
#ifndef MSGTRANSACTION_H
#define MSGTRANSACTION_H

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/struct/itemlistparam.h"

#include "gamedef.h"


#pragma pack(push) 
#pragma pack(4)


namespace Protocol
{
	/*
		�������
	*/

	// ������ĳ�˽���
	class CSReqTrade
	{
	public:
		CSReqTrade();
		MessageHeader		header;

		int					uid;
	};

	// ת����������
	class SCReqTradeRoute
	{
	public:
		SCReqTradeRoute();
		MessageHeader		header;
		int					req_uid;
		GameName			req_name;
		int					level;
		char				sex;
		char				prof;
		char				camp;
		char				reserved;
		long long			avatar_timestamp;
	};

	// ת���������󷵻�
	class CSReqTradeRet
	{
	public:
		CSReqTradeRet();
		MessageHeader		header;

		short				result;			// 1 ͬ�� 0 ��ͬ��
		short				reserve_sh;
		int					req_uid;
	};

	// ����������
	class CSTradeLockReq
	{
	public:
		CSTradeLockReq();
		MessageHeader		header;
	};

	// ������ȷ��
	class CSTradeAffirmReq
	{
	public:
		CSTradeAffirmReq();
		MessageHeader		header;
	};

	// ȡ������
	class CSTradeCancle
	{
	public:
		CSTradeCancle();
		MessageHeader		header;
	};

	// ����ͭ�ҷ��Ͻ��׼�
	class CSTradeGoldReq
	{
	public:
		CSTradeGoldReq();
		MessageHeader		header;
		int					gold;
		int					coin;
	};

	// ��ͭ�ҷ��Ͻ��׼�
	class SCTradeGold
	{
	public:
		SCTradeGold();
		MessageHeader		header;
		int					is_me;	// 0 ���Լ���1�Է�
		int					gold;
		int					coin;
	};

	// ������Ʒ���Ͻ��׼�
	class CSTradeItemReq
	{
	public:
		CSTradeItemReq();
		MessageHeader		header;
		short				trade_index;	// ���׼��±�
		short				knapsack_index; // �����±꣬-1 ����ɾ����trade_index
		int					item_num;		// ��������
	};

	// ����Ʒ���Ͻ��׼�
	class SCTradeItem
	{
	public:
		SCTradeItem();
		MessageHeader		header;
		short				is_me;	// 0 ���Լ���1�Է�
		short				trade_index;
		short				knapsack_index;
		short				reserve_sh;
		ItemID				item_id;
		short				num;
		UInt32				invalid_time;
	};

	// ����Ʒ���Ͻ��׼�(����Ʒ���в���[װ��])
	class SCTradeItemParam
	{
	public:
		SCTradeItemParam();
		MessageHeader		header;
		short				is_me;					// 0 ���Լ���1�Է�
		short				trade_index;
		short				knapsack_index;
		short				reserve_sh;			

		ItemDataWrapper		item_wrapper;
	};

	class SCTradeState
	{
	public:
		SCTradeState();
		MessageHeader header;

		short trade_state;
		short other_trade_state;
		int	other_uid;
		GameName other_name;
	};
}


#pragma pack(pop)




#endif

