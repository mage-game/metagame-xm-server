#include "monsteraiguildfbguard.hpp"

#include "obj/character/monster.h"
#include "scene/scene.h"
#include "globalconfig/globalconfig.h"
#include "gamecommon.h"
#include "config/logicconfigmanager.hpp"

MonsterAIGuildFbGuard::MonsterAIGuildFbGuard(Character *me)
	: MonsterAIBase(me, MonsterInitParam::AITYPE_GUILDFB_GUARD),
	m_aoi_handle(-1), m_aoi_range(15), m_aoi_setting_point(-1, -1), m_enemy_size(0), m_target_pos(0, 0)
{

}

MonsterAIGuildFbGuard::~MonsterAIGuildFbGuard()
{

}

void MonsterAIGuildFbGuard::Init(const MonsterAIParam &mp)
{
	MonsterAIBase::Init(mp);
	m_aoi_range = mp.aoi_range;
}

void MonsterAIGuildFbGuard::OnEnterScene()
{
	m_aoi_setting_point = m_me->GetPos();
	m_aoi_handle = m_me->GetScene()->GetZoneMatrix()->CreateCircleAOI(m_me->GetId(), m_birth_pos, m_aoi_range, m_aoi_range);
}

void MonsterAIGuildFbGuard::OnLeaveScene()
{
	m_me->GetScene()->GetZoneMatrix()->DestroyAOI(m_aoi_handle);
	m_enemy = INVALID_OBJ_ID;
}

void MonsterAIGuildFbGuard::OnAOIEnter(ObjID obj_id)
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

void MonsterAIGuildFbGuard::OnAOILeave(ObjID obj_id)
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

void MonsterAIGuildFbGuard::AI(unsigned long interval)
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
			if (m_enemy == INVALID_OBJ_ID && !FindEnemy(interval))
			{
				if (m_enemy != INVALID_OBJ_ID)
				{
					m_enemy = INVALID_OBJ_ID;
				}

				GotoTargetPosi();
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

			if (Character::MOVE_RUN == m_me->GetLogicResult()) MoveAOI();
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

bool MonsterAIGuildFbGuard::IsEnemy(Obj *obj)
{
	if (Obj::OBJ_TYPE_MONSTER == obj->GetObjType())
	{
		Monster *monster = (Monster *)obj;
		return monster->IsAlive() && MonsterInitParam::AITYPE_GUILDFB_ENEMY == monster->GetAIType();
	}
 
	return false;
}

bool MonsterAIGuildFbGuard::IsFriend(Obj *obj, bool check_alive)
{
	if (Obj::OBJ_TYPE_ROLE == obj->GetObjType())
	{
		return true;
	}

	return false;
}

bool MonsterAIGuildFbGuard::IsAttackWhenOnAttack(ObjID attacker)
{
	if (INVALID_OBJ_ID == m_enemy)
	{
		return true;
	}

	return false;
}

bool MonsterAIGuildFbGuard::FindEnemy(unsigned long interval)
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

	m_enemy_size = this->SearchNearEnemys(interval, m_birth_pos, m_aoi_range, m_enemy_list, MAX_ENEMY_NUM);

	if (m_enemy_size > 0)
	{
		m_enemy = m_enemy_list[RandomNum(m_enemy_size)];
		return true;
	}

	this->OnEnemyLost();

	return false;
}

void MonsterAIGuildFbGuard::MoveAOI()
{
	const int PETAI_AOI_SENSITIVE_DISTANCE = (int)GLOBALCONFIG->GetKeyConfig_PetAoiSensitiveDistance();	// PetAi中移动距离大于这个时MoveAOI
	if ((m_me->GetPos() - m_aoi_setting_point).IsLongerThan(PETAI_AOI_SENSITIVE_DISTANCE))
	{
		m_aoi_setting_point = m_me->GetPos();
		m_me->GetScene()->GetZoneMatrix()->MoveAOI(m_aoi_handle);
	}
}

void MonsterAIGuildFbGuard::GotoTargetPosi()
{
	if (m_target_pos.x > 0 && m_target_pos.y > 0)
	{
		Posi deltaP = m_target_pos - m_me->GetPos();

		if (m_me->IsStatic() && NULL != m_me->GetScene() && deltaP.IsLongerThan(1))
		{
			Scalar distance = 0;
			Dir dir = gamecommon::GetMoveDir(deltaP, &distance);

			m_me->SetDir(dir, distance);
		}
	}
}

