#include "effectinvisible.hpp"

#include "obj/character/character.h"

EffectInvisible::EffectInvisible() : m_effect_time(0), m_buff_type(EBT_INVISIBLE)
{

}

EffectInvisible::EffectInvisible(ObjID deliverer, UInt16 skill, int effect_time, char product_level, char product_method)
	: EffectBase(deliverer, skill, product_method, product_level), 
	m_effect_time(effect_time), m_buff_type(EBT_INVISIBLE)
{
	m_merge_rule = EffectBase::MERGE_RULE_NOMERGE_1;
}

EffectInvisible::~EffectInvisible()
{

}

bool EffectInvisible::Update(unsigned long interval, Character *cha, bool *die)
{
	m_effect_time -= (int)interval;
	if (m_effect_time > 0)
	{
		return true;
	}

	return false;
}

void EffectInvisible::GetEffectParam(short *count, long long param[EFFECT_INFO_PARAM_MAX])
{
	UNSTD_STATIC_CHECK(EFFECT_INFO_PARAM_MAX >= 2);

	param[0] = m_effect_time;
	param[1] = m_buff_type;

	*count = 2;
}

bool EffectInvisible::Serialize(TLVSerializer &s) const
{
	return SerializeBase(s) && s.Push(m_effect_time) && s.Push(m_buff_type);
}

bool EffectInvisible::Unserialize(TLVUnserializer &s)
{
	return UnserializeBase(s) && s.Pop(&m_effect_time) && s.Pop(&m_buff_type);
}
