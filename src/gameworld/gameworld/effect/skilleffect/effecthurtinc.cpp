#include "effecthurtinc.hpp"

EffectIncHurt::EffectIncHurt() 
	: m_effect_time(0), m_inc_percent_to_role(0), m_inc_percent_to_monster(0)
{

}

EffectIncHurt::EffectIncHurt(ObjID deliverer, UInt16 skill, int effect_time, char product_level, char product_method) 
	: EffectBase(deliverer, skill, product_method, product_level),
	m_effect_time(effect_time), m_inc_percent_to_role(0), m_inc_percent_to_monster(0)
{
	m_merge_rule = EffectBase::MERGE_RULE_NOMERGE_2;
}

EffectIncHurt::~EffectIncHurt()
{

}

bool EffectIncHurt::Update(unsigned long interval, Character *cha, bool *die)
{
	m_effect_time -= (int)interval;

	return m_effect_time > 0;
}

void EffectIncHurt::SetIncPercent(int percent)
{
	if (percent > 0)
	{
		m_inc_percent_to_role = percent;
		m_inc_percent_to_monster = percent;
	}	
}

void EffectIncHurt::SetIncPercent(int percent_to_role, int percent_to_monster)
{
	if (percent_to_role > 0)
	{
		m_inc_percent_to_role = percent_to_role;
		m_inc_percent_to_monster = percent_to_monster;
	}
}

int EffectIncHurt::GetIncPercent(Character *target)
{
	if (NULL != target && Obj::OBJ_TYPE_MONSTER == target->GetObjType())
	{
		return m_inc_percent_to_monster;
	}
	else
	{
		return m_inc_percent_to_role; 
	}
}

void EffectIncHurt::GetEffectParam(short *count, long long param[EFFECT_INFO_PARAM_MAX])
{
	UNSTD_STATIC_CHECK(EFFECT_INFO_PARAM_MAX >= 3);

	param[0] = m_effect_time;
	param[1] = m_inc_percent_to_role;
	param[2] = m_inc_percent_to_monster;

	*count = 3;
}

bool EffectIncHurt::Serialize(TLVSerializer &s) const
{
	return SerializeBase(s) && s.Push(m_effect_time) && s.Push(m_inc_percent_to_role) && s.Push(m_inc_percent_to_monster);
}

bool EffectIncHurt::Unserialize(TLVUnserializer &s)
{
	return UnserializeBase(s) && s.Pop(&m_effect_time) && s.Pop(&m_inc_percent_to_role) && s.Pop(&m_inc_percent_to_monster);
}
