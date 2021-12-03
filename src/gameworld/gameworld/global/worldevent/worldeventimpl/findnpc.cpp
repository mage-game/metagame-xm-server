#include "findnpc.hpp"
#include "scene/scene.h"
#include "obj/character/monster.h"
#include "obj/obj.h"
#include "world.h"
#include "obj/character/role.h"
#include "config/logicconfigmanager.hpp"
#include "monster/monsterpool.h"
#include "global/worldevent/worldeventconfig.hpp"
#include "obj/otherobj/worldeventobj.h"
#include "servercommon/errornum.h"
#include "protocal/msgworldevent.h"
#include "global/worldevent/roleworldevent.hpp"

WorldEventFindNpc::WorldEventFindNpc(WorldEventManager *world_event_manager)
	: WorldEvent(world_event_manager, WORLD_EVENT_TYPE_FIND_NPC)
{

}

WorldEventFindNpc::~WorldEventFindNpc()
{
	
}

void WorldEventFindNpc::Init(const WorldEventParam &ev_param)
{
	m_score = ev_param.score_list[m_event_type];
}

void WorldEventFindNpc::GetInitParam(WorldEventParam *ev_param)
{
	ev_param->score_list[m_event_type] = m_score;
}

void WorldEventFindNpc::Update(unsigned long interval, time_t now_second)
{
	
}

void WorldEventFindNpc::OnLogicStand()
{
	
}

void WorldEventFindNpc::OnLogicStart()
{
	this->RefreshAllNPC();
}

void WorldEventFindNpc::OnLogicStop()
{
	this->ClearAllNPC();
}

void WorldEventFindNpc::OnTouchEventObj(Role *role, int scene_id, ObjID obj_id)
{
	if (NULL == role)
	{
		return;
	}

	NPCItem *npc_item = this->FindNpc(scene_id, obj_id);
	if (NULL == npc_item)
	{
		return;
	}

	Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(scene_id, 0);
	if (NULL == scene)
	{
		return;
	}

	Obj *obj = scene->GetObj(obj_id);
	if (NULL == obj || (NULL != obj && Obj::OBJ_TYPE_WORLDEVENT_OBJ != obj->GetObjType()))
	{
		return;
	}

	WorldEventObj *npc_obj = (WorldEventObj *)obj;
	if (WORLD_EVENT_OBJ_NPC != npc_obj->GetWorldEventType())
	{
		return;
	}

	if (npc_obj->GetWorldEventID() != role->GetRoleWorldEvent()->GetCurrFindNpcId())
	{
		role->NoticeNum(errornum::EN_WORLD_EVENT_NOT_FIT_NPC);
		return;
	}

	int npc_index = -1;
	if (!LOGIC_CONFIG->GetWorldEventCfg().GetNpcIDIndex(npc_obj->GetWorldEventID(), &npc_index))
	{
		return;
	}

	// 加分 设置找到npc
	role->GetRoleWorldEvent()->AddScore(m_event_type, 1);
	role->GetRoleWorldEvent()->SetFindNpc(npc_index);

	// 刷怪
	if (MONSTERPOOL->IsMonsterExist(npc_obj->GetEventParam()))
	{
		Monster *monster = MONSTERPOOL->CreateMonster(npc_obj->GetEventParam(), scene, npc_obj->GetPos());
		if (NULL != monster)
		{
			monster->SetOwnerUserId(role->GetUserId(), false);
		}
	}

	
	Posi old_npc_pos = npc_obj->GetPos();

	WorldEventSceneConfigItem *scene_cfg_item = m_npc_pos_cfg.FindSceneCfg(scene_id);
	if (NULL != scene_cfg_item)
	{
		scene_cfg_item->RandPos();

		Posi new_npc_pos(0, 0);
		if (scene_cfg_item->TopPos(&new_npc_pos))
		{
			WorldEventObj *new_npc_obj = LOGIC_CONFIG->GetWorldEventCfg().CreateWorldEventObj(npc_item->npc_id, scene, new_npc_pos);
			if (NULL != new_npc_obj)
			{
				npc_item->Reset();

				npc_item->npc_id = new_npc_obj->GetWorldEventID();
				npc_item->scene_id = scene_id;
				npc_item->obj_id = new_npc_obj->GetId();

				scene_cfg_item->PopPos();
				scene_cfg_item->PushPos(old_npc_pos);

				scene->DeleteObj(obj_id);
				npc_obj = NULL; // 后边不能再用这个指针
			}
		}
	}
}

void WorldEventFindNpc::OnKillMonster(Role *role, Monster *monster)
{
	if (NULL == role || NULL == monster)
	{
		return;
	}

	if (!LOGIC_CONFIG->GetWorldEventCfg().IsFindNpcMonster(monster->GetMonsterId()))
	{
		return;
	}

	if (role->GetUserId() != monster->GetOwnerUserId())
	{
		return;
	}

	Protocol::SCWorldEventKillFindNpcMonster wekfnm;
	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&wekfnm, sizeof(wekfnm));
}

