#include "monsteraicrosspastureanimal.hpp"
#include "scene/scene.h"
#include "effect/skilleffect/effectmovespeed.hpp"
#include "obj/character/role.h"
#include "servercommon/serverconfig/crosspastureconfig.hpp"
#include "protocal/msgactivity.hpp"
#include "gameworld/engineadapter.h"
#include "servercommon/noticenum.h"
#include "servercommon/servercommon.h"

MonsterAICrossPastureAnimal::MonsterAICrossPastureAnimal(Character *me)
	: MonsterAIBase(me, MonsterInitParam::AITYPE_CROSS_PASTURE_ANIMAL), check_enemy_speed_time_interval(0)
{

}

MonsterAICrossPastureAnimal::~MonsterAICrossPastureAnimal()
{

}

void MonsterAICrossPastureAnimal::ForceSetEnemy(ObjID obj_id)
{
	if (obj_id != m_enemy && obj_id != m_me->GetId())
	{
		if (INVALID_OBJ_ID != obj_id)
		{
			Obj *old_enemy_obj = m_me->GetScene()->GetObj(m_enemy);
			if (NULL != old_enemy_obj && Obj::OBJ_TYPE_ROLE == old_enemy_obj->GetObjType())
			{
				Role *role = (Role*)old_enemy_obj;
				int reason = Protocol::CROSS_PSATURE_ANIMAL_NOTIC_TYPE_BE_ROBBED;

				GameName robber_name = { 0 };
				Obj *new_enemy_obj = m_me->GetScene()->GetObj(obj_id);
				if (NULL != new_enemy_obj && Obj::OBJ_TYPE_ROLE == new_enemy_obj->GetObjType())
				{
					new_enemy_obj->GetName(robber_name);
				}

				this->NoticEnemy(role, reason, robber_name);
			}
		}

		this->CheckAndRemoveEnemyBuff();
	}

	MonsterAIBase::ForceSetEnemy(obj_id);
	this->DecEnemySpeed(obj_id);

	Obj *new_enemy_obj = m_me->GetScene()->GetObj(obj_id);
	if (NULL != new_enemy_obj && Obj::OBJ_TYPE_ROLE == new_enemy_obj->GetObjType())
	{
		Role *role = (Role*)new_enemy_obj;

		int reason = Protocol::CROSS_PSATURE_ANIMAL_NOTIC_TYPE_FOLLOW;
		GameName robber_name = { 0 };
		this->NoticEnemy(role, reason, robber_name);
	}
}

void MonsterAICrossPastureAnimal::OnDie(ObjID killer)
{
	this->CheckAndRemoveEnemyBuff();
}

void MonsterAICrossPastureAnimal::OnRoleDie(ObjID obj_id)
{
	if (m_enemy == obj_id)
	{
		m_enemy = INVALID_OBJ_ID;
		m_me->StopAttack();
	}
}

void MonsterAICrossPastureAnimal::OnAttack(ObjID attacker, UInt16 skill_id, Attribute delta_hp)
{
	// MonsterAIBase::OnAttack(attacker, skill_id, delta_hp); 不走基类逻辑
	if (attacker == m_enemy) return;

	++ m_attacker_times_map[attacker];

	if (m_attacker_times_map[attacker] >= CROSS_PASTURE_ANIMAL_HATE_MAX_ATTACK_TIMES)
	{
		int old_enemy = m_enemy;
		if (INVALID_OBJ_ID != old_enemy)
		{
			Obj *old_enemy_obj = m_me->GetScene()->GetObj(m_enemy);
			if (NULL != old_enemy_obj && Obj::OBJ_TYPE_ROLE == old_enemy_obj->GetObjType())
			{
				Role *role = (Role*)old_enemy_obj;
				GameName robber_name = { 0 };
				int reason = Protocol::CROSS_PSATURE_ANIMAL_NOTIC_TYPE_BE_ROBBED;

				Obj *new_enemy_obj = m_me->GetScene()->GetObj(attacker);
				if (NULL != new_enemy_obj && Obj::OBJ_TYPE_ROLE == new_enemy_obj->GetObjType())
				{
					new_enemy_obj->GetName(robber_name);
				}

				this->NoticEnemy(role, reason, robber_name);
			}

			this->CheckAndRemoveEnemyBuff();						// 敌人变化，先将上一个敌人的buff移除			
		}

		this->DecEnemySpeed(attacker);

		m_enemy = attacker;
		m_attacker_times_map.clear();

		Obj *enemy_obj = m_me->GetScene()->GetObj(m_enemy);
		if (NULL != enemy_obj && Obj::OBJ_TYPE_ROLE == enemy_obj->GetObjType())
		{
			Role *role = (Role*)enemy_obj;
			GameName robber_name = { 0 };
			int reason = Protocol::CROSS_PSATURE_ANIMAL_NOTIC_TYPE_FOLLOW;

			this->NoticEnemy(role, reason, robber_name);
		}
	}
}

