#include "changescenedelayer.h"
#include "obj/character/role.h"
#include "scene/scenemanager.h"
#include "scene/scene.h"

#include "gamelog.h"

#include "world.h"

#include "protocal/msgscene.h"

ChangeSceneDelayer::ChangeSceneDelayer(SceneManager *scene_manager, 
				   SceneIndex scene_index, 
				   ObjID obj_id, 
				   const UserID &user_id, 
				   int target_scene_id, 
				   int target_scene_key, 
				   const Posi &target_pos,
				   int change_scene_reason)
	: m_scene_manager(scene_manager),
	m_scene_index(scene_index),
	m_obj_id(obj_id),
	m_user_id(user_id),
	m_target_scene_id(target_scene_id),
	m_target_scene_key(target_scene_key),
	m_target_pos(target_pos),
	m_change_scene_reason(change_scene_reason)
{
}

void ChangeSceneDelayer::OnTimer()
{
	Role *role = m_scene_manager->GetRole(m_scene_index, m_obj_id);
	if (NULL == role || role->GetUserId() != m_user_id) return;

	if (CHANGE_SCENE_REASON_INVALID != m_change_scene_reason)
	{
		Protocol::SCChangeSceneNotify csn;
		csn.change_scene_reason = m_change_scene_reason;
		csn.target_scene_id = m_target_scene_id;
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&csn, sizeof(csn));
	}

	gamelog::g_log_debug.printf(LL_DEBUG, "ChangeSceneDelayer::OnTimer: %s[%d %d]", role->GetName(), role->GetUserId().db_index, role->GetUserId().role_id);

	World::GetInstWorld()->GetSceneManager()->GoTo(role, m_target_scene_id, m_target_scene_key, m_target_pos);
}

void ChangeSceneDelayer::Free()
{
	delete this;
}

