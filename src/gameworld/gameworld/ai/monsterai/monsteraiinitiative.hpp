#ifndef __MONSTERAIINITIATIVE_HPP__
#define __MONSTERAIINITIATIVE_HPP__

#include "monsteraimovearound.hpp"

// 巡逻主动：巡逻、主动攻击
class MonsterAIInitiative : public MonsterAIMoveAround
{
public:
	MonsterAIInitiative(Character *me, int aitype = MonsterInitParam::AITYPE_INITIATIVE);
	virtual ~MonsterAIInitiative();

	virtual void Init(const MonsterAIParam &mp);

	virtual void OnEnterScene();
	virtual void OnLeaveScene();
	virtual void OnAOIEnter(ObjID obj_id);
	virtual void OnAOILeave(ObjID obj_id);

	virtual void AI(unsigned long interval);

	void *operator new(size_t c);
	void operator delete(void *m);

protected:
	bool FindEnemy(unsigned long interval = 0);
	virtual bool IsAttackWhenOnAttack(ObjID attacker);
	
	UInt32 m_aoi_handle;
	unsigned int m_aoi_range; // 巡逻范围，单位：格子

	const static unsigned int MAX_ENEMY_NUM	= 5;
	ObjID	m_enemy_list[MAX_ENEMY_NUM];
	unsigned int		m_enemy_size;
};

#endif

