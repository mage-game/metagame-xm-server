#include "effectreboundhurt.hpp"

#include "obj/character/character.h"

EffectReboundHurt::EffectReboundHurt()
	: m_effect_time(0), m_percent(0)
{

}

EffectReboundHurt::EffectReboundHurt(ObjID deliverer, UInt16 skill, int effect_time, int percent, char product_level, char product_method)
	: EffectBase(deliverer, skill, product_method, product_level), 
	m_effect_time(effect_time), m_percent(percent)
{
	m_merge_rule = EffectBase::MERGE_RULE_NOMERGE_3;
}

EffectReboundHurt::~EffectReboundHurt()
{

}

bool EffectReboundHurt::Update(unsigned long interval, Character *cha, bool *die)
{
	m_effect_time -= (int)interval;
	if (m_effect_time > 0)
	{
		return true;
	}

	return false;
}

void EffectReboundHurt::GetEffectParam(short *count, long long param[EFFECT_INFO_PARAM_MAX])
{
	UNSTD_STATIC_CHECK(EFFECT_INFO_PARAM_MAX >= 2);

	param[0] = m_effect_time;
	param[1] = m_percent;

	*count = 2;
}

bool EffectReboundHurt::Serialize(TLVSerializer &s) const
{
	return SerializeBase(s) && s.Push(m_effect_time) && s.Push(m_percent);
}

bool EffectReboundHurt::Unserialize(TLVUnserializer &s)
{
	return UnserializeBase(s) && s.Pop(&m_effect_time) && s.Pop(&m_percent);
}

int EffectReboundHurt::CompareWith(EffectBase *effect)
{
	if (NULL == effect || EFFECT_TYPE_REBOUND_HURT != effect->GetEffectType()) return EffectBase::CMP_RET_NOTHING;

	EffectReboundHurt *effect_other = (EffectReboundHurt *)effect;
	if (effect_other->GetBuffType() != this->GetBuffType()) return EffectBase::CMP_RET_NOTHING;

	int val = abs(m_percent);
	int val_other = abs(effect_other->GetReboundPercent());

	if (val > val_other) return EffectBase::CMP_RET_BIGGER;
	if (val < val_other) return EffectBase::CMP_RET_SMALLER;
	if (this->GetLeftTimeMS() > effect_other->GetLeftTimeMS()) return EffectBase::CMP_RET_BIGGER;
	if (this->GetLeftTimeMS() < effect_other->GetLeftTimeMS()) return EffectBase::CMP_RET_SMALLER;

	return EffectBase::CMP_RET_EQUAL;
}
