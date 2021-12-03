#include "effectshenbing.hpp"

#include "obj/character/character.h"

EffectShenbing::EffectShenbing() 
: m_effect_time(0), m_maxhp(0), m_gongji(0), m_fangyu(0), m_mingzhong(0), m_shanbi(0), m_baoji(0), m_jianren(0)
{
}

EffectShenbing::EffectShenbing(ObjID deliverer, UInt16 skill, int effect_time, char product_level, char product_method) 
: EffectBase(deliverer, skill, product_method, product_level), 
m_effect_time(effect_time), m_maxhp(0), m_gongji(0), m_fangyu(0), m_mingzhong(0), m_shanbi(0), m_baoji(0), m_jianren(0)
{
	m_merge_rule = EffectBase::MERGE_RULE_NOMERGE_1;
}

EffectShenbing::~EffectShenbing()
{

}

void EffectShenbing::Effect(Character *cha, bool *die)
{
	this->Attach(cha, true);
}

bool EffectShenbing::Update(unsigned long interval, Character *cha, bool *die)
{
	m_effect_time -= interval;
	if (m_effect_time > 0)
	{
		return true;
	}

	this->Attach(cha, false);

	return false;
}

void EffectShenbing::ClearEffect(Character *cha)
{
	this->Attach(cha, false);
}

bool EffectShenbing::IsPositiveEffect() const
{
	return true;
}

void EffectShenbing::Attach(Character *cha, bool is_attach)
{
	int tmp_count = 0;
	CharIntAttrs::AttrPair tmp_pair_list[32];

	if (m_maxhp > 0)
	{
		tmp_pair_list[tmp_count].attr_type = CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP;
		tmp_pair_list[tmp_count].attr_value = (is_attach ? m_maxhp : -m_maxhp);
		++ tmp_count;
	}

	if (m_gongji > 0)
	{
		tmp_pair_list[tmp_count].attr_type = CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI;
		tmp_pair_list[tmp_count].attr_value = (is_attach ? m_gongji : -m_gongji);
		++ tmp_count;
	}

	if (m_fangyu > 0)
	{
		tmp_pair_list[tmp_count].attr_type = CharIntAttrs::FIGHT_CHARINTATTR_TYPE_FANGYU;
		tmp_pair_list[tmp_count].attr_value = (is_attach ? m_fangyu : -m_fangyu); 
		++ tmp_count;
	}

	if (m_mingzhong > 0)
	{
		tmp_pair_list[tmp_count].attr_type = CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MINGZHONG;
		tmp_pair_list[tmp_count].attr_value = (is_attach ? m_mingzhong : -m_mingzhong); 
		++ tmp_count;
	}

	if (m_shanbi > 0)
	{
		tmp_pair_list[tmp_count].attr_type = CharIntAttrs::FIGHT_CHARINTATTR_TYPE_SHANBI;
		tmp_pair_list[tmp_count].attr_value = (is_attach ? m_shanbi : -m_shanbi);  
		++ tmp_count;
	}

	if (m_baoji > 0)
	{
		tmp_pair_list[tmp_count].attr_type = CharIntAttrs::FIGHT_CHARINTATTR_TYPE_BAOJI;
		tmp_pair_list[tmp_count].attr_value = (is_attach ? m_baoji : -m_baoji); 
		++ tmp_count;
	}

	if (m_jianren > 0)
	{
		tmp_pair_list[tmp_count].attr_type = CharIntAttrs::FIGHT_CHARINTATTR_TYPE_JIANREN;
		tmp_pair_list[tmp_count].attr_value = (is_attach ? m_jianren : -m_jianren); 
		++ tmp_count;
	}

	cha->ChangeCharAttrHelper(tmp_pair_list, tmp_count);
}

void EffectShenbing::GetEffectParam(short *count, long long param[EFFECT_INFO_PARAM_MAX])
{
	UNSTD_STATIC_CHECK(EFFECT_INFO_PARAM_MAX >= 8);

	param[0] = m_effect_time; 
	param[1] = m_maxhp;
	param[2] = m_gongji;
	param[3] = m_fangyu;
	param[4] = m_mingzhong;
	param[5] = m_shanbi;
	param[6] = m_baoji;
	param[7] = m_jianren;

	*count = 8;
}

bool EffectShenbing::Serialize(TLVSerializer &s) const
{
	return SerializeBase(s) && s.Push(m_effect_time) &&
		s.Push(m_maxhp) && s.Push(m_gongji) && s.Push(m_fangyu) && s.Push(m_mingzhong) &&
		s.Push(m_shanbi) && s.Push(m_baoji) && s.Push(m_jianren);
}

bool EffectShenbing::Unserialize(TLVUnserializer &s)
{
	return UnserializeBase(s) && s.Pop(&m_effect_time) && 
		s.Pop(&m_maxhp) && s.Pop(&m_gongji) && s.Pop(&m_fangyu) && s.Pop(&m_mingzhong) &&
		s.Pop(&m_shanbi) && s.Pop(&m_baoji) && s.Pop(&m_jianren);
}

void EffectShenbing::Merge(EffectBase *new_effect, Character *cha)
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

void EffectShenbing::AddMaxHPEffect(Attribute maxhp)
{
	m_maxhp = maxhp;
}

void EffectShenbing::AddGongJiEffect(Attribute gongji)
{
	m_gongji = gongji;
}

void EffectShenbing::AddFangYuEffect(Attribute fangyu)
{
	m_fangyu = fangyu;
}

void EffectShenbing::AddMingZhongEffect(Attribute mingzhong)
{
	m_mingzhong = mingzhong;
}

void EffectShenbing::AddShanBiEffect(Attribute shanbi)
{
	m_shanbi = shanbi;
}

void EffectShenbing::AddBaojiEffect(Attribute baoji)
{
	m_baoji = baoji;
}

void EffectShenbing::AddJianRenEffect(Attribute jianren)
{
	m_jianren = jianren;
}

void EffectShenbing::ReEffect(Character *cha, bool *die)
{
	this->Attach(cha, true);
}
