#include "effectattrstore.hpp"

#include "obj/character/character.h"
#include "obj/character/role.h"
#include "scene/scene.h"
#include "skill/skillbase.hpp"
#include "effect/skilleffect/effectonce.hpp"

EffectAttrStore::EffectAttrStore()
	: m_effect_time(0), m_has_time_limit(false), m_level(0), m_cur_attr(0), m_range(0), m_max_num(0), m_range_type(0), m_angle(0)
{
	//
}

EffectAttrStore::EffectAttrStore(ObjID deliverer, UInt16 product_id, int effect_time, char product_level, char product_method)
	:EffectBase(deliverer, product_id, product_method, product_level),
	m_effect_time(effect_time), m_has_time_limit(false), m_level(0), m_cur_attr(0), m_range(0), m_max_num(0), m_range_type(0), m_angle(0)
{
	m_has_time_limit = m_effect_time > 0;
	m_merge_rule = EffectBase::MERGE_RULE_NOMERGE_6;
}

EffectAttrStore::~EffectAttrStore()
{
	
}

bool EffectAttrStore::Update(unsigned long interval, Character *cha, bool *die)
{
	m_effect_time -= (int)interval;
	if (m_effect_time > 0)
	{
		return true;
	}

	// ·´µ¯ÉËº¦
	if (m_cur_attr > 0)
	{
		ObjID target_list[32] = {INVALID_OBJ_ID};
		Scene *scene = cha->GetScene();
		const int count = SkillBase::GetTargetByArea(cha, cha->GetPos(), m_range, target_list, m_max_num);

		for (int i = 0; i < count; ++i)
		{
			Character *tmp_target = (Character *)scene->GetObj(target_list[i]);
			if (NULL == tmp_target) continue;

			if (!cha->IsEnemy(tmp_target))
			{
				continue;
			}

			EffectOnce *effect = new EffectOnce(cha->GetId(), 0, -m_cur_attr, EffectBase::FIGHT_TYPE_NORMAL);
			tmp_target->AddEffect(effect);
		}
	}

	return false;
}

// ÀÛ¼Ó
void EffectAttrStore::AddUp(int attr)
{
	m_cur_attr += attr;
}

void EffectAttrStore::SetParams(int range, int max_num, int range_type, int angle)
{
	m_range = range;
	m_max_num = max_num;
	m_range_type = range_type;
	m_angle = angle;
}

void EffectAttrStore::GetEffectParam(short *count, long long param[EFFECT_INFO_PARAM_MAX])
{
	param[0] = m_effect_time;
	*count = 1;
}

bool EffectAttrStore::Serialize(TLVSerializer &s) const
{
	return SerializeBase(s) && s.Push(m_effect_time) &&
			s.Push(m_level) && s.Push(m_cur_attr) &&
			s.Push(m_range) && s.Push(m_max_num) &&
			s.Push(m_range_type) && s.Push((m_angle));
}

bool EffectAttrStore::Unserialize(TLVUnserializer &s)
{
	bool succ = UnserializeBase(s) && s.Pop(&m_effect_time) &&
		s.Pop(&m_level) && s.Pop(&m_cur_attr) && 
		s.Pop(&m_range) && s.Pop(&m_max_num) &&
		s.Pop(&m_range_type) && s.Pop(&m_angle);

	m_has_time_limit = m_effect_time > 0;

	return succ;
}