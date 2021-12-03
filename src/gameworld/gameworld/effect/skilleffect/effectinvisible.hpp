#ifndef _EFFECT_INVISIBLE_
#define _EFFECT_INVISIBLE_

#include <stdlib.h>
#include "effect/effectbase.hpp"

enum INVISIBLE_STATUS
{
	INVISIBLE_SATTUS_CAN_SEE,
	INVISIBLE_SATTUS_CAN_NOT_SEE
};

// 隐身
class EffectInvisible : public EffectBase
{
public:
	EffectInvisible();
	EffectInvisible(ObjID deliverer, UInt16 skill, int effect_time, char product_level, char product_method = EffectBase::PRODUCT_METHOD_SKILL);
	virtual ~EffectInvisible();

	virtual char GetEffectType() { return EFFECT_TYPE_INVISIBLE; }

	virtual unsigned char GetBuffType() const { return m_buff_type; }

	virtual void Effect(Character *cha, bool *die) {}
	virtual void ReEffect(Character *cha, bool *die) {}
	virtual bool Update(unsigned long interval, Character *cha, bool *die);

	virtual bool IsPositiveEffect() const { return true; }
	virtual int GetLeftTimeMS() { return m_effect_time; }

	virtual void GetEffectParam(short *count, long long param[EFFECT_INFO_PARAM_MAX]);

	virtual bool Serialize(TLVSerializer &s) const;
	virtual bool Unserialize(TLVUnserializer &s);

	void SetBuffType(unsigned char buff_type) { m_buff_type = buff_type; }

	void *operator new(size_t c);
	void operator delete(void *m);

protected:
	int m_effect_time;				// 作用剩余时间
	
	unsigned char m_buff_type;
};

#endif
