
#ifndef DATABASE_H
#define DATABASE_H

#include "datalayer/commandmanager.h"
#include "utility/logagent.h"

class DBCommand;
class MataTableDef;
class CacheBase;
class ILogModule;

class DataBaseLayer
{
public:
	static DataBaseLayer &Instance();
	~DataBaseLayer();

	bool					Init(ILogModule *log, const std::string &log_dir, const char *name, size_t init_size_MB, bool is_delay_load, const std::vector<std::string> &not_cache_table, const std::vector<std::string> &load_table, unsigned long flush_interval);
	void					Release();

	DBCommand*				GetCommand(int table_id);
	const MataTableDef*		GetTable(int table_id);
	CacheBase*				GetCache(int table_id);
	bool					UseCache(int table_id);

	bool					IsDelayLoad() { return m_cm.IsDelayLoad(); }

	void					CacheMonitor();
protected:
	DataBaseLayer();
private:
	CommandManager m_cm;
	LogAgent m_monitor_log;
};

#include "datalayer/dbopera/dbcommand.h"
#include "datalayer/cache/cachecommand.h"
#include "datalayer/dbopera/dataadapter.h"
#include "datalayer/tabledef/tableid.h"

class StmtHelper
{
public:
	StmtHelper(IConnection *conn, IStatement *stmt):m_conn(conn), m_stmt(stmt)
	{
	}
	~StmtHelper()
	{
		m_conn->rollback();
		delete m_stmt;
		ConnPool::Instance()->PutConn(m_conn);
	}
protected:
	IConnection	*m_conn;
	IStatement	*m_stmt;
};

#define INIT_STMT(CONN, STMT, CONN_ERROR_RET)\
	IConnection *CONN = ConnPool::Instance()->GetConn();\
	if (CONN == 0) return CONN_ERROR_RET;\
	IStatement *STMT = CONN->createStatement();\
	StmtHelper __stmt_help(CONN, STMT);

#endif

