#ifndef __DAILYFINDLISTPARAM_HPP__
#define __DAILYFINDLISTPARAM_HPP__

#include "common/tlvprotocol.h"
#include "servercommon/welfaredef.hpp"

class DailyFindListParam
{
public:
	bool Serialize(TLVSerializer &outstream) const;
	bool Unserialize(TLVUnserializer &instream);

	struct DailyFindItem
	{
		short index;
		char change_state;

		unsigned int find_time;
		short role_level;
		int find_count;
		int param;
		long long param1;
	};

	int count;
	DailyFindItem daily_find_list[DAILYFIND_TYPE_MAX * WELFARE_DAILY_FIND_MAXDAYS];
};

#endif

