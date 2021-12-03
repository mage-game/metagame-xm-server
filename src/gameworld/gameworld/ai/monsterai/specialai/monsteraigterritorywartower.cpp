#include "monsteraigterritorywartower.hpp"

#include "obj/character/role.h"
#include "global/guild/guildmanager.hpp"
#include "global/guild/guild.hpp"
#include "config/logicconfigmanager.hpp"

MonsterAITerritoryWarTower::MonsterAITerritoryWarTower(Character *me)
	: MonsterAIStaticInitiative(me, MonsterInitParam::AITYPE_SPECIALAI_TERRITORYWAR_TOWER),m_side(-1)
{

}


MonsterAITerritoryWarTower::~MonsterAITerritoryWarTower()
{

}

void MonsterAITerritoryWarTower::Init(const MonsterAIParam &mp)
{
	MonsterAIStaticInitiative::Init(mp);
}

void MonsterAITerritoryWarTower::AI(unsigned long interval)
{
	if (!CheckSpecialStatus())
	{
		return;
	}

	switch (m_me->GetLogicResult())
	{
	case Character::MOVE_STATIC:
	case Character::LOGIC_NONE:
	case Character::MOVE_RUN:
	case Character::ATTACK_NONE:
		if (m_enemy != INVALID_OBJ_ID || FindEnemy(interval))
		{
			if (CanStartAttack())
			{
				StartAttack(m_enemy, true);
			}
			else 
			{
				m_me->StopAttack();
				m_enemy = INVALID_OBJ_ID;
			}
		}
		break;
	case Character::ATTACK_DONE:
	case Character::ATTACK_COOLDOWN:
	case Character::ATTACK_SPECIAL_STATE:
		if (CanStartAttack())				
		{
			SetNextSkill(m_enemy);
		}
		break;
	case Character::ATTACK_OUTOFRANG:
		m_me->StopAttack();
		m_enemy = INVALID_OBJ_ID;
		break;
	case Character::ACTION_PLAYING:
		break;
	default:
		break;
	}
}

bool MonsterAITerritoryWarTower::IsEnemy(Obj *obj)
{
	if (Obj::OBJ_TYPE_ROLE == obj->GetObjType())
	{
		Role *role = (Role*)obj;
		if (INVALID_GUILD_ID != role->GetGuildID())
		{
			Guild *guild = GuildManager::Instance().GetGuild(role->GetUserId());
			if (NULL != guild)
			{
				int rank = guild->GetGuildBattleRank() % 2;
				return rank != m_side && role->IsAlive();
			}
		}
	}

	return false;
}

bool MonsterAITerritoryWarTower::IsFriend(Obj *obj, bool check_alive)
{
	if (NULL != obj && Obj::OBJ_TYPE_ROLE == obj->GetObjType())
	{
		Role *role = (Role*)obj;

		if (INVALID_GUILD_ID != role->GetGuildID())
		{
			Guild *guild = GuildManager::Instance().GetGuild(role->GetUserId());
			if (NULL != guild)
			{
				int rank = guild->GetGuildBattleRank() % 2;
				return (!check_alive || role->IsAlive()) && rank == GetSide() && role->IsAlive();
			}
		}
	}

	return false;
}


