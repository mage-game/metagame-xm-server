#include "effectbuff.hpp"

#include "obj/character/character.h"
#include <set>

EffectBuff::EffectBuff() 
	: m_effect_time(0), m_count(0), m_recover_hp(false), m_buff_type(EBT_INVALID)
{
	memset(m_percent_list, 0, sizeof(m_percent_list));
}

EffectBuff::EffectBuff(ObjID deliverer, UInt16 skill, int effect_time, char product_level, char product_method, unsigned char buff_type)
	: EffectBase(deliverer, skill, product_method, product_level),
	m_effect_time(effect_time), m_count(0), m_recover_hp(false),m_buff_type(buff_type)
{
	m_merge_rule = EffectBase::MERGE_RULE_MERGE;
	memset(m_percent_list, 0, sizeof(m_percent_list));
}

EffectBuff::~EffectBuff()
{

}

void EffectBuff::Effect(Character *cha, bool *die)
{
	this->Attach(cha, true);
}

bool EffectBuff::Update(unsigned long interval, Character *cha, bool *die)
{
	m_effect_time -= (int)interval;
	if (m_effect_time > 0)
	{
		return true;
	}

	this->Attach(cha, false);

	return false;
}

void EffectBuff::ClearEffect(Character *cha)
{
	this->Attach(cha, false);
}

unsigned char EffectBuff::GetBuffType() const
{
	if (m_buff_type != EBT_INVALID)
	{
		return m_buff_type;
	}

	// 为完成策划需求，此乃下策。如果以后这个类需要展示多个不同种类的加属性特效，再算
	for (int i = 0; i < m_count && i < MAX_ATTR_COUNT; ++i)
	{
		if (m_pair_list[i].attr_type == CharIntAttrs::FIGHT_CHARINTATTR_TYPE_FANGYU && 
			EffectBase::PRODUCT_METHOD_TALENT_SKILL != m_product_method)
		{
			if (m_pair_list[i].attr_value > 0)
			{
				return EBT_ADD_FANGYU;
			}
			return EBT_DEC_FANGYU;
		}
	}
	return m_buff_type;
}

bool EffectBuff::IsPositiveEffect() const
{
	for (int i = 0; i < m_count; ++i)
	{
		if (m_pair_list[i].attr_value < 0) return false;
	}

	return true;
}

void EffectBuff::AddAttrEffect(char attr_type, long long attr_value, int percent)
{
	if (CharIntAttrs::IsFightAttr(attr_type, false) && 0 != attr_value && m_count < static_cast<int>(sizeof(m_pair_list) / sizeof(m_pair_list[0])))
	{
		m_pair_list[m_count].attr_type = attr_type;
		m_pair_list[m_count].attr_value = attr_value;
		m_percent_list[m_count] = percent;

		++ m_count;
	}
}

long long EffectBuff::GetAttrValue(char attr_type)
{
	if (CharIntAttrs::IsFightAttr(attr_type, false))
	{
		for (int i = 0; i < m_count && i < MAX_ATTR_COUNT; ++ i)
		{
			if (attr_type == m_pair_list[i].attr_type)
			{
				return  m_pair_list[i].attr_value;
			}
		}
	}

	return 0;
}

void EffectBuff::Attach(Character *cha, bool is_attach)
{
	if (m_count > 0) 
	{
		int tmp_count = 0;
		CharIntAttrs::AttrPair tmp_pair_list[MAX_ATTR_COUNT];

		bool chg_maxhp = false;

		for (int i = 0; i < m_count && i < static_cast<int>(sizeof(tmp_pair_list) / sizeof(tmp_pair_list[0])); ++i)
		{
			tmp_pair_list[tmp_count].attr_type = m_pair_list[i].attr_type;
			tmp_pair_list[tmp_count].attr_value = (is_attach ? m_pair_list[i].attr_value : -m_pair_list[i].attr_value);
			tmp_pair_list[tmp_count].attr_value *= m_merge_layer;

			++ tmp_count;

			if (CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP == m_pair_list[i].attr_type) chg_maxhp = true;
		}

		cha->ChangeCharAttrHelper(tmp_pair_list, tmp_count);

		if (chg_maxhp && m_recover_hp)
		{
			Attribute maxhp = cha->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
			cha->ChangeHp(&maxhp, true);
		}
	}
}

void EffectBuff::GetEffectParam(short *count, long long param[EFFECT_INFO_PARAM_MAX])
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

bool EffectBuff::Serialize(TLVSerializer &s) const
{
	UNSTD_STATIC_CHECK(sizeof(m_pair_list) / sizeof(m_pair_list[0]) >= 3);
	UNSTD_STATIC_CHECK(sizeof(m_pair_list) / sizeof(m_pair_list[0]) == sizeof(m_percent_list) / sizeof(m_percent_list[0]));

	return SerializeBase(s) && s.Push(m_effect_time) && s.Push(m_count) &&
			s.Push(m_pair_list[0].attr_type) && s.Push(m_pair_list[0].attr_value) && s.Push(m_percent_list[0]) &&
			s.Push(m_pair_list[1].attr_type) && s.Push(m_pair_list[1].attr_value) && s.Push(m_percent_list[1]) &&
			s.Push(m_pair_list[2].attr_type) && s.Push(m_pair_list[2].attr_value) && s.Push(m_percent_list[2]);
}

bool EffectBuff::Unserialize(TLVUnserializer &s)
{
	return UnserializeBase(s) && s.Pop(&m_effect_time) && s.Pop(&m_count) &&
			s.Pop(&m_pair_list[0].attr_type) && s.Pop(&m_pair_list[0].attr_value) && s.Pop(&m_percent_list[0]) &&
			s.Pop(&m_pair_list[1].attr_type) && s.Pop(&m_pair_list[1].attr_value) && s.Pop(&m_percent_list[1]) &&
			s.Pop(&m_pair_list[2].attr_type) && s.Pop(&m_pair_list[2].attr_value) && s.Pop(&m_percent_list[2]);
}

