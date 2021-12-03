#ifndef __LGPROTOCAL_H__
#define __LGPROTOCAL_H__

#pragma pack(push) 
#pragma pack(4)

// loginserver && gameworld 的交互
#include "servercommon/logindef.h"

namespace lgprotocal
{

	enum _MsgType
	{
		MT_INVALID,

		MT_REGISTER_GS = 23000,			// gameworld注册到登录服务器
		MT_LG_REGISTER_GS,				// 登录服务器通知gameworld有其他gameworld注册
		MT_LG_UNREGISTER_GS,			// 登录服务器通知gameworld有其他gameworld反注册
		MT_ROLE_FORBID,					// gameworld通知login封号
		MT_KICK_ROLE,					// login封号后通知gameworld
	};

	struct MessageHeader
	{
		MessageHeader() : msg_type(MT_INVALID) {}
		MessageHeader(int type) : msg_type(type) {}

		int msg_type;
	};

	#define MAX_SCENE_LIST_NUM	2048

	struct GLRegisterGS
	{
		GLRegisterGS() : header(MT_REGISTER_GS) {}
		MessageHeader header;

		int index;
		char ip[16];
		unsigned short port;
		unsigned short scene_count;
		int	scene_list[MAX_SCENE_LIST_NUM];
	};

	struct LGRegisterGS 
	{
		LGRegisterGS() : header(MT_LG_REGISTER_GS) {}
		MessageHeader header;

		int index;
		unsigned short reserved;
		unsigned short scene_count;
		int scene_list[MAX_SCENE_LIST_NUM];
	};

	struct LGUnRegisterGS
	{
		LGUnRegisterGS() : header(MT_LG_UNREGISTER_GS) {}
		MessageHeader header;

		unsigned short reserved;
		unsigned short scene_count;
		int scene_list[MAX_SCENE_LIST_NUM];
	};

	struct RoleForbid
	{
		RoleForbid() : header(MT_ROLE_FORBID) {}
		MessageHeader header;

		PlatName plat_name;			// 被封用户平台名
		char reason[64];			// 原因
		int forbit_time_s;			// 禁号时长
	};

	struct LGKickRole
	{
		LGKickRole() : header(MT_KICK_ROLE) {}
		MessageHeader header;
		
		int role_id;				// 角色ID
	};
}

#pragma pack(pop)

#endif // __LGPROTOCAL_H__

