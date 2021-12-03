#include "worldstatus3param.hpp"

#include "servercommon/servercommon.h"
#include "servercommon/struct/structcommon.h"

bool WorldStatus3Param::Serialize(TLVSerializer &outstream) const
{
	bool ret = false;

	{
		TLVSerializer common_data_data;
		common_data_data.Reset((char*)&common_data, sizeof(common_data));
		common_data_data.MoveCurPos(sizeof(common_data));

		ret = outstream.Push(common_data_data);
		if (!ret) return false;
	}

	{
		TLVSerializer toprank_data;
		toprank_data.Reset((char*)&toprank_param, sizeof(toprank_param));
		toprank_data.MoveCurPos(sizeof(toprank_param));

		ret = outstream.Push(toprank_data);
		if (!ret) return false;
	}
	
	{
		TLVSerializer serializer_data;
		serializer_data.Reset((char*)&lottery_count_param, sizeof(lottery_count_param));
		serializer_data.MoveCurPos(sizeof(lottery_count_param));

		ret = outstream.Push(serializer_data);
		if (!ret) return false;
	}

	{
		TLVSerializer rand_activity_param_data;
		rand_activity_param_data.Reset((char*)&rand_activity_param, sizeof(rand_activity_param));
		rand_activity_param_data.MoveCurPos(sizeof(rand_activity_param));

		ret = outstream.Push(rand_activity_param_data);
		if (!ret) return false;
	}

	{
		TLVSerializer lucky_cloud_buy_param_data;
		lucky_cloud_buy_param_data.Reset((char*)&lucky_cloud_buy_param, sizeof(lucky_cloud_buy_param));
		lucky_cloud_buy_param_data.MoveCurPos(sizeof(lucky_cloud_buy_param));

		ret = outstream.Push(lucky_cloud_buy_param_data);
		if (!ret) return false;
	}

	return ret;
}

bool WorldStatus3Param::Unserialize(TLVUnserializer &instream)
{
	bool ret = false;

	TLVUnserializer common_data_data;
	{
		ret = instream.Pop(&common_data_data);
		if (!ret) return false;

		if (common_data_data.Size() <= sizeof(common_data))
		{
			memcpy(&common_data, common_data_data.Ptr(), common_data_data.Size());
		}
	}

	{
		TLVUnserializer toprank_data;
		ret = instream.Pop(&toprank_data);
		if (!ret) return false;

		if (toprank_data.Size() <= sizeof(toprank_param))
		{
			memcpy(&toprank_param, toprank_data.Ptr(), toprank_data.Size());
		}
	}
	
	{
		TLVUnserializer serializer_data;
		ret = instream.Pop(&serializer_data);
		if (!ret) return false;

		if (serializer_data.Size() <= sizeof(lottery_count_param))
		{
			memcpy(&lottery_count_param, serializer_data.Ptr(), serializer_data.Size());
		}
	}

	{
		TLVUnserializer rand_activity_param_data;
		ret = instream.Pop(&rand_activity_param_data);
		if (!ret) return false;

		if (rand_activity_param_data.Size() <= sizeof(rand_activity_param))
		{
			memcpy(&rand_activity_param, rand_activity_param_data.Ptr(), rand_activity_param_data.Size());
		}
	}

	{
		TLVUnserializer lucky_cloud_buy_param_data;
		ret = instream.Pop(&lucky_cloud_buy_param_data);
		if (!ret) return false;

		if (lucky_cloud_buy_param_data.Size() <= sizeof(lucky_cloud_buy_param))
		{
			memcpy(&lucky_cloud_buy_param, lucky_cloud_buy_param_data.Ptr(), lucky_cloud_buy_param_data.Size());
		}
	}


	return ret;
}

