#ifndef __EFFECTSHENBING_HPP__
#define __EFFECTSHENBING_HPP__

#include <stdlib.h>

#include "effect/effectbase.hpp"

#include "obj/character/attribute.hpp"

// 神兵
class EffectShenbing : public EffectBase
{
public:
	EffectShenbing();
	EffectShenbing(ObjID deliverer, UInt16 skill, int effect_time, char product_level, char product_method = EffectBase::PRODUCT_METHOD_SKILL);
	virtual ~EffectShenbing();

	virtual char GetEffectType() { return EFFECT_TYPE_SHENBING; }
	virtual void Effect(Character *cha, bool *die);
	virtual void ReEffect(Character *cha, bool *die);
	virtual bool Update(unsigned long interval, Character *cha, bool *die);
	virtual void ClearEffect(Character *cha);

	virtual int GetLeftTimeMS() { return static_cast<int>(m_effect_time); }

	virtual void Merge(EffectBase *new_effect, Character *cha);

	virtual bool IsPositiveEffect() const;

	void AddMaxHPEffect(Attribute maxhp);
	void AddGongJiEffect(Attribute gongji);
	void AddFangYuEffect(Attribute fangyu);
	void AddMingZhongEffect(Attribute mingzhong);
	void AddShanBiEffect(Attribute shanbi);
	void AddBaojiEffect(Attribute baoji);
	void AddJianRenEffect(Attribute jianren);

	virtual void GetEffectParam(short *count, long long param[EFFECT_INFO_PARAM_MAX]);

	void *operator new(size_t c);
	void operator delete(void *m);

	virtual bool Serialize(TLVSerializer &s) const;
	virtual bool Unserialize(TLVUnserializer &s);

protected:
	void Attach(Character *cha, bool is_attach);

	long long m_effect_time;												// 作用时间

	Attribute m_maxhp;
	Attribute m_gongji;
	Attribute m_fangyu;
	Attribute m_mingzhong;
	Attribute m_shanbi;
	Attribute m_baoji;
	Attribute m_jianren;
};

#endif