void MonsterAICrossPastureAnimal::AI(unsigned long interval)
{
	check_enemy_speed_time_interval += (int)interval;
	if (check_enemy_speed_time_interval >= CROSS_PASTURE_ANIMAL_CHECK_SPEED_TIME_S * 1000)
	{
		check_enemy_speed_time_interval = 0;

		if (INVALID_OBJ_ID != m_enemy)
		{
			this->DecEnemySpeed(m_enemy);
		}
	}

	switch (m_me->GetLogicResult())
	{
	case Character::MOVE_STATIC:
	case Character::LOGIC_NONE:
	case Character::MOVE_RUN:
		{
			if (INVALID_OBJ_ID != m_enemy)
			{
				if (this->IsOutRange())
				{
					m_enemy = INVALID_OBJ_ID;
					m_me->StopAttack();

					this->CheckAndRemoveEnemyBuff();
				}
				else
				{
					this->Follow(m_enemy, interval);
				}
// 				if (this->CanStartAttack())
// 				{
// 					this->StartAttack(m_enemy, true);
// 				}
// 				else
// 				{
// 					if (this->IsOutRange())
// 					{
// 						m_enemy = INVALID_OBJ_ID;
// 						m_me->StopAttack();
// 					}
// 					else
// 					{
// 						this->Follow(m_enemy, interval);
// 					}
// 				}
			}
			else
			{
				m_me->StopAttack();
			}
		}
		break;
	}
}

bool MonsterAICrossPastureAnimal::IsOutRange()
{
	if (INVALID_OBJ_ID == m_enemy)
	{
		return true;
	}
	
	Obj *obj = m_me->GetScene()->GetObj(m_enemy);
	if (NULL == obj || Obj::OBJ_TYPE_ROLE != obj->GetObjType())
	{
		return true;
	}

	bool outof_range = (obj->GetPos() - m_me->GetPos()).IsLongerThan(m_run_range);
	return outof_range;
}

void MonsterAICrossPastureAnimal::DecEnemySpeed(ObjID enemy_obj_id)
{
	if (INVALID_OBJ_ID == enemy_obj_id || enemy_obj_id == m_me->GetId()) return;

	Obj *enemy_obj = m_me->GetScene()->GetObj(enemy_obj_id);
	if (NULL != enemy_obj && Obj::OBJ_TYPE_ROLE == enemy_obj->GetObjType())
	{
		Role *role = (Role*)enemy_obj;

		int dec_percent = CrossPastureConfig::Instance().GetOtherCfg().dec_speed_percent;
		int chg_movespeed = (int)(role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_MOVE_SPEED) * (dec_percent / 100.0));

		int effct_time = CROSS_PASTURE_ANIMAL_DECROLE_SPEED_TIME_S * 1000;
		EffectMoveSpeed *effect_speed = new EffectMoveSpeed(m_me->GetId(), 1, effct_time, EBT_CHIHUAN, 1, EffectBase::PRODUCT_METHOD_CROSS_PASTURE_ANIMAL_DEC_SPEED);
		effect_speed->SetMoveSpeed(-chg_movespeed, dec_percent);
		effect_speed->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_1);
		effect_speed->SetSave(false);
		role->AddEffect(effect_speed);
	}
}

void MonsterAICrossPastureAnimal::CheckAndRemoveEnemyBuff()
{
	Obj *enemy_obj = m_me->GetScene()->GetObj(m_enemy);
	if (NULL != enemy_obj && Obj::OBJ_TYPE_ROLE == enemy_obj->GetObjType())
	{
		Role *role = (Role*)enemy_obj;

		role->GetEffectList()->FindAndRemoveEffect(EffectBase::EFFECT_TYPE_MOVESPEED, EBT_CHIHUAN, m_me->GetId(), EffectBase::PRODUCT_METHOD_CROSS_PASTURE_ANIMAL_DEC_SPEED);
	}
}

void MonsterAICrossPastureAnimal::NoticEnemy(Role *role, int reason, GameName robber)
{
	if (NULL == role) return;

	Protocol::SCCPPlayerHasAttachAnimalNotic msg;
	msg.notic_reason = reason;
	F_STRNCPY(msg.robber_name, robber, sizeof(GameName));
	
	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&msg, sizeof(msg));
}