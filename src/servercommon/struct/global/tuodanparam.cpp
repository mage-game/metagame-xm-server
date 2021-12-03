#include "tuodanparam.hpp"

#include "servercommon/servercommon.h"
#include "servercommon/struct/structcommon.h"

OLD_USE_INTERNAL_NETWORK();

bool TuodanParam::Serialize(TLVSerializer &outstream) const
{
	bool ret = outstream.Push(count);
	if (!ret) return false;

	for (int i = 0; i < count && i < MAX_ONCE_TUODAN_COUNT; ++i)
	{
		ret = outstream.Push(tuodan_list[i].change_state) && outstream.Push(tuodan_list[i].index);
		if (!ret) return false;

		if (tuodan_list[i].change_state == structcommon::CS_DELETE) continue;

		const char *notice_tmp = tuodan_list[i].notice;

		ret = outstream.Push(tuodan_list[i].uid) 
			&& outstream.Push(tuodan_list[i].create_time)
			&& outstream.Push(notice_tmp);
		if (!ret) return false;
	}

	return true;
}

bool TuodanParam::Unserialize(TLVUnserializer &instream)
{
	bool ret = instream.Pop(&count);
	if (!ret) return false;

	for (int i = 0; i < count && i < MAX_ONCE_TUODAN_COUNT; ++i)
	{
		ret = instream.Pop(&tuodan_list[i].change_state) && instream.Pop(&tuodan_list[i].index);
		if (!ret) return false;

		if (tuodan_list[i].change_state == structcommon::CS_DELETE) continue;

		const char *notice_tmp = 0;

		ret = instream.Pop(&tuodan_list[i].uid) 
			&& instream.Pop(&tuodan_list[i].create_time) 
			&& instream.Pop(&notice_tmp);
		if (!ret) return false;

		STRNCPY(tuodan_list[i].notice, notice_tmp, sizeof(tuodan_list[0].notice));
	}

	return true;
}