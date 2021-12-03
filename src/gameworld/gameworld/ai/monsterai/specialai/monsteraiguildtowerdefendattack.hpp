#ifndef _MONSTERAIGUILDTOWERDEFENDATTACK_HPP_
#define _MONSTERAIGUILDTOWERDEFENDATTACK_HPP_

#include "ai/monsterai/monsteraibase.hpp"

class MonsterAIGuildTowerDefendAttack : public MonsterAIBase
{
public:
	MonsterAIGuildTowerDefendAttack(Character *me);
	virtual ~MonsterAIGuildTowerDefendAttack();

	virtual void Init(const MonsterAIParam &mp);

	virtual void OnEnterScene();
	virtual void OnLeaveScene();
	virtual void OnAOIEnter(ObjID obj_id);
	virtual void OnAOILeave(ObjID obj_id);

	virtual void AI(unsigned long interval);

	void *operator new(size_t c);
	void operator delete(void *m);

	void SetTowerDefendTargetPos(const Posi &pos) { m_target_pos = pos; }

protected:
	virtual bool IsEnemy(Obj *obj);
	virtual bool IsFriend(Obj *obj, bool check_alive = true);

	virtual bool IsAttackWhenOnAttack(ObjID attacker);

	bool FindEnemy(unsigned long interval);

	void MoveAOI();
	void GotoTargetPosi();

	UInt32 m_aoi_handle;
	unsigned int m_aoi_range;
	Posi m_aoi_setting_point;

	const static unsigned int MAX_ENEMY_NUM	= 8;
	ObjID m_enemy_list[MAX_ENEMY_NUM];
	unsigned int m_enemy_size;

	int m_towerdefend_level;								// ¹Ø¿¨
	Posi m_target_pos;
};


#endif
