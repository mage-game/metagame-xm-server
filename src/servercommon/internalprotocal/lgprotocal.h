#ifndef __LGPROTOCAL_H__
#define __LGPROTOCAL_H__

#pragma pack(push) 
#pragma pack(4)

// loginserver && gameworld �Ľ���
#include "servercommon/logindef.h"

namespace lgprotocal
{

	enum _MsgType
	{
		MT_INVALID,

		MT_REGISTER_GS = 23000,			// gameworldע�ᵽ��¼������
		MT_LG_REGISTER_GS,				// ��¼������֪ͨgameworld������gameworldע��
		MT_LG_UNREGISTER_GS,			// ��¼������֪ͨgameworld������gameworld��ע��
		MT_ROLE_FORBID,					// gameworld֪ͨlogin���
		MT_KICK_ROLE,					// login��ź�֪ͨgameworld
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

		PlatName plat_name;			// �����û�ƽ̨��
		char reason[64];			// ԭ��
		int forbit_time_s;			// ����ʱ��
	};

	struct LGKickRole
	{
		LGKickRole() : header(MT_KICK_ROLE) {}
		MessageHeader header;
		
		int role_id;				// ��ɫID
	};
}

#pragma pack(pop)

#endif // __LGPROTOCAL_H__

