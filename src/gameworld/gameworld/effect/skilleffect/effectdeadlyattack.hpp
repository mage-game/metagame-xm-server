#ifndef __EFFECT_DEADLY_ATTACK_HPP__
#define __EFFECT_DEADLY_ATTACK_HPP__

#include <stdlib.h>

#include "effect/effectbase.hpp"

// 致命一击，有该buff时被攻击者掉固定百分比血量
class EffectDeadlyAttack : public EffectBase
{
public:
	EffectDeadlyAttack(ObjID deliverer, UInt16 skill, int effect_time, int buff_type, int hp_hurt_per, char priduct_method = EffectBase::PRODUCT_METHOD_SKILL)
		: EffectBase(deliverer, skill, priduct_method, 1), m_effect_time(effect_time), m_hurt_fix_hp_per(hp_hurt_per), m_buff_type(buff_type){}
	virtual ~EffectDeadlyAttack() {}

	virtual char GetEffectType() { return EFFECT_TYPE_DEADLY_ATTACK; }

	virtual void Effect(Character *cha, bool *die){}
	virtual void ReEffect(Character *cha, bool *die){}
	virtual bool Update(unsigned long interval, Character *cha, bool *die);

	virtual int GetLeftTimeMS() { return m_effect_time; }

	int GetFixHurtPer(Character *target);

	virtual unsigned char GetBuffType() const { return m_buff_type; }
	virtual void GetEffectParam(short *count, long long param[EFFECT_INFO_PARAM_MAX]);

	virtual bool Serialize(TLVSerializer &s) const;
	virtual bool Unserialize(TLVUnserializer &s);

	void *operator new(size_t c);
	void operator delete(void *m);

protected:
	
	int	m_effect_time;
	int m_hurt_fix_hp_per;		// 掉血万分比
	unsigned char m_buff_type;
};

#endif