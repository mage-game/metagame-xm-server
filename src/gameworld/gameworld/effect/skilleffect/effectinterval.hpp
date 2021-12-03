#ifndef __EFFICTINTERVAL_HPP__
#define __EFFICTINTERVAL_HPP__

#include <stdlib.h>

#include "effect/effectbase.hpp"

class Character;
class Scene;

// ��ʱ�޸�HP  (ʹ�����effect����һ��Ҫע��product_method������Ӱ�����effect����������忴EffectList::ClearFightEffect)
class EffectInterval : public EffectBase
{
public:
	EffectInterval();
	EffectInterval(ObjID deliverer, UInt16 skill, unsigned char buff_type, char product_level, char product_method = EffectBase::PRODUCT_METHOD_SKILL);
	virtual ~EffectInterval();

	void SetParam(int interval, int count, Attribute delta_hp, int deliver_roleid = 0);
	Attribute GetRemainDropBlood() { return m_count * m_merge_layer * m_delta_hp; }

	virtual char GetEffectType() { return EFFECT_TYPE_INTERVAL_HP; }

	virtual void Effect(Character *cha, bool *die) {}
	virtual void ReEffect(Character *cha, bool *die) {}
	virtual bool Update(unsigned long interval, Character *cha, bool *die);

	virtual unsigned char GetBuffType() const { return m_buff_type; }
	virtual int GetLeftTimeMS() { return m_count * m_interval; }
	virtual void GetEffectParam(short *count, long long param[EFFECT_INFO_PARAM_MAX]);

	virtual void Merge(EffectBase *new_effect, Character *cha);

	virtual bool IsPositiveEffect() const { return m_delta_hp > 0; }

	void SetFightType(int fight_type) { m_fight_type = fight_type; }

	void *operator new(size_t c);
	void operator delete(void *m);

	virtual bool Serialize(TLVSerializer &s) const;
	virtual bool Unserialize(TLVUnserializer &s);

protected:
	int			m_interval_time;					// ������֮�侭����ʱ��

	unsigned char m_buff_type;

	Attribute	m_delta_hp;
	int			m_interval;							// ��λ������
	int			m_count;							// ����
	int			m_deliver_roleid;

	int			m_fight_type;
};

#endif

