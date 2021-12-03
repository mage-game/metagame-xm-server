#ifndef __MSG_BIG_CHATFACE_HPP__
#define __MSG_BIG_CHATFACE_HPP__

#include "servercommon/bigchatfacedef.hpp"
#include "servercommon/userprotocal/msgheader.h"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	class SCBigChatFaceAllInfo					// 2406
	{
	public:
		SCBigChatFaceAllInfo();
		MessageHeader header;

		short big_face_level;
		short reserve_sh;
	};

	class CSBigChatFaceUpLevelReq				// 8275
	{
	public:
		CSBigChatFaceUpLevelReq();
		MessageHeader header;
	};
}

#pragma pack(pop)

#endif
