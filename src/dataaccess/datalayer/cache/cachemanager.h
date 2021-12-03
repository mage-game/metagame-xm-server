

#ifndef CACHEMANAGER_H
#define CACHEMANAGER_H

#include <string>
#include "fastdb.h"
USE_FASTDB_NAMESPACE

class CacheManager
{
public:
	~CacheManager();
	bool Init(const char* dbname, size_t init_size_MB);
	void Release();
	static CacheManager* Instance();

	/*
		该接口只允许用于内部Cache文件夹中
	*/
	dbDatabase *GetCache();

	size_t GetCacheSize();
	size_t GetCacheUsedSize();
	size_t GetCacheRealUsedSize();

	int GetReaderNum();
	int GetWriterNum();
	int GetBlockedReaderNum();
	int GetBlockedWriterNum();
	int GetNumOfUser();

private:
	CacheManager();

	static CacheManager *_instance;
	std::string m_dbname;
	dbDatabase m_db;
};

#endif

