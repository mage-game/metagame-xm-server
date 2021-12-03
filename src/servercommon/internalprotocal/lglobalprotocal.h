#ifndef __LGLOBALPROTOCAL_H__
#define __LGLOBALPROTOCAL_H__

#include "servercommon/serverdef.h"
#include "servercommon/logindef.h"

#pragma pack(push) 
#pragma pack(4)

// ��¼��������ȫ�ַ�����ͨѶ

namespace lglobalprotocal
{
	enum _MsgType
	{
		MT_INVALID,

		MT_REGISTER_GL = 24000,				// ȫ�ַ�����ע�ᵽ��¼������	
		MT_USER_LOGIN_GL,					// ֪ͨ��¼������ �û��Ѿ���¼��ȫ�ַ�����
		MT_USER_LOGOUT_GL,					// ֪ͨ��¼������ �û��Ѿ��˳�
		MT_KICK_USER_FAIL_GL,				// ֪ͨ��¼������ ���û�ʧ��

		MT_KICK_USER_LG,					// ��¼������֪ͨȫ�ַ����� ����
		MT_WALLOW_NOTICE_LG,				// ��¼������֪ͨȫ�ַ����� ����������

		MT_USER_INFO_RECOVER_GL,			// ��¼������������ �����ָ�����
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

