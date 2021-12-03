#ifndef __MSG_TUODAN_HPP__
#define __MSG_TUODAN_HPP__

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/serverdef.h"
#include "servercommon/struct/global/publicsaleparam.hpp"
#include "servercommon/tuodandef.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	enum TUODAN_REQ_TYPE
	{
		TUODAN_UPDATE = 0,
		TUODAN_DELETE,
	};

	// 脱单信息
	struct UserTuodanInfo
	{
		int uid;
		GameName name;
		char prof;
		char sex;
		short level;
		int capability;
		long long avatar_timestamp;
		unsigned int create_time;
		TuodanNotice notice;
	};

	// 请求全部信息--8371
	class CSGetAllTuodanInfo
	{
	public:
		CSGetAllTuodanInfo();

		MessageHeader header;
	};

	// 发送全部信息--8372
	class SCAllTuodanInfo
	{
	public:
		SCAllTuodanInfo();
		MessageHeader header;

		int tuodan_list_count;
		UserTuodanInfo tuodan_list[MAX_TUODAN_COUNT];
	};
	
	// 操作请求--8373
	class CSTuodanREQ
	{
	public:
		CSTuodanREQ();
		MessageHeader header;
		
		int req_type;
		TuodanNotice notice;
	};

	// 发送单条信息--8374
	class SCSingleTuodanInfo
	{
	public:
		SCSingleTuodanInfo();
		MessageHeader header;
		
		int change_type;                 //0:更新，1:删除
		UserTuodanInfo tuodan_info;
	};
}

#pragma pack(pop)

#endif // __MSG_TUODAN_HPP__
