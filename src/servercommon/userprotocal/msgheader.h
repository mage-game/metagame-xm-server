#ifndef __MSGHEADER_H__
#define __MSGHEADER_H__

#include "servercommon/basedef.h"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	enum
	{
		MT_INVALID = 0,
	};

	class MessageHeader
	{
	public:
		MessageHeader() : msg_type(MT_INVALID) {}
		MessageHeader(Int32 type) : msg_type(type), server_id(-1) {}

		unsigned short msg_type;
		unsigned short server_id;
	};
}

#pragma pack(pop)

#endif // __MSGHEADER_H__

