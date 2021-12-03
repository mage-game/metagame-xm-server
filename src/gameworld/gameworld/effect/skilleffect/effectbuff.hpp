#ifndef __EFFECTBUFF_HPP__
#define __EFFECTBUFF_HPP__

#include <stdlib.h>

#include "effect/effectbase.hpp"

#include "obj/character/attribute.hpp"

// 修改属性类，可同时最多改3种
class EffectBuff : public EffectBase
{
public:
	EffectBuff();
	EffectBuff(ObjID deliverer, UInt16 skill, int effect_time, char product_level, char product_method = EffectBase::PRODUCT_METHOD_SKILL, unsigned char buff_type = EBT_INVALID);
	virtual ~EffectBuff();

	virtual char GetEffectType() { return EFFECT_TYPE_ATTRBUFF; }
	virtual void Effect(Character *cha, bool *die);
	virtual void ReEffect(Character *cha, bool *die);
	virtual bool Update(unsigned long interval, Character *cha, bool *die);
	virtual void ClearEffect(Character *cha);
	virtual unsigned char GetBuffType() const;

	virtual void SetLeftTimeMS(int effect_time) { if (effect_time >= 0) m_effect_time = effect_time; }
	virtual int GetLeftTimeMS() { return m_effect_time; }

	virtual void Merge(EffectBase *new_effect, Character *cha);

	virtual bool IsPositiveEffect() const;

	// 这里修改的是变化量

	/************************************************************************/
	/*  配合客户端显示的规则：
		正值，优先放在第一个属性，所有由百分比计算得来的数值都要把百分比填上
		参数2>0，增益，<0，减益
		参数3！=0，百分比，否则，数值										
		
		护送例外*/
	/************************************************************************/

	void AddAttrEffect(char attr_type, long long attr_value, int percent = 0);
	long long GetAttrValue(char attr_type);

	void AddGongJiEffect(Attribute gongji, short percent = 0);
	void AddFangYuEffect(Attribute fangyu, short percent = 0);
	void AddMingZhongEffect(Attribute mingzhong, short percent = 0);
	void AddShanBiEffect(Attribute shanbi, short percent = 0);
	void AddBaojiEffect(Attribute baoji, short percent = 0);
	void AddJianRenEffect(Attribute jianren, short percent = 0);
	void AddJingZhunPerEffect(Attribute jingzhun, short percent = 0);
	void AddPerBaoJiHurtEffect(Attribute baoji, short percent = 0);
	void AddMoveSpeedEffect(Attribute movespeed, short percent);	// 如果要看到速度特效，请用EffectMoveSpeed
	void AddGeDangEffect(Attribute gedang, short percent = 0);

	void SetRecoverHP() { m_recover_hp = true; }

	virtual void GetEffectParam(short *count, long long param[EFFECT_INFO_PARAM_MAX]);

	void *operator new(size_t c);
	void operator delete(void *m);

	virtual bool Serialize(TLVSerializer &s) const;
	virtual bool Unserialize(TLVUnserializer &s);

	virtual void Recalc(Character *chara);

protected:
	void Attach(Character *cha, bool is_attach);

	int m_effect_time;												// 作用时间

	static const int MAX_ATTR_COUNT = 3;

	int m_count;													// 作用属性数量
	CharIntAttrs::AttrPair m_pair_list[MAX_ATTR_COUNT];				// 属性数值列表
	int m_percent_list[MAX_ATTR_COUNT];								// 属性百分比列表

	bool m_recover_hp;												// 是否满血

	unsigned char m_buff_type;
};

#endif


