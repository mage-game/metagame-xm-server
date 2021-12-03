
#ifndef MYSQLCONNFACTROY_H
#define MYSQLCONNFACTROY_H

#include <string>
#include "db/connfactroy.h"

class MysqlConnFactroy:public IConnFactroy
{
public:
	MysqlConnFactroy( std::string username, std::string pw, std::string host,unsigned short port, std::string charset, std::string database);
	virtual ~MysqlConnFactroy();
	virtual IConnection* CreateConn();
private:
	std::string m_username;
	std::string m_pw;
	std::string m_host;
	unsigned short m_port;
	std::string m_charset;
	std::string m_database;
};





#endif



