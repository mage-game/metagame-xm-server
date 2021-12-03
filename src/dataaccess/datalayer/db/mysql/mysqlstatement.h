

#ifndef MYSQLSTATEMENT_H
#define MYSQLSTATEMENT_H

#include "db/statement.h"

struct st_mysql_stmt;
typedef st_mysql_stmt MYSQL_STMT;

struct st_mysql_bind;
typedef st_mysql_bind MYSQL_BIND;

class MysqlStatement:public IStatement
{
public:
	MysqlStatement();
	//MysqlStatement(IConnection* conn);
	~MysqlStatement();

	virtual void setConnection(IConnection* conn);
	virtual IConnection* getConnection();

	virtual void* getStmt();
	//返回预处理INSERT或UPDATE语句为AUTO_INCREMENT列生成的值
	virtual long long getInsertID();
	virtual bool prepare(const char* sql);
	virtual void clearParameters();
	virtual void close();

	virtual bool setByte(int paramIndex, char value);
	//virtual bool setBytes(int paramIndex, const char* value, int length);

	virtual bool setBoolean(int paramIndex, bool value);

	virtual bool setInt32(int paramIndex, int value);
	virtual bool setDouble(int paramIndex, double value);
	virtual bool setLong64(int paramIndex, long long value );

	virtual bool setString(int paramIndex, const char* value, unsigned long length);

	virtual bool setDate(int paramIndex, const tm& value);
	virtual bool setTime(int paramIndex, const tm& value);
	virtual bool setTimestamp(int paramIndex, const tm& value);

	virtual int execute();
	virtual int execute(const char* sql);

	//virtual int executeUpdate();
	//virtual int executeUpdate(const char* sql);

	virtual IResultset* executeQuery();
	virtual IResultset* executeQuery(const char* sql);

protected:
	bool validIndex(int index);
	void initParams();
	void cleanParam(MYSQL_BIND* param);
	void destroyParams();

private:
	IConnection* m_conn;
	MYSQL_STMT* m_stmt;
	MYSQL_BIND* m_params;
	int m_paramcount;
};

#endif
