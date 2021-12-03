#ifndef __BATTLEFIELD_SHENSHI_HPP__
#define __BATTLEFIELD_SHENSHI_HPP__

#include "character.h"

class Role;

class BattleFieldShenShi: public Character
{
public:
	BattleFieldShenShi();
	virtual ~BattleFieldShenShi();

	void Init(Attribute hp);

	virtual	void Update(unsigned long interval, time_t now_second, unsigned int now_dayid);

	virtual bool StartAttack(ObjID obj_id) { return false; }
	virtual bool PerformSkill(int skill_index, const Posi &pos, ObjID target_id) { return false; }

	virtual bool CanBeCaptureBySkill() { return false; }

	virtual void OnAttack(ObjID attacker, UInt16 skill_id, Attribute &delta_hp, bool from_skill);
	virtual void OnDie(ObjID killer);

	void *	operator new(size_t c);
	void	operator delete(void *m);

	const UserID & GetOwnerUserId() { return m_owner_user_id; } 
	ObjID GetOwnerObjId() { return m_owner_obj_id; }
	bool HaveOwner() { return INVALID_OBJ_ID != m_owner_obj_id && INVALID_USER_ID != m_owner_user_id; }
	void SetOwner(ObjID owner_objid, const UserID &owner_user_id);

	virtual bool IsFixHurtOnAttack() { return true; }

protected:
	ObjID m_owner_obj_id;								// 主人对象ID
	UserID m_owner_user_id;								// 主人角色ID
};

#endif // __BATTLEFIELD_SHENSHI_HPP__

