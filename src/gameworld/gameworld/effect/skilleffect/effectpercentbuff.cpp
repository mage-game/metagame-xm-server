#include "effectpercentbuff.hpp"

#include "obj/character/character.h"
#include "obj/character/role.h"

EffectPercentBuff::EffectPercentBuff() 
	: m_effect_time(0), m_count(0)
{
	m_has_time_limit = m_effect_time > 0;
	memset(m_percent_list, 0, sizeof(m_percent_list));
}

EffectPercentBuff::EffectPercentBuff(ObjID deliverer, UInt16 skill, int effect_time, char product_level, char product_method) 
	: EffectBase(deliverer, skill, product_method, product_level), 
	m_effect_time(effect_time), m_count(0)
{
	m_has_time_limit = effect_time > 0;
	m_merge_rule = EffectBase::MERGE_RULE_MERGE;
	memset(m_percent_list, 0, sizeof(m_percent_list));
}

EffectPercentBuff::~EffectPercentBuff()
{

}

void EffectPercentBuff::Effect(Character *cha, bool *die)
{
	m_recalc_reason_on_add_or_del = CharIntAttrs::RECALC_REASON_BUFF;
	this->Attach(cha, true);
}

bool EffectPercentBuff::Update(unsigned long interval, Character *cha, bool *die)
{
	if (!m_has_time_limit)
	{
		return true;
	}

	m_effect_time -= (int)interval;
	if (m_effect_time > 0)
	{
		return true;
	}

	return false;
}

void EffectPercentBuff::ClearEffect(Character *cha)
{
	this->Attach(cha, false);
}

bool EffectPercentBuff::IsPositiveEffect() const
{
	for (int i = 0; i < m_count; ++i)
	{
		if (m_pair_list[i].attr_value < 0) return false;
	}

	return true;
}

void EffectPercentBuff::AddAttrEffect(char attr_type, short percent)
{
	if (CharIntAttrs::IsRoleBaseAttr(attr_type) && 0 != percent && m_count < static_cast<int>(sizeof(m_pair_list) / sizeof(m_pair_list[0])))
	{
		m_pair_list[m_count].attr_type = attr_type;
		m_pair_list[m_count].attr_value = percent;
		m_percent_list[m_count] = percent;

		++ m_count;
	}
}

void EffectPercentBuff::Attach(Character *cha, bool is_attach)
{
	if(NULL != cha && Obj::OBJ_TYPE_ROLE == cha->GetObjType() &&
		m_recalc_reason_on_add_or_del > CharIntAttrs::RECALC_REASON_INVALID && m_recalc_reason_on_add_or_del < CharIntAttrs::RECALC_REASON_MAX)
	{
		Role *role = (Role *)cha;
		role->ReCalcAttr(m_recalc_reason_on_add_or_del, false, false, CharIntAttrs::ATTR_NOTIFY_REASON_NORMAL, true);
	}
}

void EffectPercentBuff::GetEffectParam(short *count, long long param[EFFECT_INFO_PARAM_MAX])
{
	UNSTD_STATIC_CHECK(EFFECT_INFO_PARAM_MAX >= 10);
	UNSTD_STATIC_CHECK(sizeof(m_pair_list) / sizeof(m_pair_list[0]) >= 3);
	UNSTD_STATIC_CHECK(sizeof(m_pair_list) / sizeof(m_pair_list[0]) == sizeof(m_percent_list) / sizeof(m_percent_list[0]));

	param[0] = m_effect_time; 
	param[1] = m_pair_list[0].attr_type;
	param[2] = m_pair_list[0].attr_value;
	param[3] = m_percent_list[0];
	param[4] = m_pair_list[1].attr_type;
	param[5] = m_pair_list[1].attr_value;
	param[6] = m_percent_list[1];
	param[7] = m_pair_list[2].attr_type;
	param[8] = m_pair_list[2].attr_value;
	param[9] = m_percent_list[2];
	
	*count = 10;
}

bool EffectPercentBuff::Serialize(TLVSerializer &s) const
{
	UNSTD_STATIC_CHECK(sizeof(m_pair_list) / sizeof(m_pair_list[0]) >= 3);
	UNSTD_STATIC_CHECK(sizeof(m_pair_list) / sizeof(m_pair_list[0]) == sizeof(m_percent_list) / sizeof(m_percent_list[0]));

	return SerializeBase(s) && s.Push(m_effect_time) && s.Push(m_count) &&
			s.Push(m_pair_list[0].attr_type) && s.Push(m_pair_list[0].attr_value) && s.Push(m_percent_list[0]) &&
			s.Push(m_pair_list[1].attr_type) && s.Push(m_pair_list[1].attr_value) && s.Push(m_percent_list[1]) &&
			s.Push(m_pair_list[2].attr_type) && s.Push(m_pair_list[2].attr_value) && s.Push(m_percent_list[2]);
}

bool EffectPercentBuff::Unserialize(TLVUnserializer &s)
{
	bool succ = UnserializeBase(s) && s.Pop(&m_effect_time) && s.Pop(&m_count) &&
			s.Pop(&m_pair_list[0].attr_type) && s.Pop(&m_pair_list[0].attr_value) && s.Pop(&m_percent_list[0]) &&
			s.Pop(&m_pair_list[1].attr_type) && s.Pop(&m_pair_list[1].attr_value) && s.Pop(&m_percent_list[1]) &&
			s.Pop(&m_pair_list[2].attr_type) && s.Pop(&m_pair_list[2].attr_value) && s.Pop(&m_percent_list[2]);

	m_has_time_limit = m_effect_time > 0;

	return succ;
}

void EffectPercentBuff::Merge(EffectBase *new_effect, Character *cha)
{
	if (NULL != new_effect && NULL != cha && EffectBase::EFFECT_TYPE_ATTRBUFF == new_effect->GetEffectType())
	{
		if (m_merge_layer < m_max_merge_layer)
		{
			this->Attach(cha, false);								// 先取消叠加前的层数效果

			m_merge_layer += new_effect->GetMergeLayer();

			if (m_merge_layer > m_max_merge_layer)
			{
				m_merge_layer = m_max_merge_layer;
			}

			this->Attach(cha, true);								// 再附加叠加后的层数效果
		}

		m_effect_time = new_effect->GetLeftTimeMS();
	}
}

void EffectPercentBuff::ReCalcPercent(CharIntAttrs &base_add)
{
	for (int i = 0; i < m_count; ++i)
 	{
 		if (0 != m_percent_list[i])
 		{
 			int base_attr_type = m_pair_list[i].attr_type;
			if (CharIntAttrs::IsRoleBaseAttr(base_attr_type) && base_attr_type > CharIntAttrs::CHARINTATTR_TYPE_INVALID && base_attr_type < CharIntAttrs::CHARINTATTR_TYPE_MAX)
 			{
 				base_add.m_percent_attrs[base_attr_type] += m_percent_list[i] * 0.01f;
 			}
 		}
 	}
}

void EffectPercentBuff::ReEffect(Character *cha, bool *die)
{
	this->Attach(cha, true);
}
