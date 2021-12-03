

#include "connpool.h"
#include "db/connfactroy.h"
#include "db/connection.h"
#include <time.h>
#include <stdio.h>

#define DEFAULT_POOL_LEN	10

ConnPool* ConnPool::Instance()
{
	static ConnPool _instance;
	return &_instance;
}

ConnPool::ConnPool():m_conn_factroy(0)
{
	
}

ConnPool::~ConnPool()
{
	Release();
}

void ConnPool::Init(IConnFactroy *cf)
{
	m_conn_factroy = cf;
	int count = DEFAULT_POOL_LEN;
	while (count --)
	{
		IConnection *mc = m_conn_factroy->CreateConn();
		m_conn_pool.push(mc);
	}
}
void ConnPool::Release()
{
	while (m_conn_pool.size() != 0)
	{
		delete m_conn_pool.front();
		m_conn_pool.pop();
	}
}

IConnection* ConnPool::GetConn()
{
	
	IConnection *conn = 0;

	m_conn_pool_lock.Lock();
	if (m_conn_pool.size() != 0)
	{
		conn = m_conn_pool.front();
		m_conn_pool.pop();
	}
	m_conn_pool_lock.Unlock();

	if (conn == 0)
	{
		if (m_conn_factroy == 0)
		{
			return 0;
		}
		conn = m_conn_factroy->CreateConn();
	}

	if (!conn->isActive())
	{
		conn->reconnect();
		if (!conn->isActive())
		{
			conn->close();
			delete conn;
			time_t now = time(0);
			struct tm *p_tm = localtime(&now);
			char time_str[64];
			strftime(time_str, 64, "%Y-%m-%d %H:%M:%S", p_tm);
			printf("ERROR : ConnPool::GetConn isActive return false. time[%s]\n", time_str);
			fflush(stdout);
			return 0;
		}
	}

	return conn;
}

void ConnPool::PutConn(IConnection *conn)
{
	if (0 == conn)
	{
		return;
	}

	if (conn->isDestroy())
	{
		delete conn; conn = 0;

		time_t now = time(0);
		struct tm *p_tm = localtime(&now);
		char time_str[64];
		strftime(time_str, 64, "%Y-%m-%d %H:%M:%S", p_tm);
		printf("ERROR : ConnPool::PutConn destory conn. time[%s]\n", time_str);
		fflush(stdout);

		return;
	}

	m_conn_pool_lock.Lock();
	m_conn_pool.push(conn);
	m_conn_pool_lock.Unlock();
}

