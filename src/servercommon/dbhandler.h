#ifndef __DBHANDLER_H__
#define __DBHANDLER_H__

#include "irmimodule.h"

struct DBHandler
{
	static const int MAX_ROLE_DB_INDEX = 128;

	rmi::Session m_accounter;
	rmi::Session m_name;
	rmi::Session m_global;
	rmi::Session m_system;

	rmi::Session m_role[MAX_ROLE_DB_INDEX];
	unsigned int m_role_session_num;

	rmi::Session m_cross;

	void Init(const rmi::Session &accounter, rmi::Session *role_db, unsigned int role_db_num, const rmi::Session &global_session, 
		const rmi::Session &system_session, const rmi::Session &name_session, const rmi::Session &cross_session = rmi::Session())
	{
		m_accounter = accounter;

		for (unsigned int i = 0; i < (unsigned int)MAX_ROLE_DB_INDEX && i < role_db_num; ++i)
		{
			m_role[i] = role_db[i];
		}

		m_role_session_num = role_db_num;
		m_global = global_session;
		m_system = system_session;
		m_name = name_session;

		m_cross = cross_session;
	}

	const rmi::Session &GetAcounterDB() const{ return m_accounter; }
	const rmi::Session &GetGlobalDB() const { return m_global; }
	const rmi::Session &GetSystemDB() const { return m_system; }
	const rmi::Session &GetNameDB() const { return m_name; }
	const rmi::Session &GetRoleDB() const { return m_role[0]; }
	const rmi::Session &GetCrossDB() const { return m_cross; }
};

#endif


