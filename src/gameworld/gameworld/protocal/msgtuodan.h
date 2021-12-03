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

	// �ѵ���Ϣ
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

	// ����ȫ����Ϣ--8371
	class CSGetAllTuodanInfo
	{
	public:
		CSGetAllTuodanInfo();

		MessageHeader header;
	};

	// ����ȫ����Ϣ--8372
	class SCAllTuodanInfo
	{
	public:
		SCAllTuodanInfo();
		MessageHeader header;

		int tuodan_list_count;
		UserTuodanInfo tuodan_list[MAX_TUODAN_COUNT];
	};
	
	// ��������--8373
	class CSTuodanREQ
	{
	public:
		CSTuodanREQ();
		MessageHeader header;
		
		int req_type;
		TuodanNotice notice;
	};

	// ���͵�����Ϣ--8374
	class SCSingleTuodanInfo
	{
	public:
		SCSingleTuodanInfo();
		MessageHeader header;
		
		int change_type;                 //0:���£�1:ɾ��
		UserTuodanInfo tuodan_info;
	};
}

#pragma pack(pop)

#endif // __MSG_TUODAN_HPP__
