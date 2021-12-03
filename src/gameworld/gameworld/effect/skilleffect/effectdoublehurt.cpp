#include "effectdoublehurt.hpp"


EffectDoubleHurt::EffectDoubleHurt()
	: m_effect_time(0), m_hurt_percent(0), m_fix_hurt(0)
{

}

EffectDoubleHurt::EffectDoubleHurt(ObjID deliverer, UInt16 skill, int effect_time, char product_level, char product_method)
	: EffectBase(deliverer, skill, product_method, product_level),
	m_effect_time(effect_time), m_hurt_percent(0), m_fix_hurt(0)
{
	m_merge_rule = EffectBase::MERGE_RULE_NOMERGE_4;
}

EffectDoubleHurt::~EffectDoubleHurt()
{

}


bool EffectDoubleHurt::Update(unsigned long interval, Character *cha, bool *die)
{
	m_effect_time -= (int)interval;

	return m_effect_time > 0;
}

void EffectDoubleHurt::GetEffectParam(short *count, long long param[EFFECT_INFO_PARAM_MAX])
{
	UNSTD_STATIC_CHECK(EFFECT_INFO_PARAM_MAX >= 3);

	param[0] = m_effect_time;
	param[1] = m_hurt_percent;
	param[2] = m_fix_hurt;

	*count = 3;
}

bool EffectDoubleHurt::Serialize(TLVSerializer &s) const
{
	return SerializeBase(s) && s.Push(m_effect_time) && s.Push(m_hurt_percent) && s.Push(m_fix_hurt);
}

bool EffectDoubleHurt::Unserialize(TLVUnserializer &s)
{
	return UnserializeBase(s) && s.Pop(&m_effect_time) && s.Pop(&m_hurt_percent) && s.Pop(&m_fix_hurt);
}
