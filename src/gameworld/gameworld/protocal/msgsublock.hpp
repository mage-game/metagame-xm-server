#ifndef MSGSUBLOCK_H
#define MSGSUBLOCK_H

#include "servercommon/userprotocal/msgheader.h"

#pragma pack(push) 
#pragma pack(4)

enum SUBLOCK_OPERATE_TYPE
{
	SUBLOCK_OPERATE_TYPE_INVALID = 0,
	SUBLOCK_OPERATE_TYPE_CREATE_PW,
	SUBLOCK_OPERATE_TYPE_DELETE_PW,
	SUBLOCK_OPERATE_TYPE_LOCK,
	SUBLOCK_OPERATE_TYPE_UNLOCK,
	SUBLOCK_OPERATE_TYPE_MAX,
};

namespace Protocol
{
	class SCSubLockOperateSucc
	{
	public:
		SCSubLockOperateSucc();
		MessageHeader header;

		int		operate_type;
	};

	class SCSubLockOperateFail
	{
	public:
		SCSubLockOperateFail();
		MessageHeader header;

		short	operate_type;
		short	today_error_times;
	};

	class SCSubLockInfo
	{
	public:
		SCSubLockInfo();
		MessageHeader header;

		short			has_pw;
		short			today_error_times;
	};

	//////////////////////////////////////////////////////////////////////////
	class CSSubLockCreatePw
	{
	public:
		CSSubLockCreatePw();
		MessageHeader header;

		SublockMD5	password_md5;
	};

	class CSSubLockDeletePw
	{
	public:
		CSSubLockDeletePw();
		MessageHeader header;

		SublockMD5	password_md5;
	};

	class CSSubLockLock
	{
	public:
		CSSubLockLock();
		MessageHeader header;
	};

	class CSSubLockUnlock
	{
	public:
		CSSubLockUnlock();
		MessageHeader header;

		SublockMD5	password_md5;
	};

};

#pragma pack(pop)

#endif

