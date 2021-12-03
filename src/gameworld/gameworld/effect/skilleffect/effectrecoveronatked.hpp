#ifndef _EFFECT_RECOVER_ON_ATKED_HPP_
#define _EFFECT_RECOVER_ON_ATKED_HPP_


#include "effect/effectbase.hpp"

// ������Ѫ
class EffectRecoverOnAtked : public EffectBase
{
public:
	EffectRecoverOnAtked();
	EffectRecoverOnAtked(ObjID deliverer, UInt16 skill, int effect_time, int percent, char product_level, char product_method = EffectBase::PRODUCT_METHOD_SKILL);
	virtual ~EffectRecoverOnAtked();

	virtual char GetEffectType() { return EFFECT_TYPE_RECOVER_ON_ATKED; }

	virtual void Effect(Character *cha, bool *die) {}
	virtual void ReEffect(Character *cha, bool *die) {}
	virtual bool Update(unsigned long interval, Character *cha, bool *die);

	virtual bool IsPositiveEffect() const { return true; }
	virtual int GetLeftTimeMS() { return m_effect_time; }

	virtual void GetEffectParam(short *count, long long param[EFFECT_INFO_PARAM_MAX]);

	int GetRecoverPercent() { return m_percent; }

	virtual int CompareWith(EffectBase *effect);

	virtual bool Serialize(TLVSerializer &s) const;
	virtual bool Unserialize(TLVUnserializer &s);

	void *operator new(size_t c);
	void operator delete(void *m);

protected:
	int m_effect_time;				// ����ʣ��ʱ��
	int m_percent;					// �����ٷֱ�
};

#endif
