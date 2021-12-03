#include "monsteraiguildfbenemy.hpp"

#include "obj/character/monster.h"
#include "scene/scene.h"
#include "globalconfig/globalconfig.h"
#include "gamecommon.h"
#include "config/logicconfigmanager.hpp"

MonsterAIGuildFbEnemy::MonsterAIGuildFbEnemy(Character *me)
	: MonsterAIMoveAround(me, MonsterInitParam::AITYPE_GUILDFB_ENEMY),
	m_guardboss_objid(INVALID_OBJ_ID), m_aoi_handle(-1), m_aoi_range(15), m_aoi_setting_point(0, 0), m_enemy_size(0)
{

}

MonsterAIGuildFbEnemy::~MonsterAIGuildFbEnemy()
{

}

void MonsterAIGuildFbEnemy::Init(const MonsterAIParam &mp)
{
	MonsterAIMoveAround::Init(mp);
	m_aoi_range = mp.aoi_range;
}

void MonsterAIGuildFbEnemy::OnEnterScene()
{
	m_aoi_setting_point = m_me->GetPos();
	m_aoi_handle = m_me->GetScene()->GetZoneMatrix()->CreateCircleAOI(m_me->GetId(), m_birth_pos, m_aoi_range, m_aoi_range);
}

void MonsterAIGuildFbEnemy::OnLeaveScene()
{
	m_me->GetScene()->GetZoneMatrix()->DestroyAOI(m_aoi_handle);
	m_enemy = INVALID_OBJ_ID;
}

void MonsterAIGuildFbEnemy::OnAOIEnter(ObjID obj_id)
{
	if (obj_id == m_enemy) return;

	Obj *obj = m_me->GetScene()->GetObj(obj_id);
	if (NULL == obj) return;

	if (m_me->IsEnemy(obj))
	{
		for (int i = 0; i < (int)m_enemy_size && i < (int)MAX_ENEMY_NUM; i++)
		{
			if (m_enemy_list[i] == obj_id) return;
		}

		if (m_enemy_size < MAX_ENEMY_NUM)
		{
			m_enemy_list[m_enemy_size] = obj_id;
			++ m_enemy_size;
		}

		if (m_enemy == INVALID_OBJ_ID) // ûĿ�� ͨ��FindEnemy Ѱ�� 
		{
			if(FindEnemy())
			{
				StartAttack(m_enemy, false);
			}
		}
		else // ��Ŀ�꣬��������������ǹ֣����л�  ��Ϊֻ��һֻ�֣�����ֱ�����ã���Ȼ���ñ��ⲻ�ϵ������л�����Ŀ������
		{
			if (Obj::OBJ_TYPE_MONSTER == obj->GetObjType())
			{
				m_enemy = obj_id;
				m_guardboss_objid = obj_id;

				if(FindEnemy())
				{
					StartAttack(m_enemy, false);
				}
			}
		}
	}
}

void MonsterAIGuildFbEnemy::OnAOILeave(ObjID obj_id)
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

		if (Obj::OBJ_TYPE_MONSTER == obj->GetObjType())
		{
			m_guardboss_objid = INVALID_OBJ_ID;
		}
	}
}

void MonsterAIGuildFbEnemy::AI(unsigned long interval)
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
			if ( (m_enemy == INVALID_OBJ_ID && !FindEnemy(interval)) || IsOutRange())
			{
				if (m_enemy != INVALID_OBJ_ID)
				{
					m_rest_time = 0;	// �������̻�ȥ ��Ȼ�����־ͻ��ڸô���Ϣ ����������ʱ���в��Ϲ�����
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

			if (Character::MOVE_RUN == m_me->GetLogicResult()) MoveAOI();
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

bool MonsterAIGuildFbEnemy::IsEnemy(Obj *obj)
{
	if (Obj::OBJ_TYPE_ROLE == obj->GetObjType()) // ��ֻҪ���ţ����ǵ���
	{
		Character *cha = (Role*)obj;
		return cha->IsAlive();
	}
	else if (Obj::OBJ_TYPE_MONSTER == obj->GetObjType()) // �ػ��� ���ǵ���
	{
		Monster *monster = (Monster *)obj;
		return monster->IsAlive() && MonsterInitParam::AITYPE_GUILDFB_GUARD == monster->GetAIType();
	}

	return false;
}

bool MonsterAIGuildFbEnemy::IsFriend(Obj *obj, bool check_alive)
{
	if (Obj::OBJ_TYPE_MONSTER == obj->GetObjType())
	{
		Monster *monster = (Monster*)obj;
		return (!check_alive || monster->IsAlive()) && MonsterInitParam::AITYPE_GUILDFB_ENEMY == monster->GetAIType();
	}

	return false;
}

bool MonsterAIGuildFbEnemy::FindEnemy(unsigned long interval)
{
	// �������ǿ�����ã���Ȼ����ĳ��������ƣ��ᵼ�¹�����ѡ������ΪĿ��
	// ��Ϊ �������ֻ�����˽�ɫ�ĳ��
	// if (this->CheckHateDrive()) return true;

	if (INVALID_OBJ_ID != m_guardboss_objid)
	{
		m_enemy = m_guardboss_objid;
		return true;
	}

	while (m_enemy_size != 0)
	{
		Obj *obj = m_me->GetScene()->GetObj(m_enemy_list[0]); // û��ɾ�����ڹ����ĵ���
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

void MonsterAIGuildFbEnemy::MoveAOI()
{
	const int PETAI_AOI_SENSITIVE_DISTANCE = (int)GLOBALCONFIG->GetKeyConfig_PetAoiSensitiveDistance();	// PetAi���ƶ�����������ʱMoveAOI
	if ((m_me->GetPos() - m_aoi_setting_point).IsLongerThan(PETAI_AOI_SENSITIVE_DISTANCE))
	{
		m_aoi_setting_point = m_me->GetPos();
		m_me->GetScene()->GetZoneMatrix()->MoveAOI(m_aoi_handle);
	}
}


bool MonsterAIGuildFbEnemy::IsAttackWhenOnAttack(ObjID attacker)
{
	if (INVALID_OBJ_ID == m_enemy && !(m_me->GetPos() - m_birth_pos).IsLongerThan(m_aoi_range))
	{
		return true;
	}

	return false;
}




