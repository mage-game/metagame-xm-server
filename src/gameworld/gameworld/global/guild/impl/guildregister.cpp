#include "guildregister.hpp"

#include "servercommon/servercommon.h"
#include "global/guild/guild.hpp"
#include "global/guild/guildmanager.hpp"

#include "global/usercache/usercache.hpp"
#include "global/worldstatus/worldstatus.hpp"

GuildRegister::GuildRegister()
	: m_guild(NULL), m_member_count(0), m_tuanzhang_user_id(INVALID_USER_ID)
{
}

GuildRegister::~GuildRegister()
{

}

void GuildRegister::OnGuildTuanZhangChanged(const UserID &user_id)
{
	if (NULL != m_guild && UidToInt(m_tuanzhang_user_id) != 0)
	{
		if(WorldStatus::Instance().GetGCZChengzhuID() == UidToInt(m_tuanzhang_user_id))  //盟主改变时，取消城主
		{
			WorldStatus::Instance().OnSyncGCZInfo(m_guild->GetGuildID(), 0);
		}

		if (WorldStatus::Instance().GetKillCrossBossRankMengzhu() == UidToInt(m_tuanzhang_user_id))
		{
			WorldStatus::Instance().SyncKillCrossBossRankGuild(0, m_guild->GetGuildID());
		}
	}


	m_tuanzhang_user_id = user_id;	
}

UserID GuildRegister::GetGuildTuanZhangInfo(GameName role_name, char *vip_level)
{
	UserCacheNode *uc = UserCacheManager::Instance().GetUserNode(UidToInt(m_tuanzhang_user_id));
	if (NULL != uc)
	{
		F_STRNCPY(role_name, uc->user_name, sizeof(GameName));
		if (NULL != vip_level) *vip_level = uc->vip_level;
	}
			
	return m_tuanzhang_user_id;
}

char GuildRegister::GetGuildTuanzhangVipLevel()
{
	UserCacheNode *uc = UserCacheManager::Instance().GetUserNode(UidToInt(m_tuanzhang_user_id));
	if (NULL != uc)
	{
		return uc->vip_level;
	}

	return 0;
}

char GuildRegister::GetGuildTuanzhangProf()
{
	UserCacheNode *uc = UserCacheManager::Instance().GetUserNode(UidToInt(m_tuanzhang_user_id));
	if (NULL != uc)
	{
		return uc->prof;
	}

	return 0;
}

char GuildRegister::GetGuildTuanzhangSex()
{
	UserCacheNode *uc = UserCacheManager::Instance().GetUserNode(UidToInt(m_tuanzhang_user_id));
	if (NULL != uc)
	{
		return uc->sex;
	}

	return 0;
}

long long GuildRegister::GetGuildTuanzhangUuid()
{
	UserCacheNode *uc = UserCacheManager::Instance().GetUserNode(UidToInt(m_tuanzhang_user_id));
	if (NULL != uc)
	{
		return uc->uuid;
	}

	return 0;
}

