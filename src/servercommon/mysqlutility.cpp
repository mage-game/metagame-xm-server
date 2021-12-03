#include "mysqlutility.hpp"

#include <stdio.h>


void CMysqlUtility::Init(TMysqlStatement *pstMysqlStat)
{
	if (!pstMysqlStat)
	{
		return;
	}

	pstMysqlStat->pstRes = 0;
	pstMysqlStat->alFieldsLength = 0;
	pstMysqlStat->pstCurMysqlConn = 0;
	pstMysqlStat->iResNum = 0;
	pstMysqlStat->iQueryType = -1;
	memset(&pstMysqlStat->stMysqlConn, 0, sizeof(pstMysqlStat->stMysqlConn));

	//初始化mysql的连接
	mysql_init(&(pstMysqlStat->stMysqlConn.stMysql));

	pstMysqlStat->iResNotNull = 0;
	pstMysqlStat->pstCurMysqlConn = &(pstMysqlStat->stMysqlConn); //设置当前连接指针
	pstMysqlStat->pstCurMysqlConn->iDBConnect = 0;

	pstMysqlStat->bConnected = false;
}

int CMysqlUtility::ConnectDB(TMysqlStatement *pstMysqlStat, const char *sHostAddress, const char *sUserName, 
							  const char *sPassword, const char *sDBName, int iPort, char *sErrMsg, const char *pcCharSet /*= "utf8"*/)
{
	if (!pstMysqlStat)
	{
		return -100;
	}

	if (pstMysqlStat->bConnected == true)
	{
		return 0;
	}

	bool bSelectDB = false;

	//如果要连接的地址和当前的地址不是同一台机器则先close当前的连接,再重新建立连接
	if (strcmp(pstMysqlStat->pstCurMysqlConn->sHostAddress, sHostAddress) != 0)
	{
		if (pstMysqlStat->pstCurMysqlConn->iDBConnect == 1)
		{
			CloseDatabase(pstMysqlStat);
		}

		mysql_options(&(pstMysqlStat->pstCurMysqlConn->stMysql), MYSQL_SET_CHARSET_NAME, "utf8");

		if (mysql_real_connect(&(pstMysqlStat->pstCurMysqlConn->stMysql), sHostAddress, sUserName, sPassword, sDBName, iPort,
			NULL, CLIENT_FOUND_ROWS) == 0)
		{
			sprintf(sErrMsg, "Fail To Connect To Mysql: %s", mysql_error(&(pstMysqlStat->pstCurMysqlConn->stMysql)));
			return -1;
		}	

		pstMysqlStat->pstCurMysqlConn->iDBConnect = 1;
		bSelectDB = true;
	}
	else  //如果当前没有连接，则连接mysql 
	{
		if (pstMysqlStat->pstCurMysqlConn->iDBConnect == 0)
		{
			mysql_options(&(pstMysqlStat->pstCurMysqlConn->stMysql), MYSQL_SET_CHARSET_NAME, "utf8");

			if (mysql_real_connect(&(pstMysqlStat->pstCurMysqlConn->stMysql), sHostAddress, sUserName, sPassword, sDBName, iPort,
				NULL, CLIENT_FOUND_ROWS) == 0)
			{
				sprintf(sErrMsg, "Fail To Connect To Mysql: %s", mysql_error(&(pstMysqlStat->pstCurMysqlConn->stMysql)));
				return -2;
			}	
			pstMysqlStat->pstCurMysqlConn->iDBConnect = 1;
		}	
	}


	if (bSelectDB || (strcmp(pstMysqlStat->pstCurMysqlConn->sDBName, sDBName) != 0))
	{
		int iSelectRet = mysql_select_db(&(pstMysqlStat->pstCurMysqlConn->stMysql), sDBName);
		if (iSelectRet != 0)
		{
			sprintf(sErrMsg, "Cannot(%d) Select Database %s: %s",
				iSelectRet, sDBName, mysql_error(&(pstMysqlStat->pstCurMysqlConn->stMysql)));
			return -3;
		}
	}	

	strcpy(pstMysqlStat->pstCurMysqlConn->sHostAddress, sHostAddress);
	strcpy(pstMysqlStat->pstCurMysqlConn->sDBName, sDBName);	
	strcpy(sErrMsg, "");

	pstMysqlStat->bConnected = true;

	return 0;
}

