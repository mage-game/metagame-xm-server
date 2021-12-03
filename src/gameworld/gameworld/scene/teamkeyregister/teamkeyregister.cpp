#include "teamkeyregister.hpp"

#include "world.h"

#include "scene/scene.h"
#include "global/team/teammanager.hpp"
#include "global/team/team.hpp"

TeamKeyRegister::TeamKeyRegister()
{

}

TeamKeyRegister::~TeamKeyRegister()
{

}

TeamKeyRegister & TeamKeyRegister::Instance()
{
	static TeamKeyRegister tkr;
	return tkr;
}

void TeamKeyRegister::RegisterTeamKey(const UserID &user_id, int scene_id, int scene_key, unsigned int create_time)
{
	m_user_teamkey_map[user_id] = TeamKey(scene_id, scene_key, create_time);
}

int TeamKeyRegister::GetTeamSceneKey(const UserID &user_id)
{
	UserTeamKeyMapIt it = m_user_teamkey_map.find(user_id);
	if (it != m_user_teamkey_map.end())
	{
		Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(it->second.scene_id, it->second.scene_key);
		if (NULL != scene && scene->GetCreateTime() == it->second.create_time)
		{
			m_user_teamkey_map.erase(it);

			return scene->GetSceneKey();
		}

		m_user_teamkey_map.erase(it);
	}

	return 0;
}