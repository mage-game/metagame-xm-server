#include "fishpoolparam.hpp"

#include "servercommon/servercommon.h"
#include "servercommon/struct/structcommon.h"

OLD_USE_INTERNAL_NETWORK();

bool FishPoolListParam::Serialize(TLVSerializer &outstream) const
{
	bool ret = outstream.Push(count);
	if (!ret) return false;

	for (int i = 0; i < count && i < FISH_POOL_ONCE_LOAD_MAX_COUNT; ++ i)
	{
		const FishPoolAttr &fish_pool_attr = fish_pool_list[i];

		ret = outstream.Push(fish_pool_attr.id_fishpool) &&
			outstream.Push(fish_pool_attr.change_state) && 
			outstream.Push(fish_pool_attr.role_id);
		if (!ret)
		{
			return false;
		}

		if (structcommon::CS_DELETE == fish_pool_attr.change_state) 
		{
			continue;
		}

		{
			TLVSerializer tlv_serializer; 
			tlv_serializer.Reset((void*)&fish_pool_attr.common_data, sizeof(fish_pool_attr.common_data));
			tlv_serializer.MoveCurPos(sizeof(fish_pool_attr.common_data));

			ret = outstream.Push(tlv_serializer);
			if (!ret)
			{
				return false;
			}
		}
	}

	return true;
}

bool FishPoolListParam::Unserialize(TLVUnserializer &instream)
{
	bool ret = instream.Pop(&count);
	if (!ret)
	{
		return false;
	}

	for (int i = 0; i < count && i < FISH_POOL_ONCE_LOAD_MAX_COUNT; ++ i)
	{
		FishPoolAttr &fish_pool_attr = fish_pool_list[i];

		ret = instream.Pop(&fish_pool_attr.id_fishpool) &&
			instream.Pop(&fish_pool_attr.change_state) &&
			instream.Pop(&fish_pool_attr.role_id);
		if (!ret)
		{
			return false;
		}

		if (structcommon::CS_DELETE == fish_pool_attr.change_state)
		{
			continue;
		}

		{
			TLVUnserializer tlv_unserializer;
			ret = instream.Pop(&tlv_unserializer);
			if (!ret)
			{
				return false;
			}

			if (sizeof(fish_pool_attr.common_data) >= tlv_unserializer.Size())
			{
				memcpy(&fish_pool_attr.common_data, tlv_unserializer.Ptr(), tlv_unserializer.Size());
			}
		}
	}

	return true;
}

