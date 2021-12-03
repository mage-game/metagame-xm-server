

#include "cachemanager.h"

CacheManager* CacheManager::_instance = 0;

CacheManager::CacheManager()
{

}

CacheManager::~CacheManager()
{

}

bool CacheManager::Init(const char* dbname, size_t init_size_MB)
{
	if (!m_db.isOpen())
	{
		m_dbname = dbname;
		m_dbname +=".dbs";

		FASTDB_NS::dbDatabase::OpenParameters openParameters;
		openParameters.databaseName = m_dbname.c_str();
		openParameters.initSize = init_size_MB * 1024 * 1024;

		return m_db.open(openParameters);
	}
	return false;
}

void CacheManager::Release()
{
	if (m_db.isOpen())
	{
		m_db.close();
		m_dbname = "";
	}
}

CacheManager* CacheManager::Instance()
{
	if (0 == _instance)
	{
		_instance = new CacheManager();
	}
	return _instance;
}


dbDatabase *CacheManager::GetCache()
{
	return &m_db;
}

size_t CacheManager::GetCacheSize()
{
	return m_db.getDatabaseSize();
}

size_t CacheManager::GetCacheUsedSize()
{
	return m_db.getUsedDatabaseSize();
}

size_t CacheManager::GetCacheRealUsedSize()
{
	dbMemoryStatistic stat;
	m_db.getMemoryStatistic(stat);
	return stat.used;
}

int CacheManager::GetReaderNum()
{
	return m_db.getNumberOfReaders();
}

int CacheManager::GetWriterNum()
{
	return m_db.getNumberOfWriters();
}

int CacheManager::GetBlockedReaderNum()
{
	return m_db.getNumberOfBlockedReaders();
}

int CacheManager::GetBlockedWriterNum()
{
	return m_db.getNumberOfBlockedWriters();
}

int CacheManager::GetNumOfUser()
{
	return m_db.getNumberOfUsers();
}

