#ifndef __CROSS_STATUS_PARAM_HPP__
#define __CROSS_STATUS_PARAM_HPP__

#include "common/tlvprotocol.h"
#include "servercommon/serverdef.h"
#include "servercommon/basedef.h"
#include "servercommon/crossdef.hpp"
#include "servercommon/rankdef.hpp"

class CrossStatusParam
{
public:
	bool Serialize(TLVSerializer &outstream) const;
	bool Unserialize(TLVUnserializer &instream);

	void Reset();

	unsigned int last_update_dayid;
	CrossPersonRankTitleParam cross_person_rank_title_param;
	CrossActivityParam cross_activity_param;
};

typedef char CrossPersonRankTitleParamHex[sizeof(CrossPersonRankTitleParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(CrossPersonRankTitleParamHex) < 8192);

typedef char CrossActivityParamHex[sizeof(CrossActivityParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(CrossActivityParamHex) < 2048);
#endif // __CROSS_STATUS_PARAM_HPP__

