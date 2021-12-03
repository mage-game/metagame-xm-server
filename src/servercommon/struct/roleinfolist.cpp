#include "roleinfolist.h"
#include "common/tlvprotocol.h"

#include "servercommon/servercommon.h"

OLD_USE_INTERNAL_NETWORK();

bool RoleInfoList::Serialize(TLVSerializer *outstream) const
{
	bool ret = outstream->Push(count);
	if (!ret) return false;

	for (int i = 0; i < count && i < MAX_ROLE_NUM; ++i)
	{
		ret = outstream->Push(role_info_list[i].role_id) 
			&& outstream->Push(role_info_list[i].scene_id) 
			&& outstream->Push(role_info_list[i].last_scene_id)
			&& outstream->Push(role_info_list[i].level) 
			&& outstream->Push(role_info_list[i].professional) 
			&& outstream->Push(role_info_list[i].create_time)
			&& outstream->Push(role_info_list[i].last_login_time)
			&& outstream->Push(role_info_list[i].avatar)
			&& outstream->Push(role_info_list[i].sex)
			&& outstream->Push(role_info_list[i].country)
			&& outstream->Push(role_info_list[i].wuqi_id)
			&& outstream->Push(role_info_list[i].shizhuang_wuqi)
			&& outstream->Push(role_info_list[i].shizhuang_wuqi_is_special)
			&& outstream->Push(role_info_list[i].shizhuang_body)
			&& outstream->Push(role_info_list[i].shizhuang_body_is_special)
			&& outstream->Push(role_info_list[i].wing_used_imageid)
			&& outstream->Push(role_info_list[i].halo_used_imageid)
			&& outstream->Push(role_info_list[i].yaoshi_used_imageid)
			&& outstream->Push(role_info_list[i].toushi_used_imageid)
			&& outstream->Push(role_info_list[i].qilinbi_used_imageid)
			&& outstream->Push(role_info_list[i].mask_used_imageid)
			&& outstream->Push(role_info_list[i].upgrade_used_imageid_0)
			&& outstream->Push(role_info_list[i].upgrade_used_imageid_1)
			&& outstream->Push(role_info_list[i].upgrade_used_imageid_2)
			&& outstream->Push(role_info_list[i].upgrade_used_imageid_3)
			&& outstream->Push(role_info_list[i].upgrade_used_imageid_4)
			&& outstream->Push(role_info_list[i].upgrade_used_imageid_5)
			&& outstream->Push(role_info_list[i].upgrade_used_imageid_6)
			&& outstream->Push(role_info_list[i].upgrade_used_imageid_7)
			&& outstream->Push(role_info_list[i].upgrade_used_imageid_8);
		if (!ret) return false;

		const char *name = role_info_list[i].role_name;
		ret = outstream->Push(name);
		if (!ret) return false;
	}

	return ret;
}

bool RoleInfoList::Unserialize(TLVUnserializer *instream)
{
	bool ret = instream->Pop(&count);
	if (!ret) return false;

	for (int i = 0; i < count && i < MAX_ROLE_NUM; ++i)
	{
		ret = instream->Pop(&role_info_list[i].role_id)
			&& instream->Pop(&role_info_list[i].scene_id)
			&& instream->Pop(&role_info_list[i].last_scene_id)
			&& instream->Pop(&role_info_list[i].level)
			&& instream->Pop(&role_info_list[i].professional)
			&& instream->Pop(&role_info_list[i].create_time)
			&& instream->Pop(&role_info_list[i].last_login_time)
			&& instream->Pop(&role_info_list[i].avatar)
			&& instream->Pop(&role_info_list[i].sex)
			&& instream->Pop(&role_info_list[i].country)
			&& instream->Pop(&role_info_list[i].wuqi_id)
			&& instream->Pop(&role_info_list[i].shizhuang_wuqi)
			&& instream->Pop(&role_info_list[i].shizhuang_wuqi_is_special)
			&& instream->Pop(&role_info_list[i].shizhuang_body)
			&& instream->Pop(&role_info_list[i].shizhuang_body_is_special)
			&& instream->Pop(&role_info_list[i].wing_used_imageid)
			&& instream->Pop(&role_info_list[i].halo_used_imageid)
			&& instream->Pop(&role_info_list[i].yaoshi_used_imageid)
			&& instream->Pop(&role_info_list[i].toushi_used_imageid)
			&& instream->Pop(&role_info_list[i].qilinbi_used_imageid)
			&& instream->Pop(&role_info_list[i].mask_used_imageid)
			&& instream->Pop(&role_info_list[i].upgrade_used_imageid_0)
			&& instream->Pop(&role_info_list[i].upgrade_used_imageid_1)
			&& instream->Pop(&role_info_list[i].upgrade_used_imageid_2)
			&& instream->Pop(&role_info_list[i].upgrade_used_imageid_3)
			&& instream->Pop(&role_info_list[i].upgrade_used_imageid_4)
			&& instream->Pop(&role_info_list[i].upgrade_used_imageid_5)
			&& instream->Pop(&role_info_list[i].upgrade_used_imageid_6)
			&& instream->Pop(&role_info_list[i].upgrade_used_imageid_7)
			&& instream->Pop(&role_info_list[i].upgrade_used_imageid_8);
		if (!ret) return false;

		const char *name = 0;
		ret = instream->Pop(&name);
		if (!ret) return false;

		STRNCPY(role_info_list[i].role_name, name, sizeof(role_info_list[0].role_name));
	}

	return ret;
}
