#ifndef __FRIEND_EXP_BOTTLE_PARAM_HPP__
#define __FRIEND_EXP_BOTTLE_PARAM_HPP__

#include "servercommon/servercommon.h"

#pragma pack(push) 
#pragma pack(4)

struct FriendExpBottleParam
{
	FriendExpBottleParam() { this->Reset(); }

	void Reset()
	{
		get_exp_count = 0;
		state = 0;
		next_add_bottle_exp_time = 0;
		exp = 0;
		next_broadcast_time = 0;
		auto_add_count = 0;
	}

	int get_exp_count;
	int state;
	unsigned int next_add_bottle_exp_time;
	unsigned long long exp;
	unsigned int next_broadcast_time;
	int auto_add_count;
};

#pragma pack(pop)

typedef char FriendExpBottleParamHex[sizeof(FriendExpBottleParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(FriendExpBottleParamHex) < 64);

#endif // __FRIEND_EXP_BOTTLE_PARAM_HPP__