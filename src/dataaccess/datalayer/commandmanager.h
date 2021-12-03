
#ifndef COMMANDMANAGER_H
#define COMMANDMANAGER_H

#include <vector>
#include <string>

#include "common/platform/thread/thread.h"

class MataTableDef;
class DBCommand;
class CacheBase;


/*	增加表可能需要修改下面几个变量
 *	@ALL_TABLE_NUM 所有表个数					
 *  @NOT_CACHE_TABLE_NUM 不使用缓存的表个数
 *  @@NOT_DELAY_TABLE_NUM 不延迟加载表的个数 （启动加载到内存中）
 */ 

static const int ALL_TABLE_NUM = 47;
static const int NOT_CACHE_TABLE_NUM = 5;
static const int NOT_DELAY_TABLE_NUM = 23;

class CommandManager
{
public:
	struct TableOperator
	{
		MataTableDef	*table;
		DBCommand		*command;
		CacheBase		*cache;
	};
	typedef std::vector<TableOperator>	OperatorList;

	CommandManager();
	~CommandManager();

	bool					Init(const char *name, size_t init_size_MB, bool is_delay_load, const std::vector<std::string> &not_cache_table, const std::vector<std::string> &load_table, unsigned long flush_interval=500);
	void					Release();

	DBCommand*				GetCommand(int table_id);
	const MataTableDef*		GetTable(int table_id);
	CacheBase*				GetCache(int table_id);
	bool					UseCache(int table_id);

	bool					IsDelayLoad() { return m_is_delay_load; }
protected:
	OperatorList		m_operator_list;
	std::string			m_db_name;

	bool				m_is_delay_load;

	bool				m_run_flush;
	unsigned long		m_flush_interval;
	Thread				m_cache_flush_thread;
	static DWORD		CacheFlushThread(void *p);
	bool				CacheFlush();

	bool				UnnagtiveCache();
};


#endif


