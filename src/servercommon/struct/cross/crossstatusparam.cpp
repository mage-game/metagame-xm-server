#include "crossstatusparam.hpp"

#include "servercommon/servercommon.h"
#include "servercommon/struct/structcommon.h"

bool CrossStatusParam::Serialize(TLVSerializer &outstream) const
{
	bool ret = outstream.Push(last_update_dayid);
	if (!ret) return false;

	{
		TLVSerializer cross_person_rank_title_data;
		cross_person_rank_title_data.Reset((char*)&cross_person_rank_title_param, sizeof(cross_person_rank_title_param));
		cross_person_rank_title_data.MoveCurPos(sizeof(cross_person_rank_title_param));

		ret = outstream.Push(cross_person_rank_title_data);
		if (!ret) return false;
	}
	{
		TLVSerializer cross_activity_data;
		cross_activity_data.Reset((char*)&cross_activity_param, sizeof(cross_activity_param));
		cross_activity_data.MoveCurPos(sizeof(cross_activity_param));

		ret = outstream.Push(cross_activity_data);
		if (!ret) return false;
	}

	return ret;
}

bool CrossStatusParam::Unserialize(TLVUnserializer &instream)
{
	bool ret = instream.Pop(&last_update_dayid);
	if (!ret) return false;

	{
		TLVUnserializer cross_person_rank_title_data;
		ret = instream.Pop(&cross_person_rank_title_data);
		if (!ret) return false;

		if (cross_person_rank_title_data.Size() <= sizeof(cross_person_rank_title_param))
		{
			memcpy(&cross_person_rank_title_param, cross_person_rank_title_data.Ptr(), cross_person_rank_title_data.Size());
		}

	}
	{
		TLVUnserializer cross_activity_data;
		ret = instream.Pop(&cross_activity_data);
		if (!ret) return false;

		if (cross_activity_data.Size() <= sizeof(cross_activity_param))
		{
			memcpy(&cross_activity_param, cross_activity_data.Ptr(), cross_activity_data.Size());
		}
	}
	
	return ret;
}

void CrossStatusParam::Reset()
{
	last_update_dayid = 0;
}

