
#include "serverclmempool.h"

#include "common/memory/clmempool.h"

namespace servermempool
{

	class ServerClMemPool: public CLMemPool 
	{
	public:
		ServerClMemPool():CLMemPool("ServerClMem")
		{
			unsigned int length_list[] = {32, 64, 128, 256, 512, 1024, 2048, 8192, 65536, 262144, 524288, 1048576};
			this->Init(length_list, 12, 24);
		}

		virtual ~ServerClMemPool()
		{
		}
	};
	ServerClMemPool g_server_mem_pool;
}


void * ServerMsgMem::operator new[](size_t c)
{
	return servermempool::g_server_mem_pool.Alloc((unsigned int)c);
}

void ServerMsgMem::operator delete[](void *m)
{
	servermempool::g_server_mem_pool.Free(m);
}

