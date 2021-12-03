#include "quanmintafang.hpp"

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
#include "skill/skillbase.hpp"

WorldEventQuanmintafang::WorldEventQuanmintafang(WorldEventManager *world_event_manager)
	: WorldEvent(world_event_manager, WORLD_EVENT_TYPE_QUANMINTAFANG), m_alive_tower_count(0), m_door_count(0), 
	m_next_flush_time(0), m_next_addscore_time(0)
{

}

WorldEventQuanmintafang::~WorldEventQuanmintafang()
{

}

void WorldEventQuanmintafang::Init(const WorldEventParam &ev_param)
{
	m_score = ev_param.score_list[m_event_type];
}

void WorldEventQuanmintafang::GetInitParam(WorldEventParam *ev_param)
{
	ev_param->score_list[m_event_type] = m_score;
}

void WorldEventQuanmintafang::OnMonsterDisapper(Monster *monster)
{
	if (NULL == monster)
	{
		return;
	}

	if (LOGIC_CONFIG->GetWorldEventCfg().IsQuanmintafangEventMonster(monster->GetMonsterId()))
	{
		int tower_idx = monster->GetSpecialParam();
		if (tower_idx >= 0 && tower_idx < WORLD_EVENT_QUANMINGTAFANG_TOWER_MAX)
		{
			TowerItem &tower_item = m_tower_list[tower_idx];
			
			Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(tower_item.scene_id, 0);
			if (NULL != scene)
			{
				Obj *obj = scene->GetObj(tower_item.obj_id);
				if (NULL != obj && Obj::OBJ_TYPE_WORLDEVENT_OBJ == obj->GetObjType())
				{
					WorldEventObj *worldev_obj = (WorldEventObj*)obj;
					if (WORLD_EVENT_OBJ_TOWER == worldev_obj->GetWorldEventType())
					{
						QuanmintafangConfig &quanmintafang_cfg = LOGIC_CONFIG->GetWorldEventCfg().GetQuanmintafangConfig();

						worldev_obj->DecHp(quanmintafang_cfg.monster_disapper_hp);

						if (worldev_obj->GetHp() <= 0) // 塔死了，门也删掉
						{
							for (int i = 0; i < m_door_count; i++)
							{
								if (m_door_list[i].belong_tower == tower_idx)
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
												m_door_list[i].Reset();
											}
										}
									}
								}
							}

							tower_item.Reset();
							-- m_alive_tower_count;

							if (m_alive_tower_count <= 0)
							{
								this->ForceEndLogicState();
							}
						}
					}
				}
			}
		}
	}
}

void WorldEventQuanmintafang::OnKillMonster(Role *role, Monster *monster)
{
	if (NULL == role || NULL == monster)
	{
		return;
	}

	if (LOGIC_CONFIG->GetWorldEventCfg().IsQuanmintafangEventMonster(monster->GetMonsterId()))
	{
		QuanmintafangConfig &quanmintafang_cfg = LOGIC_CONFIG->GetWorldEventCfg().GetQuanmintafangConfig();

		role->GetRoleWorldEvent()->AddScore(m_event_type, quanmintafang_cfg.kill_monster_score);	
	}
}

void WorldEventQuanmintafang::Update(unsigned long interval, time_t now_second)
{
	if (m_event_type == m_world_event_manger->GetEventType() && WORLD_EVENT_LOGIC_TYPE_START == m_world_event_manger->GetLogicState())
	{
		QuanmintafangConfig &quanmintafang_cfg = LOGIC_CONFIG->GetWorldEventCfg().GetQuanmintafangConfig();

		if (now_second >= m_next_flush_time)
		{
			this->FlushMonster(); 
			m_next_flush_time = now_second + quanmintafang_cfg.flush_monster_interval;
		}
		
		if (now_second >= m_next_addscore_time)
		{
			this->TowerAddScore();
			m_next_addscore_time = now_second + quanmintafang_cfg.tower_addscore_interval;
		}
	}
}

