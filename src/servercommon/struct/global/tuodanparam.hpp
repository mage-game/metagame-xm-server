#ifndef __TUODAN_PARAM_HPP__
#define __TUODAN_PARAM_HPP__

#include "servercommon/basedef.h"
#include "servercommon/serverdef.h"
#include "servercommon/tuodandef.hpp"
#include "common/tlvprotocol.h"

class TuodanParam
{
public:
	bool Serialize(TLVSerializer &outstream) const;
	bool Unserialize(TLVUnserializer &instream);

	struct DBTuodanItem
	{
		char change_state;
		int index;

		int uid;
		unsigned int create_time;
		TuodanNotice notice;
	};

	int count;
	DBTuodanItem tuodan_list[MAX_ONCE_TUODAN_COUNT];
};

#endif  // __TUODAN_PARAM_HPP__
