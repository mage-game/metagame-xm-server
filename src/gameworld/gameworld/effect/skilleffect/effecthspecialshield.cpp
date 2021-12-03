#include "effecthspecialshield.hpp"

#include "obj/character/character.h"
#include "other/event/eventhandler.hpp"
#include "protocal/msgfight.h"
#include "scene/scene.h"

EffectSpecialShield::EffectSpecialShield() 
	: m_effect_time(0), m_effective_times(0), m_fix_hurt(0), m_buff_type(EBT_HPSTORE)
{

}

EffectSpecialShield::EffectSpecialShield(ObjID deliverer, UInt16 skill, int effect_time, int hp_restore, char product_level,unsigned char buff_type, char product_method) 
	: EffectBase(deliverer, skill, product_method, product_level), 
	m_effect_time(effect_time), m_effective_times(hp_restore), m_max_effective_times(hp_restore), m_fix_hurt(0),m_buff_type(buff_type)
{
	m_merge_rule = EffectBase::MERGE_RULE_NOMERGE_2;
}

EffectSpecialShield::~EffectSpecialShield()
{

}

bool EffectSpecialShield::Update(unsigned long interval, Character *cha, bool *die)
{
	m_effect_time -= (int)interval;
	if (m_effect_time > 0 && m_effective_times > 0)
	{
		return true;
	}

	return false;
}

void EffectSpecialShield::SetFixHurt(long long fix_hurt)
{
	m_fix_hurt = fix_hurt < 0 ? fix_hurt : -1;  
}

bool EffectSpecialShield::ReplaceHurt(Attribute *delta_hp, Character *cha)
{
	if (*delta_hp >= 0) return false;

	if (m_effective_times > 0)
	{
		*delta_hp = m_fix_hurt;
		m_effective_times--;
		this->SendChangeEffecttimes(cha);
		return true;
	}

	this->SendChangeEffecttimes(cha);

	return false;
}

void EffectSpecialShield::SendChangeEffecttimes(Character *cha)
{
	Protocol::SCSpecialShieldChangeBlood hscb;

	hscb.obj_id = cha->GetId();
	hscb.real_hurt = m_fix_hurt;
	hscb.left_times = m_effective_times;
	hscb.max_times = m_max_effective_times;

	cha->GetScene()->GetZoneMatrix()->NotifyAreaMsg(cha->GetObserHandle(), &hscb, sizeof(hscb));

}

void EffectSpecialShield::GetEffectParam(short *count, long long param[EFFECT_INFO_PARAM_MAX])
{
	UNSTD_STATIC_CHECK(EFFECT_INFO_PARAM_MAX >= 3);

	param[0] = m_effect_time;
	param[1] = m_effective_times;
	param[2] = m_fix_hurt;

	*count = 3;
}

bool EffectSpecialShield::Serialize(TLVSerializer &s) const
{
	return SerializeBase(s) && s.Push(m_effect_time) && s.Push(m_effective_times) && s.Push(m_max_effective_times) && s.Push(m_fix_hurt) && s.Push(m_buff_type);
}

bool EffectSpecialShield::Unserialize(TLVUnserializer &s)
{
	return UnserializeBase(s) && s.Pop(&m_effect_time) && s.Pop(&m_effective_times) && s.Pop(&m_max_effective_times) && s.Pop(&m_fix_hurt) && s.Pop(&m_buff_type);
}
