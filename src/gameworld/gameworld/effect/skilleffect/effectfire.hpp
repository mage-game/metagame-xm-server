#ifndef __EFFECTFIRE_HPP__
#define __EFFECTFIRE_HPP__

#include <stdlib.h>

#include "effect/effectbase.hpp"

class Character;

// 火
class EffectFire : public EffectBase
{
public:
	EffectFire();
	EffectFire(ObjID deliverer, UInt16 skill, char product_level, char product_method = EffectBase::PRODUCT_METHOD_SKILL);
	virtual ~EffectFire();

	void SetFireParam(int interval, int count, Attribute delta_hp_self, Attribute delta_hp_other);
	void SetIntervalTime(int interval_time);

	virtual char GetEffectType() { return EFFECT_TYPE_FIRE; }

	virtual void Effect(Character *cha, bool *die) {}
	virtual void ReEffect(Character *cha, bool *die) {}
	virtual bool Update(unsigned long interval, Character *cha, bool *die);

	virtual unsigned char GetBuffType() const { return EBT_FIER; }
	virtual int GetLeftTimeMS() { return m_count * m_interval; }
	virtual void GetEffectParam(short *count, long long param[EFFECT_INFO_PARAM_MAX]);

	virtual void ClearEffect(Character *cha) {}

	virtual bool IsPositiveEffect() const { return m_delta_hp_self <= 0; }

	void *operator new(size_t c);
	void operator delete(void *m);

	virtual bool Serialize(TLVSerializer &s) const;
	virtual bool Unserialize(TLVUnserializer &s);
	virtual bool CanEffect(Character *cha);

	void SetRandPercent(int rand_percent) { m_rand_percent = rand_percent; }

protected:
	int			m_interval_time;

	int			m_interval;									// 间隔
	int			m_count;									// 次数
	long long	m_delta_hp_self;							// 对自己伤害
	long long	m_delta_hp_other;							// 对他人伤害
	int			m_rand_percent;								// 随机百分比
};

#endif

