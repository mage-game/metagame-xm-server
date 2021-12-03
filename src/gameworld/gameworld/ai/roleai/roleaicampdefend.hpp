#ifndef __ROLEAI_CAPMDEFEND_HPP__
#define __ROLEAI_CAPMDEFEND_HPP__

#include "roleaibase.hpp"

//  ÿŒ¿µÒœÒ
class RoleAICampDefend : public RoleAIBase
{
public:
	RoleAICampDefend(Character *me);
	virtual ~RoleAICampDefend();

	virtual void OnAttack(ObjID attacker, UInt16 skill_id, Attribute delta_hp);

	virtual void AI(unsigned long interval);

	void *operator new(size_t c);
	void operator delete(void *m);

protected:

	virtual bool IsEnemy(Obj *obj) { return false; }
};

#endif