void EffectBuff::Merge(EffectBase *new_effect, Character *cha)
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

void EffectBuff::AddGongJiEffect(Attribute gongji, short percent)
{
	if (0 != gongji && m_count < static_cast<int>(sizeof(m_pair_list) / sizeof(m_pair_list[0])))
	{
		m_pair_list[m_count].attr_type = CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI;
		m_pair_list[m_count].attr_value = gongji;
		m_percent_list[m_count] = percent;

		++ m_count;
	}
}

void EffectBuff::AddFangYuEffect(Attribute fangyu, short percent)
{
	if (0 != fangyu && m_count < static_cast<int>(sizeof(m_pair_list) / sizeof(m_pair_list[0])))
	{
		m_pair_list[m_count].attr_type = CharIntAttrs::FIGHT_CHARINTATTR_TYPE_FANGYU;
		m_pair_list[m_count].attr_value = fangyu;
		m_percent_list[m_count] = percent;

		++ m_count;
	}
}

void EffectBuff::AddMingZhongEffect(Attribute mingzhong, short percent)
{
	if (0 != mingzhong && m_count < static_cast<int>(sizeof(m_pair_list) / sizeof(m_pair_list[0])))
	{
		m_pair_list[m_count].attr_type = CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MINGZHONG;
		m_pair_list[m_count].attr_value = mingzhong;
		m_percent_list[m_count] = percent;

		++ m_count;
	}
}

void EffectBuff::AddShanBiEffect(Attribute shanbi, short percent)
{
	if (0 != shanbi && m_count < static_cast<int>(sizeof(m_pair_list) / sizeof(m_pair_list[0])))
	{
		m_pair_list[m_count].attr_type = CharIntAttrs::FIGHT_CHARINTATTR_TYPE_SHANBI;
		m_pair_list[m_count].attr_value = shanbi;
		m_percent_list[m_count] = percent;

		++ m_count;
	}
}

void EffectBuff::AddBaojiEffect(Attribute baoji, short percent)
{
	if (0 != baoji && m_count < static_cast<int>(sizeof(m_pair_list) / sizeof(m_pair_list[0])))
	{
		m_pair_list[m_count].attr_type = CharIntAttrs::FIGHT_CHARINTATTR_TYPE_BAOJI;
		m_pair_list[m_count].attr_value = baoji;
		m_percent_list[m_count] = percent;

		++ m_count;
	}
}

void EffectBuff::AddJianRenEffect(Attribute jianren, short percent)
{
	if (0 != jianren && m_count < static_cast<int>(sizeof(m_pair_list) / sizeof(m_pair_list[0])))
	{
		m_pair_list[m_count].attr_type = CharIntAttrs::FIGHT_CHARINTATTR_TYPE_JIANREN;
		m_pair_list[m_count].attr_value = jianren;
		m_percent_list[m_count] = percent;

		++ m_count;
	}
}

void EffectBuff::AddJingZhunPerEffect(Attribute jingzhun, short percent)
{
	if (0 != jingzhun && m_count < static_cast<int>(sizeof(m_pair_list) / sizeof(m_pair_list[0])))
	{
		m_pair_list[m_count].attr_type = CharIntAttrs::FIGHT_CHARINTATTR_TYPE_PER_JINGZHUN;
		m_pair_list[m_count].attr_value = jingzhun;
		m_percent_list[m_count] = percent;
		++ m_count;
	}
}

void EffectBuff::AddPerBaoJiHurtEffect(Attribute baoji, short percent)
{
	if (0 != baoji && m_count < static_cast<int>(sizeof(m_pair_list) / sizeof(m_pair_list[0])))
	{
		m_pair_list[m_count].attr_type = CharIntAttrs::FIGHT_CHARINTATTR_TYPE_PER_BAOJI_HURT;
		m_pair_list[m_count].attr_value = baoji;
		m_percent_list[m_count] = percent;
		++ m_count;
	}
}

void EffectBuff::AddMoveSpeedEffect(Attribute movespeed, short percent)
{
	if (0 != movespeed && m_count < static_cast<int>(sizeof(m_pair_list) / sizeof(m_pair_list[0])))
	{
		m_pair_list[m_count].attr_type = CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MOVE_SPEED;
		m_pair_list[m_count].attr_value = movespeed;
		m_percent_list[m_count] = percent;

		++ m_count;
	}
}

void EffectBuff::AddGeDangEffect(Attribute gedang, short percent)
{
	if (0 != gedang && m_count < static_cast<int>(sizeof(m_pair_list) / sizeof(m_pair_list[0])))
	{
		m_pair_list[m_count].attr_type = CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GEDANG_PER;
		m_pair_list[m_count].attr_value = gedang;
		m_percent_list[m_count] = percent;

		++m_count;
	}
}

void EffectBuff::Recalc(Character *chara)
{
	if (NULL != chara)
	{
		for (int i = 0; i < m_count; ++i)
		{
			if (CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MOVE_SPEED == m_pair_list[i].attr_type && 0 != m_percent_list[i])
			{
				long long now_chg = m_pair_list[i].attr_value;

				m_pair_list[i].attr_value = chara->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_MOVE_SPEED) * m_percent_list[i] / 100;

				long long chg_val = m_pair_list[i].attr_value - now_chg;

				chara->ChangeCharAttrHelper(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MOVE_SPEED, chg_val);

				break;
			}
		}
	}
}

void EffectBuff::ReEffect(Character *cha, bool *die)
{
	this->Attach(cha, true);
}
