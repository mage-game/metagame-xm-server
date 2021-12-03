#ifndef _MONSTERAIGUILDFB_ENEMY_HPP_
#define _MONSTERAIGUILDFB_ENEMY_HPP_

#include "ai/monsterai/monsteraimovearound.hpp"

class MonsterAIGuildFbEnemy : public MonsterAIMoveAround
{
public:
	MonsterAIGuildFbEnemy(Character *me);
	virtual ~MonsterAIGuildFbEnemy();

	virtual void Init(const MonsterAIParam &mp);

	virtual void OnEnterScene();
	virtual void OnLeaveScene();
	virtual void OnAOIEnter(ObjID obj_id);
	virtual void OnAOILeave(ObjID obj_id);

	virtual void AI(unsigned long interval);

	void *operator new(size_t c);
	void operator delete(void *m);

protected:
	virtual bool IsEnemy(Obj *obj);
	virtual bool IsFriend(Obj *obj, bool check_alive = true);

	// 这里必须去掉仇恨驱动，不然怪物的仇恨驱动设计，会导致怪重新选中人作为目标
	// 因为 仇恨驱动只保存了角色的仇恨
	virtual bool IsHateDrive() { return false; }

	void MoveAOI();

	virtual bool IsAttackWhenOnAttack(ObjID attacker);

	bool FindEnemy(unsigned long interval = 0);

	ObjID m_guardboss_objid;

	UInt32 m_aoi_handle;
	unsigned int m_aoi_range;
	Posi m_aoi_setting_point;

	const static unsigned int MAX_ENEMY_NUM	= 8;
	ObjID m_enemy_list[MAX_ENEMY_NUM];
	unsigned int m_enemy_size;
};

#endif


