#ifndef __EFFECTONCE_HPP__
#define __EFFECTONCE_HPP__

#include <stdlib.h>

#include "effect/effectbase.hpp"


/************************************************************************/
/*                           计算一次伤害                                 */
/* 1. 如果是被攻击时触发的伤害反弹，一定要注意避免相互反弹造成递归调用，
      此时product_method设置为EffectBase::PRODUCT_METHOD_REBOUNDHURT     */
/* 2. 如果是加血操作，deliverer最好是INVALID_OBJ_ID，并且要判断角色IsAlive()  */
/************************************************************************/
class EffectOnce : public EffectBase
{
public:
	EffectOnce(ObjID deliverer, UInt16 skill, Attribute delta_hp, int fight_type, char product_method = EffectBase::PRODUCT_METHOD_SKILL)
		: EffectBase(deliverer, skill, product_method, 1), m_delta_hp(delta_hp), m_fight_type(fight_type), m_blood_seq(0), m_passive_flag(0) {}
	virtual ~EffectOnce() {}

	virtual char GetEffectType() { return EFFECT_TYPE_ONCE; }
	virtual void Effect(Character *cha, bool *die);
	virtual void ReEffect(Character *cha, bool *die) {}
	virtual bool Update(unsigned long interval, Character *cha, bool *die) { return false; }

	virtual bool IsPositiveEffect() const { return (m_delta_hp > 0); }

	void SetDeltaHp(Attribute delta_hp) { m_delta_hp = delta_hp; }
	void SetFightType(int fight_type) { m_fight_type = fight_type; }

	void SetBloodSeq(unsigned char blood_seq) { m_blood_seq = blood_seq; }
	void SetPassiveFlag(int passive_flag) { m_passive_flag = passive_flag; }

	virtual int GetLeftTimeMS() { return 0; }

	void *operator new(size_t c);
	void operator delete(void *m);

protected:
	Attribute		m_delta_hp;
	unsigned short	m_fight_type;	
	unsigned char	m_blood_seq;
	int				m_passive_flag;
};

#endif