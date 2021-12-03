#ifndef __MONSTER_AI_CROSS_PASTURE_ANIMAL_HPP__
#define __MONSTER_AI_CROSS_PASTURE_ANIMAL_HPP__

#include "ai/monsterai/monsteraibase.hpp"
#include <map>

const static int CROSS_PASTURE_ANIMAL_HATE_MAX_ATTACK_TIMES = 5;			// �������ﱻ�������蹥������
const static int CROSS_PASTURE_ANIMAL_DECROLE_SPEED_TIME_S = 5;				// ���ٳ���ʱ��
const static int CROSS_PASTURE_ANIMAL_CHECK_SPEED_TIME_S = 3;				// �����˼���buffʱ��

/********************************************************
 * �����������AI��
 * 1 ������5�κ����������������棩
 * 2 ��Ҹ��˹�����볬��׷�ٷ�Χ��ȡ������
 ********************************************************/
class MonsterAICrossPastureAnimal : public  MonsterAIBase
{
public:
	MonsterAICrossPastureAnimal(Character *me);
	virtual ~MonsterAICrossPastureAnimal();

	virtual void OnEnterScene() {}
	virtual void OnLeaveScene() {}
	virtual void OnAOIEnter(ObjID obj_id) {}
	virtual void OnAOILeave(ObjID obj_id) {}
	virtual void OnDie(ObjID killer);

	virtual void OnRoleDie(ObjID obj_id);
	virtual void OnAttack(ObjID attacker, UInt16 skill_id, Attribute delta_hp);

	virtual void ForceSetEnemy(ObjID obj_id);

	virtual void AI(unsigned long interval);

	void *operator new(size_t c);
	void operator delete(void *m);

protected:
	virtual bool IsAttackWhenOnAttack(ObjID attacker) { return false; }
	virtual bool IsOutRange();

	void DecEnemySpeed(ObjID enemy_id = INVALID_OBJ_ID);
	void CheckAndRemoveEnemyBuff();
	void NoticEnemy(Role *role, int reason, GameName robber);

	std::map<ObjID, int> m_attacker_times_map;// �����߹����������Դ˼��㹥��Ŀ�꣩
	int check_enemy_speed_time_interval;
};
#endif

