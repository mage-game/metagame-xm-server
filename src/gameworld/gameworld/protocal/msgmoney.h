
#ifndef MSGMONEY_H
#define MSGMONEY_H


#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/basedef.h"

#pragma pack(push) 
#pragma pack(4)



namespace Protocol
{
	/*
		Ǯ
	*/

	class SCMoney
	{
	public:
		SCMoney();
		MessageHeader		header;
		Int64				gold;
		Int64				gold_bind;
		Int64				coin;
		Int64				coin_bind;
	};

	class CSAccountGoldReq
	{
	public:
		CSAccountGoldReq();
		MessageHeader		header;
	};

	class SCAccountGoldAck
	{
	public:
		SCAccountGoldAck();
		MessageHeader		header;
		Int64				gold;
	};

	class CSAccountGetGoldReq
	{
	public:
		CSAccountGetGoldReq();
		MessageHeader		header;
		Int64				gold;
	};

	enum AccountGetGoldAckResult
	{
		AGGAR_SUC=0,
		AGGAR_NO_GOLD=-1,
		AGGAR_GET_GOLD_ZERO=-2,
		AGGAR_SERVER_ERROR=-3,
		AGGAR_ACCOUNT_GET_TIME_LIMIT=-4,
	};

	class SCAccountGetGoldAck
	{
	public:
		SCAccountGetGoldAck();
		MessageHeader		header;
		int					result;
		Int64				gold;
	};
}


#pragma pack(pop)



#endif
