#include "monsteraitowerdefendteam.hpp"
#include "obj/character/monster.h"
#include "scene/scene.h"
#include "globalconfig/globalconfig.h"
#include "gamecommon.h"
#include "config/logicconfigmanager.hpp"
#include "other/fb/towerdefendconfig.hpp"
#include "scene/map.h"

MonsterAITowerDefendTeam::MonsterAITowerDefendTeam(Character *me)
	: MonsterAIBase(me, MonsterInitParam::AITYPE_SPECIALAI_TOWERDEFEND_TEAM_ATTACK),
	m_birth_path(0), m_aoi_handle(-1), m_aoi_range(15), m_aoi_setting_point(-1, -1), m_enemy_size(0), m_walk_path_way(true),m_cur_walk_pos(0)
{

}

MonsterAITowerDefendTeam::~MonsterAITowerDefendTeam()
{

}

void MonsterAITowerDefendTeam::Init(const MonsterAIParam &mp)
{
	MonsterAIBase::Init(mp);
	m_aoi_range = mp.aoi_range;
}

void MonsterAITowerDefendTeam::OnEnterScene()
{
	m_aoi_setting_point = m_me->GetPos();
	m_aoi_handle = m_me->GetScene()->GetZoneMatrix()->CreateCircleAOI(m_me->GetId(), m_birth_pos, m_aoi_range, m_aoi_range);
}

void MonsterAITowerDefendTeam::OnLeaveScene()
{
	m_me->GetScene()->GetZoneMatrix()->DestroyAOI(m_aoi_handle);
	m_enemy = INVALID_OBJ_ID;
}

void MonsterAITowerDefendTeam::OnAOIEnter(ObjID obj_id)
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

void MonsterAITowerDefendTeam::OnAOILeave(ObjID obj_id)
{
	Obj *obj = m_me->GetScene()->GetObj(obj_id);
	if (NULL == obj) return;

	if (m_me->IsEnemy(obj))
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

void MonsterAITowerDefendTeam::AI(unsigned long interval)
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
		if (MoveAround())
		{
			break;
		}
		else if (m_enemy == INVALID_OBJ_ID && !FindEnemy())
		{
			if (m_enemy != INVALID_OBJ_ID)
			{
				m_enemy = INVALID_OBJ_ID;
			}

			GotoNextPosi();
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

		break;

	case Character::ACTION_PLAYING:
		break;
	case Character::ATTACK_DONE:
	case Character::ATTACK_COOLDOWN:
	case Character::ATTACK_SPECIAL_STATE:
		SetNextSkill(m_enemy);
		break;
	case Character::ATTACK_OUTOFRANG:
		Follow(m_enemy, interval);
		break;
	}
}

void MonsterAITowerDefendTeam::SetBirthPath(int birth_path)
{
	m_birth_path = birth_path;
	m_cur_walk_pos = 0;
	Scene * scene = m_me->GetScene();
	if (NULL != scene && Scene::SCENE_TYPE_TOWERDEFEND_TEST == scene->GetSceneType())
	{
		LOGIC_CONFIG->GetTowerDefendConfig().GetPathWayListTest(m_birth_path, m_walk_pos);
	}
	else
	{
		LOGIC_CONFIG->GetTowerDefendConfig().GetPathWayList(m_birth_path, m_walk_pos);
	}
}

bool MonsterAITowerDefendTeam::IsEnemy(Obj *obj)
{
	if (Obj::OBJ_TYPE_MONSTER == obj->GetObjType())
	{
		Monster *monster = (Monster*)obj;
		return (MonsterInitParam::AITYPE_SPECIALAI_TOWERDEFEND_TEAM_ATTACK != monster->GetAIType() && monster->IsAlive());
	}
	else if (Obj::OBJ_TYPE_ROLE == obj->GetObjType())
	{
		Role *role = (Role*)obj;
		return role->IsAlive();
	}

	return false;
}

bool MonsterAITowerDefendTeam::IsFriend(Obj *obj, bool check_alive)
{
	if (Obj::OBJ_TYPE_MONSTER == obj->GetObjType())
	{
		Monster *monster = (Monster*)obj;
		return (!check_alive || monster->IsAlive()) && (MonsterInitParam::AITYPE_SPECIALAI_TOWERDEFEND_TEAM_ATTACK == monster->GetAIType());
	}

	return false;
}

bool MonsterAITowerDefendTeam::FindEnemy()
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

bool MonsterAITowerDefendTeam::IsAttackWhenOnAttack(ObjID attacker)
{
	Obj *enemy_obj = m_me->GetScene()->GetObj(m_enemy);
	if (NULL != enemy_obj) 
	{
		Obj *attacker_obj = m_me->GetScene()->GetObj(attacker);
		if (NULL != attacker_obj)
		{
			if (Obj::OBJ_TYPE_MONSTER == enemy_obj->GetObjType() && Obj::OBJ_TYPE_ROLE == attacker_obj->GetObjType())
			{
				return true;
			}
		}
	}

	return MonsterAIBase::IsAttackWhenOnAttack(attacker);
}

void MonsterAITowerDefendTeam::MoveAOI()
{
	const int PETAI_AOI_SENSITIVE_DISTANCE = (int)GLOBALCONFIG->GetKeyConfig_PetAoiSensitiveDistance();	// PetAi中移动距离大于这个时MoveAOI
	if ((m_me->GetPos() - m_aoi_setting_point).IsLongerThan(PETAI_AOI_SENSITIVE_DISTANCE))
	{
		m_aoi_setting_point = m_me->GetPos();
		m_me->GetScene()->GetZoneMatrix()->MoveAOI(m_aoi_handle);
	}
}

void MonsterAITowerDefendTeam::GotoNextPosi()
{
	Scene * scene = m_me->GetScene();
	if (m_me->IsStatic() && NULL != scene)
	{
		Posi pos;

		if (Scene::SCENE_TYPE_TOWERDEFEND_TEST == scene->GetSceneType())
		{
			LOGIC_CONFIG->GetTowerDefendConfig().GetNextPosiTest(m_birth_path, m_me->GetPos(), &pos);
		}
		else
		{
			LOGIC_CONFIG->GetTowerDefendConfig().GetNextPosi(m_birth_path, m_me->GetPos(), &pos);
		}
		
		
		Posi tmp_pos;
		for (int i = 0; i < 20; i++)
		{
			tmp_pos = gamecommon::GetDisRandPos(pos, 3);
			if (m_me->GetScene()->GetMap()->IsValidatePos(Obj::OBJ_TYPE_ROLE, tmp_pos.x, tmp_pos.y))
			{
				pos = tmp_pos;
				break;
			}
		}

		{
			Posi deltaP = pos - m_me->GetPos();

			Scalar distance = 0;
			Dir dir = gamecommon::GetMoveDir(deltaP, &distance);

			m_me->SetDir(dir, distance);
		}
	}
}

bool MonsterAITowerDefendTeam::MoveAround()
{
	if (m_walk_pos.size() <= 0 || m_cur_walk_pos >= m_walk_pos.size()-2) return false;

	if (m_walk_pos[m_cur_walk_pos] == m_me->GetPos()) // 到达 停止行走
	{
		if (m_cur_walk_pos < m_walk_pos.size()-1)
		{
			m_cur_walk_pos++;
		}
	}

	Posi deltaP = m_walk_pos[m_cur_walk_pos] - m_me->GetPos();
	Scalar distance = 0;
	Dir dir = gamecommon::GetMoveDir(deltaP, &distance);
	m_me->SetDir(dir, distance);

	return true;
}
