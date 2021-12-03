#ifndef _EFFECTABSORBHP_HPP_
#define _EFFECTABSORBHP_HPP_

#include "effect/effectbase.hpp"

// 吸血

class EffectAbsorbHP : public EffectBase
{
public:
	EffectAbsorbHP();
	EffectAbsorbHP(ObjID deliverer, UInt16 skill, int effect_time, int percent, char product_level, char product_method = EffectBase::PRODUCT_METHOD_SKILL, unsigned char buff_type = EBT_INVALID);
	virtual ~EffectAbsorbHP();

	virtual char GetEffectType() { return EFFECT_TYPE_ABSORB_HP; }

	virtual void Effect(Character *cha, bool *die) {}
	virtual void ReEffect(Character *cha, bool *die) {}
	virtual bool Update(unsigned long interval, Character *cha, bool *die);

	virtual unsigned char GetBuffType() const;

	virtual bool IsPositiveEffect() const { return true; }
	virtual int GetLeftTimeMS() { return m_effect_time; }

	virtual void GetEffectParam(short *count, long long param[EFFECT_INFO_PARAM_MAX]);

	int GetAbsorbPercent() { return m_percent; }

	virtual bool Serialize(TLVSerializer &s) const;
	virtual bool Unserialize(TLVUnserializer &s);

	void *operator new(size_t c);
	void operator delete(void *m);

protected:
	int m_effect_time;				// 作用剩余时间
	int m_percent;					// 吸血百分比

	unsigned char m_buff_type;
};


#endif

