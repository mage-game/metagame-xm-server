

#ifndef CONNPOOL_H
#define CONNPOOL_H

#include "common/platform/thread/mutex.h"
#include <queue>

class IConnection;
class IConnFactroy;

class ConnPool
{
public:
	~ConnPool();

	static ConnPool* Instance();

	void			Init(IConnFactroy *cf);
	void			Release();
	IConnection*	GetConn();
	void			PutConn(IConnection *conn);  // �ú������ܻ��ͷŵ�conn���� ע�⣡����

private:
	ConnPool();

	typedef std::queue<IConnection*>	ConnList;
	ConnList	m_conn_pool;
	Mutex		m_conn_pool_lock;

	IConnFactroy *m_conn_factroy;
};

#endif

