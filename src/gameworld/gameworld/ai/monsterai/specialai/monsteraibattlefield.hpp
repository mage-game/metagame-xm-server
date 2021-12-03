#ifndef __MONSTER_AI_BATTLE_FIELD_HPP__
#define __MONSTER_AI_BATTLE_FIELD_HPP__

#include "ai/monsterai/monsteraistaticinitiative.hpp"

class MonsterAIBattleField : public MonsterAIStaticInitiative
{
public:
	MonsterAIBattleField(Character *me);
	virtual ~MonsterAIBattleField();

	virtual void Init(const MonsterAIParam &mp);
	virtual void AI(unsigned long interval);

	void *operator new(size_t c);
	void operator delete(void *m);

	void SetParams(int side, int gongji_delta_percent = 0) { m_battle_field_side = side, m_gongji_delta_percent = gongji_delta_percent; }
	int GetBattleFieldSide() { return m_battle_field_side; }

	void AddGongji();

protected:
	virtual bool IsEnemy(Obj *obj);
	virtual bool IsFriend(Obj *obj, bool check_alive = true);

	int m_battle_field_side;

	ObjID m_lastenemy_objid;												// ��󹥻��Ľ�ɫobjid
	Attribute m_gongji_total_delta;											// ���﹥���� �ܵĸı���
	int m_gongji_delta_percent;												// ����ÿ�ι��� ���������Ӱٷֱ�
};

#endif // __MONSTER_AI_BATTLE_FIELD_HPP__
