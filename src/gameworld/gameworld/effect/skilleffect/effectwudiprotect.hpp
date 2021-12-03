#ifndef _EFFECT_WUDIPROTECT_
#define _EFFECT_WUDIPROTECT_


#include <stdlib.h>

#include "effect/effectbase.hpp"

// 无敌保护	受到的所有伤害都为1
class EffectWudiProtect : public EffectBase
{
public:
	EffectWudiProtect();
	EffectWudiProtect(ObjID deliverer, UInt16 skill, int effect_time, char product_level, char product_method = EffectBase::PRODUCT_METHOD_SKILL);
	virtual ~EffectWudiProtect();

	virtual char GetEffectType() { return EFFECT_TYPE_WUDI_PROTECT; }
	virtual unsigned char GetBuffType() const { return EBT_WUDI_PROTECT; }

	virtual void Effect(Character *cha, bool *die) {}
	virtual void ReEffect(Character *cha, bool *die) {}
	virtual bool Update(unsigned long interval, Character *cha, bool *die);

	virtual bool IsPositiveEffect() const { return true; }
	virtual int GetLeftTimeMS() { return m_effect_time; }

	virtual void GetEffectParam(short *count, long long param[EFFECT_INFO_PARAM_MAX]);

	virtual bool Serialize(TLVSerializer &s) const;
	virtual bool Unserialize(TLVUnserializer &s);

	void *operator new(size_t c);
	void operator delete(void *m);

protected:
	int m_effect_time;				// 作用剩余时间
};

#endif
