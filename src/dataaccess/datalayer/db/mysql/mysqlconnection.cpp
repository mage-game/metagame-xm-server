#include "mysqlconnection.h"
#include "mysqlstatement.h"
#include "cache/cachetransaction.h"

#include "my_global.h"
#include "mysql.h"
#include <string.h>

MysqlConnection::MysqlConnection(const char *dbuser, const char *dbpw, const char *dbname, const char *dbhost, unsigned int dbport, const char* charset)
{
	strcpy(m_username, dbuser);
	strcpy(m_pw, dbpw);
	strcpy(m_database, dbname);
	strcpy(m_host, dbhost);
	m_port = dbport;
	strcpy(m_charset, charset);

	m_connected = false;
	m_autoCommit = true;
	m_begin = false;
	m_isCache = true;

	m_destroy = false;

	m_mysql = 0;
	m_transaction = new CacheTransaction();
}

MysqlConnection::~MysqlConnection()
{
	close();

	delete m_transaction;
	m_transaction = 0;
}

IConnection* MysqlConnection::clone()
{
	return new MysqlConnection(m_username, m_pw, m_database, m_host, m_port, m_charset);
}

bool MysqlConnection::connect()
{
	close();
	//int ret = 0;
	MYSQL* tmp = 0;
	m_mysql = mysql_init(0);
	tmp = mysql_real_connect(m_mysql, m_host, m_username, m_pw, m_database, m_port, 0, 0);
	if (0 == tmp)
	{
		close();
		return false;
	}
	/*ret = */mysql_set_character_set(m_mysql, m_charset);
	/*ret = */setAutoCommit(m_autoCommit);
	m_connected = true;
	return true;
}

bool MysqlConnection::reconnect()
{
	return connect();
}

bool MysqlConnection::isActive()
{
	if (0 != m_mysql && 0 == mysql_ping(m_mysql))
	{
		return true;
	}
	return false;
}

bool MysqlConnection::isBegin()
{
	return m_begin;
}

void MysqlConnection::close()
{
	if (0 != m_mysql)
	{
		mysql_close(m_mysql);
		m_mysql = 0;
	}
	m_connected = false;
	m_begin = false;
}

bool MysqlConnection::isClosed()
{
	return !m_connected;
}

bool MysqlConnection::setAutoCommit(bool AutoCommit)
{
	if(0 != m_mysql && 0 == mysql_autocommit(m_mysql, AutoCommit) )
	{
		m_autoCommit = AutoCommit;
		return true;
	}
	return false;
}

bool MysqlConnection::getAutoCommit()
{
	return m_autoCommit;
}

//void MysqlConnection::setIsCache(bool iscache)
//{
//	m_isCache = iscache;
//}

bool MysqlConnection::getIsCache()
{
	return m_isCache;
}

void* MysqlConnection::getconn()
{
	return (void*)m_mysql;
}

ITransaction* MysqlConnection::getTransaction()
{
	return m_transaction;
}

long long MysqlConnection::getInsertID()
{
	if(0 != m_mysql)
	{
		return mysql_insert_id(m_mysql);
	}
	return -1;
}


bool MysqlConnection::begin(bool iscache)
{
	if (!m_begin && 0 != m_mysql && 0 == mysql_real_query(m_mysql, "begin", sizeof("begin")))
	{
		m_begin = true;
		m_isCache = iscache;
		if (m_isCache)
		{
			m_transaction->begin();
		}
		return true;
	}
	return false;
}


bool MysqlConnection::commit()
{
	if (!m_begin)
	{
		return false;
	}
	if (0 != m_mysql && 0 == mysql_commit(m_mysql))
	{
		m_begin = false;
		if (m_isCache && m_transaction->isBegin())
		{
			m_transaction->commit();
		}
		return true;
	}
	else
	{
		m_destroy = true;  // 提交失败，则释放掉MysqlConnection对象，防止被再次使用
	}
	return false;
}

bool MysqlConnection::rollback()
{
	if (!m_begin)
	{
		return false;
	}
	if (0 != m_mysql && 0 == mysql_rollback(m_mysql))
	{
		m_begin = false;
		if (m_isCache && m_transaction->isBegin())
		{
			m_transaction->rollback();
		}
		return true;
	}
	else
	{
		m_destroy = true;  // 回滚失败，则释放掉MysqlConnection对象，防止被再次使用
	}
	return false;
}

IStatement* MysqlConnection::createStatement()
{
	IStatement* stmt = new MysqlStatement();
	stmt->setConnection(this);
	return stmt;
}

int MysqlConnection::getErrNo() const
{
	return mysql_errno(m_mysql);
}

const char* MysqlConnection::getError() const
{
	return mysql_error(m_mysql);
}



