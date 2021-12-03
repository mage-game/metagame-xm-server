#include "shenmozhixi.hpp"
#include "scene/scene.h"
#include "obj/character/monster.h"
#include "obj/obj.h"
#include "world.h"
#include "obj/character/role.h"
#include "config/logicconfigmanager.hpp"
#include "global/worldevent/worldeventconfig.hpp"
#include "config/joinlimitconfig.hpp"
#include "obj/otherobj/worldeventobj.h"
#include "servercommon/errornum.h"
#include "other/rolestatus/rolestatusmanager.hpp"

WorldEventShenmozhiji::WorldEventShenmozhiji(WorldEventManager *world_event_manager)
: WorldEvent(world_event_manager, WORLD_EVENT_OBJ_SHENMOZHIXI)
{

}

WorldEventShenmozhiji::~WorldEventShenmozhiji()
{

}

void WorldEventShenmozhiji::Init(const WorldEventParam &ev_param)
{
	m_score = ev_param.score_list[m_event_type];
}

void WorldEventShenmozhiji::GetInitParam(WorldEventParam *ev_param)
{
	ev_param->score_list[m_event_type] = m_score;
}

void WorldEventShenmozhiji::OnLogicStart()
{
	this->RefreshAllPortal();
}

void WorldEventShenmozhiji::OnLogicStop()
{
	this->ClearAllPortal();
}

void WorldEventShenmozhiji::OnTouchEventObj(Role *role, int scene_id, ObjID obj_id)
{
	if (NULL == role)
	{
		return;
	}

	PortalItem *portal_item = this->FindPortal(scene_id, obj_id);
	if (NULL == portal_item)
	{
		return;
	}

	Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(scene_id, 0);
	if (NULL == scene)
	{
		return;
	}

	Obj *obj = scene->GetObj(obj_id);
	if (NULL == obj || Obj::OBJ_TYPE_WORLDEVENT_OBJ != obj->GetObjType())
	{
		return;
	}

	WorldEventObj *old_portal_obj = (WorldEventObj *)obj;
	if (WORLD_EVENT_OBJ_SHENMOZHIXI != old_portal_obj->GetWorldEventType())
	{
		return;
	}

	JoinLimitUnit limit_unit;
	bool is_limit = LOGIC_CONFIG->GetJoinLimitConfig().GetOtherJoinLimit(JLMT_OTHER_SHENMOZHIXI, &limit_unit);
	if (is_limit && role ->GetLevel() < limit_unit.level)
	{
		role->NoticeNum(errornum::EN_WORLD_EVENT_SHENMEZHIXI_FB_LEVEL_LIMIT);
		return;
	}

	if (!role->GetRoleStatusManager()->CanEnterFB())
	{
		return;
	}

	// ÖØË¢ÃÅ
	m_protal_pos_cfg.RandPos();
	portal_item->Reset();

	WorldEventPosItem new_pos_item;
	if (m_protal_pos_cfg.Top(&new_pos_item))
	{
		Scene *new_scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(new_pos_item.scene_id, 0);
		if (NULL != new_scene)
		{
			int portal_world_event_id = LOGIC_CONFIG->GetWorldEventCfg().GetOtherCfg().shenmozhixi_world_event_id;

			WorldEventObj *new_portal_obj = LOGIC_CONFIG->GetWorldEventCfg().CreateWorldEventObj(portal_world_event_id, new_scene, new_pos_item.pos);
			if (NULL != new_portal_obj)
			{
				portal_item->scene_id = new_pos_item.scene_id;
				portal_item->obj_id = new_portal_obj->GetId();

				m_protal_pos_cfg.Pop();
				m_protal_pos_cfg.Push(WorldEventPosItem(scene_id, old_portal_obj->GetPos()));
			}
		}
	}

	scene->DeleteObj(old_portal_obj->GetId());

	// ËÍÈëfb
	{
		const WorldEventOtherConfig &other_cfg = LOGIC_CONFIG->GetWorldEventCfg().GetOtherCfg();
		World::GetInstWorld()->GetSceneManager()->GoTo(role, other_cfg.shenmozhixi_fb_scene_id, role->GetUID(), other_cfg.shenmozhixi_fb_birth_pos);	
	}
}

WorldEventShenmozhiji::PortalItem * WorldEventShenmozhiji::FindPortal(int scene_id, ObjID obj_id)
{
	for (int i = 0; i < WORLD_EVENT_SHENMOZHIXI_PORTAL_MAX_COUNT; ++ i)
	{
		if (m_portal_item_list[i].IsValid())
		{
			if (scene_id == m_portal_item_list[i].scene_id && obj_id == m_portal_item_list[i].obj_id)
			{
				return &m_portal_item_list[i];
			}
		}
	}

	return NULL;
}

void WorldEventShenmozhiji::ClearAllPortal()
{
	for (int i = 0; i < WORLD_EVENT_SHENMOZHIXI_PORTAL_MAX_COUNT; ++ i)
	{
		PortalItem &portal_item = m_portal_item_list[i];
		if (!portal_item.IsValid())
		{
			continue;
		}

		Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(portal_item.scene_id, 0);
		if (NULL != scene)
		{
			Obj *obj = scene->GetObj(portal_item.obj_id);
			if (NULL != obj && Obj::OBJ_TYPE_WORLDEVENT_OBJ == obj->GetObjType())
			{
				WorldEventObj *world_event_obj = (WorldEventObj *)obj;
				if (WORLD_EVENT_OBJ_SHENMOZHIXI == world_event_obj->GetWorldEventType())
				{
					scene->DeleteObj(portal_item.obj_id);
				}
			}
		}

		portal_item.Reset();
	}
}

void WorldEventShenmozhiji::RefreshAllPortal()
{
	this->ClearAllPortal();

	m_protal_pos_cfg = LOGIC_CONFIG->GetWorldEventCfg().GetShenmozhijiPosCfg();
	m_protal_pos_cfg.RandPos();

	int portal_world_event_id = LOGIC_CONFIG->GetWorldEventCfg().GetOtherCfg().shenmozhixi_world_event_id;

	for (int i = 0; i < WORLD_EVENT_SHENMOZHIXI_PORTAL_MAX_COUNT; ++ i)
	{
		PortalItem &portal_item = m_portal_item_list[i];
		portal_item.Reset();
		
		WorldEventPosItem pos_item;
		if (m_protal_pos_cfg.Top(&pos_item))
		{
			Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(pos_item.scene_id, 0);
			if (NULL != scene)
			{
				WorldEventObj *portal_obj = LOGIC_CONFIG->GetWorldEventCfg().CreateWorldEventObj(portal_world_event_id, scene, pos_item.pos);
				if (NULL != portal_obj)
				{
					portal_item.scene_id = portal_obj->GetScene()->GetSceneID();
					portal_item.obj_id = portal_obj->GetId();

					m_protal_pos_cfg.Pop();
				}
			}
		}
	}
}


