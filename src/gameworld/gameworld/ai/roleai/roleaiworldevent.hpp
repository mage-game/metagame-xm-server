#ifndef __ROLEAI_WORLDEVENT_HPP__
#define __ROLEAI_WORLDEVENT_HPP__

#include "roleaibase.hpp"

// 世界事件机器人
class RoleAIWorldEvent : public RoleAIBase
{
public:
	RoleAIWorldEvent(Character *me);
	virtual ~RoleAIWorldEvent();

	virtual void AI(unsigned long interval);
	

	
	void *operator new(size_t c);
	void operator delete(void *m);

protected:

	virtual bool IsEnemy(Obj *obj);

	void GoBack();
	bool IsOutRange();

};

#endif

