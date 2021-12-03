#ifndef _EFFECT_MODIFY_GLOBALCD_HPP_
#define _EFFECT_MODIFY_GLOBALCD_HPP_

#include "effect/effectbase.hpp"

// 修改全局CD
class EffectModifyGlobalCD : public EffectBase
{
public:
	EffectModifyGlobalCD();
	EffectModifyGlobalCD(ObjID deliverer, UInt16 skill, int effect_time, int percent, char product_level, char product_method = EffectBase::PRODUCT_METHOD_SKILL);
	virtual ~EffectModifyGlobalCD();

	virtual char GetEffectType() { return EFFECT_TYPE_MODIFY_GLOBAL_CD; }

	virtual void Effect(Character *cha, bool *die) {}
	virtual void ReEffect(Character *cha, bool *die) {}
	virtual bool Update(unsigned long interval, Character *cha, bool *die);

	virtual bool IsPositiveEffect() const { return m_percent < 0; }
	virtual int GetLeftTimeMS() { return m_effect_time; }

	virtual void GetEffectParam(short *count, long long param[EFFECT_INFO_PARAM_MAX]);

	void ModifyGlobalCD(long long *global_cd);

	virtual bool Serialize(TLVSerializer &s) const;
	virtual bool Unserialize(TLVUnserializer &s);

	void *operator new(size_t c);
	void operator delete(void *m);

protected:
	int m_effect_time;				// 作用剩余时间
	int m_percent;					// 修改百分比
};

#endif
