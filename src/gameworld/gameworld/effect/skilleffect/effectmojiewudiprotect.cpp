#include "effectmojiewudiprotect.hpp"

#include "obj/character/character.h"

EffectMojieWudiProtect::EffectMojieWudiProtect()
	: m_effect_time(0)
{

}

EffectMojieWudiProtect::EffectMojieWudiProtect(ObjID deliverer, UInt16 skill, int effect_time, char product_level, char product_method)
	: EffectBase(deliverer, skill, product_method, product_level), 
	m_effect_time(effect_time)
{
	m_merge_rule = EffectBase::MERGE_RULE_NOMERGE_1;
}

EffectMojieWudiProtect::~EffectMojieWudiProtect()
{

}

bool EffectMojieWudiProtect::Update(unsigned long interval, Character *cha, bool *die)
{
	m_effect_time -= (int)interval;
	if (m_effect_time > 0)
	{
		return true;
	}

	return false;
}

void EffectMojieWudiProtect::GetEffectParam(short *count, long long param[EFFECT_INFO_PARAM_MAX])
{
	UNSTD_STATIC_CHECK(EFFECT_INFO_PARAM_MAX >= 1);

	param[0] = m_effect_time;

	*count = 1;
}

bool EffectMojieWudiProtect::Serialize(TLVSerializer &s) const
{
	return SerializeBase(s) && s.Push(m_effect_time);
}

bool EffectMojieWudiProtect::Unserialize(TLVUnserializer &s)
{
	return UnserializeBase(s) && s.Pop(&m_effect_time);
}
