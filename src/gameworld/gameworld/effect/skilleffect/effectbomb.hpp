#ifndef __EFFECTBOMB_HPP__
#define __EFFECTBOMB_HPP__

#include <stdlib.h>

#include "effect/effectbase.hpp"

class Character;

// ը��
class EffectBomb : public EffectBase
{
public:
	EffectBomb();
	EffectBomb(ObjID deliverer, UInt16 skill, unsigned char buff_type, char product_level, int deliver_uid, char product_method = EffectBase::PRODUCT_METHOD_SKILL);
	virtual ~EffectBomb();

	void SetBloodParam(int interval, int count, Attribute delta_hp);
	void SetBombParam(long long injure, int hurt_percent_to_holder, int hurt_percent_to_other);

	virtual char GetEffectType() { return EFFECT_TYPE_BOMB; }

	virtual void Effect(Character *cha, bool *die) {}
	virtual void ReEffect(Character *cha, bool *die) {}
	virtual bool Update(unsigned long interval, Character *cha, bool *die);

	virtual unsigned char GetBuffType() const { return m_buff_type; }
	virtual int GetLeftTimeMS() { return m_count * m_interval; }
	virtual void GetEffectParam(short *count, long long param[EFFECT_INFO_PARAM_MAX]);

	virtual void ClearEffect(Character *cha) {}

	void ForceToExplode();
	void Explode(Character *cha, bool *die);

	virtual bool IsPositiveEffect() const { return false; }

	void *operator new(size_t c);
	void operator delete(void *m);

	virtual bool Serialize(TLVSerializer &s) const;
	virtual bool Unserialize(TLVUnserializer &s);

protected:
	int			m_interval_time;							// ������֮�侭����ʱ��

	unsigned char m_buff_type;

	int			m_deliverer_uid;							// �ͷ���uid

	long long 	m_delta_hp;									// ÿ����Ѫ�� ը��Ҳ��Ѫ
	int			m_interval;									// ÿ�����
	int			m_count;									// ������

	long long	m_injure;									// �˺���
	int			m_hurt_percent_to_holder;					// ��ըʱ���Գ����ߵ��˺��ٷֱ�
	int			m_hurt_percent_to_other;					// ��ըʱ���������˵��˺��ٷֱ�
};

#endif

