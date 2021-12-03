#ifndef _EFFECT_GRADUAL_HURT_HPP_
#define _EFFECT_GRADUAL_HURT_HPP_

#include "effect/effectbase.hpp"

class Character;

// 渐增伤害
class EffectGradualHurt : public EffectBase
{
public:
	EffectGradualHurt();
	EffectGradualHurt(ObjID deliverer, UInt16 skill, char product_level, char product_method = EffectBase::PRODUCT_METHOD_SKILL);
	virtual ~EffectGradualHurt();

	void SetParam(int interval, int count, long long init_hurt, int percent, long long fix_hurt);

	virtual char GetEffectType() { return EFFECT_TYPE_GRADUAL_HURT; }

	virtual void Effect(Character *cha, bool *die) {}
	virtual void ReEffect(Character *cha, bool *die) {}
	virtual bool Update(unsigned long interval, Character *cha, bool *die);

	virtual int GetLeftTimeMS() { return m_count * m_interval; }
	virtual void GetEffectParam(short *count, long long param[EFFECT_INFO_PARAM_MAX]);

	virtual bool IsPositiveEffect() const { return false; }

	void *operator new(size_t c);
	void operator delete(void *m);

	virtual bool Serialize(TLVSerializer &s) const;
	virtual bool Unserialize(TLVUnserializer &s);

protected:
	int			m_interval_time;							// 在两跳之间经过的时间

	long long	m_init_hurt;
	int			m_percent;
	long long	m_fix;
	int			m_interval;
	int			m_count;
	int			m_past_count;
};


#endif
