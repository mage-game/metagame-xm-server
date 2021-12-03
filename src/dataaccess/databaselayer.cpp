
#include "databaselayer.h"
#include "itimermodule.h"
#include "engineadapter.h"
#include "ilogmodule.h"

DataBaseLayer::DataBaseLayer()
{
	
}

DataBaseLayer::~DataBaseLayer()
{
	
}

DataBaseLayer &DataBaseLayer::Instance()
{
	static DataBaseLayer db;
	return db;
}


bool DataBaseLayer::Init(ILogModule *log, const std::string &log_dir, const char *name, size_t init_size_MB, bool is_delay_load, const std::vector<std::string> &not_cache_table, const std::vector<std::string> &load_table, unsigned long flush_interval)
{
	bool ret = m_cm.Init(name, init_size_MB, is_delay_load, not_cache_table, load_table, flush_interval);

	if (!ret) return ret;

	m_monitor_log.SetLogModule(log);
	m_monitor_log.SetCatagory("CacheMonitor", (log_dir+"dblog/cachemonitor.txt").c_str());
	
	// 启动定时检查策略
	CacheMonitor();

	return ret;
}

void DataBaseLayer::Release()
{
	m_cm.Release();
}

DBCommand* DataBaseLayer::GetCommand(int table_id)
{
	return m_cm.GetCommand(table_id);
}

const MataTableDef* DataBaseLayer::GetTable(int table_id)
{
	return m_cm.GetTable(table_id);
}

CacheBase* DataBaseLayer::GetCache(int table_id)
{
	return m_cm.GetCache(table_id);
}

bool DataBaseLayer::UseCache(int table_id)
{
	return m_cm.UseCache(table_id);
}

class CacheMonitorTimer : public ITimerCallback
{
	DataBaseLayer *dblayer;
public:
	CacheMonitorTimer(DataBaseLayer *db):dblayer(db){}
	virtual ~CacheMonitorTimer(){}

	virtual void OnTimer()
	{
		dblayer->CacheMonitor();
	}

	virtual void Free() { delete this; }
};

void DataBaseLayer::CacheMonitor()
{
	unsigned int cache_size = (unsigned int)CacheManager::Instance()->GetCacheSize();
	unsigned int cache_used_size = (unsigned int)CacheManager::Instance()->GetCacheUsedSize();
	unsigned int real_used_size = (unsigned int)CacheManager::Instance()->GetCacheRealUsedSize();

	cache_size /= 1024;
	cache_used_size /= 1024;
	real_used_size /= 1024;
	
	int reader_num = CacheManager::Instance()->GetReaderNum();
	int writer_num = CacheManager::Instance()->GetWriterNum();
	int blocked_reader_num = CacheManager::Instance()->GetBlockedReaderNum();
	int blocked_writer_num = CacheManager::Instance()->GetBlockedWriterNum();
	int user_num = CacheManager::Instance()->GetNumOfUser();

	m_monitor_log.printf(LL_INFO, "cache_size:%u/%u(KB) real_use:%u(KB) reader:%d writer:%d blocked_reader:%d blocked_writer:%d user:%d", cache_used_size, cache_size, real_used_size,
		reader_num, writer_num, blocked_reader_num, blocked_writer_num, user_num);

	EngineAdapter::Instance().CreateTimerSecond(60, new CacheMonitorTimer(this));
}


