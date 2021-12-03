#ifndef _MONSTERAIGUILDFB_GUARD_HPP_
#define _MONSTERAIGUILDFB_GUARD_HPP_

#include "ai/monsterai/monsteraibase.hpp"

class MonsterAIGuildFbGuard : public MonsterAIBase
{
public:
	MonsterAIGuildFbGuard(Character *me);
	virtual ~MonsterAIGuildFbGuard();

	virtual void Init(const MonsterAIParam &mp);

	virtual void OnEnterScene();
	virtual void OnLeaveScene();
	virtual void OnAOIEnter(ObjID obj_id);
	virtual void OnAOILeave(ObjID obj_id);

	virtual void AI(unsigned long interval);

	void *operator new(size_t c);
	void operator delete(void *m);

	void SetGuardBossTargetPos(const Posi &pos) { m_target_pos = pos; }

protected:
	virtual bool IsEnemy(Obj *obj);
	virtual bool IsFriend(Obj *obj, bool check_alive = true);

	virtual bool IsAttackWhenOnAttack(ObjID attacker);

	bool FindEnemy(unsigned long interval = 0);

	void MoveAOI();
	void GotoTargetPosi();

	UInt32 m_aoi_handle;
	unsigned int m_aoi_range;
	Posi m_aoi_setting_point;

	const static unsigned int MAX_ENEMY_NUM	= 8;
	ObjID m_enemy_list[MAX_ENEMY_NUM];
	unsigned int m_enemy_size;

	Posi m_target_pos;
};


#endif

// _MONSTERAIGUILDFB_GUARD_HPP_