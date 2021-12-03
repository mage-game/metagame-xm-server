#include "createmonsterdelayer.h"

#include "scene/scenemanager.h"
#include "scene/scene.h"

#include "monsterpool.h"

#include "obj/character/monster.h"
#include "obj/character/role.h"

#include "ai/monsterai/monsteraibase.hpp"

#include "world.h"

CreateMonsterDelayer::CreateMonsterDelayer(SceneManager *scene_manager, 
					SceneIndex scene_index, 
					UInt16 monster_id, 
					const Posi &pos, 
					ObjID enemy_objid, 
					const UserID &enemy_user_id, 
					const UserID &owner_user_id, 
					int add_maxhp)
	: m_scene_manager(scene_manager), 
	m_scene_index(scene_index), 
	m_monster_id(monster_id), 
	m_pos(pos), 
	m_enemy_objid(enemy_objid), 
	m_enemy_user_id(enemy_user_id),
	m_owner_user_id(owner_user_id), 
	m_add_maxhp(add_maxhp)
{

}

void CreateMonsterDelayer::OnTimer()
{
	Scene *scene = m_scene_manager->GetSceneByIndex(m_scene_index);
	if (NULL == scene) return;

	Monster *monster = MONSTERPOOL->CreateMonster(m_monster_id, scene, m_pos, m_add_maxhp);
	if (NULL != monster)
	{
		monster->SetOwnerUserId(m_owner_user_id);

		if (INVALID_OBJ_ID != m_enemy_objid && INVALID_USER_ID != m_enemy_user_id)
		{
			Obj *enemy_obj = scene->GetObj(m_enemy_objid);
			if (NULL != enemy_obj && Obj::OBJ_TYPE_ROLE == enemy_obj->GetObjType())
			{
				Role *enemy_role = (Role*)enemy_obj;
				if (enemy_role->GetUserId() == m_enemy_user_id)
				{
					monster->ForceSetEnemy(m_enemy_objid);
				}
			}
		}
	}
}

void CreateMonsterDelayer::Free()
{
	delete this;
}

