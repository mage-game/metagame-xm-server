
#ifndef MSGREGISTER_H
#define MSGREGISTER_H

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/logindef.h"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	/*
		���������ٽ�ɫ���
	*/

	class CSCreateRole
	{
	public:
		CSCreateRole();
		MessageHeader		header;

		PlatName			pname;
		GameName			role_name;
		unsigned int		login_time;
		LoginStr			login_str;
		short				server;
		char				anti_wallow;
		char				avatar;
		char				sex;
		char				prof;				// ְҵ
		short				reserved2;
		int					plat_spid;			// ƽ̨������ID
	};

	class SCCreateRoleAck
	{
	public:
		enum RET_TYPE
		{
			RESULT_TYPE_SUCC = 0,					// �ɹ�
			RESULT_TYPE_NO_SPACE = -1,				// ӵ�н�ɫ̫�࣬û�пռ��ٴ���ɫ
			RESULT_TYPE_EXIST_NAME = -2,			// ����
			RESULT_TYPE_NAME_INVALID = -3,			// ���ֲ��Ϸ�
			RESULT_TYPE_SERVER_LIMIT = -4,			// ��������ֹ������ɫ
		};

		SCCreateRoleAck();
		MessageHeader		header;
		int					result;
		int					role_id;
		GameName			role_name;
		char				avatar;
		char				sex;
		char				prof;
		char				resvered;
		int					level;
		unsigned int		create_time;
	};

	class CSDestroyRole
	{
	public:
		CSDestroyRole();
		MessageHeader		header;

		PlatName			pname;
		unsigned int		login_time;
		LoginStr			login_str;
		char				anti_wallow;
		char				reserved1;
		short				server;

		int					role_id;
	};

	class SCDestroyRoleAck
	{
	public:
		SCDestroyRoleAck();
		MessageHeader		header;
		int					role_id;
		int					result;
	};

	class CSChangePlatName
	{
	public:
		CSChangePlatName();
		MessageHeader		header;

		int server;
		PlatName			old_name;
		PlatName			new_name;
	};

	class SCChangePlatNameAck
	{
	public:
		SCChangePlatNameAck();
		MessageHeader		header;

		int					result;
	};
}

#pragma pack(pop)

#endif
