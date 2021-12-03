#ifndef _EFFECTHURTDEC_HPP_
#define _EFFECTHURTDEC_HPP_

#include "effect/effectbase.hpp"
#include "obj/obj.h"
#include "obj/character/character.h"

// 伤害减弱
class EffectDecHurt : public EffectBase
{
public:
	EffectDecHurt();
	EffectDecHurt(ObjID deliverer, UInt16 skill, int effect_time, char product_level,char product_method = EffectBase::PRODUCT_METHOD_SKILL, char buff_type = EBT_DEC_HURT);
	virtual ~EffectDecHurt();

	virtual char GetEffectType() { return EFFECT_TYPE_DEC_HURT; }
	virtual unsigned char GetBuffType() const { return m_buff_type; }

	virtual void Effect(Character *cha, bool *die) {}
	virtual void ReEffect(Character *cha, bool *die) {}
	virtual bool Update(unsigned long interval, Character *cha, bool *die);

	virtual int GetLeftTimeMS() { return m_effect_time; }

	virtual void SetDecPercent(int percent);
	virtual void SetDecPercent(int percent_to_role, int percent_to_monster);

	int GetDecPercent(Character *target = NULL);
	
	virtual void SetDecFix(int fix);
	int GetDecFix(Character *target = NULL);

	virtual bool IsPositiveEffect() const { return true; }

	void GetEffectParam(short *count, long long param[EFFECT_INFO_PARAM_MAX]);

	virtual bool Serialize(TLVSerializer &s) const;
	virtual bool Unserialize(TLVUnserializer &s);

	void *operator new(size_t c);
	void operator delete(void *m);

protected:
	int	m_effect_time;		// 作用总时间
	int m_left_time;		// 作用剩余时间
	int m_buff_type;			

	int m_dec_to_role;
	int m_dec_to_monster;
	int m_dec_fix;
};

#endif
