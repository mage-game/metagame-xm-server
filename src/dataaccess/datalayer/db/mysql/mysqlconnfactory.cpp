

#include "mysqlconnfactroy.h"
#include "mysqlconnection.h"


MysqlConnFactroy::MysqlConnFactroy(	std::string username, std::string pw, std::string host,unsigned short port, std::string charset, std::string database)
{
	m_username = username;
	m_pw = pw;
	m_host = host;
	m_port = port;
	m_charset = charset;
	m_database = database;
}

MysqlConnFactroy::~MysqlConnFactroy()
{

}

IConnection* MysqlConnFactroy::CreateConn()
{
	IConnection* ic = new MysqlConnection(m_username.c_str(), m_pw.c_str(), m_database.c_str(), m_host.c_str(), m_port, m_charset.c_str());
	ic->connect();
	return ic;
}


