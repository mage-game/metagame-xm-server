#ifndef __FISH_POOL_PARAM_HPP__
#define __FISH_POOL_PARAM_HPP__

#include "servercommon/fishpooldef.hpp"
#include "common/tlvprotocol.h"

class FishPoolListParam
{
public:
	bool Serialize(TLVSerializer &outstream) const;
	bool Unserialize(TLVUnserializer &instream);

	struct FishPoolAttr
	{
		FishPoolAttr() : id_fishpool(0), change_state(0), role_id(0) {}

		long long id_fishpool;			// 仅仅初始化的时候有用 

		char change_state;
		int role_id;

		FishPoolCommonDataParam common_data;
	};

	FishPoolListParam() : count(0) {}

	int count;
	FishPoolAttr fish_pool_list[FISH_POOL_ONCE_LOAD_MAX_COUNT];
};

#endif


