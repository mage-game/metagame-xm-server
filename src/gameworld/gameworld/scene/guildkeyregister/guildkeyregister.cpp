#include "guildkeyregister.hpp"

#include "world.h"

#include "scene/scene.h"

GuildKeyRegister::GuildKeyRegister()
{

}

GuildKeyRegister::~GuildKeyRegister()
{

}

GuildKeyRegister & GuildKeyRegister::Instance()
{
	static GuildKeyRegister tkr;
	return tkr;
}

void GuildKeyRegister::RegisterGuildKey(const UserID &user_id, int scene_id, int scene_key, unsigned int create_time)
{
	m_user_guildkey_map[user_id] = GuildKey(scene_id, scene_key, create_time);
}

int GuildKeyRegister::GetGuildSceneKey(const UserID &user_id)
{
	UserGuildKeyMapIt it = m_user_guildkey_map.find(user_id);
	if (it != m_user_guildkey_map.end())
	{
		Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(it->second.scene_id, it->second.scene_key);
		if (NULL != scene && scene->GetCreateTime() == it->second.create_time)
		{
			m_user_guildkey_map.erase(it);

			return scene->GetSceneKey();
		}

		m_user_guildkey_map.erase(it);
	}

	return 0;
}

void GuildKeyRegister::RemoveGuildKey(int scene_key)
{
	for (UserGuildKeyMapIt it = m_user_guildkey_map.begin(); it != m_user_guildkey_map.end(); )
	{
		if (it->second.scene_key == scene_key)
		{
			m_user_guildkey_map.erase(it++);
		}
		else
		{
			++it;
		}
	}
}


