#ifndef __WGPROTOCAL_H__
#define __WGPROTOCAL_H__

#include <string.h>
#include "servercommon/serverdef.h"

#pragma pack(push) 
#pragma pack(4)

// Gateway 与 后端 服务器的交互协议 

namespace wgprotocal
{
	enum _MsgType
	{
		MT_INVALID,

		MT_NET_RECV_MSG,							// 有用户消息到达					
		MT_NET_DISCONNECT_NOTICE,					// 断线通知

		MT_NET_SEND_MSG,							// 给用户发消息					
		MT_NET_MUL_SEND_MSG,						// 给多个用户发消息
		MT_NET_DISCONNECT,							// 断开用户
		MT_HAS_CHECK,								// 确认用户已登录 广播消息才允许推送
		MT_NET_SEND_ALL,							// 整个网关广播
	};

	struct MessageHeader
	{
		MessageHeader() : msg_type(MT_INVALID) {}
		MessageHeader(char type) : msg_type(type), netid(-1) {}

		short msg_type;
		unsigned short netid;
		IP user_ip;					// 实际用户IP
	};

	struct WGNetRecvMsg
	{
		WGNetRecvMsg() : header(MT_NET_RECV_MSG) {}
		MessageHeader header;
		// 从该地址开始为收到消息的内容
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
		// 后面紧更着消息内容
	};

	struct GWNetSendAll
	{
		GWNetSendAll() : header(MT_NET_SEND_ALL) {}
		MessageHeader header;
		// 后面紧更着消息内容
	};

	struct GWNetMulSendMsg
	{
		GWNetMulSendMsg() : header(MT_NET_MUL_SEND_MSG) {}
		MessageHeader header;
		int	count;
		// 后面紧跟着unsigned short netid的数组
		// 后面紧跟着消息内容
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

