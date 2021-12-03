/******************************************************************************************
FileName: friendparam.h
Author: 
Description: ���������ݿ��еĶ��� 
Version: 1.0
History: 
<author>    <time>					<version >    <desc>
shell       2011/08/01/ 16:04:01     1.0           ά��
*****************************************************************************************/

#ifndef FRIENDPARSM_H
#define FRIENDPARSM_H

#include "structcommon.h"
#include "servercommon/basedef.h"
#include "common/tlvprotocol.h"
#include "servercommon/serverdef.h"
#include "servercommon/frienddef.hpp"

class FriendListParam
{
public:
	bool Serialize(TLVSerializer &outstream) const;
	bool Unserialize(TLVUnserializer &instream);

	struct Friend 
	{
		char			change_state;

		int				index;
		int				friend_uid;
		int				intimacy;
		int				team_today_intimacy;
		int				gift_count;
		int				favorable_impression;
		int				day_chat_count;
		long long		add_timestamp;			// ��Ӻ���ʱ��
	};

	int count;
	Friend friend_list[MAX_FRIEND_NUM];
};

#endif

