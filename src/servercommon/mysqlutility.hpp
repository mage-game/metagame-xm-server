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

	int	iDBConnect;						//�Ƿ��Ѿ������϶�Ӧ��Database, 0=�Ͽ���1=������	
	MYSQL stMysql;						//��ǰ�򿪵�Mysql����
} MYSQL_CONN;

typedef struct 
{
	MYSQL_RES *pstRes;					//��ǰ������RecordSet
	MYSQL_ROW stRow;					//��ǰ������һ��
	unsigned long *alFieldsLength;		//��ǰ�и���field����

	MYSQL_CONN stMysqlConn;				//Mysql��������ĵ�һ��
	MYSQL_CONN *pstCurMysqlConn;		//��ǰ�򿪵�Mysql����ָ��

	int iResNotNull;					//��ǰ������RecordSet�Ƿ�Ϊ��,0=�գ�1=�ǿ�
	int	iResNum;						//��ǰ������RecordSet�ļ�¼��Ŀ
	int	iQueryType;						//��ǰ������SQL����Ƿ񷵻�Recordset, 0=��Ҫ��1=select

	int	iSQLLen;						//sql���ĳ��ȣ�Ϊ��ʹ��mysql_real_query
	char szSQLString[8 * 1024];			//��ǰ������SQL���, 8K

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


