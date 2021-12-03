#include "campparam.hpp"

#include "servercommon/servercommon.h"
#include "servercommon/struct/structcommon.h"

OLD_USE_INTERNAL_NETWORK();

bool CampBaseParam::Serialize(TLVSerializer &outstream) const
{
	return outstream.Push(camp_type) && outstream.Push(role_count) && outstream.Push(guild_count) && outstream.Push(rich_level) 
		&& outstream.Push(earnings_end_time1) && outstream.Push(earnings1) && outstream.Push(earnings_end_time2) && outstream.Push(earnings2)
		&& outstream.Push(total_capability);
}

bool CampBaseParam::Unserialize(TLVUnserializer &instream)
{
	return instream.Pop(&camp_type) && instream.Pop(&role_count) && instream.Pop(&guild_count) && instream.Pop(&rich_level) 
		&& instream.Pop(&earnings_end_time1) && instream.Pop(&earnings1) && instream.Pop(&earnings_end_time2) && instream.Pop(&earnings2)
		&& instream.Pop(&total_capability);
}

bool CampParam::Serialize(TLVSerializer &outstream) const
{
	if (!outstream.Push(count)) return false;

	if (count < 0 || count > CAMP_TYPE_MAX) return false;

	for (int i = 0; i < count && i < CAMP_TYPE_MAX; ++i)
	{
		if (!camp_list[i].base_info.Serialize(outstream)) return false;
	}

	return true;
}

bool CampParam::Unserialize(TLVUnserializer &instream)
{
	if (!instream.Pop(&count)) return false;

	if (count < 0 || count > CAMP_TYPE_MAX) return false;

	for (int i = 0; i < count && i < CAMP_TYPE_MAX; ++i)
	{
		if (!camp_list[i].base_info.Unserialize(instream)) return false;

	}

	return true;
}
