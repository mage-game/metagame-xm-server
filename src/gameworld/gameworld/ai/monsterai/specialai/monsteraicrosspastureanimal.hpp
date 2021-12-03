#ifndef __MONSTER_AI_CROSS_PASTURE_ANIMAL_HPP__
#define __MONSTER_AI_CROSS_PASTURE_ANIMAL_HPP__

#include "ai/monsterai/monsteraibase.hpp"
#include <map>

const static int CROSS_PASTURE_ANIMAL_HATE_MAX_ATTACK_TIMES = 5;			// 牧场怪物被吸引所需攻击次数
const static int CROSS_PASTURE_ANIMAL_DECROLE_SPEED_TIME_S = 5;				// 减速持续时间
const static int CROSS_PASTURE_ANIMAL_CHECK_SPEED_TIME_S = 3;				// 检查敌人减速buff时间

/********************************************************
 * 跨服牧场怪物AI：
 * 1 被攻击5次后吸引到攻击（跟随）
 * 2 玩家跟此怪物距离超过追踪范围则取消跟随
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

	std::map<ObjID, int> m_attacker_times_map;// 攻击者攻击次数（以此计算攻击目标）
	int check_enemy_speed_time_interval;
};
#endif

