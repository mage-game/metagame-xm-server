#ifndef __ROLEAI_CHALLENGEFILE_HPP__
#define __ROLEAI_CHALLENGEFILE_HPP__

#include "roleaibase.hpp"

// 竞技场机器人
class RoleAIChallengefile : public RoleAIBase
{
public:
	RoleAIChallengefile(Character *me);
	virtual ~RoleAIChallengefile();

	virtual void AI(unsigned long interval);

	void *operator new(size_t c);
	void operator delete(void *m);

	void SetWaitMs(unsigned long interval) { m_wailt_ms = interval; }

protected:

	unsigned long m_interval;
	unsigned long m_wailt_ms;
	virtual bool IsEnemy(Obj *obj) { return true; }
};

#endif

