#include "worldstatus2param.hpp"

#include "servercommon/servercommon.h"
#include "servercommon/struct/structcommon.h"

bool WorldStatus2Param::Serialize(TLVSerializer &outstream) const
{
	bool ret = false;

	{
		TLVSerializer drop_log_list_data;
		drop_log_list_data.Reset((char*)&drop_log_list_param, sizeof(drop_log_list_param));
		drop_log_list_data.MoveCurPos(sizeof(drop_log_list_param));

		ret = outstream.Push(drop_log_list_data);
		if (!ret) return false;
	}

	{
		TLVSerializer common_data_2_data;
		common_data_2_data.Reset((char*)&common_data_2, sizeof(common_data_2));
		common_data_2_data.MoveCurPos(sizeof(common_data_2));

		ret = outstream.Push(common_data_2_data);
		if (!ret) return false;
	}

	{
		TLVSerializer wedding_data;
		wedding_data.Reset((char*)&wedding_param, sizeof(wedding_param));
		wedding_data.MoveCurPos(sizeof(wedding_param));

		ret = outstream.Push(wedding_data);
		if (!ret) return false;
	}

	{
		TLVSerializer profession_global_data;
		profession_global_data.Reset((char*)&global_profess_param, sizeof(global_profess_param));
		profession_global_data.MoveCurPos(sizeof(global_profess_param));

		ret = outstream.Push(profession_global_data);
		if (!ret) return false;
	}

	{
		TLVSerializer drop_log_fb_data;
		drop_log_fb_data.Reset((char*)&drop_log_fb_param,sizeof(drop_log_fb_param));
		drop_log_fb_data.MoveCurPos(sizeof(drop_log_fb_param));

		ret = outstream.Push(drop_log_fb_data);
		if (!ret) return false;
	}

	{
		TLVSerializer world_acquisition_log_data;
		world_acquisition_log_data.Reset((char*)&world_acquisition_log_param, sizeof(world_acquisition_log_param));
		world_acquisition_log_data.MoveCurPos(sizeof(world_acquisition_log_param));

		ret = outstream.Push(world_acquisition_log_data);
		if (!ret) return false;
	}

	return ret;
}

bool WorldStatus2Param::Unserialize(TLVUnserializer &instream)
{
	bool ret = false;

	TLVUnserializer drop_log_list_data;
	{
		ret = instream.Pop(&drop_log_list_data);
		if (!ret) return false;

		if (drop_log_list_data.Size() <= sizeof(drop_log_list_param))
		{
			memcpy(&drop_log_list_param, drop_log_list_data.Ptr(), drop_log_list_data.Size());
		}
	}

	TLVUnserializer common_data_2_data;
	{
		ret = instream.Pop(&common_data_2_data);
		if (!ret) return false;

		if (common_data_2_data.Size() <= sizeof(common_data_2))
		{
			memcpy(&common_data_2, common_data_2_data.Ptr(), common_data_2_data.Size());
		}
	}

	TLVUnserializer wedding_data;
	{
		ret = instream.Pop(&wedding_data);
		if (!ret) return false;

		if (wedding_data.Size() <= sizeof(wedding_param))
		{
			memcpy(&wedding_param, wedding_data.Ptr(), wedding_data.Size());
		}
	}

	TLVUnserializer profession_global_data;
	{
		ret = instream.Pop(&profession_global_data);
		if (!ret) return false;

		if (profession_global_data.Size() <= sizeof(global_profess_param))
		{
			memcpy(&global_profess_param, profession_global_data.Ptr(), profession_global_data.Size());
		}
	}

	TLVUnserializer drop_log_fb_data;
	{
		ret = instream.Pop(&drop_log_fb_data);
		if (!ret) return false;

		if (drop_log_fb_data.Size() <= sizeof(drop_log_fb_param))
		{
			memcpy(&drop_log_fb_param, drop_log_fb_data.Ptr(), drop_log_fb_data.Size());
		}
	}

	TLVUnserializer world_acquisition_log_data;
	{
		ret = instream.Pop(&world_acquisition_log_data);
		if (!ret) return false;

		if (world_acquisition_log_data.Size() <= sizeof(world_acquisition_log_param))
		{
			memcpy(&world_acquisition_log_param, world_acquisition_log_data.Ptr(), world_acquisition_log_data.Size());
		}
	}

	return ret;
}
