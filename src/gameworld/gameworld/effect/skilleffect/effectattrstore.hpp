#ifndef _EFFECTATTRSTORE_HPP_
#define _EFFECTATTRSTORE_HPP_

#include "effect/effectbase.hpp"

// 存储类，按照时间累计属性，达到最大或者时间到删除
class EffectAttrStore : public EffectBase
{
public:
	EffectAttrStore();
	EffectAttrStore(ObjID deliverer, UInt16 product_id, int effect_time, char product_level, char product_method = EffectBase::PRODUCT_METHOD_SKILL);
	virtual ~EffectAttrStore();

	virtual void SetLeftTimeMS(int effect_time) { if (effect_time > 0) m_effect_time = effect_time; }
	virtual int GetLeftTimeMS() { return m_effect_time; }
	virtual char GetEffectType() { return EFFECT_TYPE_ATTR_STORE; }
	virtual void Effect(Character *cha, bool *die) {}
	virtual void ReEffect(Character *cha, bool *die) {}
	virtual bool Update(unsigned long interval, Character *cha, bool *die);
// 	virtual void Merge(EffectBase *new_effect, Character *cha);
	virtual bool IsPositiveEffect() const { return false; }
	virtual void GetEffectParam(short *count, long long param[EFFECT_INFO_PARAM_MAX]);

	virtual void AddUp(int attr);
	virtual void SetParams(int range, int max_num, int range_type, int angle);

	void *operator new(size_t c);
	void operator delete(void *m);
	virtual bool Serialize(TLVSerializer &s) const;
	virtual bool Unserialize(TLVUnserializer &s);

protected:
	int m_effect_time;		// 作用时间
	bool m_has_time_limit;	// 是否有时间限制

	unsigned short m_level;
	unsigned short m_cur_attr;		// 当前属性值
	int m_range;
	int m_max_num;
	int m_range_type;
	int m_angle;
	
private:
};

#endif