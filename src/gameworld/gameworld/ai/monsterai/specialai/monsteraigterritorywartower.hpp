#ifndef __MONSTER_AI_TERRITORYWAR_TOWER_HPP__
#define __MONSTER_AI_TERRITORYWAR_TOWER_HPP__

#include "ai/monsterai/monsteraistaticinitiative.hpp"

class MonsterAITerritoryWarTower : public MonsterAIStaticInitiative
{
public:
	MonsterAITerritoryWarTower(Character *me);
	virtual ~MonsterAITerritoryWarTower();

	virtual void Init(const MonsterAIParam &mp);
	virtual void AI(unsigned long interval);

	void *operator new(size_t c);
	void operator delete(void *m);

	void SetSide(short side) { m_side = side; }
	int GetSide() { return m_side; }

protected:
	virtual bool IsEnemy(Obj *obj);
	virtual bool IsFriend(Obj *obj, bool check_alive = true);
	
	short m_side;
};

#endif // __MONSTER_AI_TERRITORYWAR_TOWER_HPP__


