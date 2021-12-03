#include "effectcounter.hpp"

EffectCounter::EffectCounter()
	: m_effect_time(0), m_buff_type(EBT_INVALID), m_inc_hurt_percent(0)
{

}

EffectCounter::EffectCounter(ObjID deliverer, UInt16 skill, int effect_time, unsigned char buff_type, char product_level, char product_method)
	: EffectBase(deliverer, skill, product_method, product_level),
	m_effect_time(effect_time), m_buff_type(buff_type), m_inc_hurt_percent(0)
{
	m_merge_rule = EffectBase::MERGE_RULE_COUNTER;
}

EffectCounter::~EffectCounter()
{

}

bool EffectCounter::Update(unsigned long interval, Character *cha, bool *die)
{
	m_effect_time -= (int)interval;

	return m_effect_time > 0;
}

void EffectCounter::Merge(EffectBase *new_effect, Character *cha)
{
	if (NULL != new_effect && EffectBase::EFFECT_TYPE_COUNTER == new_effect->GetEffectType())
	{
		m_merge_layer += new_effect->GetMergeLayer();

		if (m_merge_layer > m_max_merge_layer)
		{
			m_merge_layer = m_max_merge_layer;
		}

		m_effect_time = new_effect->GetLeftTimeMS();
	}
}

void EffectCounter::GetEffectParam(short *count, long long param[EFFECT_INFO_PARAM_MAX])
{
	UNSTD_STATIC_CHECK(EFFECT_INFO_PARAM_MAX >= 3);

	param[0] = m_effect_time; 
	param[1] = m_buff_type;
	param[2] = m_inc_hurt_percent;
	
	*count = 3;
}

bool EffectCounter::Serialize(TLVSerializer &s) const
{
	return SerializeBase(s) && s.Push(m_effect_time) && s.Push(m_buff_type) && s.Push(m_inc_hurt_percent);
}

bool EffectCounter::Unserialize(TLVUnserializer &s)
{
	return UnserializeBase(s) && s.Pop(&m_effect_time) && s.Pop(&m_buff_type) && s.Pop(&m_inc_hurt_percent);
}

int EffectCounter::GetIncHurtPercent()
{
	return m_inc_hurt_percent * m_merge_layer;
}

