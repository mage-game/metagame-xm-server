#ifndef __MONSTERAISTATIC_HPP__
#define __MONSTERAISTATIC_HPP__

#include "monsteraibase.hpp"

// ��ֹ�Ĺ����ֹ������������ �ỹ�� ��׷��
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

