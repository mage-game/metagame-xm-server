/***************************************************************************
FileName: msgcsrank.h
Author: fyh
Description: ���а� �ͻ���-������ ���Э�� 
Version: 1.0
History: 
<author>    <time>					<version >    <desc>
fyh		   2011/09/06/ 17:48:05     1.0           ����
**************************************************************************/

#ifndef __MSG_CS_RANK_HPP__
#define __MSG_CS_RANK_HPP__

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/serverdef.h"
#include "servercommon/struct/global/publicsaleparam.hpp"
#include "servercommon/rankdef.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	class CSGetPersonRankList
	{
	public:
		CSGetPersonRankList();

		MessageHeader header;

		int rank_type;    
	};

	class CSGetPersonRankTopUser
	{
	public:
		CSGetPersonRankTopUser();

		MessageHeader header;

		int rank_type;
	};

	class CSGetWorldLevel
	{
	public:
		CSGetWorldLevel();

		MessageHeader header;
	};

	class CSGetGuildRankList
	{
	public:
		CSGetGuildRankList();

		MessageHeader header;

		int rank_type;
	};

	class CSGetTeamRankList
	{
	public:
		CSGetTeamRankList();

		MessageHeader header;

		int rank_type;    
	};

	class CSGetCoupleRankList		// 10054
	{
	public:
		CSGetCoupleRankList();
		MessageHeader header;

		int rank_type;
	};

	//////////////////////////////////////// ������а� /////////////////////////////////////////
	class SCGetSpecialRankValueAck			// 10006
	{
	public:
		SCGetSpecialRankValueAck();
		MessageHeader header;

		int rank_type;						// ���а�����
		int rank_value;						// ���а�ֵ
	};

	class CSGetSpecialRankValue		// 10055
	{
	public:
		CSGetSpecialRankValue();
		MessageHeader header;

		int rank_type;			// ���а�����
	};
}

#pragma pack(pop)

#endif // __MSG_CS_RANK_HPP__


