#include "tiannvsanhua.hpp"
#include "scene/scene.h"
#include "obj/character/monster.h"
#include "obj/obj.h"
#include "world.h"
#include "obj/character/role.h"
#include "config/logicconfigmanager.hpp"
#include "global/worldevent/worldeventconfig.hpp"
#include "obj/otherobj/worldeventobj.h"
#include "servercommon/errornum.h"
#include "global/worldevent/worldeventmanager.hpp"
#include "gamecommon.h"
#include "monster/drop/droppool.hpp"
#include "effect/skilleffect/effectspecialstate.hpp"
#include "scene/map.h"
#include "monster/monsterpool.h"
#include "obj/otherobj/fallingitem.h"

WorldEventTiannvshanhua::WorldEventTiannvshanhua(WorldEventManager *world_event_manager)
: WorldEvent(world_event_manager, WORLD_EVENT_TYPE_TIANNVSANHUA), m_last_refresh_falling_item_timestamp(0), m_last_cal_monster_move_pos_timestamp(0)
{

}

WorldEventTiannvshanhua::~WorldEventTiannvshanhua()
{

}


void WorldEventTiannvshanhua::Init(const WorldEventParam &ev_param)
{
	m_score = ev_param.score_list[m_event_type];
}

void WorldEventTiannvshanhua::GetInitParam(WorldEventParam *ev_param)
{
	ev_param->score_list[m_event_type] = m_score;
}

void WorldEventTiannvshanhua::Update(unsigned long interval, time_t now_second)
{
	if (m_event_type == m_world_event_manger->GetEventType() && WORLD_EVENT_LOGIC_TYPE_START == m_world_event_manger->GetLogicState())
	{
		this->CheckRefreshFallingItem(now_second);
		this->CalMonsterMovePos(now_second);
	}
}

void WorldEventTiannvshanhua::OnLogicStart()
{
	this->RefreshAllMonster();
}

void WorldEventTiannvshanhua::OnLogicStop()
{
	this->ClearAllMonster();
}

void WorldEventTiannvshanhua::CheckRefreshFallingItem(time_t now_second)
{
	if (now_second >= m_last_refresh_falling_item_timestamp + WORLD_EVENT_TIANNVSHANHUA_REFRESH_FALLING_ITEM_INTERVAL_S)
	{
		 m_last_refresh_falling_item_timestamp = now_second;

		const WorldEventOtherConfig &other_cfg = LOGIC_CONFIG->GetWorldEventCfg().GetOtherCfg();

		Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(other_cfg.tiannvshanhua_scene_id, 0);
		if (NULL == scene)
		{
			return;
		}

		const WorldEventFallingItemConfig &falling_item_cfg = LOGIC_CONFIG->GetWorldEventCfg().GetTiannvshanhuaFallingItemCfg();

		for (int i = 0; i < WORLD_EVENT_TIANNVSHANHUA_MONSTER_COUNT; ++ i)
		{
			MonsterItem &monster_item = m_monster_item[i];

			if (monster_item.IsValid())
			{
				Obj *obj = scene->GetObj(monster_item.obj_id);
				if (NULL != obj && Obj::OBJ_TYPE_MONSTER == obj->GetObjType())
				{
					Monster *monster = (Monster *)obj;
					if (monster->GetMonsterId() == other_cfg.tiannnvshanhua_monster_id)
					{
						for (int j = 0; j < falling_item_cfg.cfg_count; ++ j)
						{
							const WorldEventFallingItemConfig::ConfigItem &cfg_item = falling_item_cfg.cfg_list[j];

							Posi drop_pos = gamecommon::GetDisRandPos(monster->GetPos(), other_cfg.tiannvshanhua_drop_range);
							if (!scene->GetMap()->Validate(Obj::OBJ_TYPE_ROLE, drop_pos.x, drop_pos.y))
							{
								drop_pos = monster->GetPos();
							}

							DROPPOOL->DropToHelper(scene, BIND_GOLD_ITEM_ID, 0, true, cfg_item.bind_gold, 
								drop_pos, INVALID_USER_ID, monster->GetMonsterId(), false, FALLING_ITEM_BROADCAST_TYPE_NO_BROADCAST, false, NULL);
						}
					}
				}
			}
		}
	}
}

