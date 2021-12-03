#include "effectinterval.hpp"

#include "obj/character/character.h"
#include "obj/character/role.h"
#include "obj/obj.h"
#include "scene/scene.h"

EffectInterval::EffectInterval() 
	: m_interval_time(0), m_buff_type(0), m_delta_hp(0), m_interval(0), m_count(0), m_fight_type(FIGHT_TYPE_NORMAL)
{

}

EffectInterval::EffectInterval(ObjID deliverer, UInt16 skill, unsigned char buff_type, char product_level, char product_method)
	: EffectBase(deliverer, skill, product_method, product_level),
	m_interval_time(0), m_buff_type(buff_type), m_delta_hp(0), m_interval(0), m_count(0), m_deliver_roleid(0)
{
	m_merge_rule = EffectBase::MERGE_RULE_NOMERGE_4;
	m_fight_type = FIGHT_TYPE_NORMAL;
}

EffectInterval::~EffectInterval()
{

}

bool EffectInterval::Update(unsigned long interval, Character *cha, bool *die)
{
	m_interval_time += (int)interval;
	if (m_interval_time < m_interval)
	{
		return true;
	}
	
	// 当释放者在同场景中不存在，或者记录的角色不是之前的释放者。则去掉该buff。以免死亡时killer用了旧的objid。
	if (m_delta_hp < 0)
	{
		if(cha->GetScene())
		{
			Obj *obj = cha->GetScene()->GetObj(this->GetDeliverer());
			if (NULL == obj)
			{
				return false;
			}

			if (Obj::OBJ_TYPE_ROLE == obj->GetObjType())
			{
				Role *deliever_role = dynamic_cast<Role *>(obj);
				if (m_deliver_roleid > 0 && m_deliver_roleid != deliever_role->GetUID())
				{
					return false;
				}
			}
		}
	}

	m_interval_time = 0;

	-- m_count;

	Attribute real_delta_hp = m_merge_layer * m_delta_hp, view_delta_hp = m_merge_layer * m_delta_hp;
	EffectHP(&real_delta_hp, &view_delta_hp, cha, die, false, m_fight_type);

	SendAttackBlock(cha, real_delta_hp, view_delta_hp, (unsigned short)m_fight_type, 0, true);

	if (m_count > 0) return true;

	return false;
}

void EffectInterval::GetEffectParam(short *count, long long param[EFFECT_INFO_PARAM_MAX])
{
	UNSTD_STATIC_CHECK(EFFECT_INFO_PARAM_MAX >= 4);
	
	param[0] = this->GetLeftTimeMS();
	param[1] = m_delta_hp;
	param[2] = m_interval;
	param[3] = m_buff_type;

	*count = 4;
}

bool EffectInterval::Serialize(TLVSerializer &s) const
{
	return SerializeBase(s) && s.Push(m_interval_time) && s.Push(m_buff_type)  && s.Push(m_delta_hp) && s.Push(m_interval) && s.Push(m_count) && s.Push(m_fight_type);
}

bool EffectInterval::Unserialize(TLVUnserializer &s)
{
	return UnserializeBase(s) && s.Pop(&m_interval_time) && s.Pop(&m_buff_type) && s.Pop(&m_delta_hp) && s.Pop(&m_interval) && s.Pop(&m_count) && s.Pop(&m_fight_type);
}

void EffectInterval::Merge(EffectBase *new_effect, Character *cha)
{
	if (NULL != new_effect && EffectBase::EFFECT_TYPE_INTERVAL_HP == new_effect->GetEffectType() && this->GetBuffType() == new_effect->GetBuffType())
	{
		m_merge_layer += new_effect->GetMergeLayer();

		if (m_merge_layer > m_max_merge_layer)
		{
			m_merge_layer = m_max_merge_layer;
		}

		if (m_interval > 0)
		{
			m_count = new_effect->GetLeftTimeMS() / m_interval;
		}	
	}
}

void EffectInterval::SetParam(int interval, int count, Attribute delta_hp, int deliver_roleid)
{
	m_interval = interval;
	m_count = count;
	m_delta_hp = delta_hp;
	m_deliver_roleid = deliver_roleid;
}
