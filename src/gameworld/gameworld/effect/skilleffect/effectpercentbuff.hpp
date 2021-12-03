#ifndef __EFFECTPERCENTBUFF_HPP__
#define __EFFECTPERCENTBUFF_HPP__

#include <stdlib.h>

#include "effect/effectbase.hpp"

#include "obj/character/attribute.hpp"

// �޸����԰ٷֱ�
class EffectPercentBuff : public EffectBase
{
public:
	EffectPercentBuff();
	EffectPercentBuff(ObjID deliverer, UInt16 skill, int effect_time, char product_level, char product_method = EffectBase::PRODUCT_METHOD_SKILL);
	virtual ~EffectPercentBuff();

	virtual char GetEffectType() { return EFFECT_TYPE_ATTR_PER; }
	virtual void Effect(Character *cha, bool *die);
	virtual void ReEffect(Character *cha, bool *die);
	virtual bool Update(unsigned long interval, Character *cha, bool *die);
	virtual void ClearEffect(Character *cha);

	virtual int GetLeftTimeMS() { return m_effect_time; }

	virtual void Merge(EffectBase *new_effect, Character *cha);

	virtual bool IsPositiveEffect() const;

	void AddAttrEffect(char attr_type, short percent = 0);

	virtual void GetEffectParam(short *count, long long param[EFFECT_INFO_PARAM_MAX]);

	void *operator new(size_t c);
	void operator delete(void *m);

	virtual bool Serialize(TLVSerializer &s) const;
	virtual bool Unserialize(TLVUnserializer &s);

	void ReCalcPercent(CharIntAttrs &base_add);

protected:
	void Attach(Character *cha, bool is_attach);

	int m_effect_time;												// ����ʱ��
	bool m_has_time_limit;											// �Ƿ���ʱ������
	static const int MAX_ATTR_COUNT = 3;

	int m_count;													// ������������
	CharIntAttrs::AttrPair m_pair_list[MAX_ATTR_COUNT];				// ������ֵ�б�
	short m_percent_list[MAX_ATTR_COUNT];							// ���԰ٷֱ��б�
};

#endif


