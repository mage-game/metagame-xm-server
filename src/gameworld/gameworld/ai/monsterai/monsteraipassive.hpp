#ifndef __MONSTERAIPASSIVE_HPP__
#define __MONSTERAIPASSIVE_HPP__

#include "monsteraimovearound.hpp"

// 巡逻、不主动攻击 会还手 会追踪
class MonsterAIPassive : public  MonsterAIMoveAround
{
public:
	MonsterAIPassive(Character *me);
	virtual ~MonsterAIPassive();

	virtual void AI(unsigned long interval);

	void *operator new(size_t c);
	void operator delete(void *m);

protected:
	bool FindEnemy();

	virtual bool IsAttackWhenOnAttack(ObjID attacker);
};

#endif // __MONSTERAIPASSIVE_HPP__


