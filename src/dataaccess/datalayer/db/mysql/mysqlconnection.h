

#ifndef MYSQLCONNECTION_H
#define MYSQLCONNECTION_H

//#pragma comment(linker,"/nodefaultlib:LIBCMT.lib")
//#pragma comment(linker,"/nodefaultlib:MSVCRTD.lib")

#include "db/connection.h"

struct st_mysql;
typedef st_mysql MYSQL;
class ITransaction;

class MysqlConnection:public IConnection
{
public:
	MysqlConnection(const char *dbuser, const char *dbpw, const char *dbname, const char *dbhost, unsigned int dbport, const char* charset);
	virtual ~MysqlConnection();

	virtual IConnection* clone() ;

	virtual bool connect();
	virtual bool reconnect();

	virtual bool isActive();
	virtual bool isBegin();

	virtual void close();
	virtual bool isClosed();

	virtual bool setAutoCommit(bool AutoCommit);
	virtual bool getAutoCommit();

	//virtual void setIsCache(bool iscache);
	virtual bool getIsCache();

	virtual void* getconn();
	virtual ITransaction* getTransaction();
	//返回由以前的INSERT或UPDATE语句为AUTO_INCREMENT列生成的值
	virtual long long getInsertID();

	virtual bool begin(bool iscache=true);
	virtual bool commit();
	virtual bool rollback();

	virtual IStatement* createStatement();

	virtual int getErrNo() const;
	virtual const char* getError() const;

	virtual bool isDestroy() { return m_destroy; }

private:
	MYSQL *m_mysql;
	ITransaction* m_transaction;

	char m_username[64];
	char m_pw[64];
	char m_host[64];
	unsigned short m_port;
	char m_charset[32];
	char m_database[64];

	bool m_connected;
	bool m_autoCommit;
	bool m_begin;
	bool m_isCache;

	bool m_destroy;
};

#endif


