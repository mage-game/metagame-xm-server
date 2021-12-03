#ifndef __MSGCHATMSG_H__
#define __MSGCHATMSG_H__

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/chatserverdef.h"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	enum CHAT_CONTENT_TYPE
	{
		CHAT_CONTENT_TYPE_TEXT,//����
		CHAT_CONTENT_TYPE_AUDIO,//�������
		CHAT_CONTENT_TYPE_AUDIO_2,//�շ�����
		CHAT_CONTENT_TYPE_SELF,//�Լ����ֻ���

		CHAT_CONTENT_TYPE_MAX,
	};

	struct CSChannelChat		// 8150	����Ƶ������
	{
		CSChannelChat();
		MessageHeader header;

		char content_type;
		char reserve_ch;
		short channel_type;		// 0�����磬1����Ӫ��2��������3����ӣ�4������, 5��������⣬ 6�����ɴ���
		int msg_length;
		// �����������Ϣ����
	};

	struct SCChannelChat		// 8100	�·�Ƶ������
	{
		SCChannelChat();
		MessageHeader header;

		int	from_uid;
		UserName username;		// char [32]

		char sex;
		char camp;
		char prof;
		char authority_type;

		char content_type;
		char tuhaojin_color;		// ��������ɫ��0 ��ʾδ����
		char bigchatface_status;	// �������飬0 ��ʾδ����
		char personalize_window_bubble_type;		// ������������ݣ���-1 ��ʾδ����
		long long avatar_timestamp;
		long long uuid;

		short level;
		char vip_level;
		char channel_type;

		short guild_signin_count;					// ������ǩ������
		char is_msg_record;							// ����
		char personalize_window_avatar_type;		// ���������(ͷ��), -1 ��ʾδ����

		char is_answer_true;						// �����Ƿ���ȷ(�������˴���)
		char reserve_ch;
		short origin_type;							// ��Դ���ͣ��ο�namespace chatdef::enum ORIGIN_TYPE
		short has_xianzunka_flag;					// ����ӵ�б�־
		short reserve_sh;

		unsigned int msg_timestamp;
		
		unsigned int msg_length;
		// �����������Ϣ����
	};

	struct CSSingleChat				// 8151	����˽��
	{
		CSSingleChat();
		MessageHeader header;

		char content_type;
		char is_echo;		// 1���Ա��(����Ϣ��ʾ���Ҳࡣ���ڷ���������˽�Ļ��ԣ���Ҫʵ�ַ�ʽ:�ԶԷ�������Լ�����)
		char special_param; // 1��������������˽��
		char reserve_sh;
		int to_uid;
		long long to_uuid;
		int msg_length;
		// �����������Ϣ����
	};

	struct CSForbidChatInfo		// 8152
	{
		CSForbidChatInfo();
		MessageHeader header;
	};

	struct SCSingleChat				// 8101	�·�˽��
	{
		SCSingleChat();
		MessageHeader header;

		int	from_uid;
		long long from_uuid;
		UserName username;			// char [32]
		char sex;
		char camp;
		char vip_level;
		char prof;
		char authority_type;
		char content_type;
		short level;
		char tuhaojin_color;		// ��������ɫ��0 ��ʾδ����
		char bigchatface_status;	// �������飬0 ��ʾδ����
		char personalize_window_bubble_type;		// ������������ݣ���-1 ��ʾδ����
		char personalize_window_avatar_type;		// ���������ͷ�񣩣�-1 ��ʾδ����
		char is_echo;				// 1���Ա��
		char special_param;			// 1�����������ĳ���˽��
		short has_xianzunka_flag;
		long long avatar_timestamp;
		unsigned int msg_timestamp;
		unsigned int msg_length;
		// �����������Ϣ����
	};

	struct SCSingleChatUserNotExist		// 8102	֪ͨĿ�겻����
	{
		SCSingleChatUserNotExist();
		MessageHeader header;

		int	to_uid;
	};

	struct SCOpenLevelLimit		// 8103
	{
		SCOpenLevelLimit();
		MessageHeader header;

		struct ForbidTimeInfo
		{
			short begin_hour;					// ��ֹ���쿪ʼʱ��
			short end_hour;						// ��ֹ�������ʱ��
		};

		int ignoreLevelLimit;
		short is_forbid_audio_chat;
		short is_forbid_change_avatar;						// �Ƿ��������ͷ��
		int chat_limit_condition_type;						// ���������������� CHAT_LIMIT_CONDITION_TYPE_AND=0:ͬʱ�����ɫ�ȼ���vip�ȼ��� CHAT_LIMIT_CONDITION_TYPE_OR=1:��������һ������
		int open_level_list[OPENLEVEL_LIMIT_TYPE_MAX];
		short vip_level_list[OPENLEVEL_LIMIT_TYPE_MAX];

		ForbidTimeInfo forbid_time_info_list[OPENLEVEL_LIMIT_TYPE_MAX];
		short is_forbid_cross_speaker;
		short reserve_sh;
	};

	struct SCForbidChatInfo		//8104 �����Ϣ
	{
		SCForbidChatInfo();
		MessageHeader header;

		static const int MAX_FORBID_UID_COUNT = 500;

		int forbid_uid_count;
		int forbid_uid_list[MAX_FORBID_UID_COUNT];
	};

	struct SCForbidUserInfo		// 8105	���˽�����Ϣ
	{
		SCForbidUserInfo();

		MessageHeader header;

		unsigned int forbid_talk_end_timestamp;
	};

	struct SCChatBoardListInfo    // 8106  �·������¼�����б�
	{
		SCChatBoardListInfo();
		MessageHeader header;

		short channel_type;		// ����Ƶ������
		short list_count;		// �����¼����
		
		// ���������������char buff���� [len_1 + buff_1 ... len_n + buff_n]
		// len_ ����int���ͣ���ʾÿһ��������Ϣ���ĳ���
		// buff_ ����ÿһ��������Ϣ��
	};

	// ˽������״̬֪ͨ----------------------------
	enum SINGLE_CHAT_REQ
	{
		SINGLE_CHAT_REQ_ADD = 0,				// ��Ӽ�����������״̬ p1:plat_type p2:target_id
		SINGLE_CHAT_REQ_DELETE = 1,				// ɾ������������������״̬ p1:plat_type p2:target_id
		SINGLE_CHAT_REQ_DELETE_ALL = 2,			// ɾ��ȫ��������������״̬ 
	};

	struct CSSingleChatReq		// 8153 ˽������
	{
		CSSingleChatReq();
		MessageHeader header;

		short req_type;
		short reserve_sh;

		int param_1;
		int param_2;
	};

	struct SCSingleChatOnlineStatus		// 8107	�������������Ϣ
	{
		SCSingleChatOnlineStatus();
		MessageHeader header;

		long long uuid = 0;					// uuid
		char online_type = 0;				// ��������
		char reserve_ch = 0;
		short reserve_sh = 0;
	};
}

#pragma pack(pop)

#endif // __MSGCHATMSG_H__

