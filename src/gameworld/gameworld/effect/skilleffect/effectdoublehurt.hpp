#ifndef _EFFECTDOUBLEHURT_HPP_
#define _EFFECTDOUBLEHURT_HPP_

#include "effect/effectbase.hpp"

// ¶þ´ÎÉËº¦
class EffectDoubleHurt : public EffectBase
{
public:
	EffectDoubleHurt();
	EffectDoubleHurt(ObjID deliverer, UInt16 skill, int effect_time, char product_level, char product_method = EffectBase::PRODUCT_METHOD_SKILL);
	virtual ~EffectDoubleHurt();

	virtual char GetEffectType() { return EFFECT_TYPE_DOUBLE_HURT; }

	virtual void Effect(Character *cha, bool *die) {}
	virtual void ReEffect(Character *cha, bool *die) {}
	virtual bool Update(unsigned long interval, Character *cha, bool *die);

	virtual int GetLeftTimeMS() { return m_effect_time; }

	virtual bool IsPositiveEffect() const { return true; }

	int GetHurtPercent() { return m_hurt_percent; }
	int GetFixHurt() { return m_fix_hurt; }
	void SetHurtParam(int hurt_percent, int fix_hurt) { m_hurt_percent = hurt_percent; m_fix_hurt = fix_hurt; }

	void GetEffectParam(short *count, long long param[EFFECT_INFO_PARAM_MAX]);

	virtual bool Serialize(TLVSerializer &s) const;
	virtual bool Unserialize(TLVUnserializer &s);

	void *operator new(size_t c);
	void operator delete(void *m);

protected:
	int	m_effect_time;
	int m_hurt_percent;
	int m_fix_hurt;
};

#endif
