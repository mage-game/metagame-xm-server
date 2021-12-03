#include "effecthurtdec.hpp"

EffectDecHurt::EffectDecHurt() 
	: m_effect_time(0), m_left_time(0),m_buff_type(EBT_DEC_HURT), m_dec_to_role(0), m_dec_to_monster(0), m_dec_fix(0)
{

}

EffectDecHurt::EffectDecHurt(ObjID deliverer, UInt16 skill, int effect_time, char product_level, char product_method, char buff_type) 
	: EffectBase(deliverer, skill, product_method, product_level),
	m_effect_time(effect_time), m_left_time(effect_time),m_buff_type(buff_type), m_dec_to_role(0), m_dec_to_monster(0), m_dec_fix(0)
{
	m_merge_rule = EffectBase::MERGE_RULE_NOMERGE_2;
}

EffectDecHurt::~EffectDecHurt()
{

}

bool EffectDecHurt::Update(unsigned long interval, Character *cha, bool *die)
{
	m_left_time -= (int)interval;
	if (m_left_time > 0)
	{
		return true;
	}

	return false;
}

// 角色对自身减少 m_dec_to_role 的百分比伤害
// 怪物对自身减少 m_dec_to_monster 的百分比伤害
void EffectDecHurt::SetDecPercent(int percent)
{
	if (percent > 0)
	{
		m_dec_to_role = percent;
		m_dec_to_monster = percent;
	}
}

void EffectDecHurt::SetDecPercent(int percent_to_role, int percent_to_monster)
{
	if (percent_to_role > 0)
	{
		m_dec_to_role = percent_to_role;
	}

	if (percent_to_monster > 0)
	{
		m_dec_to_monster = percent_to_monster;
	}
}

int EffectDecHurt::GetDecPercent(Character *target)
{ 
	if (NULL != target && Obj::OBJ_TYPE_MONSTER == target->GetObjType())					  
	{
		return m_dec_to_monster;
	}
	else
	{
		return m_dec_to_role; 
	}
}

void EffectDecHurt::SetDecFix(int fix)
{
	m_dec_fix = fix;
}

int EffectDecHurt::GetDecFix(Character *target)
{
	int ret = 0;
	if (NULL != target)
	{
		ret = m_dec_fix;
	}

	return ret;
}

void EffectDecHurt::GetEffectParam(short *count, long long param[EFFECT_INFO_PARAM_MAX])
{
	UNSTD_STATIC_CHECK(EFFECT_INFO_PARAM_MAX >= 3);

	param[0] = m_effect_time;
	param[1] = m_left_time;
	param[2] = m_buff_type;
	param[3] = m_dec_to_role;
	param[4] = m_dec_to_monster;
	param[5] = m_dec_fix;

	*count = 6;
}

bool EffectDecHurt::Serialize(TLVSerializer &s) const
{
	return SerializeBase(s) && s.Push(m_effect_time) && s.Push(m_dec_to_role) &&
		s.Push(m_dec_to_monster) && s.Push(m_dec_fix) && s.Push(m_left_time) && s.Push(m_buff_type);
}

bool EffectDecHurt::Unserialize(TLVUnserializer &s)
{
	return UnserializeBase(s) && s.Pop(&m_effect_time) && s.Pop(&m_dec_to_role) &&
		s.Pop(&m_dec_to_monster) && s.Pop(&m_dec_fix) && s.Pop(&m_left_time) && s.Pop(&m_buff_type);
}