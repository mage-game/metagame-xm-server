#ifndef __EFFECTMULTIFUNBUFF_HPP__
#define __EFFECTMULTIFUNBUFF_HPP__

#include <stdlib.h>

#include "effect/effectbase.hpp"
#include "obj/character/attribute.hpp"

// 其他BUFF
class EffectOtherBuff : public EffectBase
{
public:
	EffectOtherBuff();
	EffectOtherBuff(ObjID deliverer, UInt16 product_id, int effect_time, char product_level, char product_method = EffectBase::PRODUCT_METHOD_SKILL);
	virtual ~EffectOtherBuff();

	virtual char GetEffectType() { return EFEECT_TYPE_OTHER; }
	virtual void Effect(Character *cha, bool *die);
	virtual void ReEffect(Character *cha, bool *die);
	virtual bool Update(unsigned long interval, Character *cha, bool *die);
	void IntervalEffect(Character *cha);
	virtual void ClearEffect(Character *cha);

	void ReCalcBaseAttr(CharIntAttrs &base_add);

	virtual void SetLeftTimeMS(int effect_time) { if (effect_time >= 0) m_effect_time = effect_time; }
	virtual int GetLeftTimeMS() { return m_effect_time; }

	virtual void Merge(EffectBase *new_effect, Character *cha);

	virtual void GetEffectParam(short *count, long long param[EFFECT_INFO_PARAM_MAX]);

	void *operator new(size_t c);
	void operator delete(void *m);

	virtual bool Serialize(TLVSerializer &s) const;
	virtual bool Unserialize(TLVUnserializer &s);

	// 采集速度
	void IncGatherSpeedPer(short per) { m_param1 = per; }
	short GetIncGatherSpeedPer() { return m_param1; }

	// 对建筑物伤害
	void IncToBuildingHurtPer(short per) { m_param1 = per; }
	short GetIncToBuildingHurtPer() { return m_param1; }

	// 设置气血间隔改变值
	void SetContinueHp(int interval, unsigned short hp) {m_interval = interval; m_param1 = hp; }

	// 设置mp间隔改变值
	void SetContinueMp(int interval, unsigned short mp) {m_interval = interval; m_param1 = mp; }

	// 增加穿戴等级
	void IncPutOnLevel(short level) { m_param1 = level; }
	short GetIncPutOnLevel() { return m_param1; }

	// 经验额外加成百分比
	void IncExtraExpPer(short per) { m_param1 = per; }
	short GetIncExtraExpPer() {return m_param1; }

	// 攻击别人时提升伤害
	void IncHurtOnAtk(unsigned short per) { m_param1 = per; }
	unsigned short GetIncHurtOnAtk() { return m_param1; }

	// 攻城战 下一次攻击会额外造成对方总血量百分比
	void SetGczNextHpPerHurt(short per) { m_param1 = per;}
	short GettGczNextHpPerHurt() { return m_param1; }

	// 攻城战 下一次攻击对方的防御降低20%，持续10S
	void SetGczNextDecFYPer(short per, short time) { m_param1 = per; m_param2 = time; }
	void GetGczNextDecFYPer(short *per, short *time) { if(NULL != per && NULL != time) { *per = m_param1; *time = m_param2; }}

	// 攻城战 下一次攻击雕像会使直接减少雕像3%的气血
	void SetGczReduceBossHpPer(short per) { m_param1 = per;}
	short GetGczReduceBossHpPer() { return m_param1; }

	// 攻城战 设置下一次攻击会使受到攻击的玩家及附近的玩家受到2S的沉默效果
	void SetGczNextCMTime(short time) { m_param1 = time; }
	short GetGczNextCMTime() { return m_param1; }

	// 攻城战 下一次攻击会使受到攻击的玩家及附近的玩家受到2S的定身效果
	void SetGczNextDSTime(short time) { m_param1 = time; }
	short GetGczNextDSTime() { return m_param1; }

	//  攻城战 下一次攻击会使受到攻击的玩家受到2S的眩晕
	void SetGczNextXYTime(short time) { m_param1 = time; }
	short GetGczNextXYTime() { return m_param1; }

	// 增加人物基础属性
	void SetRoleBaseAttr(int attr_type, int value) { m_param1 = static_cast<int>(attr_type); m_param2 = static_cast<int>(value); }

protected:
	void Attach(Character *cha, bool is_attach);

	int m_effect_time;												// 作用时间

	int m_interval_is_do_now;										// 处理间隔是否马上执行，不等x秒后再执行
	int m_interval;													// 处理间隔时间
	int m_interval_elapse_time;

	bool m_has_time_limit;

	unsigned short m_param1;
	unsigned short m_param2;
	unsigned short m_param3;
	unsigned short m_param4;
};

#endif