void WorldEventTiannvshanhua::CalMonsterMovePos(time_t now_second)
{
	const static int CHECK_INTERVAL = 1;

	if (now_second >= m_last_cal_monster_move_pos_timestamp + CHECK_INTERVAL)
	{
		m_last_cal_monster_move_pos_timestamp = now_second;

		const WorldEventOtherConfig &other_cfg = LOGIC_CONFIG->GetWorldEventCfg().GetOtherCfg();

		Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(other_cfg.tiannvshanhua_scene_id, 0);
		if (NULL == scene)
		{
			return;
		}

		for (int i = 0; i < WORLD_EVENT_TIANNVSHANHUA_MONSTER_COUNT; ++ i)
		{
			MonsterItem &monster_item = m_monster_item[i];
			if (monster_item.IsValid())
			{
				Obj *obj = scene->GetObj(monster_item.obj_id);
				if (NULL != obj && Obj::OBJ_TYPE_MONSTER == obj->GetObjType())
				{
					Monster *monster = (Monster *)obj;
					if (monster->GetMonsterId() == other_cfg.tiannnvshanhua_monster_id)
					{
						if (monster->GetDirDistance() <= 0)
						{
							if (monster_item.rest_time_s > 0)
							{
								monster_item.rest_time_s -= CHECK_INTERVAL;
								continue;
							}

							int curr_move_index = monster_item.curr_move_pos_index % WORLD_EVENT_TIANNVSHANHUA_MOVE_POINT_MAX_COUNT;
							Posi curr_target_pos = LOGIC_CONFIG->GetWorldEventCfg().GetTiannvshanhuaMonsterMovePos(i, curr_move_index);

							if ((monster->GetPos() - curr_target_pos).IsLongerThan(1))
							{
								Posi deltaP = curr_target_pos - monster->GetPos();
								Scalar distance = 0;
								Dir dir = gamecommon::GetMoveDir(deltaP, &distance);

								monster->SetDir(dir, distance);
							}
							else
							{
								int next_move_index = (curr_move_index + 1) % WORLD_EVENT_TIANNVSHANHUA_MOVE_POINT_MAX_COUNT;
								Posi next_target_pos = LOGIC_CONFIG->GetWorldEventCfg().GetTiannvshanhuaMonsterMovePos(i, next_move_index);

								Posi deltaP = next_target_pos - monster->GetPos();
								Scalar distance = 0;
								Dir dir = gamecommon::GetMoveDir(deltaP, &distance);

								monster->SetDir(dir, distance);

								monster_item.curr_move_pos_index = next_move_index;

								monster_item.rest_time_s = WORLD_EVENT_TIANNVSHANHUA_MONSTER_REST_TIME_S;
							}
						}
					}
				}
			}
		}
	}
}

void WorldEventTiannvshanhua::RefreshAllMonster()
{
	this->ClearAllMonster();

	const WorldEventOtherConfig &other_cfg = LOGIC_CONFIG->GetWorldEventCfg().GetOtherCfg();

	Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(other_cfg.tiannvshanhua_scene_id, 0);
	if (NULL != scene)
	{
		for (int i = 0; i < WORLD_EVENT_TIANNVSHANHUA_MONSTER_COUNT; ++ i)
		{
			MonsterItem &monster_item = m_monster_item[i];
			monster_item.Reset();

			Posi birth_pos = LOGIC_CONFIG->GetWorldEventCfg().GetTiannvshanhuaMonsterMovePos(i, 0);

			Monster *monster = MONSTERPOOL->CreateMonster(other_cfg.tiannnvshanhua_monster_id, scene, birth_pos);
			if (NULL != monster)
			{
				EffectSpecialState *special_effect = new EffectSpecialState(INVALID_OBJ_ID, 0, 3600 * 1000, EBT_WUDI, 1, EffectBase::PRODUCT_METHOD_SYSTEM);
				monster->AddEffect(special_effect);

				monster_item.obj_id = monster->GetId();
			}
		}
	}

	m_last_refresh_falling_item_timestamp = EngineAdapter::Instance().Time();
}

void WorldEventTiannvshanhua::ClearAllMonster()
{
	int tiannvsanhua_scene_id = LOGIC_CONFIG->GetWorldEventCfg().GetOtherCfg().tiannvshanhua_scene_id;
	int tiannvsanhua_monster_id = LOGIC_CONFIG->GetWorldEventCfg().GetOtherCfg().tiannnvshanhua_monster_id;

	for (int i = 0; i < WORLD_EVENT_TIANNVSHANHUA_MONSTER_COUNT; ++ i)
	{
		MonsterItem &monster_item = m_monster_item[i];
		if (!monster_item.IsValid())
		{
			continue;
		}

		Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(tiannvsanhua_scene_id, 0);
		if (NULL != scene)
		{
			Obj *obj = scene->GetObj(monster_item.obj_id);
			if (NULL != obj && Obj::OBJ_TYPE_MONSTER == obj->GetObjType())
			{
				Monster *monster_obj = (Monster *)obj;

				if (monster_obj->GetMonsterId() == tiannvsanhua_monster_id)
				{
					scene->DeleteObj(monster_item.obj_id);
				}
			}
		}

		monster_item.Reset();
	}
}



