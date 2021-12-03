#ifndef __MSG_SC_CAMP_HPP_
#define __MSG_SC_CAMP_HPP_

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/serverdef.h"
#include "servercommon/campdef.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	class SCCampInfo
	{
	public:
		SCCampInfo();
		MessageHeader		header;

		struct CampInfoItem
		{
			int camp_type;
			int camp_role_count;
			long long total_capability;
		};

		CampInfoItem	camp_item_list[CAMP_TYPE_MAX];
	};

	class SCCurDaoQuZhenBaoRefreshCamp
	{
	public:
		SCCurDaoQuZhenBaoRefreshCamp();
		MessageHeader header;

		int cur_refresh_camp;
	};
}

#pragma pack(pop)

#endif  // __MSG_SC_CAMP_HPP_
