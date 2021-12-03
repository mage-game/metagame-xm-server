#include "towerdefendteamfunc.hpp"
#include "obj/character/role.h"
#include "towerdefendconfig.hpp"
#include "servercommon/errornum.h"
#include "servercommon/noticenum.h"
#include "scene/scenemanager.h"

//////////////////////////////////////////////////////////////////////////
RealiveDelayer::RealiveDelayer(SceneManager *scene_manager, SceneIndex scene_index, ObjID obj_id, const UserID &user_id, int hp_percent)
	: m_scene_manager(scene_manager), m_scene_index(scene_index), m_obj_id(obj_id), m_user_id(user_id), m_hp_percent(hp_percent)
{

}

void RealiveDelayer::OnTimer()
{
	Scene *scene = m_scene_manager->GetSceneByIndex(m_scene_index);
	if (NULL == scene) return;

	Role *role = m_scene_manager->GetRole(m_scene_index, m_obj_id);
	if (NULL == role || role->GetUserId() != m_user_id) return;

	if (role->IsAlive()) return;

	role->ReAlive(m_hp_percent, 0, role->GetPos());
}

void RealiveDelayer::Free()
{
	delete this;
}
