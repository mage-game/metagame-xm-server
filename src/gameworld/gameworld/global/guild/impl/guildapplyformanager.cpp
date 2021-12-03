#include "guildapplyformanager.hpp"

#include "engineadapter.h"
#include "servercommon/struct/structcommon.h"

#include "global/guild/guild.hpp"
#include "global/guild/guildmanager.hpp"

#include "global/usercache/usercache.hpp"

GuildApplyForManager::GuildApplyForManager()
	: m_guild(NULL), m_change_flag(false)
{
	memset(m_dirty_mark, 0, sizeof(m_dirty_mark));
	memset(m_old_state, 0, sizeof(m_old_state));
}

GuildApplyForManager::~GuildApplyForManager()
{

}

void GuildApplyForManager::Init(const GuildApplyForParam &apply_for_param)
{
	int count = 0;

	if (apply_for_param.count < MAX_APPLY_FOR_COUNT)
	{
		count = apply_for_param.count;
	}
	else 
	{
		count = MAX_APPLY_FOR_COUNT;
	}

	for (int i = count - 1; i >= 0; i--)
	{
		int index = (int)apply_for_param.apply_for_list[i].index;
		if (index < 0 || index >= MAX_APPLY_FOR_COUNT) continue;
		if (!m_apply_for_list[index].Invalid()) continue;

		m_apply_for_list[index].apply_user_id = IntToUid(apply_for_param.apply_for_list[i].uid);
		m_apply_for_list[index].applyfor_time = apply_for_param.apply_for_list[i].applyfor_time;

		m_guild->GetGuildManager()->OnUserAddApplyFor(m_apply_for_list[index].apply_user_id , m_guild->GetGuildID());

		m_old_state[index] = true;
	}
}

void GuildApplyForManager::GetChangeApplyForList(GuildApplyForParam *apply_for_param)
{
	if (!m_change_flag)
	{
		apply_for_param->count = 0;
		return;
	}

	int count = 0;
	for (int i = 0; i < MAX_APPLY_FOR_COUNT; ++i)
	{
		if (m_dirty_mark[i])
		{
			if (m_apply_for_list[i].Invalid())
			{
				if (!m_old_state[i]) continue;

				apply_for_param->apply_for_list[count].change_state = structcommon::CS_DELETE;
				apply_for_param->apply_for_list[count].index = i;
			}
			else
			{
				apply_for_param->apply_for_list[count].change_state = m_old_state[i] ? structcommon::CS_UPDATE : structcommon::CS_INSERT;
				apply_for_param->apply_for_list[count].index = i;
				
				apply_for_param->apply_for_list[count].uid = UidToInt(m_apply_for_list[i].apply_user_id);
				apply_for_param->apply_for_list[count].applyfor_time = m_apply_for_list[i].applyfor_time;
			}

			++count;
		}
	}

	apply_for_param->count = count;
}

void GuildApplyForManager::ClearDirtyMark()
{
	if (!m_change_flag) return;

	memset(m_dirty_mark, 0, sizeof(m_dirty_mark));

	for (int i = 0; i < MAX_APPLY_FOR_COUNT; ++i)
	{
		m_old_state[i] = !m_apply_for_list[i].Invalid();
	}

	m_change_flag = false;
}


void GuildApplyForManager::Update(unsigned long interval, time_t now_second)
{
}

bool GuildApplyForManager::AddApplyFor(const UserID &user_id)
{
	for (int i = 0; i < MAX_APPLY_FOR_COUNT; ++i)
	{
		if (m_apply_for_list[i].Invalid())
		{
			m_apply_for_list[i].apply_user_id = user_id;
			m_apply_for_list[i].applyfor_time = (unsigned int)(EngineAdapter::Instance().Time());

			m_change_flag = true;
			m_dirty_mark[i] = true;

			return true;
		}
	}

	return false;
}

GuildApplyForManager::ApplyFor * GuildApplyForManager::GetApplayFor(const UserID &user_id)
{
	for (int i = 0; i < MAX_APPLY_FOR_COUNT; ++i)
	{
		if (!m_apply_for_list[i].Invalid() && m_apply_for_list[i].apply_user_id == user_id)
		{
			return &m_apply_for_list[i];
		}
	}

	return NULL;
}

