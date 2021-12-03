#ifndef __LGLOBALPROTOCAL_H__
#define __LGLOBALPROTOCAL_H__

#include "servercommon/serverdef.h"
#include "servercommon/logindef.h"

#pragma pack(push) 
#pragma pack(4)

// 登录服务器与全局服务器通讯

namespace lglobalprotocal
{
	enum _MsgType
	{
		MT_INVALID,

		MT_REGISTER_GL = 24000,				// 全局服务器注册到登录服务器	
		MT_USER_LOGIN_GL,					// 通知登录服务器 用户已经登录到全局服务器
		MT_USER_LOGOUT_GL,					// 通知登录服务器 用户已经退出
		MT_KICK_USER_FAIL_GL,				// 通知登录服务器 踢用户失败

		MT_KICK_USER_LG,					// 登录服务器通知全局服务器 踢人
		MT_WALLOW_NOTICE_LG,				// 登录服务器通知全局服务器 防沉迷提醒

		MT_USER_INFO_RECOVER_GL,			// 登录服务器重启后 重连恢复数据
	};

	struct MessageHeader
	{
		MessageHeader() : msg_type(MT_INVALID) {}
		MessageHeader(int type) : msg_type(type) {}

		int msg_type;
	};

	struct GlobalLRegister
	{
		GlobalLRegister() : header(MT_REGISTER_GL) {}
		MessageHeader header;

		int cur_role_num;
		int max_role_num;
		int	is_valid;
	};

	struct GlobalLUserLogin
	{
		GlobalLUserLogin() : header(MT_USER_LOGIN_GL) {}
		MessageHeader header;

		int db_index;
		int role_id;
		GameName role_name;
		int is_micro_pc;
	};

	struct GlobalLUserLogout
	{
		GlobalLUserLogout() : header(MT_USER_LOGOUT_GL) {}
		MessageHeader header;

		int db_index;
		int role_id;
		GameName role_name;
		int is_micro_pc;
	};

	struct LGlobalKickUser
	{
		LGlobalKickUser() : header(MT_KICK_USER_LG) {}
		MessageHeader header;

		int db_index;
		int role_id;
		int	reason;
	};

	struct GlobalKickUserFail
	{
		GlobalKickUserFail() : header(MT_KICK_USER_FAIL_GL) {}
		MessageHeader header;

		int db_index;
		int role_id;
	};

	struct LGlobalWloowNotice
	{
		LGlobalWloowNotice() : header(MT_WALLOW_NOTICE_LG) {}
		MessageHeader header;

		int db_index;
		int role_id;
		int notice_type;
	};

	struct UserRecoverItem
	{
		UserID	user_id;
		PlatName platname;
	};

	static const int MAX_USER_RECOVER_ITEM_NUM = 1024;

	struct GlobalLUserInfoRecover
	{
		GlobalLUserInfoRecover():header(MT_USER_INFO_RECOVER_GL){}
		MessageHeader		header;

		int					count;
		UserRecoverItem		info_list[MAX_USER_RECOVER_ITEM_NUM];
	};
}

#pragma pack(pop)

#endif

