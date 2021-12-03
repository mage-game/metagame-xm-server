#include "effectattrhurtrandinc.hpp"
#include "obj/character/character.h"

EffectAttrRandIncHurt::EffectAttrRandIncHurt()
	: m_left_time(0), m_buff_type(0), m_count(0), m_inc_hurt_prob(0), m_inc_hurt_percent(0)
{

}

EffectAttrRandIncHurt::EffectAttrRandIncHurt(ObjID deliverer, UInt16 skill, int effect_time, unsigned char buff_type, char product_level, char priduct_method)
	: EffectBase(deliverer, skill, priduct_method, product_level), m_left_time(effect_time), m_buff_type(buff_type),
	m_count(0), m_inc_hurt_prob(0), m_inc_hurt_percent(0)
{
	m_merge_rule = EffectBase::MERGE_RULE_NOMERGE_2;
}

EffectAttrRandIncHurt::~EffectAttrRandIncHurt()
{

}

void EffectAttrRandIncHurt::Effect(Character *cha, bool *die)
{
	this->Attach(cha, true);
}

bool EffectAttrRandIncHurt::Update(unsigned long interval, Character *cha, bool *die)
{
	m_left_time -= (int)interval;
	if (m_left_time > 0)
	{
		return true;
	}

	this->Attach(cha, false);

	return false;
}

void EffectAttrRandIncHurt::ClearEffect(Character *cha)
{
	this->Attach(cha, false);
}

void EffectAttrRandIncHurt::AddAttrEffect(char attr_type, int attr_value)
{
	if (CharIntAttrs::IsFightAttr(attr_type, false) && m_count < static_cast<int>(sizeof(m_pair_list) / sizeof(m_pair_list[0])))
	{
		m_pair_list[m_count].attr_type = attr_type;
		m_pair_list[m_count].attr_value = attr_value;

		++ m_count;
	}
}

void EffectAttrRandIncHurt::GetEffectParam(short *count, long long param[EFFECT_INFO_PARAM_MAX])
{
	UNSTD_STATIC_CHECK(EFFECT_INFO_PARAM_MAX >= 8);
	UNSTD_STATIC_CHECK(sizeof(m_pair_list) / sizeof(m_pair_list[0]) >= 2);

	param[0] = m_buff_type;
	param[1] = m_left_time;
	param[2] = m_pair_list[0].attr_type;
	param[3] = m_pair_list[0].attr_value;
	param[4] = m_pair_list[1].attr_type;
	param[5] = m_pair_list[1].attr_value;
	param[6] = m_inc_hurt_prob;
	param[7] = m_inc_hurt_percent;

	*count = 8;
}

bool EffectAttrRandIncHurt::Serialize(TLVSerializer &s) const
{
	UNSTD_STATIC_CHECK(sizeof(m_pair_list) / sizeof(m_pair_list[0]) >= 2);

	return SerializeBase(s) && s.Push(m_left_time) && s.Push(m_buff_type) && s.Push(m_count) &&
		s.Push(m_pair_list[0].attr_type) && s.Push(m_pair_list[0].attr_value) &&
		s.Push(m_pair_list[1].attr_type) && s.Push(m_pair_list[1].attr_value) &&
		s.Push(m_inc_hurt_prob) && s.Push(m_inc_hurt_percent);
}

bool EffectAttrRandIncHurt::Unserialize(TLVUnserializer &s)
{
	UNSTD_STATIC_CHECK(sizeof(m_pair_list) / sizeof(m_pair_list[0]) >= 2);

	return UnserializeBase(s) && s.Pop(&m_left_time) && s.Pop(&m_buff_type) && s.Pop(&m_count) &&
		s.Pop(&m_pair_list[0].attr_type) && s.Pop(&m_pair_list[0].attr_value) &&
		s.Pop(&m_pair_list[1].attr_type) && s.Pop(&m_pair_list[1].attr_value) &&
		s.Pop(&m_inc_hurt_prob) && s.Pop(&m_inc_hurt_percent);
}

int EffectAttrRandIncHurt::GetIncPercent()
{
	if (m_inc_hurt_prob >= 100)
	{
		return m_inc_hurt_percent;
	}

	if (m_inc_hurt_prob <= 0)
	{
		return 0;
	}

	int rand_val = RandomNum(100);
	if (rand_val < m_inc_hurt_prob)
	{
		return m_inc_hurt_percent;
	}

	return 0;
}

void EffectAttrRandIncHurt::Attach(Character *cha, bool is_attach)
{
	if (m_count > 0) 
	{
		int tmp_count = 0;
		CharIntAttrs::AttrPair tmp_pair_list[MAX_ATTR_COUNT];

		for (int i = 0; i < m_count && i < static_cast<int>(sizeof(tmp_pair_list) / sizeof(tmp_pair_list[0])); ++i)
		{
			tmp_pair_list[tmp_count].attr_type = m_pair_list[i].attr_type;
			tmp_pair_list[tmp_count].attr_value = (is_attach ? m_pair_list[i].attr_value : -m_pair_list[i].attr_value);

			++ tmp_count;
		}

		cha->ChangeCharAttrHelper(tmp_pair_list, tmp_count);
	}
}

void EffectAttrRandIncHurt::ReEffect(Character *cha, bool *die)
{
	this->Attach(cha, true);
}
