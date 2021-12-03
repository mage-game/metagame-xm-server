#ifndef MSG_XUNZHANG_H
#define MSG_XUNZHANG_H

#include "gamedef.h"
#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/struct/xunzhangparam.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	class SCAllXunZhangInfo								// 所有信息
	{
	public:
		SCAllXunZhangInfo();
		MessageHeader header;

		short level_list[MAX_XUNZHANG_COUNT];
	};

	class CSXunZhangUplevelReq
	{
	public:
		CSXunZhangUplevelReq();
		MessageHeader header;

		int xunzhang_id;
		int is_only_bind;
	};
}

#pragma pack(pop)

#endif