int CMysqlUtility::CloseDatabase(TMysqlStatement *pstMysqlStat)
{
	if (!pstMysqlStat)
	{
		return -100;
	}

	if (pstMysqlStat->iResNotNull == 1)
	{
		mysql_free_result(pstMysqlStat->pstRes);
		pstMysqlStat->iResNotNull= 0;
	}		

	//关闭链表中第一个mysql连接
	if (pstMysqlStat->stMysqlConn.iDBConnect == 1)
		mysql_close(&(pstMysqlStat->stMysqlConn.stMysql));

	//设置当前连接指针
	pstMysqlStat->stMysqlConn.iDBConnect = 0;

	pstMysqlStat->pstCurMysqlConn = &(pstMysqlStat->stMysqlConn);		
	pstMysqlStat->pstCurMysqlConn->sHostAddress[0] = '\0';

	return 0;
}

int CMysqlUtility::FreeResult(TMysqlStatement *pstMysqlStat)
{
	if (!pstMysqlStat)
	{
		return -100;
	}

	if (pstMysqlStat->iResNotNull == 1)
	{
		mysql_free_result(pstMysqlStat->pstRes);
		pstMysqlStat->iResNotNull = 0;
	}		

	return 0;
}

int CMysqlUtility::FetchRow(TMysqlStatement *pstMysqlStat, char *sErrMsg)
{
	if (!pstMysqlStat)
	{
		return -100;
	}

	if (pstMysqlStat->iResNotNull == 0)
	{
		sprintf(sErrMsg, "Recordset is Null");
		return -1;	
	}		

	if (pstMysqlStat->iResNum == 0)
	{
		sprintf(sErrMsg, "Recordset count=0");
		return -2;	
	}		

	pstMysqlStat->stRow = mysql_fetch_row(pstMysqlStat->pstRes);

	pstMysqlStat->alFieldsLength = mysql_fetch_lengths(pstMysqlStat->pstRes);

	return 0;
}

int CMysqlUtility::AffectedRow(TMysqlStatement *pstMysqlStat, char *sErrMsg)
{
	if (!pstMysqlStat)
	{
		return -100;
	}

	if ((pstMysqlStat->pstCurMysqlConn == NULL) || (pstMysqlStat->pstCurMysqlConn->iDBConnect == 0))
	{
		strcpy(sErrMsg, "Has Not Connect To DB Server Yet");
		return -1;
	}

	int iAffectedRow = (int)mysql_affected_rows(&(pstMysqlStat->pstCurMysqlConn->stMysql));
	return iAffectedRow;
}

int CMysqlUtility::ExecSelect(TMysqlStatement *pstMysqlStat, int* iErrNo,char *sErrMsg)
{
	if (!pstMysqlStat)
	{
		return -100;
	}

	// 检查参数是否正确
	if (pstMysqlStat->iQueryType != SQL_TYPE_SELECT)
	{
		sprintf(sErrMsg, "iQueryType != SQL_TYPE_SELECT");
		return -1;
	}

	if ((pstMysqlStat->szSQLString[0] != 's') && (pstMysqlStat->szSQLString[0] != 'S'))
	{
		sprintf(sErrMsg, "QueryType=1, But SQL is not select");
		return -2;
	}

	// 是否需要关闭原来RecordSet
	if (pstMysqlStat->iResNotNull == 1)
	{
		mysql_free_result(pstMysqlStat->pstRes);
		pstMysqlStat->iResNotNull = 0;
	}		

	if ((pstMysqlStat->pstCurMysqlConn == NULL) || (pstMysqlStat->pstCurMysqlConn->iDBConnect == 0))
	{
		strcpy(sErrMsg, "Has Not Connect to mysql");
		return -1;
	}

	// 执行相应的SQL语句
	int iRet = mysql_query(&(pstMysqlStat->pstCurMysqlConn->stMysql), pstMysqlStat->szSQLString);
	if (iRet != 0)
	{
		//出错信息
		*iErrNo = mysql_errno(&(pstMysqlStat->pstCurMysqlConn->stMysql));
		sprintf(sErrMsg, "Fail To Execute SQL: %s", mysql_error(&(pstMysqlStat->pstCurMysqlConn->stMysql)));
		return -1;
	}

	// 保存结果
	pstMysqlStat->pstRes = mysql_store_result(&(pstMysqlStat->pstCurMysqlConn->stMysql));
	pstMysqlStat->iResNum = (int)mysql_num_rows(pstMysqlStat->pstRes);
	pstMysqlStat->iResNotNull = 1;

	return 0;
}

