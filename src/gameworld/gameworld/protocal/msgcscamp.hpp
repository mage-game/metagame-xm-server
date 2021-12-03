#ifndef __MSG_CS_CAMP_HPP__
#define __MSG_CS_CAMP_HPP__

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/serverdef.h"
#include "servercommon/campdef.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	class CSGetCampInfo
	{
	public:
		CSGetCampInfo();
		MessageHeader		header;
	};
}

#pragma pack(pop)

#endif  // __MSG_CS_CAMP_HPP__