void WorldEventQuanmintafang::FlushMonster()
{
	QuanmintafangConfig &quanmintafang_cfg = LOGIC_CONFIG->GetWorldEventCfg().GetQuanmintafangConfig();

	for (int i = 0; i < quanmintafang_cfg.towercfg_count && i < WORLD_EVENT_QUANMINGTAFANG_TOWER_MAX; i++)  // 4个塔
	{
		// 检查塔是否还健在
		{
			bool is_tower_alive = false;
			{
				if (m_tower_list[i].IsValid())
				{
					Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(m_tower_list[i].scene_id, 0);
					if (NULL != scene)
					{
						Obj *obj = scene->GetObj(m_tower_list[i].obj_id);
						if (NULL != obj && Obj::OBJ_TYPE_WORLDEVENT_OBJ == obj->GetObjType())
						{
							WorldEventObj *worldev_obj = (WorldEventObj*)obj;
							if (WORLD_EVENT_OBJ_TOWER == worldev_obj->GetWorldEventType())
							{
								is_tower_alive = true;
							}
						}
					}
				}
			}
			if (!is_tower_alive)
			{
				m_tower_list[i].Reset();
				continue;
			}
		}

		// 刷怪
		QuanmintafangConfig::TowerCfgItem &towercfg = quanmintafang_cfg.towercfg_list[i];
		for (int j = 0; j < towercfg.start_count; j++) // j条路
		{
			int monster_id = quanmintafang_cfg.RandomMonsterID();
			if (monster_id > 0)
			{
				Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(towercfg.scene_id, 0);
				if (NULL != scene) // 只设定路线 不主动清除怪 要策划配置好怪物消失时间
				{
					Posi deltaP = towercfg.disapper_pos - towercfg.start_pos_list[j];
					Scalar distance = 0;
					Dir dir = gamecommon::GetMoveDir(deltaP, &distance);

					Monster *monster = MONSTERPOOL->CreateMonster(monster_id, scene, towercfg.start_pos_list[j], dir, distance);
					if (NULL != monster)
					{
						monster->GetEffectList()->AddSpecialBuff(EBT_MIANYI_XUANYUN);
						monster->GetEffectList()->AddSpecialBuff(EBT_MIANYI_DINGSHEN);
						monster->GetEffectList()->AddSpecialBuff(EBT_MIANYI_CHIHUAN);

						monster->SetSpecialParam(i);		// 设置好这个怪属于哪个塔
					}
				}
			}
		}
	}
}

