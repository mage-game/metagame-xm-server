#ifndef _EFFECTATTRCONTAINER_HPP_
#define _EFFECTATTRCONTAINER_HPP_

#include "effect/effectbase.hpp"
#include "obj/character/attribute.hpp"

// 属性容器效果，例如血池，玩家从血池中回血，血池为空时退出
class EffectAttrContainer : public EffectBase
{
public:
	EffectAttrContainer();
	EffectAttrContainer(ObjID deliverer, UInt16 product_id, int effect_time, char product_level, char product_method = EffectBase::PRODUCT_METHOD_SKILL);
	virtual ~EffectAttrContainer();

	virtual void SetLeftTimeMS(int effect_time) { if (effect_time > 0) m_effect_time = effect_time; }
	virtual int GetLeftTimeMS() { return m_effect_time; }
	virtual char GetEffectType() { return EFFECT_TYPE_ATTR_CONTAINER; }
	virtual void Effect(Character *cha, bool *die) {}
	virtual void ReEffect(Character *cha, bool *die) {}		// 角色init的时候调用，各effect根据自己情况实现，基本原则是不要改HP
	virtual bool Update(unsigned long interval, Character *cha, bool *die);
	virtual void Merge(EffectBase *new_effect, Character *cha);
	virtual bool IsPositiveEffect() const { return false; }
	virtual void GetEffectParam(short *count, long long param[EFFECT_INFO_PARAM_MAX]);

	void DoEffect(Character *cha);			// 执行效果
	bool HasUsedUp() const;					// 是否用光
	// virtual void MergeParam(EffectBase *effect);
	unsigned short GetTotalAttr() { return m_total_attr; } // 返回总数量
	void SetContinueHp(int interval, unsigned short total_hp, unsigned short hp, unsigned short idle_hp) { m_interval = interval; m_total_attr = total_hp; m_fight_attr = hp; m_idle_attr = idle_hp; }
	void SetContinueMp(int interval, unsigned short total_mp, unsigned short mp, unsigned short idle_mp) { m_interval = interval; m_total_attr = total_mp; m_fight_attr = mp; m_idle_attr = idle_mp; }

	void *operator new(size_t c);
	void operator delete(void *m);
	virtual bool Serialize(TLVSerializer &s) const;
	virtual bool Unserialize(TLVUnserializer &s);

protected:
	int m_effect_time;			// 作用时间
	int m_interval;				// 时间间隔
	int m_elapse_time;			// 时间流逝
	bool m_do_now;				// 立即执行一次，不等m_interval
	bool m_has_time_limit;		// 是否有时间限制
	unsigned short m_used_amount;

	unsigned short m_level;			// 等级level
	unsigned short m_total_attr;	// 内容总量
	unsigned short m_fight_attr;	// 战斗状态增加数量
	unsigned short m_idle_attr;		// 休闲状态增加数量
};

#endif