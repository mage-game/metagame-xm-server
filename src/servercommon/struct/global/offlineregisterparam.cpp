#include "offlineregisterparam.hpp"

#include "servercommon/servercommon.h"
#include "servercommon/struct/structcommon.h"

OLD_USE_INTERNAL_NETWORK();

bool OfflineRegisterParam::Serialize(TLVSerializer &outstream) const
{
	bool ret = outstream.Push(count);
	if (!ret) return false;

	for (int i = 0; i < count && i < MAX_OFFLINE_REGISTER_COUNT; ++i)
	{
		ret = outstream.Push(offline_register_list[i].id_offline_register)
			&& outstream.Push(offline_register_list[i].change_state)
			&& outstream.Push(offline_register_list[i].role_id)
			&& outstream.Push(offline_register_list[i].increase_charm)
			&& outstream.Push(offline_register_list[i].last_divorce_time)
			&& outstream.Push(offline_register_list[i].chongzhi_cmd_flag)
			&& outstream.Push(offline_register_list[i].red_paper_consume_gold)
			&& outstream.Push(offline_register_list[i].red_paper_lucky_value)
			&& outstream.Push(offline_register_list[i].space_cai_count)
			&& outstream.Push(offline_register_list[i].space_gift_count)
			&& outstream.Push(offline_register_list[i].qingyuan_bless_days);
		if (!ret) 
		{
			return false;     
		}

		{
			TLVSerializer tlv_data;
			int length = sizeof(offline_register_list[i].guild_box_info);
			tlv_data.Reset(((void*)&offline_register_list[i].guild_box_info), length);
			tlv_data.MoveCurPos(length);
			if (!outstream.Push(tlv_data)) return false;
		}

		SERIALIZE_DATA(&offline_register_list[i].common_param, sizeof(offline_register_list[i].common_param));
//		SERIALIZE_DATA(&offline_register_list[i].profess_param, sizeof(offline_register_list[i].profess_param));

		if (offline_register_list[i].change_state == structcommon::CS_DELETE) continue;
	}

	return true;
}

bool OfflineRegisterParam::Unserialize(TLVUnserializer &instream)
{
	bool ret = instream.Pop(&count);
	if (!ret) return false;

	for (int i = 0; i < count && i < MAX_OFFLINE_REGISTER_COUNT; ++i)
	{
		ret = instream.Pop(&offline_register_list[i].id_offline_register)
			&& instream.Pop(&offline_register_list[i].change_state)
			&& instream.Pop(&offline_register_list[i].role_id)
			&& instream.Pop(&offline_register_list[i].increase_charm)
			&& instream.Pop(&offline_register_list[i].last_divorce_time) 
			&& instream.Pop(&offline_register_list[i].chongzhi_cmd_flag)
			&& instream.Pop(&offline_register_list[i].red_paper_consume_gold)
			&& instream.Pop(&offline_register_list[i].red_paper_lucky_value)
			&& instream.Pop(&offline_register_list[i].space_cai_count)
			&& instream.Pop(&offline_register_list[i].space_gift_count)
			&& instream.Pop(&offline_register_list[i].qingyuan_bless_days);
		if (!ret) 
		{
			return false;
		}

		{
			TLVUnserializer tlv_data;
			if (!instream.Pop(&tlv_data)) return false;
			if (tlv_data.Size() <= sizeof(offline_register_list[i].guild_box_info))
			{
				memcpy(&offline_register_list[i].guild_box_info, tlv_data.Ptr(), tlv_data.Size());
			} 
		}

		UNSERIALIZE_DATA(&offline_register_list[i].common_param, sizeof(offline_register_list[i].common_param));
//		UNSERIALIZE_DATA(&offline_register_list[i].profess_param, sizeof(offline_register_list[i].profess_param));

		if (offline_register_list[i].change_state == structcommon::CS_DELETE) continue;

	}

	return true;
}

