#ifndef __MONSTERAITOWERDEFENDSHOUHU_HPP__
#define __MONSTERAITOWERDEFENDSHOUHU_HPP__

#include "ai/monsterai/monsteraistaticinitiative.hpp"


class MonsterAITowerDefendShouhu : public MonsterAIStaticInitiative
{
public:
	MonsterAITowerDefendShouhu(Character *me);
	virtual ~MonsterAITowerDefendShouhu();

	virtual void Init(const MonsterAIParam &mp);

	virtual void AI(unsigned long interval);

protected:
	virtual bool IsEnemy(Obj *obj);

	virtual bool IsFriend(Obj *obj, bool check_alive = true);
};

#endif

