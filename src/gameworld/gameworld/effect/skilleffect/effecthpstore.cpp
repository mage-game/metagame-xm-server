#include "effecthpstore.hpp"

#include "obj/character/character.h"
#include "other/event/eventhandler.hpp"

EffectHpStore::EffectHpStore() 
	: m_effect_time(0), m_hp_restore(0), m_replace_percent(0)
{

}

EffectHpStore::EffectHpStore(ObjID deliverer, UInt16 skill, int effect_time, long long  hp_restore, char product_level, char product_method)
	: EffectBase(deliverer, skill, product_method, product_level), 
	m_effect_time(effect_time), m_hp_restore(hp_restore), m_replace_percent(0)
{
	m_merge_rule = EffectBase::MERGE_RULE_NOMERGE_2;
}

EffectHpStore::~EffectHpStore()
{

}

bool EffectHpStore::Update(unsigned long interval, Character *cha, bool *die)
{
	m_effect_time -= (int)interval;
	if (m_effect_time > 0 && m_hp_restore > 0)
	{
		return true;
	}

	return false;
}

void EffectHpStore::SetReplacePercent(float replace_percent)
{
	m_replace_percent = (replace_percent >= 0 && replace_percent <= 100 ? replace_percent : 100);
}

bool EffectHpStore::ReplaceHurt(Attribute *delta_hp)
{
	if (*delta_hp >= 0) return false;

	Attribute replace_hurt = static_cast<Attribute>(-*delta_hp * (m_replace_percent / 100.0f));

	if (m_hp_restore >= replace_hurt)
	{
		m_hp_restore -= replace_hurt;
		*delta_hp += replace_hurt;
		return (0 == *delta_hp);
	}

	*delta_hp += m_hp_restore;
	m_hp_restore = 0;

	return false;
}

void EffectHpStore::GetEffectParam(short *count, long long param[EFFECT_INFO_PARAM_MAX])
{
	UNSTD_STATIC_CHECK(EFFECT_INFO_PARAM_MAX >= 3);

	param[0] = m_effect_time;
	param[1] = m_hp_restore;
	param[2] = static_cast<int>(m_replace_percent * 100);

	*count = 3;
}

bool EffectHpStore::Serialize(TLVSerializer &s) const
{
	return SerializeBase(s) && s.Push(m_effect_time) && s.Push(m_hp_restore) && s.Push(m_replace_percent);
}

bool EffectHpStore::Unserialize(TLVUnserializer &s)
{
	return UnserializeBase(s) && s.Pop(&m_effect_time) && s.Pop(&m_hp_restore) && s.Pop(&m_replace_percent);
}
