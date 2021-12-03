#include "monsteraistaticinitiative.hpp"

#include "obj/character/monster.h"
#include "scene/scene.h"

MonsterAIStaticInitiative::MonsterAIStaticInitiative(Character *me, int aitype)
	: MonsterAIBase(me, aitype), 
	m_aoi_range(0), 
	m_aoi_handle(-1),
	m_enemy_size(0)
{

}

MonsterAIStaticInitiative::~MonsterAIStaticInitiative()
{

}

void MonsterAIStaticInitiative::Init(const MonsterAIParam &mp)
{
	MonsterAIBase::Init(mp);
	m_aoi_range = mp.aoi_range;
}

void MonsterAIStaticInitiative::OnEnterScene()
{
	m_aoi_handle = m_me->GetScene()->GetZoneMatrix()->CreateCircleAOI(m_me->GetId(), m_birth_pos, m_aoi_range, m_aoi_range);
}

void MonsterAIStaticInitiative::OnLeaveScene()
{
	m_me->GetScene()->GetZoneMatrix()->DestroyAOI(m_aoi_handle);
}

void MonsterAIStaticInitiative::OnAOIEnter(ObjID obj_id)
{
	Obj *obj = m_me->GetScene()->GetObj(obj_id);
	if (NULL == obj) return;

	if (m_me->IsEnemy(obj))
	{
		for (int i = 0; i < m_enemy_size; i++)
		{
			if (m_enemy_list[i] == obj_id) return;
		}

		if (m_enemy_size < MAX_ENEMY_NUM)
		{
			m_enemy_list[m_enemy_size] = obj_id;
			++ m_enemy_size;

			if (m_enemy == INVALID_OBJ_ID)
			{
				m_enemy = obj_id;
				StartAttack(m_enemy, false);
			}
		}
	}
}

void MonsterAIStaticInitiative::OnAOILeave(ObjID obj_id)
{
	Obj *obj = m_me->GetScene()->GetObj(obj_id);
	if (NULL == obj) return;

	if (m_me->IsEnemy(obj))
	{
		for (int i = 0; i < m_enemy_size; ++i)
		{
			if (m_enemy_list[i] == obj_id)
			{
				m_enemy_list[i] = m_enemy_list[m_enemy_size - 1];
				--m_enemy_size;

				if (m_enemy == obj_id)
				{
					m_me->StopAttack();
					m_enemy = INVALID_OBJ_ID;
				}
				break;
			}
		}
	}
}

void MonsterAIStaticInitiative::AI(unsigned long interval)
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
		if (CanStartAttack())				//	战场将帅 除默认技外只有一个群技 且CD=0 会不断SetNextSkill施放 即使enemy已死
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

bool MonsterAIStaticInitiative::FindEnemy(unsigned long interval)
{
	if (m_enemy != INVALID_OBJ_ID)
	{
		return false;
	}

	while (m_enemy_size > 0)
	{
		Obj *obj = m_me->GetScene()->GetObj(m_enemy_list[0]);
		if (obj == 0 || !m_me->IsEnemy(obj))
		{
			-- m_enemy_size;
			m_enemy_list[0] = m_enemy_list[m_enemy_size];

			continue;
		}

		if (m_enemy_size > 0) m_enemy = m_enemy_list[RandomNum(m_enemy_size)];
		
		return true;
	}

	//if (m_enemy_size > 0)
	//{
	//	if (m_enemy_size > 0) m_enemy = m_enemy_list[RandomNum(m_enemy_size)];

	//	return true;
	//}

	m_enemy = INVALID_OBJ_ID;

	m_enemy_size = this->SearchNearEnemys(interval, m_birth_pos, m_aoi_range, m_enemy_list, MAX_ENEMY_NUM);

	if (m_enemy_size > 0)
	{
		m_enemy = m_enemy_list[RandomNum(m_enemy_size)];
		return true;
	}

	return false;
}

