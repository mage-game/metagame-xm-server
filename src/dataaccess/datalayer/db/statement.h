
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

	//������ʵ�����ָ�룬��MYSQL_STMT*
	virtual void* getStmt() = 0;
	//����Ԥ����INSERT��UPDATE���ΪAUTO_INCREMENT�����ɵ�ֵ
	virtual long long getInsertID() = 0;

	virtual bool prepare(const char* sql) = 0;
	virtual void clearParameters() = 0;
	virtual void close() = 0;

	//д��һ���ֽڣ�����mysql�е�TINYINT
	virtual bool setByte(int paramIndex, char value) = 0;
	//д��N���ֽڣ�����mysql�е�BLOB/TEXT
	//virtual bool setBytes(int paramIndex, const char* value, int length) = 0;

	//д��һ��������
	virtual bool setBoolean(int paramIndex, bool value) = 0;

	//д��һ��32λ���ͣ�����mysql�е�INT
	virtual bool setInt32(int paramIndex, int value) = 0;
	//д��һ��˫���ȸ����ͣ�����mysql�е�DOUBLE
	virtual bool setDouble(int paramIndex, double value) = 0;
	//д��һ��64λ��������mysql�е�BIGINT
	virtual bool setLong64(int paramIndex, long long value ) = 0;

	//д���ַ���������mysql�е�VARCHAR
	virtual bool setString(int paramIndex, const char* value, unsigned long length) = 0;

	//д������+ʱ�䣬����mysql�е�DATETIME
	virtual bool setDate(int paramIndex, const tm& value) = 0;
	//д��ʱ�䣬����mysql�е�TIME
	virtual bool setTime(int paramIndex, const tm& value) = 0;
	//д��ʱ�䣬����mysql�е�TIMESTAMP�������ֶθ���ʱ�����Զ�����
	virtual bool setTimestamp(int paramIndex, const tm& value) = 0;

	virtual int execute() = 0;
	virtual int execute(const char* sql) = 0;

	//virtual int executeUpdate() = 0;
	//virtual int executeUpdate(const char* sql) = 0;

	virtual IResultset* executeQuery() = 0;
	virtual IResultset* executeQuery(const char* sql) = 0;

};

#endif

