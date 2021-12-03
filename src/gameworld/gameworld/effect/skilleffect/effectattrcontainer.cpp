#include "effectattrcontainer.hpp"

#include "obj/character/character.h"
#include "obj/character/role.h"
#include "other/rolestatus/rolestatusmanager.hpp"

EffectAttrContainer::EffectAttrContainer()
	: m_effect_time(0), m_interval(0), m_elapse_time(0), m_do_now(true), m_has_time_limit(false), m_used_amount(0), m_level(0), m_total_attr(0), m_fight_attr(0), m_idle_attr(0)
{
	//
}

EffectAttrContainer::EffectAttrContainer(ObjID deliverer, UInt16 product_id, int effect_time, char product_level, char product_method)
	: EffectBase(deliverer, product_id, product_method, product_level),
	m_effect_time(effect_time), m_interval(0), m_elapse_time(0), m_do_now(true), m_has_time_limit(false), m_used_amount(0), m_level(0), m_total_attr(0), m_fight_attr(0), m_idle_attr(0)
{
	m_has_time_limit = m_effect_time > 0;
	m_merge_rule = EffectBase::MERGE_RULE_NOMERGE_5;
}

EffectAttrContainer::~EffectAttrContainer()
{
}

bool EffectAttrContainer::Update(unsigned long interval, Character *cha, bool *die)
{
	if (!this->HasUsedUp()) // 还没用完
	{
		if (interval > 0)
		{
			if (m_do_now) // 开始时执行一次
			{
				this->DoEffect(cha);
				m_do_now = false;
			}

			m_elapse_time += (int)interval;
			if (m_elapse_time >= m_interval)
			{
				this->DoEffect(cha);
				m_elapse_time = 0;
			}
		}

		return true;
	}
	m_used_amount = 0;

	return false;
}

void EffectAttrContainer::DoEffect(Character *cha)
{
	if (NULL == cha) return;

	Attribute attr = 0;
	// 战斗中
	if (dynamic_cast<Role *>(cha)->GetRoleStatusManager()->IsInFightStatus())
	{
		attr = Attribute(m_fight_attr);
	}
	else
	{
		attr = Attribute(m_idle_attr);
	}

	if (PRODUCT_ID_HP_CONTINUE == m_product_id)
	{
		if (cha->IsFullHp()) return;
		cha->ChangeHp(&attr, true);
	}
	else if (PRODUCT_ID_MP_CONTINUE == m_product_id)
	{
		if (cha->IsFullMp()) return;
		cha->ChangeMp(&attr, true);
	}

	m_used_amount += (unsigned short)attr;
}

bool EffectAttrContainer::HasUsedUp() const
{
	return (m_total_attr - m_used_amount) <= 0;
}

void EffectAttrContainer::Merge(EffectBase *new_effect, Character *cha)
{
	m_total_attr += dynamic_cast<EffectAttrContainer *>(new_effect)->GetTotalAttr(); // 总数量累计
}

void EffectAttrContainer::GetEffectParam(short *count, long long param[EFFECT_INFO_PARAM_MAX])
{
	param[0] = (m_total_attr - m_used_amount)/m_fight_attr;
	param[1] = m_fight_attr;
	param[2] = m_idle_attr;
	param[3] = m_total_attr - m_used_amount;

	*count = 4;
}

// void EffectAttrContainer::MergeParam(EffectBase *effect)
// {
// 	m_total_attr += dynamic_cast<EffectAttrContainer *>(effect)->GetTotalAttr();  // 总数量累计
// }

bool EffectAttrContainer::Serialize(TLVSerializer &s) const
{
	return SerializeBase(s) && s.Push(m_effect_time) &&
			s.Push(m_interval) && s.Push(m_elapse_time) && s.Push(m_used_amount) &&
			s.Push(m_level) && s.Push(m_total_attr) && s.Push(m_fight_attr) && s.Push(m_idle_attr);
}

bool EffectAttrContainer::Unserialize(TLVUnserializer &s)
{
	bool succ = UnserializeBase(s) && s.Pop(&m_effect_time) &&
			s.Pop(&m_interval) && s.Pop(&m_elapse_time) && s.Pop(&m_used_amount) &&
			s.Pop(&m_level) && s.Pop(&m_total_attr) && s.Pop(&m_fight_attr) && s.Pop(&m_idle_attr);

	m_has_time_limit = m_effect_time > 0;

	return succ;
}