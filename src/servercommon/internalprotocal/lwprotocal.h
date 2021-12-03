#ifndef __LWPROTOCAL_H__
#define __LWPROTOCAL_H__

#pragma pack(push) 
#pragma pack(4)

// ��½������������ͨѶ

namespace lwprotocal
{
	enum _MsgType
	{
		MT_INVALID,

		MT_REGISTER_GW,						// ����ע�ᵽ��¼������
		MT_LW_REGISTER_GS,					// ��¼������֪ͨ���� ��gameworldע��
	};

	struct MessageHeader
	{
		MessageHeader() : msg_type(MT_INVALID) {}
		MessageHeader(int type) : msg_type(type) {}

		int msg_type;
	};

	struct WLRegisterGW
	{
		WLRegisterGW() : header(MT_REGISTER_GW) {}		// ����ע�ᵽ��¼�� ���ߵ�½�� ����������Ϣ
		MessageHeader header;

		char hostname[64];					// �޸ĸó��ȵ�ͬʱ �����޸�Protocol::SCLoginAck::gs_hostname�ĳ���
		unsigned short port;
		unsigned short reserved;
	};

	struct LWOnGSRegister 
	{
		LWOnGSRegister() : header(MT_LW_REGISTER_GS) {}  // �������� ȥ����gamesvr 
		MessageHeader header;

		int index;
		char ip[16];
		unsigned short port;
		unsigned short reserved;
	};
}

#pragma pack(pop)

#endif // __LWPROTOCAL_H__

