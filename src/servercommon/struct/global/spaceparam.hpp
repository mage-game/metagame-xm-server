#ifndef __SPACE_PARAM_HPP__
#define __SPACE_PARAM_HPP__

#include "common/tlvprotocol.h"
#include "servercommon/serverdef.h"

static const int SPACE_MESSAGE_MAX = 50;							// 空间消息最大条数 

static const int SPACE_LIUYAN_MAX = 50;								// 空间留言最大条数 
static const int SPACE_CAI_MAX = 10;								// 踩一踩 最大条数 
static const int SPACE_GIFT_MAX = 10;								// 送礼 最大条数 

static const int SPACE_TIMEOUT_S = 7 * 24 * 60 * 60;				// 空间的记录保存7天

enum SPACE_RECORED_TYPE
{
	SPACE_RECORED_TYPE_IVNALID = 0,

	SPACE_RECORED_TYPE_VISTOR,										// 访客
	SPACE_RECORED_TYPE_GIFT,										// 送礼
	SPACE_RECORED_TYPE_MSG,											// 消息

	SPACE_RECORED_TYPE_MAX,
};

struct SpaceRecord
{
	long long id_record;						// 仅仅初始化的时候有用 
	char change_state; 

	SpaceRecord() 
		: id_record(0), change_state(0), record_unq_id(0), owner_uid(0), from_uid(0), to_uid(0), 
		happen_timestamp(0), happen_space_uid(0), type(SPACE_RECORED_TYPE_IVNALID), is_huifu(0)
	{
		memset(msg, 0, sizeof(msg));
	}

	bool CanRemove(unsigned int now_second)
	{
		return now_second >= happen_timestamp + SPACE_TIMEOUT_S;
	}

	long long record_unq_id;
	int owner_uid;

	int from_uid;
	int to_uid;

	unsigned int happen_timestamp;
	int happen_space_uid;
	short type;
	short is_huifu;
	SpaceMessage msg;
};

class SpaceParam
{
public:
	bool Serialize(TLVSerializer &outstream) const;
	bool Unserialize(TLVUnserializer &instream);

	static const int SPACE_MAX_RECORD_COUNT = 1024;

	int count;
	SpaceRecord record_list[SPACE_MAX_RECORD_COUNT];
};


#endif  // __OFFLINE_REGISTER_PARAM_HPP__

