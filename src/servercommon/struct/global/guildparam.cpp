#include "guildparam.hpp"

#include "servercommon/servercommon.h"
#include "servercommon/struct/structcommon.h"

OLD_USE_INTERNAL_NETWORK();

bool GuildApplyForParam::Serialize(TLVSerializer &outstream) const
{
	bool ret = outstream.Push(count);
	if (!ret) return false;

	for (int i = 0; i < count && i < MAX_APPLY_FOR_COUNT; ++i)
	{
		ret = outstream.Push(apply_for_list[i].change_state) && outstream.Push(apply_for_list[i].index);
		if (!ret) return false;

		if (apply_for_list[i].change_state == structcommon::CS_DELETE) continue;

		ret = outstream.Push(apply_for_list[i].uid)
			&& outstream.Push(apply_for_list[i].applyfor_time);
		if (!ret) return false;
	}

	return true;
}

bool GuildApplyForParam::Unserialize(TLVUnserializer &instream)
{
	bool ret = instream.Pop(&count);
	if (!ret) return false;

	for (int i = 0; i < count && i < MAX_APPLY_FOR_COUNT; ++i)
	{
		ret = instream.Pop(&apply_for_list[i].change_state) && instream.Pop(&apply_for_list[i].index);
		if (!ret) return false;

		if (apply_for_list[i].change_state == structcommon::CS_DELETE) continue;

		ret = instream.Pop(&apply_for_list[i].uid)
			&& instream.Pop(&apply_for_list[i].applyfor_time);
		if (!ret) return false;
	}

	return true;
}

bool GuildMemberParam::Serialize(TLVSerializer &outstream) const
{
	bool ret = outstream.Push(count);
	if (!ret) return false;

	for (int i = 0; i < count && i < MAX_MEMBER_COUNT; ++i)
	{
		ret = outstream.Push(member_list[i].change_state) && outstream.Push(member_list[i].index);
		if (!ret) return false;

		if (member_list[i].change_state == structcommon::CS_DELETE) continue;

		TLVSerializer common_data;
		SERIALIZE_GUILD_PARAM(common_data, member_list[i].common_data);

		ret = outstream.Push(member_list[i].uid)
			&& outstream.Push(member_list[i].post)
			&& outstream.Push(member_list[i].gongxian)
			&& outstream.Push(member_list[i].total_gongxian)
			&& outstream.Push(member_list[i].join_time)
			&& outstream.Push(member_list[i].last_login_time)
			&& outstream.Push(member_list[i].lucky_color)
			&& outstream.Push(member_list[i].today_guild_pao_saizi_times)
			&& outstream.Push(member_list[i].today_last_guild_pao_saizi_time)
			&& outstream.Push(member_list[i].today_guild_saizi_score)
			&& outstream.Push(member_list[i].maze_layer)
			&& outstream.Push(member_list[i].maze_complete_time)
			&& outstream.Push(common_data);
		if (!ret) return false;
	}

	return true;
}

bool GuildMemberParam::Unserialize(TLVUnserializer &instream)
{
	bool ret = instream.Pop(&count);
	if (!ret) return false;

	for (int i = 0; i < count && i < MAX_MEMBER_COUNT; ++i)
	{
		ret = instream.Pop(&member_list[i].change_state) && instream.Pop(&member_list[i].index);
		if (!ret) return false;

		if (member_list[i].change_state == structcommon::CS_DELETE) continue;

		TLVUnserializer common_data;

		ret = instream.Pop(&member_list[i].uid)
			&& instream.Pop(&member_list[i].post)
			&& instream.Pop(&member_list[i].gongxian)
			&& instream.Pop(&member_list[i].total_gongxian)
			&& instream.Pop(&member_list[i].join_time)
			&& instream.Pop(&member_list[i].last_login_time)
			&& instream.Pop(&member_list[i].lucky_color)
			&& instream.Pop(&member_list[i].today_guild_pao_saizi_times)
			&& instream.Pop(&member_list[i].today_last_guild_pao_saizi_time)
			&& instream.Pop(&member_list[i].today_guild_saizi_score)
			&& instream.Pop(&member_list[i].maze_layer)
			&& instream.Pop(&member_list[i].maze_complete_time)
			&& instream.Pop(&common_data);
		if (!ret) return false;

		DESERIALIZE_GUILD_PARAM(common_data, member_list[i].common_data);
	}

	return true;
}

