#include "effectmovespeed.hpp"

#include "obj/character/character.h"

EffectMoveSpeed::EffectMoveSpeed() 
	: m_effect_time(0), m_buff_type(0), m_movespeed(0), m_percent(0)
{

}

EffectMoveSpeed::EffectMoveSpeed(ObjID deliverer, UInt16 skill, int effect_time, unsigned char buff_type, char product_level, char product_method)
	: EffectBase(deliverer, skill, product_method, product_level), 
	m_effect_time(effect_time), m_buff_type(buff_type), m_movespeed(0), m_percent(0)
{
	m_merge_rule = EffectBase::MERGE_RULE_NOMERGE_3;
}

EffectMoveSpeed::~EffectMoveSpeed()
{

}

void EffectMoveSpeed::Effect(Character *cha, bool *die)
{
	switch (m_buff_type)
	{
	case EBT_CHIHUAN:
	case EBT_JIASU:
		{
			this->ChangeSpeed(cha, true);
		}
		break;
	}
}

bool EffectMoveSpeed::Update(unsigned long interval, Character *cha, bool *die)
{
	m_effect_time -= (int)interval;

	if (m_effect_time > 0)
	{
		return true;
	}

	if (EBT_CHIHUAN == m_buff_type || EBT_JIASU == m_buff_type)	
	{
		this->ChangeSpeed(cha, false);
	}

	return false;
}

void EffectMoveSpeed::ClearEffect(Character *cha)
{
	if (EBT_CHIHUAN == m_buff_type || EBT_JIASU == m_buff_type)	this->ChangeSpeed(cha, false);
}

bool EffectMoveSpeed::IsPositiveEffect() const
{
	switch (m_buff_type)
	{
	case EBT_CHIHUAN:
		return false;
	}

	return true;
}

void EffectMoveSpeed::ChangeSpeed(Character *cha, bool is_attach)
{
	cha->ChangeCharAttrHelper(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MOVE_SPEED, is_attach ? m_movespeed : -m_movespeed);
}

void EffectMoveSpeed::GetEffectParam(short *count, long long param[EFFECT_INFO_PARAM_MAX])
{
	UNSTD_STATIC_CHECK(EFFECT_INFO_PARAM_MAX >= 4);

	param[0] = m_effect_time;
	param[1] = m_buff_type;
	param[2] = m_movespeed;
	param[3] = m_percent;

	*count = 4;
}

bool EffectMoveSpeed::Serialize(TLVSerializer &s) const
{
	return SerializeBase(s) && s.Push(m_effect_time) && s.Push(m_buff_type) && s.Push(m_movespeed) && s.Push(m_percent);
}

bool EffectMoveSpeed::Unserialize(TLVUnserializer &s)
{
	return UnserializeBase(s) && s.Pop(&m_effect_time) && s.Pop(&m_buff_type) && s.Pop(&m_movespeed) && s.Pop(&m_percent);
}

int EffectMoveSpeed::CompareWith(EffectBase *effect)
{
	if (NULL == effect || EFFECT_TYPE_MOVESPEED != effect->GetEffectType()) return EffectBase::CMP_RET_NOTHING;

	EffectMoveSpeed *effect_other = (EffectMoveSpeed *)effect;
	if (effect_other->GetBuffType() != this->GetBuffType()) return EffectBase::CMP_RET_NOTHING;

	int val = abs(m_movespeed);
	int val_other = abs(effect_other->GetMoveSpeed());

	if (val > val_other) return EffectBase::CMP_RET_BIGGER;
	if (val < val_other) return EffectBase::CMP_RET_SMALLER;
	if (this->GetLeftTimeMS() > effect_other->GetLeftTimeMS()) return EffectBase::CMP_RET_BIGGER;
	if (this->GetLeftTimeMS() < effect_other->GetLeftTimeMS()) return EffectBase::CMP_RET_SMALLER;

	return EffectBase::CMP_RET_EQUAL;
}

void EffectMoveSpeed::Recalc(Character *chara)
{
	if (NULL != chara)
	{
		if (0 != m_percent)
		{
			int now_chg = m_movespeed;

			m_movespeed = static_cast<int>(chara->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_MOVE_SPEED) * m_percent / 100);

			int chg_val = m_movespeed - now_chg;

			chara->ChangeCharAttrHelper(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MOVE_SPEED, chg_val);
		}
	}
}

void EffectMoveSpeed::ReEffect(Character *cha, bool *die)
{
	switch (m_buff_type)
	{
	case EBT_CHIHUAN:
	case EBT_JIASU:
		{
			this->ChangeSpeed(cha, true);
		}
		break;
	}
}

bool EffectMoveSpeed::IsChihuanEffect() const
{
	return m_movespeed < 0;
}
