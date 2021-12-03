

#ifndef MYSQLRESULT_H
#define MYSQLRESULT_H

#include "db/resultset.h"
#include <map>

class IStatement;

struct st_mysql_bind;
typedef st_mysql_bind MYSQL_BIND;

struct st_mysql_res;
typedef st_mysql_res MYSQL_RES;

class MysqlResult:public IResultset
{
public:
	MysqlResult();
	virtual ~MysqlResult();

	IStatement* getStmt();
	void setStmt(IStatement* stmt);

	virtual bool init();
	virtual void close();

	virtual int getColumnCount();
	virtual std::string getColumnName(int index);
	virtual bool getColumnDataType(int index, int* type);

	virtual int findColumn(const char* columnName);
	virtual bool next(bool* isErr);

	virtual bool isNull(int index, bool* isnull);
	virtual bool isNull(const char* columnName, bool* isnull);

	virtual bool getByte(int index, char* ret);
	virtual bool getByte(const char* columnName, char* ret);

	//virtual bool getBytes(int index, char* ret, int& length);
	//virtual bool getBytes(const char* columnName, char* ret, int& length);

	virtual bool getBoolean(int index, bool* ret);
	virtual bool getBoolean(const char* columnName, bool* ret);

	virtual bool getString(int index, char* ret, unsigned long* length);
	virtual bool getString(const char* columnName, char* ret, unsigned long* length);

	virtual bool getInt32(int index, int* ret);
	virtual bool getInt32(const char* columnName, int* ret);

	virtual bool getDouble(int index, double* ret);
	virtual bool getDouble(const char* columnName, double* ret);

	virtual bool getDate(int index, tm* ret);
	virtual bool getDate(const char* columnName, tm* ret);

	virtual bool getTime(int index, tm* ret);
	virtual bool getTime(const char* columnName, tm* ret);

	virtual bool getTimestamp(int index, tm* ret);
	virtual bool getTimestamp(const char* columnName, tm* ret);

	virtual bool getLong64(int index, long long* ret);
	virtual bool getLong64(const char* columnName, long long* ret);

	struct IndexType 
	{
		unsigned int index;
		int type;
	};

	struct TypeName
	{
		int type;
		std::string name;
	};
protected:
	bool validIndex(int index);
	void cleanParam(MYSQL_BIND* bind);
	void clear();

private:
	std::map<std::string, IndexType> m_fieldNameInfo;
	TypeName *m_fieldIndexInfo;
	MYSQL_RES *m_metadata;
	unsigned int m_fieldCount;
	MYSQL_BIND *m_result;
};
#endif

