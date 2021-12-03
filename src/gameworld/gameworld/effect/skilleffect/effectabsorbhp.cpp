#include "effectabsorbhp.hpp"

#include "obj/character/character.h"

EffectAbsorbHP::EffectAbsorbHP()
: m_effect_time(0), m_percent(0), m_buff_type(EBT_INVALID)
{

}

EffectAbsorbHP::EffectAbsorbHP(ObjID deliverer, UInt16 skill, int effect_time, int percent, char product_level, char product_method, unsigned char buff_type)
: EffectBase(deliverer, skill, product_method, product_level), 
m_effect_time(effect_time), m_percent(percent), m_buff_type(buff_type)
{
	m_merge_rule = EffectBase::MERGE_RULE_NOMERGE_1;
}

EffectAbsorbHP::~EffectAbsorbHP()
{

}

bool EffectAbsorbHP::Update(unsigned long interval, Character *cha, bool *die)
{
	m_effect_time -= (int)interval;
	if (m_effect_time > 0)
	{
		return true;
	}

	return false;
}

unsigned char EffectAbsorbHP::GetBuffType() const
{
	return m_buff_type;
}

void EffectAbsorbHP::GetEffectParam(short *count, long long param[EFFECT_INFO_PARAM_MAX])
{
	UNSTD_STATIC_CHECK(EFFECT_INFO_PARAM_MAX >= 2);

	param[0] = m_effect_time;
	param[1] = m_percent;

	*count = 2;
}

bool EffectAbsorbHP::Serialize(TLVSerializer &s) const
{
	return SerializeBase(s) && s.Push(m_effect_time) && s.Push(m_percent);
}

bool EffectAbsorbHP::Unserialize(TLVUnserializer &s)
{
	return UnserializeBase(s) && s.Pop(&m_effect_time) && s.Pop(&m_percent);
}
