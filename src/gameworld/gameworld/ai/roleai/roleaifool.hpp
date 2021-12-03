#ifndef __ROLEAISTATIC_HPP__
#define __ROLEAISTATIC_HPP__

#include "roleaibase.hpp"

// ¾²Ö¹µÄ»úÆ÷ÈË£º¾²Ö¹
class RoleAIFool : public RoleAIBase
{
public:
	RoleAIFool(Character *me);
	virtual ~RoleAIFool();

	virtual void Init(const ChallengeParam &mp);

	virtual void AI(unsigned long interval);

	void *operator new(size_t c);
	void operator delete(void *m);

protected:

	virtual bool IsEnemy(Obj *obj) { return false; }
};

#endif

