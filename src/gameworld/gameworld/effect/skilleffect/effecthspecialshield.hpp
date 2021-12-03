#ifndef __EFFECT_SPECIALSHIELD_HPP__
#define __EFFECT_SPECIALSHIELD_HPP__

#include <stdlib.h>

#include "effect/effectbase.hpp"

// ���ܣ�ÿ�ε��̶�Ѫ
class EffectSpecialShield : public EffectBase
{
public:
	EffectSpecialShield();
	EffectSpecialShield(ObjID deliverer, UInt16 skill, int effect_time, int effective_times, char product_level, unsigned char buff_type = EBT_HPSTORE,char product_method = EffectBase::PRODUCT_METHOD_SKILL);
	virtual ~EffectSpecialShield();

	virtual char GetEffectType() { return EFFECT_TYPE_SPECIAL_SHIELD; }

	virtual unsigned char GetBuffType() const { return m_buff_type; }

	virtual void Effect(Character *cha, bool *die) {}
	virtual void ReEffect(Character *cha, bool *die) {}
	virtual bool Update(unsigned long interval, Character *cha, bool *die);

	virtual bool IsPositiveEffect() const { return true; }
	virtual int GetLeftTimeMS() { return m_effect_time; }

	int GetLeftEffectiveTimes(){return m_effective_times;}
	void SetFixHurt(long long fix_hurt);
	bool ReplaceHurt(Attribute *delta_hp, Character *cha);

	void SendChangeEffecttimes(Character *cha);

	virtual void GetEffectParam(short *count, long long param[EFFECT_INFO_PARAM_MAX]);

	virtual bool Serialize(TLVSerializer &s) const;
	virtual bool Unserialize(TLVUnserializer &s);

	void *operator new(size_t c);
	void operator delete(void *m);

protected:
	int m_effect_time;				// ����ʣ��ʱ��
	int m_effective_times;			// ��Ч����
	int m_max_effective_times;		// ����Ч����
	long long m_fix_hurt;			// �̶���Ѫ
	unsigned char m_buff_type;
};

#endif


