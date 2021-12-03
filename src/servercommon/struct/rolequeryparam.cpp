#include "rolequeryparam.hpp"

inline bool SerializeAutoData(TLVSerializer &outstream, const DataPageParam &data_page)
{
	TLVSerializer data_page_stream;
	data_page_stream.Reset((void*)data_page.data, data_page.len);
	data_page_stream.MoveCurPos(data_page.len);

	// serialize data page to outstream
	if (!outstream.Push(data_page.len) || !outstream.Push(data_page_stream))
	{
		return false;
	}

	return true;
}

inline bool UnserializeAutoData(TLVUnserializer &instream, DataPageParam &data_page)
{
	// unserialize data page from instream
	if (!instream.Pop(&data_page.len))
	{
		return false;
	}

	TLVUnserializer data_page_stream;
	if (!instream.Pop(&data_page_stream))
	{
		return false;
	}

	if (sizeof(data_page.data) >= data_page_stream.Size())
	{
		memcpy((void*)data_page.data, data_page_stream.Ptr(), data_page_stream.Size());
	}
	else
	{
		return false;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
bool RoleQueryParam::Serialize(TLVSerializer &outstream) const
{
	const char *p_rolename = role_name;

	bool ret = outstream.Push(role_id) &&
		outstream.Push(p_rolename) &&
		outstream.Push(sex) &&
		outstream.Push(camp) &&
		outstream.Push(prof) &&
		outstream.Push(vip_level) &&
		outstream.Push(level) &&
		outstream.Push(guild_id) &&
		outstream.Push(avatar_timestamp);
	if (!ret)
	{
		return false;
	}

	SERIALIZE_DATA(&title_param, sizeof(title_param));
	SERIALIZE_DATA(&common_data_param, sizeof(common_data_param));
	SERIALIZE_DATA(&wing_param, sizeof(wing_param));
	SERIALIZE_DATA(&role_init_param, sizeof(role_init_param));

	SERIALIZE_DATA(&qingyuan_param, sizeof(qingyuan_param));
	SERIALIZE_DATA(&shizhuang_param, sizeof(shizhuang_param));

	SERIALIZE_DATA(&mount_param, sizeof(mount_param));
	SERIALIZE_DATA(&stone_param, sizeof(stone_param));

	SERIALIZE_DATA(&pet_param, sizeof(pet_param));
	SERIALIZE_DATA(&equipment_param, sizeof(equipment_param));

	SERIALIZE_DATA(&halo_param, sizeof(halo_param));
	SERIALIZE_DATA(&shengong_param, sizeof(shengong_param));
	SERIALIZE_DATA(&shenyi_param, sizeof(shenyi_param));

	SERIALIZE_DATA(&xiannv_shouhu_param, sizeof(xiannv_shouhu_param));
	SERIALIZE_DATA(&jingling_fazhen_param, sizeof(jingling_fazhen_param));
	SERIALIZE_DATA(&jingling_guanghuan_param, sizeof(jingling_guanghuan_param));

	SERIALIZE_DATA(&fight_mount_param, sizeof(fight_mount_param));

	SERIALIZE_DATA(&xiannv_param, sizeof(xiannv_param));
	SERIALIZE_DATA(&jingling_param, sizeof(jingling_param));
	SERIALIZE_DATA(&lieming_param, sizeof(lieming_param));

	SERIALIZE_DATA(&footprint_param, sizeof(footprint_param));
	SERIALIZE_DATA(&cloak_param, sizeof(cloak_param));
	SERIALIZE_DATA(&shenzhuang_param, sizeof(shenzhuang_param));
	SERIALIZE_DATA(&fabao_param, sizeof(fabao_param));

	SERIALIZE_DATA(&yaoshi_param, sizeof(yaoshi_param));
	SERIALIZE_DATA(&toushi_param, sizeof(toushi_param));
	SERIALIZE_DATA(&qilinbi_param, sizeof(qilinbi_param));
	SERIALIZE_DATA(&mask_param, sizeof(mask_param));
	SERIALIZE_DATA(&mojie_param, sizeof(mojie_param));
	SERIALIZE_DATA(&zhuanzhi_equip_param, sizeof(zhuanzhi_equip_param));

	SERIALIZE_DATA(&upgrade_manager_param, sizeof(upgrade_manager_param));
	SERIALIZE_DATA(&shenbing_param, sizeof(shenbing_param));

	SERIALIZE_DATA(&equipbaptize_param, sizeof(equipbaptize_param));

	SERIALIZE_DATA(&greate_soldier_param, sizeof(greate_soldier_param));

	SERIALIZE_DATA(&baizhan_equip_param, sizeof(baizhan_equip_param));

	// 10±í BLOB×Ö¶Î
	if (!SerializeAutoData(outstream, data_page_10)) return false;

	return true;
}

bool RoleQueryParam::Unserialize(TLVUnserializer &instream)
{
	const char *p_rolename = NULL;

	bool ret = instream.Pop(&role_id) &&
		instream.Pop(&p_rolename) &&
		instream.Pop(&sex) &&
		instream.Pop(&camp) &&
		instream.Pop(&prof) &&
		instream.Pop(&vip_level) &&
		instream.Pop(&level) &&
		instream.Pop(&guild_id) &&
		instream.Pop(&avatar_timestamp);
	if (!ret)
	{
		return false;
	}

	STRNCPY(role_name, p_rolename, sizeof(role_name));

	UNSERIALIZE_DATA(&title_param, sizeof(title_param));
	UNSERIALIZE_DATA(&common_data_param, sizeof(common_data_param));
	UNSERIALIZE_DATA(&wing_param, sizeof(wing_param));
	UNSERIALIZE_DATA(&role_init_param, sizeof(role_init_param));

	UNSERIALIZE_DATA(&qingyuan_param, sizeof(qingyuan_param));
	UNSERIALIZE_DATA(&shizhuang_param, sizeof(shizhuang_param));

	UNSERIALIZE_DATA(&mount_param, sizeof(mount_param));
	UNSERIALIZE_DATA(&stone_param, sizeof(stone_param));

	UNSERIALIZE_DATA(&pet_param, sizeof(pet_param));
	UNSERIALIZE_DATA(&equipment_param, sizeof(equipment_param));

	UNSERIALIZE_DATA(&halo_param, sizeof(halo_param));
	UNSERIALIZE_DATA(&shengong_param, sizeof(shengong_param));
	UNSERIALIZE_DATA(&shenyi_param, sizeof(shenyi_param));

	UNSERIALIZE_DATA(&xiannv_shouhu_param, sizeof(xiannv_shouhu_param));
	UNSERIALIZE_DATA(&jingling_fazhen_param, sizeof(jingling_fazhen_param));
	UNSERIALIZE_DATA(&jingling_guanghuan_param, sizeof(jingling_guanghuan_param));

	UNSERIALIZE_DATA(&fight_mount_param, sizeof(fight_mount_param));

	UNSERIALIZE_DATA(&xiannv_param, sizeof(xiannv_param));
	UNSERIALIZE_DATA(&jingling_param, sizeof(jingling_param));
	UNSERIALIZE_DATA(&lieming_param, sizeof(lieming_param));

	UNSERIALIZE_DATA(&footprint_param, sizeof(footprint_param));
	UNSERIALIZE_DATA(&cloak_param, sizeof(cloak_param));
	UNSERIALIZE_DATA(&shenzhuang_param, sizeof(shenzhuang_param));
	UNSERIALIZE_DATA(&fabao_param, sizeof(fabao_param));

	UNSERIALIZE_DATA(&yaoshi_param, sizeof(yaoshi_param));
	UNSERIALIZE_DATA(&toushi_param, sizeof(toushi_param));
	UNSERIALIZE_DATA(&qilinbi_param, sizeof(qilinbi_param));
	UNSERIALIZE_DATA(&mask_param, sizeof(mask_param));
	UNSERIALIZE_DATA(&mojie_param, sizeof(mojie_param));
	UNSERIALIZE_DATA(&zhuanzhi_equip_param, sizeof(zhuanzhi_equip_param));

	UNSERIALIZE_DATA(&upgrade_manager_param, sizeof(upgrade_manager_param));
	UNSERIALIZE_DATA(&shenbing_param, sizeof(shenbing_param));

	UNSERIALIZE_DATA(&equipbaptize_param, sizeof(equipbaptize_param));

	UNSERIALIZE_DATA(&greate_soldier_param, sizeof(greate_soldier_param));

	UNSERIALIZE_DATA(&baizhan_equip_param, sizeof(baizhan_equip_param));

	// 10±í BLOB×Ö¶Î
	if (!UnserializeAutoData(instream, data_page_10)) return false;

	return true;
}