bool GuildEventParam::Serialize(TLVSerializer &outstream) const
{
	bool ret = outstream.Push(count);
	if (!ret) return false;

	for (int i = 0; i < count && i < MAX_ALL_GUILD_EVENT_COUNT; ++i)
	{
		ret = outstream.Push(event_list[i].change_state) && outstream.Push(event_list[i].index);
		if (!ret) return false;

		if (event_list[i].change_state == structcommon::CS_DELETE) continue;

		const char *event_owner_tmp = event_list[i].event_owner;
		const char *sparam0_tmp = event_list[i].sparam0;

		ret = outstream.Push(event_list[i].event_type)
			&& outstream.Push(event_owner_tmp)
			&& outstream.Push(event_list[i].event_owner_post)
			&& outstream.Push(event_list[i].event_time)
			&& outstream.Push(event_list[i].big_event)
			&& outstream.Push(event_list[i].cannot_remove)
			&& outstream.Push(event_list[i].param0)
			&& outstream.Push(event_list[i].param1)
			&& outstream.Push(event_list[i].param2)
			&& outstream.Push(event_list[i].param3)
			&& outstream.Push(sparam0_tmp);
		if (!ret) return false;
	}

	return true;
}

bool GuildEventParam::Unserialize(TLVUnserializer &instream)
{
	bool ret = instream.Pop(&count);
	if (!ret) return false;

	for (int i = 0; i < count && i < MAX_ALL_GUILD_EVENT_COUNT; ++i)
	{
		ret = instream.Pop(&event_list[i].change_state) && instream.Pop(&event_list[i].index);
		if (!ret) return false;

		if (event_list[i].change_state == structcommon::CS_DELETE) continue;

		const char *event_owner_tmp = NULL;
		const char *sparam0_tmp = NULL;

		ret = instream.Pop(&event_list[i].event_type)
			&& instream.Pop(&event_owner_tmp)
			&& instream.Pop(&event_list[i].event_owner_post)
			&& instream.Pop(&event_list[i].event_time)
			&& instream.Pop(&event_list[i].big_event)
			&& instream.Pop(&event_list[i].cannot_remove)
			&& instream.Pop(&event_list[i].param0)
			&& instream.Pop(&event_list[i].param1)
			&& instream.Pop(&event_list[i].param2)
			&& instream.Pop(&event_list[i].param3)
			&& instream.Pop(&sparam0_tmp);
		if (!ret) return false;

		STRNCPY(event_list[i].event_owner, event_owner_tmp, sizeof(event_list[0].event_owner));
		STRNCPY(event_list[i].sparam0, sparam0_tmp, sizeof(event_list[0].sparam0));
	}

	return true;
}

bool GuildBaseParam::Serialize(TLVSerializer &outstream) const
{
	const char *guild_name_tmp = guild_name;
	const char *notice_tmp = notice;
	const char *monster_finish_username_tmp = monster_finish_username;

	bool ret = outstream.Push(guild_id) && outstream.Push(guild_name_tmp) && outstream.Push(create_time) && outstream.Push(guild_level) && 
		outstream.Push(guild_exp) && outstream.Push(guild_last_uplevel_timestamp);
	if (!ret) return false;

	ret = outstream.Push(storage_use_limit) && outstream.Push(shop_use_limit) && outstream.Push(last_update_dayid) && 
		outstream.Push(last_active_time) && outstream.Push(warning_dismiss_days) && outstream.Push(applyfor_setup) && outstream.Push(camp) && outstream.Push(notice_tmp) &&
		outstream.Push(auto_kickout_setup) && outstream.Push(guild_max_capability) && outstream.Push(applyfor_need_capability) && outstream.Push(applyfor_need_level) &&
		outstream.Push(callin_times) && outstream.Push(monster_finish_seq) && outstream.Push(active_degree) && outstream.Push(monster_finish_username_tmp);
	if (!ret) return false;

	{
		TLVSerializer common_data_data;
		common_data_data.Reset((char*)&common_data_param, sizeof(common_data_param));
		common_data_data.MoveCurPos(sizeof(common_data_param));

		ret = outstream.Push(common_data_data);
		if (!ret) return false;
	}

	{
		TLVSerializer boss_list_data;
		boss_list_data.Reset((char*)&guild_boss_list, sizeof(guild_boss_list));
		boss_list_data.MoveCurPos(sizeof(guild_boss_list));
		ret = outstream.Push(boss_list_data);
		if (!ret) return false;
	}

	{
		TLVSerializer storge_item_list_data;
		storge_item_list_data.Reset((char*)&storge_item_list, sizeof(storge_item_list));
		storge_item_list_data.MoveCurPos(sizeof(storge_item_list));
		ret = outstream.Push(storge_item_list_data);
		if (!ret) return false;
	}

	{
		TLVSerializer guild_enemy_list_data;
		guild_enemy_list_data.Reset((char*)&guild_enemy_list, sizeof(guild_enemy_list));
		guild_enemy_list_data.MoveCurPos(sizeof(guild_enemy_list));
		ret = outstream.Push(guild_enemy_list_data);
		if (!ret) return false;
	}

	return true;
}

