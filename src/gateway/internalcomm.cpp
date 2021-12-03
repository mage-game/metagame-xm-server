
#include "common/md5/md5cpp.h"
#include "internalcomm.h"

void InternalComm::Init(INetworkModule *internal_network)
{
	m_internal_network = internal_network;
}