bool GuildApplyForManager::RemoveApplyFor(const UserID &user_id)
{
	for (int i = 0; i < MAX_APPLY_FOR_COUNT; ++i)
	{
		if (!m_apply_for_list[i].Invalid() && m_apply_for_list[i].apply_user_id == user_id)
		{
			m_apply_for_list[i].Reset();

			m_change_flag = true;
			m_dirty_mark[i] = true;

			return true;
		}
	}

	return false;
}

bool GuildApplyForManager::ApplyForListIsEmpty()
{
	for (int i = 0; i < MAX_APPLY_FOR_COUNT; ++i)
	{
		if (!m_apply_for_list[i].Invalid())
		{
			return false;
		}
	}

	return true;
}

void GuildApplyForManager::GetApplyForList(Protocol::SCGuildGetApplyForList *guild_apply_for_list)
{
	int count = 0;

	for (int i = 0; i < MAX_APPLY_FOR_COUNT; ++i)
	{
		if (m_apply_for_list[i].Invalid())
		{
			continue;
		}

		UserCacheNode *uc = UserCacheManager::Instance().GetUserNode(UidToInt(m_apply_for_list[i].apply_user_id));
		if (NULL == uc)
		{
			continue;
		}

		guild_apply_for_list->apply_for_list[count].uid = UidToInt(m_apply_for_list[i].apply_user_id);
		F_STRNCPY(guild_apply_for_list->apply_for_list[count].role_name, uc->user_name, sizeof(guild_apply_for_list->apply_for_list[0].role_name));
		guild_apply_for_list->apply_for_list[count].level = uc->level;
		guild_apply_for_list->apply_for_list[count].prof = uc->prof;
		guild_apply_for_list->apply_for_list[count].sex = uc->sex;
		guild_apply_for_list->apply_for_list[count].vip_level = uc->vip_level;
		guild_apply_for_list->apply_for_list[count].capability = uc->capability;
		guild_apply_for_list->apply_for_list[count].applyfor_time = m_apply_for_list[i].applyfor_time;

		++count;
	}

	guild_apply_for_list->count = count;
}

int GuildApplyForManager::GetApplyForList(UserID apply_for_userid_list[MAX_APPLY_FOR_COUNT])
{
	if (NULL == apply_for_userid_list)
	{
		return 0;
	}

	int count = 0;

	for (int i = 0; i < MAX_APPLY_FOR_COUNT; ++i)
	{
		if (m_apply_for_list[i].Invalid())
		{
			continue;
		}

		UserCacheNode *uc = UserCacheManager::Instance().GetUserNode(UidToInt(m_apply_for_list[i].apply_user_id));
		if (NULL == uc)
		{
			continue;
		}

		apply_for_userid_list[count] = m_apply_for_list[i].apply_user_id;
		++count;
	}

	return count;
}

bool GuildApplyForManager::AddInvite(const UserID &user_id)
{
	int swap_pos = 0;

	for (int i = 0; i < MAX_INVITE_COUNT; ++i)
	{
		if (m_invite_list[i].Invalid())
		{
			m_invite_list[i].beinvite_user_id = user_id;	
			m_invite_list[i].invite_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());	

			return true;
		}
		else if (m_invite_list[i].invite_time < m_invite_list[swap_pos].invite_time)
		{
			swap_pos = i;
		}
	}

	m_invite_list[swap_pos].beinvite_user_id = user_id;	
	m_invite_list[swap_pos].invite_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	return true;
}

bool GuildApplyForManager::RemoveInvite(const UserID &user_id)
{
	for (int i = 0; i < MAX_INVITE_COUNT; ++i)
	{
		if (!m_invite_list[i].Invalid() && m_invite_list[i].beinvite_user_id == user_id)
		{
			m_invite_list[i].Reset();

			return true;
		}
	}

	return false;
}

bool GuildApplyForManager::IsbeInvite(const UserID &user_id)
{
	for (int i = 0; i < MAX_INVITE_COUNT; ++i)
	{
		if (!m_invite_list[i].Invalid() && m_invite_list[i].beinvite_user_id == user_id)
		{
			return true;
		}
	}

	return false;
}
