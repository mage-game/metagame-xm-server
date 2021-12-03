/******************************************************************************************
FileName: blacklistsparam.hpp
Author: shell
Description: 黑名单在数据库中的定义 
Version: 1.0
History: 
<author>    <time>					<version >    <desc>
shell       2011/08/01/ 16:04:05     1.0           创建
*****************************************************************************************/

#ifndef RELATIONBLACKLISTSPARSM_H
#define RELATIONBLACKLISTSPARSM_H

#include "structcommon.h"
#include "servercommon/basedef.h"
#include "common/tlvprotocol.h"
#include "servercommon/serverdef.h"
#include "servercommon/frienddef.hpp"

class BlacklistsParam
{
public:
	bool Serialize(TLVSerializer &outstream) const;
	bool Unserialize(TLVUnserializer &instream);

	struct Black
	{
		char			change_state;

		int				index;
		int				uid;
		GameName		gamename;
		char			sex;
		char			prof;
	};

	int count;
	Black black_list[MAX_BLACK_NUM];
};

#endif
