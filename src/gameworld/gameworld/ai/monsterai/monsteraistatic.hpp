#ifndef __MONSTERAISTATIC_HPP__
#define __MONSTERAISTATIC_HPP__

#include "monsteraibase.hpp"

// 静止的怪物：静止、不主动攻击 会还手 会追踪
class MonsterAIStatic : public MonsterAIBase
{
public:
	MonsterAIStatic(Character *me);
	virtual ~MonsterAIStatic();

	virtual void AI(unsigned long interval);

	void *operator new(size_t c);
	void operator delete(void *m);

protected:
	void GoBack();
	virtual bool IsAttackWhenOnAttack(ObjID attacker);
};

#endif

