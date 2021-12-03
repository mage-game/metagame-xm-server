#ifndef __MONSTER_AI_BUILD_TOWER_HPP__
#define __MONSTER_AI_BUILD_TOWER_HPP__

#include "ai/monsterai/monsteraistaticinitiative.hpp"

class MonsterAIBuildTower : public MonsterAIStaticInitiative
{
public:
	MonsterAIBuildTower(Character *me);
	virtual ~MonsterAIBuildTower();

	virtual void Init(const MonsterAIParam &mp);
	virtual void AI(unsigned long interval);

	void *operator new(size_t c);
	void operator delete(void *m);

protected:
	virtual bool IsEnemy(Obj *obj);
	virtual bool IsFriend(Obj *obj, bool check_alive = true);

};

#endif // __MONSTER_AI_BUILD_TOWER_HPP__


