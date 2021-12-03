#include "effectfire.hpp"
#include "skill/skilldef.hpp"
#include "obj/character/character.h"
#include "obj/character/role.h"
#include "scene/scene.h"
#include "skill/skillbase.hpp"
#include "scene/speciallogic/speciallogic.hpp"

EffectFire::EffectFire() 
	: m_interval_time(0), m_interval(0), m_count(0), m_delta_hp_self(0), m_delta_hp_other(0), m_rand_percent(0)
{
}

EffectFire::EffectFire(ObjID deliverer, UInt16 skill, char product_level, char product_method)
	: EffectBase(deliverer, skill, product_method, product_level),
	m_interval_time(0), m_interval(0), m_count(0), m_delta_hp_self(0), m_delta_hp_other(0)
{
	m_merge_rule = EffectBase::MERGE_RULE_NOMERGE_3;
}

EffectFire::~EffectFire()
{
}

bool EffectFire::Update(unsigned long interval, Character *cha, bool *die)
{
	m_interval_time += (int)interval;
	if (m_interval_time < m_interval)
	{
		return true;
	}

	m_interval_time = 0;

	if (m_count > 0)
	{
		--m_count;

		// ÉÕµÐÈË
		if (m_delta_hp_other > 0)
		{
			static ObjID target_list[DEFAULT_AOE_ENEMY_NUM] = {0};
			int target_count = SkillBase::GetTargetByArea(cha, cha->GetPos(), DEFAULT_AOE_ATTACK_RANGE, target_list, DEFAULT_AOE_ENEMY_NUM);
			for (int i = 0; i < target_count && i < DEFAULT_AOE_ENEMY_NUM; i++)
			{
				Character *target_obj = (Character *)cha->GetScene()->GetObj(target_list[i]);
				if (NULL != target_obj && Obj::IsCharactor(target_obj->GetObjType()))
				{
					long long delta_hp = m_delta_hp_other;
					if (m_rand_percent > 0)
					{
						delta_hp = (long long)(m_delta_hp_other * (1.0 + (RandomNum(m_rand_percent * 2 + 1) - m_rand_percent) / 100.0));
					}
					delta_hp = cha->GetScene()->GetSpecialLogic()->SpecialRecalcInjure(cha, target_obj, delta_hp);
					Attribute real_delta_hp = -delta_hp, view_delta_hp = -delta_hp;
					bool is_die = false;
					EffectHP(&real_delta_hp, &view_delta_hp, target_obj, &is_die, false);
					if (0 != real_delta_hp)
					{
						SendAttackBlock(target_obj, real_delta_hp, view_delta_hp, FIGHT_TYPE_NORMAL, 0, true);
						if (is_die)
						{
							target_obj->OnDie(m_deliverer);
						}
					}
				}
			}
		}

		// ÉÕ×Ô¼º
		if (m_delta_hp_self > 0)
		{
			long long delta_hp = m_delta_hp_self;
			if (m_rand_percent > 0)
			{
				delta_hp = (long long)(m_delta_hp_self * (1.0 + (RandomNum(m_rand_percent * 2 + 1) - m_rand_percent) / 100.0));
			}

			Attribute real_delta_hp = -delta_hp, view_delta_hp = -delta_hp;
			EffectHP(&real_delta_hp, &view_delta_hp, cha, die, false);
			if (0 != real_delta_hp)
			{
				SendAttackBlock(cha, real_delta_hp, view_delta_hp, FIGHT_TYPE_NORMAL, 0, true);
			}
		}
		
		return true;
	}

	return false;
}

void EffectFire::GetEffectParam(short *count, long long param[EFFECT_INFO_PARAM_MAX])
{
	UNSTD_STATIC_CHECK(EFFECT_INFO_PARAM_MAX >= 4);

	param[0] = this->GetLeftTimeMS();
	param[1] = m_interval;
	param[2] = m_delta_hp_self;
	param[3] = m_delta_hp_other;

	*count = 4;
}

bool EffectFire::Serialize(TLVSerializer &s) const
{
	return SerializeBase(s) && s.Push(m_interval_time) &&
		s.Push(m_interval) && s.Push(m_count) && s.Push(m_delta_hp_self) && s.Push(m_delta_hp_other);
}

bool EffectFire::Unserialize(TLVUnserializer &s)
{
	return UnserializeBase(s) && s.Pop(&m_interval_time) &&
		s.Pop(&m_interval) && s.Pop(&m_count) && s.Pop(&m_delta_hp_self) && s.Pop(&m_delta_hp_other);
}

bool EffectFire::CanEffect(Character *cha)
{
	return NULL != cha && cha->IsAlive();
}

void EffectFire::SetFireParam(int interval, int count, Attribute delta_hp_self, Attribute delta_hp_other)
{
	m_interval = interval;
	m_count = count;
	m_delta_hp_self = delta_hp_self;
	m_delta_hp_other = delta_hp_other;
}

void EffectFire::SetIntervalTime(int interval_time)
{
	m_interval_time = interval_time;
}