//int CMysqlUtility::RealUpdate(TMysqlStatement *pstMysqlStat, int* iErrNo,char *sErrMsg)
//{
//	if (!pstMysqlStat)
//	{
//		return -100;
//	}
//
//	// 检查参数是否正确
//	if (pstMysqlStat->iQueryType != SQL_TYPE_UPDATE)
//	{
//		sprintf(sErrMsg, "iQueryType != SQL_TYPE_UPDATE");
//		return -1;
//	}
//
//	// 是否需要关闭原来RecordSet
//	if (pstMysqlStat->iResNotNull == 1)
//	{
//		mysql_free_result(pstMysqlStat->pstRes);
//		pstMysqlStat->iResNotNull = 0;
//	}		
//
//	if ((pstMysqlStat->pstCurMysqlConn == NULL) || (pstMysqlStat->pstCurMysqlConn->iDBConnect == 0))
//	{
//		strcpy(sErrMsg, "Has Not Connect To DB Server Yet");
//		return -1;
//	}
//
//	// 执行相应的SQL语句
//	int iRet = mysql_real_query(&(pstMysqlStat->pstCurMysqlConn->stMysql), pstMysqlStat->szSQLString, pstMysqlStat->iSQLLen);
//	if (iRet != 0)
//	{
//		//出错信息
//		*iErrNo=mysql_errno(&(pstMysqlStat->pstCurMysqlConn->stMysql));
//		sprintf(sErrMsg, "Fail To Execute SQL: %s", mysql_error(&(pstMysqlStat->pstCurMysqlConn->stMysql)));
//		return -1;
//	}
//
//	return 0;
//}

int CMysqlUtility::SetCharSet(TMysqlStatement *pstMysqlStat, int* iErrNo,char *sErrMsg, const char *pcCharSet /*= "utf8"*/)
{
	if (!pstMysqlStat)
	{
		return -100;
	}

	// 是否需要关闭原来RecordSet
	/*	if (pstMysqlStat->iResNotNull==1)
	{
	mysql_free_result(pstMysqlStat->pstRes);
	pstMysqlStat->iResNotNull=0;
	}*/		

	if ((pstMysqlStat->pstCurMysqlConn == NULL) || (pstMysqlStat->pstCurMysqlConn->iDBConnect == 0))
	{
		strcpy(sErrMsg, "Has Not Connect to mysql");
		return -1;
	}

	pstMysqlStat->iSQLLen = sprintf(pstMysqlStat->szSQLString, "set names %s", pcCharSet);

	// 执行相应的SQL语句
	int iRet = mysql_real_query(&(pstMysqlStat->pstCurMysqlConn->stMysql), pstMysqlStat->szSQLString, pstMysqlStat->iSQLLen);
	if (iRet != 0)
	{
		//出错信息
		*iErrNo = mysql_errno(&(pstMysqlStat->pstCurMysqlConn->stMysql));
		sprintf(sErrMsg, "Fail To Execute SQL: %s", mysql_error(&(pstMysqlStat->pstCurMysqlConn->stMysql)));
		return -1;
	}

	// 保存结果
	/*pstMysqlStat->pstRes = mysql_store_result(&(pstMysqlStat->pstCurMysqlConn->stMysql));
	pstMysqlStat->iResNum = mysql_num_rows(pstMysqlStat->pstRes);
	pstMysqlStat->iResNotNull = 1;*/

	return 0;
}
