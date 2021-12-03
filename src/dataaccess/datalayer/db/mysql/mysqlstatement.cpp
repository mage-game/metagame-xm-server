#include "db/connection.h"
#include "mysqlstatement.h"
#include "mysqlresult.h"

#include "my_global.h"
#include "mysql.h"

#include "common/memory/clmempool.h"

//////////////////////////////////////////////////////////////////////////
// ÄÚ´æ³Ø
//////////////////////////////////////////////////////////////////////////

namespace msmempool
{
	class MsgMemPool: public CLMemPool 
	{
	public:
		MsgMemPool():CLMemPool("DBStmtMsgMem")
		{
			unsigned int length_list[] = {16, 32, 64, 128, 256, 512, 2048, 4096};
			this->Init(length_list, 8, 32);
		}
		virtual ~MsgMemPool()
		{
		}
	};

	MsgMemPool g_msg_mem_pool;
}

//////////////////////////////////////////////////////////////////////////



MysqlStatement::MysqlStatement()
{
	m_conn = 0;
	m_stmt = 0;
	m_params = 0;
	m_paramcount = 0;
}

//MysqlStatement::MysqlStatement(IConnection* conn)
//{
//	m_conn = conn;
//	m_sqlstr = 0;
//	m_stmt = 0;
//	m_params = 0;
//	m_paramcount = 0;
//}

MysqlStatement::~MysqlStatement()
{
	close();
	m_conn = 0;
}

void MysqlStatement::setConnection(IConnection *conn)
{
	m_conn = conn;
}

IConnection* MysqlStatement::getConnection()
{
	return m_conn;
}

void* MysqlStatement::getStmt()
{
	return (void*)m_stmt;
}

long long MysqlStatement::getInsertID()
{
	if(0 != m_stmt)
	{
		return mysql_stmt_insert_id(m_stmt);
	}
	if(0 != m_conn)
	{
		return m_conn->getInsertID();
	}
	return -1;
}
bool MysqlStatement::prepare(const char* sql)
{
	if (0 != m_stmt)
	{
		close();
	}

	if(0 == m_conn)
	{
		return false;
	}

	MYSQL* tmp = (MYSQL*)(m_conn->getconn());
	if(0 == tmp)
	{
		return false;
	}

	m_stmt = mysql_stmt_init(tmp);
	if(0 == m_stmt)
	{
		return false;
	}

	int ret = mysql_stmt_prepare(m_stmt, sql ,(unsigned long)strlen(sql));
	if(0 != ret)
	{
		close();
		return false;
	}

	initParams();
	return true;
}
void MysqlStatement::clearParameters()
{
	destroyParams();
}
void MysqlStatement::close()
{
	destroyParams();
	if (0 != m_stmt)
	{
		mysql_stmt_close(m_stmt);
		m_stmt = 0;
	}
	m_paramcount = 0;
}

bool MysqlStatement::setByte(int paramIndex, char value)
{
	if(!validIndex(paramIndex))
	{
		return false;
	}
	MYSQL_BIND *bind = &m_params[paramIndex];
	bind->buffer_type = MYSQL_TYPE_TINY;
	//bind->buffer = malloc(sizeof(char));
	bind->buffer = msmempool::g_msg_mem_pool.Alloc(sizeof(char));
	*((unsigned char*)bind->buffer) = (unsigned char)value;
	bind->length = 0;
	return true;
}
//bool MysqlStatement::setBytes(int paramIndex, const char* value, int length)
//{
//
//	if(!validIndex(paramIndex))
//	{
//		return false;
//	}
//	MYSQL_BIND* bind = &m_params[paramIndex];
//	bind->buffer_type = MYSQL_TYPE_BLOB;
//	bind->buffer_length = length;
//	//bind->buffer = malloc(length);
//	bind->buffer = msmempool::g_msg_mem_pool.Alloc(length);
//	memcpy(bind->buffer,value,length);
//	bind->length = 0;
//	//if(0 != mysql_stmt_send_long_data(m_stmt,0,value,length))
//	//{
//	//	return false;
//	//}
//	return true;
//}

bool MysqlStatement::setBoolean(int paramIndex, bool value)
{
	return setByte(paramIndex, (char)value);
}

