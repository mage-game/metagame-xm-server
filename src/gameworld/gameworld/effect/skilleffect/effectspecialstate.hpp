#ifndef __EFFECT_SPECIAL_STATE_HPP__
#define __EFFECT_SPECIAL_STATE_HPP__

#include <stdlib.h>

#include "effect/effectbase.hpp"

class Character;

// ������BUFF
class EffectSpecialState : public EffectBase
{
public:
	EffectSpecialState();
	EffectSpecialState(ObjID deliverer, UInt16 skill, int effect_time, unsigned char buff_type, char product_level, char priduct_method = EffectBase::PRODUCT_METHOD_SKILL);
	virtual ~EffectSpecialState();

	virtual char GetEffectType() { return EFFECT_TYPE_SPECIAL_STATE; }
	virtual void Effect(Character *cha, bool *die);
	virtual void ReEffect(Character *cha, bool *die) {}
	virtual bool Update(unsigned long interval, Character *cha, bool *die);
	virtual void ClearEffect(Character *cha) {}

	virtual bool IsPositiveEffect() const;

	virtual bool IsDingshenEffect() const;
	virtual bool IsXuanyunEffect() const;
	virtual bool IsChemoEffect() const;
	virtual bool IsChihuanEffect() const;

	virtual void SetLeftTimeMS(int effect_time) { if (effect_time >= 0) m_effect_time = effect_time; }
	virtual int GetLeftTimeMS() { return m_effect_time; }

	virtual unsigned char GetBuffType() const { return m_buff_type; }
	virtual void GetEffectParam(short *count, long long param[EFFECT_INFO_PARAM_MAX]);

	/* 
	@param�������Ҫ��Ϊ�ͻ��˲���p_3������p_3_w����BUFF��������ʾ�����ڿͻ����������⴦��
			��ô�ڵ��øú���ʱ����Ҫ���ò�������1000����֤�ͻ�����ʾ������
			Ȼ����ʹ�ø�ֵ���м���ǰ�ٳ���1000����֤���ݲ�����
	*/
	void SetParam(int param) { m_param = param; }
	int GetParam() { return m_param; }

	void *operator new(size_t c);
	void operator delete(void *m);

	virtual bool Serialize(TLVSerializer &s) const;
	virtual bool Unserialize(TLVUnserializer &s);

protected:
	int	m_effect_time;
	unsigned char m_buff_type;
	int m_param;
};

#endif // __EFFECT_SPECIAL_STATE_HPP__


