#ifndef __MonsterAIJUSTRUN_HPP__
#define __MonsterAIJUSTRUN_HPP__

#include "monsteraimovearound.hpp"

// 乱跑的怪物：巡逻，不攻击不还手
class MonsterAIJustRun : public  MonsterAIMoveAround
{
public:
	MonsterAIJustRun(Character *me);
	virtual ~MonsterAIJustRun();

	virtual void OnEnterScene() {}
	virtual void OnLeaveScene() {}
	virtual void OnAOIEnter(ObjID obj_id) {}
	virtual void OnAOILeave(ObjID obj_id) {}

	virtual void AI(unsigned long interval);

	void *operator new(size_t c);
	void operator delete(void *m);

protected:
	virtual bool IsAttackWhenOnAttack(ObjID attacker) { return false; }
};
#endif