bool MysqlStatement::setInt32(int paramIndex, int value)
{
	if(!validIndex(paramIndex))
	{
		return false;
	}
	MYSQL_BIND* bind = &m_params[paramIndex];
	bind->buffer_type = MYSQL_TYPE_LONG;
	//bind->buffer = malloc(sizeof(int));
	bind->buffer = msmempool::g_msg_mem_pool.Alloc(sizeof(int));
	*(int*)bind->buffer = value;
	bind->length = 0;
	return true;
}
bool MysqlStatement::setDouble(int paramIndex, double value)
{
	if(!validIndex(paramIndex))
	{
		return false;
	}
	MYSQL_BIND* bind = &m_params[paramIndex];
	bind->buffer_type = MYSQL_TYPE_DOUBLE;
	//bind->buffer = malloc(sizeof(double));
	bind->buffer = msmempool::g_msg_mem_pool.Alloc(sizeof(double));
	*(double*)bind->buffer = value;
	bind->length = 0;
	return true;
}
bool MysqlStatement::setLong64(int paramIndex, long long value )
{
	if(!validIndex(paramIndex))
	{
		return false;
	}
	MYSQL_BIND* bind = &m_params[paramIndex];
	bind->buffer_type = MYSQL_TYPE_LONGLONG;
	//bind->buffer = malloc(sizeof(long long));
	bind->buffer = msmempool::g_msg_mem_pool.Alloc(sizeof(long long));
	*(long long*)bind->buffer = value;
	bind->length = 0;
	return true;
}

bool MysqlStatement::setString(int paramIndex, const char* value, unsigned long length)
{
	if(!validIndex(paramIndex))
	{
		return false;
	}
	MYSQL_BIND* bind = &m_params[paramIndex];
	bind->buffer_type = MYSQL_TYPE_VAR_STRING;
	bind->buffer_length = length;
	//bind->buffer = malloc(length);
	bind->buffer = msmempool::g_msg_mem_pool.Alloc(length);
	memcpy(bind->buffer, value, length);
	bind->length = 0;
	return true;
}

bool MysqlStatement::setDate(int paramIndex, const tm& value)
{
	if(!validIndex(paramIndex))
	{
		return false;
	}
	MYSQL_BIND* bind = &m_params[paramIndex];
	bind->buffer_type = MYSQL_TYPE_DATETIME;
	//bind->buffer = malloc(sizeof(MYSQL_TIME));
	bind->buffer = msmempool::g_msg_mem_pool.Alloc(sizeof(MYSQL_TIME));

	memset(bind->buffer,0,sizeof(MYSQL_TIME));
	((MYSQL_TIME*)bind->buffer)->year = value.tm_year;
	((MYSQL_TIME*)bind->buffer)->month = value.tm_mon;
	((MYSQL_TIME*)bind->buffer)->day = value.tm_mday;
	((MYSQL_TIME*)bind->buffer)->hour = value.tm_hour;
	((MYSQL_TIME*)bind->buffer)->minute = value.tm_min;
	((MYSQL_TIME*)bind->buffer)->second = value.tm_sec;

	bind->length = 0;
	return true;
}

bool MysqlStatement::setTime(int paramIndex, const tm& value)
{
	if(!validIndex(paramIndex))
	{
		return false;
	}
	MYSQL_BIND* bind = &m_params[paramIndex];
	bind->buffer_type = MYSQL_TYPE_TIME;
	//bind->buffer = malloc(sizeof(MYSQL_TIME));
	bind->buffer = msmempool::g_msg_mem_pool.Alloc(sizeof(MYSQL_TIME));

	memset(bind->buffer,0,sizeof(MYSQL_TIME));
	((MYSQL_TIME*)bind->buffer)->year = 0;//value.tm_year;
	((MYSQL_TIME*)bind->buffer)->month = 0;//value.tm_mon;
	((MYSQL_TIME*)bind->buffer)->day = 0;//value..tm_mday;
	((MYSQL_TIME*)bind->buffer)->hour = value.tm_hour;
	((MYSQL_TIME*)bind->buffer)->minute = value.tm_min;
	((MYSQL_TIME*)bind->buffer)->second = value.tm_sec;

	bind->length = 0;
	return true;
}

