#include "spaceparam.hpp"

#include "servercommon/servercommon.h"
#include "servercommon/struct/structcommon.h"

OLD_USE_INTERNAL_NETWORK();

bool SpaceParam::Serialize(TLVSerializer &outstream) const
{
	bool ret = outstream.Push(count);
	if (!ret) return false;

	for (int i = 0; i < count && i < SPACE_MAX_RECORD_COUNT; ++i)
	{
		const char *msg = record_list[i].msg;

		ret = outstream.Push(record_list[i].id_record)
			&& outstream.Push(record_list[i].change_state)
			&& outstream.Push(record_list[i].record_unq_id)
			&& outstream.Push(record_list[i].owner_uid)
			&& outstream.Push(record_list[i].from_uid)
			&& outstream.Push(record_list[i].to_uid)
			&& outstream.Push(record_list[i].happen_timestamp)
			&& outstream.Push(record_list[i].happen_space_uid)
			&& outstream.Push(record_list[i].type)
			&& outstream.Push(record_list[i].is_huifu)
			&& outstream.Push(msg);
		if (!ret) 
		{
			return false;     
		}
	}

	return true;
}

bool SpaceParam::Unserialize(TLVUnserializer &instream)
{
	bool ret = instream.Pop(&count);
	if (!ret) return false;

	for (int i = 0; i < count && i < SPACE_MAX_RECORD_COUNT; ++i)
	{
		const char *msg = NULL;

		ret = instream.Pop(&record_list[i].id_record)
			&& instream.Pop(&record_list[i].change_state)
			&& instream.Pop(&record_list[i].record_unq_id)
			&& instream.Pop(&record_list[i].owner_uid)
			&& instream.Pop(&record_list[i].from_uid)
			&& instream.Pop(&record_list[i].to_uid)
			&& instream.Pop(&record_list[i].happen_timestamp) 
			&& instream.Pop(&record_list[i].happen_space_uid) 
			&& instream.Pop(&record_list[i].type)
			&& instream.Pop(&record_list[i].is_huifu)
			&& instream.Pop(&msg);
		if (!ret) 
		{
			return false;
		}

		STRNCPY(record_list[i].msg, msg, sizeof(record_list[0].msg));
	}

	return true;
}

