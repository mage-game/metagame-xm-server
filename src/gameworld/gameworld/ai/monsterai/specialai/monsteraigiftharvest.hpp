#ifndef __MONSTER_AI_GIFT_HARVEST_HPP__
#define __MONSTER_AI_GIFT_HARVEST_HPP__

#include "ai/monsterai/monsteraibase.hpp"
#include <map>

const static int GIFT_HARVEST_MONSTER_HATE_MAX_ATTACK_TIMES = 5;			// ���ﱻ�������蹥������

/********************************************************
 * �����ո����AI��
 * 1 ������5�κ����������������棩
 * 2 ��Ҹ��˹�����볬��׷�ٷ�Χ��ȡ������
 ********************************************************/
class MonsterAIGiftHarvest : public  MonsterAIBase
{
public:
	MonsterAIGiftHarvest(Character *me);
	virtual ~MonsterAIGiftHarvest();

	virtual void OnEnterScene() {}
	virtual void OnLeaveScene() {}
	virtual void OnAOIEnter(ObjID obj_id) {}
	virtual void OnAOILeave(ObjID obj_id) {}

	virtual void OnRoleDie(ObjID obj_id);
	virtual void OnAttack(ObjID attacker, UInt16 skill_id, Attribute delta_hp);

	virtual void AI(unsigned long interval);

	void *operator new(size_t c);
	void operator delete(void *m);

protected:
	virtual bool IsAttackWhenOnAttack(ObjID attacker) { return false; }
	virtual bool IsOutRange();

	std::map<ObjID, int> m_attacker_times_map;// �����߹����������Դ˼��㹥��Ŀ�꣩
};
#endif// __MONSTER_AI_GIFT_HARVEST_HPP__

