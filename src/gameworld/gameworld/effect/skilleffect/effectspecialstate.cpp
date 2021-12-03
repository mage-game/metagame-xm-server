#include "effectspecialstate.hpp"

#include "obj/character/character.h"

EffectSpecialState::EffectSpecialState() 
	: m_effect_time(0), m_buff_type(0), m_param(0)
{

}

EffectSpecialState::EffectSpecialState(ObjID deliverer, UInt16 skill, int effect_time, unsigned char buff_type, char product_level, char priduct_method)
	: EffectBase(deliverer, skill, priduct_method, product_level), m_effect_time(effect_time), m_buff_type(buff_type), m_param(0)
{
	m_merge_rule = EffectBase::MERGE_RULE_NOMERGE_1;
}

EffectSpecialState::~EffectSpecialState()
{

}

void EffectSpecialState::Effect(Character *cha, bool *die)
{
	switch (m_buff_type)
	{
	case EBT_XUANYUN:
	case EBT_FROZEN:
		{
			cha->ActionStop();
		}
		break;

	case EBT_DINGSHEN:
		{
			cha->MoveStop();
		}
		break;
	}
}

bool EffectSpecialState::Update(unsigned long interval, Character *cha, bool *die)
{
	m_effect_time -= (int)interval;

	return m_effect_time > 0;
}

bool EffectSpecialState::IsPositiveEffect() const
{
	switch (m_buff_type)
	{
	case EBT_XUANYUN:
	case EBT_DINGSHEN:
	case EBT_CHENMO:
	case EBT_BIANXING_FOOL:
	case EBT_FROZEN:
		return false;

	case EBT_MIANYI_XUANYUN:
	case EBT_MIANYI_DINGSHEN:
	case EBT_MIANYI_CHENMO:
	case EBT_MIANYI_CHIHUAN:
	case EBT_WUDI:
	case EBT_TERRITORYWAR_TOWER_WUDI:
	case EBT_DEC_CONTROL_BUFF_TIME:
		return true;
	}

	return true;
}

void EffectSpecialState::GetEffectParam(short *count, long long param[EFFECT_INFO_PARAM_MAX])
{
	UNSTD_STATIC_CHECK(EFFECT_INFO_PARAM_MAX >= 2);

	param[0] = m_effect_time;
	param[1] = m_buff_type;
	param[2] = m_param;

	*count = 3;
}

bool EffectSpecialState::Serialize(TLVSerializer &s) const
{
	return SerializeBase(s) && s.Push(m_effect_time) && s.Push(m_buff_type) && s.Push(m_param);
}

bool EffectSpecialState::Unserialize(TLVUnserializer &s)
{
	return UnserializeBase(s) && s.Pop(&m_effect_time) && s.Pop(&m_buff_type) && s.Pop(&m_param);
}

bool EffectSpecialState::IsDingshenEffect() const
{
	return EBT_DINGSHEN == m_buff_type;
}

bool EffectSpecialState::IsXuanyunEffect() const
{
	switch (m_buff_type)
	{
	case EBT_XUANYUN:
	case EBT_FROZEN:
		return true;
	}

	return false;
}

bool EffectSpecialState::IsChemoEffect() const
{
	return EBT_CHENMO == m_buff_type || EBT_BIANXING_FOOL == m_buff_type;
}

bool EffectSpecialState::IsChihuanEffect() const
{
	return EBT_CHIHUAN == m_buff_type;
}
