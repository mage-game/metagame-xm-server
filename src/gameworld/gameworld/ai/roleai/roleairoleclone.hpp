#ifndef __ROLEAI_ROLECLONE_HPP__
#define __ROLEAI_ROLECLONE_HPP__

#include "roleaibase.hpp"

// ½ÇÉ«·ÖÉí
class RoleAIRoleClone : public RoleAIBase
{
public:
	RoleAIRoleClone(Character *me);
	virtual ~RoleAIRoleClone();

	virtual void AI(unsigned long interval);

	void *operator new(size_t c);
	void operator delete(void *m);

protected:

	virtual bool IsEnemy(Obj *obj);
};

#endif

