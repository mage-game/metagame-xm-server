#ifndef __MONSTER_AI_RUNNING_MONSTER_HPP__
#define __MONSTER_AI_RUNNING_MONSTER_HPP__

#include "ai/monsterai/monsteraimovearound.hpp"

static const int MAX_RUNNING_MONSTER_AI_RUNNING_STEP = 20;

class MonsterAIRunningMonster : public MonsterAIMoveAround
{
public:
	MonsterAIRunningMonster(Character *me);
	virtual ~MonsterAIRunningMonster();

	virtual void Init(const MonsterAIParam &mp);
	virtual void AI(unsigned long interval);

	void SetWalkPosList(int pos_count, Posi walk_pos_list[MAX_RUNNING_MONSTER_AI_RUNNING_STEP]);
	bool IsAtWalkEndPos();

	void *operator new(size_t c);
	void operator delete(void *m);

protected:
	void MoveAround(unsigned long interval);

	virtual bool IsAttackWhenOnAttack(ObjID attacker) { return false; }
	virtual bool IsHateDrive() { return false; }
};

#endif // __MONSTER_AI_RUNNING_MONSTER_HPP__


