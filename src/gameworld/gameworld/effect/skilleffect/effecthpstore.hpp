#ifndef __EFFECT_HPRESTORE_HPP__
#define __EFFECT_HPRESTORE_HPP__

#include <stdlib.h>

#include "effect/effectbase.hpp"

// 护盾
class EffectHpStore : public EffectBase
{
public:
	EffectHpStore();
	EffectHpStore(ObjID deliverer, UInt16 skill, int effect_time, long long hp_restore, char product_level, char product_method = EffectBase::PRODUCT_METHOD_SKILL);
	virtual ~EffectHpStore();

	virtual char GetEffectType() { return EFFECT_TYPE_HPSTORE; }

	virtual unsigned char GetBuffType() const { return EBT_HPSTORE; }

	virtual void Effect(Character *cha, bool *die) {}
	virtual void ReEffect(Character *cha, bool *die) {}
	virtual bool Update(unsigned long interval, Character *cha, bool *die);

	virtual bool IsPositiveEffect() const { return true; }
	virtual int GetLeftTimeMS() { return m_effect_time; }

	void SetReplacePercent(float replace_percent);
	bool ReplaceHurt(Attribute *delta_hp);

	virtual void GetEffectParam(short *count, long long param[EFFECT_INFO_PARAM_MAX]);

	virtual bool Serialize(TLVSerializer &s) const;
	virtual bool Unserialize(TLVUnserializer &s);

	void *operator new(size_t c);
	void operator delete(void *m);

protected:
	int m_effect_time;				// 作用剩余时间
	long long m_hp_restore;			// 血量
	float m_replace_percent;		// 减免百分比
};

#endif


