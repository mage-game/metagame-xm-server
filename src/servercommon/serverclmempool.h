#ifndef SERVERCLMEMPOOL_H
#define SERVERCLMEMPOOL_H

#include <stdlib.h>

class ServerMsgMem
{
public:
	void *operator new[](size_t c);
	void operator delete[](void *m);
};

#endif
