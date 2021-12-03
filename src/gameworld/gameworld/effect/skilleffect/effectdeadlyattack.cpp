#include "effectdeadlyattack.hpp"
#include "obj/character/character.h"
#include "scene/scene.h"
#include "obj/character/role.h"

bool EffectDeadlyAttack::Update(unsigned long interval, Character *cha, bool *die)
{
	m_effect_time -= (int)interval;

	return m_effect_time > 0;
}

int EffectDeadlyAttack::GetFixHurtPer(Character *target)
{
	int ret = 0;
	if (NULL != target)
	{
		ret = m_hurt_fix_hp_per;
	}

	return ret;
}

void EffectDeadlyAttack::GetEffectParam(short *count, long long param[EFFECT_INFO_PARAM_MAX])
{
	UNSTD_STATIC_CHECK(EFFECT_INFO_PARAM_MAX >= 2);

	param[0] = m_effect_time;
	param[1] = m_hurt_fix_hp_per;

	*count = 2;
}

bool EffectDeadlyAttack::Serialize(TLVSerializer &s) const
{
	return SerializeBase(s) && s.Push(m_effect_time) && s.Push(m_hurt_fix_hp_per) && s.Push(m_buff_type);
}

bool EffectDeadlyAttack::Unserialize(TLVUnserializer &s)
{
	return UnserializeBase(s) && s.Pop(&m_effect_time) && s.Pop(&m_hurt_fix_hp_per) && s.Pop(&m_buff_type);
}

