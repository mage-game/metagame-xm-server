
#ifndef SKILLLISTPARAM_H
#define SKILLLISTPARAM_H

#include "common/tlvprotocol.h"
#include "servercommon/servercommon.h"

class SkillListParam
{
public:
	bool Serialize(TLVSerializer &outstream) const;
	bool Unserialize(TLVUnserializer &instream);

	struct __changeskill 
	{
		char index;
		char change_state;

		unsigned short skill_id;
		int level;
		long long last_perform_time;
		int exp;
	};

	static const int MAX_SKILL_NUM = 128;

	int count;
	__changeskill skill_list[MAX_SKILL_NUM];
};

struct SkillOtherParam
{
	SkillOtherParam() { this->Reset(); }

	void Reset() 
	{
		reserved = 0;
	}

	int reserved;
};

typedef char SkillOtherParamHex[sizeof(SkillOtherParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(SkillOtherParamHex) < 64);

extern const SkillOtherParam INVALID_SKILL_OTHER_PARAM;

#endif

