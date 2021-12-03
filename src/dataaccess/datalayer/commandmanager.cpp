
#include "cache/cachebase.h"
#include "dbopera/dbcommand.h"
#include "dbopera/dataadapter.h"
#include "cache/cachemanager.h"

#include "db/connection.h"
#include "db/statement.h"
#include "db/connpool.h"
#include "commandmanager.h"
#include "tablefactroy.h"
#include "tabledef/tableid.h"

#include "common/platform/system.h"


CommandManager::CommandManager():m_is_delay_load(false), m_run_flush(false), m_flush_interval(500)
{
	
}

CommandManager::~CommandManager()
{
	
}

bool CommandManager::Init(const char *name, size_t init_size_MB, bool is_delay_load, const std::vector<std::string> &not_cache_table, const std::vector<std::string> &load_table, unsigned long flush_interval)
{
	m_db_name = name;
	bool ret = CacheManager::Instance()->Init(m_db_name.c_str(), init_size_MB);	// 启动fastdb
	if (!ret)
	{
		return false;
	}

	int not_cache_table_count = 0, not_delay_table_count = 0;

	m_is_delay_load = is_delay_load;

	m_operator_list = tabledef::GetTableFactroy();

	for (int i = 0; i < (int)m_operator_list.size(); ++i)
	{
		bool is_cache = true;
		for (int j = 0; j < (int)not_cache_table.size(); ++j)
		{
			if (not_cache_table[j] == m_operator_list[i].table->m_table_name)
			{
				++ not_cache_table_count;

				is_cache = false;
				break;
			}
		}
		
		if (is_cache)
		{
			m_operator_list[i].command->Init(m_operator_list[i].table, m_operator_list[i].cache);
			m_operator_list[i].cache->Init(m_operator_list[i].table, m_operator_list[i].command);
		}
		else
		{
			delete m_operator_list[i].cache;
			m_operator_list[i].cache = 0;
			m_operator_list[i].command->Init(m_operator_list[i].table, 0);
		}
	}

	IConnection *conn = ConnPool::Instance()->GetConn();
	if (conn == 0)
	{
		//ConnPool::Instance()->PutConn(conn);  // 空指针不需要再put到连接池
		return false;
	}
	IStatement *stmt_tmp = conn->createStatement();
	conn->begin(false);
	for (int i = 0; i <(int)m_operator_list.size(); ++i)
	{
		if (m_operator_list[i].cache != 0)
		{
			bool is_load = true;
			if (m_is_delay_load)
			{
				is_load = false;
				for (int j = 0; j < (int)load_table.size(); ++j)
				{
					if (load_table[j] == m_operator_list[i].table->m_table_name)
					{
						is_load = true;
						break;
					}
				}
			}
			if (is_load)
			{
				++ not_delay_table_count;

				m_operator_list[i].cache->Load(stmt_tmp);
			}
		}
	}
	conn->commit();
	delete stmt_tmp;
	ConnPool::Instance()->PutConn(conn);

	m_run_flush = true;
	m_flush_interval = flush_interval;
	m_cache_flush_thread.Run(CacheFlushThread, this);

	int oper_size = (int)m_operator_list.size();
	if (oper_size != ALL_TABLE_NUM || not_cache_table_count != NOT_CACHE_TABLE_NUM || not_delay_table_count != NOT_DELAY_TABLE_NUM)
	{
		printf("ALL_TABLE_NUM(%d) != oper_size(%d) || NOT_CACHE_TABLE_NUM(%d) != not_cache_table(%d) || NOT_DELAY_TABLE_NUM(%d) != not_delay_table_count(%d) \n",
			ALL_TABLE_NUM, oper_size,
			NOT_CACHE_TABLE_NUM, not_cache_table_count,
			NOT_DELAY_TABLE_NUM, not_delay_table_count);
		return false;
	}

	return true;
}

void CommandManager::Release()
{
	m_run_flush = false;
	m_cache_flush_thread.Join();

	CacheFlush();
	UnnagtiveCache();

	for (int i = 0; i < (int)m_operator_list.size(); ++i)
	{
		if (m_operator_list[i].cache != 0)
		{
			delete m_operator_list[i].cache;
		}
		delete m_operator_list[i].command;
		delete m_operator_list[i].table;
	}
	m_operator_list.clear();

	CacheManager::Instance()->Release();
}

DBCommand* CommandManager::GetCommand(int table_id)
{
	assert(table_id < tabledef::TABLE_COUNT);
	return m_operator_list[table_id].command;
}

const MataTableDef* CommandManager::GetTable(int table_id)
{
	assert(table_id < tabledef::TABLE_COUNT);
	return m_operator_list[table_id].table;
}

CacheBase* CommandManager::GetCache(int table_id)
{
	assert(table_id < tabledef::TABLE_COUNT);
	return m_operator_list[table_id].cache;
}

bool CommandManager::UseCache(int table_id)
{
	assert(table_id < tabledef::TABLE_COUNT);
	return m_operator_list[table_id].cache != 0;
}

DWORD CommandManager::CacheFlushThread(void *p)
{
	CommandManager *cm = (CommandManager*)p;

	unsigned long lash_flush_time = PITime();

	while (cm->m_run_flush)
	{
		unsigned long now = PITime();
		if (now - lash_flush_time > cm->m_flush_interval)
		{
			bool ret = cm->CacheFlush();
			if (!ret)
			{
				// ***************************
			}
			lash_flush_time = now;
		}
		PISleep(100);
	}
	
	return 0;
}

bool CommandManager::CacheFlush()
{
	//return;
	IConnection *conn = ConnPool::Instance()->GetConn();
	if (conn == 0)
	{
		return false;
	}
	IStatement *stmt_tmp = conn->createStatement();
	conn->begin(false);
	for (int i = 0; i < (int)m_operator_list.size(); ++i)
	{
		if (m_operator_list[i].cache != 0)
		{
			m_operator_list[i].cache->Flush(stmt_tmp);
		}
	}
	conn->commit();
	delete stmt_tmp;
	ConnPool::Instance()->PutConn(conn);
	return true;
}

bool CommandManager::UnnagtiveCache()
{
	IConnection *conn = ConnPool::Instance()->GetConn();
	if (conn == 0)
	{
		return false;
	}
	IStatement *stmt_tmp = conn->createStatement();
	conn->begin(false);
	for (int i = 0; i < (int)m_operator_list.size(); ++i)
	{
		if (m_operator_list[i].cache != 0)
		{
			m_operator_list[i].cache->Unnagtive(stmt_tmp);
		}
	}
	conn->commit();
	delete stmt_tmp;
	ConnPool::Instance()->PutConn(conn);

	return true;
}


