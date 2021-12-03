#ifndef __CHATMSGCODE_H__
#define __CHATMSGCODE_H__

namespace Protocol
{
	enum
	{
		CHATCTRL_SC_BEGIN = 8000,
		CHATCTRL_CS_BEGIN = 8050,

		CHATMSG_SC_BEGIN = 8100,
		CHATMSG_CS_BEGIN = 8150,
	};

	enum _MsgTypeChat
	{
		MT_USER_LOGIN_ACK_SC = CHATCTRL_SC_BEGIN,			// �û���¼����
		MT_USER_BE_DESTROY_SC,								// ֪ͨ�û�������
		MT_INTO_CHANNEL_SC,									// �û�����Ƶ��
		MT_OUT_OF_CHANNEL_SC,								// �û��뿪Ƶ��
		MT_NOTICE_ONLINE_SC,								// ��������
		MT_NOTICE_OFFLINE_SC,								// ��������
		MT_NOTICE_ONLINE_STATUS_SC,							// ����״̬�仯����		

		MT_USER_LOGIN_CS = CHATCTRL_CS_BEGIN,				// �û������¼���������
		MT_MUTE_USER_CS,									// ����ĳ�û�
		MT_UNMUTE_USER_CS,									// ȡ������
		MT_CHANGE_ONLINE_STATUS_CS,							// �û��������״̬

		////////////////////////////////////////////////////////////////////////////////////////////////////////

		MT_CHANNEL_CHAT_SC = 8100,							// Ƶ�����췵��
		MT_SINGLE_CHAT_SC = 8101,							// ˽�ķ���
		MT_SINGLE_CHAT_USET_NOT_EXIST_SC = 8102,			// ˽�� ��ʾ�û�������
		MT_CHAT_LEVEL_LIMIT_SC = 8103,						// ����ȼ�������ʾ
		MT_FORBID_CHAT_INFO_SC = 8104,						// ��ֹ������Ϣ
		MT_FORBID_TALK_USER_INFO_SC = 8105,					// ��ɫ������Ϣ
		MT_CHAT_BOARD_LIST_SC = 8106,						// �·���ɫ�����¼�б�
		MT_SINGLE_CHAT_INFO_SC = 8107,						// �·���ɫ˽��״̬

		MT_CHANNEL_CHAT_CS = 8150,							// ����Ƶ������
		MT_SINGLE_CHAT_CS = 8151,							// ����˽��
		MT_FORBID_CHAT_INFO_CS = 8152,						// �������б�
		MT_SINGLE_CHAT_REQ_CS = 8153,						// ��ɫ˽�Ĳ���
	};
}

#endif // __CHATMSGCODE_H__

