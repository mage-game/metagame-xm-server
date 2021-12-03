#ifndef __GCPROTOCAL_H__
#define __GCPROTOCAL_H__

#pragma pack(push) 
#pragma pack(4)

/*
	ChatServer ���������

	�ṩ���ܣ��ṩ����Ƶ�� �����û� �����û���Ƶ���� ��Ƶ��ɾ���û� �����û�������Ƶ���ȹ���
	���Ʒ����������û� �û�����ChatServer(����˳���ϸ�Ҫ��)�� �û���Ϣ����Ƶ���й㲥 

	���Ʒ�����ͨ��Ƶ��ID���û�ID�������û� ֮����ChatServer֪ͨ�û�����(�˳�)ĳ��Ƶ��

	ע�⣺Ƶ��ID���û�ID���ɿ��Ʒ�����ָ���� Ψһ���ɿ��Ʒ�������֤
*/

#include "servercommon/chatserverdef.h"

namespace gcprotocal
{
	// MAX_GCPROTOCAL_LENGTH 1024 ������servercommon.h��
	enum _MsgType
	{
		MT_INVALID,

		MT_GC_BEGIN = 20000,
		MT_CREATE_USER,							// ����һ�������û�
		MT_DESTROY_USER,						// ����һ�������û�
		MT_CHANGE_USER_INFO,					// �û���Ϣ�ı�ͬ��
		MT_CREATE_CHANNEL,						// ����һ������Ƶ��
		MT_DESTROY_CHANNEL,						// ����һ������Ƶ��
		MT_ADD_USER_TO_CHANNEL,					// �û���������Ƶ��
		MT_DELETE_USER_FROM_CHANNEL,			// �û��뿪����Ƶ��
		MT_MUTE_USER,							// �û���ĳ���û�����
		MT_UNMUTE_USER,							// �û���ĳ���û��������
		MT_ADD_ONLINE_NOTICE,					// ��ע������ʾ
		MT_DELETE_ONLINE_NOTICE,				// ȡ��������ʾ
		MT_CHANNEL_FORBID,						// ����
		MT_GUILD_SYSTEM_MSG,					// ����ϵͳ��Ϣ
		MT_RESET_USER_NAME,						// ��ɫ����
		MT_RELOAD_CONFIG,						// �ض�����
		MT_PRINT_STATUS,						// �������
		MT_SINGLE_CHAT_MONITOR,					// ˽�ļ����Ϣ
		MT_WORLD_CHAT_MONITOR,					// ����Ƶ����������Ϣ
		MT_GUILD_CHAT_MONITOR,					// ����Ƶ����������Ϣ
	};

	struct MessageHeader
	{
		MessageHeader() : msg_type(MT_INVALID) {}
		MessageHeader(int type) : msg_type(type) {}

		int	msg_type;
	};

	struct CreateUser
	{
		CreateUser() : header(MT_CREATE_USER) {}
		MessageHeader header;

		UID	uid;
		UserKey key;
		UserName name;
	};

	struct DestroyUser
	{
		DestroyUser() : header(MT_DESTROY_USER) {}
		MessageHeader header;

		UID	uid;
	};

	struct ChangeUserInfo
	{
		ChangeUserInfo() : header(MT_CHANGE_USER_INFO) {}
		MessageHeader	header;

		UID uid;
		char avatar;
		char sex;
		char camp;
		char vip_type;
		char camp_post;
		char guild_post;
		char prof;
		char authority_type;
		short level;
		short vip_level;
	};

	struct CreateChannel
	{
		CreateChannel() : header(MT_CREATE_CHANNEL) {}
		MessageHeader header;

		ChannelID cid;
		unsigned int cooldown;
		ChannelType	type;
		ChannelName name;
	};

	struct DestroyChannel
	{
		DestroyChannel() : header(MT_DESTROY_CHANNEL) {}
		MessageHeader header;

		ChannelID cid;
	};

	struct AddUserToChannel
	{
		AddUserToChannel() : header(MT_ADD_USER_TO_CHANNEL) {}
		MessageHeader header;

		UID	uid;
		ChannelID cid;
	};

	struct DeleteUserFromChannel
	{
		DeleteUserFromChannel() : header(MT_DELETE_USER_FROM_CHANNEL) {}
		MessageHeader header;

		UID	uid;
		ChannelID cid;
	};

	struct MuteUser
	{
		MuteUser() : header(MT_MUTE_USER) {}
		MessageHeader header;

		UID uid;
		UID mute_uid;
	};

	struct UnMuteUser
	{
		UnMuteUser() : header(MT_UNMUTE_USER) {}
		MessageHeader header;

		UID uid;
		UID	unmute_uid;
	};

#define MAX_NOTICE_NUM	1024

	struct AddOnlineNotice
	{
		AddOnlineNotice() : header(MT_ADD_ONLINE_NOTICE) {}
		MessageHeader header;

		UID uid;
		int	count;
		UID	notice_list[MAX_NOTICE_NUM];
	};

	struct DeleteOnlineNotice
	{
		DeleteOnlineNotice() : header(MT_DELETE_ONLINE_NOTICE) {}
		MessageHeader header;

		UID uid;
		UID	d_notice_uid;
	};

	struct ChannelForbid
	{
		ChannelForbid() : header(MT_CHANNEL_FORBID) {}
		MessageHeader header;

		UID	uid;
		unsigned int forbid_time;
	};

	struct PrintStatus
	{
		PrintStatus() : header(MT_PRINT_STATUS) {}
		MessageHeader header;
	};

	struct GuildSystemMsg 
	{
		GuildSystemMsg() : header(MT_GUILD_SYSTEM_MSG) {}
		MessageHeader header;

		ChannelID channel_id;
		int msg_length;
		char msg[MAX_CHAT_LENGTH];
	};

	struct ResetUserName
	{
		ResetUserName():header(MT_RESET_USER_NAME){}
		MessageHeader	header;

		UID				uid;
		UserName		name;
	};

	struct ChatReloadConfig
	{
		ChatReloadConfig():header(MT_RELOAD_CONFIG){}
		MessageHeader	header;
	};

	struct SingleChatMonitor
	{
		SingleChatMonitor():header(MT_SINGLE_CHAT_MONITOR){}
		MessageHeader	header;

		UID				uid;
		unsigned int	msg_time;

		int				msg_length;
		char			msg[MAX_CHAT_LENGTH];	
	};

	struct WorldChatMonitor
	{
		WorldChatMonitor():header(MT_WORLD_CHAT_MONITOR){}
		MessageHeader	header;

		UID				uid;
		unsigned int	msg_time;

		int				msg_length;
		char			msg[MAX_CHAT_LENGTH];	
	};

	struct GuildChatMonitor
	{
		GuildChatMonitor():header(MT_GUILD_CHAT_MONITOR){}
		MessageHeader	header;

		UID				uid;
		unsigned int	msg_time;

		int				msg_length;
		char			msg[MAX_CHAT_LENGTH];	
	};
}

#pragma pack(pop)

#endif


