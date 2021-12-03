#ifndef __MONSTERAIJUSTFLEE_HPP__
#define __MONSTERAIJUSTFLEE_HPP__

#include "monsteraibase.hpp"

// 逃跑怪：被打不还手只会逃
class MonsterAIJustFlee : public  MonsterAIBase
{
public:
	MonsterAIJustFlee(Character *me);
	virtual ~MonsterAIJustFlee();

	virtual void OnEnterScene() {}
	virtual void OnLeaveScene() {}
	virtual void OnAOIEnter(ObjID obj_id) {}
	virtual void OnAOILeave(ObjID obj_id) {}

	virtual void AI(unsigned long interval);
	virtual void OnAttack(ObjID attacker, UInt16 skill_id, Attribute delta_hp);

	void *operator new(size_t c);
	void operator delete(void *m);

protected:
	virtual bool IsAttackWhenOnAttack(ObjID attacker) { return false; }
};
#endif

