#include "worldstatusparam.hpp"

#include "servercommon/servercommon.h"
#include "servercommon/struct/structcommon.h"

bool WorldStatusParam::Serialize(TLVSerializer &outstream) const
{
	bool ret = outstream.Push(start_time) && 
		outstream.Push(world_time) && 
		outstream.Push(last_update_dayid) && 
		outstream.Push(xianmengzhan_mengzhu_id);

	if (!ret)
	{
		return false;
	}

	{
		TLVSerializer activity_open_record_data;
		activity_open_record_data.Reset((char*)&activity_open_records, sizeof(activity_open_records));
		activity_open_record_data.MoveCurPos(sizeof(activity_open_records));

		ret = outstream.Push(activity_open_record_data);
		if (!ret) return false;
	}

	{
		TLVSerializer common_data_data;
		common_data_data.Reset((char*)&common_data, sizeof(common_data));
		common_data_data.MoveCurPos(sizeof(common_data));

		ret = outstream.Push(common_data_data);
		if (!ret) return false;
	}

	{
		TLVSerializer challenge_field_data_data;
		challenge_field_data_data.Reset((char*)&challenge_field_data, sizeof(challenge_field_data));
		challenge_field_data_data.MoveCurPos(sizeof(challenge_field_data));

		ret = outstream.Push(challenge_field_data_data);
		if (!ret) return false;
	}

	{
		TLVSerializer world_event_param_data;
		world_event_param_data.Reset((char*)&world_event_param, sizeof(world_event_param));
		world_event_param_data.MoveCurPos(sizeof(world_event_param));

		ret = outstream.Push(world_event_param_data);
		if (!ret) return false;
	}

	{
		TLVSerializer csa_activity_rank_data;
		csa_activity_rank_data.Reset((char*)&csa_activity_rank_param, sizeof(csa_activity_rank_param));
		csa_activity_rank_data.MoveCurPos(sizeof(csa_activity_rank_param));

		ret = outstream.Push(csa_activity_rank_data);
		if (!ret) return false;
	}

	{
		TLVSerializer csa_activity_other_data;
		csa_activity_other_data.Reset((char*)&csa_activity_other_param, sizeof(csa_activity_other_param));
		csa_activity_other_data.MoveCurPos(sizeof(csa_activity_other_param));

		ret = outstream.Push(csa_activity_other_data);
		if (!ret) return false;
	}

	{
		TLVSerializer refresh_data;
		refresh_data.Reset((char*)&boss_refresh_param, sizeof(boss_refresh_param));
		refresh_data.MoveCurPos(sizeof(boss_refresh_param));

		ret = outstream.Push(refresh_data);
		if (!ret) return false;
	}

	{
		TLVSerializer cross_guildbattle_data;
		cross_guildbattle_data.Reset((char*)&cross_guildbattle_param, sizeof(cross_guildbattle_param));
		cross_guildbattle_data.MoveCurPos(sizeof(cross_guildbattle_param));

		ret = outstream.Push(cross_guildbattle_data);
		if (!ret) return false;
	}

	{
		TLVSerializer csa_boss_rank_data;
		csa_boss_rank_data.Reset((char*)&csa_boss_rank_param, sizeof(csa_boss_rank_param));
		csa_boss_rank_data.MoveCurPos(sizeof(csa_boss_rank_param));

		ret = outstream.Push(csa_boss_rank_data);
		if (!ret) return false;
	}

	{
		TLVSerializer cross_rand_activity_data;
		cross_rand_activity_data.Reset((char*)&cross_rand_activity_param, sizeof(cross_rand_activity_param));
		cross_rand_activity_data.MoveCurPos(sizeof(cross_rand_activity_param));

		ret = outstream.Push(cross_rand_activity_data);
		if (!ret) return false;
	}

	{
		TLVSerializer cross_rand_activity_big_data;
		cross_rand_activity_big_data.Reset((char*)&cross_rand_activity_big_param, sizeof(cross_rand_activity_big_param));
		cross_rand_activity_big_data.MoveCurPos(sizeof(cross_rand_activity_big_param));

		ret = outstream.Push(cross_rand_activity_big_data);
		if (!ret) return false;
	}

	{
		TLVSerializer cross_rand_activity_big_data2;
		cross_rand_activity_big_data2.Reset((char*)&cross_rand_activity_big_param2, sizeof(cross_rand_activity_big_param2));
		cross_rand_activity_big_data2.MoveCurPos(sizeof(cross_rand_activity_big_param2));

		ret = outstream.Push(cross_rand_activity_big_data2);
		if (!ret) return false;
	}

	{
		TLVSerializer cross_cloudpurchase_data;
		cross_cloudpurchase_data.Reset((char*)&cross_cloudpurchase_param, sizeof(cross_cloudpurchase_param));
		cross_cloudpurchase_data.MoveCurPos(sizeof(cross_cloudpurchase_param));

		ret = outstream.Push(cross_cloudpurchase_data);
		if (!ret) return false;
	}

	return ret;
}

