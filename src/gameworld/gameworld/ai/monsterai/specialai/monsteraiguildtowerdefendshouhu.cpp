#include "monsteraiguildtowerdefendshouhu.hpp"
#include "obj/character/monster.h"
#include "scene/scene.h"
#include "globalconfig/globalconfig.h"
#include "gamecommon.h"

MonsterAIGuildTowerDefendShouHu::MonsterAIGuildTowerDefendShouHu(Character *me)
	: MonsterAIBase(me, MonsterInitParam::AITYPE_SPECIALAI_GUILD_TOWERDEFEND_SHOUHU),
	m_aoi_handle(-1), m_aoi_range(15), m_aoi_setting_point(-1, -1), m_enemy_size(0)
{

}

MonsterAIGuildTowerDefendShouHu::~MonsterAIGuildTowerDefendShouHu()
{

}

void MonsterAIGuildTowerDefendShouHu::Init(const MonsterAIParam &mp)
{
	MonsterAIBase::Init(mp);
	m_aoi_range = mp.aoi_range;
}

void MonsterAIGuildTowerDefendShouHu::OnEnterScene()
{
	m_aoi_setting_point = m_me->GetPos();
	m_aoi_handle = m_me->GetScene()->GetZoneMatrix()->CreateCircleAOI(m_me->GetId(), m_birth_pos, m_aoi_range, m_aoi_range);
}

void MonsterAIGuildTowerDefendShouHu::OnLeaveScene()
{
	m_me->GetScene()->GetZoneMatrix()->DestroyAOI(m_aoi_handle);
	m_enemy = INVALID_OBJ_ID;
}

void MonsterAIGuildTowerDefendShouHu::OnAOIEnter(ObjID obj_id)
{
	if (obj_id == m_enemy) return;

	Obj *obj = m_me->GetScene()->GetObj(obj_id);
	if (NULL == obj) return;

	if (m_me->IsEnemy(obj))
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
			if(FindEnemy())
			{
				StartAttack(m_enemy, false);
			}
		}
	}
}

void MonsterAIGuildTowerDefendShouHu::OnAOILeave(ObjID obj_id)
{
	Obj *obj = m_me->GetScene()->GetObj(obj_id);
	if (NULL == obj) return;

	if (m_me->IsEnemy(obj))
	{
		if (m_enemy == obj_id)
		{
			m_me->StopAttack();
			m_enemy = INVALID_OBJ_ID;
		}

		for (unsigned int i = 0; i < m_enemy_size; ++i)
		{
			if (m_enemy_list[i] == obj_id)
			{
				m_enemy_list[i] = m_enemy_list[m_enemy_size - 1];
				--m_enemy_size;

				break;
			}
		}
	}
}

void MonsterAIGuildTowerDefendShouHu::AI(unsigned long interval)
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
		{
			if (m_enemy == INVALID_OBJ_ID && !FindEnemy())
			{
				if (m_enemy != INVALID_OBJ_ID)
				{
					m_enemy = INVALID_OBJ_ID;
				}
			}
			else
			{
				if (CanStartAttack())
				{
					StartAttack(m_enemy, true);
				}
			}
		}
		break;

	case Character::ACTION_PLAYING:
		break;
	case Character::ATTACK_DONE:
	case Character::ATTACK_COOLDOWN:
	case Character::ATTACK_SPECIAL_STATE:
		SetNextSkill(m_enemy);
		break;
	case Character::ATTACK_OUTOFRANG:
		break;
	}
}

bool MonsterAIGuildTowerDefendShouHu::IsEnemy(Obj *obj)
{
	if (Obj::OBJ_TYPE_MONSTER == obj->GetObjType())
	{
		Monster *monster = (Monster*)obj;
		return monster->IsAlive() && MonsterInitParam::AITYPE_SPECIALAI_GUILD_TOWERDEFEND_ATTACK == monster->GetAIType();
	}

	return false;
}

bool MonsterAIGuildTowerDefendShouHu::IsFriend(Obj *obj, bool check_alive)
{
	if (Obj::OBJ_TYPE_ROLE == obj->GetObjType())
	{
		Character *cha = (Character*)obj;
		return !check_alive || cha->IsAlive();
	}
	else if (Obj::OBJ_TYPE_MONSTER == obj->GetObjType())
	{
		Monster *monster = (Monster*)obj;
		return (!check_alive || monster->IsAlive()) && MonsterInitParam::AITYPE_SPECIALAI_GUILD_TOWERDEFEND_ATTACK != monster->GetAIType();
	}

	return false;
}

bool MonsterAIGuildTowerDefendShouHu::FindEnemy()
{
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

	m_enemy = INVALID_OBJ_ID;

	return false;
}

