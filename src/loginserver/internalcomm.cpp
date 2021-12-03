#include "common/md5/md5cpp.h"
#include "internalcomm.h"
#include <sstream>

void InternalComm::Init(INetworkModule *internal_network, const std::string &session_key_base)
{
	m_internal_network = internal_network;
	MD5 md5;
	MD5_buffer(&md5, session_key_base.c_str(), (unsigned long)session_key_base.size());
	MD52String(&m_session_key_base, md5);
}

void InternalComm::SetDBSession(const rmi::Session &accounter, rmi::Session *role_db, unsigned int role_db_num, const rmi::Session &global_session, 
								const rmi::Session &system_session, const rmi::Session &name_session)
{
	m_db_handler.Init(accounter, role_db, role_db_num, global_session, system_session, name_session);
}