bool GuildBaseParam::Unserialize(TLVUnserializer &instream)
{
	const char *guild_name_tmp = NULL;
	const char *notice_tmp = NULL;
	const char *monster_finish_username_tmp = NULL;

	bool ret = instream.Pop(&guild_id) && instream.Pop(&guild_name_tmp) && instream.Pop(&create_time) && instream.Pop(&guild_level) &&
		instream.Pop(&guild_exp) && instream.Pop(&guild_last_uplevel_timestamp);
	if (!ret) return false;

	ret = instream.Pop(&storage_use_limit) && instream.Pop(&shop_use_limit) && instream.Pop(&last_update_dayid) &&
		instream.Pop(&last_active_time) && instream.Pop(&warning_dismiss_days) && instream.Pop(&applyfor_setup) && instream.Pop(&camp) && instream.Pop(&notice_tmp) &&
		instream.Pop(&auto_kickout_setup) && instream.Pop(&guild_max_capability) && instream.Pop(&applyfor_need_capability) && instream.Pop(&applyfor_need_level) &&
		instream.Pop(&callin_times) && instream.Pop(&monster_finish_seq) && instream.Pop(&active_degree) && instream.Pop(&monster_finish_username_tmp);
	if (!ret) return false;

	STRNCPY(guild_name, guild_name_tmp, sizeof(guild_name));
	STRNCPY(notice, notice_tmp, sizeof(notice));
	STRNCPY(monster_finish_username, monster_finish_username_tmp, sizeof(monster_finish_username));

	{
		TLVUnserializer common_data_data;
		ret = instream.Pop(&common_data_data);
		if (!ret) return false;

		if (common_data_data.Size() <= sizeof(common_data_param))
		{
			memcpy(&common_data_param, common_data_data.Ptr(), common_data_data.Size());
		}
	}

	{
		TLVUnserializer boss_list_data;
		ret = instream.Pop(&boss_list_data);
		if (!ret) return false;

		if (boss_list_data.Size() <= sizeof(guild_boss_list))
		{
			memcpy(&guild_boss_list, boss_list_data.Ptr(), boss_list_data.Size());
		}
	}

	{
		TLVUnserializer storge_item_list_data;
		ret = instream.Pop(&storge_item_list_data);
		if (!ret) return false;

		if (storge_item_list_data.Size() <= sizeof(storge_item_list))
		{
			memcpy(&storge_item_list, storge_item_list_data.Ptr(), storge_item_list_data.Size());
		}
	}

	{
		TLVUnserializer guild_enemy_list_data;
		ret = instream.Pop(&guild_enemy_list_data);
		if (!ret) return false;

		if (guild_enemy_list_data.Size() <= sizeof(guild_enemy_list))
		{
			memcpy(&guild_enemy_list, guild_enemy_list_data.Ptr(), guild_enemy_list_data.Size());
		}
	}

	return true;
}

bool AllGuildParam::Serialize(TLVSerializer &outstream) const
{
	bool ret = outstream.Push(count);
	if (!ret) return false;

	for (int i = 0; i < count && i < MAX_ONCE_GUILD_COUNT; ++i)
	{
		ret = guild_list[i].base_param.Serialize(outstream) 
			&& guild_list[i].apply_for_param.Serialize(outstream) 
			&& guild_list[i].member_param.Serialize(outstream)
			&& guild_list[i].event_param.Serialize(outstream);
		if (!ret) return false;
	}

	return true;
}

bool AllGuildParam::Unserialize(TLVUnserializer &instream)
{
	bool ret = instream.Pop(&count);
	if (!ret) return false;

	for (int i = 0; i < count && i < MAX_ONCE_GUILD_COUNT; ++i)
	{
		ret = guild_list[i].base_param.Unserialize(instream) 
			&& guild_list[i].apply_for_param.Unserialize(instream) 
			&& guild_list[i].member_param.Unserialize(instream)
			&& guild_list[i].event_param.Unserialize(instream);
		if (!ret) return false;
	}

	return true;
}
