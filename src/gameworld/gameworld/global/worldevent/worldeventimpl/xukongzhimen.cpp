#include "xukongzhimen.hpp"

#include "obj/character/role.h"
#include "world.h"
#include "scene/scene.h"
#include "obj/character/monster.h"
#include "obj/otherobj/worldeventobj.h"
#include "monster/monsterpool.h"

#include "config/logicconfigmanager.hpp"
#include "global/worldevent/worldeventconfig.hpp"
#include "global/worldevent/worldeventmanager.hpp"
#include "global/worldevent/roleworldevent.hpp"
#include "effect/skilleffect/effectspecialstate.hpp"

#include "gamecommon.h"
#include "other/specialappearance/specialappearance.hpp"

WorldEventXukongzhimen::WorldEventXukongzhimen(WorldEventManager *world_event_manager)
	: WorldEvent(world_event_manager, WORLD_EVENT_TYPE_XUKONGZHIMEN), 
	m_door_count(0), m_next_flush_monster_time(0)
{

}

WorldEventXukongzhimen::~WorldEventXukongzhimen()
{

}

void WorldEventXukongzhimen::Init(const WorldEventParam &ev_param)
{
	m_score = ev_param.score_list[m_event_type];
}

void WorldEventXukongzhimen::GetInitParam(WorldEventParam *ev_param)
{
	ev_param->score_list[m_event_type] = m_score;
}

void WorldEventXukongzhimen::Update(unsigned long interval, time_t now_second)
{
	if (m_event_type == m_world_event_manger->GetEventType() && WORLD_EVENT_LOGIC_TYPE_START == m_world_event_manger->GetLogicState())
	{
		if (now_second >= m_next_flush_monster_time)
		{
			XukongzhimengConfig &xukongzhimeng_cfg = LOGIC_CONFIG->GetWorldEventCfg().GetXukongzhimengConfig();

			this->FlushMonster();
			m_next_flush_monster_time = now_second + xukongzhimeng_cfg.flush_monster_interval;
		}
	}
}

void WorldEventXukongzhimen::FlushMonster()
{
	XukongzhimengConfig &xukongzhimeng_cfg = LOGIC_CONFIG->GetWorldEventCfg().GetXukongzhimengConfig();
	for (int i = 0; i < xukongzhimeng_cfg.doorcfg_count; i++) // 4个虚空之门
	{
		XukongzhimengConfig::DoorCfgItem &door = xukongzhimeng_cfg.doorcfg_list[i];
		for (int j = 0; j < door.disapper_count; j++)	   // 每个虚空之门有j个路线
		{
			int monster_id = xukongzhimeng_cfg.RandomMonsterID();
			if (monster_id > 0)
			{
				Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(door.scene_id, 0);
				if (NULL != scene)   // 只设定路线 不主动清除怪 要策划配置好怪物消失时间
				{
					Posi deltaP = door.disapper_pos_list[j] - door.start_pos;
					Scalar distance = 0;
					Dir dir = gamecommon::GetMoveDir(deltaP, &distance);
					Monster *monster = MONSTERPOOL->CreateMonster(monster_id, scene, door.start_pos, dir, distance);

					if (NULL != monster)
					{
						monster->GetEffectList()->AddSpecialBuff(EBT_MIANYI_XUANYUN);
						monster->GetEffectList()->AddSpecialBuff(EBT_MIANYI_DINGSHEN);
						monster->GetEffectList()->AddSpecialBuff(EBT_MIANYI_CHIHUAN);
					}
				}
			}
		}
	}
}

void WorldEventXukongzhimen::OnLogicStart()
{
	this->FlushDoor();
}

void WorldEventXukongzhimen::OnLogicStop()
{
	m_next_flush_monster_time = 0;

	this->ClearDoor();
}

void WorldEventXukongzhimen::OnSetOtherSpecialAppearance(Role *role, int special_appearacne_type)
{
	if (NULL == role)
	{
		return;
	}

	if (SPECIAL_APPERANCE_TYPE_WORD_EVENT_YURENCARD == special_appearacne_type)
	{
		role->GetRoleWorldEvent()->AddScore(m_event_type, 1);
	}
}

void WorldEventXukongzhimen::FlushDoor()
{
	m_door_count = 0;

	XukongzhimengConfig &xukongzhimeng_cfg = LOGIC_CONFIG->GetWorldEventCfg().GetXukongzhimengConfig();

	for (int i = 0; i < xukongzhimeng_cfg.doorcfg_count; i++) // 4个虚空之门
	{
		XukongzhimengConfig::DoorCfgItem &door_cfg = xukongzhimeng_cfg.doorcfg_list[i];

		Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(door_cfg.scene_id, 0);
		if (NULL != scene)
		{
			WorldEventObj *event_obj = LOGIC_CONFIG->GetWorldEventCfg().CreateWorldEventObj(xukongzhimeng_cfg.door_event_id, scene, door_cfg.start_pos);
			if (NULL != event_obj)
			{
				if (m_door_count < WORLD_EVENT_XUKONGZHIMENG_DOOR_MAX)
				{
					m_door_list[m_door_count].scene_id = door_cfg.scene_id;
					m_door_list[m_door_count].obj_id = event_obj->GetId();

					++ m_door_count;
				}
			}
		}
	}
}

void WorldEventXukongzhimen::ClearDoor()
{
	for (int i = 0; i < m_door_count; i++)
	{
		Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(m_door_list[i].scene_id, 0);
		if (NULL != scene)
		{
			Obj *obj = scene->GetObj(m_door_list[i].obj_id);
			if (NULL != obj && Obj::OBJ_TYPE_WORLDEVENT_OBJ == obj->GetObjType())
			{
				WorldEventObj *worldev_obj = (WorldEventObj*)obj;
				if (WORLD_EVENT_OBJ_XUKONGZHIMENG == worldev_obj->GetWorldEventType())
				{
					scene->DeleteObj(worldev_obj->GetId());
				}
			}
		}
	}

	m_door_count = 0;
}








