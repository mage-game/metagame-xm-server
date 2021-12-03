#include "effectotherbuff.hpp"

#include "obj/character/character.h"
#include "obj/character/role.h"

EffectOtherBuff::EffectOtherBuff() 
	: m_effect_time(0), m_interval_is_do_now(true), m_interval(0), m_interval_elapse_time(0), m_has_time_limit(false), m_param1(0), m_param2(0), m_param3(0), m_param4(0)
{
	m_has_time_limit = m_effect_time > 0;
}

EffectOtherBuff::EffectOtherBuff(ObjID deliverer, UInt16 product_id, int effect_time, char product_level, char product_method) 
	: EffectBase(deliverer, product_id, product_method, product_level), 
	m_effect_time(effect_time), m_interval_is_do_now(true), m_interval(0), m_interval_elapse_time(0), m_has_time_limit(false),
	m_param1(0), m_param2(0), m_param3(0), m_param4(0)
{

	m_has_time_limit = m_effect_time > 0;
	m_merge_rule = EffectBase::MERGE_RULE_NOMERGE_1;
}

EffectOtherBuff::~EffectOtherBuff()
{

}

void EffectOtherBuff::Effect(Character *cha, bool *die)
{
	if (PRODUCT_ID_INC_PUTON_LEVEL == m_product_id)
	{
		m_recalc_reason_on_add_or_del = CharIntAttrs::RECALC_REASON_EQUIPMENT;
	}
	
	else if (m_product_id > EffectBase::PRODUCT_ID_BASE_ATTR_BEGIN && m_product_id < EffectBase::PRODUCT_ID_BASE_ATTR_END)
	{
		m_recalc_reason_on_add_or_del = CharIntAttrs::RECALC_REASON_BUFF;
	}

	this->Attach(cha, true);
}

bool EffectOtherBuff::Update(unsigned long interval, Character *cha, bool *die)
{
	if (!m_has_time_limit)  //是否有时间限制，没有则一直存在，直到外部移除
	{
		return true;
	}

	m_effect_time -= (int)interval;
	if (m_effect_time > 0)
	{
		if (interval > 0)		//需要处理隔的情况
		{
			if (m_interval_is_do_now)
			{
				this->IntervalEffect(cha);
				m_interval_is_do_now = false;
			}

			m_interval_elapse_time += (int)interval;			//间隔事件处理
			if (m_interval_elapse_time >= m_interval)
			{
				this->IntervalEffect(cha);
				m_interval_elapse_time = 0;
			}
		}
		
		return true;
	}

	return false;
}

// 回血回蓝
void EffectOtherBuff::IntervalEffect(Character *cha)
{
	if (NULL == cha) return;

	if (PRODUCT_ID_HP_CONTINUE == m_product_id || PRODUCT_ID_HP_DAN == m_product_id )
	{
		Attribute hp = Attribute(m_param1);
		cha->ChangeHp(&hp, true);
	}
	else if (PRODUCT_ID_MP_CONTINUE == m_product_id)
	{
		Attribute mp = Attribute(m_param1);
		cha->ChangeMp(&mp, true);
	}
}

void EffectOtherBuff::ClearEffect(Character *cha)
{
	this->Attach(cha, false);
}

void EffectOtherBuff::Attach(Character *cha, bool is_attach)
{
	if(NULL != cha && Obj::OBJ_TYPE_ROLE == cha->GetObjType() &&
		m_recalc_reason_on_add_or_del > CharIntAttrs::RECALC_REASON_INVALID && m_recalc_reason_on_add_or_del < CharIntAttrs::RECALC_REASON_MAX)
	{
		Role *role = (Role *)cha;
		role->ReCalcAttr(m_recalc_reason_on_add_or_del);
	}
}

//计算基础属性
void EffectOtherBuff::ReCalcBaseAttr(CharIntAttrs &base_add)
{
	if (m_product_id > EffectBase::PRODUCT_ID_BASE_ATTR_BEGIN && m_product_id < EffectBase::PRODUCT_ID_BASE_ATTR_END)
	{
		if (CharIntAttrs::IsRoleBaseAttr(m_param1) && m_param2 != 0)
		{
//			base_add.m_attrs[m_param1] += m_param2;
			base_add.AddValue(m_param1, m_param2);
		}
	}
}

void EffectOtherBuff::GetEffectParam(short *count, long long param[EFFECT_INFO_PARAM_MAX])
{
	param[0] = m_effect_time; 
	param[1] = m_interval;
	param[2] = m_param1;
	param[3] = m_param2;
	param[4] = m_param3;
	param[5] = m_param4;
	
	*count = 6;
}

bool EffectOtherBuff::Serialize(TLVSerializer &s) const
{
	return SerializeBase(s) && s.Push(m_effect_time) &&
			s.Push(m_interval) && s.Push(m_interval_elapse_time) &&
			s.Push(m_param1) && s.Push(m_param2) && s.Push(m_param3) && s.Push(m_param4);
}

bool EffectOtherBuff::Unserialize(TLVUnserializer &s)
{
	bool succ = UnserializeBase(s) && s.Pop(&m_effect_time) &&
			s.Pop(&m_interval) && s.Pop(&m_interval_elapse_time) &&
			s.Pop(&m_param1) && s.Pop(&m_param2) && s.Pop(&m_param3) && s.Pop(&m_param4);

	m_has_time_limit = m_effect_time > 0;

	return succ;
}

void EffectOtherBuff::Merge(EffectBase *new_effect, Character *cha)
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

void EffectOtherBuff::ReEffect(Character *cha, bool *die)
{
	this->Attach(cha, true);
}

