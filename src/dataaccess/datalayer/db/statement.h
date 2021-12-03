
#ifndef ISTATEMENT_H
#define ISTATEMENT_H

#include <time.h>
#include "db/dbdef.h"

class IConnection;
class IResultset;

class IStatement
{
public:
	IStatement(){};
	virtual ~IStatement(){};

	virtual void setConnection(IConnection* conn) = 0;
	virtual IConnection* getConnection() = 0;

	//获得语句实体对象指针，如MYSQL_STMT*
	virtual void* getStmt() = 0;
	//返回预处理INSERT或UPDATE语句为AUTO_INCREMENT列生成的值
	virtual long long getInsertID() = 0;

	virtual bool prepare(const char* sql) = 0;
	virtual void clearParameters() = 0;
	virtual void close() = 0;

	//写入一个字节，例如mysql中的TINYINT
	virtual bool setByte(int paramIndex, char value) = 0;
	//写入N个字节，例如mysql中的BLOB/TEXT
	//virtual bool setBytes(int paramIndex, const char* value, int length) = 0;

	//写入一个布尔型
	virtual bool setBoolean(int paramIndex, bool value) = 0;

	//写入一个32位整型，例如mysql中的INT
	virtual bool setInt32(int paramIndex, int value) = 0;
	//写入一个双精度浮点型，例如mysql中的DOUBLE
	virtual bool setDouble(int paramIndex, double value) = 0;
	//写入一个64位数，例如mysql中的BIGINT
	virtual bool setLong64(int paramIndex, long long value ) = 0;

	//写入字符串，例如mysql中的VARCHAR
	virtual bool setString(int paramIndex, const char* value, unsigned long length) = 0;

	//写入日期+时间，例如mysql中的DATETIME
	virtual bool setDate(int paramIndex, const tm& value) = 0;
	//写入时间，例如mysql中的TIME
	virtual bool setTime(int paramIndex, const tm& value) = 0;
	//写入时间，例如mysql中的TIMESTAMP，其他字段更新时，会自动更新
	virtual bool setTimestamp(int paramIndex, const tm& value) = 0;

	virtual int execute() = 0;
	virtual int execute(const char* sql) = 0;

	//virtual int executeUpdate() = 0;
	//virtual int executeUpdate(const char* sql) = 0;

	virtual IResultset* executeQuery() = 0;
	virtual IResultset* executeQuery(const char* sql) = 0;

};

#endif