void WorldEventFindNpc::ClearAllNPC()
{
	for (int i = 0; i < WORLD_EVENT_NPC_MAX_COUNT; ++ i)	
	{
		NPCItem &npc_item = m_npc_list[i];

		if (npc_item.IsValid())
		{
			Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(npc_item.scene_id, 0);
			if (NULL != scene)
			{
				Obj *obj = scene->GetObj(npc_item.obj_id);
				if (NULL != obj && Obj::OBJ_TYPE_WORLDEVENT_OBJ == obj->GetObjType())
				{
					WorldEventObj *ev_obj = (WorldEventObj*)obj;
					if (WORLD_EVENT_OBJ_NPC == ev_obj->GetWorldEventType())
					{
						scene->DeleteObj(obj->GetId());
					}
				}
			}
		}

		npc_item.Reset();
	}
}

void WorldEventFindNpc::RefreshAllNPC()
{
	this->ClearAllNPC();

	m_npc_pos_cfg = LOGIC_CONFIG->GetWorldEventCfg().GetNpcPosCfg();

	UNSTD_STATIC_CHECK(WORLD_EVENT_NPC_LOCATE_SCENE_MAX_COUNT * WORLD_EVENT_NPC_COUNT_PER_SCENE == 
		WORLD_EVENT_NPC_TYPE_MAX_COUNT * WORLD_EVENT_NPC_MAX_COUNT_PER_TYPE);

	{
		int npc_id_list[WORLD_EVENT_NPC_TYPE_MAX_COUNT];
		LOGIC_CONFIG->GetWorldEventCfg().GetNpcIDList(npc_id_list);

		for (int i = 0; i < WORLD_EVENT_NPC_TYPE_MAX_COUNT; ++ i)					// 把NPC分配到m_npc_list 保证每种npc有3个
		{
			int npc_id = npc_id_list[i];

			for (int j = 0; j < WORLD_EVENT_NPC_MAX_COUNT_PER_TYPE; ++ j)
			{
				int npc_index = i * WORLD_EVENT_NPC_MAX_COUNT_PER_TYPE + j;

				m_npc_list[npc_index].npc_id = npc_id;
			}
		}

		for (int i = 0; i < WORLD_EVENT_NPC_MAX_COUNT; ++ i)						// 打乱NPC顺序
		{
			int swap_index = RandomNum(WORLD_EVENT_NPC_MAX_COUNT);

			NPCItem tmp_item = m_npc_list[i];
			m_npc_list[i] = m_npc_list[swap_index];
			m_npc_list[swap_index] = tmp_item;
		}
	}

	{																				// 给npc分配位置, 并刷出来
		for (int i = 0; i < WORLD_EVENT_NPC_LOCATE_SCENE_MAX_COUNT; ++ i)
		{
			WorldEventSceneConfigItem &scene_cfg = m_npc_pos_cfg.scene_list[i];
			scene_cfg.RandPos();

			Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(scene_cfg.scene_id, 0);
			if (NULL == scene)
			{
				continue;
			}

			for (int j = 0; j < WORLD_EVENT_NPC_COUNT_PER_SCENE; ++ j)
			{
				NPCItem &npc_item = m_npc_list[i * WORLD_EVENT_NPC_COUNT_PER_SCENE + j];

				Posi npc_pos(0, 0);
				scene_cfg.TopPos(&npc_pos);

				WorldEventObj * npc_obj = LOGIC_CONFIG->GetWorldEventCfg().CreateWorldEventObj(npc_item.npc_id, scene, npc_pos);
				if (NULL == npc_obj)
				{
					npc_item.Reset();
					continue;
				}

				npc_item.scene_id = scene_cfg.scene_id;
				npc_item.obj_id = npc_obj->GetId();

				scene_cfg.PopPos();
			}
		}
	}
}

void WorldEventFindNpc::GetNpcLocateInfo(Role *role, int npc_id)
{
	if (NULL == role)
	{
		return;
	}

	Protocol::SCWorldEventNpcLocateInfo wenli;
	wenli.find_npc_id = npc_id;
	wenli.locate_scene_id_count = 0;

	for (int i = 0; i < WORLD_EVENT_NPC_MAX_COUNT && wenli.locate_scene_id_count < WORLD_EVENT_NPC_MAX_COUNT_PER_TYPE; ++ i)
	{
		const NPCItem &npc_item = m_npc_list[i];

		if (npc_item.npc_id == npc_id)
		{
			wenli.locate_scend_idlist[wenli.locate_scene_id_count] = npc_item.scene_id;
			++ wenli.locate_scene_id_count;
		}
	}

	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&wenli, sizeof(wenli));
}

WorldEventFindNpc::NPCItem * WorldEventFindNpc::FindNpc(int scene_id, ObjID obj_id)
{
	for (int i = 0; i < WORLD_EVENT_NPC_MAX_COUNT; ++ i)
	{
		if (m_npc_list[i].IsValid())
		{
			if (scene_id == m_npc_list[i].scene_id && obj_id == m_npc_list[i].obj_id)
			{
				return &m_npc_list[i];
			}
		}
	}

	return NULL;
}




