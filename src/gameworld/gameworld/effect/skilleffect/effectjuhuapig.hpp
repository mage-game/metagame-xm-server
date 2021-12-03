#ifndef _EFFECT_JUHUAPIG_HPP_
#define _EFFECT_JUHUAPIG_HPP_


#include "effect/effectbase.hpp"

class Character;

class EffectJuhuaPig : public EffectBase
{
public:
	EffectJuhuaPig();
	EffectJuhuaPig(ObjID deliverer, UInt16 skill, char product_level, char product_method = EffectBase::PRODUCT_METHOD_SKILL);
	virtual ~EffectJuhuaPig();

	virtual char GetEffectType() { return EFFECT_TYPE_JUHUAPIG; }

	virtual void Effect(Character *cha, bool *die) {}
	virtual void ReEffect(Character *cha, bool *die) {}
	virtual bool Update(unsigned long interval, Character *cha, bool *die);

	void SetParam(int effect_time, long long injure_deadline, int rebound_percent, int range, long long init_hp);

	virtual int GetLeftTimeMS() { return m_effect_time; }
	virtual void GetEffectParam(short *count, long long param[EFFECT_INFO_PARAM_MAX]);

	virtual void ClearEffect(Character *cha) {}

	virtual bool IsPositiveEffect() const { return true; }

	void *operator new(size_t c);
	void operator delete(void *m);

	virtual bool Serialize(TLVSerializer &s) const;
	virtual bool Unserialize(TLVUnserializer &s);

protected:
	void Explode(Character *cha, long long injure);

	int m_effect_time;
	long long m_injure_deadline;
	int m_rebound_percent;
	int m_range;
	long long m_init_hp;
};

#endif
