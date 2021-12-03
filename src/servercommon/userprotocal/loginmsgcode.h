#ifndef __LOGINMAGCODE_H__
#define __LOGINMAGCODE_H__

namespace Protocol
{
	enum
	{
		/*LOGIN_SC_BEGIN = 7000,
		LOGIN_CS_BEGIN = 7050,*/

		REGISTER_SC_BEGIN = 7100,
		REGISTER_CS_BEGIN = 7150,
	};

	enum _MsgTypeLogin
	{
		
		///////////////////////////////////////// ��¼��������� /////////////////////////////////////////
		MT_LOGIN_ACK_SC = 7000,						// ��½���󷵻�
		MT_ROLE_LIST_SC = 7001,						// ��ɫ�б�
		MT_GET_THREAD_ACK_SC = 7002,				// �鿴������״̬����
		MT_ANTI_WALLOW_EX_RET_SC = 7003,			// ��������֤�����Է���
		MT_ACCOUNT_KEY_ERROR_SC = 7004,				// KEY����֪ͨ
		MT_MERGE_ROLE_LIST_SC = 7005,				// �Ϸ���Ľ�ɫ�б�
		MT_PROF_NUM_INFO_SC = 7006,					// ְҵ������Ϣ��
		MT_L_HEARTBEAT_SC = 7007,					// ����������

		//MT_LOGIN_REQ_CS = 7050,					// ��½����
		//MT_ROLE_REQ_CS = 7051,					// ��ɫ��¼����
		MT_L_HEARTBEAT_CS = 7052,					// ������
		MT_GET_TREAD_CS = 7053,						// �鿴������������Ϣ
		MT_ANTI_WALLOW_CS = 7054,					// ͨ��������
		MT_ANTI_WALLOW_EX_CS = 7055,				// �����Լ���
		MT_NEW_LOGIN_REQ_CS = 7056,					// �µ�¼����
		MT_NEW_ROLE_REQ_CS = 7057,					// �½�ɫ��¼����
		
		///////////////////////////////////////// ��ɫ��� /////////////////////////////////////////
		MT_CREATE_ROLE_ACK_SC = 7100,				// ������ɫ����
		MT_DESTROY_ROLE_ACK_SC = 7101,				// ���ٽ�ɫ����
		MT_CHANGE_PLATNAME_ACK_SC = 7102,			// �޸�ƽ̨������ ���ض�ģʽ��ʹ�ã�

		MT_CREATE_ROLE_REQ_CS = 7150,				// ������ɫ
		MT_DESTROY_ROLE_REQ_CS = 7151,				// ���ٽ�ɫ
		MT_CHANGE_PLATNAME_REQ_CS = 7152,			// �޸�ƽ̨��
	};
}

#endif // __LOGINMAGCODE_H__
