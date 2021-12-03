#include "effectmodifyglobalcd.hpp"

#include "obj/character/character.h"

EffectModifyGlobalCD::EffectModifyGlobalCD()
	: m_effect_time(0), m_percent(0)
{

}

EffectModifyGlobalCD::EffectModifyGlobalCD(ObjID deliverer, UInt16 skill, int effect_time, int percent, char product_level, char product_method)
	: EffectBase(deliverer, skill, product_method, product_level), 
	m_effect_time(effect_time), m_percent(percent)
{
	m_merge_rule = EffectBase::MERGE_RULE_NOMERGE_2;
}

EffectModifyGlobalCD::~EffectModifyGlobalCD()
{

}

bool EffectModifyGlobalCD::Update(unsigned long interval, Character *cha, bool *die)
{
	m_effect_time -= (int)interval;
	if (m_effect_time > 0)
	{
		return true;
	}

	return false;
}

void EffectModifyGlobalCD::GetEffectParam(short *count, long long param[EFFECT_INFO_PARAM_MAX])
{
	UNSTD_STATIC_CHECK(EFFECT_INFO_PARAM_MAX >= 2);

	param[0] = m_effect_time;
	param[1] = m_percent;

	*count = 2;
}

void EffectModifyGlobalCD::ModifyGlobalCD(long long *global_cd)
{
	if (NULL == global_cd) return;

	long long chg_val = static_cast<long long>(*global_cd * (m_percent / 100.0f));
	*global_cd += chg_val;
}

bool EffectModifyGlobalCD::Serialize(TLVSerializer &s) const
{
	return SerializeBase(s) && s.Push(m_effect_time) && s.Push(m_percent);
}

bool EffectModifyGlobalCD::Unserialize(TLVUnserializer &s)
{
	return UnserializeBase(s) && s.Pop(&m_effect_time) && s.Pop(&m_percent);
}
