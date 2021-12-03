#ifndef __LWPROTOCAL_H__
#define __LWPROTOCAL_H__

#pragma pack(push) 
#pragma pack(4)

// 登陆服务器跟网关通讯

namespace lwprotocal
{
	enum _MsgType
	{
		MT_INVALID,

		MT_REGISTER_GW,						// 网关注册到登录服务器
		MT_LW_REGISTER_GS,					// 登录服务器通知网关 有gameworld注册
	};

	struct MessageHeader
	{
		MessageHeader() : msg_type(MT_INVALID) {}
		MessageHeader(int type) : msg_type(type) {}

		int msg_type;
	};

	struct WLRegisterGW
	{
		WLRegisterGW() : header(MT_REGISTER_GW) {}		// 网关注册到登录服 告诉登陆服 网关连接信息
		MessageHeader header;

		char hostname[64];					// 修改该长度的同时 必须修改Protocol::SCLoginAck::gs_hostname的长度
		unsigned short port;
		unsigned short reserved;
	};

	struct LWOnGSRegister 
	{
		LWOnGSRegister() : header(MT_LW_REGISTER_GS) {}  // 告诉网关 去连接gamesvr 
		MessageHeader header;

		int index;
		char ip[16];
		unsigned short port;
		unsigned short reserved;
	};
}

#pragma pack(pop)

#endif // __LWPROTOCAL_H__

