#include "rankparam.hpp"

#include "servercommon/servercommon.h"
#include "servercommon/struct/structcommon.h"

OLD_USE_INTERNAL_NETWORK();

bool PersonRankParam::Serialize(TLVSerializer &outstream) const
{
	bool ret = outstream.Push(count);
	if (!ret) return false;

	for (int i = 0; i < count && i < PERSON_RANK_TYPE_MAX * PERSON_RANK_MAX_SIZE; ++i)
	{
		ret = outstream.Push(person_rank_list[i].change_state) && outstream.Push(person_rank_list[i].index);
		if (!ret) return false;

		if (person_rank_list[i].change_state == structcommon::CS_DELETE) continue;

		const char * user_name_tmp = person_rank_list[i].user_name;
		const char * flexible_name_tmp = person_rank_list[i].flexible_name;

		ret = outstream.Push(person_rank_list[i].uid)
			&& outstream.Push(user_name_tmp)
			&& outstream.Push(flexible_name_tmp)
			&& outstream.Push(person_rank_list[i].sex)
			&& outstream.Push(person_rank_list[i].prof)
			&& outstream.Push(person_rank_list[i].camp) 
			&& outstream.Push(person_rank_list[i].vip_level) 
			&& outstream.Push(person_rank_list[i].exp)
			&& outstream.Push(person_rank_list[i].level)
			&& outstream.Push(person_rank_list[i].rank_type)
			&& outstream.Push(person_rank_list[i].rank_value)
			&& outstream.Push(person_rank_list[i].record_index)
			&& outstream.Push(person_rank_list[i].flexible_int)
			&& outstream.Push(person_rank_list[i].flexible_ll);
		if (!ret) return false;
	}

	return true;
}

bool PersonRankParam::Unserialize(TLVUnserializer &instream)
{
	bool ret = instream.Pop(&count);
	if (!ret) return false;

	for (int i = 0; i < count && i < PERSON_RANK_TYPE_MAX * PERSON_RANK_MAX_SIZE; ++i)
	{
		ret = instream.Pop(&person_rank_list[i].change_state) && instream.Pop(&person_rank_list[i].index);
		if (!ret) return false;

		if (person_rank_list[i].change_state == structcommon::CS_DELETE) continue;

		const char *user_name_tmp = 0;
		const char *flexible_name_tmp = 0;

		ret = instream.Pop(&person_rank_list[i].uid)
			&& instream.Pop(&user_name_tmp)
			&& instream.Pop(&flexible_name_tmp)
			&& instream.Pop(&person_rank_list[i].sex)
			&& instream.Pop(&person_rank_list[i].prof)
			&& instream.Pop(&person_rank_list[i].camp)
			&& instream.Pop(&person_rank_list[i].vip_level)
			&& instream.Pop(&person_rank_list[i].exp)
			&& instream.Pop(&person_rank_list[i].level)
			&& instream.Pop(&person_rank_list[i].rank_type)
			&& instream.Pop(&person_rank_list[i].rank_value)
			&& instream.Pop(&person_rank_list[i].record_index)
			&& instream.Pop(&person_rank_list[i].flexible_int)
			&& instream.Pop(&person_rank_list[i].flexible_ll);
		if (!ret) return false;

		STRNCPY(person_rank_list[i].user_name, user_name_tmp, sizeof(person_rank_list[0].user_name));
		STRNCPY(person_rank_list[i].flexible_name, flexible_name_tmp, sizeof(person_rank_list[0].flexible_name));
	}

	return true;
}

bool TeamRankParam::Serialize(TLVSerializer &outstream) const
{
	bool ret = outstream.Push(count);
	if (!ret) return false;

	for (int i = 0; i < count && i < TEAM_RANK_TYPE_MAX * TEAM_RANK_MAX_SIZE; ++i)
	{
		ret = outstream.Push(team_rank_list[i].change_state) && outstream.Push(team_rank_list[i].index);
		if (!ret) return false;

		if (team_rank_list[i].change_state == structcommon::CS_DELETE) continue;

		for (int j = 0; j < MAX_TEAM_MEMBER_NUM; ++ j)
		{
			const char *user_name_tmp = team_rank_list[i].member_list[j].user_name;
			ret = outstream.Push(team_rank_list[i].member_list[j].uid) && outstream.Push(user_name_tmp) && outstream.Push(team_rank_list[i].member_list[j].camp);
			if (!ret) return false;
		}

		ret = outstream.Push(team_rank_list[i].rank_type)
			&& outstream.Push(team_rank_list[i].rank_value)
			&& outstream.Push(team_rank_list[i].flexible_int)
			&& outstream.Push(team_rank_list[i].flexible_ll);
		if (!ret) return false;
	}

	return true;
}

