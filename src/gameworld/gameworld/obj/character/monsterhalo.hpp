#ifndef _MONSTER_HALO_HPP_
#define _MONSTER_HALO_HPP_

#include "character.h"

class Monster;

class MonsterHalo : public Character
{
public:
	MonsterHalo(ObjID owner_obj_id, int owner_key, UInt16 product_id, char product_method);
	virtual ~MonsterHalo();

	void Init(Attribute hp);

	virtual	void Update(unsigned long interval, time_t now_second, unsigned int now_dayid);

	void SetHaloInfo(int range, int interval, int count);
	void SetEffectParam(int effect_target, const SkillAttachEffectParam &effect_param);

	ObjID GetOwnerObjID() { return m_owner_obj_id; }
	int GetEffectType() { return m_effect_param.effect_type;}

	virtual bool CanBeCaptureBySkill() { return false; }

	void *	operator new(size_t c);
	void	operator delete(void *m);

protected:
	Monster * GetOwnerMonster();

	void Attach(const Posi &pos);

	UInt16 m_product_id;									// 产生此效果的技能id 场景buff类型
	char m_product_method;									// EffectBase::PRODUCT_METHOD

	int m_range;											// 作用范围
	int m_interval;											// 作用间隔
	int m_left_count;										// 作用剩余次数
	int m_past_count;										// 已经作用的次数

	int m_effect_target;									// 效果目标
	SkillAttachEffectParam m_effect_param;					// 效果 

	bool m_first;											// 标记第一次effect
	int m_effect_time;										// 已作用时间

	ObjID m_owner_obj_id;									// 所有者
	int m_owner_key;										
};

#endif