bool WorldStatusParam::Unserialize(TLVUnserializer &instream)
{
	bool ret = instream.Pop(&start_time) &&
		instream.Pop(&world_time) && 
		instream.Pop(&last_update_dayid) &&
		instream.Pop(&xianmengzhan_mengzhu_id);

	if (!ret)
	{
		return false;
	}

	TLVUnserializer activity_open_records_data;
	{
		ret = instream.Pop(&activity_open_records_data);
		if (!ret) return false;

		if (activity_open_records_data.Size() <= sizeof(activity_open_records))
		{
			memcpy(&activity_open_records, activity_open_records_data.Ptr(), activity_open_records_data.Size());
		}
	}

	TLVUnserializer common_data_data;
	{
		ret = instream.Pop(&common_data_data);
		if (!ret) return false;

		if (common_data_data.Size() <= sizeof(common_data))
		{
			memcpy(&common_data, common_data_data.Ptr(), common_data_data.Size());
		}
	}

	TLVUnserializer challenge_field_data_data;
	{
		ret = instream.Pop(&challenge_field_data_data);
		if (!ret) return false;

		if (challenge_field_data_data.Size() <= sizeof(challenge_field_data))
		{
			memcpy(&challenge_field_data, challenge_field_data_data.Ptr(), challenge_field_data_data.Size());
		}
	}

	TLVUnserializer world_event_param_data;
	{
		ret = instream.Pop(&world_event_param_data);
		if (!ret) return false;

		if (world_event_param_data.Size() <= sizeof(world_event_param))
		{
			memcpy(&world_event_param, world_event_param_data.Ptr(), world_event_param_data.Size());
		}
	}

	TLVUnserializer csa_activity_rank_data;
	{
		ret = instream.Pop(&csa_activity_rank_data);
		if (!ret) return false;

		if (csa_activity_rank_data.Size() <= sizeof(csa_activity_rank_param))
		{
			memcpy(&csa_activity_rank_param, csa_activity_rank_data.Ptr(), csa_activity_rank_data.Size());
		}
	}

	TLVUnserializer csa_activity_other_data;
	{
		ret = instream.Pop(&csa_activity_other_data);
		if (!ret) return false;

		if (csa_activity_other_data.Size() <= sizeof(csa_activity_other_param))
		{
			memcpy(&csa_activity_other_param, csa_activity_other_data.Ptr(), csa_activity_other_data.Size());
		}
	}

	{
		TLVUnserializer refresh_data;
		ret = instream.Pop(&refresh_data);
		if (!ret) return false;

		if (refresh_data.Size() <= sizeof(boss_refresh_param))
		{
			memcpy(&boss_refresh_param, refresh_data.Ptr(), refresh_data.Size());
		}
	}

	{
		TLVUnserializer cross_guildbattle_data;
		ret = instream.Pop(&cross_guildbattle_data);
		if (!ret) return false;

		if (cross_guildbattle_data.Size() <= sizeof(cross_guildbattle_param))
		{
			memcpy(&cross_guildbattle_param, cross_guildbattle_data.Ptr(), cross_guildbattle_data.Size());
		}
	}

	{
		TLVUnserializer csa_boss_rank_data;
		ret = instream.Pop(&csa_boss_rank_data);
		if (!ret) return false;

		if (csa_boss_rank_data.Size() <= sizeof(csa_boss_rank_param))
		{
			memcpy(&csa_boss_rank_param, csa_boss_rank_data.Ptr(), csa_boss_rank_data.Size());
		}
	}

	TLVUnserializer cross_rand_activity_data;
	{
		ret = instream.Pop(&cross_rand_activity_data);
		if (!ret) return false;

		if (cross_rand_activity_data.Size() <= sizeof(cross_rand_activity_param))
		{
			memcpy(&cross_rand_activity_param, cross_rand_activity_data.Ptr(), cross_rand_activity_data.Size());
		}
	}

	TLVUnserializer cross_rand_activity_big_data;
	{
		ret = instream.Pop(&cross_rand_activity_big_data);
		if (!ret) return false;

		if (cross_rand_activity_big_data.Size() <= sizeof(cross_rand_activity_big_param))
		{
			memcpy(&cross_rand_activity_big_param, cross_rand_activity_big_data.Ptr(), cross_rand_activity_big_data.Size());
		}
	}

	TLVUnserializer cross_rand_activity_big_data2;
	{
		ret = instream.Pop(&cross_rand_activity_big_data2);
		if (!ret) return false;

		if (cross_rand_activity_big_data2.Size() <= sizeof(cross_rand_activity_big_param2))
		{
			memcpy(&cross_rand_activity_big_param2, cross_rand_activity_big_data2.Ptr(), cross_rand_activity_big_data2.Size());
		}
	}

	TLVUnserializer cross_cloudpurchase_data;
	{
		ret = instream.Pop(&cross_cloudpurchase_data);
		if (!ret) return false;

		if (cross_cloudpurchase_data.Size() <= sizeof(cross_cloudpurchase_param))
		{
			memcpy(&cross_cloudpurchase_param, cross_cloudpurchase_data.Ptr(), cross_cloudpurchase_data.Size());
		}
	}

	return ret;
}

