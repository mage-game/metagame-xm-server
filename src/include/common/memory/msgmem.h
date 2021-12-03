
#ifndef MSGMEN_H
#define MSGMEN_H

#include <stdio.h>

class MsgMem
{
public:
	void *operator new[](size_t c);
	void operator delete[](void *m);
};

#endif