bool TeamRankParam::Unserialize(TLVUnserializer &instream)
{
	bool ret = instream.Pop(&count);
	if (!ret) return false;

	for (int i = 0; i < count && i < TEAM_RANK_TYPE_MAX * TEAM_RANK_MAX_SIZE; ++i)
	{
		ret = instream.Pop(&team_rank_list[i].change_state) && instream.Pop(&team_rank_list[i].index);
		if (!ret) return false;

		if (team_rank_list[i].change_state == structcommon::CS_DELETE) continue;

		for (int j = 0; j < MAX_TEAM_MEMBER_NUM; ++ j)
		{
			const char *user_name_tmp = NULL;
			ret = instream.Pop(&team_rank_list[i].member_list[j].uid) && instream.Pop(&user_name_tmp) && instream.Pop(&team_rank_list[i].member_list[j].camp);
			if (!ret) return false;

			STRNCPY(team_rank_list[i].member_list[j].user_name, user_name_tmp, sizeof(team_rank_list[i].member_list[j].user_name));
		}

		ret = instream.Pop(&team_rank_list[i].rank_type)
			&& instream.Pop(&team_rank_list[i].rank_value)
			&& instream.Pop(&team_rank_list[i].flexible_int)
			&& instream.Pop(&team_rank_list[i].flexible_ll);
		if (!ret) return false;
	}

	return true;
}

bool CrossPersonRankParam::Serialize(TLVSerializer &outstream) const
{
	bool ret = outstream.Push(count);
	if (!ret) return false;

	for (int i = 0; i < count && i < CROSS_PERSON_RANK_TYPE_MAX * CROSS_PERSON_RANK_MAX_SIZE; ++i)
	{
		ret = outstream.Push(cross_person_rank_list[i].change_state) && outstream.Push(cross_person_rank_list[i].index);
		if (!ret) return false;

		if (cross_person_rank_list[i].change_state == structcommon::CS_DELETE) continue;

		const char * user_name_tmp = cross_person_rank_list[i].user_name;
		const char * flexible_name_tmp = cross_person_rank_list[i].flexible_name;

		ret = outstream.Push(cross_person_rank_list[i].uuid) 
			&& outstream.Push(user_name_tmp)
			&& outstream.Push(cross_person_rank_list[i].level)
			&& outstream.Push(cross_person_rank_list[i].prof)
			&& outstream.Push(cross_person_rank_list[i].sex)
			&& outstream.Push(cross_person_rank_list[i].camp) 
			&& outstream.Push(cross_person_rank_list[i].exp)
			&& outstream.Push(cross_person_rank_list[i].rank_type)
			&& outstream.Push(cross_person_rank_list[i].rank_value)
			&& outstream.Push(cross_person_rank_list[i].record_index)
			&& outstream.Push(cross_person_rank_list[i].flexible_ll) 
			&& outstream.Push(flexible_name_tmp)
			&& outstream.Push(cross_person_rank_list[i].flexible_int)
			&& outstream.Push(cross_person_rank_list[i].vip_level);
		if (!ret) return false;
	}

	return true;
}

bool CrossPersonRankParam::Unserialize(TLVUnserializer &instream)
{
	bool ret = instream.Pop(&count);
	if (!ret) return false;

	for (int i = 0; i < count && i < CROSS_PERSON_RANK_TYPE_MAX * CROSS_PERSON_RANK_MAX_SIZE; ++i)
	{
		ret = instream.Pop(&cross_person_rank_list[i].change_state) && instream.Pop(&cross_person_rank_list[i].index);
		if (!ret) return false;

		if (cross_person_rank_list[i].change_state == structcommon::CS_DELETE) continue;

		const char *user_name_tmp = 0;
		const char *flexible_name_tmp = 0;

		ret = instream.Pop(&cross_person_rank_list[i].uuid) 
			&& instream.Pop(&user_name_tmp)
			&& instream.Pop(&cross_person_rank_list[i].level)
			&& instream.Pop(&cross_person_rank_list[i].prof)
			&& instream.Pop(&cross_person_rank_list[i].sex)
			&& instream.Pop(&cross_person_rank_list[i].camp)
			&& instream.Pop(&cross_person_rank_list[i].exp)
			&& instream.Pop(&cross_person_rank_list[i].rank_type)
			&& instream.Pop(&cross_person_rank_list[i].rank_value)
			&& instream.Pop(&cross_person_rank_list[i].record_index)
			&& instream.Pop(&cross_person_rank_list[i].flexible_ll) 
			&& instream.Pop(&flexible_name_tmp)
			&& instream.Pop(&cross_person_rank_list[i].flexible_int)
			&& instream.Pop(&cross_person_rank_list[i].vip_level);
		if (!ret) return false;

		STRNCPY(cross_person_rank_list[i].user_name, user_name_tmp, sizeof(cross_person_rank_list[0].user_name));
		STRNCPY(cross_person_rank_list[i].flexible_name, flexible_name_tmp, sizeof(cross_person_rank_list[0].flexible_name));
	}

	return true;
}