bool MysqlStatement::setTimestamp(int paramIndex, const tm& value)
{
	if(!validIndex(paramIndex))
	{
		return false;
	}
	MYSQL_BIND* bind = &m_params[paramIndex];
	bind->buffer_type = MYSQL_TYPE_TIMESTAMP;
	//bind->buffer = malloc(sizeof(MYSQL_TIME));
	bind->buffer = msmempool::g_msg_mem_pool.Alloc(sizeof(MYSQL_TIME));

	memset(bind->buffer,0,sizeof(MYSQL_TIME));
	((MYSQL_TIME*)bind->buffer)->year = value.tm_year;
	((MYSQL_TIME*)bind->buffer)->month = value.tm_mon;
	((MYSQL_TIME*)bind->buffer)->day = value.tm_mday;
	((MYSQL_TIME*)bind->buffer)->hour = value.tm_hour;
	((MYSQL_TIME*)bind->buffer)->minute = value.tm_min;
	((MYSQL_TIME*)bind->buffer)->second = value.tm_sec;

	bind->length = 0;
	return true;
}

int MysqlStatement::execute( )
{
	if (0 == m_stmt || 0 != mysql_stmt_bind_param(m_stmt,m_params) || 0 != mysql_stmt_execute(m_stmt))
	{
		const char* err = mysql_error(((MYSQL*)(m_conn->getconn())));
		if (err)
		{
			printf(err);
		}
		//if(mysql_errno((MYSQL*)(m_conn->getconn()))) // mysql_fetch_row() failed due to an error
		//{
		//	// printf("Error: %s\n", mysql_error((MYSQL*)(m_conn->getconn())));
		//	printf("execute Error: \n");
		//}

		if (0 == m_stmt)
		{
			printf("0 == m_stmt Error: \n");
		}

		return -1;
	}

	// unsigned long long rowscount = mysql_stmt_affected_rows(m_stmt);

	return (int)(mysql_stmt_affected_rows(m_stmt));
}

int MysqlStatement::execute(const char* sql)
{
	close();

	if(0 == m_conn || 0 != mysql_real_query((MYSQL*)(m_conn->getconn()),sql,(unsigned long)strlen(sql)))
	{
		return -1;
	}

	return (int)(mysql_affected_rows((MYSQL*)(m_conn->getconn())));
}

//int MysqlStatement::executeUpdate()
//{
//	if(0 == m_stmt || 0 != mysql_stmt_bind_param(m_stmt,m_params) || 0 != mysql_stmt_execute(m_stmt))
//	{
//		return -1;
//	}
//	return (int)mysql_stmt_affected_rows(m_stmt);
//}
//
//int MysqlStatement::executeUpdate(const char* sql)
//{
//	close();
//	if(0 == m_conn || 0 != mysql_real_query((MYSQL*)(m_conn->getconn()), sql, (unsigned long)strlen(sql)))
//	{
//		return -1;
//	}
//	return (int)mysql_affected_rows((MYSQL*)(m_conn->getconn()));
//}

IResultset* MysqlStatement::executeQuery()
{
	if(0 == m_stmt || 0 != mysql_stmt_bind_param(m_stmt,m_params) || 0 != mysql_stmt_execute(m_stmt))
	{
		return 0;
	}

	IResultset* result = new MysqlResult();
	result->setStmt(this);
	result->init();
	return result;
}

IResultset* MysqlStatement::executeQuery(const char* sql)
{
	prepare( sql );
	return executeQuery();
}

bool MysqlStatement::validIndex(int index)
{
	if (index < 0 || index>=m_paramcount)
	{
		return false;
	}
	cleanParam(&m_params[index]);
	return true;
}

void MysqlStatement::initParams()
{
	destroyParams();
	m_paramcount = mysql_stmt_param_count(m_stmt);
	if (m_paramcount > 0)
	{
		m_params = new MYSQL_BIND[m_paramcount];
		memset(m_params,0,sizeof(MYSQL_BIND)*m_paramcount);
	}
}

void MysqlStatement::cleanParam(MYSQL_BIND* param)
{
	if (0 != param->buffer )
	{
		//free(param->buffer);
		msmempool::g_msg_mem_pool.Free(param->buffer);
		param->buffer = 0;
	}
	if (0 != param->length )
	{
		delete param->length;
		param->length = 0;
	}
	if (0 != param->is_null )
	{
		delete param->is_null;
		param->is_null = 0;
	}
}

void MysqlStatement::destroyParams()
{
	if (0 != m_params)
	{
		for (int i = 0; i<m_paramcount; i++)
		{
			cleanParam(&m_params[i]);
		}
		delete[] m_params;
	}
	m_paramcount = 0;
	m_params = 0;
}

