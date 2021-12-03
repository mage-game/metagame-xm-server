#include "crossrecordlistparam.hpp"

#include "servercommon/servercommon.h"
#include "servercommon/struct/structcommon.h"

bool CrossRecordListParam::Serialize(TLVSerializer &outstream) const
{
	bool ret = outstream.Push(count);
	if (!ret) return false;

	for (int i = 0; i < count && i < CROSS_RECORD_MAX_COUNT; ++i)
	{
		ret = outstream.Push(cross_record_list[i].id_cross_record) &&
			outstream.Push(cross_record_list[i].change_state) &&
			outstream.Push(cross_record_list[i].role_id);
		if (!ret) 
		{
			return false;
		}

		if (cross_record_list[i].change_state == structcommon::CS_DELETE) continue;

		const char * role_name = cross_record_list[i].role_name;

		TLVSerializer record_data;
		record_data.Reset((void *)&cross_record_list[i].user_record_data, sizeof(UserCrossRecordData));
		record_data.MoveCurPos(sizeof(UserCrossRecordData));

		TLVSerializer appearance_data;
		appearance_data.Reset((void *)&cross_record_list[i].user_appearance_data, sizeof(RoleAppearance));
		appearance_data.MoveCurPos(sizeof(RoleAppearance));
		
		ret = outstream.Push(role_name) &&
			outstream.Push(cross_record_list[i].level) &&
			outstream.Push(cross_record_list[i].avatar) &&
			outstream.Push(cross_record_list[i].sex) &&
			outstream.Push(cross_record_list[i].prof) &&
			outstream.Push(cross_record_list[i].camp) &&
			outstream.Push(cross_record_list[i].order_view_match_index) &&
			outstream.Push(record_data) &&
			outstream.Push(appearance_data);
		if (!ret)
		{
			return false;
		}
	}

	return true;
}

bool CrossRecordListParam::Unserialize(TLVUnserializer &instream)
{
	bool ret = instream.Pop(&count);
	if (!ret) return false;

	for (int i = 0; i < count && i < CROSS_RECORD_MAX_COUNT; ++i)
	{
		ret = instream.Pop(&cross_record_list[i].id_cross_record) &&
			instream.Pop(&cross_record_list[i].change_state) &&
			instream.Pop(&cross_record_list[i].role_id);
		if (!ret) 
		{
			return false;
		}

		if (cross_record_list[i].change_state == structcommon::CS_DELETE) continue;
		
		const char * role_name = 0;

		TLVUnserializer record_data;
		TLVUnserializer appearance_data;

		ret = instream.Pop(&role_name) &&
			instream.Pop(&cross_record_list[i].level) &&
			instream.Pop(&cross_record_list[i].avatar) &&
			instream.Pop(&cross_record_list[i].sex) &&
			instream.Pop(&cross_record_list[i].prof) &&
			instream.Pop(&cross_record_list[i].camp) &&
			instream.Pop(&cross_record_list[i].order_view_match_index) &&
			instream.Pop(&record_data) &&
			instream.Pop(&appearance_data);
		if (!ret)
		{
			return false;
		}

		STRNCPY(cross_record_list[i].role_name, role_name, sizeof(GameName));

		if (record_data.Size() <= sizeof(UserCrossRecordData))
		{
			memcpy(&cross_record_list[i].user_record_data, record_data.Ptr(), record_data.Size());
		}

		if (appearance_data.Size() <= sizeof(RoleAppearance))
		{
			memcpy(&cross_record_list[i].user_appearance_data, appearance_data.Ptr(), appearance_data.Size());
		}
	}

	return true;
}

