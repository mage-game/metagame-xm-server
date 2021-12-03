#include "monsteraiinitiative.hpp"

#include "obj/character/monster.h"
#include "scene/scene.h"

MonsterAIInitiative::MonsterAIInitiative(Character *me, int aitype)
	: MonsterAIMoveAround(me, aitype), 
	m_aoi_handle(-1),
	m_aoi_range(15),
	m_enemy_size(0)
{

}

MonsterAIInitiative::~MonsterAIInitiative()
{

}

void MonsterAIInitiative::Init(const MonsterAIParam &mp)
{
	MonsterAIMoveAround::Init(mp);
	m_aoi_range = mp.aoi_range;
}

void MonsterAIInitiative::OnEnterScene()
{
	m_aoi_handle = m_me->GetScene()->GetZoneMatrix()->CreateCircleAOI(m_me->GetId(), m_birth_pos, m_aoi_range, m_aoi_range);
}

void MonsterAIInitiative::OnLeaveScene()
{
	m_me->GetScene()->GetZoneMatrix()->DestroyAOI(m_aoi_handle);
}

void MonsterAIInitiative::OnAOIEnter(ObjID obj_id)
{ 
	if (obj_id == m_enemy) return;

	Obj *obj = m_me->GetScene()->GetObj(obj_id);
	if (NULL == obj) return;
	
	if (this->IsEnemy(obj))
	{
		for (int i = 0; i < (int)m_enemy_size; i++)
		{
			if (m_enemy_list[i] == obj_id) return;
		}

		if (m_enemy_size < MAX_ENEMY_NUM)
		{
			m_enemy_list[m_enemy_size] = obj_id;
			++ m_enemy_size;
		}

		if (m_enemy == INVALID_OBJ_ID)
		{
			if (FindEnemy())	
			{
				StartAttack(m_enemy, false);
			}
		}
	}
}

void MonsterAIInitiative::OnAOILeave(ObjID obj_id)
{
	Obj *obj = m_me->GetScene()->GetObj(obj_id);
	if (NULL == obj) return;

	if (this->IsEnemy(obj))
	{
		for (unsigned int i = 0; i < m_enemy_size; ++i)
		{
			if (m_enemy_list[i] == obj_id)
			{
				if (i != m_enemy_size - 1)
				{
					m_enemy_list[i] = m_enemy_list[m_enemy_size - 1];
				}
				--m_enemy_size;
				break;
			}
		}
	}
}

void MonsterAIInitiative::AI(unsigned long interval)
{
	if (!CheckSpecialStatus())
	{
		return;
	}

	switch (m_me->GetLogicResult())
	{
	case Character::MOVE_STATIC:
	case Character::ATTACK_NONE:
	case Character::LOGIC_NONE:
	case Character::MOVE_RUN:
		if ( (m_enemy == INVALID_OBJ_ID && !FindEnemy(interval)) || IsOutRange())
		{
			if (m_enemy != INVALID_OBJ_ID)
			{
				m_rest_time = 0;	// 让其立刻回去 不然主动怪就会在该处休息 则可以在这段时间中不断攻击它
				m_enemy = INVALID_OBJ_ID;
				m_me->MoveStop();
			}
			MoveAround(interval);
		}
		else
		{
			if (CanStartAttack())
			{
				StartAttack(m_enemy, true);
			}
			else
			{
				Follow(m_enemy, interval);
			}
		}
		break;
	case Character::ACTION_PLAYING:
		break;
	case Character::ATTACK_DONE:
	case Character::ATTACK_COOLDOWN:
	case Character::ATTACK_SPECIAL_STATE:
		if (this->CanStartAttack())
		{
			SetNextSkill(m_enemy);
		}
		break;
	case Character::ATTACK_OUTOFRANG:
		Follow(m_enemy, interval);
		break;
	}
}

// 查找敌人，找到敌人后，随机返回一个（赋值给m_enemy）
bool MonsterAIInitiative::FindEnemy(unsigned long interval)
{
	if (this->CheckHateDrive()) return true;

	while (m_enemy_size != 0)
	{
		Obj *obj = m_me->GetScene()->GetObj(m_enemy_list[0]); // 没有删除正在攻击的敌人
		if (NULL == obj || !m_me->IsEnemy(obj))
		{
			--m_enemy_size;
			if (m_enemy_size != 0)
			{
				m_enemy_list[0] = m_enemy_list[m_enemy_size];
			}

			continue;
		}

		if (m_enemy_size > 0) m_enemy = m_enemy_list[RandomNum(m_enemy_size)];

		return true;
	}

	//if (m_enemy_size > 0)
	//{
	//	if (m_enemy_size > 0) m_enemy = m_enemy_list[RandomNum(m_enemy_size)];
	//
	//	return true;
	//}

	m_enemy = INVALID_OBJ_ID;

	m_enemy_size = this->SearchNearEnemys(interval, m_birth_pos, m_aoi_range, m_enemy_list, MAX_ENEMY_NUM);

	if (m_enemy_size > 0)
	{
		m_enemy = m_enemy_list[RandomNum(m_enemy_size)];
		return true;
	}
	
	this->OnEnemyLost();

	return false;
}

// 能否攻击
bool MonsterAIInitiative::IsAttackWhenOnAttack(ObjID attacker)
{
	return !(m_me->GetPos() - m_birth_pos).IsLongerThan(m_aoi_range) && (INVALID_OBJ_ID == m_enemy);
}

