#ifndef __BIG_CHATFACE_DEF_HPP__
#define __BIG_CHATFACE_DEF_HPP__

#include "servercommon.h"

#pragma pack(push, 4)

struct BigChatFaceParam
{
	BigChatFaceParam() { this->Reset(); }

	void Reset()
	{
		big_face_level = 0;
		reserve = 0;
	}

	short big_face_level;
	short reserve;
};

typedef char BigChatFaceParamHex[sizeof(BigChatFaceParam) * 2 + 1];	
UNSTD_STATIC_CHECK(sizeof(BigChatFaceParamHex) < 64);

#pragma pack(pop)

#endif
