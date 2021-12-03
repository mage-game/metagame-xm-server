#ifndef _EFFECTCOUNTER_HPP_
#define _EFFECTCOUNTER_HPP_

#include "effect/effectbase.hpp"

// ¼ÆÊýÆ÷
class EffectCounter : public EffectBase
{
public:
	EffectCounter();
	EffectCounter(ObjID deliverer, UInt16 skill, int effect_time, unsigned  char buff_type, char product_level, char product_method = EffectBase::PRODUCT_METHOD_SKILL);
	virtual ~EffectCounter();

	virtual void Effect(Character *cha, bool *die) {}
	virtual void ReEffect(Character *cha, bool *die) {}
	virtual bool Update(unsigned long interval, Character *cha, bool *die);

	virtual unsigned char GetBuffType() const { return m_buff_type; }

	virtual int GetLeftTimeMS() { return m_effect_time; }

	virtual void Merge(EffectBase *new_effect, Character *cha);

	virtual char GetEffectType() { return EFFECT_TYPE_COUNTER; }
	virtual bool IsPositiveEffect() const { return true; }

	void SetIncHurtPercent(int percent) { m_inc_hurt_percent = percent; }
	int GetIncHurtPercent();

	virtual void GetEffectParam(short *count, long long param[EFFECT_INFO_PARAM_MAX]);

	virtual bool Serialize(TLVSerializer &s) const;
	virtual bool Unserialize(TLVUnserializer &s);

	void *operator new(size_t c);
	void operator delete(void *m);

protected:
	int	m_effect_time;
	unsigned char m_buff_type;

	int m_inc_hurt_percent;
};

#endif
