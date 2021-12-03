
#ifndef IRESULTSET_H
#define IRESULTSET_H

#include <time.h>
#include <string>
#include <string.h>
#include "db/dbdef.h"

class IStatement;

class IResultset
{
public:
	IResultset(){};
	virtual ~IResultset(){};

	virtual IStatement* getStmt() = 0;
	virtual void setStmt(IStatement* stmt) = 0;
	
	virtual bool init() = 0;
	virtual void close() = 0;

	virtual int getColumnCount() = 0;
	virtual std::string getColumnName(int index) = 0;
	virtual bool getColumnDataType(int index, int* type) = 0;

	virtual int findColumn(const char* columnName) = 0;

	//获取下一行数据，返回获取失败时，若isErr为false则读取完成，若为true则发生异常
	virtual bool next(bool* isErr) = 0;

	virtual bool isNull(int index, bool* isnull) = 0;
	virtual bool isNull(const char* columnName, bool* isnull) = 0;

	virtual bool getByte(int index, char* ret) = 0;
	virtual bool getByte(const char* columnName, char* ret) = 0;

	//getBytes是为了获取类似mysql中的BLOB/TEXT类型的数据，尚未实现
	//virtual bool getBytes(int index, char* ret, int& length) = 0;
	//virtual bool getBytes(const char* columnName, char* ret, int& length) = 0;

	virtual bool getBoolean(int index, bool* ret) = 0;
	virtual bool getBoolean(const char* column, bool* ret) = 0;

	virtual bool getString(int index, char* ret, unsigned long* length) = 0;
	virtual bool getString(const char* pColumn, char* ret, unsigned long* length) = 0;

	virtual bool getInt32(int index, int* ret) = 0;
	virtual bool getInt32(const char* column, int* ret) = 0;

	virtual bool getDouble(int index, double* ret) = 0;
	virtual bool getDouble(const char* column, double* ret) = 0;

	virtual bool getDate(int index, tm* ret) = 0;
	virtual bool getDate(const char* column, tm* ret) = 0;

	virtual bool getTime(int index, tm* ret) = 0;
	virtual bool getTime(const char* column, tm* ret) = 0;

	virtual bool getTimestamp(int index, tm* ret) = 0;
	virtual bool getTimestamp(const char* column, tm* ret) = 0;

	virtual bool getLong64(int index, long long* ret) = 0;
	virtual bool getLong64(const char* column, long long* ret) = 0;

protected:
	IStatement* m_stmt;
};

#endif

