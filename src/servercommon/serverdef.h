
#ifndef SERVERDEF_H
#define SERVERDEF_H

#include "common/vector2d.h"

#define INVALID_OBJ_ID		((ObjID)(-1))
typedef unsigned short		ObjID;
typedef unsigned short		SceneIndex;
typedef int					BusinessGoodID;
typedef int					GuildID;
typedef unsigned short		ItemID;

typedef int					Axis;
typedef TVector2<Axis>		Posi;

typedef char GameName[32];
typedef char GuildName[32];
typedef char GuildNotice[256];
typedef char SessionKey[32];
typedef char TitleName[256];
typedef char MarriageSeekingNotice[128];
typedef char SpaceMessage[64];
typedef char TuodanNotice[64];
typedef char LoveContractNotice[64];

typedef char ReplyInfo[384];
typedef char Signature[64];
typedef char UserDefinedReply[1536];

static const int MAX_DAYCOUNT_COUNT_ID = 256;
typedef char DayCountData[MAX_DAYCOUNT_COUNT_ID + 1];  //  需要增长的话直接改该数字 然后修改数据库字段长度 其他地方已经检查过不用进行任何修改

static const int HANGUPSET_MAX_SIZE = 124;
typedef char HangupSetData[HANGUPSET_MAX_SIZE];

static const int CUSTOMSET_MAX_SIZE = 60;
typedef char CustomSetData[CUSTOMSET_MAX_SIZE];

typedef char SublockMD5[32];
typedef char SublockString[sizeof(SublockMD5) + 1];

typedef char LanternQuestionAnswer[32];
#define _cstring char*

enum SEX
{
	FEMALE = 0,
	MALE,
	SEX_MAX,
};

const int MAX_MSG_SIZE = 4 * 1024 * 1024;		// 最大的消息包绝对不可能超过 4M

struct GSNetID
{
	GSNetID():gateway_netid(-1), netid(-1){}
	GSNetID(const GSNetID &v):gateway_netid(v.gateway_netid), netid(v.netid) {}
	GSNetID(unsigned short gn, unsigned short nid):gateway_netid(gn), netid(nid){}
	bool operator<(const GSNetID& right) const
	{
		return gateway_netid < right.gateway_netid || (gateway_netid == right.gateway_netid && netid < right.netid);
	}
	bool operator==(const GSNetID &right) const
	{
		return gateway_netid == right.gateway_netid && netid == right.netid;
	}
	bool operator!=(const GSNetID &right) const
	{
		return gateway_netid != right.gateway_netid || netid != right.netid;
	}
	unsigned short gateway_netid;
	unsigned short netid;
};

static const GSNetID INVALID_GSNETID(-1, -1);

struct UserID
{
	UserID():db_index(-1), role_id(0) {}
	UserID(const UserID &v) : db_index(v.db_index), role_id(v.role_id) {}
	UserID(int dbi, int rid) : db_index(dbi), role_id(rid) {}

	bool operator<(const UserID& right) const
	{
		return db_index < right.db_index || (db_index == right.db_index && role_id < right.role_id);
	}

	bool operator==(const UserID &right) const
	{
		return db_index == right.db_index && role_id == right.role_id;
	}

	bool operator!=(const UserID &right) const
	{
		return db_index != right.db_index || role_id != right.role_id;
	}

	int db_index;
	int role_id;
};

static const UserID INVALID_USER_ID(-1, 0);
static const UserID VIRTUAL_USER_ID(0, 0);

#endif

