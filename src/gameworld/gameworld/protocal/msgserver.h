#ifndef __MSGSERVER_H__
#define __MSGSERVER_H__

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/serverdef.h"
#include "servercommon/logindef.h"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	class CSEnterGlobalServerReq
	{
	public:
		CSEnterGlobalServerReq();
		MessageHeader header;

		int	scene_id;					// 该字段对globalserver没有什么意义 纯粹用于验证key
		int	scene_key;					// 同上
		int	last_scene_id;
		int	role_id;
		GameName role_name;
		int time;
		SessionKey key;
		PlatName pname;
		short server;
		short is_micro_pc;
	};

	enum __EnterGolbalServerResult
	{
		EGS_RESULT_SUC = 0,
		EGS_USER_EXIST = -1,
	};
	
	class SCEnterGlobalServerAck
	{
	public:
		SCEnterGlobalServerAck();
		MessageHeader header;

		int result;
	};

	class CSTimeReq
	{
	public:
		CSTimeReq();
		MessageHeader header;
	};

	class SCTimeAck
	{
	public:
		SCTimeAck();
		MessageHeader header;

		unsigned int server_time;
		unsigned int server_real_start_time;
		int open_days;
		unsigned int server_real_combine_time;
	};

	class SCWallowNotice
	{
	public:
		SCWallowNotice();
		MessageHeader header;

		int notice_type;
	};

	class SCChangeThread
	{
	public:
		SCChangeThread();
		MessageHeader header;
		int game_thread;
	};

	class SCDisconnectNotice
	{
	public:
		SCDisconnectNotice();
		MessageHeader header;

		int reason;
	};

}

#pragma pack(pop)

#endif

