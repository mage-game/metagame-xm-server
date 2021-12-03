#include "blacklistsparam.h"

#include "servercommon/servercommon.h"

OLD_USE_INTERNAL_NETWORK();

bool BlacklistsParam::Serialize(TLVSerializer &outstream)const
{
	if (!outstream.Push(count))
	{
		return false;
	}

	bool ret = true;

	for (int i = 0; i < count && i < MAX_BLACK_NUM; ++i)
	{
		ret = outstream.Push(black_list[i].change_state) && outstream.Push(black_list[i].index);

		if (!ret)
		{
			return false;
		}

		if (black_list[i].change_state == structcommon::CS_DELETE)
		{
			continue;
		}

		const char * hate_gamename = black_list[i].gamename;

		ret = outstream.Push(black_list[i].uid) &&
			outstream.Push(hate_gamename) &&
			outstream.Push(black_list[i].sex) &&
			outstream.Push(black_list[i].prof);
		if (!ret)
		{
			return false;
		}
	}

	return ret;
};

bool BlacklistsParam::Unserialize(TLVUnserializer &instream)
{
	if (!instream.Pop(&count))
	{
		return false;
	}

	bool ret = true;

	for (int i = 0; i < count && i < MAX_BLACK_NUM; ++i)
	{
		ret = instream.Pop(&black_list[i].change_state) && instream.Pop(&black_list[i].index);

		if (!ret)
		{
			return false;
		}

		if (black_list[i].change_state == structcommon::CS_DELETE)
		{
			continue;
		}

		const char *hate_gamename = 0;

		ret = instream.Pop(&black_list[i].uid) &&
			instream.Pop(&hate_gamename) &&
			instream.Pop(&black_list[i].sex) &&
			instream.Pop(&black_list[i].prof);
		if (!ret)
		{
			return false;
		}

		STRNCPY(black_list[i].gamename, hate_gamename, sizeof(black_list[0].gamename));
	}

	return ret;
}

