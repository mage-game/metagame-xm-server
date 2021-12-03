#include "common/md5/md5cpp.h"
#include "internalcomm.h"

void InternalComm::SetDBSession(const rmi::Session &accounter, rmi::Session *role_db, unsigned int role_db_num, const rmi::Session &global_session, 
								const rmi::Session &system_session, const rmi::Session &name_session, const rmi::Session &cross_session)
{
	m_db_handler.Init(accounter, role_db, role_db_num, global_session, system_session, name_session, cross_session);
}

