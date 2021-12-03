/******************************************************************************************
FileName: chatserverdef.hpp
Author: dreamwang
Description: �����������ض��� 
Version: 1.0
History: 
<author>    <time>					<version >    <desc>
dreamwang   2011/08/08/ 09:04:05     1.0           ����
*****************************************************************************************/

#ifndef __CHATDEF_H__
#define __CHATDEF_H__

#include "servercommon/basedef.h"
#include "servercommon/serverdef.h"

typedef unsigned int	NetID;
typedef Int32			UID;
typedef UInt32			UserKey;					// ���û������û�ʱ�����û� �û�ͬʱ����һ��UserKey ���߱������
typedef Int32			ChannelID;
typedef Int32			ChannelType;


typedef char UserName[32];
typedef char ChannelName[32];

static const int WORLD_CHANNEL_ID = 0;
static const int MAX_CHAT_LENGTH = 2048;
static const int MAX_SPEAKER_MSG_LENGTH = 512;

static const int CHANNEL_OPEN_LEVEL_DEFAULT = 10;

// static const int SINGLECHAT_OPEN_LEVEL = 140;

// ������
static const int MAX_SINGLE_CHAT_MONITOR_LENGTH = 512;
static const int MAX_GUILD_CHAT_MONITOR_LENGTH = 128;
static const int MAX_WORLD_CHAT_MONITOR_LENGTH = 128;
static const int MAX_MAIL_MONITOR_SUBJECT_LENGTH = 128;

enum OPENLEVEL_LIMIT_TYPE
{
	OPENLEVEL_LIMIT_TYPE_WORLD = 0,			// ��������
	OPENLEVEL_LIMIT_TYPE_CAMP,				// ��Ӫ����
	OPENLEVEL_LIMIT_TYPE_SCENE,				// ��������
	OPENLEVEL_LIMIT_TYPE_TEAM,				// �������
	OPENLEVEL_LIMIT_TYPE_GUILD,				// ��������
	OPENLEVEL_LIMIT_TYPE_SINGLE,			// ˽��
	OPENLEVEL_LIMIT_TYPE_SEND_MAIL,			// �ʼ�
	OPENLEVEL_LIMIT_TYPE_SPEAKER,			// ����

	OPENLEVEL_LIMIT_TYPE_MAX,
};

enum CHAT_LIMIT_CONDITION_TYPE
{
	CHAT_LIMIT_CONDITION_TYPE_AND = 0,		// ��������������
	CHAT_LIMIT_CONDITION_TYPE_OR,			// ��������������

	CHAT_LIMIT_CONDITION_TYPE_MAX
};

#endif // __CHATDEF_H__


