#ifndef _EFFECTHURTINC_HPP_
#define _EFFECTHURTINC_HPP_

#include "effect/effectbase.hpp"
#include "obj/obj.h"
#include "obj/character/character.h"

// …À∫¶º”…Ó
class EffectIncHurt : public EffectBase
{
public:
	EffectIncHurt();
	EffectIncHurt(ObjID deliverer, UInt16 skill, int effect_time, char product_level, char product_method = EffectBase::PRODUCT_METHOD_SKILL);
	virtual ~EffectIncHurt();

	virtual char GetEffectType() { return EFFECT_TYPE_INC_HURT; }

	virtual void Effect(Character *cha, bool *die) {}
	virtual void ReEffect(Character *cha, bool *die) {}
	virtual bool Update(unsigned long interval, Character *cha, bool *die);

	virtual int GetLeftTimeMS() { return m_effect_time; }

	virtual void SetIncPercent(int percent);
	virtual void SetIncPercent(int percent_to_role, int percent_to_monster);

	int GetIncPercent(Character *target = NULL);

	virtual bool IsPositiveEffect() const { return true; }

	void GetEffectParam(short *count, long long param[EFFECT_INFO_PARAM_MAX]);

	virtual bool Serialize(TLVSerializer &s) const;
	virtual bool Unserialize(TLVUnserializer &s);

	void *operator new(size_t c);
	void operator delete(void *m);

protected:
	int	m_effect_time;
	int m_inc_percent_to_role;
	int m_inc_percent_to_monster;
};

#endif
