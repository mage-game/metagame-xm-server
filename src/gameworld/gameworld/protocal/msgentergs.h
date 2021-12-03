
#ifndef MSGENTERGS_H
#define MSGENTERGS_H

#include "gamedef.h"
#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/logindef.h"


#pragma pack(push) 
#pragma pack(4)


namespace Protocol
{
	/* 
	���볡����������أ��Լ��Ƴ���Ϸ����
	*/
	class CSUserEnterGSReq
	{
	public:
		CSUserEnterGSReq();
		MessageHeader	header;
		int				scene_id;
		int				scene_key;
		int				last_scene_id;
		int				role_id;
		GameName		role_name;
		int				time;
		char			is_login;			// ��½���ǳ����л�
		char			reserved1;
		short			server;
		SessionKey		key;
		PlatName		pname;
		int				is_micro_pc;
		int				plat_spid;		// ƽ̨������ID
	};

	class SCServerBusy
	{
	public:
		SCServerBusy();
		MessageHeader	header;
	};

	enum __EnterGSResult
	{
		ENTERGS_RESULT_SUC=0,
		ENTERGS_USER_EXIST=-1,
		ENTERGS_RESULT_SCENE_NO_EXIST=-2,
	};
	class SCUserEnterGSAck
	{
	public:
		SCUserEnterGSAck();
		MessageHeader	header;
		int				result;
	};

	class SCChangeGS
	{
	public:
		SCChangeGS();
		MessageHeader	header;
		int				scene_id;
		int				scene_key;
		int				last_scene_id;
		int				uid;
		unsigned int	time;
		SessionKey		key;
		unsigned short	gs_index;
		unsigned short	is_login;
	};

	class CSUserLogout
	{
	public:
		CSUserLogout();
		MessageHeader	header;
	};

	class CSTransportReq
	{
	public:
		CSTransportReq();
		MessageHeader	header;
		int				transport_index;
	};

	class CSHeartBeat
	{
	public:
		CSHeartBeat();
		MessageHeader	header;
	};
}


#pragma pack(pop)


#endif


