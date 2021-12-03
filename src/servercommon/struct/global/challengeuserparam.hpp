#ifndef __CHALLENGE_USER_PARAM_HPP__
#define __CHALLENGE_USER_PARAM_HPP__

#include "common/tlvprotocol.h"
#include "servercommon/serverdef.h"
#include "servercommon/challengefielddef.hpp"

class ChallengeUserListParam
{
public:
	bool Serialize(TLVSerializer &outstream) const;
	bool Unserialize(TLVUnserializer &instream);

	struct ChallengeUserAttr
	{
		ChallengeUserAttr() : change_state(0), rankpos(0), role_id(0) {}

		char change_state;
		int rankpos;
		long long role_id;

		ChallengeParam challenge_p;
		ChallengeUserCommonData common_data;
		ChallengeUserReport report_info;
		RoleAppearance role_appearance;
	};

	ChallengeUserListParam() : count(0) {}

	int count;
	ChallengeUserAttr user_list[CHALLENGE_FIELD_ONCE_OPER_USER_COUNT];
};

#endif  // __CHALLENGE_USER_PARAM_HPP__

