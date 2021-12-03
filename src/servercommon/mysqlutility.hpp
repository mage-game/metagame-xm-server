#ifndef _MYSQLUTILITY_HPP_
#define _MYSQLUTILITY_HPP_


#include <stdlib.h>
#include <string.h>

#include "my_global.h"
#include "mysql.h"
#ifdef max
#undef max
#undef min
#endif


enum
{
	SQL_TYPE_UPDATE = 0,
	SQL_TYPE_SELECT = 1,
};

typedef struct 
{
	char sHostAddress[64];	
	char sUserName[64];		
	char sPassword[128];		
	char sDBName[64];		

	int	iDBConnect;						//是否已经连接上对应的Database, 0=断开，1=连接上	
	MYSQL stMysql;						//当前打开的Mysql连接
} MYSQL_CONN;

typedef struct 
{
	MYSQL_RES *pstRes;					//当前操作的RecordSet
	MYSQL_ROW stRow;					//当前操作的一行
	unsigned long *alFieldsLength;		//当前行各个field长度

	MYSQL_CONN stMysqlConn;				//Mysql连接链表的第一项
	MYSQL_CONN *pstCurMysqlConn;		//当前打开的Mysql连接指针

	int iResNotNull;					//当前操作的RecordSet是否为空,0=空，1=非空
	int	iResNum;						//当前操作的RecordSet的记录数目
	int	iQueryType;						//当前操作的SQL语句是否返回Recordset, 0=不要，1=select

	int	iSQLLen;						//sql语句的长度，为了使用mysql_real_query
	char szSQLString[8 * 1024];			//当前操作的SQL语句, 8K

	bool bConnected;
}TMysqlStatement;

class CMysqlUtility
{
public:
	static void Init(TMysqlStatement *pstMysqlStat);
	static int ConnectDB(TMysqlStatement *pstMysqlStat, const char *sHostAddress, const char *sUserName, 
		const char *sPassword, const char *sDBName, int iPort, char *sErrMsg, const char *pcCharSet = "utf8");

	static int CloseDatabase(TMysqlStatement *pstMysqlStat);
	static int FreeResult(TMysqlStatement *pstMysqlStat);
	static int FetchRow(TMysqlStatement *pstMysqlStat, char *sErrMsg);
	static int AffectedRow(TMysqlStatement *pstMysqlStat, char *sErrMsg);

	static int ExecSelect(TMysqlStatement *pstMysqlStat, int* iErrNo, char *sErrMsg);
	//static int RealUpdate(TMysqlStatement *pstMysqlStat, int* iErrNo, char *sErrMsg);

	static int SetCharSet(TMysqlStatement *pstMysqlStat, int* iErrNo, char *sErrMsg, const char *pcCharSet = "utf8");
};

#endif


