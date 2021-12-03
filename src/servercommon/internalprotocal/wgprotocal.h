#ifndef __WGPROTOCAL_H__
#define __WGPROTOCAL_H__

#include <string.h>
#include "servercommon/serverdef.h"

#pragma pack(push) 
#pragma pack(4)

// Gateway �� ��� �������Ľ���Э�� 

namespace wgprotocal
{
	enum _MsgType
	{
		MT_INVALID,

		MT_NET_RECV_MSG,							// ���û���Ϣ����					
		MT_NET_DISCONNECT_NOTICE,					// ����֪ͨ

		MT_NET_SEND_MSG,							// ���û�����Ϣ					
		MT_NET_MUL_SEND_MSG,						// ������û�����Ϣ
		MT_NET_DISCONNECT,							// �Ͽ��û�
		MT_HAS_CHECK,								// ȷ���û��ѵ�¼ �㲥��Ϣ����������
		MT_NET_SEND_ALL,							// �������ع㲥
	};

	struct MessageHeader
	{
		MessageHeader() : msg_type(MT_INVALID) {}
		MessageHeader(char type) : msg_type(type), netid(-1) {}

		short msg_type;
		unsigned short netid;
		IP user_ip;					// ʵ���û�IP
	};

	struct WGNetRecvMsg
	{
		WGNetRecvMsg() : header(MT_NET_RECV_MSG) {}
		MessageHeader header;
		// �Ӹõ�ַ��ʼΪ�յ���Ϣ������
	};

	struct WGNetDisconnect
	{
		WGNetDisconnect() : header(MT_NET_DISCONNECT_NOTICE) {}
		MessageHeader header;
	};

	struct GWNetSendMsg
	{
		GWNetSendMsg() : header(MT_NET_SEND_MSG) {}
		MessageHeader header;
		// �����������Ϣ����
	};

	struct GWNetSendAll
	{
		GWNetSendAll() : header(MT_NET_SEND_ALL) {}
		MessageHeader header;
		// �����������Ϣ����
	};

	struct GWNetMulSendMsg
	{
		GWNetMulSendMsg() : header(MT_NET_MUL_SEND_MSG) {}
		MessageHeader header;
		int	count;
		// ���������unsigned short netid������
		// �����������Ϣ����
	};

	struct GWNetDisconnect
	{
		GWNetDisconnect() : header(MT_NET_DISCONNECT), uid(0), scene_id(-1)
		{
			memset(role_name, 0, sizeof(role_name));
			memset(server_name, 0, sizeof(server_name));
			memset(reason, 0, sizeof(reason));
		}

		MessageHeader	header;

		int				uid;
		GameName		role_name;
		char			server_name[32];
		int				scene_id;
		char			reason[128];
	};

	struct GWHasCheck
	{
		GWHasCheck() : header(MT_HAS_CHECK) {}
		MessageHeader header;
	};

}

#pragma pack(pop)

#endif

