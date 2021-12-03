#ifndef _EFFECT_FORBIDRECOVER_
#define _EFFECT_FORBIDRECOVER_


#include "effect/effectbase.hpp"

// ��ֹ�ָ� һ��ʱ���ڽ�ֹ�ָ�Ѫ��������������ʱ���ɽ�ֹ��
class EffectForbidRecover : public EffectBase
{
public:
	EffectForbidRecover();
	EffectForbidRecover(ObjID deliverer, UInt16 skill, int effect_time, char product_level, char product_method = EffectBase::PRODUCT_METHOD_SKILL);
	virtual ~EffectForbidRecover();

	virtual char GetEffectType() { return EFFECT_TYPE_FORBID_RECOVER; }
	virtual unsigned char GetBuffType() const { return EBT_FORBID_RECOVER_HP; }

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
	int m_effect_time;				// ����ʣ��ʱ��
};

#endif  // _EFFECT_FORBIDRECOVER_
