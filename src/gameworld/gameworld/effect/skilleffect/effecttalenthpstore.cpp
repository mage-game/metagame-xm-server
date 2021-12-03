#include "effecttalenthpstore.hpp"

#include "obj/character/character.h"
#include "other/event/eventhandler.hpp"

EffectTalentHpStore::EffectTalentHpStore() 
	: m_effect_time(0), m_hp_restore(0), m_replace_percent(0)
{

}

EffectTalentHpStore::EffectTalentHpStore(ObjID deliverer, UInt16 skill, int effect_time, char product_level, char product_method) 
	: EffectBase(deliverer, skill, product_method, product_level), 
	m_effect_time(effect_time), m_hp_restore(0), m_replace_percent(0)
{
	m_merge_rule = EffectBase::MERGE_RULE_NOMERGE_2;
}

EffectTalentHpStore::~EffectTalentHpStore()
{

}

bool EffectTalentHpStore::Update(unsigned long interval, Character *cha, bool *die)
{
	m_effect_time -= (int)interval;
	if (m_effect_time > 0)
	{
		return true;
	}

	if (m_hp_restore > 0 && cha)
	{
		Attribute hp = static_cast<Attribute>(m_hp_restore * m_replace_percent);
		cha->ChangeHp(&hp);
	}

	return false;
}

void EffectTalentHpStore::SetReplacePercent(float replace_percent)
{
	m_replace_percent = (replace_percent >= 0 && replace_percent <= 100 ? replace_percent : 100);
}

bool EffectTalentHpStore::ReplaceHurt(long long *delta_hp)
{
	m_hp_restore -= *delta_hp;
	*delta_hp = 0;
	return false;
}

void EffectTalentHpStore::GetEffectParam(short *count, long long param[EFFECT_INFO_PARAM_MAX])
{
	UNSTD_STATIC_CHECK(EFFECT_INFO_PARAM_MAX >= 3);

	param[0] = m_effect_time;
	param[1] = m_hp_restore;
	param[2] = static_cast<long long>(m_replace_percent * 100);

	*count = 3;
}

bool EffectTalentHpStore::Serialize(TLVSerializer &s) const
{
	return SerializeBase(s) && s.Push(m_effect_time) && s.Push(m_hp_restore) && s.Push(m_replace_percent);
}

bool EffectTalentHpStore::Unserialize(TLVUnserializer &s)
{
	return UnserializeBase(s) && s.Pop(&m_effect_time) && s.Pop(&m_hp_restore) && s.Pop(&m_replace_percent);
}
