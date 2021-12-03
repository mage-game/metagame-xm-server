#ifndef _EFFECT_MOVESPEED_HPP_
#define _EFFECT_MOVESPEED_HPP_

#include "effect/effectbase.hpp"

class Character;

// 修改移动速度
class EffectMoveSpeed : public EffectBase
{
public:
	EffectMoveSpeed();
	EffectMoveSpeed(ObjID deliverer, UInt16 skill, int effect_time, unsigned char buff_type, char product_level, char product_method = EffectBase::PRODUCT_METHOD_SKILL);
	virtual ~EffectMoveSpeed();

	virtual char GetEffectType() { return EFFECT_TYPE_MOVESPEED; }
	virtual void Effect(Character *cha, bool *die);
	virtual void ReEffect(Character *cha, bool *die);
	virtual bool Update(unsigned long interval, Character *cha, bool *die);
	virtual void ClearEffect(Character *cha);

	virtual bool IsPositiveEffect() const;
	virtual int GetLeftTimeMS() { return m_effect_time; }
	virtual unsigned char GetBuffType() const { return m_buff_type; }

	int CompareWith(EffectBase *effect);

	void SetMoveSpeed(int movespeed, short percent) { m_movespeed = movespeed; m_percent = percent; }
	int GetMoveSpeed() { return m_movespeed; }

	virtual bool IsChihuanEffect() const;

	virtual void Recalc(Character *chara);

	virtual void GetEffectParam(short *count, long long param[EFFECT_INFO_PARAM_MAX]);

	void *operator new(size_t c);
	void operator delete(void *m);

	virtual bool Serialize(TLVSerializer &s) const;
	virtual bool Unserialize(TLVUnserializer &s);

protected:
	int	m_effect_time;
	unsigned char m_buff_type;

	int m_movespeed;
	short m_percent;				// 只用来客户端显示

	void ChangeSpeed(Character *cha, bool is_attach);
};

#endif
