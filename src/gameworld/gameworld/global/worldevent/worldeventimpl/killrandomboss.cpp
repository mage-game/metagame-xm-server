#include "killrandomboss.hpp"

#include "obj/character/role.h"
#include "world.h"
#include "scene/scene.h"
#include "obj/character/monster.h"
#include "monster/monsterpool.h"

#include "config/logicconfigmanager.hpp"
#include "global/worldevent/worldeventconfig.hpp"
#include "global/worldevent/roleworldevent.hpp"

WorldEventKillRandBoss::WorldEventKillRandBoss(WorldEventManager *world_event_manager)
	: WorldEvent(world_event_manager, WORLD_EVENT_TYPE_KILL_RANDOM_BOSS), m_boss_count(0), m_last_check_boss_alive_timestamp(0)
{

}

WorldEventKillRandBoss::~WorldEventKillRandBoss()
{

}

void WorldEventKillRandBoss::Init(const WorldEventParam &ev_param)
{
	m_score = ev_param.score_list[m_event_type];
}

void WorldEventKillRandBoss::GetInitParam(WorldEventParam *ev_param)
{
	ev_param->score_list[m_event_type] = m_score;
}

void WorldEventKillRandBoss::OnHurtMonster(Role *role, Monster *monster, int hurt)
{
	if (NULL == role || NULL == monster || hurt <= 0)
	{
		return;
	}

	if (LOGIC_CONFIG->GetWorldEventCfg().IsKillRandBossEventBoss(monster->GetMonsterId()))
	{
		role->GetRoleWorldEvent()->AddScore(m_event_type, hurt);
	}
}

void WorldEventKillRandBoss::OnLogicStart()
{
	this->ClearBoss();

	this->FlushBoss(); 
}

void WorldEventKillRandBoss::OnLogicStop()
{
	this->ClearBoss();
}

void WorldEventKillRandBoss::Update(unsigned long interval, time_t now_second)
{
	if (now_second >= m_last_check_boss_alive_timestamp + 3)
	{
		m_last_check_boss_alive_timestamp = now_second;

		bool has_alive_boss = false;
		{
			for (int i = 0; i < m_boss_count; i++)
			{
				BossItem &boss_item = m_boss_list[i];
				if (boss_item.IsVaild())
				{
					Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(boss_item.scene_id, 0);
					if (NULL != scene)
					{
						Obj *obj = scene->GetObj(boss_item.objid);
						if (NULL == obj || Obj::OBJ_TYPE_MONSTER != obj->GetObjType())
						{
							boss_item.Reset();
							continue;
						}

						Monster *monster = (Monster*)obj;
						if (monster->GetMonsterId() != boss_item.boss_id)
						{
							boss_item.Reset();
							continue;
						}

						has_alive_boss = true;
						break;
					}
				}
			}
		}
		if (!has_alive_boss)
		{
			this->ForceEndLogicState();
		}
	}
}

void WorldEventKillRandBoss::ClearBoss()
{
	for (int i = 0; i < m_boss_count; i++)
	{
		BossItem &boss_item = m_boss_list[i];
		if (boss_item.IsVaild())
		{
			Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(boss_item.scene_id, 0);
			if (NULL != scene)
			{
				Obj *obj = scene->GetObj(boss_item.objid);
				if (NULL != obj && Obj::OBJ_TYPE_MONSTER == obj->GetObjType())
				{
					Monster *monster = (Monster*)obj;
					if (monster->GetMonsterId() == boss_item.boss_id) // 确认是这个活动放出去的boss 要回收它了 
					{
						monster->ForceSetDead();
					}
				}
			}

			boss_item.Reset();
		}
	}

	m_boss_count = 0;
}

void WorldEventKillRandBoss::FlushBoss()
{
	m_boss_count = 0;

	KillRandomBossConfig randomboss_cfg = LOGIC_CONFIG->GetWorldEventCfg().GetKillRandomBossConfig();
	randomboss_cfg.RandPos();

	for (int i = 0; i < randomboss_cfg.flush_num; i++)
	{
		int boss_id = randomboss_cfg.RandomBossID();
		if (0 != boss_id)
		{
			KillRandomBossConfig::FlushPosCfgItem &poscfg_item = randomboss_cfg.pos_cfg_list[i];
			if (poscfg_item.IsVaild())
			{
				Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(poscfg_item.scene_id, 0);
				if (NULL != scene)
				{
					Monster *monster = MONSTERPOOL->CreateMonster(boss_id, scene, poscfg_item.pos);
					if (NULL != monster)
					{
						if (m_boss_count < WORLD_EVENT_RANDOM_FLUSH_BOSS_MAX)
						{
							m_boss_list[m_boss_count].scene_id = poscfg_item.scene_id;
							m_boss_list[m_boss_count].objid = monster->GetId();
							m_boss_list[m_boss_count].boss_id = boss_id;

							++ m_boss_count;
						}
					}
				}
			}
		}
	}
}




