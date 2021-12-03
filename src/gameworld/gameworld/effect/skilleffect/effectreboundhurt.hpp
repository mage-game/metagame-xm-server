#ifndef _EFFECT_REBOUNDHURT_HPP_
#define _EFFECT_REBOUNDHURT_HPP_

#include "effect/effectbase.hpp"

// 反弹伤害
class EffectReboundHurt : public EffectBase
{
public:
	EffectReboundHurt();
	EffectReboundHurt(ObjID deliverer, UInt16 skill, int effect_time, int percent, char product_level, char product_method = EffectBase::PRODUCT_METHOD_SKILL);
	virtual ~EffectReboundHurt();

	virtual char GetEffectType() { return EFFECT_TYPE_REBOUND_HURT; }

	virtual void Effect(Character *cha, bool *die) {}
	virtual void ReEffect(Character *cha, bool *die) {}
	virtual bool Update(unsigned long interval, Character *cha, bool *die);

	virtual bool IsPositiveEffect() const { return true; }
	virtual int GetLeftTimeMS() { return m_effect_time; }

	virtual unsigned char GetBuffType() const { return EBT_REBOUNDHURT; }

	virtual void GetEffectParam(short *count, long long param[EFFECT_INFO_PARAM_MAX]);

	int GetReboundPercent() { return m_percent; }

	virtual int CompareWith(EffectBase *effect);

	virtual bool Serialize(TLVSerializer &s) const;
	virtual bool Unserialize(TLVUnserializer &s);

	void *operator new(size_t c);
	void operator delete(void *m);

protected:
	int m_effect_time;				// 作用剩余时间
	int m_percent;					// 反弹百分比
};

#endif
