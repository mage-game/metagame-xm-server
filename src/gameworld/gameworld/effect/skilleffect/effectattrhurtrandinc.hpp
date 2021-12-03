#ifndef _EFFECT_ATTR_HURTRANDINC_HPP_
#define _EFFECT_ATTR_HURTRANDINC_HPP_

#include "effect/effectbase.hpp"

#include "obj/character/attribute.hpp"

class Character;

// 加属性并随机伤害加深
class EffectAttrRandIncHurt : public EffectBase
{
public:
	EffectAttrRandIncHurt();
	EffectAttrRandIncHurt(ObjID deliverer, UInt16 skill, int effect_time, unsigned char buff_type, char product_level, char priduct_method = EffectBase::PRODUCT_METHOD_SKILL);
	virtual ~EffectAttrRandIncHurt();

	virtual char GetEffectType() { return EFFECT_TYPE_ATTR_RAND_INC_HURT; }
	virtual void Effect(Character *cha, bool *die);
	virtual void ReEffect(Character *cha, bool *die);
	virtual bool Update(unsigned long interval, Character *cha, bool *die);
	virtual void ClearEffect(Character *cha);

	virtual bool IsPositiveEffect() const { return true; }

	virtual int GetLeftTimeMS() { return m_left_time; }

	void AddAttrEffect(char attr_type, int attr_value);
	void SetIncHurtParam(int prob, int percent) { m_inc_hurt_prob = prob; m_inc_hurt_percent = percent; }

	int GetIncPercent();

	virtual unsigned char GetBuffType() const { return m_buff_type; }
	virtual void GetEffectParam(short *count, long long param[EFFECT_INFO_PARAM_MAX]);

	void *operator new(size_t c);
	void operator delete(void *m);

	virtual bool Serialize(TLVSerializer &s) const;
	virtual bool Unserialize(TLVUnserializer &s);

protected:
	void Attach(Character *cha, bool is_attach);

	int	m_left_time;
	unsigned char m_buff_type;

	static const int MAX_ATTR_COUNT = 2;

	int m_count;													// 作用属性数量
	CharIntAttrs::AttrPair m_pair_list[MAX_ATTR_COUNT];				// 属性数值列表

	int m_inc_hurt_prob;
	int m_inc_hurt_percent;
};

#endif

