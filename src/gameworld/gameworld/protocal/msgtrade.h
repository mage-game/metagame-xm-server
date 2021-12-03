
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
		交易相关
	*/

	// 请求与某人交易
	class CSReqTrade
	{
	public:
		CSReqTrade();
		MessageHeader		header;

		int					uid;
	};

	// 转发交易请求
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

	// 转发交易请求返回
	class CSReqTradeRet
	{
	public:
		CSReqTradeRet();
		MessageHeader		header;

		short				result;			// 1 同意 0 不同意
		short				reserve_sh;
		int					req_uid;
	};

	// 请求交易锁定
	class CSTradeLockReq
	{
	public:
		CSTradeLockReq();
		MessageHeader		header;
	};

	// 请求交易确认
	class CSTradeAffirmReq
	{
	public:
		CSTradeAffirmReq();
		MessageHeader		header;
	};

	// 取消交易
	class CSTradeCancle
	{
	public:
		CSTradeCancle();
		MessageHeader		header;
	};

	// 请求将铜币放上交易架
	class CSTradeGoldReq
	{
	public:
		CSTradeGoldReq();
		MessageHeader		header;
		int					gold;
		int					coin;
	};

	// 将铜币放上交易架
	class SCTradeGold
	{
	public:
		SCTradeGold();
		MessageHeader		header;
		int					is_me;	// 0 是自己，1对方
		int					gold;
		int					coin;
	};

	// 请求将物品放上交易架
	class CSTradeItemReq
	{
	public:
		CSTradeItemReq();
		MessageHeader		header;
		short				trade_index;	// 交易架下标
		short				knapsack_index; // 背包下标，-1 代表删除该trade_index
		int					item_num;		// 交易数量
	};

	// 将物品放上交易架
	class SCTradeItem
	{
	public:
		SCTradeItem();
		MessageHeader		header;
		short				is_me;	// 0 是自己，1对方
		short				trade_index;
		short				knapsack_index;
		short				reserve_sh;
		ItemID				item_id;
		short				num;
		UInt32				invalid_time;
	};

	// 将物品放上交易架(该物品带有参数[装备])
	class SCTradeItemParam
	{
	public:
		SCTradeItemParam();
		MessageHeader		header;
		short				is_me;					// 0 是自己，1对方
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

