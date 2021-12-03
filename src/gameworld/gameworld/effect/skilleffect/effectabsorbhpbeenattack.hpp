#ifndef _EFFECT_ABSORB_HP_BEEN_ATTACK_HPP_
#define _EFFECT_ABSORB_HP_BEEN_ATTACK_HPP_

#include "effect/effectbase.hpp"

class EffectAbsorbHPBeenAttack : public EffectBase
{
public:
	EffectAbsorbHPBeenAttack();
	EffectAbsorbHPBeenAttack(ObjID deliverer, UInt16 skill, int effect_time, int percent, char product_level, char product_method = EffectBase::PRODUCT_METHOD_SKILL);
	virtual ~EffectAbsorbHPBeenAttack();

	virtual char GetEffectType() { return EFFECT_TYPE_ABSORB_HP_BEEN_ATTACK; }

	virtual void Effect(Character *cha, bool *die) {}
	virtual void ReEffect(Character *cha, bool *die) {}
	virtual bool Update(unsigned long interval, Character *cha, bool *die);

	virtual bool IsPositiveEffect() const { return true; }
	virtual int GetLeftTimeMS() { return m_effect_time; }

	virtual void GetEffectParam(short *count, long long param[EFFECT_INFO_PARAM_MAX]);

	int GetAbsorbPercent() const { return m_percent; }

	virtual bool Serialize(TLVSerializer &s) const;
	virtual bool Unserialize(TLVUnserializer &s);

	void *operator new(size_t c);
	void operator delete(void *m);

protected:
	int m_effect_time;				// 作用剩余时间
	int m_percent;					// 吸血百分比
};


#endif //_EFFECT_ABSORB_HP_BEEN_ATTACK_HPP_

