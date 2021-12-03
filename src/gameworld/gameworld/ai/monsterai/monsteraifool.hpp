#ifndef __MONSTERAIFOOL_HPP__
#define __MONSTERAIFOOL_HPP__

#include "monsteraibase.hpp"

// �޴��Ĺ������������
class MonsterAIFool : public MonsterAIBase
{
public:
	MonsterAIFool(Character *me);
	virtual ~MonsterAIFool();

	virtual void Init(const MonsterAIParam &mp);

	virtual void AI(unsigned long interval);

	void *operator new(size_t c);
	void operator delete(void *m);

protected:
	virtual bool IsAttackWhenOnAttack(ObjID attacker) { return false; }
};

#endif
