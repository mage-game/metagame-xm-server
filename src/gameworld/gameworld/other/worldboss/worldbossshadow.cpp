#include "worldbossshadow.hpp"
#include "config/logicconfigmanager.hpp"
#include "world.h"
#include "scene/scene.h"
#include "gamelog.h"
#include "monster/monsterpool.h"
#include "servercommon/internalprotocal/gglobalprotocal.h"
#include "internalcomm.h"
#include "global/worldboss/worldbossbaseconfig.hpp"
#include "world.h"
#include "servercommon/string/gameworldstr.h"
#include "obj/character/monster.h"
#include "global/worldboss/worldbossmanager.hpp"

WorldBossShadow & WorldBossShadow::Instance()
{
	static WorldBossShadow shadow;
	return shadow;
}

WorldBossShadow::WorldBossShadow() : m_last_update_time(0)
{

}

WorldBossShadow::~WorldBossShadow()
{

}

void WorldBossShadow::Update(unsigned long interval)
{

}

void WorldBossShadow::OnBossRefresh(WorldBossManager::WorldBossInfo &world_boss)
{
	if (world_boss.boss_id < 0) return;

	const WorldBossInfoCfg *pos_cfg = LOGIC_CONFIG->GetWorldBossConfig().GetWorldBossByBossId(world_boss.boss_id);
	if (NULL == pos_cfg) return;

	Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(pos_cfg->scene_id, 0);
	if (NULL != scene)
	{
		int boss_id = world_boss.boss_id;

		Monster *new_monster = MONSTERPOOL->CreateMonster(boss_id, scene, pos_cfg->born_pos);
		if (NULL != new_monster)
		{
			world_boss.max_hp = new_monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
			world_boss.first_activity_hudun_hp = (long long)(world_boss.max_hp * (world_boss.first_trigger_hudun_per * ROLE_ATTR_PER));
			world_boss.second_activity_hudun_hp = (long long)(world_boss.max_hp * (world_boss.second_trigger_hudun_per * ROLE_ATTR_PER));
			world_boss.boss_status = WorldBossManager::ENBOSSSTATUS_EXIST;
			m_boss_set.insert(boss_id);
		}
	}
}

void WorldBossShadow::OnAttack(Role *attacker, Monster *monster, Attribute &delta_hp)
{
	if (NULL == monster || NULL == attacker || delta_hp >= 0)
	{
		return;
	}

	if (m_boss_set.find((int)monster->GetMonsterId()) == m_boss_set.end())
	{
		return;
	}

	WorldBossManager::Instance().OnAttack(attacker, monster, delta_hp);
}

void WorldBossShadow::OnBeenAttack(Role *attacker, Monster *monster, UInt16 skill_id, Attribute delta_hp, bool from_skill)
{
	if (NULL == monster || NULL == attacker || delta_hp >= 0)
	{
		return;
	}

	if (m_boss_set.find((int)monster->GetMonsterId()) == m_boss_set.end())
	{
		return;
	}

	WorldBossManager::Instance().OnBeenAttack(attacker, monster, skill_id, delta_hp, from_skill);
}

void WorldBossShadow::OnMonsterDie(int monster_id, int killer_uid, GameName killer_name)
{
	if (m_boss_set.find(monster_id) == m_boss_set.end()) return;

	m_boss_set.erase(monster_id);

	static gglobalprotocal::ReportWorldBossToGlobal sbft;
	sbft.event_type = gglobalprotocal::ReportWorldBossToGlobal::ENEVENTTYPE_DIED;
	sbft.boss_id = monster_id;
	sbft.killer_uid = killer_uid;
	F_STRNCPY(sbft.killer_name, killer_name, sizeof(GameName));

	WorldBossManager::Instance().MonsterDie(&sbft);
}

bool WorldBossShadow::IsWorldBossAndAlive(int boss_id)
{
	if (m_boss_set.find(boss_id) != m_boss_set.end())
	{
		return true;
	}

	return false;
}
