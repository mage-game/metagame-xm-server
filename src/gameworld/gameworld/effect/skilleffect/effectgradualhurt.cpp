#include "effectgradualhurt.hpp"

#include "obj/character/character.h"

EffectGradualHurt::EffectGradualHurt() 
	: m_interval_time(0), m_init_hurt(0), m_percent(0), m_fix(0), m_interval(0), m_count(0), m_past_count(0)
{

}

EffectGradualHurt::EffectGradualHurt(ObjID deliverer, UInt16 skill, char product_level, char product_method)
	: EffectBase(deliverer, skill, product_method, product_level),
	m_interval_time(0), m_init_hurt(0), m_percent(0), m_fix(0), m_interval(0), m_count(0), m_past_count(0)
{
	m_merge_rule = EffectBase::MERGE_RULE_NOMERGE_4;
}

EffectGradualHurt::~EffectGradualHurt()
{

}

bool EffectGradualHurt::Update(unsigned long interval, Character *cha, bool *die)
{
	m_interval_time += (int)interval;
	if (m_interval_time < m_interval)
	{
		return true;
	}

	m_interval_time = 0;

	++ m_past_count;

	long long injure = -static_cast<long long>(m_init_hurt * (1 + (m_past_count - 1) * (m_percent / 100.0f)) + (m_past_count - 1) * m_fix);

	-- m_count;

	Attribute real_delta_hp = injure, view_delta_hp = injure;
	EffectHP(&real_delta_hp, &view_delta_hp, cha, die, false);

	SendAttackBlock(cha, real_delta_hp, view_delta_hp, FIGHT_TYPE_NORMAL, 0, false);

	if (m_count > 0) return true;

	return false;
}

void EffectGradualHurt::GetEffectParam(short *count, long long param[EFFECT_INFO_PARAM_MAX])
{
	UNSTD_STATIC_CHECK(EFFECT_INFO_PARAM_MAX >= 5);

	param[0] = m_init_hurt;
	param[1] = m_interval;
	param[2] = m_count * m_interval;
	param[3] = m_percent;
	param[4] = m_fix;

	*count = 5;
}

bool EffectGradualHurt::Serialize(TLVSerializer &s) const
{
	return SerializeBase(s) && s.Push(m_interval_time) && s.Push(m_init_hurt) && s.Push(m_interval) && 
		s.Push(m_count) && s.Push(m_past_count) && s.Push(m_percent) && s.Push(m_fix);
}

bool EffectGradualHurt::Unserialize(TLVUnserializer &s)
{
	return UnserializeBase(s) && s.Pop(&m_interval_time) && s.Pop(&m_init_hurt) && s.Pop(&m_interval) &&
		s.Pop(&m_count) && s.Pop(&m_past_count) && s.Pop(&m_percent) && s.Pop(&m_fix);
}

void EffectGradualHurt::SetParam(int interval, int count, long long init_hurt, int percent, long long fix_hurt)
{
	m_interval = interval;
	m_count = count;
	m_init_hurt = init_hurt;
	m_percent = percent;
	m_fix = fix_hurt;
}
