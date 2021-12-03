#ifndef __OBSERITEM_H__
#define __OBSERITEM_H__

#include <stdlib.h>

#include "gamedef.h"

class ObserItem
{
public:
	ObserItem()
		: obj_id(INVALID_OBJ_ID), 
		pos(0, 0),
		be_obs_local(-2,-2), 
		be_obs_index(-1)
	{

	}

	~ObserItem() {}

	ObjID	obj_id;
	Posi	pos;			// 所处位置

	Posu	be_obs_local;	// 被观察区域
	UInt32	be_obs_index;	// 处在被观察区域的索引

	void *operator new(size_t c);
	void operator delete(void *m);
};

#endif

