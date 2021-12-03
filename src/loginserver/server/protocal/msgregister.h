
#ifndef MSGREGISTER_H
#define MSGREGISTER_H

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/logindef.h"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	/*
		创建和销毁角色相关
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
		char				prof;				// 职业
		short				reserved2;
		int					plat_spid;			// 平台的渠道ID
	};

	class SCCreateRoleAck
	{
	public:
		enum RET_TYPE
		{
			RESULT_TYPE_SUCC = 0,					// 成功
			RESULT_TYPE_NO_SPACE = -1,				// 拥有角色太多，没有空间再创角色
			RESULT_TYPE_EXIST_NAME = -2,			// 重名
			RESULT_TYPE_NAME_INVALID = -3,			// 名字不合法
			RESULT_TYPE_SERVER_LIMIT = -4,			// 服务器禁止创建角色
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
