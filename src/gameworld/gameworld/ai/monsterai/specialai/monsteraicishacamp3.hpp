#ifndef __MONSTER_AI_CISHA_CAMP3_HPP__
#define __MONSTER_AI_CISHA_CAMP3_HPP__

#include "ai/monsterai/monsteraimovearound.hpp"

class MonsterAiCishaCamp3 : public  MonsterAIMoveAround
{
public:
	MonsterAiCishaCamp3(Character *me);
	virtual ~MonsterAiCishaCamp3();

	virtual void AI(unsigned long interval);
	virtual void Init(const MonsterAIParam &mp);

	void *operator new(size_t c);
	void operator delete(void *m);

protected:
	virtual bool IsEnemy(Obj *obj);
	virtual bool IsFriend(Obj *obj, bool check_alive = true);
	bool FindEnemy();

	virtual bool IsAttackWhenOnAttack(ObjID attacker);
};

#endif // __MONSTER_AI_CISHA_CAMP3_HPP__


