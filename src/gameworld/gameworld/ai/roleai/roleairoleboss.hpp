#ifndef __ROLEAI_ROLEBOSS_HPP__
#define __ROLEAI_ROLEBOSS_HPP__

#include "roleaibase.hpp"

// ½ÇÉ«boss
class RoleAIRoleBoss : public RoleAIBase
{
public:
	RoleAIRoleBoss(Character *me);
	virtual ~RoleAIRoleBoss();

	virtual void AI(unsigned long interval);

	void *operator new(size_t c);
	void operator delete(void *m);

protected:

	virtual bool IsEnemy(Obj *obj);

	void GoBack();
	bool IsOutRange();
};

#endif