void WorldEventQuanmintafang::TowerAddScore()
{
	QuanmintafangConfig &quanmintafang_cfg = LOGIC_CONFIG->GetWorldEventCfg().GetQuanmintafangConfig();
	std::set<int> has_add_score_uid;

	for (int tower_i = 0; tower_i < WORLD_EVENT_QUANMINGTAFANG_TOWER_MAX; tower_i++)
	{
		if (!m_tower_list[tower_i].IsValid()) // 塔已挂 
		{
			continue;
		}

		QuanmintafangConfig::TowerCfgItem &tower_cfg = quanmintafang_cfg.towercfg_list[tower_i];					// 人为配置跟刷新保持一致

		Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(m_tower_list[tower_i].scene_id, 0);
		if (NULL != scene)
		{
			Obj *obj = scene->GetObj(m_tower_list[tower_i].obj_id);
			if (NULL != obj && Obj::OBJ_TYPE_WORLDEVENT_OBJ == obj->GetObjType())
			{
				WorldEventObj *worldev_obj = (WorldEventObj*)obj;
				if (WORLD_EVENT_OBJ_TOWER == worldev_obj->GetWorldEventType())
				{
					for (int start_i = 0; start_i < tower_cfg.start_count; start_i++)
					{
						int addscore_pos_x = (m_tower_list[tower_i].pos.x + tower_cfg.start_pos_list[start_i].x) / 2;			// 取开始点 跟塔的点的中心点
						int addscore_pos_y = (m_tower_list[tower_i].pos.y + tower_cfg.start_pos_list[start_i].y) / 2;

						static ObjID target_list[128] = {INVALID_OBJ_ID};
						int count = SkillBase::GetSceneObjByArea(scene, Posi(addscore_pos_x, addscore_pos_y), quanmintafang_cfg.tower_addscore_range, 
							target_list, sizeof(target_list) / sizeof(target_list[0]));
						if (count > 0)
						{
							for (int obj_j = 0; obj_j < count; obj_j++)
							{
								Obj *role_obj = scene->GetObj(target_list[obj_j]);
								if (NULL != role_obj && Obj::OBJ_TYPE_ROLE == role_obj->GetObjType())
								{
									Role *role = (Role*)role_obj;

									if (has_add_score_uid.end() == has_add_score_uid.find(role->GetUID()))
									{
										has_add_score_uid.insert(role->GetUID());
										role->GetRoleWorldEvent()->AddScore(m_event_type, quanmintafang_cfg.tower_addscore);
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

void WorldEventQuanmintafang::FlushTower()
{
	m_alive_tower_count = 0;

	QuanmintafangConfig &quanmintafang_cfg = LOGIC_CONFIG->GetWorldEventCfg().GetQuanmintafangConfig();

	for (int i = 0; i < quanmintafang_cfg.towercfg_count && i < WORLD_EVENT_QUANMINGTAFANG_TOWER_MAX; i++)
	{
		m_tower_list[i].Reset();

		QuanmintafangConfig::TowerCfgItem &tower_cfg = quanmintafang_cfg.towercfg_list[i];

		Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(tower_cfg.scene_id, 0);
		if (NULL != scene)
		{
			WorldEventObj *event_obj = LOGIC_CONFIG->GetWorldEventCfg().CreateWorldEventObj(quanmintafang_cfg.tower_event_id, scene, tower_cfg.disapper_pos);
			if (NULL != event_obj)
			{
				m_tower_list[i].scene_id = tower_cfg.scene_id;
				m_tower_list[i].pos = tower_cfg.disapper_pos;
				m_tower_list[i].obj_id = event_obj->GetId();
			}

			if (m_tower_list[i].IsValid()) // 塔刷出来
			{
				++ m_alive_tower_count;

				for (int j = 0; j < tower_cfg.start_count; ++ j)
				{
					WorldEventObj *event_obj = LOGIC_CONFIG->GetWorldEventCfg().CreateWorldEventObj(quanmintafang_cfg.door_event_id, scene, tower_cfg.start_pos_list[j]);
					if (NULL != event_obj)
					{
						if (m_door_count < DOOR_MAX_COUNT)
						{
							m_door_list[m_door_count].scene_id = event_obj->GetScene()->GetSceneID();
							m_door_list[m_door_count].obj_id = event_obj->GetId();
							m_door_list[m_door_count].belong_tower= i;
							++ m_door_count;
						}
					}
				}
			}
		}
	}
}

void WorldEventQuanmintafang::ClearTower()
{
	for (int i = 0; i < WORLD_EVENT_QUANMINGTAFANG_TOWER_MAX; i++)
	{
		Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(m_tower_list[i].scene_id, 0);
		if (NULL != scene)
		{
			Obj *obj = scene->GetObj(m_tower_list[i].obj_id);
			if (NULL != obj && Obj::OBJ_TYPE_WORLDEVENT_OBJ == obj->GetObjType())
			{
				WorldEventObj *worldev_obj = (WorldEventObj*)obj;
				if (WORLD_EVENT_OBJ_TOWER == worldev_obj->GetWorldEventType())
				{
					scene->DeleteObj(worldev_obj->GetId());
				}
			}
		}

		m_tower_list[i].Reset();
	}

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

void WorldEventQuanmintafang::OnLogicStart()
{
	this->FlushTower();
}

void WorldEventQuanmintafang::OnLogicStop()
{
	m_next_flush_time = 0;

	this->ClearTower();
}


